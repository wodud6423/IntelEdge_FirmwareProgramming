/*
 * uart.c
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */
#include <stdbool.h>
#include <stdio.h>
#include "uart.h"
#include "cli.h"
extern UART_HandleTypeDef huart3;

static uint8_t rxd;
//static bool rxd_flag = false;

#define D_BUF_MAX	100	// uart Data String Buffer
#define D_BUF_OBJ_MAX	3 // uart

typedef struct {
  uint8_t buf[D_BUF_MAX+1]; // +1 is For Last NULL of String Type!
  uint8_t idx; //
  uint8_t flag;
} BUF_T;

static BUF_T gBufObjs[3];

static void (*uart_cbf)(void*);



void uart_init(void)
{
	for(int i =0;i<D_BUF_OBJ_MAX;i++){
		gBufObjs[i].idx = 0;
		gBufObjs[i].flag = false;
	}
	HAL_UART_Receive_IT(&huart3, (uint8_t *)&rxd, 1);

}
void uart_regcbf(void (*cbf)(void*))
{
	uart_cbf =cbf;

}
void uart_thread(void *arg)
{
	for(int i =0;i<D_BUF_OBJ_MAX;i++){
		if(gBufObjs[i].flag == true)
		{
			if(uart_cbf != NULL) uart_cbf((void*)&gBufObjs[i]);
			gBufObjs[i].idx = 0;
			gBufObjs[i].flag = false;
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	volatile uint8_t rxd_origin;

	if(huart == &huart3){
		rxd_origin =rxd;
		HAL_UART_Receive_IT(huart, (uint8_t *)&rxd, 1);

		BUF_T *p = (BUF_T *)&gBufObjs[0];

		if(p->flag == false){
			p->buf[p->idx] = rxd;
			// Overwrite func
			p->idx++;
			p->idx %= D_BUF_MAX;

			// Not Overwirte Func
			if(p->idx < D_BUF_MAX)p->idx++;

			// Full Buffer So Can Sending
			if(rxd == "\r"||rxd == "\n"){
				p->buf[p->idx] = 0;// "\0 = 0"
				p->flag = true;
			}
		}

	}
	HAL_UART_Receive_IT(&huart3, (uint8_t *)&rxd, 1);
}
