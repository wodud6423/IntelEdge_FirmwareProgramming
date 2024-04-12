/*
 * pooling_thread.c
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */
#include <stdio.h>
#include <stdbool.h>
#include "adc.h"
#include "polling.h"
// ADC기능에 대한 콜백함수 선언부
static void adc_callback(void *arg);
static void adc_callback_2(void *arg);

// 스레드 구조를 정의하는 스레드 구조체
typedef struct {
	uint32_t period, count; // 폴링방식을 위한 주기와 카운터
	bool flag;
	void (*cbf)(void *);
} THR_T; // thread type

volatile THR_T gThrObjs[]  = {
	{ .period = 500, 	.cbf = adc_thread			},
	{ .period = 1500, 	.cbf = adc_callback_2		},
	{ .cbf = NULL			}
};

void polling_init(void)
{
	adc_init();
	adc_regcbf(0, adc_callback);
}

void polling_thread(void *arg)
{
	static uint16_t thr_idx = 0;

	if (gThrObjs[thr_idx].flag == true) {
		gThrObjs[thr_idx].flag = false;
		gThrObjs[thr_idx].cbf(NULL);
	}
	thr_idx++;
	if (gThrObjs[thr_idx].cbf == NULL) thr_idx = 0;
}

static void adc_callback(void *arg)
{
	printf("adc[0] value = %d\r\n", *(uint16_t *)arg);
}

static void adc_callback_2(void *arg)
{
	printf("adc[1] value = %d\r\n", *(uint16_t *)arg);
}



// io.c 파일 HAL_IncTick() 함수에서 호출
// HAL_IncTick() 함수는 systick irq handler에서 호출됨(인터럽트 서비스 루틴임)
// 1ms 마다 호출됨
void polling_update(void)
{
	for (int i=0; gThrObjs[i].cbf != NULL; i++) {
		gThrObjs[i].count++;
		gThrObjs[i].count %= gThrObjs[i].period;
		if (gThrObjs[i].count == 0) gThrObjs[i].flag = true;
	}
}
