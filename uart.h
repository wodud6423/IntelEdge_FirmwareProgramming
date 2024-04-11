/*
 * uart.h
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */

#ifndef SRC_UART_H_
#define SRC_UART_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void uart_init(void);
void uart_thread(void *);

#ifdef __cplusplus
}
#endif

#endif /* SRC_UART_H_ */
