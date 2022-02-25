#ifndef SD_HARDWARE_H_
#define SD_HARDWARE_H_
/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "common.h"

extern SPI_HandleTypeDef sdspi;

/******************************************************************************
* Hardware Defines: Change these to port to another STM32F4 Project
*******************************************************************************/
#define SD_CSN_Pin               GPIO_PIN_4
#define SD_CSN_GPIO_Port         GPIOA

#define SD_SPI_GPIO_Port         GPIOA
#define SD_SCK_PIN               GPIO_PIN_5
#define SD_MOSI_PIN              GPIO_PIN_7
#define SD_MISO_PIN              GPIO_PIN_6

#define SD_SPI_SCK_AF            GPIO_AF5_SPI1
#define SD_SPI_MOSI_AF           GPIO_AF5_SPI1
#define SD_SPI_MISO_AF           GPIO_AF5_SPI1
#define SD_SPI_PERIPHERAL        SPI1

#define SD_CSN_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define SD_SPI_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define SD_SPI_CLK_ENABLE()       __HAL_RCC_SPI1_CLK_ENABLE()
#define SD_CSN_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define SD_SPI_CLK_DISABLE()      __HAL_RCC_SPI1_CLK_DISABLE()
#define SD_BAUD_PRESCALER         SPI_BAUDRATEPRESCALER_8
#define SD_SPI_HANDLE             sdspi
#define SPI_TIMEOUT               (1000U)

/**********************************************************************************
* Public Hardware Functions: Used to link to the FATFS Middleware
***********************************************************************************/
void    SD_IO_Init(void);
void    SD_IO_CSState(uint8_t state);
void    SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength);
uint8_t SD_IO_WriteByte(uint8_t Data);
void    SD_IO_DeInit(void);

#endif
