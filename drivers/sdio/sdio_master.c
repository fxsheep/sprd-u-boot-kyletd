/******************************************************************************
 ** File Name:      sdio_master.c
 ** Author:         Andy.chen
 ** DATE:           21/02/2010
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.
 ** Description:    This file describe operation of SD host.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION
 ** 21/02/2010     Andy.Chen          Create                                  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <common.h>
#include <asm/arch/bits.h>
#include "sdio_card_pal.h"

#define CARD_SDIO_SLOT_MAX 1
#define DEFAULT_CARD_BLOCKLEN 512
#define CARD_CMD_MAX_TIME 5000
#define SDIO_CLK_25M	    (25000000)
#define CARD_SDIO_PRINT(x) printf x

#define CARD_SDIO_ASSERT(x) 	\
{				\
	if(!(x)){		\
		CARD_SDIO_PRINT(("assert in function %s line %d",__func__, __LINE__));	\
		while(1);				\
	}			\
}
#define mdelay(x)	udelay(1000*x)

typedef enum SDIO_CCCR_REG_ADDR_TAG{
    CCCR_SDIO_CCCR_VER       = 0x00,         // 0x00
    CCCR_SD_VER,                             // 0x01
    CCCR_IO_ENABLE,
    CCCR_IO_READY,
    CCCR_INT_ENABLE,
    CCCR_INT_PENDING,
    CCCR_IO_ABORT,
    CCCR_BUS_CONTROL,
    CCCR_CARD_CAP,
    CCCR_CIS_PTR,
    CCCR_BUS_SUSPEND         = 0x0C,
    CCCR_FUNC_SEL,
    CCCR_EXEC_FLAG,
    CCCR_READY_FLAG,
    CCCR_FN0_BLK_SIZE,
    CCCR_POWER_CTR           = 0x12,
    CCCR_HIGH_SPEED, 
    CCCR_RFU,
    CCCR_REV                 = 0xF0,
    CCCR_REG_ADDR_MAX        = 0xFF
}SDIO_CCCR_REG_ADDR_E;

typedef enum SDIO_FBR_REG_ADDR_TAG{
    FBR_FUNC1_TYPE              = 0x100,
    FBR_FUNC1_EXT,
    FBR_FUNC1_CIS_PTR           = 0x109,
    FBR_FUNC1_CSA_PTR           = 0x10C,
    FBR_FUNC1_CSA_DATA_WINDOW   = 0x10F,
    FBR_FUNC1_BLK_SIZE          = 0x110,
    FBR_FUNC1_ADDR_MAX          = 0x1FF
}SDIO_FBR_REG_ADDR_E;

CARD_PORT_T cardPort[CARD_SDIO_SLOT_MAX];

//-----------------------------------------------------------------------------------
//	To judge whether the handle is valid
//-----------------------------------------------------------------------------------
static int _IsCardHandleValid(CARD_SDIO_HANDLE cardHandle)
{
	int i;

	for(i = 0; i < CARD_SDIO_SLOT_MAX; i++){
		if(cardHandle == &cardPort[i])
			return cardHandle->open_flag;
	}
	return FALSE;
}


static int __ReadRCA(CARD_SDIO_HANDLE cardHandle,unsigned short*RCA)
{
	unsigned char rspBuf[16];
	unsigned short tmpRCA;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));

	if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD3_SEND_RELATIVE_ADDR,0,NULL,rspBuf))
	{
		return FALSE;
	}
	tmpRCA = 0;
	tmpRCA = rspBuf[0];
	tmpRCA = tmpRCA<<8;
	tmpRCA |= rspBuf[1];
        if(RCA != NULL)
		*RCA = tmpRCA;

	CARD_SDIO_PRINT(("SDIO RCA = %x",tmpRCA));

	return TRUE;
}
static int __SelectCard(CARD_SDIO_HANDLE cardHandle)
{
	unsigned char rspBuf[16];
	unsigned short RCA = cardHandle->RCA;
	unsigned int argument = 0;

	argument = RCA;
	argument = argument<<16;

	if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD7_SELECT_DESELECT_CARD,argument,NULL,rspBuf))
		return FALSE;
	return TRUE;
}

static int __ReadCCCRInfo(CARD_SDIO_HANDLE cardHandle)
{
    unsigned char rspBuf[16];
    unsigned int argument = 0;
    unsigned int rw_flag  = 0;        // 1:write, 0:read
    unsigned int raw_flag = 0;        // 1:read after write
    unsigned int addr     = CCCR_SDIO_CCCR_VER;
    unsigned int data     = 0;        // 0

    unsigned int cis_ptr;

    CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));

    CARD_SDIO_PRINT(("CCCR Information---------------------------"));
    // cccr version-------------
    addr     = CCCR_SDIO_CCCR_VER;
    argument = ((rw_flag&0x1)<<31)|((0&0x7)<<28)|((raw_flag&0x1)<<27)|((addr&0x1FFFF)<<9)|(data&0xff); 
    if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD52_IO_RW_DIRECT,argument,NULL,rspBuf))
		return FALSE;
    CARD_SDIO_PRINT(("SDIO CMD52 read CCCR/SDIO Revision response 0x%02x,0x%02x,0x%02x,0x%02x\n",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3]));
    switch(rspBuf[3]&0xF)
    {
        case 0:
            CARD_SDIO_PRINT(("CCCR/FBR Version 1.00"));
            break;
        case 1:
            CARD_SDIO_PRINT(("CCCR/FBR Version 1.10"));
            break;
        case 2:
            CARD_SDIO_PRINT(("CCCR/FBR Version 2.00"));
            break;
        default:
            CARD_SDIO_PRINT(("CCCR/FBR Version err!!"));
            break;
            
    }

    //sdio version-----------------
    switch((rspBuf[3]>>4)&0xF)
    {
        case 0:
            CARD_SDIO_PRINT(("SDIO Version 1.00"));
            break;
        case 1:
            CARD_SDIO_PRINT(("SDIO Version 1.10"));
            break;
        case 3:
            CARD_SDIO_PRINT(("SDIO Version 2.00"));
            break;
        default:
            CARD_SDIO_PRINT(("SDIO Version err!!"));
            break;
            
    }
    
    // SD version-------------
    addr     = CCCR_SD_VER;
    argument = ((rw_flag&0x1)<<31)|((0&0x7)<<28)|((raw_flag&0x1)<<27)|((addr&0x1FFFF)<<9)|(data&0xff); 
    if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD52_IO_RW_DIRECT,argument,NULL,rspBuf))
	return FALSE;
    //CARD_SDIO_PRINT(("SDIO CMD52 read SD Revision response 0x%02x,0x%02x,0x%02x,0x%02x",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3]));
    switch(rspBuf[3]&0xF){
        case 0:
            CARD_SDIO_PRINT(("SD Version 1.00"));
            break;
        case 1:
            CARD_SDIO_PRINT(("SD Version 1.10"));
            break;
        case 2:
            CARD_SDIO_PRINT(("SD Version 2.00"));
            break;
        default:
            CARD_SDIO_PRINT(("SD Version err!!"));
            break;
            
    }

    // card capability -------------
    CARD_SDIO_PRINT(("card capability:"));
    addr     = CCCR_CARD_CAP;
    argument = ((rw_flag&0x1)<<31)|((0&0x7)<<28)|((raw_flag&0x1)<<27)|((addr&0x1FFFF)<<9)|(data&0xff); 
    if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD52_IO_RW_DIRECT,argument,NULL,rspBuf))
	return FALSE;

    // cis pointer-------------
    addr     = CCCR_CIS_PTR;
    argument = ((rw_flag&0x1)<<31)|((0&0x7)<<28)|((raw_flag&0x1)<<27)|((addr&0x1FFFF)<<9)|(data&0xff); 
    if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD52_IO_RW_DIRECT,argument,NULL,rspBuf))
	return FALSE;
    cis_ptr = rspBuf[3];
    
    addr     = CCCR_CIS_PTR+1;
    argument = ((rw_flag&0x1)<<31)|((0&0x7)<<28)|((raw_flag&0x1)<<27)|((addr&0x1FFFF)<<9)|(data&0xff); 
    if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD52_IO_RW_DIRECT,argument,NULL,rspBuf))
        return FALSE;
    cis_ptr |= (((unsigned int)rspBuf[3])<<8);
    
    addr     = CCCR_CIS_PTR+2;
    argument = ((rw_flag&0x1)<<31)|((0&0x7)<<28)|((raw_flag&0x1)<<27)|((addr&0x1FFFF)<<9)|(data&0xff); 
    if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD52_IO_RW_DIRECT,argument,NULL,rspBuf))
        return FALSE;
    cis_ptr |= (((unsigned int)rspBuf[3])<<16);
    
    CARD_SDIO_PRINT(("Common CIS Pointer:0x%x",cis_ptr));
    return TRUE;
}

static int __ReadFBRInfo(CARD_SDIO_HANDLE cardHandle)
{
    unsigned char rspBuf[16];
    unsigned int argument = 0;
    unsigned int rw_flag  = 0;        // 1:write, 0:read
    unsigned int raw_flag = 0;        // 1:read after write
    unsigned int addr     = FBR_FUNC1_TYPE;
    unsigned int data     = 0;        // 0

    unsigned int cis_ptr;

    CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));

    CARD_SDIO_PRINT(("FBR Information---------------------------"));
    // fbr type-------------
    addr     = FBR_FUNC1_TYPE;
    argument = ((rw_flag&0x1)<<31)|((0&0x7)<<28)|((raw_flag&0x1)<<27)|((addr&0x1FFFF)<<9)|(data&0xff); 
    if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD52_IO_RW_DIRECT,argument,NULL,rspBuf))
        return FALSE;
    switch(rspBuf[3]&0xF){
        case 0:
            CARD_SDIO_PRINT(("SDIO interface support none!"));
            break;
        case 1:
            CARD_SDIO_PRINT(("SDIO interface: UART"));
            break;
        case 2:
            CARD_SDIO_PRINT(("SDIO interface: A-type BT"));
            break;
        case 3:
            CARD_SDIO_PRINT(("SDIO interface: B-type BT"));
            break;
        case 4:
            CARD_SDIO_PRINT(("SDIO interface: GPS"));
            break;
        case 5:
            CARD_SDIO_PRINT(("SDIO interface: Camera"));
            break;
        case 6:
            CARD_SDIO_PRINT(("SDIO interface: PHS"));
            break;
        case 7:
            CARD_SDIO_PRINT(("SDIO interface: WLAN"));
            break;
        case 8:
            CARD_SDIO_PRINT(("SDIO interface: Embeded-ATA"));
            break;
        case 0xF:
            //CARD_SDIO_PRINT(("SDIO interface: extension"));
            break;

        default:
            CARD_SDIO_PRINT(("SDIO interface code err!!"));
            break;            
    }

    (rspBuf[3]&BIT_7)?(CARD_SDIO_PRINT(("FBR function support csa"))):(CARD_SDIO_PRINT(("FBR function not support csa")));

    // extended sdio interface-------------
    addr     = FBR_FUNC1_EXT;
    argument = ((rw_flag&0x1)<<31)|((0&0x7)<<28)|((raw_flag&0x1)<<27)|((addr&0x1FFFF)<<9)|(data&0xff); 
    if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD52_IO_RW_DIRECT,argument,NULL,rspBuf))
        return FALSE;
    
    // cis pointer-------------
    addr     = FBR_FUNC1_CIS_PTR;
    argument = ((rw_flag&0x1)<<31)|((0&0x7)<<28)|((raw_flag&0x1)<<27)|((addr&0x1FFFF)<<9)|(data&0xff); 
    if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD52_IO_RW_DIRECT,argument,NULL,rspBuf))
        return FALSE;
    cis_ptr = rspBuf[3];

    addr     = FBR_FUNC1_CIS_PTR+1;
    argument = ((rw_flag&0x1)<<31)|((0&0x7)<<28)|((raw_flag&0x1)<<27)|((addr&0x1FFFF)<<9)|(data&0xff); 
    if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD52_IO_RW_DIRECT,argument,NULL,rspBuf))
        return FALSE;
    cis_ptr |= (((unsigned int)rspBuf[3])<<8);

    addr     = FBR_FUNC1_CIS_PTR+2;
    argument = ((rw_flag&0x1)<<31)|((0&0x7)<<28)|((raw_flag&0x1)<<27)|((addr&0x1FFFF)<<9)|(data&0xff); 
    if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD52_IO_RW_DIRECT,argument,NULL,rspBuf))
        return FALSE;
    cis_ptr |= (((unsigned int)rspBuf[3])<<16);

    CARD_SDIO_PRINT(("Func1 CIS Pointer:0x%x",cis_ptr));
    return TRUE;

}
int SDIO_ReadByte(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int RegAddr,unsigned char* buf)
{
	unsigned char rspBuf[16];
    	unsigned int argument = 0;
    	unsigned int rw_flag  = 0;        // 1:write, 0:read
    	unsigned int raw_flag = 0;        // 1:read after write
    	unsigned int addr     = RegAddr;

	//CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));

    	argument = ((rw_flag&0x1)<<31)|((0&0x7)<<28)|((raw_flag&0x1)<<27)|((addr&0x1FFFF)<<9); 
	if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD52_IO_RW_DIRECT,argument,NULL,rspBuf))
		return FALSE;

    	*buf = rspBuf[3];

	return TRUE;
}
int SDIO_WriteByte(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int RegAddr,unsigned char* wbuf,unsigned char* rbuf)
{
	unsigned char rspBuf[16];
    	unsigned int argument = 0;
    	unsigned int rw_flag  = 1;        // 1:write, 0:read
    	unsigned int raw_flag = 1;        // 1:read after write
    	unsigned int addr     = RegAddr;
    	unsigned int data     = *wbuf;        

	//CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));

    	argument = ((rw_flag&0x1)<<31)|((0&0x7)<<28)|((raw_flag&0x1)<<27)|((addr&0x1FFFF)<<9)|(data&0xff); 
	if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD52_IO_RW_DIRECT,argument,NULL,rspBuf))
		return FALSE;

    	if(NULL != rbuf)
        	*rbuf = rspBuf[3];

	return TRUE;
}
int __FuncEnable(CARD_SDIO_HANDLE cardHandle)
{
    unsigned char wdata = BIT_1; // IOE1
    unsigned char rdata = 0;
    
    if(FALSE == SDIO_WriteByte(cardHandle, SDIO_SLAVE_FUNC_0, CCCR_IO_ENABLE, &wdata, NULL))
        return FALSE;

    if(FALSE == SDIO_ReadByte(cardHandle, SDIO_SLAVE_FUNC_0, CCCR_IO_READY,&rdata))
        return FALSE;
    while(0 == (rdata&BIT_1))
    {
        if(FALSE == SDIO_ReadByte(cardHandle, SDIO_SLAVE_FUNC_0, CCCR_IO_READY,&rdata))
            return FALSE;
        
    }
    CARD_SDIO_PRINT(("func1 ready!!!"));
    return TRUE;
}
static int SDIO_SetSpeedMode(CARD_SDIO_HANDLE cardHandle,int mode)
{
	unsigned char wdata = BIT_1;
    	unsigned char rdata = 0;

    	if(FALSE == SDIO_ReadByte(cardHandle, SDIO_SLAVE_FUNC_0, CCCR_HIGH_SPEED,&rdata))
        	return FALSE;
	if(mode == 0){
		SDHOST_Cfg_SpeedMode(cardHandle->sdioPalHd->sdio_port,SDIO_LOWSPEED);
		wdata = 0;
    		if(FALSE == SDIO_WriteByte(cardHandle, SDIO_SLAVE_FUNC_0, CCCR_HIGH_SPEED, &wdata, &rdata))
        		return FALSE;
	} else {
		SDHOST_Cfg_SpeedMode(cardHandle->sdioPalHd->sdio_port,SDIO_HIGHSPEED);
		wdata = BIT_1;
    		if(FALSE == SDIO_WriteByte(cardHandle, SDIO_SLAVE_FUNC_0, CCCR_HIGH_SPEED, &wdata, &rdata))
        		return FALSE;
	}
	CARD_SDIO_PRINT(("SDIO_SetSpeedMode %d %d\n",rdata,wdata));
	return TRUE;
}
int SDIO_SetBusClock(CARD_SDIO_HANDLE cardHandle,int clock)
{
	int ret = TRUE;

	if(clock >=SDIO_CLK_25M){
		ret = SDIO_SetSpeedMode(cardHandle,1);
	}else{
		ret = SDIO_SetSpeedMode(cardHandle,0);
	}

	if(ret == TRUE){
        	SDHOST_internalClk_OnOff(cardHandle->sdioPalHd->sdio_port,CLK_OFF);
        	SDHOST_SD_clk_OnOff(cardHandle->sdioPalHd->sdio_port,CLK_OFF);
		SDHOST_SD_Clk_Freq_Set(cardHandle->sdioPalHd->sdio_port,clock);
        	SDHOST_internalClk_OnOff(cardHandle->sdioPalHd->sdio_port,CLK_ON);
        	SDHOST_SD_clk_OnOff(cardHandle->sdioPalHd->sdio_port,CLK_ON);
	}
	return ret;
}
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
CARD_SDIO_HANDLE SPRD_SDSlave_Open(int slotNo)
{
	CARD_SDIO_ASSERT(slotNo < CARD_SDIO_SLOT_MAX);

	if(TRUE == cardPort[slotNo].open_flag)
		return NULL;

	cardPort[slotNo].open_flag = TRUE;
	cardPort[slotNo].sdioPalHd = SPRD_SDSlave_Pal_Open((SDIO_CARD_PAL_SLOT_E)slotNo);
	
	cardPort[slotNo].RCA = 0x1000 | slotNo;
	cardPort[slotNo].bus_width = SDIO_CARD_PAL_1_BIT;

	return &cardPort[slotNo];
}

/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
void SPRD_SDSlave_Close(CARD_SDIO_HANDLE cardHandle)
{
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));

	SPRD_SDSlave_Pal_Close(cardHandle->sdioPalHd);
	cardHandle->open_flag = FALSE;
}

void SPRD_SDSlave_PwrCtl(CARD_SDIO_HANDLE cardHandle,int switchCtl)
{
	if(TRUE == switchCtl)
		SPRD_SDSlave_Pal_Pwr(cardHandle->sdioPalHd,SDIO_CARD_PAL_ON);
	else 
		SPRD_SDSlave_Pal_Pwr(cardHandle->sdioPalHd,SDIO_CARD_PAL_OFF);
}

void CARD_SDIO_SlotSelect(int slotNo)
{
        
    CARD_SDIO_ASSERT(slotNo < CARD_SDIO_SLOT_MAX);

    SDIO_Card_Pal_SlotSelect((SDIO_CARD_PAL_SLOT_E)slotNo);
}


//-----------------------------------------------------------------------------------
//	Set data bus width of card
//-----------------------------------------------------------------------------------
int SDIO_SetBusWidth(CARD_SDIO_HANDLE cardHandle,SDIO_CARD_PAL_BUFWIDTH_E width)
{
    unsigned char wdata = ((SDIO_CARD_PAL_1_BIT == width)?(0x80):(0x82));
    unsigned char rdata = 0;
    
    if(width == cardHandle->bus_width)
	return TRUE;
    
    if(FALSE == SDIO_WriteByte(cardHandle, SDIO_SLAVE_FUNC_0, CCCR_BUS_CONTROL, &wdata, &rdata))
        return FALSE;
    
    if((rdata&0x3) != (wdata&0x3)){   
        mdelay(100);
        if(FALSE == SDIO_ReadByte(cardHandle, SDIO_SLAVE_FUNC_0, CCCR_BUS_CONTROL, &rdata))
            return FALSE;
        if((rdata&0x3) != (wdata&0x3)){        
            CARD_SDIO_PRINT(("Set bus-width failed:0x%x\r\n",rdata));            
            return FALSE;
        }
    }
    switch(width)
    {
		case SDIO_CARD_PAL_1_BIT:
			SPRD_SDSlave_Pal_SetBusWidth(cardHandle->sdioPalHd,SDIO_CARD_PAL_1_BIT);
                        CARD_SDIO_PRINT(("SDIO_SetBusWidth = SDIO_CARD_PAL_1_BIT  \n"));
		break;

		case SDIO_CARD_PAL_4_BIT:
			SPRD_SDSlave_Pal_SetBusWidth(cardHandle->sdioPalHd,SDIO_CARD_PAL_4_BIT);
                        CARD_SDIO_PRINT(("SDIO_SetBusWidth = SDIO_CARD_PAL_4_BIT  \n")); 
		break;

		default:
			CARD_SDIO_ASSERT(0);
		break;
    }
    cardHandle->bus_width = width;
     
    return TRUE;
}
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

int SDIO_SetBlockLength(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int length)
{

        unsigned char wdata = 0;
        unsigned char rdata = 0;
        unsigned int  reg_addr = 0;
        int i;
	
        CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));

	if(length == cardHandle->BlockLen[func])
		return TRUE;
        reg_addr = ((func)?(FBR_FUNC1_BLK_SIZE):(CCCR_FN0_BLK_SIZE));
        
        wdata = length&0xFF;
        for(i=0;i<2;i++) {
        	
        	if(FALSE == SDIO_WriteByte(cardHandle, SDIO_SLAVE_FUNC_0, reg_addr, &wdata, &rdata))
                	return FALSE;
        
		if(rdata != wdata){   
            		mdelay(10);
            		rdata = 0;
            		if(FALSE == SDIO_ReadByte(cardHandle, SDIO_SLAVE_FUNC_0, reg_addr, &rdata))
                		return FALSE;
            		if(rdata != wdata){        
                		CARD_SDIO_PRINT(("Set func%d blk-legth Failed!!\r\n",func));            
                		return FALSE;
            		}
        	}
                wdata = (length>>8)&0xFF;
                reg_addr++;
	}
    	
	cardHandle->BlockLen[func] = length;
    
     	CARD_SDIO_PRINT(("Set func%d blk-len = %dbyte success!\n",func, cardHandle->BlockLen[func]));

	return TRUE;

}

/*****************************************************************************/
//  Description:  Initialize card, change card from idle state to standy by state ,and get some information from card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
int SPRD_SDSlave_InitCard(CARD_SDIO_HANDLE cardHandle)
{
	unsigned char rspBuf[16];
	unsigned short RCA;
        SDIO_CARD_PAL_ERROR_E result;
        CARD_SDIO_PRINT(("SPRD_SDSlave_InitCard\n"));
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));

	cardHandle->bus_width = SDIO_CARD_PAL_1_BIT;
	cardHandle->BlockLen[0] = 0;
	cardHandle->BlockLen[1] = 0;
        do{
	    result = SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD5_SEND_OP_COND,0/*0x00FF8000*/,NULL,rspBuf);
            if(SDIO_CARD_PAL_ERR_NONE != result)
	          CARD_SDIO_PRINT(("SDIO_CMD5_SEND_OP_COND failed\n"));
	    else break;
	}while(1);
	CARD_SDIO_PRINT(("SDIO_CMD5_SEND_OP_COND successfully !!!\n"));
	CARD_SDIO_PRINT(("SDIO CMD5 response 0x%02x,0x%02x,0x%02x,0x%02x\n",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3]));
	if(0 == ((rspBuf[0]>>4)&0x7)){
	    	CARD_SDIO_PRINT(("SDIO CMD5 Read func-num error:0\n"));
		return FALSE;
	}

	do{
		if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD5_SEND_OP_COND,BIT_18/*00x00FF8000*/,NULL,rspBuf))
			return FALSE;
		
		CARD_SDIO_PRINT(("SDIO CMD5 response 0x%02x,0x%02x,0x%02x,0x%02x\n",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3]));
		//if(0 != (rspBuf[0]&BIT_7))
			break;

	}while(1); /*lint !e506*/
	
    	CARD_SDIO_PRINT(("     slave function:%d",((rspBuf[0]>>4)&0x7)));
    	CARD_SDIO_PRINT(("     slave memory present:%d",((rspBuf[0]>>3)&0x1)));
    	CARD_SDIO_PRINT(("     slave voltage bit:0x%02x%02x%02x",rspBuf[1],rspBuf[2],rspBuf[3]));

    	if(FALSE == __ReadRCA(cardHandle,&RCA)){
        	CARD_SDIO_PRINT(("READ RCA Failed!!\r\n"));
        	return FALSE;
    	}
    	cardHandle->RCA = RCA;

    	// Now SDIO card is in Stand-by State
	if(FALSE == __SelectCard(cardHandle)){
        	CARD_SDIO_PRINT(("SELECT SDIO CARD Failed!!\r\n"));
		return FALSE;
	}

    	// Now SDIO card is in commond state    
    	if(FALSE == __ReadCCCRInfo(cardHandle)){
        	CARD_SDIO_PRINT(("READ CCCR information Failed!!\r\n"));
		return FALSE;
    	}

    	if(FALSE == __ReadFBRInfo(cardHandle)){
        	CARD_SDIO_PRINT(("READ FBR information Failed!!\r\n"));
		return FALSE;
    	}
   
	return TRUE;
}
int SDIO_ReadBytes(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int RegAddr,unsigned int inc_flag,unsigned int num,unsigned char* buf)
{
	unsigned char rspBuf[16];
    	unsigned int argument = 0;
	unsigned int rw_flag,blk_mode;
	CARD_DATA_PARAM_T data;

    	if(num >= 512){
        	CARD_SDIO_PRINT(("SDIO read byte-num too big!"));
        	return FALSE;
    	}

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));

	data.blkLen = num;
	data.blkNum = 0;
	data.databuf = buf;

    	rw_flag     = 0;    // read
    	blk_mode    = 0;    // byte mode 
    	argument = ((rw_flag&0x1)<<31)|((func&0x7)<<28)|((blk_mode&0x1)<<27)|((inc_flag&0x1)<<26)|((RegAddr&0x1FFFF)<<9)|(num&0x1FF); 
	if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD53_READ_BYTES,argument,&data,rspBuf))
		return FALSE;
	return TRUE;
}
int SDIO_ReadBlocks(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int RegAddr,unsigned int inc_flag,unsigned int num,unsigned char* buf)
{
	unsigned char rspBuf[16];
    	unsigned int argument = 0;
	unsigned int rw_flag,blk_mode;
	CARD_DATA_PARAM_T data;

    	if(num >= 512){
        	CARD_SDIO_PRINT(("SDIO read block count too big!"));
        	return FALSE;
    	}
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));

	data.blkLen = cardHandle->BlockLen[func];
	data.blkNum = num;
	data.databuf = buf;

    	rw_flag     = 0;    // read
    	blk_mode    = 1;    // block mode 

    	argument = ((rw_flag&0x1)<<31)|((func&0x7)<<28)|((blk_mode&0x1)<<27)|((inc_flag&0x1)<<26)|((RegAddr&0x1FFFF)<<9)|(num&0x1FF); 
	if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD53_READ_BLOCKS,argument,&data,rspBuf))
		return FALSE;

	return TRUE;
}


int SDIO_WriteBytes(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int RegAddr,unsigned int inc_flag,unsigned int num,unsigned char* buf)
{
	unsigned char rspBuf[16];
    	unsigned int argument = 0;
	unsigned int rw_flag,blk_mode;
	CARD_DATA_PARAM_T data;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));
    	if(num >= 512){
        	CARD_SDIO_PRINT(("SDIO write byte-num too big!"));
        	return FALSE;
    	}
	data.blkLen = num;
	data.blkNum = 0;
	data.databuf = buf;

    	rw_flag     = 1;    // write
    	blk_mode    = 0;    // byte mode 

    	argument = ((rw_flag&0x1)<<31)|((func&0x7)<<28)|((blk_mode&0x1)<<27)|((inc_flag&0x1)<<26)|((RegAddr&0x1FFFF)<<9)|(num&0x1FF); 
	if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD53_WRITE_BYTES,argument,&data,rspBuf))
		return FALSE;

	return TRUE;
}
int SDIO_WriteBlocks(CARD_SDIO_HANDLE cardHandle,unsigned int func,unsigned int RegAddr,unsigned int inc_flag,unsigned int num,unsigned char* buf)
{
	unsigned char rspBuf[16];
    	unsigned int argument = 0;
	unsigned int rw_flag,blk_mode;
	CARD_DATA_PARAM_T data;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));
    	if(num >= 512){
        	CARD_SDIO_PRINT(("SDIO write block-cnt too big!"));
       		return FALSE;
    	}
	data.blkLen = cardHandle->BlockLen[func];
	data.blkNum = num;
	data.databuf = buf;

    	rw_flag     = 1;    // write
    	blk_mode    = 1;    // block mode 

    	argument = ((rw_flag&0x1)<<31)|((func&0x7)<<28)|((blk_mode&0x1)<<27)|((inc_flag&0x1)<<26)|((RegAddr&0x1FFFF)<<9)|(num&0x1FF); 
	if(SDIO_CARD_PAL_ERR_NONE != SPRD_SDSlave_Pal_SendCmd(cardHandle->sdioPalHd,SDIO_CMD53_WRITE_BLOCKS,argument,&data,rspBuf))
		return FALSE;

	return TRUE;
}



