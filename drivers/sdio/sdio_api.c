/******************************************************************************
 ** File Name:      sdio.c                                                    *
 ** Author:         jiayong.yang                                              *
 ** DATE:           25/06/2012                                                *
 ** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    add sdio API for modem boot in u-boot in SDIO mode        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 25/06/2010     jiayong.yang       Create                                  *
 ******************************************************************************/
#include <common.h>
#include <asm/arch/bits.h>
#include "asm/arch/sci_types.h"
#include "sdio_card_pal.h"
#include "sdio_master_pal.h"

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/
#define MAX_BLOCK_COUNT     511   
#define MAX_BLOCK_SIZE      512
#define MAX_BUF_SIZE        (MAX_BLOCK_COUNT*MAX_BLOCK_SIZE)
#define SPRD_SDIO_CLK	    (16000000)
#define SDIO_PRINTF(x) printf x

#define mdelay(x)	udelay(1000*x)

typedef void* SDIO_HANDLE;
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**----------------------------------------------------------------------------*/
typedef enum CARD_SLOT_NAME_TAG{
	CARD_SLOT_0,
	TEST_MAX_SLOT_NO
}CARD_SLOT_NAME_E;

typedef enum RW_CASE_SDIO_TAG{
    RW_BYTE_MODE,
    RW_BLOCK_MODE,
    RW_MAX_MODE
}RW_MODE_E;
extern void Dcache_CleanRegion(unsigned int addr, unsigned int length);
/**---------------------------------------------------------------------------*
 **                          Variables                                        *
 **---------------------------------------------------------------------------*/

static int __sdio_write(CARD_SDIO_HANDLE cardHandle,
        unsigned int block_len,
        unsigned int count,
        unsigned int start_addr,
        unsigned char* buf,
        RW_MODE_E mode)
{
    unsigned int inc_flag = 1; // addr increase
    int ret = TRUE;

    switch(mode)
    {
        case RW_BYTE_MODE:
            if(FALSE == SDIO_WriteBytes(cardHandle,SDIO_SLAVE_FUNC_1,start_addr,inc_flag,count,buf)){
                SDIO_PRINTF((" writebytes failed!!"));
                ret = FALSE;
                break;
            }
        break;
    
        case RW_BLOCK_MODE:
            if(FALSE == SDIO_WriteBlocks(cardHandle,SDIO_SLAVE_FUNC_1,start_addr,inc_flag,count,buf)){
                SDIO_PRINTF(("writeblocks failed!!"));
                ret = FALSE;
                break;
            }              
        break;
        default:
            SDIO_PRINTF(("mode-type out of range"));
            return FALSE;
        break;
    }

    return ret;
}

static int __sdio_read(CARD_SDIO_HANDLE cardHandle,
        unsigned int block_len,
        unsigned int count,
        unsigned int start_addr,
        unsigned char* buf,
        RW_MODE_E mode)
{
    unsigned int inc_flag = 1; // addr increase
    int ret = TRUE;

    // delay 
       
    switch(mode)
    {
        case RW_BYTE_MODE:
            if(FALSE == SDIO_ReadBytes(cardHandle,SDIO_SLAVE_FUNC_1,start_addr,inc_flag,count,buf)){
                SDIO_PRINTF(("test readbytes failed!!"));
                ret = FALSE;
                break;
            } 
            //MMU_InvalideDCACHEALL((unsigned int)buf,(unsigned int)count);       
            //SDIO_PRINTF(("ACK(0x%08x):0x%08x 0x%08x\n",(unsigned int)buf,*(int *)buf,*(int *)(buf+4)));
        break;
    
        case RW_BLOCK_MODE:
            if(FALSE == SDIO_ReadBlocks(cardHandle,SDIO_SLAVE_FUNC_1,start_addr,inc_flag,count,buf)){
                SDIO_PRINTF(("test readblocks failed!!"));
                ret = FALSE;
                break;
            }    
            SDIO_PRINTF(("test readblocks done!!"));
        break;
        default:
            SDIO_PRINTF(("mode-type out of range"));
            return FALSE;
        break;
    }

    return ret;
}

static CARD_SDIO_HANDLE __init_slot(int slotNo)
{
    	CARD_SDIO_HANDLE cardHandle;
	int block_size = MAX_BLOCK_SIZE;
    
    	// Step1: Open
    	cardHandle = SPRD_SDSlave_Open(slotNo);

    	if(NULL == cardHandle){
        	SDIO_PRINTF(("slot%d Open Failed!!\r\n", slotNo));
        	return NULL;
    	}

    	// Step2: select slot, this step must be after card open
    	CARD_SDIO_SlotSelect(slotNo);
    
    	// Step3: Power on
    	SPRD_SDSlave_PwrCtl(cardHandle,TRUE);

    	//SDHOST_SetClkDiv(slotNo,2);
 
    	// Step4: Init
    	if(FALSE == SPRD_SDSlave_InitCard(cardHandle)){
        	SDIO_PRINTF(("slot%d Init Failed!!\r\n", slotNo));
        	return NULL;
    	}
	if(FALSE == SDIO_SetBusClock(cardHandle,SPRD_SDIO_CLK)){
		SDIO_PRINTF(("Set bus clock  Failed!!\r\n"));
		return NULL;
	}
	if(FALSE == SDIO_SetBusWidth(cardHandle,SDIO_CARD_PAL_4_BIT)){
		SDIO_PRINTF(("Set bus width Failed!!\r\n"));
		return NULL;
	}
	
	
    	if(FALSE == SDIO_SetBlockLength(cardHandle,SDIO_SLAVE_FUNC_0,block_size )){
                SDIO_PRINTF(("Set func0 blk-len Failed!!\r\n"));
                return NULL;
    	}
    	if(FALSE == SDIO_SetBlockLength(cardHandle,SDIO_SLAVE_FUNC_1,block_size )){
                SDIO_PRINTF(("Set func1 blk-len Failed!!\r\n"));
                return NULL;
    	}
	

        __FuncEnable(cardHandle);
        
    	return cardHandle;
}
int sdio_write(SDIO_HANDLE handle,unsigned char *buffer,int size)
{
	int block_count = 0;
	int byte_count = 0;
	int write_len = 0;
	int result;
	
	block_count = size/MAX_BLOCK_SIZE;
	byte_count = size%MAX_BLOCK_SIZE;

	if(block_count >= MAX_BLOCK_COUNT){
		SDIO_PRINTF(("too many block!!!\r\n"));
		return 0;
	}
        Dcache_CleanRegion((unsigned int)buffer,(unsigned int)size);
	if(block_count != 0){
		result = __sdio_write((CARD_SDIO_HANDLE)handle,MAX_BLOCK_SIZE,block_count,0,buffer,RW_BLOCK_MODE);
		if(result == TRUE){
			write_len = block_count * MAX_BLOCK_SIZE;
                        buffer += write_len;
		} else  {
			SDIO_PRINTF(("write block failed!!\r\n"));	
			return 0;
		}
	}
	if(byte_count != 0){
		result = __sdio_write((CARD_SDIO_HANDLE)handle,MAX_BLOCK_SIZE,byte_count,0,buffer,RW_BYTE_MODE);
		if(result == TRUE)
			write_len += byte_count;
		else {
	                SDIO_PRINTF(("write byte failed!!\r\n"));			
		}
	}
	return write_len;
	 
}
int sdio_read(SDIO_HANDLE handle,unsigned char *buffer,int size)
{
	int block_count = 0;
	int byte_count = 0;
	int read_len = 0;
	int result;
	
	block_count = size/MAX_BLOCK_SIZE;
	byte_count = size%MAX_BLOCK_SIZE;

	if(block_count >= MAX_BLOCK_COUNT){
		SDIO_PRINTF(("too many block!!!\r\n"));
		return 0;
	}
        //MMU_InvalideDCACHEALL((unsigned int)buffer,(unsigned int)size);
	if(block_count != 0){
		result = __sdio_read((CARD_SDIO_HANDLE)handle,MAX_BLOCK_SIZE,block_count,0,buffer,RW_BLOCK_MODE);
		if(result == TRUE){
			read_len = block_count * MAX_BLOCK_SIZE;
			buffer += read_len;
		} else	{
			SDIO_PRINTF(("read block failed!!\r\n"));			
			return 0;
		}
	}
	if(byte_count != 0){
		result = __sdio_read((CARD_SDIO_HANDLE)handle,MAX_BLOCK_SIZE,byte_count,0,buffer,RW_BYTE_MODE);
		if(result == TRUE)
			read_len += byte_count;
		else {
			SDIO_PRINTF(("read byte failed!!\r\n"));			
		}
	}
	return read_len;
}

// public function
SDIO_HANDLE sdio_open(void)
{ 
    CARD_SDIO_HANDLE sio_handle;
    MMU_DisableIDCM();
    SDIO_PRINTF(("init slot0 ...\r\n"));
       
    sio_handle = __init_slot(0);
    if(NULL == sio_handle){
            SDIO_PRINTF(("slot0 handle is NULL!!\r\n"));
    }
    return (SDIO_HANDLE)sio_handle;
}
void sdio_close(SDIO_HANDLE handle)
{
	SPRD_SDSlave_Close((CARD_SDIO_HANDLE)handle);
	return;
}
