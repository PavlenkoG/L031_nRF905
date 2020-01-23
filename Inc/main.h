/*
 * main.h
 *
 *  Created on: 22 Nov 2019
 *      Author: GRPA
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32l0xx_hal.h"
#include "stm32l031xx.h"

#include "platform_def.h"
#include "usart.h"
#include "spi.h"
#include "nRF905.h"
#include "console.h"

void processUserCommand (struct userCmd_t *userCmd, struct nRF905_dev *nRF905);
void nRF905_BurstRxHandler(struct nRF905_dev *nRF905);

#endif /* INC_MAIN_H_ */
