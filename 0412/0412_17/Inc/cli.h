/*
 * cli.h
 *
 *  Created on: Apr 12, 2024
 *      Author: iot00
 */

#ifndef INC_CLI_H_
#define INC_CLI_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void cli_init(void);
void cli_thread(void *);

#ifdef __cplusplus
}
#endif

#endif /* INC_CLI_H_ */
