/*
 * app.c
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */
#include <stdio.h>
#include "io.h"
#include "polling.h"
#include "button.h"
#include "uart.h"
#include "app.h"

void app_init(void);

void app(void)
{
	printf("System Started.....!\r\n");

	app_init();

	while (1) {
		polling_thread(NULL);
		button_thread(NULL);
		uart_thread(NULL);
	}
}

void app_init(void)
{
	io_exti_init();
	polling_init();
	button_init();
	uart_init();
}

