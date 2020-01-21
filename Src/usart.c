/*
 * usart.c
 *
 *  Created on: 14 Jan 2020
 *      Author: GRPA
 */

#include "main.h"

UART_HandleTypeDef usart2;

uint8_t uartRx[UART_RX_BUFFER_LEN];
uint8_t stringReceived = 0;
void DEBUG_USART_Init (void) {

	__HAL_UART_DBG_RCC_ENA;
	usart2.Instance = USART2;
	usart2.Init.BaudRate = 115200;
	usart2.Init.WordLength = UART_WORDLENGTH_8B;
	usart2.Init.StopBits = UART_STOPBITS_1;
	usart2.Init.Mode = UART_MODE_TX_RX;
	usart2.Init.Parity = UART_PARITY_NONE;

    usart2.pRxBuffPtr = &uartRx[0];
    usart2.RxXferSize = UART_RX_BUFFER_LEN;
    usart2.RxXferCount = 0;
//  usart2.RxCpltCallback = HAL_USART_RxCpltCallback;

	HAL_UART_Init(&usart2);
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void HAL_UART_MspInit(UART_HandleTypeDef *husart) {

	GPIO_InitTypeDef GPIO_InitStruct;
	if (husart->Instance == USART2) {
		__HAL_UART_DBG_GPIO_RCC_ENA;
		GPIO_InitStruct.Pin = UART_DBG_RX_PIN | UART_DBG_TX_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = UART_GPIO_AF_PORT;

		HAL_GPIO_Init(UART_DBG_PORT, &GPIO_InitStruct);
	}
}
