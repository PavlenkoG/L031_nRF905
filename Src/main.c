#include "main.h"
#include "stm32l0xx_hal.h"

#include <stdio.h>
#include <stdlib.h>
static void SystemClock_Config(void);
static void Error_Handler(void);

struct nRF905_dev nRF905dev;
uint8_t tx[] = "Hello x";
uint8_t nRF905_Read (uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
uint8_t nRF905_Write (uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);

void main (void) {

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
	nRF905dev.nRF905_HandleTypeDef.nRF905_rx_addr_size = NRF905_ADDR_SIZE_4;
	nRF905dev.nRF905_HandleTypeDef.nRF905_tx_addr_size = NRF905_ADDR_SIZE_4;
	nRF905dev.nRF905_HandleTypeDef.nRF905_rx_pw = 7;
	nRF905dev.nRF905_HandleTypeDef.nRF905_tx_pw = 7;
	nRF905dev.nRF905_HandleTypeDef.nRF905_rx_addr = 0x12345678;
	nRF905dev.nRF905_HandleTypeDef.nRF905_outclk = NRF905_OUTCLK_DISABLE;
	nRF905dev.nRF905_HandleTypeDef.nRF905_xof_freq = NFR905_XOF_16MHZ;
	nRF905dev.nRF905_HandleTypeDef.nRF905_crc = NRF905_CRC_DISABLE;
	nRF905dev.nRF905_HandleTypeDef.nRF905_tx_addr = 0x12345678;

	nRF905dev.read = nRF905_Read;
	nRF905dev.write = nRF905_Write;


    nRF905_init(&nRF905dev);
    nRF905_getConfigRegisters(&nRF905dev);
    nRF905_printConfig(&nRF905dev);

    __HAL_USART_ENABLE_IT(&usart2,USART_IT_RXNE);

    while (1) {
    	tx[6] = cnt;
    	if (cnt < 0x39) {
    		cnt ++;
    	} else {
    		cnt = 0x30;
    	}
//    	nRF905_sendData(&nRF905dev,&tx[0],sizeof(tx) - 1);
    	HAL_Delay(1000);
    }

}

void HAL_USART_RxCpltCallback(USART_HandleTypeDef *UartHandle)
{
	HAL_Delay(1);
	//HAL_USART_Transmit(&usart2, UartHandle->pRxBuffPtr, 1, 0xFFFF);
}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSI Oscillator (16 MHz)*/
  RCC_OscInitStruct.OscillatorType    = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState          = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue  = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState      = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Select HSI as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    Error_Handler();
  }

}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

int __io_putchar(int ch) {
    HAL_USART_Transmit(&usart2, (uint8_t *) &ch, 1, 0x5000);
    return ch;
}

uint8_t nRF905_Read (uint8_t *pCmd, uint8_t *pData, uint16_t Size) {
	uint8_t status;
    NRF905_Select();
    HAL_SPI_TransmitReceive(&spi_nrf,pCmd,&status,1,5000);
    HAL_SPI_Receive(&spi_nrf,pData,Size,5000);
    NRF905_Unselect();
    return status;
}

uint8_t nRF905_Write (uint8_t *pCmd, uint8_t *pData, uint16_t Size) {
	uint8_t status;
    NRF905_Select();
    HAL_SPI_TransmitReceive(&spi_nrf,pCmd,&status,1,5000);
    HAL_SPI_Transmit(&spi_nrf,pData,Size,5000);
    NRF905_Unselect();
    return status;
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
