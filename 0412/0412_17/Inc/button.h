/*
 * button.h
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void button_init(void);
void button_thread(void *);

#ifdef __cplusplus
}
#endif

#endif /* INC_BUTTON_H_ */
