#include "console.h"
#include <string.h>

const uint8_t setRegister[] =     "setConfRegister";
const uint8_t setChannelCmd[] =   "setChannel";
const uint8_t setBandCmd[] =      "setBand";
const uint8_t setPwr[] =          "setTxPower";
const uint8_t setRedRx[] =        "setReducedRx";
const uint8_t setAutoRetran[] =   "setAutoRetran";
const uint8_t setRxAddr[] =       "setRxAddr";
const uint8_t setRxPw[] =         "setRxAddLen";
const uint8_t setTxPw[] =         "setTxAddLen";
const uint8_t setRxLen[] =        "setRxLen";
const uint8_t setTxLen[] =        "setTxLen";
const uint8_t setTxAddr[] =       "setTxAddr";
const uint8_t setUpClk[] =        "setUpClk";
const uint8_t setXOF[] =          "setXOF";
const uint8_t setCRC[] =          "setCRC";
const uint8_t writeTxPayload[] =  "txPayloadW";
const uint8_t readTxPayload[] =   "txPayloadR";
const uint8_t printCfg[] =        "printConfig";
const uint8_t startSckBrst[] =    "startTxBrst";
const uint8_t help[] =            "help";

void commandHandler(struct userCmd_t *usercmd, uint8_t *rxBuffer, uint8_t len) {
    uint8_t data_present;
    uint8_t data;
    uint8_t lastUserData;
    uint8_t hexDecSelect;
    uint8_t lenCnt = len;
    int32_t msgNumberSign;
    uint8_t commandLen;
    usercmd->command = 0;
    usercmd->argCount = 0;
    hexDecSelect = 10;

    // set channel command
    if (!(strncmp((char*) (rxBuffer), setChannelCmd, __CMD_LENGTH(setChannelCmd)))) {
        commandLen = __CMD_LENGTH(setChannelCmd);
        data_present = 1;
        usercmd->command = CHANNEL_SET;
    // set band
    } else if (!(strncmp((char*) (rxBuffer), setBandCmd, __CMD_LENGTH(setBandCmd)))) {
        commandLen = __CMD_LENGTH(setBandCmd);
        data_present = 1;
        usercmd->command = BAND_SET;
    // set transmit power
    } else if (!(strncmp((char*) (rxBuffer), setPwr, __CMD_LENGTH(setPwr)))) {
        commandLen = __CMD_LENGTH(setPwr);
        data_present = 1;
        usercmd->command = SET_PWR;
    // set reduced RX Power
    } else if (!(strncmp((char*) (rxBuffer), setRedRx, __CMD_LENGTH(setRedRx)))) {
        commandLen = __CMD_LENGTH(setRedRx);
        data_present = 1;
        usercmd->command = SET_REDUCED_RX;
    // set auto retransmit
    } else if (!(strncmp((char*) (rxBuffer), setAutoRetran, __CMD_LENGTH(setAutoRetran)))) {
        commandLen = __CMD_LENGTH(setAutoRetran);
        data_present = 1;
        usercmd->command = SET_AUTO_RETRAN;
    // set rx address length
    } else if (!(strncmp((char*) (rxBuffer), setRxPw, __CMD_LENGTH(setRxPw)))) {
        commandLen = __CMD_LENGTH(setRxPw);
        data_present = 1;
        usercmd->command = SET_RX_ADDR_LEN;
    // set tx address length
    } else if (!(strncmp((char*) (rxBuffer), setTxPw, __CMD_LENGTH(setTxPw)))) {
        commandLen = __CMD_LENGTH(setTxPw);
        data_present = 1;
        usercmd->command = SET_TX_ADDR_LEN;
    // set rx address
    } else if (!(strncmp((char*) (rxBuffer), setRxAddr, __CMD_LENGTH(setRxAddr)))) {
        commandLen = __CMD_LENGTH(setRxAddr);
        data_present = 1;
        usercmd->command = SET_RX_ADDR;
    // set rx payload length
    } else if (!(strncmp((char*) (rxBuffer), setRxLen, __CMD_LENGTH(setRxLen)))) {
        commandLen = __CMD_LENGTH(setRxLen);
        data_present = 1;
        usercmd->command = SET_RX_PAYLOAD_LEN;
    // set tx payload length
    } else if (!(strncmp((char*) (rxBuffer), setTxLen, __CMD_LENGTH(setTxLen)))) {
        commandLen = __CMD_LENGTH(setTxLen);
        data_present = 1;
        usercmd->command = SET_TX_PAYLOAD_LEN;
    // set up clock parameter
    } else if (!(strncmp((char*) (rxBuffer), setUpClk, __CMD_LENGTH(setUpClk)))) {
        commandLen = __CMD_LENGTH(setUpClk);
        data_present = 1;
        usercmd->command = SET_UP_CLK;
    // set any register
    } else if (!(strncmp((char*) (rxBuffer), setRegister, __CMD_LENGTH(setRegister)))) {
        commandLen = __CMD_LENGTH(setRegister);
        data_present = 1;
        usercmd->command = SET_REGISTER;
    // set oscillator frequency
    } else if (!(strncmp((char*) (rxBuffer), setXOF, __CMD_LENGTH(setXOF)))) {
        commandLen = __CMD_LENGTH(setXOF);
        data_present = 1;
        usercmd->command = SET_XOF;
    // set CRC config
    } else if (!(strncmp((char*) (rxBuffer), setCRC, __CMD_LENGTH(setCRC)))) {
        commandLen = __CMD_LENGTH(setCRC);
        data_present = 1;
        usercmd->command = SET_CRC;
    // set TX Address
    } else if (!(strncmp((char*) (rxBuffer), setTxAddr, __CMD_LENGTH(setTxAddr)))) {
        commandLen = __CMD_LENGTH(setTxAddr);
        data_present = 1;
        usercmd->command = SET_TX_ADDR;
    // write transmit payload
    } else if (!(strncmp((char*) (rxBuffer), writeTxPayload, __CMD_LENGTH(writeTxPayload)))) {
        commandLen = __CMD_LENGTH(writeTxPayload);
        data_present = 1;
        usercmd->command = SET_TX_PAYLOAD;
    // read transmit payload
    } else if (!(strncmp((char*) (rxBuffer), readTxPayload, __CMD_LENGTH(readTxPayload)))) {
        commandLen = __CMD_LENGTH(readTxPayload);
        data_present = 0;
        usercmd->command = GET_TX_PAYLOAD;
    // start shock burst transmit
    } else if (!(strncmp((char*) (rxBuffer), startSckBrst, __CMD_LENGTH(startSckBrst)))) {
        commandLen = __CMD_LENGTH(startSckBrst);
        data_present = 0;
        usercmd->command = START_SK_BURST;
    // print config registers
    } else if (!(strncmp((char*) (rxBuffer), printCfg, __CMD_LENGTH(printCfg)))) {
        data_present = 0;
        usercmd->command = PRINT_CONFIG;
    // print help
    } else if (!(strncmp((char*) (rxBuffer), help, __CMD_LENGTH(help)))) {
        data_present = 0;
        usercmd->command = HELP_CMD;
    //
    } else {
        usercmd->command = ERROR_CMD;
    }

    lenCnt = lenCnt - commandLen;
    rxBuffer = rxBuffer + commandLen;
    if (data_present) {

        lastUserData = *rxBuffer;
        rxBuffer++;
        data = *rxBuffer;
        while (lenCnt) {
            // first symbol after space
            if ((data >= '0' && data <= '9') && lastUserData == ' ') {
                usercmd->argCount++;
                if (usercmd->argCount >= USER_ARGUMENTS) {
                    //error
                    usercmd->args[usercmd->argCount - 1] = 0;
                } else {
                    hexDecSelect = 10;
                    usercmd->args[usercmd->argCount - 1] = (data - '0');
                }
            } else if (data >= '0' && data <= '9') {
                usercmd->args[usercmd->argCount - 1] = usercmd->args[usercmd->argCount - 1] * hexDecSelect + (data - '0');
            } else if (data >= 'A' && data <= 'F') {
                usercmd->args[usercmd->argCount - 1] = usercmd->args[usercmd->argCount - 1] * hexDecSelect + (data - 55);
            } else if (data >= 'a' && data <= 'f') {
                usercmd->args[usercmd->argCount - 1] = usercmd->args[usercmd->argCount - 1] * hexDecSelect + (data - 87);
            } else if (data == 'x') {
                usercmd->args[usercmd->argCount - 1] = 0;
                hexDecSelect = 16;
            } else if (data == 0x0d || data == 0x0a) {
                break;
            }

            lastUserData = *rxBuffer;
            rxBuffer++;
            data = *rxBuffer;
            lenCnt--;
        }
    }
}

void printHelp (void) {
    printf ("%s : set channel value\r\n", setChannelCmd);
    printf ("%s : set Band 0 for 433 MHz, 2 for 868 MHz\r\n", setBandCmd);
    printf ("%s : set Transmit power 0/4/8/12 -10dBm/-2dBm/6dBm/10dBm\r\n", setPwr);
    printf ("%s : set reduced rx power (0x10) \r\n", setRedRx);
    printf ("%s : set auto retransmit (0x20) \r\n", setAutoRetran);
    printf ("%s : set RX address length 1-3 \r\n", setRxPw);
    printf ("%s : set TX address length 1-3 \r\n", setTxPw);
    printf ("%s : set RX address \r\n", setRxAddr);
    printf ("%s : set RX payload length \r\n", setRxLen);
    printf ("%s : set RX payload length \r\n", setTxLen);
    printf ("%s : set output clock 0/4/5/6/7 - disable/4MHz/2MHz/1MHz/500KHz \r\n", setUpClk);
    printf ("%s : set oscillator frequency 0/0x8/0x10/0x18/0x20 (4/8/12/16/20MHz)\r\n", setXOF);
    printf ("%s : set CRC 0/0x40/0xc0 (disable/8bit/16bit)\r\n", setXOF);
    printf ("%s : print nRF905 configuration registers\r\n", printCfg);
    printf ("%s : write transmit payload\r\n", writeTxPayload);
    printf ("%s : read transmit payload\r\n", readTxPayload);
    printf ("%s : start shock burst transmit\r\n", startSckBrst);

    printf ("%s : set TX Adress\r\n", setTxAddr);
    printf ("%s : print this help\r\n", help);
}


