/******************************************************************************
* Includes
*******************************************************************************/
#include "gpio.h"
#include "rcc.h"
#include "fatfs.h"
#include "common.h"
#include "stm32f4xx_hal.h"


/******************************************************************************
* Public Function Definitions
*******************************************************************************/
int main(){

    FRESULT res;                                                /* FatFs function common result code */
    uint32_t byteswritten, bytesread;                           /* File write/read counts */
    uint8_t wtext[] = "This is STM32 working with FatFs. Test"; /* File write buffer */
    uint8_t rtext[100];                                         /* File read buffer */

    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    FATFS_Init();

    if(f_mount(&USERFatFS, (TCHAR const*)USERPath, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      HAL_GPIO_WritePin(LED_GPIO_PORT, RED_LED_PIN, GPIO_PIN_SET);
      Error_Handler();
    }

    if(f_open(&USERFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) 
    {
        /* 'STM32.TXT' file Open for write Error */
        HAL_GPIO_WritePin(LED_GPIO_PORT, RED_LED_PIN, GPIO_PIN_SET);
        Error_Handler();
    }

    res = f_write(&USERFile, wtext, sizeof(wtext), (void *)&byteswritten);

    if((byteswritten == 0) || (res != FR_OK))
    {
            /* 'STM32.TXT' file Write or EOF Error */
        HAL_GPIO_WritePin(LED_GPIO_PORT, RED_LED_PIN, GPIO_PIN_SET);
        Error_Handler();
    }

    f_close(&USERFile);

    res = f_read(&USERFile, rtext, sizeof(rtext), (UINT*)&bytesread);

    f_close(&USERFile);


    while(1){
        HAL_GPIO_TogglePin(LED_GPIO_PORT, GREEN_LED_PIN);
        HAL_Delay(1000);
    }
}
