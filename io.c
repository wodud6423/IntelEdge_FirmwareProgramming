/*
 * io.c
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */


// EXTI : external interrupt > 외부에서 온 interrupt

#include <stdbool.h>
#include "polling.h"
#include "io.h"

extern UART_HandleTypeDef huart3;

int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xffff);
	return ch;
}

// HAL단의 틱을 갱신하는 함수를 재정의
void HAL_IncTick(void)
{
  uwTick += uwTickFreq;

  // 1ms마다 polling layer에 포함된 thread의 count 값을 올이기 위한 호출
  polling_update();
}

//-----------------------------------------------

// EXTI는 0~15까지 있고 총 16개임
#define D_IO_EXTI_MAX		16
static IO_EXTI_T gIOExtiObjs[D_IO_EXTI_MAX];

static void io_exti_dummy(uint8_t rf, void *arg);

void io_exti_init(void)
{
	// 배열 전체 임시 초기화
	for (int i=0; i<D_IO_EXTI_MAX; i++) {
		gIOExtiObjs[i].port = NULL;
		gIOExtiObjs[i].pin = 0;
		gIOExtiObjs[i].cbf = io_exti_dummy;
	}

	// 우리가 사용할 유저버튼인 13번만 초기화
	gIOExtiObjs[13].port = USER_Btn_GPIO_Port;
	gIOExtiObjs[13].pin = USER_Btn_Pin;
}

// 콜백함수를 변경하기 위한 함수 선언
bool io_exti_regcbf(uint8_t idx, IO_CBF_T cbf)
{
	if (idx > D_IO_EXTI_MAX) return false;
	gIOExtiObjs[idx].cbf = cbf;
	return true;
}

// 외부에서 온 interrupt가 오면 처리하는 콜백 함수
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// 전체 16개 중에 몇번째 interrupt인지 확인하기 위한 for문
	for (volatile uint16_t i=0; i<D_IO_EXTI_MAX; i++) {
		// 저장되어 있는 포트와 핀의 정보를 받아옴
		volatile GPIO_PinState sts = HAL_GPIO_ReadPin(gIOExtiObjs[i].port, gIOExtiObjs[i].pin);
		// 해당 핀이 지금 들어온 핀가 맞다면,
		// 그 위치의 콜백 펑션을 호출
		if (GPIO_Pin & (0x01 << i)) 	gIOExtiObjs[i].cbf((uint8_t)sts, (void *)&i);
	}
}

// 콜백 펑션이 없어 오류나는거 방지용 더미
static void io_exti_dummy(uint8_t rf, void *arg)
{
	(void)rf;
	(void)arg;
}
