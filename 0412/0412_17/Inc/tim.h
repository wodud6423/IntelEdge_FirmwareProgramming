/*
 * timer.h
 *
 *  Created on: Apr 12, 2024
 *      Author: IOT
 */

#ifndef INC_TIM_H_
#define INC_TIM_H_

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

void tim_init(void);
void tim_thread(void * arg);
void tim_duty_set_crr(uint16_t duty);
void tim_duty_set_prescale(uint32_t perscale);
void tim_duty_set_autoreload(uint32_t autoreload);

#ifdef __cplusplus
}
#endif


#endif /* INC_TIM_H_ */
