/*
 * usart.c
 *
 *  Created on: 14 Jan 2020
 *      Author: GRPA
 */

#include "main.h"

USART_HandleTypeDef usart2;
void DEBUG_USART_Init (void) {

	__HAL_UART_DBG_RCC_ENA;
	usart2.Instance = USART2;
	usart2.Init.BaudRate = 115200;
	usart2.Init.WordLength = 8;
	usart2.Init.StopBits = USART_STOPBITS_1;
	usart2.Init.Mode = USART_MODE_TX_RX;
	usart2.Init.Parity = USART_PARITY_NONE;

	HAL_USART_Init(&usart2);
}

void HAL_USART_MspInit(USART_HandleTypeDef *husart) {

	GPIO_InitTypeDef GPIO_InitStruct;
	if (husart->Instance == USART2) {
		__HAL_UART_DBG_GPIO_RCC_ENA;
		GPIO_InitStruct.Pin = UART_DBG_RX_PIN | UART_DBG_TX_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = UART_GPIO_AF_PORT;

		HAL_GPIO_Init(UART_DBG_PORT, &GPIO_InitStruct);
	}
}
