/*
 * pooling_thread.h
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */

#ifndef INC_POLLING_H_
#define INC_POLLING_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void polling_init(void);
void polling_thread(void *);
void polling_update(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_POLLING_H_ */
