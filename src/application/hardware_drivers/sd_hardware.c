/******************************************************************************
* Includes
*******************************************************************************/
#include "sd_hardware.h"
#include "stm32f4xx_hal.h"


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

