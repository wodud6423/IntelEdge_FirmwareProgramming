/*
 * adc.c
 *
 *  Created on: Apr 9, 2024
 *      Author: iot00
 */

#include <stdio.h>
#include "adc.h"

static void adc_dummy(void *);

static ADC_T gAdcObjs[] = {
	{ .cbf = adc_dummy 	},
	{ .cbf = NULL			}
};

void adc_init(void)
{
}

void adc_regcbf(uint16_t idx, ADC_CBF cbf)
{
	gAdcObjs[idx].cbf = cbf;
}

void adc_thread(void *arg)
{
	(void)arg;

	static uint16_t value = 100;

	for (int i=0; gAdcObjs[i].cbf != NULL; i++) {
		value+=100;
		gAdcObjs[i].value = value;
		gAdcObjs[i].cbf((void *)&gAdcObjs[i].value);
	}
}

//--------------
// adc_hw()

static void adc_dummy(void *)
{
	printf("I'm ADC dummy\r\n");
	return;
}
