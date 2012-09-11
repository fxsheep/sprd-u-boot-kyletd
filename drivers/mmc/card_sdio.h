#include "asm/arch/sci_types.h"

#ifndef __SD_SDIO_20_H__
#define __SD_SDIO_20_H__

struct CARD_PORT_TAG;
typedef struct CARD_PORT_TAG* CARD_SDIO_HANDLE;

/*****************************************************************************/
//  Description:  Get Card operation handle,after we get the handle ,we can get next operation way of card
//  Author: Jason.wu
//  Param
//		host_slot: slot number
//  Return:
//		Not zero: success
//		zero : fail
//  Note: 
/*****************************************************************************/
typedef enum
{
	CARD_SDIO_SLOT_0,
	CARD_SDIO_SLOT_1,
	CARD_SDIO_SLOT_2,
	CARD_SDIO_SLOT_3,
	CARD_SDIO_SLOT_4,
	CARD_SDIO_SLOT_5,
	CARD_SDIO_SLOT_6,
	CARD_SDIO_SLOT_7,
	CARD_SDIO_SLOT_MAX
}
CARD_SDIO_SLOT_NO;

typedef enum
{
	NORMAL_SPEED_MODE,
	HIGH_SPEED_MODE,
	SPEED_MODE_MAX
}
CARD_SPEED_MODE;

typedef enum CARD_EMMC_PARTITION_TPYE_TAG
{
	PARTITION_USER,
	PARTITION_BOOT1,
	PARTITION_BOOT2,
	PARTITION_RPMB,
	PARTITION_GENERAL_P1,
	PARTITION_GENERAL_P2,
	PARTITION_GENERAL_P3,
	PARTITION_GENERAL_P4,
	PARTITION_MAX
}CARD_EMMC_PARTITION_TPYE;

PUBLIC uint32 Emmc_GetCapacity(CARD_EMMC_PARTITION_TPYE cardPartiton);


PUBLIC CARD_SDIO_HANDLE CARD_SDIO_Open(CARD_SDIO_SLOT_NO slotNo);
/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void CARD_SDIO_Close(CARD_SDIO_HANDLE cardHandle);
/*****************************************************************************/
//  Description:  Turn on or turn off the card 
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		switchCtl: TRUE, turn on the card ,FALSE, turn off the card
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void CARD_SDIO_PwrCtl(CARD_SDIO_HANDLE cardHandle,BOOLEAN switchCtl);
/*****************************************************************************/
//  Description:  Initialize card, change card from idle state to standy by state ,and get some information from card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		speedmode: HighSpeed mode or normal speed mode
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_InitCard(CARD_SDIO_HANDLE cardHandle, CARD_SPEED_MODE speedmode);
/*****************************************************************************/
//  Description:  Set block length of card.  the data will be transmited in block unit
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		length: block length
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_SetBlockLength(CARD_SDIO_HANDLE cardHandle,uint32 length);
/*****************************************************************************/
//  Description:  Read single block data from card , unit is block
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		BlockId: block number that you want to read
//		buf : that data read from card will be stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_ReadSingleBlock(CARD_EMMC_PARTITION_TPYE  cardPartiton, CARD_SDIO_HANDLE cardHandle,uint32 BlockId,uint8* buf);
/*****************************************************************************/
//  Description:  Read multi block data from card , unit is block
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		startBlock:  start block number that you want to read
//		num: the number of block you want to read
//		buf : that data read from card will be stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_ReadMultiBlock(CARD_EMMC_PARTITION_TPYE  cardPartiton, CARD_SDIO_HANDLE cardHandle,uint32 startBlock,uint32 num,uint8* buf);
/*****************************************************************************/
//  Description:  Wrtie single block data to card , unit is block
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		BlockId: block number that you want to write
//		buf : that data to be writen to card is stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_WriteSingleBlock(CARD_EMMC_PARTITION_TPYE  cardPartiton, CARD_SDIO_HANDLE cardHandle,uint32 BlockId,uint8* buf);
/*****************************************************************************/
//  Description:  Wrtie multi block data to card , unit is block
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		startBlock: start block number that you want to write
//		num: the number of block you want to write
//		buf : the data to be writen to card is stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_WriteMultiBlock(CARD_EMMC_PARTITION_TPYE  cardPartiton, CARD_SDIO_HANDLE cardHandle,uint32 startBlock,uint32 num,uint8* buf);
/*****************************************************************************/
//  Description:  erase card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		startBlock: start block number that you want to erase
//		endBlock : end block number that you want to erase
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_Erase(CARD_SDIO_HANDLE cardHandle,uint32 startBlock,uint32 endBlock);
/*****************************************************************************/
//  Description:  Get total block number of card ,block length is fix to 512 byte
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//  Return:
//		uint32 value : total block num
//  Note: 
/*****************************************************************************/
PUBLIC uint32 CARD_SDIO_GetCapacity(CARD_SDIO_HANDLE cardHandle);

PUBLIC BOOLEAN CARD_SDIO_EnterMcex(CARD_SDIO_HANDLE cardHandle,BOOLEAN* ifEnterMcex );

PUBLIC BOOLEAN CARD_SDIO_SendPsi(CARD_SDIO_HANDLE cardHandle,uint32 arg,uint8* buf);

PUBLIC BOOLEAN CARD_SDIO_ReadSecCmd(CARD_SDIO_HANDLE cardHandle,uint8* buf);

PUBLIC BOOLEAN CARD_SDIO_WriteSecCmd(CARD_SDIO_HANDLE cardHandle,uint8* buf);

PUBLIC BOOLEAN CARD_SDIO_CtlTrm(CARD_SDIO_HANDLE cardHandle);

PUBLIC BOOLEAN CARD_SDIO_ExitMcex(CARD_SDIO_HANDLE cardHandle);

PUBLIC BOOLEAN CARD_SDIO_IsEnterHighSpeed(CARD_SDIO_HANDLE cardHandle);

PUBLIC BOOLEAN Emmc_Init(void);

PUBLIC BOOLEAN Emmc_Write(CARD_EMMC_PARTITION_TPYE  cardPartiton, uint32 startBlock,uint32 num,uint8* buf);

PUBLIC BOOLEAN Emmc_Erase(CARD_EMMC_PARTITION_TPYE cardPartiton, uint32 startBlock,uint32 num);

PUBLIC BOOLEAN Emmc_Read(CARD_EMMC_PARTITION_TPYE  cardPartiton, uint32 startBlock,uint32 num,uint8* buf);

PUBLIC uint32 Emmc_GetCapacity(CARD_EMMC_PARTITION_TPYE cardPartiton);



#endif





