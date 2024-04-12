/*
 * timer.c
 *
 *  Created on: Apr 12, 2024
 *      Author: IOT
 */
#include "tim.h"

extern TIM_HandleTypeDef htim3;

void tim_init(void)
{
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);

}
void tim_thread(void * arg)
{

}

void tim_duty_set_crr(uint16_t duty)
{
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,duty);
}

void tim_duty_set_prescale(uint32_t perscale)
{
	__HAL_TIM_SET_PRESCALER(&htim3,perscale);
}
void tim_duty_set_autoreload(uint32_t autoreload)
{
	__HAL_TIM_SET_AUTORELOAD(&htim3,autoreload);
}
