/*
 * cli.c
 *
 *  Created on: Apr 12, 2024
 *      Author: IOT
 */
#include <string.h>
#include <stdio.h>
#include "cli.h"
#include "uart.h"

typedef struct{
	char *cmd; // Command Input Type
	uint8_t no; // Minimum Command Input Number
	int (*cbf)(int,char**); // int argc,char* argv[]
	char *remark // Information
}CMD_LIST_T;

static int cli_led(int argc,char*argv[]);

static int cli_echo(int argc,char*argv[]);

static int cli_help(int argc,char*argv[]);

const CMD_LIST_T gCmdListObjs[] = {
		{"led",  3,  cli_led ,"led [1/2/3] [on/off]"},
		{"echo", 2,  cli_echo,"echo [echo_data]"},
		{"help", 1,  cli_help,"help"},
		{NULL	,0,	 NULL    ,NULL}

};
static void cli_parser(void* arg);
// argc and argv Copying!

static int cli_led(int argc,char*argv[])
{

	if(argc < 3){
		printf("Err : Argment No\r\n");
		return -1;
	}

	long no = strtol(argv[1],NULL,10); // Input [10]
	int onoff = strcmp(argv[2],"on");

//	if(onoff)
}

static int cli_help(int argc,char*argv[]){
	for(int i =0 ; gCmdListObjs[i].cmd != NULL;i++)
	{
		printf("%s\r\n",gCmdListObjs[i].remark);
	}

}

static int cli_echo(int argc,char*argv[]){
	if(argc < 2 )
	{
	printf("Err : Argment No\r\n");
	return -1;
	}

	printf("%s\r\n",argv[1]);

	return 0;
}

void cli_init(void)
{
	uart_regcbf(cli_parser);
}

void cli_thread(void * arg)
{
	(void)arg;

}

#define D_DELIMITER		" ,\r\n"
void cli_parser(void* arg)
{
	int argc = 0;
	char* argv[10];
	char* ptr;
	char* buf = (char *)arg;

	ptr = strtok(buf,D_DELIMITER);
	if (ptr == NULL)return;

	while(ptr !=NULL)
	{
		argv[argc] = ptr;
		argc++;
		ptr = strtok(NULL,D_DELIMITER);
	}
//	for(int i = 0;i<argc;i++){
//		printf("%d : %s \r\n",i,argv[i]);
//	}
	return;
}
