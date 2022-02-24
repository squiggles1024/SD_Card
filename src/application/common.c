/******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"
#include "cmsis_gcc.h"

/******************************************************************************
* Public Function Definitions
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line) {
      //printf("Assert fail at File %s Line %d", file, (int)line);
      while(1); 
}

void Error_Handler(void) 
{
  __disable_irq();
  while (1)
  {
  }
}