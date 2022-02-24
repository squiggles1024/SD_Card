  /* @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

#ifndef SD_CARD_H_
#define SD_CARD_H_
#include <stdint.h>

#define SD_DEFAULT_BLOCK_SIZE (512U)

typedef enum{
    SD_OK = 0,
    SD_ERROR = 1,
    SD_TIMEOUT = 2,
    SD_DATA_OK = 5
}SdCardStatus_t;

typedef struct{
  uint8_t  Reserved1         :2;      /* Reserved */
  uint16_t DeviceSize        :12;     /* Device Size */
  uint8_t  MaxRdCurrentVDDMin:3;      /* Max. read current @ VDD min */
  uint8_t  MaxRdCurrentVDDMax:3;      /* Max. read current @ VDD max */
  uint8_t  MaxWrCurrentVDDMin:3;      /* Max. write current @ VDD min */
  uint8_t  MaxWrCurrentVDDMax:3;      /* Max. write current @ VDD max */
  uint8_t  DeviceSizeMul     :3;      /* Device size multiplier */
} struct_v1;

typedef struct{
  uint8_t  Reserved1         :6;      /* Reserved */
  uint32_t DeviceSize        :22;     /* Device Size */
  uint8_t  Reserved2         :1;      /* Reserved */
} struct_v2;

/** 
  * @brief  Card Specific Data: CSD Register. See SD Card Physical Layer SimplifiedSpecification section 5.3.2
  */ 
typedef struct{
  /* Header part */
  uint8_t  CSDStruct         :2;        /* CSD structure */
  uint8_t  Reserved1         :6;        /* Reserved */
  uint8_t  TAAC              :8;        /* Data read access-time 1 */
  uint8_t  NSAC              :8;        /* Data read access-time 2 in CLK cycles */
  uint8_t  MaxBusClkFrec     :8;        /* Max. bus clock frequency */
  uint16_t CardComdClasses   :12;       /* Card command classes */
  uint8_t  RdBlockLen        :4;        /* Max. read data block length */
  uint8_t  PartBlockRead     :1;        /* Partial blocks for read allowed */
  uint8_t  WrBlockMisalign   :1;        /* Write block misalignment */
  uint8_t  RdBlockMisalign   :1;        /* Read block misalignment */
  uint8_t  DSRImpl           :1;        /* DSR implemented */
  
  /* v1 or v2 struct */
  union csd_version {
    struct_v1 v1;
    struct_v2 v2;
  } version;
  
  uint8_t  EraseSingleBlockEnable:1;         /* Erase single block enable */
  uint8_t  EraseSectorSize       :7;         /* Erase group size multiplier */
  uint8_t  WrProtectGrSize       :7;         /* Write protect group size */
  uint8_t  WrProtectGrEnable     :1;         /* Write protect group enable */
  uint8_t  Reserved2             :2;         /* Reserved */
  uint8_t  WrSpeedFact           :3;         /* Write speed factor */
  uint8_t  MaxWrBlockLen         :4;         /* Max. write data block length */
  uint8_t  WriteBlockPartial     :1;         /* Partial blocks for write allowed */
  uint8_t  Reserved3             :5;         /* Reserved */
  uint8_t  FileFormatGrouop      :1;         /* File format group */
  uint8_t  CopyFlag              :1;         /* Copy flag (OTP) */
  uint8_t  PermWrProtect         :1;         /* Permanent write protection */
  uint8_t  TempWrProtect         :1;         /* Temporary write protection */
  uint8_t  FileFormat            :2;         /* File Format */
  uint8_t  Reserved4             :2;         /* Reserved */
  uint8_t  crc                   :7;         /* Reserved */
  uint8_t  Reserved5             :1;         /* always 1*/

} SD_CSD;

typedef struct
{
  volatile uint8_t  ManufacturerID;       /* ManufacturerID */
  volatile uint16_t OEM_AppliID;          /* OEM/Application ID */
  volatile uint32_t ProdName1;            /* Product Name part1 */
  volatile uint8_t  ProdName2;            /* Product Name part2*/
  volatile uint8_t  ProdRev;              /* Product Revision */
  volatile uint32_t ProdSN;               /* Product Serial Number */
  volatile uint8_t  Reserved1;            /* Reserved1 */
  volatile uint16_t ManufactDate;         /* Manufacturing Date */
  volatile uint8_t  CID_CRC;              /* CID CRC */
  volatile uint8_t  Reserved2;            /* always 1 */
} SD_CID;

/** 
  * @brief SD Card information 
  */
typedef struct
{
  SD_CSD Csd;
  SD_CID Cid;
  uint32_t CardCapacity;              /*!< Card Capacity */
  uint32_t CardBlockSize;             /*!< Card Block Size */
  uint32_t LogBlockNbr;               /*!< Specifies the Card logical Capacity in blocks   */
  uint32_t LogBlockSize;              /*!< Specifies logical block size in bytes           */
} SD_CardInfo;

#define SD_BLOCK_SIZE    0x200

/**
  * @brief  SD detection on its memory slot
  */
#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)

#define SD_DATATIMEOUT           ((uint32_t)100000000)

SdCardStatus_t SD_Init(void);
SdCardStatus_t SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
SdCardStatus_t SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);
SdCardStatus_t SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
SdCardStatus_t SD_GetCardState(void);
SdCardStatus_t SD_GetCardInfo(SD_CardInfo *pCardInfo);
   
/* Link functions for SD Card peripheral*/


#endif