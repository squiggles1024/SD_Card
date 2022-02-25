
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
 ******************************************************************************/
#include "common.h"
#include "gpio.h"
#include "gpio_config.h"
#include "stm32f4xx_hal.h"

/******************************************************************************
* Public Function Definitions
*******************************************************************************/
/*******************************************************************************
  * @brief  Initializes low level hardware required for GPIO peripheral and clocks
  * @retval None
  ******************************************************************************/
void GPIO_Init(void)
{
  GPIO_InitTypeDef LED_InitStruct = {
    .Pin = (RED_LED_PIN | BLUE_LED_PIN | GREEN_LED_PIN),
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Speed = GPIO_SPEED_LOW,
    .Alternate = 0,
    .Pull = GPIO_NOPULL
  };

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_Init(LED_GPIO_PORT, &LED_InitStruct);
  
}

