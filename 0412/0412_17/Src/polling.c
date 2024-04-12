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
	bool flag; // 단일 코어에서 동시 작업으로 인한 에러를 방지하기위한...bool형 변수!
	void (*cbf)(void *); // 콜백 함수(함수인자로 반드시 포인터형식(위치형식)을 받아야한다!!
} THR_T; // thread type

// 여기서 포인터에 대한 개념을 다시 익혀두고 가자..
// 변수 포인터-> 그 변수의 시작주소
// EX: int a[3] = {1,2,3}; &a : a가 저장된 주소값!
// *a: 1(시작주소의 int형값)(*a=a[0])
// &a: 1의 시작주소값
// (우리는 STM32이니까 32BIT의 uint32_t형태로
// 출력될 것이다!)
// 변수인자 int*a있다면?->변수의 시작주소(변수의 위치)를 넣어달라는거야!

// 함수 포인터-> 그 함수의 이름위치
// 즉, 변수에서의 위치가 &a 였다면 함수 void a(void)의 위치는 a인 것이다!!
// 그럼 콜백함수의 인자 *cbf는 몰까? => 함수의 이름(함수의 위치)을 넣어달라는거야!

// volatile을 쓴 이유?-> 캐시에 올라가서 안돼!!
// 캐시는 메모리상이 아닌 캐시에 올려놓고 cashe - cpu // 에서만 데이터 교환이 일어난다. 근데 얘는 폴링방식의 주기가 담긴 값이 바뀌어서는 안되는 구조체이다!

volatile THR_T gThrObjs[]  = {
	{ .period = 500, 	.cbf = adc_thread			},
	{ .period = 1500, 	.cbf = adc_callback_2		},
	{ .cbf = NULL			}
};

// 폴링방식 -> 
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
