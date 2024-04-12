/*
 * app.h
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#include "main.h"
#include "uart.h"

#ifdef __cplusplus
extern "C" {
#endif

void cli_init(void);
void cli_thread(void * arg);


#ifdef __cplusplus
}
#endif

#endif /* INC_APP_H_ */
