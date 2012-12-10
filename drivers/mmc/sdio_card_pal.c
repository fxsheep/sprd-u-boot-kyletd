
#include "asm/arch/sci_types.h"
#include "asm/arch/sci_api.h"
#include "sdio_card_pal.h"
#include "sdhost_drv.h"
#include "asm/arch/mmu_drvapi.h"
#include "asm/arch/chng_freq.h"
#include "asm/arch/sc_reg.h"

#include "asm/arch/deep_sleep.h"
#include "asm/arch/deepsleep_drvapi.h"

#define DUAL_TCARD_SUPPORT
#ifndef OS_NONE
#define CARD_SDIO_EVENT
#endif

#undef NULL
#define NULL 0x0

//#define UCOS_BSD_EVENT

//#define DUAL_TCARD_SUPPORT
/*lint -save -e551*/
/*-----------------------------------------*/
//  Debug Function
/*-----------------------------------------*/
#define SDIO_CARD_PAL_ASSERT SCI_ASSERT
#if defined CONFIG_SC8825 && defined CONFIG_UBOOT_DEBUG
#define SDIO_CARD_PRINT(x) printf x
#else
#define SDIO_CARD_PRINT(x) SCI_TRACE_LOW x
#endif
/*-----------------------------------------*/
//  IRQ Function
/*-----------------------------------------*/
#if defined(OS_SELF_SIMULATOR)
#define SDIO_CARD_PAL_IRQ_DECLARE OS_CPU_SR  cpu_sr
#define SDIO_CARD_PAL_DISABLE_IRQ OS_ENTER_CRITICAL
#define SDIO_CARD_PAL_ENABLE_IRQ OS_EXIT_CRITICAL
#else
#define SDIO_CARD_PAL_IRQ_DECLARE
#define SDIO_CARD_PAL_DISABLE_IRQ //SCI_DisableIRQ
#define SDIO_CARD_PAL_ENABLE_IRQ //SCI_RestoreIRQ
#endif
/*-----------------------------------------*/


typedef struct
{
    SDIO_CARD_PAL_CMD_E cmd;
    uint32 cmdIndex;
    uint32 intFilter;
    CMD_RSP_TYPE_E Response;
    uint32 errFilter;
    uint32 transmode;
} CMD_CTL_FLG;


//      response Name       cmd int filter                                                      ,rsp            ,cmd error filter
#define CARD_SDIO_NO_RSP    NULL/*|SIG_CARD_IN|SIG_CARD_INSERT|SIG_CARD_REMOVE*/|SIG_CMD_CMP    ,CMD_NO_RSP     ,NULL
#define CARD_SDIO_R1        NULL/*|SIG_CARD_IN|SIG_CARD_INSERT|SIG_CARD_REMOVE*/|SIG_CMD_CMP    ,CMD_RSP_R1     ,ERR_RSP|ERR_CMD_INDEX| ERR_CMD_END|    ERR_CMD_CRC|    ERR_CMD_TIMEOUT
#define CARD_SDIO_R2        NULL/*|SIG_CARD_IN|SIG_CARD_INSERT|SIG_CARD_REMOVE*/|SIG_CMD_CMP    ,CMD_RSP_R2     ,ERR_RSP|               ERR_CMD_END|    ERR_CMD_CRC|    ERR_CMD_TIMEOUT
#define CARD_SDIO_R3        NULL/*|SIG_CARD_IN|SIG_CARD_INSERT|SIG_CARD_REMOVE*/|SIG_CMD_CMP    ,CMD_RSP_R3     ,ERR_RSP|               ERR_CMD_END|                    ERR_CMD_TIMEOUT
#define CARD_SDIO_R4        NULL/*|SIG_CARD_IN|SIG_CARD_INSERT|SIG_CARD_REMOVE*/|SIG_CMD_CMP    ,CMD_RSP_R4     ,ERR_RSP|ERR_CMD_INDEX| ERR_CMD_END|    ERR_CMD_CRC|    ERR_CMD_TIMEOUT
#define CARD_SDIO_R5        NULL/*|SIG_CARD_IN|SIG_CARD_INSERT|SIG_CARD_REMOVE*/|SIG_CMD_CMP    ,CMD_RSP_R5     ,ERR_RSP|ERR_CMD_INDEX| ERR_CMD_END|    ERR_CMD_CRC|    ERR_CMD_TIMEOUT
#define CARD_SDIO_R6        NULL/*|SIG_CARD_IN|SIG_CARD_INSERT|SIG_CARD_REMOVE*/|SIG_CMD_CMP    ,CMD_RSP_R6     ,ERR_RSP|ERR_CMD_INDEX| ERR_CMD_END|    ERR_CMD_CRC|    ERR_CMD_TIMEOUT
#define CARD_SDIO_R7        NULL/*|SIG_CARD_IN|SIG_CARD_INSERT|SIG_CARD_REMOVE*/|SIG_CMD_CMP    ,CMD_RSP_R7     ,ERR_RSP|ERR_CMD_INDEX| ERR_CMD_END|    ERR_CMD_CRC|    ERR_CMD_TIMEOUT
#define CARD_SDIO_R1B       NULL/*|SIG_CARD_IN|SIG_CARD_INSERT|SIG_CARD_REMOVE*/|SIG_CMD_CMP    ,CMD_RSP_R1B    ,ERR_RSP|ERR_CMD_INDEX| ERR_CMD_END|    ERR_CMD_CRC|    ERR_CMD_TIMEOUT
//#define CARD_SDIO_R5B     NULL/*|SIG_CARD_IN|SIG_CARD_INSERT|SIG_CARD_REMOVE*/|SIG_CMD_CMP    ,CMD_RSP_R5B    ,ERR_RSP|ERR_CMD_INDEX| ERR_CMD_END|    ERR_CMD_CRC|    ERR_CMD_TIMEOUT


static const CMD_CTL_FLG s_cmdDetail[] =
{
    // cmdindex,rsp,transmode
    //#define CMDname                       cmdindex    ,data int filter    +   (cmd int filter+)rsp(+cmd error filter) +   ,data error filter                      ,transmode
    {   CARD_CMD0_GO_IDLE_STATE                 ,0  ,NULL               |   CARD_SDIO_NO_RSP    |NULL                                                           ,NULL                                                                                                                               },
    //{   CARD_CMD0_GO_IDLE_STATE                 ,0  ,SIG_TRANS_CMP      |   CARD_SDIO_NO_RSP        |ERR_DATA_END|  ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,TRANS_MODE_MULTI_BLOCK|TRANS_MODE_READ                         |   TRANS_MODE_BLOCK_COUNT_EN|  TRANS_MODE_DMA_EN|  CMD_HAVE_DATA},
    {   CARD_CMD1_SEND_OP_COND /*MMC*/          ,1  ,NULL               |   CARD_SDIO_R3        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD2_ALL_SEND_CID                  ,2  ,NULL               |   CARD_SDIO_R2        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD3_SEND_RELATIVE_ADDR /*SD*/     ,3  ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD3_SET_RELATIVE_ADDR /*MMC*/     ,3  ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD4_SET_DSR                       ,4  ,NULL               |   CARD_SDIO_NO_RSP    |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD6_SWITCH_FUNC                   ,6  ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |ERR_DATA_END|  ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,                       TRANS_MODE_READ|                                                        TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_CMD7_SELECT_DESELECT_CARD          ,7  ,NULL               |   CARD_SDIO_R1       |NULL                                                           ,NULL                                                                                                                               },
    //{   CARD_CMD8_SEND_IF_COND /*SD*/           ,8  ,NULL               |   CARD_SDIO_R7        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD8_SEND_IF_COND /*SD*/           ,8  ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |ERR_DATA_END|  ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,                       TRANS_MODE_READ|                                                        TRANS_MODE_DMA_EN|  CMD_HAVE_DATA    },
    {   CARD_CMD9_SEND_CSD                      ,9  ,NULL               |   CARD_SDIO_R2        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD10_SEND_CID                     ,10 ,NULL               |   CARD_SDIO_R2        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD11_READ_DAT_UNTIL_STOP /*MMC*/  ,11 ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |ERR_DATA_END|  ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,TRANS_MODE_MULTI_BLOCK|TRANS_MODE_READ                         |   TRANS_MODE_BLOCK_COUNT_EN|  TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_CMD11_READ_DAT_UNTIL_STOP_AUT12 /*MMC*/,11,SIG_TRANS_CMP   |   CARD_SDIO_R1        |ERR_DATA_END|  ERR_DATA_CRC|   ERR_DATA_TIMEOUT    |ERR_CMD12  ,TRANS_MODE_MULTI_BLOCK|TRANS_MODE_READ|    TRANS_MODE_CMD12_EN|    TRANS_MODE_BLOCK_COUNT_EN|  TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_CMD12_STOP_TRANSMISSION            ,12 ,SIG_TRANS_CMP      |   CARD_SDIO_R1B       |NULL                                                           ,NULL                                                                                                                               },  //It is auto performed by Host
    {   CARD_CMD13_SEND_STATUS                  ,13 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD15_GO_INACTIVE_STATE            ,15 ,NULL               |   CARD_SDIO_NO_RSP    |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD16_SET_BLOCKLEN                 ,16 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD17_READ_SINGLE_BLOCK            ,17 ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |ERR_DATA_END|  ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,                       TRANS_MODE_READ|                                                        TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_CMD18_READ_MULTIPLE_BLOCK          ,18 ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |ERR_DATA_END|  ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,TRANS_MODE_MULTI_BLOCK|TRANS_MODE_READ                         |   TRANS_MODE_BLOCK_COUNT_EN|  TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_CMD18_READ_MULTIPLE_BLOCK_AUT12    ,18 ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |ERR_DATA_END|  ERR_DATA_CRC|   ERR_DATA_TIMEOUT    |ERR_CMD12  ,TRANS_MODE_MULTI_BLOCK|TRANS_MODE_READ|    TRANS_MODE_CMD12_EN |   TRANS_MODE_BLOCK_COUNT_EN|  TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_CMD20_WRITE_DAT_UNTIL_STOP /*MMC*/ ,20 ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |               ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,TRANS_MODE_MULTI_BLOCK                                         |   TRANS_MODE_BLOCK_COUNT_EN|  TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_CMD20_WRITE_DAT_UNTIL_STOP_AUT12 /*MMC*/,20,SIG_TRANS_CMP  |   CARD_SDIO_R1        |               ERR_DATA_CRC|   ERR_DATA_TIMEOUT    |ERR_CMD12  ,TRANS_MODE_MULTI_BLOCK                 |   TRANS_MODE_CMD12_EN |   TRANS_MODE_BLOCK_COUNT_EN|  TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_CMD23_SET_BLOCK_COUNT /*MMC*/      ,23 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD24_WRITE_BLOCK                  ,24 ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |               ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,                                                                                               TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_CMD25_WRITE_MULTIPLE_BLOCK         ,25 ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |               ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,TRANS_MODE_MULTI_BLOCK                                         |   TRANS_MODE_BLOCK_COUNT_EN|  TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_CMD25_WRITE_MULTIPLE_BLOCK_AUT12   ,25 ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |               ERR_DATA_CRC|   ERR_DATA_TIMEOUT    |ERR_CMD12  ,TRANS_MODE_MULTI_BLOCK                 |   TRANS_MODE_CMD12_EN |   TRANS_MODE_BLOCK_COUNT_EN|  TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },

    {   CARD_CMD26_PROGRAM_CID /*MMC*/          ,26 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD27_PROGRAM_CSD                  ,27 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD28_SET_WRITE_PROT               ,28 ,SIG_TRANS_CMP      |   CARD_SDIO_R1B       |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD29_CLR_WRITE_PROT               ,29 ,SIG_TRANS_CMP      |   CARD_SDIO_R1B       |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD30_SEND_WRITE_PROT              ,30 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },  // ??

    {   CARD_CMD32_ERASE_WR_BLK_START /*SD*/    ,32 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD33_ERASE_WR_BLK_END /*SD*/      ,33 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },

    {   CARD_CMD35_ERASE_GROUP_START /*MMC*/    ,35 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD36_ERASE_GROUP_END /*MMC*/      ,36 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },

    {   CARD_CMD38_ERASE                        ,38 ,SIG_TRANS_CMP      |   CARD_SDIO_R1B       |NULL                                                           ,NULL                                                                                                                               },

    {   CARD_CMD39_FAST_IO /*MMC*/              ,39 ,NULL               |   CARD_SDIO_R4        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD40_GO_IRQ_STATE /*MMC*/         ,40 ,NULL               |   CARD_SDIO_R5        |NULL                                                           ,NULL                                                                                                                               },

    {   CARD_CMD42_LOCK_UNLOCK_SD /*SD*/        ,42 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },  // ??
    {   CARD_CMD42_LOCK_UNLOCK_MMC /*MMC*/      ,42 ,NULL               |   CARD_SDIO_R1B       |NULL                                                           ,NULL                                                                                                                               },  // ??
    {   CARD_CMD55_APP_CMD                      ,55 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD56_GEN_CMD_SD /*SD*/            ,56 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },  //??
    {   CARD_CMD56_GEN_CMD_MMC /*MMC*/          ,56 ,NULL               |   CARD_SDIO_R1B       |NULL                                                           ,NULL                                                                                                                               },  //??


    {   CARD_ACMD6_SET_BUS_WIDTH     /*SD*/     ,6  ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_ACMD13_SD_STATUS /*SD*/            ,13 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_ACMD22_SEND_NUM_WR_BLCOKS /*SD*/   ,22 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_ACMD23_SET_WR_BLK_ERASE_COUNT /*SD*/,23,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_ACMD41_SD_SEND_OP_COND /*SD*/      ,41 ,NULL               |   CARD_SDIO_R3        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_ACMD42_SET_CLR_CARD_DETECT /*SD*/  ,42 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_ACMD51_SEND_SCR /*SD*/             ,51 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },  // ??

    {   CARD_ACMD6_SET_EXT_CSD     /*SD*/     ,6  ,NULL               |   CARD_SDIO_R1B        |NULL                                                           ,NULL                                                                                                                               },

#if defined(SPRD_SUPPORT_MCEX)
    {   CARD_CMD34_READ_SEC_CMD                 ,34 ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |ERR_DATA_END|  ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,                       TRANS_MODE_READ|                                                        TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_CMD35_WRITE_SEC_CMD                ,35 ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |               ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,                                                                                               TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_CMD36_SEND_PSI                     ,36 ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |ERR_DATA_END|  ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,                       TRANS_MODE_READ|                                                        TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_CMD37_CTL_TRM                      ,37 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
#endif

    {   CARD_CMDMAX                             ,0  ,NULL               |   CARD_SDIO_NO_RSP    |NULL                                                           ,NULL                                                                                                                               }

};

#define SDIO_CARD_PAL_MAGICNUM 0x5344494F

typedef struct SDIO_CARD_PAL_Struct
{
    BOOLEAN flag;
    uint32 MagicNum;            // "SDIO"

    SDHOST_HANDLE sdio_port;

#if defined (UCOS_BSD_EVENT)
    OS_FLAG_GRP *sdio_event;
#elif defined (CARD_SDIO_EVENT)
    SCI_EVENT_GROUP_PTR  sdio_event;
#endif

    SDIO_CARD_PAL_SLOT_E sdio_No;
} SDIO_CARD_PAL_Struct_T;

#define SDIO_CARD_PAL_SUPPORT_NUM 2
#if defined (UCOS_BSD_EVENT)
#elif defined (CARD_SDIO_EVENT)
LOCAL SDIO_CARD_PAL_Struct_T s_sdioCardPalHd[SDIO_CARD_PAL_SUPPORT_NUM] = {{FALSE,0,0,0,0},{FALSE,0,0,0,0}};
#else
LOCAL SDIO_CARD_PAL_Struct_T s_sdioCardPalHd[SDIO_CARD_PAL_SUPPORT_NUM] = {{FALSE,0,0},{FALSE,0,0}};
#endif

#ifdef DUAL_TCARD_SUPPORT
LOCAL SDIO_CARD_PAL_SLOT_E s_activeslot = SDIO_CARD_PAL_SLOT_MAX;
#endif

#if defined (UCOS_BSD_EVENT)

#elif defined (CARD_SDIO_EVENT)

#else
LOCAL volatile uint32 s_CardEvent = 0;
#endif
LOCAL volatile uint32 s_CardErrCode = 0;

//-----------------------------------------------------------------------------------
//  Clear all event  happened before ,and prepare next transmission
//-----------------------------------------------------------------------------------
LOCAL void _InitCardEvent (SDIO_CARD_PAL_HANDLE handle)
{
    s_CardErrCode = 0;
#if defined (UCOS_BSD_EVENT)
    {
        INT8U err;
        OSFlagPost (handle->sdio_event, 0xFFFFFFFF, OS_FLAG_CLR, &err);
    }
#elif defined (CARD_SDIO_EVENT)
    SCI_SetEvent (handle->sdio_event,0,SCI_AND);
#else
    s_CardEvent = 0;
#endif
}

//-----------------------------------------------------------------------------------
//  After the transmission begin , wait event during the transmission
//-----------------------------------------------------------------------------------
LOCAL uint32 _WaitCardEvent (SDIO_CARD_PAL_HANDLE handle,uint32 EventId)
{
#if defined (UCOS_BSD_EVENT)
    INT8U err;
    OSFlagPend (handle->sdio_event, EventId, OS_FLAG_WAIT_SET_ALL|OS_FLAG_CONSUME, 1000, &err);

    if (OS_TIMEOUT == err)
    {
        s_CardErrCode |= BIT_31;
    }

#elif defined (CARD_SDIO_EVENT)
    {
        uint32 actualFlag;
        uint32 eRet;

				eRet = SCI_GetEvent (handle->sdio_event,EventId,SCI_AND_CLEAR,&actualFlag,3000);
        if (SCI_SUCCESS != eRet)
        {
            s_CardErrCode |= BIT_31;
            SDIO_CARD_PRINT(("SCM Wait event error!!! eRet = 0x%x",eRet));
        }
    }
#else

    //  CARD_PRINT(("SD20 Wait event before %x",EventId));
    if(EventId == (s_CardEvent&EventId))
    {
        return 0;
    }
    else
    {
	return 1;
    }

    //  CARD_PRINT(("SD20 Wait event end %x",s_CardEvent));
#endif
}

//-----------------------------------------------------------------------------------
//  After the transmission begin , this funtion used to give event
//-----------------------------------------------------------------------------------
LOCAL void _SetCardEvent (SDIO_CARD_PAL_HANDLE handle,uint32 EventId)
{
#if defined (UCOS_BSD_EVENT)
    INT8U err;
    OSFlagPost (handle->sdio_event, EventId, OS_FLAG_SET, &err);
#elif defined (CARD_SDIO_EVENT)
    SCI_SetEvent (handle->sdio_event,EventId,SCI_OR);
#else
    s_CardEvent |= EventId;
#endif
}

//-----------------------------------------------------------------------------------
//  during the transmission ,all the event is given out by the interrupt,this function is called by the interrupt service to give the event to application
//-----------------------------------------------------------------------------------
#define DEBUG_SDIO
#ifdef DEBUG_SDIO
LOCAL uint32 s_irqCardProc_msg;
LOCAL uint32 s_sdio_dma_addr[4] = {NULL};
#endif
LOCAL void  _irqCardProc (uint32 msg, uint32 errCode, SDHOST_SLOT_NO slotNum)
{
    SDIO_CARD_PAL_HANDLE handle;
    handle = &s_sdioCardPalHd[slotNum];

    if (0 != (SIG_CARD_IN&msg)) // card interrupt happened , may be some operation error happened
    {
        _SetCardEvent (handle,SIG_CARD_IN);
    }

    if (0 != (SIG_CARD_INSERT&msg)) // card is inserted in slot
    {
        _SetCardEvent (handle,SIG_CARD_INSERT);
    }
    else if (0 != (SIG_CARD_REMOVE&msg)) // card is plug out from slot
    {
        _SetCardEvent (handle,SIG_CARD_REMOVE);
    }
    else if (0 != (SIG_ERR&msg)) // some error happened during the transmission
    {
        s_CardErrCode |= errCode;   // get the error detail
        _SetCardEvent (handle,SIG_ALL);

        return;
    }
    if (0 != (SIG_TRANS_CMP&msg))       // transmission complete
    {
        _SetCardEvent (handle,SIG_TRANS_CMP);
    }
    else if (0 != (SIG_DMA_INT&msg)) // during the transmission ,the dma buffer is out of used ,we must set new dma buffer to continue the transmission
    {
        volatile uint32 nextAdd;
#ifdef DEBUG_SDIO
        s_irqCardProc_msg = msg;
#endif
        nextAdd = SDHOST_GetDmaAddr (handle->sdio_port);
        SDHOST_SetDmaAddr (handle->sdio_port, nextAdd);
#ifdef DEBUG_SDIO
        s_sdio_dma_addr[3] = SDHOST_GetDmaAddr (handle->sdio_port);
#endif
        SDHOST_NML_IntStatus_En (handle->sdio_port, SIG_DMA_INT);
        SDHOST_NML_IntSig_En (handle->sdio_port, SIG_DMA_INT);
    }

    if (0 != (SIG_CMD_CMP&msg))     // during the transmission ,the command has be confirmed by card
    {
        _SetCardEvent (handle,SIG_CMD_CMP);
    }
}

#ifdef DUAL_TCARD_SUPPORT
LOCAL void _SlotSelect(SDIO_CARD_PAL_SLOT_E slotNo)
{
    SDIO_CARD_PAL_SLOT_E i;
    SDIO_CARD_PAL_HANDLE handle;
    
    SDIO_CARD_PAL_ASSERT(SDIO_CARD_PAL_SUPPORT_NUM > slotNo);	/*assert verified*/

    SDHOST_Slot_select((SDHOST_SLOT_NO)slotNo);
    
    for(i = 0; i < SDIO_CARD_PAL_SUPPORT_NUM; i ++)
    {
        handle = &s_sdioCardPalHd[i];
        if(PNULL == handle->sdio_port)
        {
            continue;
        }
        if(slotNo == i)
        {
			SDHOST_SD_clk_OnOff(handle->sdio_port,CLK_ON);
        }
        else
        {
			SDHOST_SD_clk_OnOff(handle->sdio_port,CLK_OFF);
        }
    }
}
#endif


PUBLIC SDIO_CARD_PAL_HANDLE SDIO_Card_Pal_Open (SDIO_CARD_PAL_SLOT_E slotNo)
{
    SDIO_CARD_PAL_IRQ_DECLARE;

    SDIO_CARD_PAL_ASSERT (SDIO_CARD_PAL_SUPPORT_NUM > slotNo);	/*assert verified*/

    // 检查是否有有效的handle存在
    SDIO_CARD_PAL_DISABLE_IRQ;

    if (TRUE == s_sdioCardPalHd[slotNo].flag)
    {
        SDIO_CARD_PAL_ENABLE_IRQ;
        SDIO_CARD_PAL_ASSERT (0);	/*assert verified*/
    }

    s_sdioCardPalHd[slotNo].flag = TRUE;
    SDIO_CARD_PAL_ENABLE_IRQ;

    s_sdioCardPalHd[slotNo].MagicNum = SDIO_CARD_PAL_MAGICNUM;
#if defined (UCOS_BSD_EVENT)
    {
        INT8U err;
        s_sdioCardPalHd[slotNo].sdio_event = PNULL;
        s_sdioCardPalHd[slotNo].sdio_event = OSFlagCreate (0, &err);

        if (PNULL == s_sdioCardPalHd[slotNo].sdio_event)
        {
            SDIO_CARD_PAL_ASSERT (0);	/*assert verified*/
        }
    }
#elif defined (CARD_SDIO_EVENT)
    s_sdioCardPalHd[slotNo].sdio_event = PNULL;
    s_sdioCardPalHd[slotNo].sdio_event = SCI_CreateEvent ("SDIO_EVENT");

    if (PNULL == s_sdioCardPalHd[slotNo].sdio_event)
    {
        SDIO_CARD_PAL_ASSERT (0);	/*assert verified*/
    }

#else
    s_CardEvent = 0;

#endif
    s_sdioCardPalHd[slotNo].sdio_port = (SDHOST_HANDLE) SDHOST_Register ( (SDHOST_SLOT_NO) slotNo,_irqCardProc);

    if (0 == s_sdioCardPalHd[slotNo].sdio_port)
    {
        SDIO_CARD_PAL_ASSERT (0);	/*assert verified*/
    }
	
#ifdef DUAL_TCARD_SUPPORT
    s_sdioCardPalHd[slotNo].sdio_No = slotNo;
    s_activeslot = slotNo;
    _SlotSelect(slotNo);
#endif

    return &s_sdioCardPalHd[slotNo];

}

PUBLIC BOOLEAN SDIO_Card_Pal_Pwr (SDIO_CARD_PAL_HANDLE handle,SDIO_CARD_PAL_PWR_E onOrOff)
{
    // 此模块只提供给Card_sdio.c这个协议使用，协议代码本身已经保证该handle的有效性，因此在这里不做特别严的检查
    SDIO_CARD_PAL_ASSERT (	/*assert verified*/
        (SDIO_CARD_PAL_MAGICNUM == handle->MagicNum)
        && (TRUE == handle->flag)
    );

#ifdef DUAL_TCARD_SUPPORT
    switch(handle->sdio_No)
    {
        case SDIO_CARD_PAL_SLOT_0:
        case SDIO_CARD_PAL_SLOT_1:
        {
             if(s_activeslot !=  handle->sdio_No)
            {
                 s_activeslot = handle->sdio_No;
                 _SlotSelect(handle->sdio_No); 
             }
         }
	  break;

	default:
	{
            SDIO_CARD_PAL_ASSERT (0);	/*assert verified*/
        }
	break;
    }
#endif	
		
    switch (onOrOff)
    {
        case SDIO_CARD_PAL_ON:
            {
                SDHOST_RST(handle->sdio_port, RST_MODULE);
                SDHOST_Cfg_BusWidth (handle->sdio_port,SDIO_1BIT_WIDTH);
#if defined (CONFIG_SC8825) || defined(CONFIG_SC7710G2)
                SDHOST_Cfg_SpeedMode (handle->sdio_port, EMMC_SDR12);
#else
                SDHOST_Cfg_SpeedMode (handle->sdio_port,SDIO_LOWSPEED);
#endif
                SDHOST_Cfg_Voltage (handle->sdio_port,VOL_3_0);

                SDHOST_SD_POWER (handle->sdio_port,POWR_ON);
                SDHOST_SD_Clk_Freq_Set (handle->sdio_port,400000);
                SDHOST_SD_clk_OnOff (handle->sdio_port,CLK_ON);
                udelay(1000);
                SDHOST_internalClk_OnOff (handle->sdio_port,CLK_ON);
                udelay(1000);
            }
            break;

        case SDIO_CARD_PAL_OFF:
            {
                SDHOST_SD_POWER (handle->sdio_port,POWR_OFF);
                SDHOST_RST (handle->sdio_port,RST_ALL);
                SDHOST_SD_clk_OnOff (handle->sdio_port,CLK_OFF);
                SDHOST_internalClk_OnOff (handle->sdio_port,CLK_OFF);
                udelay(1000);
                SDHOST_SD_clk_OnOff (handle->sdio_port,CLK_OFF);
                udelay(1000);
            }
            break;

        default:
            {
                SDIO_CARD_PAL_ASSERT (0);	/*assert verified*/
            }
            break;
    }

    return TRUE;
}

PUBLIC BOOLEAN SDIO_Card_Pal_SetClk (SDIO_CARD_PAL_HANDLE handle,SDIO_CARD_PAL_CLKTYPE_E clkType)
{
    SDIO_CARD_PAL_ASSERT (	/*assert verified*/
        (SDIO_CARD_PAL_MAGICNUM == handle->MagicNum)
        && (TRUE == handle->flag)
    );

#ifdef DUAL_TCARD_SUPPORT
    switch(handle->sdio_No)
    {
        case SDIO_CARD_PAL_SLOT_0:
        case SDIO_CARD_PAL_SLOT_1:
        {
             if(s_activeslot !=  handle->sdio_No)
            {
                 s_activeslot = handle->sdio_No;
                 _SlotSelect(handle->sdio_No); 
             }
         }
	  break;

	default:
	{
            SDIO_CARD_PAL_ASSERT (0);	/*assert verified*/
        }
	break;
    }
#endif

    SDHOST_internalClk_OnOff (handle->sdio_port,CLK_OFF);
    udelay(1000);
    SDHOST_SD_clk_OnOff (handle->sdio_port,CLK_OFF);
    udelay(1000);

    switch (clkType)
    {
        case SDIO_CARD_PAL_400KHz:
            {
                SDHOST_SD_Clk_Freq_Set (handle->sdio_port,400000);
            }
            break;
#if defined (CONFIG_SC8825) || defined(CONFIG_SC7710G2)
		case SDIO_CARD_PAL_1MHz:
                SDHOST_SD_Clk_Freq_Set (handle->sdio_port,1000000);
				break;
		case SDIO_CARD_PAL_2MHz:
                SDHOST_SD_Clk_Freq_Set (handle->sdio_port,2000000);
				break;
		case SDIO_CARD_PAL_4MHz:
                SDHOST_SD_Clk_Freq_Set (handle->sdio_port,4000000);
				break;
		case SDIO_CARD_PAL_8MHz:
                SDHOST_SD_Clk_Freq_Set (handle->sdio_port,8000000);
				break;
		case SDIO_CARD_PAL_12MHz:
                SDHOST_SD_Clk_Freq_Set (handle->sdio_port,12000000);
				break;
#endif				
        case SDIO_CARD_PAL_20MHz:
            {
                SDHOST_SD_Clk_Freq_Set (handle->sdio_port,20000000);
            }
            break;

        case SDIO_CARD_PAL_25MHz:
            {
                SDHOST_SD_Clk_Freq_Set (handle->sdio_port,25000000);
            }
            break;

        case SDIO_CARD_PAL_50MHz:
            {
                SDHOST_SD_Clk_Freq_Set (handle->sdio_port,50000000);
            }
            break;

        default:
            {
                SDIO_CARD_PAL_ASSERT (0);	/*assert verified*/
            }
            break;

    }

    SDHOST_SD_clk_OnOff (handle->sdio_port,CLK_ON);
    udelay(1000);
    SDHOST_internalClk_OnOff (handle->sdio_port,CLK_ON);
    udelay(1000);
    return TRUE;

}

PUBLIC BOOLEAN SDIO_Card_Pal_SetBusWidth (SDIO_CARD_PAL_HANDLE handle,SDIO_CARD_PAL_BUFWIDTH_E widthType)
{
    // 此模块只提供给Card_sdio.c这个协议使用，协议代码本身已经保证该handle的有效性，因此在这里不做特别严的检查
    SDIO_CARD_PAL_ASSERT (	/*assert verified*/
        (SDIO_CARD_PAL_MAGICNUM == handle->MagicNum)
        && (TRUE == handle->flag)
    );

#ifdef DUAL_TCARD_SUPPORT
    switch(handle->sdio_No)
    {
        case SDIO_CARD_PAL_SLOT_0:
        case SDIO_CARD_PAL_SLOT_1:
        {
             if(s_activeslot !=  handle->sdio_No)
            {
                 s_activeslot = handle->sdio_No;
                 _SlotSelect(handle->sdio_No); 
             }
         }
	  break;

	default:
	{
            SDIO_CARD_PAL_ASSERT (0);	/*assert verified*/
        }
	break;
    }
#endif	

    switch (widthType)
    {
        case SDIO_CARD_PAL_1_BIT:
            {
                SDHOST_Cfg_BusWidth (handle->sdio_port,SDIO_1BIT_WIDTH);
            }
            break;

        case SDIO_CARD_PAL_4_BIT:
            {
                SDHOST_Cfg_BusWidth (handle->sdio_port,SDIO_4BIT_WIDTH);
            }
            break;

        case SDIO_CARD_PAL_8_BIT:
            {
                SDHOST_Cfg_BusWidth (handle->sdio_port,SDIO_8BIT_WIDTH);
            }
            break;

        default:
            {
                SDIO_CARD_PAL_ASSERT (0);	/*assert verified*/
            }
            break;

    }

    return TRUE;
}

PUBLIC BOOLEAN SDIO_Card_Pal_SetSpeedMode (SDIO_CARD_PAL_HANDLE handle,SDIO_CARD_PAL_SPEEDMODE_E speedMode)
{
    // 此模块只提供给Card_sdio.c这个协议使用，协议代码本身已经保证该handle的有效性，因此在这里不做特别严的检查
    SDIO_CARD_PAL_ASSERT (	/*assert verified*/
        (SDIO_CARD_PAL_MAGICNUM == handle->MagicNum)
        && (TRUE == handle->flag)
    );
#if defined (CONFIG_SC8825) || defined(CONFIG_SC7710G2)
	switch (speedMode)
	{
		case EMMC_SPEED_SDR12:
			SDHOST_Cfg_SpeedMode (handle->sdio_port,EMMC_SDR12);
			break;
		case EMMC_SPEED_SDR25:
			SDHOST_Cfg_SpeedMode (handle->sdio_port,EMMC_SDR25);
			break;
		case EMMC_SPEED_SDR50:
			SDHOST_Cfg_SpeedMode (handle->sdio_port,EMMC_SDR50);
			break;
		case EMMC_SPEED_SDR104:
			SDHOST_Cfg_SpeedMode (handle->sdio_port,EMMC_SDR104);
			break;
		case EMMC_SPEED_DDR50:
			SDHOST_Cfg_SpeedMode (handle->sdio_port,EMMC_DDR50);
			break;
		default:
			return FALSE;
	}
#else
#ifdef DUAL_TCARD_SUPPORT
    switch(handle->sdio_No)
    {
        case SDIO_CARD_PAL_SLOT_0:
        case SDIO_CARD_PAL_SLOT_1:
        {
             if(s_activeslot !=  handle->sdio_No)
            {
                 s_activeslot = handle->sdio_No;
                 _SlotSelect(handle->sdio_No); 
             }
         }
	  break;

	default:
	{
            SDIO_CARD_PAL_ASSERT (0);	/*assert verified*/
        }
	break;
    }
#endif	
    // dummy function
    #if 0
    switch(speedMode)
    {
        case SDIO_CARD_PAL_LOW_SPEED:
        {
            SDHOST_Cfg_SpeedMode(handle->sdio_port,SDIO_LOWSPEED);
        }
        break;

        case SDIO_CARD_PAL_HIGH_SPEED:
        {
            SDHOST_Cfg_SpeedMode(handle->sdio_port,SDIO_HIGHSPEED);
        }
        break;

        default:
        {
            SDIO_CARD_PAL_ASSERT(0);	/*assert verified*/
        }
        break;

    }
    #endif

    // if ahb_clk < 96M, then not enter highspeed mode.
    /*if(CHIP_GetAhbClk() < 96000000)
        return FALSE;
    else  */
#endif
        return TRUE;

}



//-----------------------------------------------------------------------------------
//  Send command to card
//-----------------------------------------------------------------------------------
#ifdef DEBUG_SDIO
LOCAL CARD_DATA_PARAM_T s_dataParam;
#endif
PUBLIC SDIO_CARD_PAL_ERROR_E SDIO_Card_Pal_SendCmd (
    /*IN*/SDIO_CARD_PAL_HANDLE handle,
    /*IN*/SDIO_CARD_PAL_CMD_E cmd,
    /*IN*/uint32 argument,
    /*IN*/CARD_DATA_PARAM_T *dataParam,
    /*OUT*/uint8 *rspBuf
)
{
    uint32 tmpIntFilter;

#ifdef OS_NONE
   	uint32  isr_status;
#endif
	SDIO_CARD_PRINT(("%s : cmd:%x, cmdIndex:%x, argument:%x\r\n", __FUNCTION__, cmd, s_cmdDetail[cmd].cmdIndex, argument));
	
    SDIO_CARD_PAL_ASSERT (	/*assert verified*/
        (SDIO_CARD_PAL_MAGICNUM == handle->MagicNum)
        && (TRUE == handle->flag)
    );
    SDIO_CARD_PAL_ASSERT (cmd == s_cmdDetail[cmd].cmd);	/*assert verified*/

#ifdef DUAL_TCARD_SUPPORT
    switch(handle->sdio_No)
    {
        case SDIO_CARD_PAL_SLOT_0:
        case SDIO_CARD_PAL_SLOT_1:
        {
             if(s_activeslot !=  handle->sdio_No)
            {
                 s_activeslot = handle->sdio_No;
                 _SlotSelect(handle->sdio_No); 
             }
         }
	  break;

	default:
	{
            SDIO_CARD_PAL_ASSERT (0);	/*assert verified*/
        }
	break;
    }
#endif	

#ifndef OS_NONE    
    SCI_SDIO_EnableDeepSleep (DISABLE_AHB_SLEEP);
    CHNG_FREQ_Event_Clr (FREQ_CHNG_EVENT_SDIO,SYS_MODE_NORMAL);
#endif

    SDHOST_NML_IntSig_Dis (handle->sdio_port, SIG_ALL);
    SDHOST_NML_IntStatus_Dis (handle->sdio_port, SIG_ALL); // ???
    SDHOST_NML_IntStatus_Clr (handle->sdio_port, SIG_ALL);

    //--
    SDHOST_SetDataTimeOutValue (handle->sdio_port,0xE);

    //--
    SDHOST_SetErrCodeFilter (handle->sdio_port, s_cmdDetail[cmd].errFilter);

    tmpIntFilter = s_cmdDetail[cmd].intFilter;

    if (NULL != s_cmdDetail[cmd].errFilter)
    {
        tmpIntFilter |= SIG_ERR;
    }

    if (NULL != dataParam)
    {
        tmpIntFilter |= SIG_DMA_INT;
    }

    SDHOST_NML_IntStatus_En (handle->sdio_port, tmpIntFilter);
    SDHOST_NML_IntSig_En (handle->sdio_port, tmpIntFilter);

#ifdef OS_NONE
	*(volatile uint32 *)(INT_IRQ_EN) |= (0x1<<TB_SDIO1_INT);
#endif

    _InitCardEvent (handle);

    if (NULL != dataParam)
    {
        uint32 bufferSize = 0;
#ifdef DEBUG_SDIO
        s_dataParam = *dataParam;
#endif
        bufferSize = dataParam->blkLen  *  (dataParam->blkNum);
        //if(dataParam->direction == SDIO_DMA_OUT)
        {
               Dcache_CleanRegion((unsigned int)(dataParam->databuf), bufferSize);
               Dcache_InvalRegion((unsigned int)(dataParam->databuf), bufferSize);
        }
        //MMU_DmaCacheSync( (uint32) (dataParam->databuf), bufferSize, DMABUFFER_BIDIRECTIONAL);
       
        SDHOST_SetDmaAddr (handle->sdio_port, (uint32) (dataParam->databuf));
        SDHOST_SetDataParam (handle->sdio_port, dataParam->blkLen, dataParam->blkNum, SDIO_DMA_512K);
#ifdef DEBUG_SDIO
        s_sdio_dma_addr[0] = SDHOST_GetDmaAddr (handle->sdio_port);
#endif
    }

    SDHOST_SetCmdArg (handle->sdio_port,argument);
    //__udelay(1000);
    SDHOST_SetCmd (handle->sdio_port,s_cmdDetail[cmd].cmdIndex,s_cmdDetail[cmd].transmode,CMD_TYPE_NORMAL, s_cmdDetail[cmd].Response);
    //__udelay(1000);
    #if 0
        // Get interrupt status.
	while(0 == (isr_status & (0x1<<TB_SDIO1_INT))){
            isr_status = * (volatile uint32 *) (INT_IRQ_STS);
            }
	_SDHOST_IrqHandle(TB_SDIO1_INT);

    #ifdef DEBUG_SDIO
        if (NULL != dataParam)
        {
            s_sdio_dma_addr[1] = SDHOST_GetDmaAddr (handle->sdio_port);
        }
    #endif
#endif
//---
	while (0 != _WaitCardEvent(handle,s_cmdDetail[cmd].intFilter))
	{
		if(0 != (TB_SDIO1_INT&0x0000FFFF))
		{
			_SDHOST_IrqHandle(TB_SDIO1_INT);
		}
	}
//---
   
    SDHOST_RST (handle->sdio_port,RST_CMD_DAT_LINE);

#ifndef OS_NONE	
    SCI_SDIO_EnableDeepSleep (ENABLE_AHB_SLEEP);
    CHNG_FREQ_Event_Set (FREQ_CHNG_EVENT_SDIO,SYS_MODE_NORMAL);
#endif

    if (0 != s_CardErrCode)
    {
        if (
            (CARD_CMD38_ERASE == cmd)
            && (ERR_DATA_TIMEOUT == s_CardErrCode)
        )
        {
            /*
                该case用于防止，R1b的busy信号过长超过SDIO的TimeOut时间。
                一般较大的SD卡擦除时间较长。
            */
            uint32 tmOut;

            SDIO_CARD_PRINT ( ("SDIO_Card may be erase R1b is too long"));
            tmOut = SCI_GetTickCount();

            while (0 == (BIT_20&SDHOST_GetPinState (handle->sdio_port)))
            {
                __udelay (1*1000);

                if ( (tmOut+20000) < SCI_GetTickCount())
                {
                    #ifdef DEBUG_SDIO
                        if (NULL != dataParam)
                        {
                            s_sdio_dma_addr[2] = SDHOST_GetDmaAddr (handle->sdio_port);
                        }
                    #endif
                    SDHOST_RST (handle->sdio_port,RST_CMD_DAT_LINE);
                    return s_CardErrCode;
                }
            }
        }
        else
        {
            SDIO_CARD_PRINT ( ("SDIO_Card error = 0x%x",s_CardErrCode));
            return s_CardErrCode;
        }
    }

      /* if (NULL != dataParam)
       {
	  if(dataParam->direction == SDIO_DMA_IN)		   
			      Dcache_InvalRegion((unsigned int)(dataParam->databuf), dataParam->blkLen  *  dataParam->blkNum);
       }*/

    SDHOST_GetRspFromBuf (handle->sdio_port, s_cmdDetail[cmd].Response, rspBuf);

    return SDIO_CARD_PAL_ERR_NONE;

}

PUBLIC BOOLEAN SDIO_Card_Pal_Close (SDIO_CARD_PAL_HANDLE handle)
{
    SDIO_CARD_PAL_IRQ_DECLARE;

    // 此模块只提供给Card_sdio.c这个协议使用，协议代码本身已经保证该handle的有效性，因此在这里不做特别严的检查
    SDIO_CARD_PAL_ASSERT (	/*assert verified*/
        (SDIO_CARD_PAL_MAGICNUM == handle->MagicNum)
        && (TRUE == handle->flag)
    );

    SDIO_CARD_PAL_DISABLE_IRQ;
    handle->flag = FALSE;
    SDIO_CARD_PAL_ENABLE_IRQ;

    SDHOST_UnRegister (handle->sdio_port);

#if defined (UCOS_BSD_EVENT)
    {
        INT8U err;
        OSFlagDel (handle->sdio_event, OS_DEL_ALWAYS, &err);
        handle->sdio_event = PNULL;
    }
#elif defined (CARD_SDIO_EVENT)
    SCI_DeleteEvent (handle->sdio_event);
    handle->sdio_event = PNULL;
#else
    s_CardEvent = 0;
#endif

    return TRUE;
}

#ifdef CONFIG_EMMC_BOOT
uint32 SCI_GetTickCount(void)
{
	volatile uint32 tmp_tick1;
	volatile uint32 tmp_tick2;

	tmp_tick1 = SYSTEM_CURRENT_CLOCK;
	tmp_tick2 = SYSTEM_CURRENT_CLOCK;

	while (tmp_tick1 != tmp_tick2)
  {
	  tmp_tick1 = tmp_tick2;
	  tmp_tick2 = SYSTEM_CURRENT_CLOCK;
	  }

	return tmp_tick1;
}
#endif

