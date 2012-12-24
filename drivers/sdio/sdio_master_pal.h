

#ifndef _SDIO_MASTER_PAL_H_
#define _SDIO_MASTER_PAL_H_

/*****************************************************************************/
//  Description:  Get Card operation handle,after we get the handle ,we can get next operation way of card
//  Author: Jason.wu
//  Param
//		slotNo: slot number
//  Return:
//		Not zero: success
//		zero : fail
//  Note: 
/*****************************************************************************/
CARD_SDIO_HANDLE SPRD_SDSlave_Open(int slotNo);

/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other 
//  application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by SPRD_SDSlave_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
void SPRD_SDSlave_Close(CARD_SDIO_HANDLE cardHandle);
/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other 
//  application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by SPRD_SDSlave_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
void SPRD_SDSlave_PwrCtl(CARD_SDIO_HANDLE cardHandle,int switchCtl);
/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other 
//  application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by SPRD_SDSlave_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
void CARD_SDIO_SlotSelect(int slotNo);

int SDIO_SetBusClock(CARD_SDIO_HANDLE cardHandle,int clock);
//-----------------------------------------------------------------------------------
//	Set data bus width of card
//-----------------------------------------------------------------------------------
int SDIO_SetBusWidth(CARD_SDIO_HANDLE cardHandle,SDIO_CARD_PAL_BUFWIDTH_E width);

/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other 
//  application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by SPRD_SDSlave_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
int SDIO_SetBlockLength(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int length);

/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other 
//  application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by SPRD_SDSlave_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
int SPRD_SDSlave_InitCard(CARD_SDIO_HANDLE cardHandle);
/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other 
//  application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by SPRD_SDSlave_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
int SDIO_ReadBytes(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int RegAddr,unsigned int inc_flag,unsigned int num,unsigned char* buf);
/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other 
//  application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by SPRD_SDSlave_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
int SDIO_ReadBlocks(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int RegAddr,unsigned int inc_flag,unsigned int num,unsigned char* buf);
/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other 
//  application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by SPRD_SDSlave_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/int SDIO_WriteBytes(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int RegAddr,unsigned int inc_flag,unsigned int num,unsigned char* buf);
/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other 
//  application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by SPRD_SDSlave_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/int SDIO_WriteBlocks(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int RegAddr,unsigned int inc_flag,unsigned int num,unsigned char* buf);
/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other 
//  application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by SPRD_SDSlave_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
int SDIO_ReadByte(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int RegAddr,unsigned char* buf);
/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other 
//  application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by SPRD_SDSlave_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
int SDIO_WriteByte(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int RegAddr,unsigned char* wbuf,unsigned char* rbuf);
#endif//__SDIO_MASTER_H__





