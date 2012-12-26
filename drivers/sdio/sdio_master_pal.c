#include <common.h>
#include <asm/arch/bits.h>
#include "asm/arch/sci_types.h"
#include "sdio_card_pal.h"

/*-----------------------------------------*/
//    Debug Function
/*-----------------------------------------*/
#define SDIO_CARD_PRINT(x) printf x
/*-----------------------------------------*/
//    IRQ Function
/*-----------------------------------------*/
#if defined(OS_SELF_SIMULATOR)
#define SDIO_CARD_PAL_IRQ_DECLARE OS_CPU_SR  cpu_sr
#define SDIO_CARD_PAL_DISABLE_IRQ OS_ENTER_CRITICAL
#define SDIO_CARD_PAL_ENABLE_IRQ OS_EXIT_CRITICAL
#else
#define SDIO_CARD_PAL_IRQ_DECLARE
#define SDIO_CARD_PAL_DISABLE_IRQ SCI_DisableIRQ
#define SDIO_CARD_PAL_ENABLE_IRQ SCI_RestoreIRQ
#endif

#define SDIO_CLK_24M	    (24000000)

/*-----------------------------------------*/
#define SDIO_CARD_PAL_ASSERT(x) 	\
{				\
	if(!(x)){		\
		SDIO_CARD_PRINT(("assert in function %s line %d",__func__, __LINE__));	\
		while(1);				\
	}			\
}
#define mdelay(x)	udelay(1000*x)

typedef struct
{
    SDIO_MASTER_PAL_CMD_E cmd;
    unsigned int cmdIndex;
    unsigned int intFilter;
    CMD_RSP_TYPE_E Response;
    unsigned int errFilter;
    unsigned int transmode;
    unsigned int cmdTyte;
}CMD_CTL_FLG;


//        response Name        cmd int filter                                                        ,rsp            ,cmd error filter
#define SDIO_MASTER_R4        SIG_CMD_CMP    ,CMD_RSP_R4        ,ERR_RSP|ERR_CMD_INDEX|    ERR_CMD_END|    ERR_CMD_CRC|    ERR_CMD_TIMEOUT
#define SDIO_MASTER_R5        SIG_CMD_CMP    ,CMD_RSP_R5        ,ERR_RSP|ERR_CMD_INDEX|    ERR_CMD_END|    ERR_CMD_CRC|    ERR_CMD_TIMEOUT
#define SDIO_MASTER_R6        SIG_CMD_CMP    ,CMD_RSP_R6        ,ERR_RSP|ERR_CMD_INDEX|    ERR_CMD_END|    ERR_CMD_CRC|    ERR_CMD_TIMEOUT
#define SDIO_MASTER_R1B       SIG_CMD_CMP    ,CMD_RSP_R1B       ,ERR_RSP|ERR_CMD_INDEX|    ERR_CMD_END|    ERR_CMD_CRC|    ERR_CMD_TIMEOUT

static CMD_CTL_FLG s_cmdDetail[] = 
{
// cmdindex,rsp,transmode
//#define CMDname                        cmdindex    ,data int filter    +    (cmd int filter+)rsp(+cmd error filter)    +    ,data error filter                        ,transmode
{SDIO_CMD3_SEND_RELATIVE_ADDR,  3  ,SDIO_MASTER_R6,0,CMD_TYPE_NORMAL },
{SDIO_CMD5_SEND_OP_COND,        5  ,SDIO_MASTER_R4,0,CMD_TYPE_NORMAL },
{SDIO_CMD7_SELECT_DESELECT_CARD,7  ,SDIO_MASTER_R1B,0,CMD_TYPE_NORMAL },
{SDIO_CMD52_IO_RW_DIRECT,       52 ,SDIO_MASTER_R5,0,CMD_TYPE_NORMAL },
{SDIO_CMD53_READ_BYTES,         53 ,SIG_TRANS_CMP|SDIO_MASTER_R5|ERR_DATA_END|ERR_DATA_CRC|ERR_DATA_TIMEOUT,
     TRANS_MODE_READ|TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   ,CMD_TYPE_NORMAL },
{SDIO_CMD53_READ_BLOCKS,        53 ,SIG_TRANS_CMP|SDIO_MASTER_R5|ERR_DATA_END|ERR_DATA_CRC|ERR_DATA_TIMEOUT,
     TRANS_MODE_MULTI_BLOCK|TRANS_MODE_READ|TRANS_MODE_BLOCK_COUNT_EN|TRANS_MODE_DMA_EN|CMD_HAVE_DATA,CMD_TYPE_NORMAL },
{SDIO_CMD53_WRITE_BYTES,        53 ,SIG_TRANS_CMP|SDIO_MASTER_R5 | ERR_DATA_CRC|ERR_DATA_TIMEOUT, TRANS_MODE_DMA_EN|CMD_HAVE_DATA,CMD_TYPE_NORMAL },
{SDIO_CMD53_WRITE_BLOCKS ,      53 ,SIG_TRANS_CMP|SDIO_MASTER_R5|ERR_DATA_CRC|ERR_DATA_TIMEOUT,
     TRANS_MODE_MULTI_BLOCK|TRANS_MODE_BLOCK_COUNT_EN|TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   ,CMD_TYPE_NORMAL },
};

#define SDIO_CARD_PAL_MAGICNUM 0x5344494F


#define SDIO_CARD_PAL_SUPPORT_NUM  1
extern void _SDHOST_IrqHandle(unsigned int isrnum);
static SDIO_CARD_PAL_Struct_T s_sdioCardPalHd[SDIO_CARD_PAL_SUPPORT_NUM] = {{FALSE,0,0,0}};
static volatile unsigned int s_CardErrCode = 0;

//-----------------------------------------------------------------------------------
//    Clear all event  happened before ,and prepare next transmission
//-----------------------------------------------------------------------------------
void _InitCardEvent(SDIO_CARD_PAL_HANDLE handle)
{
    s_CardErrCode = 0;
    handle->s_CardEvent = 0;
}

//-----------------------------------------------------------------------------------
//    After the transmission begin , wait event during the transmission
//-----------------------------------------------------------------------------------
int _WaitCardEvent(SDIO_CARD_PAL_HANDLE handle,unsigned int EventId)
{
    if(EventId != (handle->s_CardEvent&EventId)){
	return 0;
    }
    return 1;
}

//-----------------------------------------------------------------------------------
//    After the transmission begin , this funtion used to give event
//-----------------------------------------------------------------------------------
static void _SetCardEvent(SDIO_CARD_PAL_HANDLE handle,unsigned int EventId)
{
    handle->s_CardEvent |= EventId;
}

unsigned int GetCardEvent(SDIO_CARD_PAL_HANDLE handle)
{
    return handle->s_CardEvent;
}

//-----------------------------------------------------------------------------------
//    during the transmission ,all the event is given out by the interrupt,this function is called by the interrupt service to give the event to application
//-----------------------------------------------------------------------------------
static void  _irqCardProc(unsigned int msg, unsigned int errCode, SDHOST_SLOT_NO slotNum)
{
    SDIO_CARD_PAL_HANDLE handle;

    if(slotNum >=SDIO_CARD_PAL_SUPPORT_NUM ) slotNum = SDIO_CARD_PAL_SUPPORT_NUM - 1;
    handle = &s_sdioCardPalHd[slotNum];
    if(errCode!=0)
        SDIO_CARD_PRINT(("slot0  msg = 0x%08x err = 0x%08x\n",msg,errCode));
    if(0 != (SIG_CARD_IN&msg)){    // card interrupt happened , may be some operation error happened
        _SetCardEvent(handle,SIG_CARD_IN);
    }
    if(0 != (SIG_CARD_INSERT&msg)){    // card is inserted in slot
        _SetCardEvent(handle,SIG_CARD_INSERT);
    }
    else if(0 != (SIG_CARD_REMOVE&msg)){    // card is plug out from slot
        _SetCardEvent(handle,SIG_CARD_REMOVE);
    }
    else if(0 != (SIG_ERR&msg)){    // some error happened during the transmission
        if(ERR_DATA_TIMEOUT == errCode && (0 != (SIG_TRANS_CMP&msg)))
            errCode &= ~ERR_DATA_TIMEOUT;
        
        if(0!= errCode){
            s_CardErrCode |= errCode;    // get the error detail 
            _SetCardEvent(handle,SIG_ALL);
            return;
        }
    }
    if(0 != (SIG_BLK_CAP&msg))
        _SetCardEvent(handle,SIG_BLK_CAP);
    if(0 != (SIG_TRANS_CMP&msg))        // transmission complete
        _SetCardEvent(handle,SIG_TRANS_CMP);
    else if(0 != (SIG_DMA_INT&msg)){    // during the transmission ,the dma buffer is out of used ,we must set new dma buffer to continue the transmission
        volatile unsigned int nextAdd;
        nextAdd = SDHOST_GetDmaAddr(handle->sdio_port);
        SDHOST_SetDmaAddr(handle->sdio_port, nextAdd);
        SDHOST_NML_IntStatus_En(handle->sdio_port, SIG_DMA_INT);
        SDHOST_NML_IntSig_En(handle->sdio_port, SIG_DMA_INT);
    }
    
    //test code-----------------------------------------
    else if(0 != (SIG_BUF_RD_RDY&msg)){
        SDHOST_NML_IntStatus_En(handle->sdio_port, SIG_BUF_RD_RDY);
        SDHOST_NML_IntSig_En(handle->sdio_port, SIG_BUF_RD_RDY);
        _SetCardEvent(handle,SIG_BUF_RD_RDY);
    }else if(0 != (SIG_BUF_WD_RDY&msg)){
        SDHOST_NML_IntStatus_En(handle->sdio_port, SIG_BUF_WD_RDY);
        SDHOST_NML_IntSig_En(handle->sdio_port, SIG_BUF_WD_RDY);
        _SetCardEvent(handle,SIG_BUF_WD_RDY);
    }
    //test end--------------------------------------------
    
    if(0 != (SIG_CMD_CMP&msg)){        // during the transmission ,the command has be confirmed by card
        _SetCardEvent(handle,SIG_CMD_CMP);
    }
}

SDIO_CARD_PAL_SLOT_E Handle2SlotNo(SDIO_CARD_PAL_HANDLE handle)
{
	SDIO_CARD_PAL_SLOT_E slotNo;
	for(slotNo=SDIO_CARD_PAL_SLOT_0;SDIO_CARD_PAL_SUPPORT_NUM > slotNo;slotNo++)
		if(handle == &s_sdioCardPalHd[slotNo])
			return slotNo;
	return SDIO_CARD_PAL_SLOT_MAX;
}

SDIO_CARD_PAL_HANDLE SPRD_SDSlave_Pal_Open(SDIO_CARD_PAL_SLOT_E slotNo)
{

    SDIO_CARD_PAL_ASSERT(SDIO_CARD_PAL_SUPPORT_NUM > slotNo);
    
    if(TRUE == s_sdioCardPalHd[slotNo].flag)
        return NULL;

    s_sdioCardPalHd[slotNo].flag = TRUE;

    s_sdioCardPalHd[slotNo].MagicNum = SDIO_CARD_PAL_MAGICNUM;
    s_sdioCardPalHd[slotNo].s_CardEvent = 0;
    //s_sdioCardPalHd[slotNo].sdio_port = (SDHOST_HANDLE)SDHOST_Register((SDHOST_SLOT_NO)slotNo,_irqCardProc);
#if defined(CONFIG_SP7702)    
    s_sdioCardPalHd[slotNo].sdio_port = (SDHOST_HANDLE)SDHOST_Register((SDHOST_SLOT_NO)1,_irqCardProc);
#elif defined(CONFIG_SC7710G2)
    s_sdioCardPalHd[slotNo].sdio_port = (SDHOST_HANDLE)SDHOST_Register((SDHOST_SLOT_NO)2,_irqCardProc);
#else
	s_sdioCardPalHd[slotNo].sdio_port = (SDHOST_HANDLE)SDHOST_Register((SDHOST_SLOT_NO)0,_irqCardProc);
#endif
    if(s_sdioCardPalHd[slotNo].sdio_port == NULL){
        return NULL;
    }
    SDHOST_NML_IntStatus_En(s_sdioCardPalHd[slotNo].sdio_port, SIG_ALL|ERR_ALL);
    SDHOST_NML_IntSig_En(s_sdioCardPalHd[slotNo].sdio_port, SIG_ALL|ERR_ALL);
    return &s_sdioCardPalHd[slotNo];

}

int SPRD_SDSlave_Pal_Pwr(SDIO_CARD_PAL_HANDLE handle,SDIO_CARD_PAL_PWR_E onOrOff)
{
    SDIO_CARD_PAL_ASSERT(
        (SDIO_CARD_PAL_MAGICNUM == handle->MagicNum)
        &&(TRUE == handle->flag)
    );

    switch(onOrOff)
    {
        case SDIO_CARD_PAL_ON:
        {
            SDHOST_Cfg_BusWidth(handle->sdio_port,SDIO_1BIT_WIDTH);
            SDHOST_Cfg_SpeedMode(handle->sdio_port,SDIO_LOWSPEED);
            SDHOST_Cfg_Voltage(handle->sdio_port,VOL_3_0);

            SDHOST_SD_POWER(handle->sdio_port,POWR_ON);
            SDHOST_SD_Clk_Freq_Set(handle->sdio_port,SDIO_CLK_24M);
            SDHOST_internalClk_OnOff(handle->sdio_port,CLK_ON);
            SDHOST_SD_clk_OnOff(handle->sdio_port,CLK_ON);

            mdelay(2);
    //        OSTimeDlyHMSM (0, 0, 2, 0);
        }
        break;

        case SDIO_CARD_PAL_OFF:
        {
            SDHOST_SD_POWER(handle->sdio_port,POWR_OFF);
            SDHOST_RST(handle->sdio_port,RST_ALL);
            SDHOST_SD_clk_OnOff(handle->sdio_port,CLK_OFF);
            SDHOST_internalClk_OnOff(handle->sdio_port,CLK_OFF);
            mdelay(2);
    //        OSTimeDlyHMSM (0, 0, 0, 200);
        }
        break;

        default:
        {
            SDIO_CARD_PAL_ASSERT(0);
        }
        break;
    }

    return TRUE;
}

int SPRD_SDSlave_Pal_SetClk(SDIO_CARD_PAL_HANDLE handle,SDIO_CARD_PAL_CLKTYPE_E clkType)
{
    SDIO_CARD_PAL_ASSERT(
        (SDIO_CARD_PAL_MAGICNUM == handle->MagicNum)
        &&(TRUE == handle->flag)
    );


    switch(clkType)
    {
        case SDIO_CARD_PAL_400KHz:
        {
            SDHOST_SD_Clk_Freq_Set(handle->sdio_port,400000);
        }
        break;

        case SDIO_CARD_PAL_20MHz:
        {
            SDHOST_SD_Clk_Freq_Set(handle->sdio_port,20000000);
        }
        break;

        case SDIO_CARD_PAL_25MHz:
        {
            SDHOST_SD_Clk_Freq_Set(handle->sdio_port,25000000);
        }
        break;

        case SDIO_CARD_PAL_50MHz:
        {
            SDHOST_SD_Clk_Freq_Set(handle->sdio_port,50000000);
        }
        break;

        default:
        {
            SDIO_CARD_PAL_ASSERT(0);
        }
        break;

    }
    SDHOST_SD_clk_OnOff(handle->sdio_port,CLK_ON);

    return TRUE;

}

int SPRD_SDSlave_Pal_SetBusWidth(SDIO_CARD_PAL_HANDLE handle,SDIO_CARD_PAL_BUFWIDTH_E widthType)
{
    SDIO_CARD_PAL_ASSERT(
        (SDIO_CARD_PAL_MAGICNUM == handle->MagicNum)
        &&(TRUE == handle->flag)
    );

    switch(widthType)
    {
        case SDIO_CARD_PAL_1_BIT:
        {
            SDHOST_Cfg_BusWidth(handle->sdio_port,SDIO_1BIT_WIDTH);
        }
        break;

        case SDIO_CARD_PAL_4_BIT:
        {
            SDHOST_Cfg_BusWidth(handle->sdio_port,SDIO_4BIT_WIDTH);
        }
        break;

        case SDIO_CARD_PAL_8_BIT:
        {
            SDHOST_Cfg_BusWidth(handle->sdio_port,SDIO_8BIT_WIDTH);
        }
        break;

        default:
        {
            SDIO_CARD_PAL_ASSERT(0);
        }
        break;

    }

    return TRUE;
}

int SPRD_SDSlave_Pal_SetSpeedMode(SDIO_CARD_PAL_HANDLE handle,SDIO_CARD_PAL_SPEEDMODE_E speedMode)
{
    SDIO_CARD_PAL_ASSERT(
        (SDIO_CARD_PAL_MAGICNUM == handle->MagicNum)
        &&(TRUE == handle->flag)
    );
    return TRUE;
}



//-----------------------------------------------------------------------------------
//    Send command to card
//-----------------------------------------------------------------------------------
SDIO_CARD_PAL_ERROR_E SPRD_SDSlave_Pal_SendCmd(
    /*IN*/SDIO_CARD_PAL_HANDLE handle,
    /*IN*/SDIO_MASTER_PAL_CMD_E cmd,
    /*IN*/unsigned int argument,
    /*IN*/CARD_DATA_PARAM_T* dataParam,
    /*OUT*/unsigned char* rspBuf
)
{
    unsigned int tmpIntFilter;

    SDIO_CARD_PAL_ASSERT(
        (SDIO_CARD_PAL_MAGICNUM == handle->MagicNum)
        &&(TRUE == handle->flag)
    );
    SDIO_CARD_PAL_ASSERT(cmd == s_cmdDetail[cmd].cmd);
    SDHOST_SetDataTimeOutValue(handle->sdio_port,0xE);
    SDHOST_SetErrCodeFilter(handle->sdio_port, s_cmdDetail[cmd].errFilter);

    tmpIntFilter = s_cmdDetail[cmd].intFilter;
    if(0 != s_cmdDetail[cmd].errFilter)
    {
        tmpIntFilter |= SIG_ERR;
    }
    if(NULL != dataParam)
    {
        tmpIntFilter |= SIG_DMA_INT;
    }
    SDHOST_NML_IntStatus_En(handle->sdio_port, tmpIntFilter);
    SDHOST_NML_IntSig_En(handle->sdio_port, tmpIntFilter);
    
    
    _InitCardEvent(handle);

    if(NULL != dataParam)
    {
        //MMU_InvalideDCACHE();
        SDHOST_SetDmaAddr(handle->sdio_port, (unsigned int)(dataParam->databuf));
        SDHOST_SetDataParam(handle->sdio_port, dataParam->blkLen, dataParam->blkNum, SDIO_DMA_512K);
    }
    if(NULL != dataParam)
            ;//SDIO_CARD_PRINT(("\ncmd%d:(0x%08x,0x%x,0x%x)\n",cmd,argument,tmpIntFilter,s_cmdDetail[cmd].transmode));
    SDHOST_SetCmdArg(handle->sdio_port,argument);
    SDHOST_SetCmd(handle->sdio_port,s_cmdDetail[cmd].cmdIndex,s_cmdDetail[cmd].transmode,s_cmdDetail[cmd].cmdTyte, s_cmdDetail[cmd].Response);
#if 0
    if(NULL == dataParam){
	SDIO_CARD_PRINT(("\ncmd%d:(0x%08x 0x%x 0x%x)\n",cmd,argument,tmpIntFilter,s_cmdDetail[cmd].transmode));
    } else {
	 extern int req_clk_status(void);
         int status;
         do{
           status = req_clk_status();
           if(status == 0)
               ;//printf("(C) = %d \n",status);
         }while(status);
    }
#else
    do{
	//_SDHOST_IrqHandle(Handle2SlotNo(handle));
        _SDHOST_IrqHandle(0);
    }while(_WaitCardEvent(handle,s_cmdDetail[cmd].intFilter)==0);
#endif    
    
    if(0 != s_CardErrCode){
        SDIO_CARD_PRINT(("SDIO_Card error = 0x%x",s_CardErrCode));
        SDHOST_RST(handle->sdio_port,RST_CMD_DAT_LINE);
//        SDHOST_Dump_SDIO_Registers(0);
        return s_CardErrCode;
    }

    SDHOST_GetRspFromBuf(handle->sdio_port, s_cmdDetail[cmd].Response, rspBuf);

    return SDIO_CARD_PAL_ERR_NONE;
}

int SPRD_SDSlave_Pal_Close(SDIO_CARD_PAL_HANDLE handle)
{

    SDIO_CARD_PAL_ASSERT(
        (SDIO_CARD_PAL_MAGICNUM == handle->MagicNum)
        &&(TRUE == handle->flag)
    );

    handle->flag = FALSE;

    SDHOST_UnRegister(handle->sdio_port);

    handle->s_CardEvent = 0;
    return TRUE;
}

void SDIO_Card_Pal_SlotSelect(SDIO_CARD_PAL_SLOT_E slotNo)
{
    SDIO_CARD_PAL_SLOT_E i;
    SDIO_CARD_PAL_HANDLE handle;
    
    SDIO_CARD_PAL_ASSERT(SDIO_CARD_PAL_SUPPORT_NUM > slotNo);
    SDHOST_Slot_select(slotNo);
    for(i = 0; i < SDIO_CARD_PAL_SUPPORT_NUM; i ++)
    {
        handle = &s_sdioCardPalHd[i];
        if(NULL == handle->sdio_port)
        {
            continue;
        }
        SDHOST_SD_clk_OnOff(handle->sdio_port,CLK_ON);
    }
}

