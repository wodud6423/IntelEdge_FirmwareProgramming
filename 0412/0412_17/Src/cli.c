/*
 * cli.c
 *
 *  Created on: Apr 12, 2024
 *      Author: iot00
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "uart.h"
#include "app.h"
#include "led.h"
#include "cli.h"
#include "tim.h"



typedef struct {
	char *cmd;	// 명령어
	uint8_t no;	// 인자 최소 갯수
	int (*cbf)(int, char **);  // int argc, char *argv[]
	char *remark; // 설명
} CMD_LIST_T;

static int cli_led(int argc,  char **argv);
static int cli_echo(int argc, char *argv[]);
static int cli_help(int argc, char *argv[]);
static int cli_mode(int argc, char *argv[]);
static int cli_dump(int argc, char *argv[]);
static int cli_pwm(int argc, char *argv[]);

static bool cli_pwm_by_crr(uint16_t num);


const CMD_LIST_T gCmdListObjs[] = {
	{ "duty",	2,		cli_pwm,		"duty [mode:decimal] [duty_rate:decimal]"	},
	{ "dump",	3,		cli_dump,		"memory dump\r\n dump [address:hex] [length:max:10 lines]"	},
	{ "mode",	2,		cli_mode,		"change application mode\r\n mode [0/1]"	},
	{ "led",	3,		cli_led,		"led [1/2/3] [on/off]"	},
	{ "echo",	2,		cli_echo,		"echo [echo data]"	},
	{ "help", 	1, 		cli_help, 		"help" 					},
	{ NULL,		0,		NULL,			NULL						}
};

static int cli_pwm(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Err : Arg No\r\n");
		return -1;
	}

	long no = strtol(argv[2], NULL, 10);
	long mode = strtol(argv[1], NULL, 10);
	switch(mode){
		case 1 :
			printf("pwm led by crr %ld, %d\r\n", no);
			tim_duty_set_crr((uint16_t) no);
		case 2 :
			printf("pwm led by prescale %ld, %d\r\n", no);
			tim_duty_set_prescale((uint32_t) no);
		case 3 :
			printf("pwm led by prescale %ld, %d\r\n", no);
			tim_duty_set_autoreload((uint32_t) no);
	}
	return 0;
}



static int cli_dump(int argc, char *argv[])
{
	uint32_t address, length, temp;

	if (argc < 3) {
		printf("Err : Arg No\r\n");
		return -1;
	}

	if (strncmp(argv[1], "0x", 2) == 0) address = (uint32_t)strtol(&argv[1][2], NULL, 16);
	else address = (uint32_t)strtol(&argv[1][0], NULL, 16);

	length = (uint32_t)strtol(argv[2], NULL, 10);
	if (length > 10) length = 10;

	printf("address  %08lX, length = %ld\r\n", address, length);

	for (int i=0; i<length; i++) {
		printf("\r\n%08lX : ", (uint32_t)address);

		temp=address;
		for (int j=0; j<16; j++) {
			printf("%02X ", *(uint8_t *)temp);
			temp++;
		}

		temp=address;
		for (int j=0; j<16; j++) {
			char c = *(char *)temp;
			c = isalnum(c) ? c : (char)' ';
			printf("%c", c);
			temp++;
		}

		address = temp;
	}
	printf("\r\n");

	return 0;
}

static int cli_mode(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Err : Arg No\r\n");
		return -1;
	}

	long mode = strtol(argv[1], NULL, 10);
	app_mode((int)mode);

	return 0;
}

static int cli_led(int argc, char *argv[])
{
	if (argc < 3) {
		printf("Err : Arg No\r\n");
		return -1;
	}

	long no = strtol(argv[1], NULL, 10);
	int onoff = strcmp(argv[2], "off");

	if (onoff != 0) onoff = 1;
	bool sts = onoff ? true : false;

	//printf("led %ld, %d\r\n", no, onoff);
	 led_onoff((uint8_t)no, sts);

	return 0;
}

static int cli_echo(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Err : Arg No\r\n");
		return -1;
	}
	printf("%s\r\n", argv[1]);

	return 0;
}

static int cli_help(int argc, char *argv[])
{
	for (int i=0; gCmdListObjs[i].cmd != NULL; i++) {
		printf("%s\r\n", gCmdListObjs[i].remark);
	}

	return 0;
}

static void cli_parser(void *arg);

void cli_init(void)
{
	uart_regcbf(cli_parser);
}

void cli_thread(void *arg)
{
	(void)arg;
}

#define D_DELIMITER		" ,\r\n"

static void cli_parser(void *arg)
{
	int argc = 0;
	char *argv[10];
	char *ptr;

	char *buf = (char *)arg;

	//printf("rx:%s\r\n", (char *)arg);
	// token 분리
	ptr = strtok(buf, D_DELIMITER);
	if (ptr == NULL) return;

	while (ptr != NULL) {
		argv[argc] = ptr;
		argc++;
		ptr = strtok(NULL, D_DELIMITER);
	}

//	for (int i=0; i<argc; i++) {
//		printf("%d:%s\r\n", i, argv[i]);
//	}

	for (int i=0; gCmdListObjs[i].cmd != NULL; i++) {
		if (strcmp(gCmdListObjs[i].cmd, argv[0]) == 0) {
			gCmdListObjs[i].cbf(argc, argv);
			return;
		}
	}

	printf("Unsupported Command\r\n");
}
