/*
 * adc.h
 *
 *  Created on: Apr 9, 2024
 *      Author: iot00
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdbool.h>
#include "main.h"

typedef void (*ADC_CBF)(void *);

typedef struct {
	uint16_t value;
	ADC_CBF cbf;	// callback function
} ADC_T;

#ifdef __cplusplus
extern "C" {
#endif

void adc_init(void);
void adc_regcbf(uint16_t idx, ADC_CBF cbf);
void adc_thread(void *);

#ifdef __cplusplus
}
#endif

#endif /* INC_ADC_H_ */
