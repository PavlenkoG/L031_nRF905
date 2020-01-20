/*
 * spi.c
 *
 *  Created on: 14 Jan 2020
 *      Author: GRPA
 */

#include "main.h"
#include "spi.h"

SPI_HandleTypeDef spi_nrf;

void SPI_nRF905_Init (void) {
	__HAL_SPI_nRF_RCC_ENA;
	spi_nrf.Instance = SPI_NRF_PORT;
	spi_nrf.Init.Mode = SPI_MODE_MASTER;
    spi_nrf.Init.Direction = SPI_DIRECTION_2LINES;
    spi_nrf.Init.DataSize = SPI_DATASIZE_8BIT;
    spi_nrf.Init.CLKPolarity = SPI_POLARITY_LOW;
    spi_nrf.Init.CLKPhase = SPI_PHASE_1EDGE;
    spi_nrf.Init.NSS = SPI_NSS_SOFT;
#ifdef STM32F091xC
    spi_nrf.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
#endif
    spi_nrf.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    spi_nrf.Init.FirstBit = SPI_FIRSTBIT_MSB;
    spi_nrf.Init.TIMode = SPI_TIMODE_DISABLE;
    spi_nrf.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spi_nrf.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&spi_nrf) != HAL_OK) {
        printf("SPI Init error \r\n");
    }

}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi) {
    GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_SPI_nRF_GPIO_RCC_ENA;

    GPIO_InitStruct.Pin = SPI_SCK_PIN | SPI_MISO_PIN | SPI_MOSI_PIN;// | SPI_NSS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = SPIO_GPIO_AF_PORT;
    HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SPI_NSS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SPI_NSS_PORT, &GPIO_InitStruct);
    NRF905_Unselect();

}

void NRF905_Select() {
    HAL_GPIO_WritePin(SPI_NSS_PORT, SPI_NSS_PIN, GPIO_PIN_RESET);
}

void NRF905_Unselect() {
    HAL_GPIO_WritePin(SPI_NSS_PORT, SPI_NSS_PIN, GPIO_PIN_SET);
}
