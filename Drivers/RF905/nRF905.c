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
	uint8_t cmdW = NRF905_CMD_W_CONFIG | NRF905_REG_CHANNEL;
	uint8_t cmdR = NRF905_CMD_R_CONFIG | NRF905_REG_CHANNEL;
	uint8_t tempReg[2] = {0,0};
	nRF905->read (&cmdR,tempReg,2);

	tempReg[0] = (uint8_t) channel;
	tempReg[1] &= NRF905_CHANNEL_MSK;
	tempReg[1] |= (uint8_t)(channel >> 8);
	tempReg[1] |= (uint8_t)band;

	nRF905->write (&cmdW,tempReg,2);
}

void nRF905_setAutoRetransmit(struct nRF905_dev *nRF905, nRF905_auto_retran_t nRF905_auto_retran) {
	uint8_t cmdW = NRF905_CMD_W_CONFIG | NRF905_REG_AUTO_RETRAN;
	uint8_t cmdR = NRF905_CMD_R_CONFIG | NRF905_REG_AUTO_RETRAN;
	uint8_t tempReg = 0;

	nRF905->read (&cmdR,&tempReg,1);

	tempReg &= NRF905_AUTO_RETRAN_MSK;
	tempReg |= nRF905_auto_retran;

	nRF905->write (&cmdW,&tempReg,1);
}

void nRF905_setLowRxPower(struct nRF905_dev *nRF905, nRF905_low_rx_t nRF905_low_rx) {
	uint8_t cmdW = NRF905_CMD_W_CONFIG | NRF905_REG_PWR;
	uint8_t cmdR = NRF905_CMD_R_CONFIG | NRF905_REG_PWR;
	uint8_t tempReg = 0;

	nRF905->read (&cmdR,&tempReg,1);

	tempReg &= NRF905_LOW_RX_MSK;
	tempReg |= nRF905_low_rx;

	nRF905->write (&cmdW,&tempReg,1);
}

void nRF905_setTransmitPower(struct nRF905_dev *nRF905, nRF905_pwr_t nRF905_pwr) {
	uint8_t cmdW = NRF905_CMD_W_CONFIG | NRF905_REG_PWR;
	uint8_t cmdR = NRF905_CMD_R_CONFIG | NRF905_REG_PWR;
	uint8_t tempReg = 0;

	nRF905->read (&cmdR,&tempReg,1);

	tempReg &= NRF905_PA_PWR_MSK;
	tempReg |= nRF905_pwr;

	nRF905->write (&cmdW,&tempReg,1);
}

void nRF905_setRxPayloadSize(struct nRF905_dev *nRF905, uint8_t size) {
	uint8_t cmdW = NRF905_CMD_W_CONFIG | NRF905_REG_RX_PAYLOAD_SIZE;

	if (size > 0 && size <= 32)
		nRF905->write (&cmdW,&size,1);
}

void nRF905_setTxPayloadSize(struct nRF905_dev *nRF905, uint8_t size) {
	uint8_t cmdW = NRF905_CMD_W_CONFIG | NRF905_REG_TX_PAYLOAD_SIZE;

	if (size > 0 && size <= 32)
		nRF905->write (&cmdW,&size,1);
}

void nRF905_setRxAddressSize(struct nRF905_dev *nRF905, nRF905_addr_size_t nRF905_addr_size) {
	uint8_t cmdW = NRF905_CMD_W_CONFIG | NRF905_REG_ADDR_WIDTH;
	uint8_t cmdR = NRF905_CMD_R_CONFIG | NRF905_REG_ADDR_WIDTH;
	uint8_t tempReg = 0;

	nRF905->read (&cmdR,&tempReg,1);

	tempReg &= NRF905_RX_ADDR_WDT_MSK;
	tempReg |= nRF905_addr_size;

	nRF905->write (&cmdW,&tempReg,1);
}
void nRF905_setTxAddressSize(struct nRF905_dev *nRF905, nRF905_addr_size_t nRF905_addr_size) {
	uint8_t cmdW = NRF905_CMD_W_CONFIG | NRF905_REG_ADDR_WIDTH;
	uint8_t cmdR = NRF905_CMD_R_CONFIG | NRF905_REG_ADDR_WIDTH;
	uint8_t tempReg = 0;

	nRF905->read (&cmdR,&tempReg,1);

	tempReg &= NRF905_TX_ADDR_WDT_MSK;
	tempReg |= (nRF905_addr_size << 4);

	nRF905->write (&cmdW,&tempReg,1);
}

void nRF905_setTXAddress(struct nRF905_dev *nRF905, uint32_t *address) {
	uint8_t cmdW = NRF905_CMD_W_TX_ADDRESS;
	uint32_t swappedAddr = swapAddr(*address);

	nRF905->write (&cmdW,&swappedAddr,4);
}
void nRF905_setRXAddress(struct nRF905_dev *nRF905, uint32_t *address) {
	uint8_t cmdW = NRF905_CMD_W_CONFIG | NRF905_REG_RX_ADDRESS;
	uint32_t swappedAddr = swapAddr(*address);

	nRF905->write (&cmdW,&swappedAddr,4);
}

uint32_t nRF905_getTXAddress(struct nRF905_dev *nRF905){
	uint32_t txAddr = 0;
	uint8_t cmdW = NRF905_CMD_R_TX_ADDRESS;
	nRF905->read (&cmdW,&txAddr,4);
	return swapAddr(txAddr);
}

void nRF905_setCRC(struct nRF905_dev *nRF905, nRF905_crc_t nRF905_crc) {
	uint8_t cmdW = NRF905_CMD_W_CONFIG | NRF905_REG_CRC;
	uint8_t cmdR = NRF905_CMD_R_CONFIG | NRF905_REG_CRC;
	uint8_t tempReg = 0;

	nRF905->read (&cmdR,&tempReg,1);

	tempReg &= NRF905_CRC_MSK;
	tempReg |= nRF905_crc;

	nRF905->write (&cmdW,&tempReg,1);
}

void nRF905_setCrstallOsc(struct nRF905_dev *nRF905, nRF905_xof_freq_t nRF905_xof_freq) {
	uint8_t cmdW = NRF905_CMD_W_CONFIG | NRF905_REG_CLK;
	uint8_t cmdR = NRF905_CMD_R_CONFIG | NRF905_REG_CLK;
	uint8_t tempReg = 0;

	nRF905->read (&cmdR,&tempReg,1);

	tempReg &= NRF905_XOF_MSK;
	tempReg |= nRF905_xof_freq;

	nRF905->write (&cmdW,&tempReg,1);
}

uint32_t swapAddr (uint32_t addr) {
	return ((addr>>24)&0xff) |
						   ((addr<<8)&0xff0000) |
	                       ((addr>>8)&0xff00) |
	                       ((addr<<24)&0xff000000);

}

void nRF905_powerUp(void) {
	HAL_GPIO_WritePin(nRF905_PWR_PORT, nRF905_PWR, GPIO_PIN_SET);
}

void nRF905_powerDown(void) {
	HAL_GPIO_WritePin(nRF905_PWR_PORT, nRF905_PWR, GPIO_PIN_RESET);
}

void nRF905_getConfigRegisters(struct nRF905_dev *nRF905) {
	uint8_t cmd = NRF905_CMD_R_CONFIG;
	nRF905->read (&cmd,nRF905->config,10);
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

