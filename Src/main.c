#include "main.h"

#include <stdio.h>
#include <stdlib.h>
static void SystemClock_Config(void);
static void Error_Handler(void);

struct nRF905_dev nRF905dev;
struct userCmd_t usercmd;
uint8_t txBuffer[32];
uint8_t rxBuffer[32];
uint8_t nRF905_Read(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
uint8_t nRF905_Write(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);

void main(void) {

    uint8_t cnt = 0x30;
    HAL_Init();

    /* Configure the System clock to have a frequency of 2 MHz (Up to 32MHZ possible) */
    SystemClock_Config();
    DEBUG_USART_Init();
    SPI_nRF905_Init();
    __HAL_SPI_ENABLE(&spi_nrf);

    nRF905dev.nRF905_HandleTypeDef.ChannelNum = 0x75;
    nRF905dev.nRF905_HandleTypeDef.nRF905_band = NRF905_BAND_868;
    nRF905dev.nRF905_HandleTypeDef.nRF905_pwr = NRF905_PWR_n10;
    nRF905dev.nRF905_HandleTypeDef.nRF905_low_rx = NRF905_LOW_RX_DISABLE;
    nRF905dev.nRF905_HandleTypeDef.nRF905_auto_retran = NRF905_AUTO_RETRAN_DISABLE;
    nRF905dev.nRF905_HandleTypeDef.nRF905_rx_addr_size = NRF905_ADDR_SIZE_3;
    nRF905dev.nRF905_HandleTypeDef.nRF905_tx_addr_size = NRF905_ADDR_SIZE_3;
    nRF905dev.nRF905_HandleTypeDef.nRF905_rx_pw = 7;
    nRF905dev.nRF905_HandleTypeDef.nRF905_tx_pw = 7;
    nRF905dev.nRF905_HandleTypeDef.nRF905_rx_addr = 0x0031fab6;
    nRF905dev.nRF905_HandleTypeDef.nRF905_outclk = NRF905_OUTCLK_DISABLE;
    nRF905dev.nRF905_HandleTypeDef.nRF905_xof_freq = NFR905_XOF_16MHZ;
    nRF905dev.nRF905_HandleTypeDef.nRF905_crc = NRF905_CRC_DISABLE;
    nRF905dev.nRF905_HandleTypeDef.nRF905_tx_addr = 0x0031fab6;

    nRF905dev.read = nRF905_Read;
    nRF905dev.write = nRF905_Write;

    nRF905_init(&nRF905dev);
    printf("STM32L031K6 eval board \r\n");
    printf("NRF905 module detected\r\n");
    nRF905_getConfigRegisters(&nRF905dev);
    nRF905_printConfig(&nRF905dev);

    __HAL_UART_ENABLE_IT(&usart2, UART_IT_RXNE);

    while (1) {
        if (stringReceived) {
            commandHandler(&usercmd, usart2.pRxBuffPtr,stringReceived);
            processUserCommand(&usercmd, &nRF905dev);
            stringReceived = 0;
        }
    }

}
/*
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
 {
 HAL_UART_Transmit(&usart2, "a", 1, 0x5000);
 }
 */

static void SystemClock_Config(void) {
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSI Oscillator (16 MHz)*/
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        /* Initialization Error */
        Error_Handler();
    }

    /* Select HSI as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }

}
/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void) {
    /* User may add here some code to deal with this error */
    while (1) {
    }
}

int __io_putchar(int ch) {
    HAL_UART_Transmit(&usart2, (uint8_t*) &ch, 1, 0x5000);
    return ch;
}

uint8_t nRF905_Read(uint8_t *pCmd, uint8_t *pData, uint16_t Size) {
    uint8_t cnt=0;
    NRF905_Select();
    HAL_SPI_TransmitReceive(&spi_nrf, pCmd, &cnt, 1, 5000);
    cnt = 0;
    if (Size) {
		HAL_SPI_Receive(&spi_nrf, pData, Size, 5000);
		cnt = Size;
    } else {
    	while (HAL_GPIO_ReadPin(nRF905_DR_PORT, nRF905_DR)) {
			HAL_SPI_Receive(&spi_nrf, pData, 1, 1000);
			cnt++;
			pData++;
    	}
    }
    NRF905_Unselect();
    return cnt;
}

uint8_t nRF905_Write(uint8_t *pCmd, uint8_t *pData, uint16_t Size) {
    uint8_t status;
    NRF905_Select();
    HAL_SPI_TransmitReceive(&spi_nrf, pCmd, &status, 1, 5000);
    HAL_SPI_Transmit(&spi_nrf, pData, Size, 5000);
    NRF905_Unselect();
    return status;
}

void processUserCommand (struct userCmd_t *userCmd, struct nRF905_dev *nRF905) {
    switch (userCmd->command) {
    case PRINT_CONFIG:
        nRF905_printConfig(nRF905);
        break;
    case CHANNEL_SET:
        if (userCmd->argCount) {
            nRF905_setChannel(nRF905,(uint16_t)userCmd->args[0]);
        } else {
            printf ("wrong argument \r\n");
        }
        break;
    case BAND_SET:
        if (userCmd->args[0] == 0 || userCmd->args[0] == 2) {
            nRF905_setBand(nRF905,(uint8_t)userCmd->args[0]);
        } else {
            printf ("wrong argument \r\n");
        }
        break;
    case SET_PWR:
        nRF905_setTransmitPower(nRF905,(uint8_t)userCmd->args[0]);
        break;
    case SET_REDUCED_RX:
        nRF905_setLowRxPower(nRF905,(uint8_t)userCmd->args[0]);
        break;
    case SET_AUTO_RETRAN:
        nRF905_setAutoRetransmit(nRF905,(uint8_t)userCmd->args[0]);
        break;
    case SET_RX_ADDR:
        nRF905_setRXAddress(nRF905,userCmd->args[0]);
        break;
    case SET_RX_ADDR_LEN:
        nRF905_setRxAddressSize(nRF905,(uint8_t)userCmd->args[0]);
        break;
    case SET_TX_ADDR_LEN:
        nRF905_setTxAddressSize(nRF905,(uint8_t)userCmd->args[0]);
        break;
    case SET_RX_PAYLOAD_LEN:
        nRF905_setRxPayloadSize(nRF905,(uint8_t)userCmd->args[0]);
        break;
    case SET_TX_PAYLOAD_LEN:
        nRF905_setTxPayloadSize(nRF905,(uint8_t)userCmd->args[0]);
        break;
    case SET_UP_CLK:
        nRF905_setClockOut(nRF905,(uint8_t)userCmd->args[0]);
        break;
    case SET_XOF:
        nRF905_setCrstallOsc(nRF905,(uint16_t)userCmd->args[0]);
        break;
    case SET_CRC:
        nRF905_setCRC(nRF905,(uint8_t)userCmd->args[0]);
        break;
    case SET_TX_ADDR:
        nRF905_setTXAddress(nRF905,userCmd->args[0]);
        break;
    case SET_TX_PAYLOAD:
        for (uint8_t i = 0; i < userCmd->argCount; i++) {
            txBuffer[i] = (uint8_t)userCmd->args[i];
        }
        nRF905_setTxPayloadData(nRF905, &txBuffer[0],userCmd->argCount);
        break;
    case GET_TX_PAYLOAD:
        nRF905_getTxPayloadData(nRF905,&txBuffer[0]);
        for (uint8_t i = 0; i < nRF905->nRF905_HandleTypeDef.nRF905_tx_pw; i ++) {
            printf("0x%x ", txBuffer[i]);
        }
            printf("\r\n");
        break;
    case START_SK_BURST:
        nRF905_startShcokBurstTx(nRF905);
        break;
    case START_RX_BURST:
    	nRF905_startShockBurstRx(nRF905);
        break;
    case ERROR_CMD:
        printf ("command error\r\n");
        break;
    case SET_REGISTER:
        break;
    case HELP_CMD:
        printHelp();
        break;
    }
}

void nRF905_BurstRxHandler(struct nRF905_dev *nRF905) {
	uint8_t rxCnt = 0;
	uint8_t i = 0;
	rxCnt = nRF905_getData(nRF905,&rxBuffer[0],0);
	printf("Received packet: ");
	if (rxCnt) {
		for (rxCnt;rxCnt >0;rxCnt--){
            printf("0x%x ", rxBuffer[i]);
            i++;
        }
            printf("\r\n");
	}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
