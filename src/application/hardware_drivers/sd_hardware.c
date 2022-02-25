/******************************************************************************
* Includes
*******************************************************************************/
#include "sd_hardware.h"
#include "stm32f4xx_hal.h"
#include "spi.h"


/******************************************************************************
* Private Global Variables
*******************************************************************************/
static const uint8_t SD_DUMMY_BYTE = 0xFF;

/******************************************************************************
* Private Function Declarations
*******************************************************************************/
static void SD_Error(void);


/******************************************************************************
* Public Function Definitions
*******************************************************************************/
/*******************************************************************************
  * @brief  Initializes low level hardware required for SD Card interface (SPI, GPIO pins, Clocks)
  * @retval None
  ******************************************************************************/
void SD_IO_Init(void)
{
  uint8_t counter;

  SD_SpiInit();
  
  /* SD chip select high */
  SD_IO_CSState(1);
  
  //See "initialization procedure for SPI Mode" @ http://elm-chan.org/docs/mmc/mmc_e.html
  /* Send dummy byte 0xFF, 10 times with CS high */
  /* Rise CS and MOSI for 80 clocks cycles */
  for (counter = 0; counter <= 9; counter++)
  {
    /* Send dummy byte 0xFF */
    SD_IO_WriteByte(SD_DUMMY_BYTE);
  }
}

/*******************************************************************************
  * @brief  Sets SD Card CSN Pin to desired state
  * @param  val: value (1 or 0) to set CSN pin to.
  * @retval None
  ******************************************************************************/
void SD_IO_CSState(uint8_t val)
{
  if(val == 1) 
  {
    HAL_GPIO_WritePin(SD_CSN_GPIO_Port,SD_CSN_Pin,GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(SD_CSN_GPIO_Port,SD_CSN_Pin,GPIO_PIN_RESET);
  }
}

/*******************************************************************************
  * @brief  Writes and Reads block of data from SD Card
  * @param  DataIn: Data to be transmitted
  * @param  DataOut: Data reception buffer
  * @param  DataLength: Number of bytes to send/receive
  * @retval None
  ******************************************************************************/
void SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_TransmitReceive(&SD_SPI_HANDLE, (uint8_t*) DataIn, DataOut, DataLength, SPI_TIMEOUT);
    
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SD_Error();
  }
}

/*******************************************************************************
  * @brief  Writes a single byte to SD Card
  * @param  Data: Byte to be transmitted
  * @retval dummy: meaningless. Required for compatibility with Adafruit SD Card Driver
  ******************************************************************************/
uint8_t SD_IO_WriteByte(uint8_t Data){
    uint8_t dummy;
    SD_IO_WriteReadData(&Data, &dummy, 1);
    return dummy;
}

/*******************************************************************************
  * @brief  DeInitializes Peripherals required for SD Card
  * @retval None
  ******************************************************************************/
void SD_IO_DeInit(void){
  HAL_SPI_DeInit(&SD_SPI_HANDLE);
}

/******************************************************************************
* Private Function Definitions
*******************************************************************************/
static void SD_Error(void)
{
  /* De-initialize the SPI communication BUS */
  SD_IO_DeInit();
  
  /* Re-Initiaize the SPI communication BUS */
  SD_IO_Init();
}

