/*
 * led.c
 *
 *  Created on: Apr 12, 2024
 *      Author: iot00
 */

#include "led.h"

#define LED_MAX			3

typedef struct {
	GPIO_TypeDef *port;
	uint16_t 			pin;
} LED_T;

const LED_T gLedObjs[LED_MAX] = {
	{	LD2_GPIO_Port, 	LD2_Pin		},
	{	LD2_GPIO_Port, 	LD2_Pin		},
	{	LD3_GPIO_Port,		LD3_Pin		}
};

bool led_onoff(uint8_t led_no, bool flag)
{
	LED_T *p;
	GPIO_PinState sts;

	if (led_no > LED_MAX) return false;

	p = (LED_T *)&gLedObjs[led_no];
	sts = flag ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(p->port, p->pin, sts);

	return true;
}

