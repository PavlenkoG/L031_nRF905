/*
 * usart.h
 *
 *  Created on: 14 Jan 2020
 *      Author: GRPA
 */

#ifndef USART_H_
#define USART_H_

#include "main.h"

#define UART_RX_BUFFER_LEN 30

extern UART_HandleTypeDef usart2;
extern uint8_t stringReceived;

void DEBUG_USART_Init (void);


#endif /* USART_H_ */
