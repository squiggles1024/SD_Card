/**
 ******************************************************************************
  * @file    user_diskio.c
  * @brief   This file includes a diskio driver skeleton to be completed by the user.
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
#include <string.h>
#include "ff_gen_drv.h"
#include "sd_card.h"


/******************************************************************************
* Function Prototypes
*******************************************************************************/
static DSTATUS SD_CheckStatus(void);
DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);

#if _USE_WRITE == 1
DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */

#if _USE_IOCTL == 1
DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

/******************************************************************************
* Private Global Variables
*******************************************************************************/
static volatile DSTATUS Stat = STA_NOINIT; //Disk Status


/******************************************************************************
* Public Global Variables
*******************************************************************************/
Diskio_drvTypeDef  USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read,

#if  _USE_WRITE
  USER_write,
#endif  /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
  USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/******************************************************************************
* Private Function Definitions
*******************************************************************************/

static DSTATUS SD_CheckStatus()
{
  Stat = STA_NOINIT;

  if(SD_GetCardState() == SD_OK)
  {
    Stat &= ~STA_NOINIT;
  }

  return Stat;
}

/******************************************************************************
* Public Function Definitions
*******************************************************************************/
/*******************************************************************************
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  ******************************************************************************/
DSTATUS USER_initialize (BYTE pdrv){

  Stat = STA_NOINIT;
  if(SD_Init() == SD_OK)
  {
    Stat = SD_CheckStatus();
  }
    return Stat;
}

/*******************************************************************************
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  ******************************************************************************/
DSTATUS USER_status (BYTE pdrv){
  return SD_CheckStatus();
}

/*******************************************************************************
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  ******************************************************************************/
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count){
  DRESULT res = RES_ERROR;

  if(SD_ReadBlocks((uint32_t*)buff, (uint32_t) (sector), count, SD_TIMEOUT) == SD_OK)
  {
    /* wait until the read operation is finished */
    while(SD_GetCardState()!= SD_OK){}
    res = RES_OK;
  }

  return res;
}

/*******************************************************************************
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  ******************************************************************************/

DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector,UINT count){
  DRESULT res = RES_ERROR;

  if(SD_WriteBlocks((uint32_t*)buff, (uint32_t)(sector), count, SD_TIMEOUT) == SD_OK)
  {
	/* wait until the Write operation is finished */
    while(SD_GetCardState() != SD_OK){}
    res = RES_OK;
  }

  return res;
}

/*******************************************************************************
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  ******************************************************************************/
#if _USE_IOCTL == 1
DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff){
  DRESULT res = RES_ERROR;
  SD_CardInfo CardInfo;

  if (Stat & STA_NOINIT) return RES_NOTRDY;

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
    SD_GetCardInfo(&CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockNbr;
    res = RES_OK;
    break;

  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    SD_GetCardInfo(&CardInfo);
    *(WORD*)buff = CardInfo.LogBlockSize;
    res = RES_OK;
    break;

  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    SD_GetCardInfo(&CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockSize / SD_DEFAULT_BLOCK_SIZE;
	res = RES_OK;
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}
#endif /* _USE_IOCTL == 1 */

