/*
 * io.c
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */

#include <stdbool.h>
#include "polling.h"
#include "io.h"

extern UART_HandleTypeDef huart3;

int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xffff);
	return ch;
}

void HAL_IncTick(void)
{
  uwTick += uwTickFreq;

  polling_update();
}

//-----------------------------------------------

#define D_IO_EXTI_MAX		16
static IO_EXTI_T gIOExtiObjs[D_IO_EXTI_MAX];

static void io_exti_dummy(uint8_t rf, void *arg);

void io_exti_init(void)
{
	for (int i=0; i<D_IO_EXTI_MAX; i++) {
		gIOExtiObjs[i].port = NULL;
		gIOExtiObjs[i].pin = 0;
		gIOExtiObjs[i].cbf = io_exti_dummy;
	}

	gIOExtiObjs[13].port = USER_Btn_GPIO_Port;
	gIOExtiObjs[13].pin = USER_Btn_Pin;
}

bool io_exti_regcbf(uint8_t idx, IO_CBF_T cbf)
{
	if (idx > D_IO_EXTI_MAX) return false;
	gIOExtiObjs[idx].cbf = cbf;
	return true;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	for (volatile uint16_t i=0; i<D_IO_EXTI_MAX; i++) {
		volatile GPIO_PinState sts = HAL_GPIO_ReadPin(gIOExtiObjs[i].port, gIOExtiObjs[i].pin);
		if (GPIO_Pin & (0x01 << i)) 	gIOExtiObjs[i].cbf((uint8_t)sts, (void *)&i);
	}
}

static void io_exti_dummy(uint8_t rf, void *arg)
{
	(void)rf;
	(void)arg;
}

