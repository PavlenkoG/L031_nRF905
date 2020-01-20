/*
 * nRF905.c
 *
 *  Created on: 15 Jan 2020
 *      Author: GRPA
 */
#include "nRF905.h"

void nRF905_init (struct nRF905_dev *nRF905) {

	uint i = 0;
	nRF905_HwConfig();
	for (i = 0; i < (sizeof(nRF905->config)/sizeof(nRF905->config[0])); i++ ) {
		nRF905->config[i] = 0;
	}
	nRF905->config[0] = (uint8_t)nRF905->nRF905_HandleTypeDef.ChannelNum;

	nRF905->config[1] |= (uint8_t)(nRF905->nRF905_HandleTypeDef.ChannelNum >> 8);
	nRF905->config[1] |= (uint8_t)nRF905->nRF905_HandleTypeDef.nRF905_band;
	nRF905->config[1] |= (uint8_t)nRF905->nRF905_HandleTypeDef.nRF905_pwr;
	nRF905->config[1] |= (uint8_t)nRF905->nRF905_HandleTypeDef.nRF905_low_rx;
	nRF905->config[1] |= (uint8_t)nRF905->nRF905_HandleTypeDef.nRF905_auto_retran;

	nRF905->config[2] |= (uint8_t)nRF905->nRF905_HandleTypeDef.nRF905_rx_addr_size;
	nRF905->config[2] |= (uint8_t)(nRF905->nRF905_HandleTypeDef.nRF905_tx_addr_size << 4);

	nRF905->config[3] = (uint8_t)nRF905->nRF905_HandleTypeDef.nRF905_rx_pw;
	nRF905->config[4] = (uint8_t)nRF905->nRF905_HandleTypeDef.nRF905_tx_pw;

	for (i = 0; i < 4; i ++ ) {
		nRF905->config[5+i] = (uint8_t)(nRF905->nRF905_HandleTypeDef.nRF905_rx_addr >> (i * 8));
	}

	nRF905->config[9] |= (uint8_t)nRF905->nRF905_HandleTypeDef.nRF905_outclk;
	nRF905->config[9] |= (uint8_t)nRF905->nRF905_HandleTypeDef.nRF905_xof_freq;
	nRF905->config[9] |= (uint8_t)nRF905->nRF905_HandleTypeDef.nRF905_crc;

}

void nRF905_HwConfig(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	// data ready pin
    GPIO_InitStruct.Pin = nRF905_DR;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(nRF905_DR_PORT, &GPIO_InitStruct);

    // address match pin
    GPIO_InitStruct.Pin = nRF905_AM;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(nRF905_AM_PORT, &GPIO_InitStruct);

    // carrier detect pin
    GPIO_InitStruct.Pin = nRF905_CD;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(nRF905_CD_PORT, &GPIO_InitStruct);

    // power up chip pin
    GPIO_InitStruct.Pin = nRF905_PWR;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(nRF905_PWR_PORT, &GPIO_InitStruct);

    // enable chip for receive and transmit
    GPIO_InitStruct.Pin = nRF905_TRXCE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(nRF905_TRXCE_PORT, &GPIO_InitStruct);

    // tx/rx mode
    GPIO_InitStruct.Pin = nRF905_TXEN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(nRF905_TXEN_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2, 0);
}

void nRF905_writeConfig (struct nRF905_dev *nRF905) {

	uint8_t cmd = NRF905_CMD_W_CONFIG;
	nRF905->write (&cmd,nRF905->config,10);

}

void nRF905_setChannel(struct nRF905_dev *nRF905, nRF905_band_t band, uint16_t channel) {
	uint8_t cmdW = NRF905_CMD_W_CONFIG;
	uint8_t cmdR = NRF905_CMD_R_CONFIG | NRF905_REG_CHANNEL;
	uint8_t tempReg[2] = {0,0};
	nRF905->read (&cmdR,tempReg,2);

	tempReg[0] = (uint8_t) channel;
	tempReg[1] &= 0xFC;
	tempReg[1] |= (uint8_t)(channel >> 8);

	/*
	nRF905_getConfigRegisters(nRF905);
	nRF905->config[0] = (uint8_t) channel;
	nRF905->config[1] &= 0xFC;

	nRF905->config[1] |= (uint8_t)(channel >> 8);
	nRF905->config[1] |= (uint8_t)band;
	*/
	nRF905->write (&cmdW,tempReg,2);
}

void nRF905_powerUp(void) {
	HAL_GPIO_WritePin(nRF905_PWR_PORT, nRF905_PWR, GPIO_PIN_SET);
}

void nRF905_powerDown(void) {
	HAL_GPIO_WritePin(nRF905_PWR_PORT, nRF905_PWR, GPIO_PIN_RESET);
}

void nRF905_getConfigRegisters(struct nRF905_dev *nRF905) {
	uint8_t cmd = NRF905_CMD_R_CONFIG;
	nRF905->read (&cmd,nRF905->config,11);
}

void nRF905_startShockBurstRx (void) {
	nRF905_powerUp();
	HAL_GPIO_WritePin(nRF905_TXEN_PORT, nRF905_TXEN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(nRF905_TRXCE_PORT, nRF905_TRXCE, GPIO_PIN_SET);
//	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void nRF905_sendData (struct nRF905_dev *nRF905, uint8_t *pTxData, uint16_t Size) {

	HAL_GPIO_WritePin(nRF905_TXEN_PORT, nRF905_TXEN, GPIO_PIN_SET);
	nRF905_powerUp();
	HAL_GPIO_WritePin(nRF905_TRXCE_PORT, nRF905_TRXCE, GPIO_PIN_RESET);

	uint8_t cmd = NRF905_CMD_W_TX_PAYLOAD;
	nRF905->write (&cmd,pTxData,Size);
	HAL_GPIO_WritePin(nRF905_TRXCE_PORT, nRF905_TRXCE, GPIO_PIN_SET);

	while (HAL_GPIO_ReadPin(nRF905_DR_PORT,nRF905_DR) == GPIO_PIN_RESET) {

	}
	HAL_GPIO_WritePin(nRF905_TRXCE_PORT, nRF905_TRXCE, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(nRF905_TXEN_PORT, nRF905_TXEN, GPIO_PIN_RESET);
	nRF905_powerDown();

}

uint8_t nRF905_getData(struct nRF905_dev *nRF905, void* data, uint8_t len) {
	uint8_t cmd = NRF905_CMD_R_RX_PAYLOAD;
	nRF905->read (&cmd,nRF905->config,len);
	return 0;

}

