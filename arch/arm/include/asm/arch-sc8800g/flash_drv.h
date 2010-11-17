/******************************************************************************
 ** File Name:     flash_drv.h                                                 *
 ** Description:                                                              *
 **        norflash operations                                                *
 ** Author:         Younger.yang                                              *
 ** DATE:           11/23/2006                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/23/2006      Younger.yang       Create for Jiuyang                     *
 ******************************************************************************/


#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Macro defines
 **---------------------------------------------------------------------------*/


#define  BIG_ENDIAN    1


#define SR_0      0x1
#define SR_1      0x2
#define SR_2      0x4
#define SR_3      0x8
#define SR_4      0x10
#define SR_5      0x20
#define SR_6      0x40
#define SR_7      0x80

/**---------------------------------------------------------------------------*
 **                         Functions
 **---------------------------------------------------------------------------*/


/**********************************************
* NorFDL_FlashReadWord
***********************************************/
uint16 NorFDL_FlashReadWord (uint32 addr);


/**********************************************
* NorFDL_FlashRead
***********************************************/
uint32 NorFDL_FlashRead (uint32 addr, uint8 *buf, uint32 read_len);


/**********************************************
* NorFDL_FlashWriteWord
***********************************************/
int  NorFDL_FlashWriteWord (uint32 addr, uint16 data);


/**********************************************
* flash_write
***********************************************/
int NorFDL_FlashWrite (uint32 addr, uint8 *buf,  uint32 len);


/**********************************************
* NorFDL_FlashEraseBlock
***********************************************/
int NorFDL_FlashEraseBlock (uint32 addr);


/**********************************************
* NorFDL_FlashErase
***********************************************/
uint32 NorFDL_FlashErase (uint32 start_addr,  uint32 size);


/**********************************************
* NorFDL_FlashEraseChip
***********************************************/
uint32 NorFDL_FlashEraseChip (void);


/**********************************************
* NorFDL_FlashWriteEnable
***********************************************/
uint32 NorFDL_FlashWriteEnable (uint32 start_addr, uint32  enable);


/**********************************************
* NorFDL_FlashWriteArea
***********************************************/
uint32 NorFDL_FlashWriteArea (uint32 addr, char *buf, uint32 size);


/**********************************************
* NorFDL_FlashCheckSectorSize
***********************************************/
uint32 NorFDL_FlashCheckSectorSize (uint32 start_addr);


