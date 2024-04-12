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
#include "cli.h"
#include "app.h"

void app_init(void);
void app_mode(int mode);
static void app_normal(void);
static void app_diagnostic(void);

static void (*mode_func)(void);


void app(void)
{
	printf("System Started.....!\r\n");

	mode_func = app_normal;

	app_init();

	while (1) {
		mode_func();
	}
}

void app_init(void)
{
	io_exti_init();
	polling_init();
	button_init();
	uart_init();
	cli_init();
}

void app_normal(void)
{
	polling_thread(NULL);
	button_thread(NULL);
	uart_thread(NULL);
	cli_thread(NULL);
}

void app_diagnostic(void)
{
	uart_thread(NULL);
	cli_thread(NULL);
}

void app_mode(int mode)
{
	if (mode == 0) {
		printf("Mode : Normal \r\n");
		mode_func = app_normal;
	} else {
		printf("Mode : Diagnostic \r\n");
		mode_func = app_diagnostic;
	}
}


