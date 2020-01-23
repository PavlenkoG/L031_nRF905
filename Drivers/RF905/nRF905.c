/*
 * nRF905.c
 *
 *  Created on: 15 Jan 2020
 *      Author: GRPA
 */
#include "nRF905.h"
/*
 uint8_t confString[][19] = {
 "Channel N         ", //Conf string 1
 "Frequency mode    ",
 "Output power      ",
 "Reduced RX power  ",
 "Auto retransmit   ",
 "RX Address width  ",
 "Tx Address width  ",
 "RX payload length ",
 "TX payload length ",
 "RX address 0      ",
 "RX address 1      ",
 "RX address 2      ",
 "RX address 3      ",
 "Output clock      ",
 "Crystal osc freq  ",
 "CRC mode          "
 };

 uint8_t paramString [][21] = {
 "433 MHz band         ", //0
 "868/933 MHz band     ", //1
 "-10dBm/100uW         ", //2
 "-2dBm/631uW          ", //3
 "6dBm/4mW             ", //4
 "10dBm/10mW           ", //5
 "Low RX power enabled ", //6
 "Low RX power disabled", //7
 "Retransmit enabled   ", //8
 "Retransmit disabled  ", //9
 "Output clock disabled", //0xA
 "4 MHz output clock   ", //0xB
 "2 MHz output clock   ", //0xC
 "1 MHz output clock   ", //0xD
 "500 KHz output clock ", //0xE
 "CRC disabled         ", //0xF
 "CRC 8 bit            ", //0x10
 "CRC 16 bit           ", //0x11
 "Osc freq 4 MHz       ", //0x12
 "Osc freq 8 MHz       ", //0x13
 "Osc freq 12 MHz      ", //0x14
 "Osc freq 16 MHz      ", //0x15
 "Osc freq 20 MHz      ", //0x16
 };

 uint8_t mapString[][5]= {
 //cfgStr|reg|mask|val|prmStr
 {0x0,0x0,0x0,0,0}, //0
 {0x1,0x0,0x0},   //1
 {0x1,0x2,0x1},   //2
 {0x2,0x0,0x2},   //3
 {0x2,0x4,0x3},   //4
 {0x2,0x8,0x4},   //5
 {0x2,0xC,0x5},   //6

 };
 */

void nRF905_init(struct nRF905_dev *nRF905) {

    uint i = 0;
    nRF905_HwConfig();

    nRF905_setChannelBand(nRF905, nRF905->nRF905_HandleTypeDef.nRF905_band, nRF905->nRF905_HandleTypeDef.ChannelNum);
    nRF905_setTransmitPower(nRF905, nRF905->nRF905_HandleTypeDef.nRF905_pwr);
    nRF905_setLowRxPower(nRF905, nRF905->nRF905_HandleTypeDef.nRF905_low_rx);
    nRF905_setAutoRetransmit(nRF905, nRF905->nRF905_HandleTypeDef.nRF905_auto_retran);
    nRF905_setRxAddressSize(nRF905, nRF905->nRF905_HandleTypeDef.nRF905_rx_addr_size);
    nRF905_setTxAddressSize(nRF905, nRF905->nRF905_HandleTypeDef.nRF905_tx_addr_size);
    nRF905_setRXAddress(nRF905, nRF905->nRF905_HandleTypeDef.nRF905_rx_addr);
    nRF905_setCRC(nRF905, nRF905->nRF905_HandleTypeDef.nRF905_crc);
    nRF905_setClockOut(nRF905, nRF905->nRF905_HandleTypeDef.nRF905_outclk);
    nRF905_setCrstallOsc(nRF905, nRF905->nRF905_HandleTypeDef.nRF905_xof_freq);
    nRF905_setTXAddress(nRF905, nRF905->nRF905_HandleTypeDef.nRF905_tx_addr);
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

void nRF905_writeConfig(struct nRF905_dev *nRF905) {

    uint8_t cmd = NRF905_CMD_W_CONFIG;
    nRF905->write(&cmd, nRF905->config, 10);
}

/**
 * @brief set the receive/transmit channel
 * @param pointer to nRF905 device structure
 * @param band 433 or 868/915 Mhz, This parameter should be a value of @ref nRF905_band_t
 * @param uint16_t channel number
 *
 * @retval
 */
void nRF905_setChannelBand(struct nRF905_dev *nRF905, nRF905_band_t band, uint16_t channel) {
    if (channel < 512) {
        uint8_t chValue = (uint8_t) channel;
        nRF905_writeConfigRegister(nRF905, NRF905_CH_NO, chValue);
        chValue = 0;
        chValue |= (uint8_t) (channel >> 8);
        chValue |= (uint8_t) band;
        nRF905_writeConfigRegister(nRF905, NRF905_HFREQ_PLL, (uint8_t) chValue);
    }
}

void nRF905_setChannel (struct nRF905_dev *nRF905, uint16_t channel) {
    if (channel < 512) {
        uint8_t chValue = (uint8_t) channel;
        nRF905_writeConfigRegister(nRF905, NRF905_CH_NO, chValue);
        chValue = 0;
        chValue |= (uint8_t) (channel >> 8);
        nRF905_writeConfigRegister(nRF905, NRF905_CH_NO_HI, (uint8_t) chValue);
    }
}

void nRF905_setBand (struct nRF905_dev *nRF905, nRF905_band_t band) {
    nRF905_writeConfigRegister(nRF905, NRF905_BAND, band);
}

/**
 * @brief set transmit power
 * @param pointer to nRF905 device structure
 * @param 100uW/631uW/4mW/10mW, This parameter should be a value of @ref nRF905_pwr_t
 *
 * @retval
 */
void nRF905_setTransmitPower(struct nRF905_dev *nRF905, nRF905_pwr_t nRF905_pwr) {
    nRF905_writeConfigRegister(nRF905, NRF905_PA_PWR, nRF905_pwr);
}

/**
 * @brief set reduced current in RX mode
 * @param pointer to nRF905 device structure
 * @param normal operation/reduced power, This parameter should be a value of @ref nRF905_low_rx_t
 *
 * @retval
 */
void nRF905_setLowRxPower(struct nRF905_dev *nRF905,
        nRF905_low_rx_t nRF905_low_rx) {
    nRF905_writeConfigRegister(nRF905, NRF905_RX_RED_PWR, nRF905_low_rx);
}

/**
 * @brief Enable retransmit contents in TX register
 * @param pointer to nRF905 device structure
 * @param no retransmission/retransmission of data packet, This parameter should be a value of @ref nRF905_auto_retran_t
 *
 * @retval
 */
void nRF905_setAutoRetransmit(struct nRF905_dev *nRF905,
        nRF905_auto_retran_t nRF905_auto_retran) {
    nRF905_writeConfigRegister(nRF905, NRF905_AUTO_RETRAN, nRF905_auto_retran);
}

/**
 * @brief Set RX-address width
 * @param pointer to nRF905 device structure
 * @param 1-4 byte lenght, This parameter should be a value of @ref nRF905_addr_size_t
 *
 * @retval
 */
void nRF905_setRxAddressSize(struct nRF905_dev *nRF905,
        nRF905_addr_size_t nRF905_addr_size) {
    nRF905_writeConfigRegister(nRF905, NRF905_RX_AFW, nRF905_addr_size);
}

/**
 * @brief Set TX-address width
 * @param pointer to nRF905 device structure
 * @param 1-4 byte lenght, This parameter should be a value of @ref nRF905_addr_size_t
 *
 * @retval
 */
void nRF905_setTxAddressSize(struct nRF905_dev *nRF905,
        nRF905_addr_size_t nRF905_addr_size) {
    nRF905_writeConfigRegister(nRF905, NRF905_TX_AFW, nRF905_addr_size << 4);
}

/**
 * @brief Set RX-Payload width
 * @param pointer to nRF905 device structure
 *
 * @param 1 to 32 bytes
 * @retval
 */
void nRF905_setRxPayloadSize(struct nRF905_dev *nRF905, uint8_t size) {
    if (size > 0 && size <= NRF905_MAX_PAYLOAD)
        nRF905_writeConfigRegister(nRF905, NRF905_RX_PW, size);
}

/**
 * @brief Set TX-Payload width
 * @param pointer to nRF905 device structure
 *
 * @param 1 to 32 bytes
 * @retval
 */
void nRF905_setTxPayloadSize(struct nRF905_dev *nRF905, uint8_t size) {
    if (size > 0 && size <= NRF905_MAX_PAYLOAD)
        nRF905_writeConfigRegister(nRF905, NRF905_TX_PW, size);
}

/**
 * @brief Set RX-Address
 * @param pointer to nRF905 device structure
 *
 * @param uint32_t address
 * @retval
 */
void nRF905_setRXAddress(struct nRF905_dev *nRF905, uint32_t address) {
    nRF905_writeConfigRegister(nRF905, NRF905_RX_ADDRESS_0, (uint8_t) address);
    nRF905_writeConfigRegister(nRF905, NRF905_RX_ADDRESS_1, (uint8_t) (address >> 8));
    nRF905_writeConfigRegister(nRF905, NRF905_RX_ADDRESS_2, (uint8_t) (address >> 16));
    nRF905_writeConfigRegister(nRF905, NRF905_RX_ADDRESS_3, (uint8_t) (address >> 24));
}

/**
 * @brief Set CRC mode
 * @param pointer to nRF905 device structure
 *
 * @param disable/8CRC/16CRC, This parameter should be a value of @ref nRF905_crc_t
 * @retval
 */
void nRF905_setCRC(struct nRF905_dev *nRF905, nRF905_crc_t nRF905_crc) {
    nRF905_writeConfigRegister(nRF905, NRF905_CRC, nRF905_crc);

}

/**
 * @brief Set Output clock frequency
 * @param pointer to nRF905 device structure
 *
 * @param disable/4MHz/2MHz/1MHz/500kHz, This parameter should be a value of @ref nRF905_outclk_t
 * @retval
 */
void nRF905_setClockOut(struct nRF905_dev *nRF905,
        nRF905_outclk_t nRF905_outclk) {
    nRF905_writeConfigRegister(nRF905, NRF905_UP_CLK, nRF905_outclk);
}

/**
 * @brief Set crystall oscillator frequency
 * @param pointer to nRF905 device structure
 *
 * @param 4MHz/8MHz/12MHz/16MHz/20MHz, This parameter should be a value of @ref nRF905_xof_freq_t
 * @retval
 */
void nRF905_setCrstallOsc(struct nRF905_dev *nRF905,
        nRF905_xof_freq_t nRF905_xof_freq) {
    nRF905_writeConfigRegister(nRF905, NRF905_XOF, nRF905_xof_freq);
}

/**
 * @brief write TX Adress register
 * @param pointer to nRF905 device structure
 *
 * @param TX Adress
 * @retval
 */
void nRF905_setTXAddress(struct nRF905_dev *nRF905, uint32_t address) {
    uint8_t cmdW = NRF905_CMD_W_TX_ADDRESS;
    nRF905->write(&cmdW, &address, 4);
}

/**
 * @brief read TX Adress register
 * @param pointer to nRF905 device structure
 *
 * @retval uint32_t TX Adress
 */
uint32_t nRF905_getTXAddress(struct nRF905_dev *nRF905) {
    uint32_t txAddr = 0;
    void *pAddr = &txAddr;
    uint8_t cmdW = NRF905_CMD_R_TX_ADDRESS;
    nRF905->read(&cmdW, pAddr, 4);
    return (txAddr);
}

void nRF905_powerUp(void) {
    HAL_GPIO_WritePin(nRF905_PWR_PORT, nRF905_PWR, GPIO_PIN_SET);
}

void nRF905_powerDown(void) {
    HAL_GPIO_WritePin(nRF905_PWR_PORT, nRF905_PWR, GPIO_PIN_RESET);
}

void nRF905_getConfigRegisters(struct nRF905_dev *nRF905) {
    uint8_t cmd = NRF905_CMD_R_CONFIG;
    uint8_t i = 0;
    nRF905->read(&cmd, nRF905->config, 10);

    nRF905->nRF905_HandleTypeDef.ChannelNum = nRF905->config[0];
    nRF905->nRF905_HandleTypeDef.ChannelNum |= ((nRF905->config[1] & 0x01) << 8);
    nRF905->nRF905_HandleTypeDef.nRF905_band = (nRF905->config[1] & 0x02);
    nRF905->nRF905_HandleTypeDef.nRF905_pwr = (nRF905->config[1] & 0x0C);
    nRF905->nRF905_HandleTypeDef.nRF905_low_rx = (nRF905->config[1] & 0x10);
    nRF905->nRF905_HandleTypeDef.nRF905_auto_retran = (nRF905->config[1] & 0x20);
    nRF905->nRF905_HandleTypeDef.nRF905_rx_addr_size = (nRF905->config[2] & 0x07);
    nRF905->nRF905_HandleTypeDef.nRF905_tx_addr_size = ((nRF905->config[2] & 0x70) >> 4);
    nRF905->nRF905_HandleTypeDef.nRF905_rx_pw = (nRF905->config[3]);
    nRF905->nRF905_HandleTypeDef.nRF905_tx_pw = (nRF905->config[4]);
    nRF905->nRF905_HandleTypeDef.nRF905_rx_addr = ((nRF905->config[8] << 24) | (nRF905->config[7] << 16) | (nRF905->config[6] << 8) | (nRF905->config[5]));
    nRF905->nRF905_HandleTypeDef.nRF905_outclk = (nRF905->config[9] & 0x07);
    nRF905->nRF905_HandleTypeDef.nRF905_xof_freq = (nRF905->config[9] & 0x38);
    nRF905->nRF905_HandleTypeDef.nRF905_crc = (nRF905->config[9] & 0xC0);
}

void nRF905_printConfig(struct nRF905_dev *nRF905) {
    uint32_t txAddr;
    nRF905_getConfigRegisters(nRF905);

    printf("Config Register : 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\r\n", nRF905->config[0],\
            nRF905->config[1],nRF905->config[2],nRF905->config[3],nRF905->config[4],nRF905->config[5], \
            nRF905->config[6], nRF905->config[7],nRF905->config[8],nRF905->config[9]);
    txAddr = nRF905_getTXAddress(nRF905);
    printf ("TX Address : 0x%x\r\n",txAddr);
    /*
     printf("channel number = 0x%x \r\n",((nRF905->config[0])|(nRF905->config[1] & 0x01) << 8));
     if (nRF905->nRF905_HandleTypeDef.nRF905_band == NRF905_BAND_868)
     printf ("868 MHz \r\n");
     if (nRF905->nRF905_HandleTypeDef.nRF905_band == NRF905_BAND_433)
     printf ("433 MHz \r\n");
     if (nRF905->nRF905_HandleTypeDef.nRF905_pwr == NRF905_PWR_n10)
     printf ("-10dBm = 100 uW\r\n");
     if (nRF905->nRF905_HandleTypeDef.nRF905_pwr == NRF905_PWR_n2)
     printf ("-2dBm = 631 uW\r\n");
     if (nRF905->nRF905_HandleTypeDef.nRF905_pwr == NRF905_PWR_6)
     printf ("-6dBm = 4 mW\r\n");
     if (nRF905->nRF905_HandleTypeDef.nRF905_pwr == NRF905_PWR_10)
     printf ("-10dBm = 10 mW\r\n");
     */
}

void nRF905_writeConfigRegister(struct nRF905_dev *nRF905,
        nRF905_registers_t nRF905_reg, uint8_t value) {
    uint8_t cmdR = NRF905_CMD_R_CONFIG;
    uint8_t cmdW = NRF905_CMD_W_CONFIG;
    uint8_t tempReg = 0;
    uint8_t msk = 0x00;

    switch (nRF905_reg) {
    // CH_NO[7:0]
    case NRF905_CH_NO:
        cmdR |= NRF905_REG_CHANNEL;
        cmdW |= NRF905_REG_CHANNEL;
        break;
    // CH_NO[8]
    case NRF905_CH_NO_HI:
        cmdR |= NRF905_REG_BAND;
        cmdW |= NRF905_REG_BAND;
        msk = NRF905_CHANNEL_HI_MSK;
        break;
    case NRF905_BAND:
        cmdR |= NRF905_REG_BAND;
        cmdW |= NRF905_REG_BAND;
        msk = NRF905_BAND_MSK;
        break;
    // CH_NO[8] and HFREQ_PLL register values
    case NRF905_HFREQ_PLL:
        cmdR |= NRF905_REG_BAND;
        cmdW |= NRF905_REG_BAND;
        msk = NRF905_CHANNEL_MSK;
        break;
    // PA_PWR [1:0]
    case NRF905_PA_PWR:
        cmdR |= NRF905_REG_PWR;
        cmdW |= NRF905_REG_PWR;
        msk = NRF905_PA_PWR_MSK;
        break;
        // RX_RED_PWR
    case NRF905_RX_RED_PWR:
        cmdR |= NRF905_REG_LOW_RX;
        cmdW |= NRF905_REG_LOW_RX;
        msk = NRF905_LOW_RX_MSK;
        break;
        // AUTO_RETRAN
    case NRF905_AUTO_RETRAN:
        cmdR |= NRF905_REG_AUTO_RETRAN;
        cmdW |= NRF905_REG_AUTO_RETRAN;
        msk = NRF905_AUTO_RETRAN_MSK;
        break;
    case NRF905_RX_AFW:
        cmdR |= NRF905_REG_ADDR_WIDTH;
        cmdW |= NRF905_REG_ADDR_WIDTH;
        msk = NRF905_RX_ADDR_WDT_MSK;
        break;
    case NRF905_TX_AFW:
        cmdR |= NRF905_REG_ADDR_WIDTH;
        cmdW |= NRF905_REG_ADDR_WIDTH;
        msk = NRF905_TX_ADDR_WDT_MSK;
        break;
    case NRF905_RX_PW:
        cmdR |= NRF905_REG_RX_PAYLOAD_SIZE;
        cmdW |= NRF905_REG_RX_PAYLOAD_SIZE;
        break;
    case NRF905_TX_PW:
        cmdR |= NRF905_REG_TX_PAYLOAD_SIZE;
        cmdW |= NRF905_REG_TX_PAYLOAD_SIZE;
        break;
    case NRF905_RX_ADDRESS_0:
        cmdR |= NRF905_REG_RX_ADDRESS;
        cmdW |= NRF905_REG_RX_ADDRESS;
        break;
    case NRF905_RX_ADDRESS_1:
        cmdR |= NRF905_REG_RX_ADDRESS + 1;
        cmdW |= NRF905_REG_RX_ADDRESS + 1;
        break;
    case NRF905_RX_ADDRESS_2:
        cmdR |= NRF905_REG_RX_ADDRESS + 2;
        cmdW |= NRF905_REG_RX_ADDRESS + 2;
        break;
    case NRF905_RX_ADDRESS_3:
        cmdR |= NRF905_REG_RX_ADDRESS + 3;
        cmdW |= NRF905_REG_RX_ADDRESS + 3;
        break;
    case NRF905_UP_CLK:
        cmdR |= NRF905_REG_UPCLK;
        cmdW |= NRF905_REG_UPCLK;
        msk = NRF905_UP_CLK_MSK;
        break;
    case NRF905_XOF:
        cmdR |= NRF905_REG_XOF;
        cmdW |= NRF905_REG_XOF;
        msk = NRF905_XOF_MSK;
        break;
    case NRF905_CRC:
        cmdR |= NRF905_REG_CRC;
        cmdW |= NRF905_REG_CRC;
        msk = NRF905_CRC_MSK;
        break;
    }

    nRF905->read(&cmdR, &tempReg, 1);
    tempReg &= msk;
    tempReg |= value;
    nRF905->write(&cmdW, &tempReg, 1);
}

void nRF905_startShockBurstRx(struct nRF905_dev *nRF905) {
    nRF905_powerUp();
    HAL_GPIO_WritePin(nRF905_TXEN_PORT, nRF905_TXEN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(nRF905_TRXCE_PORT, nRF905_TRXCE, GPIO_PIN_SET);
    nRF905->nRF905_radio_state = NRF905_RADIO_STATE_RX;
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void nRF905_startShcokBurstTx(struct nRF905_dev *nRF905) {
    HAL_GPIO_WritePin(nRF905_TXEN_PORT, nRF905_TXEN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(nRF905_TRXCE_PORT, nRF905_TRXCE, GPIO_PIN_SET);
    nRF905_powerUp();

    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
    nRF905->nRF905_radio_state = NRF905_RADIO_STATE_TX;
}

void nRF905_stopShockBurstTx(struct nRF905_dev *nRF905) {
    HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
    HAL_GPIO_WritePin(nRF905_TRXCE_PORT, nRF905_TRXCE, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(nRF905_TXEN_PORT, nRF905_TXEN, GPIO_PIN_RESET);
    nRF905_powerDown();
    nRF905->nRF905_radio_state = NRF905_RADIO_STATE_IDLE;
}

void nRF905_sendData(struct nRF905_dev *nRF905, uint8_t *pTxData, uint16_t Size) {

    HAL_GPIO_WritePin(nRF905_TXEN_PORT, nRF905_TXEN, GPIO_PIN_SET);
    nRF905_powerUp();
    HAL_GPIO_WritePin(nRF905_TRXCE_PORT, nRF905_TRXCE, GPIO_PIN_RESET);

    uint8_t cmd = NRF905_CMD_W_TX_PAYLOAD;
    nRF905->write(&cmd, pTxData, Size);
    HAL_GPIO_WritePin(nRF905_TRXCE_PORT, nRF905_TRXCE, GPIO_PIN_SET);

    while (HAL_GPIO_ReadPin(nRF905_DR_PORT, nRF905_DR) == GPIO_PIN_RESET) {

    }
    HAL_GPIO_WritePin(nRF905_TRXCE_PORT, nRF905_TRXCE, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(nRF905_TXEN_PORT, nRF905_TXEN, GPIO_PIN_RESET);
    nRF905_powerDown();

}

void nRF905_setTxPayloadData(struct nRF905_dev *nRF905, uint8_t *pTxData, uint8_t size) {
    uint8_t cmd = NRF905_CMD_W_TX_PAYLOAD;
    nRF905->write(&cmd, pTxData, size);
}

void nRF905_getTxPayloadData(struct nRF905_dev *nRF905, uint8_t *pTxData) {
    uint8_t cmd = NRF905_CMD_R_TX_PAYLOAD;
    nRF905->read(&cmd, pTxData, nRF905->nRF905_HandleTypeDef.nRF905_tx_pw);
}

uint8_t nRF905_getData(struct nRF905_dev *nRF905, void *data, uint8_t len) {
    uint8_t cmd = NRF905_CMD_R_RX_PAYLOAD;
    nRF905->read(&cmd, data, len);
    return 0;
}

