#include "stm32l0xx_hal.h"

// USART2 DEBUG
#define UART_DBG_RX_PIN     GPIO_PIN_15
#define UART_DBG_TX_PIN     GPIO_PIN_2
#define UART_DBG_PORT       GPIOA
#define UART_GPIO_AF_PORT   GPIO_AF4_USART2

#define __HAL_UART_DBG_RCC_ENA __HAL_RCC_USART2_CLK_ENABLE()
#define __HAL_UART_DBG_GPIO_RCC_ENA __HAL_RCC_GPIOA_CLK_ENABLE()

// SPI2 nRF905
#define SPI_NRF_PORT		SPI1
#define SPI_NSS_PIN			GPIO_PIN_11
#define SPI_MOSI_PIN		GPIO_PIN_5
#define SPI_MISO_PIN		GPIO_PIN_4
#define SPI_SCK_PIN			GPIO_PIN_3
#define SPI_PORT			GPIOB
#define SPI_NSS_PORT		GPIOA
#define SPIO_GPIO_AF_PORT	GPIO_AF0_SPI1

#define __HAL_SPI_nRF_RCC_ENA __HAL_RCC_SPI1_CLK_ENABLE()
#define __HAL_SPI_nRF_GPIO_RCC_ENA __HAL_RCC_GPIOB_CLK_ENABLE()

// nRF905 Pins

#define nRF905_DR			GPIO_PIN_6	// Data Ready signal
#define nRF905_DR_PORT		GPIOA
#define nRF905_AM			GPIO_PIN_5	// Address Match signal
#define nRF905_AM_PORT		GPIOA
#define nRF905_CD			GPIO_PIN_4 	// Carrier detect
#define nRF905_CD_PORT		GPIOA
#define nRF905_PWR			GPIO_PIN_0  // Power Up chip
#define nRF905_PWR_PORT		GPIOA
#define nRF905_TRXCE		GPIO_PIN_3	// Enable chip for receive and transmit
#define nRF905_TRXCE_PORT	GPIOA
#define nRF905_TXEN			GPIO_PIN_1  // TX/RX mode
#define nRF905_TXEN_PORT	GPIOA

