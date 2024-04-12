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
#include "tim.h"

//void app_init(void);
//void app_mode(int mode);

// static : 내부용 함수
static void app_normal(void);
static void app_diagnostic(void);

// static : 내부 변수
static void (*mode_func)(void);  // 함수 포인터 변수

void app(void)
{
	printf("System Started.....!\r\n");

	app_mode(1); // diagnostic mode is default.

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
	tim_init();
	cli_init();
}

static void app_normal(void)
{
	polling_thread(NULL);
	button_thread(NULL);
	uart_thread(NULL);
	tim_thread(NULL);
	cli_thread(NULL);
}

static void app_diagnostic(void)
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


