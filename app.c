/*
 * app.c
 *
 *  Created on: Apr 11, 2024
 *      Author: IOT
 */

#include <stdio.h>
#include "app.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//printf(".");
	__IO uint32_t prev;
	prev = HAL_GetTick();
	while(1)
		if ((HAL_GetTick())- prev >= 100) {
			HAL_GPIO_TogglePin(LD1_GPIO_Port,LD1_Pin);
			break;
		}
}

void app(void)
{
	printf("System Started.... \r\n");

	while(1)
	{

	}

}
