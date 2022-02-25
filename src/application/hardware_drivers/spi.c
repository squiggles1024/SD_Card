/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"
#include "spi.h"
#include "sd_hardware.h"

/******************************************************************************
* Public Global Variables
*******************************************************************************/
SPI_HandleTypeDef hspi1;

/******************************************************************************
* Public Function Definitions
*******************************************************************************/

void SD_SpiInit(void){

  SD_SPI_CLK_ENABLE();
  SD_SPI_HANDLE.Instance = SD_SPI_PERIPHERAL;
  SD_SPI_HANDLE.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  SD_SPI_HANDLE.Init.CLKPhase = SPI_PHASE_2EDGE;
  SD_SPI_HANDLE.Init.CLKPolarity = SPI_POLARITY_HIGH;
  SD_SPI_HANDLE.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  SD_SPI_HANDLE.Init.CRCPolynomial = 0x7;
  SD_SPI_HANDLE.Init.DataSize = SPI_DATASIZE_8BIT;
  SD_SPI_HANDLE.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SD_SPI_HANDLE.Init.Mode = SPI_MODE_MASTER;
  SD_SPI_HANDLE.Init.NSS = SPI_NSS_SOFT;
  SD_SPI_HANDLE.Init.TIMode = SPI_TIMODE_DISABLED;
  
  /*Intialized SPI Peripheral*/
  if (HAL_SPI_Init(&SD_SPI_HANDLE) != HAL_OK)
  {
    Error_Handler();
  }
}

/*******************************************************************************
  * @brief  Initializes low level hardware required for SPI peripheral (GPIO, Clocks)
  * @param  spiHandle: Handle of the SPI Peripheral being initialized. See STM32 HAL Docs for more detail.
  * @retval None
  ******************************************************************************/
void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SD_SPI_PERIPHERAL)
  {

  SD_CSN_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin = SD_CSN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SD_CSN_GPIO_Port, &GPIO_InitStruct);

  SD_SPI_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin = SD_SCK_PIN | SD_MOSI_PIN | SD_MISO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = SD_SPI_SCK_AF;
  HAL_GPIO_Init(SD_SPI_GPIO_Port, &GPIO_InitStruct);
  }
}

/*******************************************************************************
  * @brief  DeInitializes low level hardware required for SPI peripheral (GPIO, Clocks)
  * @param  spiHandle: Handle of the SPI Peripheral being Deinitialized. See STM32 HAL Docs for more detail.
  * @retval None
  ******************************************************************************/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {

    __HAL_RCC_SPI1_CLK_DISABLE();

    HAL_GPIO_DeInit(SD_SPI_GPIO_Port, SD_SCK_PIN | SD_MOSI_PIN | SD_MISO_PIN);

  }
}

