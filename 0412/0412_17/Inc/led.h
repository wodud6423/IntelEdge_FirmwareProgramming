/*
 * led.h
 *
 *  Created on: Apr 12, 2024
 *      Author: iot00
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include <stdbool.h>
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

bool led_onoff(uint8_t led_no, bool flag);

#ifdef __cplusplus
}
#endif

#endif /* INC_LED_H_ */
