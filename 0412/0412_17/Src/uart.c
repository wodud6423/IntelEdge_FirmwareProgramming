/*
 * uart.c
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */
#include <stdbool.h>
#include <stdio.h>
#include "uart.h"

extern UART_HandleTypeDef huart3;

static uint8_t rxdata[1];
//static bool rxd_flag = false;

#define D_BUF_OBJ_MAX	3
#define D_BUF_MAX			100
typedef struct {
  uint8_t buf[D_BUF_MAX+1]; // + '\0'
  uint8_t idx;	// count
  uint8_t flag;	// uart 를 통해 '\r' 이나 '\n' 받았을 경우
} BUF_T;

static BUF_T gBufObjs[D_BUF_OBJ_MAX];

static void (*uart_cbf)(void *);

void uart_init(void)
{
	for (int i=0; i<D_BUF_OBJ_MAX; i++) {
		gBufObjs[i].idx = 0;
		gBufObjs[i].flag = false;
	}

	// 인터럽트 방식 수신 시작 : 1바이트
	HAL_UART_Receive_IT(&huart3, (uint8_t *)&rxdata[0], 1);
}

void uart_regcbf(void (*cbf)(void *))
{
	uart_cbf = cbf;
}

void uart_thread(void *arg)
{
	for (int i=0; i<D_BUF_OBJ_MAX; i++) {
		if (gBufObjs[i].flag == true) {
			if (uart_cbf != NULL) uart_cbf((void *)&gBufObjs[i]);
			gBufObjs[i].idx = 0;
			gBufObjs[i].flag = false;
		}
	}
}

// 인터럽트 서비스 루틴 (ISR)
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	volatile uint8_t rxd;

	if (huart == &huart3) {
		rxd = rxdata[0];
		HAL_UART_Receive_IT(huart, (uint8_t *)&rxdata[0], 1);

		BUF_T *p = (BUF_T *)&gBufObjs[0];
		if (p->flag == false) {
			p->buf[p->idx] = rxd;
			//p->idx++;
			//p->idx %= D_BUF_MAX;
			if (p->idx < D_BUF_MAX) p->idx++;

			if (rxd == '\r' || rxd == '\n') {
				p->buf[p->idx] = 0; //'\0';
				p->flag = true;
			}
		}
	}
}
