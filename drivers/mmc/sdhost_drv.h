
#ifndef __SDIO_H_
#define __SDIO_H_

#include "asm/arch/sci_types.h"

#define OS_NONE

//---0x2050_0028---Host Control &Power Control & Block Gap Control & Wakeup Control Register

typedef enum SDHOST_VOL_RANGE_E_TAG
{
	VOL_1_8 = 180,	// 1.8 v
	VOL_2_65 = 265,	// 2.65 v
	VOL_2_8 = 280,	// 2.8 v
	VOL_3_0 = 300,	// 3.0 v
	VOL_RES
}
SDHOST_VOL_RANGE_E;

typedef enum SDHOST_PWR_ONOFF_E_TAG
{
	POWR_ON = 0,
	POWR_OFF
}
SDHOST_PWR_ONOFF_E;

typedef enum SDHOST_BIT_WIDTH_E_TAG
{
	SDIO_1BIT_WIDTH,
	SDIO_4BIT_WIDTH,
	SDIO_8BIT_WIDTH
}
SDHOST_BIT_WIDTH_E;

typedef enum SDHOST_SPEED_E_TAG
{
#if defined (CONFIG_SC8825) || defined(CONFIG_SC7710G2)
	EMMC_SDR12,
	EMMC_SDR25,
	EMMC_SDR50,
	EMMC_SDR104,
	EMMC_DDR50
#else
	SDIO_HIGHSPEED,	// high speed mode ,some card support this mode,clock rate can be up to 50MHz
	SDIO_LOWSPEED		// Normal speed mode , all the card support this mode default, clock rate can be up to 25MHz
#endif
}
SDHOST_SPEED_E;

typedef enum SDHOST_LED_ONOFF_E_TAG
{
	SDIO_LED_ON,	// Turn LED on
	SDIO_LED_OFF	// Turn LED off
}
SDHOST_LED_ONOFF_E;

//---0x2050_002C---Clock Control & Timeout  Control & Software Reset
typedef enum SDHOST_CLK_ONOFF_E_TAG
{
	CLK_ON,
	CLK_OFF
}
SDHOST_CLK_ONOFF_E;

//---0x2050_0004---Block Size Register & Block Count Register
typedef enum SDHOST_DMA_BUF_SIZE_E_TAG
{
	SDIO_DMA_4K,
	SDIO_DMA_8K,
	SDIO_DMA_16K,
	SDIO_DMA_32K,
	SDIO_DMA_64K,
	SDIO_DMA_128K,
	SDIO_DMA_256K,
	SDIO_DMA_512K
}
SDHOST_DMA_BUF_SIZE_E;


//---capability---
typedef enum SDHOST_CAPBILITY_SUPPORT_ENUM
{
	CAP_VOL_1_8V = (0x1<<0),	// provide 1.8v
	CAP_VOL_3_0V = (0x1<<1),	// provide 3.0v
	CAP_VOL_3_3V = (0x1<<2),	// can provide 3.3v 
	SPD_RESU = (0x1<<3),			// support pause funtion
	DMA_SPT = (0x1<<4),			// support dma function
	HIGH_SPEED = (0x1<<5)		// support high speed mode
}SDHOST_CAPBILITY_SUPPORT_E;

typedef struct SDHOST_CAPBILIT_STRUCT_TAG
{
	uint32 capbility_function;
	uint32 cability_Max_BlkLen;	// max block size that host can be support
	uint32 sd_Base_Max_Clk;		// max base clock frequence that host can support
	uint32 timeOut_Clk_unit;		// unit of base clock frequency used to detect Data Timeout Error.
	uint32 timeOut_Base_Clk;		// base clock frequency used to detect Data Timeout Error.

	uint32 max_current_for_1_8;	// Maximum Current for 1.8V
	uint32 max_current_for_3_0;	// Maximum Current for 3.0V
	uint32 max_current_for_3_3;	// Maximum Current for 3.3V
}
SDHOST_CAPBILIT_T;

//---
typedef enum SDHOST_SLOT_NO_TAG
{
	SDHOST_SLOT_0 = 0,
	SDHOST_SLOT_1 = 1,
	SDHOST_SLOT_2 = 2,
	SDHOST_SLOT_3 = 3,
	SDHOST_SLOT_4 = 4,
	SDHOST_SLOT_5 = 5,
	SDHOST_SLOT_6 = 6,
	SDHOST_SLOT_7 = 7,
	SDHOST_SLOT_MAX_NUM
}
SDHOST_SLOT_NO;


//---中断相关---
#define SIG_ERR BIT_0
#define SIG_CARD_IN BIT_1
#define SIG_CARD_INSERT BIT_2
#define SIG_CARD_REMOVE BIT_3
#define SIG_BUF_RD_RDY BIT_4
#define SIG_BUF_WD_RDY BIT_5
#define SIG_DMA_INT BIT_6
#define SIG_BLK_CAP BIT_7
#define SIG_TRANS_CMP BIT_8
#define SIG_CMD_CMP BIT_9
#define SIG_ALL (SIG_ERR|SIG_CARD_IN|SIG_CARD_INSERT|SIG_CARD_REMOVE|SIG_BUF_RD_RDY|SIG_BUF_WD_RDY|SIG_DMA_INT|SIG_BLK_CAP|SIG_TRANS_CMP|SIG_CMD_CMP)

#define ERR_RSP BIT_0
#define ERR_CMD12 BIT_1
#define ERR_CUR_LIMIT BIT_2
#define ERR_DATA_END BIT_3
#define ERR_DATA_CRC BIT_4
#define ERR_DATA_TIMEOUT BIT_5
#define ERR_CMD_INDEX BIT_6
#define ERR_CMD_END BIT_7
#define ERR_CMD_CRC BIT_8
#define ERR_CMD_TIMEOUT BIT_9
#define ERR_ALL (ERR_RSP|ERR_CMD12|ERR_CUR_LIMIT|ERR_DATA_END|ERR_DATA_CRC|ERR_DATA_TIMEOUT|ERR_CMD_INDEX|ERR_CMD_END|ERR_CMD_CRC|ERR_CMD_TIMEOUT)

#define CMD12_ERR_NOT_ISSUE
#define CMD12_ERR_INDEX
#define CMD12_ERR_END
#define CMD12_ERR_CRC
#define CMD12_ERR_TIMEOUT
#define CMD12_ERR_NOT_EXE


typedef void (*SDIO_CALLBACK)(uint32 msg,uint32 errCode,SDHOST_SLOT_NO slotNum);
struct SDHOST_PORT_T_TAG;
typedef struct SDHOST_PORT_T_TAG* SDHOST_HANDLE;

//---HOST 初始化相关---
typedef enum SDHOST_RST_TYPE_E_TAG
{
	RST_CMD_LINE,
	RST_DAT_LINE,
	RST_CMD_DAT_LINE,
	RST_ALL,
	RST_MODULE
}
SDHOST_RST_TYPE_E;


//---命令相关
#define TRANS_MODE_ATA_CMPLETE_SIG_EN BIT_0
#define TRANS_MODE_MULTI_BLOCK BIT_1
#define TRANS_MODE_READ BIT_2
#define TRANS_MODE_CMD12_EN BIT_3
#define TRANS_MODE_BLOCK_COUNT_EN BIT_4
#define TRANS_MODE_DMA_EN BIT_5
#define CMD_HAVE_DATA BIT_6

typedef enum CMD_RSP_TYPE_E_TAG
{
	CMD_NO_RSP = 0,
	CMD_RSP_R1,
	CMD_RSP_R2,
	CMD_RSP_R3,
	CMD_RSP_R4,
	CMD_RSP_R5,
	CMD_RSP_R6,
	CMD_RSP_R7,
	CMD_RSP_R1B,
	CMD_RSP_R5B
}CMD_RSP_TYPE_E;

typedef enum SDHOST_CMD_TYPE_E_TAG
{
	CMD_TYPE_NORMAL = 0,
	CMD_TYPE_SUSPEND,
	CMD_TYPE_RESUME,
	CMD_TYPE_ABORT
}SDHOST_CMD_TYPE_E;

//======================

//====================
//---Present state Register---
//====================
PUBLIC uint32 SDHOST_GetPinState(SDHOST_HANDLE sdhost_handler);

//====================
//---Power Control Register---
//====================
/*****************************************************************************/
//  Description:  power on/off led
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		onoff:
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_Led(SDHOST_HANDLE sdhost_handler,SDHOST_LED_ONOFF_E onoff);
/*****************************************************************************/
//  Description:  set data bus width
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		width: data bus width,only 1bit ,4bit and 8bit canbe used
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_Cfg_BusWidth(SDHOST_HANDLE sdhost_handler,SDHOST_BIT_WIDTH_E width);
/*****************************************************************************/
//  Description:  set bus speed mode
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		speed: speed mode ,only low speed mode and high speed mode canbe used
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_Cfg_SpeedMode(SDHOST_HANDLE sdhost_handler,SDHOST_SPEED_E speed);
/*****************************************************************************/
//  Description:  Set operation voltage of card(mmc \sd\sdio card.etc.)
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		voltage:
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_Cfg_Voltage(SDHOST_HANDLE sdhost_handler,SDHOST_VOL_RANGE_E voltage);
/*****************************************************************************/
//  Description:  Open or close power supply of card(mmc \sd\sdio card.etc.)
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		on_off:
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_SD_POWER(SDHOST_HANDLE sdhost_handler,SDHOST_PWR_ONOFF_E on_off);




//====================
//---Block Gap Control Register---
//====================
/*****************************************************************************/
//  Description:  Set the position of break point
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_BlkGapIntPosSet(SDHOST_HANDLE sdhost_handler);
/*****************************************************************************/
//  Description:  Enable pause function of host. the card must support this function also,then this function can be worked
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_EnableReadWaitCtl(SDHOST_HANDLE sdhost_handler);
/*****************************************************************************/
//  Description:  Set break point during the transmition
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_StopAtBlkGap(SDHOST_HANDLE sdhost_handler);
/*****************************************************************************/
//  Description:  When transmission is paused ,this function can resume the transmission again
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_Continue(SDHOST_HANDLE sdhost_handler);





//====================
//----Clock Control Register---
//====================
/*****************************************************************************/
//  Description:  Open or close internal clk.when this clk is disable ,the host will enter in sleep mode
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		onoff:
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_internalClk_OnOff(SDHOST_HANDLE sdhost_handler,SDHOST_CLK_ONOFF_E onoff);
/*****************************************************************************/
//  Description:  Open or close card clk.
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		onoff:
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_SD_clk_OnOff(SDHOST_HANDLE sdhost_handler,SDHOST_CLK_ONOFF_E onoff);
/*****************************************************************************/
//  Description:  Set the frequence of Card clock
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		sdio_clk: the frequence of card working clock
//  Return:
//		uint32 value : the frequency that be used acctually
//  Note: 
/*****************************************************************************/
PUBLIC uint32 SDHOST_SD_Clk_Freq_Set(SDHOST_HANDLE sdhost_handler,uint32 sdio_clk);







//====================
//---Timeout Control Register--
//====================
/*****************************************************************************/
//  Description:  Set timeout value ,this value is used during the data transmission
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		clk_cnt: the value is (2 ^ (clkcnt+13))*T_BSCLK,T_BSCLK is working frequence of host
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_SetDataTimeOutValue(SDHOST_HANDLE sdhost_handler,uint8 clk_cnt); // (2 ^ (clkcnt+13))*T_BSCLK






//====================
//---Software Reset Register---
//====================
/*****************************************************************************/
//  Description: Reset the specify module of host
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		rst_type: indicate which module will be reset(command lin\data line\all the module)
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_RST(SDHOST_HANDLE sdhost_handler,SDHOST_RST_TYPE_E rst_type);





//====================
//---Block count blcok size and DMA size Register---
//====================
/*****************************************************************************/
//  Description: Set block size \count of block\and Dma Buffer size
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		block_size: size of block( 0 <= block size <=0x0800)
//		block_cnt: the count of block(0 <= block_cnt <= 0xFFFF)
//		DMAsize:buffer size of DMA(4K,8K,16K,32K,64K,128K,256K,512K)
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_SetDataParam(SDHOST_HANDLE sdhost_handler,uint32 block_size,uint32 block_cnt,SDHOST_DMA_BUF_SIZE_E DMAsize);
/*****************************************************************************/
//  Description: Set start address of DMA buffer
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		dmaAddr: start address of DMA buffer
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_SetDmaAddr(SDHOST_HANDLE sdhost_handler, uint32 dmaAddr);
/*****************************************************************************/
//  Description: Get stop address of DMA buffer,when buffer is used ,the dma will stop at last address of buffer.this function will get this address
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//  Return:
//		uint32 value: address that DMA stoped at
//  Note: 
/*****************************************************************************/
PUBLIC uint32 SDHOST_GetDmaAddr(SDHOST_HANDLE sdhost_handler);








//====================
//---Argument Register---
//====================
/*****************************************************************************/
//  Description: Set the argument of command
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		argument
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_SetCmdArg(SDHOST_HANDLE sdhost_handler,uint32 argument);








//====================
//---CMD Register---
//====================
/*****************************************************************************/
//  Description: Set the mode of command
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		cmdIndex:	command
//		transmode:	transfer mode
//		cmd_type:	comand type ,it may be normal comman ,resume comman etc.
//		Response:	the inspect response from card. if this comman is performed by card successly ,this response will be return by card
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_SetCmd(SDHOST_HANDLE sdhost_handler,uint32 cmdIndex,uint32 transmode,SDHOST_CMD_TYPE_E cmd_type,CMD_RSP_TYPE_E Response);

//====================
//---Response Register---
//====================
/*****************************************************************************/
//  Description: Get content from host response register
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handle of host driver
//		Response:	the type of response
//		rspBuf:		the content will be stored in this place
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SDHOST_GetRspFromBuf(SDHOST_HANDLE sdhost_handler,CMD_RSP_TYPE_E Response,uint8* rspBuf);
//====




PUBLIC void SDHOST_NML_IntStatus_Clr(SDHOST_HANDLE sdhost_handler,uint32 msg);
PUBLIC void SDHOST_NML_IntStatus_En(SDHOST_HANDLE sdhost_handler,uint32 msg);
PUBLIC void SDHOST_NML_IntStatus_Dis(SDHOST_HANDLE sdhost_handler,uint32 msg);
PUBLIC void SDHOST_NML_IntSig_En(SDHOST_HANDLE sdhost_handler,uint32 msg);
PUBLIC void SDHOST_NML_IntSig_Dis(SDHOST_HANDLE sdhost_handler,uint32 msg);
PUBLIC uint32 SDHOST_GetNMLIntStatus(SDHOST_HANDLE sdhost_handler);

PUBLIC uint32 SDHOST_GetErrCode(SDHOST_HANDLE sdhost_handler);
PUBLIC void SDHOST_SetErrCodeFilter(SDHOST_HANDLE sdhost_handler,uint32 err_msg);


/*****************************************************************************/
//  Description: Regist host slot
//  Author: Jason.wu
//  Param
//		slot_NO:	which slot you want to used
//		fun:		this function will be called when event happened
//  Return:
//		Not zero: succes
//		zeror: fail
//  Note: 
/*****************************************************************************/
PUBLIC SDHOST_HANDLE SDHOST_Register(SDHOST_SLOT_NO slot_NO,SDIO_CALLBACK fun);
/*****************************************************************************/
//  Description: Free the slot resource
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handler of host driver ,this is returned by SDHOST_Register
//  Return:
//		Not zero: succes
//		zeror: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN SDHOST_UnRegister(SDHOST_HANDLE sdhost_handler);

/*****************************************************************************/
//  Description: select witch slot to work
//  Author: Jason.wu
//  Param
//		slot_NO: slot number
//  Return:
//		NONE
//  Note: This function must be applied according different platform
/*****************************************************************************/
PUBLIC void SDHOST_Slot_select(SDHOST_SLOT_NO slot_NO);







//====================
//---External function---
//====================
//===This interface must be defined according different platform ===
/*****************************************************************************/
//  Description: Set basic clk of host,card clk will divided from this clk
//  Author: Jason.wu
//  Param
//		sdhost_handler: the handler of host driver ,this is returned by SDHOST_Register
//		sdio_base_clk: the frequecy that you want to set
//  Return:
//		uint32 value :the frequency that be used acctually
//  Note: This function must be applied according different platform
/*****************************************************************************/
extern PUBLIC uint32 SDHOST_BaseClk_Set(SDHOST_SLOT_NO slot_NO,uint32 sdio_base_clk);
/*****************************************************************************/
//  Description: select appropriate pin function that is controlled by SDIO host
//  Author: Jason.wu
//  Param
//		slot_NO: slot number
//  Return:
//		NONE
//  Note: This function must be applied according different platform
/*****************************************************************************/
extern PUBLIC void SDHOST_Pin_select(SDHOST_SLOT_NO slot_NO);
//===end===



#endif

