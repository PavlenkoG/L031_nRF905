#include <stdio.h>
#include <stdlib.h>
#include "platform_def.h"

#define USER_ARGUMENTS 33

#define CHANNEL_SET 0x1
#define BAND_SET 0x2
#define SET_PWR 0x3
#define SET_REDUCED_RX 0x4
#define SET_AUTO_RETRAN 0x5
#define SET_RX_ADDR 0x6
#define SET_RX_ADDR_LEN 0x7
#define SET_TX_ADDR_LEN 0x8
#define SET_RX_PAYLOAD_LEN 0x9
#define SET_TX_PAYLOAD_LEN 0xA
#define SET_UP_CLK 0xB
#define SET_XOF 0xC
#define SET_CRC 0xD
#define SET_TX_ADDR 0xE
#define SET_TX_PAYLOAD 0xF
#define GET_TX_PAYLOAD 0x10
#define PRINT_CONFIG 0x11
#define SET_REGISTER 0x12
#define START_SK_BURST 0x13
#define START_RX_BURST 0x14
#define HELP_CMD 0x20
#define ERROR_CMD 0xFF
#define __CMD_LENGTH(__CMD__) ((sizeof(__CMD__)-1)/sizeof(uint8_t))


struct userCmd_t {
    uint8_t command;
    uint8_t argCount;
    uint32_t args[USER_ARGUMENTS];
};

void commandHandler(struct userCmd_t *usercmd, uint8_t *rxBuffer, uint8_t len);
