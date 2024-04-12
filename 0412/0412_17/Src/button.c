/*
 * button.c
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */
#include <stdbool.h>
#include <stdio.h>
#include "io.h"
#include "button.h"

#define D_USER_BTN_EXTI_NO		13

static bool flag;
static uint8_t edge_rising_or_falling;
static uint16_t button_no;
static uint32_t prev_tick;

static void button_callback_13(uint8_t rf, void *arg);

void button_init(void)
{
	prev_tick = HAL_GetTick();
	io_exti_regcbf(D_USER_BTN_EXTI_NO, button_callback_13);
}

void button_thread(void *arg)
{
	if (flag == true) {
		flag = false;
		if (button_no == D_USER_BTN_EXTI_NO) {
			printf("rf:%d, no:%d\r\n", edge_rising_or_falling,  button_no);
		}
	}
}

static void button_callback_13(uint8_t rf, void *arg)
{
	volatile uint32_t curr_tick = HAL_GetTick();

	if (curr_tick - prev_tick > 120) {
		prev_tick = curr_tick;
		edge_rising_or_falling = rf;
		button_no = *(uint16_t *)arg;
		flag = true;
	}
}
