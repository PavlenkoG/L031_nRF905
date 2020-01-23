/*
 * nRF905.h
 *
 *  Created on: 14 Jan 2020
 *      Author: GRPA
 */

#ifndef DRIVERS_RF905_NRF905_H_
#define DRIVERS_RF905_NRF905_H_

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "nRF905_defs.h"
#include "platform_def.h"

typedef uint8_t (*nRF905_fptr_t)(uint8_t *pCmd, uint8_t *pData, uint16_t Size);
typedef enum {
    NRF905_CH_NO,
    NRF905_CH_NO_HI,
    NRF905_BAND,
    NRF905_HFREQ_PLL,
    NRF905_PA_PWR,
    NRF905_RX_RED_PWR,
    NRF905_AUTO_RETRAN,
    NRF905_RX_AFW,
    NRF905_TX_AFW,
    NRF905_RX_PW,
    NRF905_TX_PW,
    NRF905_RX_ADDRESS_0,
    NRF905_RX_ADDRESS_1,
    NRF905_RX_ADDRESS_2,
    NRF905_RX_ADDRESS_3,
    NRF905_UP_CLK,
    NRF905_XOF,
    NRF905_CRC,
} nRF905_registers_t;

/**
 * \enum nRF905_radio_state_t
 * \brief Radio state
 */
typedef enum {
    NRF905_RADIO_STATE_IDLE, /**< Power down or standby */
    NRF905_RADIO_STATE_TX, /**< Transmitting */
    NRF905_RADIO_STATE_RX /**< Receive mode */
} nRF905_radio_state_t;

/**
 * \enum nRF905_band_t
 * \brief Frequency bands.
 */
typedef enum {
// NOTE:
// When using NRF905_BAND_868 and NRF905_BAND_915 for calculating channel (NRF905_CALC_CHANNEL(f, b)) they should be value 0x01,
// but when using them for setting registers their value should be 0x02.
// They're defined as 0x02 here so when used for calculating channel they're right shifted by 1

    NRF905_BAND_433 = 0x00, /**< 433MHz band */
    NRF905_BAND_868 = 0x02, /**< 868MHz band */
    NRF905_BAND_915 = 0x02 /**< 915MHz band */
} nRF905_band_t;

/**
 * \enum nRF905_pwr_t
 * \brief Output power (n means negative, n10 = -10).
 */
typedef enum {
    NRF905_PWR_n10 = 0x00, /**< -10dBm = 100uW */
    NRF905_PWR_n2 = 0x04, /**< -2dBm = 631uW */
    NRF905_PWR_6 = 0x08, /**< 6dBm = 4mW */
    NRF905_PWR_10 = 0x0C /**< 10dBm = 10mW */
} nRF905_pwr_t;

/**
 * \enum nRF905_low_rx_t
 * \brief Save a few mA by reducing receive sensitivity.
 */
typedef enum {
    NRF905_LOW_RX_DISABLE = 0x00, /**< Disable low power receive */
    NRF905_LOW_RX_ENABLE = 0x10 /**< Enable low power receive */
} nRF905_low_rx_t;

/**
 * \enum nRF905_auto_retran_t
 * \brief Constantly retransmit payload while in transmit mode.
 *
 * Can be useful in areas with lots of interference, but you'll need to make sure you can differentiate between re-transmitted packets and new packets (like an ID number).
 *
 * Other transmissions will be blocked if collision avoidance is enabled.
 */
typedef enum {
    NRF905_AUTO_RETRAN_DISABLE = 0x00, /**< Disable auto re-transmit */
    NRF905_AUTO_RETRAN_ENABLE = 0x20 /**< Enable auto re-transmit */
} nRF905_auto_retran_t;

/**
 * \enum nRF905_outclk_t
 * \brief Output a clock signal on pin 3 of IC.
 */
typedef enum {
    NRF905_OUTCLK_DISABLE = 0x00, /**< Disable output clock */
    NRF905_OUTCLK_4MHZ = 0x04, /**< 4MHz clock */
    NRF905_OUTCLK_2MHZ = 0x05, /**< 2MHz clock */
    NRF905_OUTCLK_1MHZ = 0x06, /**< 1MHz clock */
    NRF905_OUTCLK_500KHZ = 0x07, /**< 500KHz clock */
} nRF905_outclk_t;

/**
 * \enum nRF905_crc_t
 * \brief CRC Checksum.
 */
typedef enum {
    NRF905_CRC_DISABLE = 0x00, /**< Disable CRC */
    NRF905_CRC_8 = 0x40, /**< 8bit CRC */
    NRF905_CRC_16 = 0xC0, /**< 16bit CRC */
} nRF905_crc_t;

/**
 * \enum nRF905_addr_size_t
 * \brief Address size.
 */
typedef enum {
    NRF905_ADDR_SIZE_1 = 0x01, /**< 1 byte */
    NRF905_ADDR_SIZE_2 = 0x02, /**< 2 bytes */
    NRF905_ADDR_SIZE_3 = 0x03, /**< 3 bytes */
    NRF905_ADDR_SIZE_4 = 0x04, /**< 4 bytes */
} nRF905_addr_size_t;

/**
 * \enum nRF905_xof_freq_t
 * \brief Crystal oscillator frequency
 */
typedef enum {
    NFR905_XOF_4MHZ = 0x00, /** < 4 MHz */
    NFR905_XOF_8MHZ = 0x08, /** < 8 MHz */
    NFR905_XOF_12MHZ = 0x10, /** < 12 MHz */
    NFR905_XOF_16MHZ = 0x18, /** < 16 MHz */
    NFR905_XOF_20MHZ = 0x20, /** < 20 MHz */
} nRF905_xof_freq_t;

typedef struct __nRF905_HandeTypeDef {
    uint16_t ChannelNum;
    nRF905_band_t nRF905_band;
    nRF905_pwr_t nRF905_pwr;
    nRF905_low_rx_t nRF905_low_rx;
    nRF905_auto_retran_t nRF905_auto_retran;
    nRF905_addr_size_t nRF905_rx_addr_size;
    nRF905_addr_size_t nRF905_tx_addr_size;
    uint8_t nRF905_rx_pw; /*rx payload width */
    uint8_t nRF905_tx_pw;
    uint32_t nRF905_rx_addr;
    uint32_t nRF905_tx_addr;
    nRF905_outclk_t nRF905_outclk;
    nRF905_xof_freq_t nRF905_xof_freq;
    nRF905_crc_t nRF905_crc;
} nRF905_HandleTypeDef_t;

struct nRF905_dev {
    uint8_t config[10];
    nRF905_fptr_t read;
    nRF905_fptr_t write;
    nRF905_HandleTypeDef_t nRF905_HandleTypeDef;
    nRF905_radio_state_t nRF905_radio_state;
};

/**
 * Maximum payload size
 */
#define NRF905_MAX_PAYLOAD		32

/**
 * Configuration register count
 */
#define NRF905_REGISTER_COUNT	10

void nRF905_init(struct nRF905_dev *nRF905);
void nRF905_writeConfig(struct nRF905_dev *nRF905);
void nRF905_HwConfig(void);
void nRF905_setChannelBand(struct nRF905_dev *nRF905, nRF905_band_t band, uint16_t channel);
void nRF905_setChannel(struct nRF905_dev *nRF905, uint16_t channel);
void nRF905_setBand(struct nRF905_dev *nRF905, nRF905_band_t band);
void nRF905_setTransmitPower(struct nRF905_dev *nRF905, nRF905_pwr_t nRF905_pwr);
void nRF905_setLowRxPower(struct nRF905_dev *nRF905, nRF905_low_rx_t nRF905_low_rx);
void nRF905_setAutoRetransmit(struct nRF905_dev *nRF905, nRF905_auto_retran_t nRF905_auto_retran);
void nRF905_setRxAddressSize(struct nRF905_dev *nRF905, nRF905_addr_size_t size);
void nRF905_setTxAddressSize(struct nRF905_dev *nRF905, nRF905_addr_size_t size);
void nRF905_setRxPayloadSize(struct nRF905_dev *nRF905, uint8_t size);
void nRF905_setTxPayloadSize(struct nRF905_dev *nRF905, uint8_t size);
void nRF905_setCRC(struct nRF905_dev *nRF905, nRF905_crc_t nRF905_crc);
void nRF905_setClockOut(struct nRF905_dev *nRF905, nRF905_outclk_t nRF905_outclk);
void nRF905_setCrstallOsc(struct nRF905_dev *nRF905, nRF905_xof_freq_t nRF905_xof_freq);
void nRF905_setRXAddress(struct nRF905_dev *nRF905, uint32_t address);
void nRF905_setTXAddress(struct nRF905_dev *nRF905, uint32_t address);
uint32_t nRF905_getTXAddress(struct nRF905_dev *nRF905);

/**
 * Set frequency, workout the channel from the frequency
 *
 * 433MHz band: 422.4MHz - 473.5MHz, 100KHz steps
 *
 * 868/915MHz band: 844.8MHz - 947MHz, 200KHz steps
 *
 * @param [band] Frequency band
 * @param [freq] Frequency in Hz
 * @return (none)
 */

void nRF905_setFrequency(nRF905_band_t band, uint32_t freq);

void nRF905_setTxPayloadData(struct nRF905_dev *nRF905, uint8_t *pTxData, uint8_t size);
void nRF905_getTxPayloadData(struct nRF905_dev *nRF905, uint8_t *pTxData);

/**
 * Send the payload.
 *
 * @return false if other transmissions are going on and collision detection is enabled or if the radio is currently transmitting, true if transmission has successfully began
 */
uint8_t nRF905_send(void);

//
//bool nRF905_sendPacket(nrf905_packet_s*);

/**
 * Get current radio state, transmitting etc.
 *
 * @return Radio state
 */
nRF905_radio_state_t nRF905_getState(void);

/**
 * Receive mode. If the radio is currently transmitting then receive mode will be entered once it has finished.
 *
 * @return (none)
 */
void nRF905_receive(void);

/**
 * Get received payload if available.
 *
 * @param [data] Pointer to buffer to place data
 * @param [len] How many bytes to copy
 * @return false if no data available, otherwise true
 */
uint8_t nRF905_getData(struct nRF905_dev *nRF905, uint8_t *data, uint8_t len);

/**
 * Wake up into standby mode.
 * Will take 3ms to complete.
 *
 * @return (none)
 */
void nRF905_powerUp(void);

/**
 * Sleep.
 *
 * @note Must call nRF905_powerUp() before transmitting or receiving again.
 * @return (none)
 */
void nRF905_powerDown(void);

/**
 * Enter standby mode.
 *
 * @return (none)
 */
void nRF905_enterStandBy(void);

/**
 * Leave standby and go into receive mode (same as just calling nRF905_receive()).
 *
 * @return (none)
 */
inline void nRF905_leaveStandBy(void) {
    nRF905_receive();
}

/**
 * blah
 *
 * @return blah
 */
uint8_t nRF905_receiveBusy(void);

/**
 * See if airway is busy (carrier detect pin high).
 *
 * @return true if other transmissions detected, otherwise false
 */
uint8_t nRF905_airwayBusy(void);

/**
 * Disable data ready interrupt.
 *
 * If NRF905_INTERRUPTS is enabled then this function must be called before doing SPI communications with any other device.
 *
 * @return (none)
 */
inline void nRF905_interrupt_off(void) {
}

/**
 * Enable data ready interrupt.
 *
 * If NRF905_INTERRUPTS is enabled then this function must be called once you have finished doing SPI communications with another device.
 *
 * @return (none)
 */
inline void nRF905_interrupt_on(void) {
}

/**
 * Read configuration registers into byte array of ::NRF905_REGISTER_COUNT elements, mainly for debugging.
 *
 * @return (none)
 */
void nRF905_getConfigRegisters(struct nRF905_dev *nRF905);
void nRF905_printConfig(struct nRF905_dev *nRF905);
void nRF905_writeConfigRegister(struct nRF905_dev *nRF905, uint8_t reg, uint8_t value);
void nRF905_startShockBurstRx(struct nRF905_dev *nRF905);
void nRF905_startShcokBurstTx(struct nRF905_dev *nRF905);
void nRF905_stopShockBurstTx(struct nRF905_dev *nRF905);
void nRF905_sendData(struct nRF905_dev *nRF905, uint8_t *pTxData, uint16_t Size);

#endif /* DRIVERS_RF905_NRF905_H_ */
