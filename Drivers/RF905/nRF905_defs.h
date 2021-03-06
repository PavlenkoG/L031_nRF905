/*
 * nRF905_defs.h
 *
 *  Created on: 14 Jan 2020
 *      Author: GRPA
 */

#ifndef DRIVERS_RF905_NRF905_DEFS_H_
#define DRIVERS_RF905_NRF905_DEFS_H_

// Instructions
#define NRF905_CMD_NOP			0xFF
#define NRF905_CMD_W_CONFIG		0x00
#define NRF905_CMD_R_CONFIG		0x10
#define NRF905_CMD_W_TX_PAYLOAD	0x20
#define NRF905_CMD_R_TX_PAYLOAD	0x21
#define NRF905_CMD_W_TX_ADDRESS	0x22
#define NRF905_CMD_R_TX_ADDRESS	0x23
#define NRF905_CMD_R_RX_PAYLOAD	0x24
#define NRF905_CMD_CHAN_CONFIG	0x80

// Registers
#define NRF905_REG_CHANNEL		0x00

#define NRF905_REG_BAND			0x01
#define NRF905_REG_PWR			0x01
#define NRF905_REG_LOW_RX		0x01
#define NRF905_REG_AUTO_RETRAN	0x01

#define NRF905_REG_ADDR_WIDTH	0x02

#define NRF905_REG_RX_PAYLOAD_SIZE	0x03

#define NRF905_REG_TX_PAYLOAD_SIZE	0x04

#define NRF905_REG_RX_ADDRESS	0x05

#define NRF905_REG_UPCLK		0x09
#define NRF905_REG_XOF			0x09
#define NRF905_REG_CRC			0x09

// Masks
#define NRF905_CHANNEL_HI_MSK   0xFE
#define NRF905_CHANNEL_MSK		0xFC
#define NRF905_BAND_MSK         0xFD
#define NRF905_PA_PWR_MSK		0xF3
#define NRF905_LOW_RX_MSK		0xEF
#define NRF905_AUTO_RETRAN_MSK	0xDF
#define NRF905_RX_ADDR_WDT_MSK	0xF8
#define NRF905_TX_ADDR_WDT_MSK	0x8F
#define NRF905_UP_CLK_MSK		0xF8
#define NRF905_XOF_MSK			0xC7
#define NRF905_CRC_MSK			0x3F

/*
 * #define SPI_SR_RXNE_Pos             (0U)
 * #define SPI_SR_RXNE_Msk             (0x1U << SPI_SR_RXNE_Pos)
 * #define SPI_SR_RXNE                 SPI_SR_RXNE_Msk
 */
// Clock options
#define NRF905_CLK_4MHZ			0x00
#define NRF905_CLK_8MHZ			0x08
#define NRF905_CLK_12MHZ		0x10
#define NRF905_CLK_16MHZ		0x18
#define NRF905_CLK_20MHZ		0x20

// Bit positions
#define NRF905_STATUS_DR		5
#define NRF905_STATUS_AM		7

#endif /* DRIVERS_RF905_NRF905_DEFS_H_ */
