### uNucleo Connector
                    
Function | Port | CN3 Pin |-| CN4 Pin |Port| Function
----------|------|-----------|-|----------|----|----------
USART1_TX|PA9|1|-|1|VIN|-
UART1_RX|PA10|2|-|2|GND|-
-|NRST|3|-|3|RESET|-
nRF905 GND|GND|4|-|4|+5V|-
-|PA12|5|-|5|PA2|DBG_USART_TX
-|PB0|6|-|6|PA7|-
-|PB7|7|-|7|PA6|nRF905 DR
-|PB6|8|-|8|PA5|nRF905 AM
-|PB1|9|-|9|PA4|nRF905 CD
-|PF0|10|-|10|PA3|nRF905 TRXCE
-|PF1|11|-|11|PA1|nRF905 TXE
-|PA8|12|-|12|PA0|nRF905 PWR
SPI1_CS|PA11|13|-|13|AVDD|-
SPI1_MOSI|PB5|14|-|14|+3V3|nRF905 VCC
SPI1_MISO|PB4|15|-|15|PB3|SPI1_SCK


![](https://github.com/PavlenkoG/L031_nRF905/blob/master/Doc/uNucleo_with_nRF905.png)