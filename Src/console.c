
#include "console.h"
#include <string.h>
#include "main.h"

#define USER_ARGUMENTS 2

uint8_t setChannelCmd[] = "$CHSET";
uint8_t setBandCmd[] = "$BNSET";

typedef struct userCmd_t {
	nRF905_registers_t command;
	uint8_t argCount;
	uint8_t args[USER_ARGUMENTS];
} userCmd;

void commandHandler (uint8_t *rxBuffer, uint8_t len) {
	uint8_t data_present;
	uint8_t data;
	uint8_t lastUserData;
	int32_t msgNumberSign;

	userCmd usercmd;
	usercmd.command = 0;
	usercmd.argCount = 0;

	if (!(strncmp((char *) (rxBuffer), setChannelCmd, 6))) {
		data_present = 1;
	} else if (!(strncmp((char *) (rxBuffer), setBandCmd, 6))){
	}

	rxBuffer = rxBuffer + 6;
	if (data_present) {

		lastUserData = *rxBuffer;
		rxBuffer++;
		data = *rxBuffer;
		while (len) {
			if (((data >= '0' && data <= '9') || data == '-') && lastUserData == ' ') {
				usercmd.argCount ++;
				if (usercmd.argCount >= USER_ARGUMENTS) {
					//error
				} else {
					usercmd.args[usercmd.argCount - 1] = 0;
					if (data == '-') {
						msgNumberSign = -1;
					} else {
						msgNumberSign = 1;
						usercmd.args[usercmd.argCount - 1] = (data - '0');
					}
				}
			} else if (data >= '0' && data <= '9') {
				usercmd.args[usercmd.argCount - 1] = usercmd.args[usercmd.argCount - 1] * 10 + (data - '0');
			}
			if (len == 1) {
				if (lastUserData >= '0' && lastUserData <= '9') {
					if (usercmd.argCount > 0)
						usercmd.args[usercmd.argCount - 1] *= msgNumberSign;
				}
			}

			lastUserData = *rxBuffer;
			rxBuffer++;
			data = *rxBuffer;
			len--;
		}
	}
}
