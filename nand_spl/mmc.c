#include <config.h>
#include "asm/arch/sci_types.h"
#if defined (CONFIG_TIGER)
#include "asm/arch/sc8810_reg_global.h" 
#include "asm/arch/sc8810_reg_ahb.h"
#include "asm/arch/sc8810_module_config.h" 
#else
#include "asm/arch/sc8810_reg_global.h" 
#include "asm/arch/sc8810_reg_ahb.h"
#include "asm/arch/sc8810_module_config.h" 
#endif

#include <asm/arch/ldo.h>
#include <asm/arch/ldo_reg_v3.h>
#include <asm/arch/sdio_reg_v3.h>
#include <asm/arch/int_reg_v3.h>
#include <asm/arch/sys_timer_reg_v0.h>

#define     SDIO_BASE_CLK_48M       48000000        // 48 MHz
#define     SDIO_BASE_CLK_384M      384000000        // 384 MHz


#define PARTITION_CFG_BOOT_PARTITION_ENABLE_OFFSET   3
#define PARTITION_CFG_BOOT_PARTITION_ENABLE_BOOT1   (1<<PARTITION_CFG_BOOT_PARTITION_ENABLE_OFFSET)

#define PARTITION_CFG_BOOT_PARTITION_ACCESS_PARTITION_BOOT2	2

#define EXT_CSD_CARD_WIDTH_1_BIT		0
#define EXT_CSD_CARD_WIDTH_4_BIT		1
#define EXT_CSD_CARD_WIDTH_8_BIT		2

#define EXT_CSD_PARTITION_CFG_INDEX	179
#define EXT_CSD_BUS_WIDTH_INDEX		183

#define CMD6_BIT_MODE_OFFSET_ACCESS	24
#define CMD6_BIT_MODE_MASK_ACCESS	0x03000000
#define CMD6_BIT_MODE_OFFSET_INDEX		16
#define CMD6_BIT_MODE_MASK_INDEX	0x00FF0000
#define CMD6_BIT_MODE_OFFSET_VALUE		8
#define CMD6_BIT_MODE_MASK_VALUE	0x0000FF00
#define CMD6_BIT_MODE_OFFSET_CMD_SET	0
#define CMD6_BIT_MODE_MASK_CMD_SET	0x00000003

#define CMD6_ACCESS_MODE_COMMAND_SET	(0<<CMD6_BIT_MODE_OFFSET_ACCESS)
#define CMD6_ACCESS_MODE_SET_BITS	(1<<CMD6_BIT_MODE_OFFSET_ACCESS)
#define CMD6_ACCESS_MODE_CLEAR_BITS	(2<<CMD6_BIT_MODE_OFFSET_ACCESS)
#define CMD6_ACCESS_MODE_WRITE_BYTE	(3<<CMD6_BIT_MODE_OFFSET_ACCESS)

#define CMD6_CMD_SET_OLD	(0<<CMD6_BIT_MODE_OFFSET_CMD_SET)
#define CMD6_CMD_SET		(1<<CMD6_BIT_MODE_OFFSET_CMD_SET)

#define EXT_CSD_BUS_WIDTH_INDEX	183	/* R/W */
#define EXT_CSD_HS_TIMING_INDEX	185	/* R/W */
#define EXT_CSD_CARD_TYPE_INDEX	196	/* RO */
#define EXT_CSD_SEC_CNT_INDEX	212	/* RO, 4 bytes */
#define EXT_CSD_PARTITION_CFG_INDEX	179



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

typedef struct CARD_DATA_PARAM_TAG
{
	uint8* databuf;	// the buffer address ,that data will be stored in or  read from
	uint32 blkLen;	// block size
	uint32 blkNum;	// block number
}
CARD_DATA_PARAM_T;

typedef enum CARD_VERTION_E_TAG
{
	CARD_V_UNKONWN,
	CARD_SD_V1_X,
	CARD_SD_V2_0_STANDARD,
	CARD_SD_V2_0_HIGHCAP,
	CARD_MMC_331,
	CARD_MMC_441_STANDARD,
	CARD_MMC_441_HIGHCAP
}CARD_VERTION_E;

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

typedef enum
{
	SDIO_CARD_PAL_SLOT_0,
	SDIO_CARD_PAL_SLOT_1,
	SDIO_CARD_PAL_SLOT_2,
	SDIO_CARD_PAL_SLOT_3,
	SDIO_CARD_PAL_SLOT_4,
	SDIO_CARD_PAL_SLOT_5,
	SDIO_CARD_PAL_SLOT_6,
	SDIO_CARD_PAL_SLOT_7
,
	SDIO_CARD_PAL_SLOT_MAX
}
SDIO_CARD_PAL_SLOT_E;

typedef enum
{
	SDIO_CARD_PAL_ERR_NONE			= 0,
	SDIO_CARD_PAL_ERR_RSP				= BIT_0,
	SDIO_CARD_PAL_ERR_CMD12			= BIT_1,
	SDIO_CARD_PAL_ERR_CUR_LIMIT		= BIT_2,
	SDIO_CARD_PAL_ERR_DATA_END		= BIT_3,
	SDIO_CARD_PAL_ERR_DATA_CRC		= BIT_4,
	SDIO_CARD_PAL_ERR_DATA_TIMEOUT	= BIT_5,
	SDIO_CARD_PAL_ERR_CMD_INDEX		= BIT_6,
	SDIO_CARD_PAL_ERR_CMD_END		= BIT_7,
	SDIO_CARD_PAL_ERR_CMD_CRC		= BIT_8,
	SDIO_CARD_PAL_ERR_CMD_TIMEOUT	= BIT_9
}
SDIO_CARD_PAL_ERROR_E;


typedef void (*SDIO_CALLBACK)(uint32 msg,uint32 errCode,SDHOST_SLOT_NO slotNum);


typedef struct SDHOST_PORT_T_TAG
{
    volatile SDIO_REG_CFG *host_cfg;
    BOOLEAN open_flag;
    uint32 baseClock;
    uint32 sdClock;
    uint32 err_filter;
    SDIO_CALLBACK sigCallBack;
} SDHOST_PORT_T;

typedef struct SDHOST_PORT_T_TAG* SDHOST_HANDLE;

typedef struct SDIO_CARD_PAL_Struct
{
    SDHOST_HANDLE sdio_port;
    SDIO_CARD_PAL_SLOT_E sdio_No;
} SDIO_CARD_PAL_Struct_T;

typedef struct SDIO_CARD_PAL_Struct* SDIO_CARD_PAL_HANDLE;

typedef enum CARD_BUS_WIDTH_TAG
{
	CARD_WIDTH_1_BIT,
	CARD_WIDTH_4_BIT,
	CARD_WIDTH_8_BIT
}CARD_BUS_WIDTH_E;


typedef struct CARD_PORT_TAG
{

	SDIO_CARD_PAL_HANDLE sdioPalHd;
	uint16 RCA;
	CARD_BUS_WIDTH_E bus_width;
	uint32 BlockLen;

}CARD_PORT_T;


typedef enum SDHOST_RST_TYPE_E_TAG
{
	RST_CMD_LINE,
	RST_DAT_LINE,
	RST_CMD_DAT_LINE,
	RST_ALL,
	RST_MODULE
}
SDHOST_RST_TYPE_E;

//---ÃüÁîÏà¹Ø
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

typedef enum
{
	NORMAL_SPEED_MODE,
	HIGH_SPEED_MODE,
	SPEED_MODE_MAX
}
CARD_SPEED_MODE;


LOCAL SDHOST_PORT_T sdio_port_ctl[SDHOST_SLOT_MAX_NUM];

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


typedef enum
{
// cmdindex,rsp,transmode
	CARD_CMD0_GO_IDLE_STATE,
	CARD_CMD1_SEND_OP_COND, /*MMC*/
	CARD_CMD2_ALL_SEND_CID,
	CARD_CMD3_SET_RELATIVE_ADDR, /*MMC*/
	CARD_CMD7_SELECT_DESELECT_CARD,
	CARD_CMD12_STOP_TRANSMISSION,	//It is auto performed by Host
	CARD_CMD13_SEND_STATUS,
	CARD_CMD16_SET_BLOCKLEN,
	CARD_CMD18_READ_MULTIPLE_BLOCK,
        CARD_ACMD6_SET_EXT_CSD,
	CARD_CMDMAX
}
SDIO_CARD_PAL_CMD_E;

typedef struct
{
    SDIO_CARD_PAL_CMD_E cmd;
    uint32 cmdIndex;
    uint32 intFilter;
    CMD_RSP_TYPE_E Response;
    uint32 errFilter;
    uint32 transmode;
} CMD_CTL_FLG;

typedef enum SDHOST_CLK_ONOFF_E_TAG
{
	CLK_ON,
	CLK_OFF
}
SDHOST_CLK_ONOFF_E;

typedef struct
{
    uint32 msg;
    uint32 errCode;
    SDHOST_SLOT_NO slotNum;
    SDHOST_HANDLE pSdhost_handler;

} ISR_Buffer_T;


typedef struct
{
	LDO_ID_E id;
	unsigned int bp_reg;
	unsigned int bp;
        #ifdef CONFIG_SC7710G2
	unsigned int bp_rst_reg;
        #endif
	unsigned int bp_rst;
	unsigned int level_reg_b0;
	unsigned int b0;
	unsigned int b0_rst;
	unsigned int level_reg_b1;
	unsigned int b1;
	unsigned int b1_rst;
	unsigned int valid_time;
	unsigned int init_level;
	int ref;
}LDO_CTL_T, * LDO_CTL_PTR;

typedef struct
{
	SLP_LDO_E id;
	unsigned int ldo_reg;
	unsigned int mask;
	SLP_BIT_DEF_E value;
	int valid;
	unsigned int reserved;
}SLP_LDO_CTL_T, * SLP_LDO_CTL_PTR;


typedef enum
{
	SDIO_CARD_PAL_OFF,
	SDIO_CARD_PAL_ON
}SDIO_CARD_PAL_PWR_E;


#if defined (CONFIG_SC8825) 
LDO_CTL_T ldo_ctl_data_sdio3 =
    {
        LDO_LDO_SDIO3,  ANA_LDO_PD_CTL1, BIT_4,  BIT_5,  ANA_LDO_VCTL4,  BIT_4, BIT_5,
        ANA_LDO_VCTL4,  BIT_7, BIT_6, NULL,   LDO_VOLT_LEVEL_FAULT_MAX,     NULL
    };
LDO_CTL_T ldo_ctl_data_vdd30 =
    {
        LDO_LDO_VDD30,  ANA_LDO_PD_CTL1, BIT_6,  BIT_7,  ANA_LDO_VCTL1,  BIT_8,BIT_9,
        ANA_LDO_VCTL1,  BIT_10, BIT_11, NULL,   LDO_VOLT_LEVEL_FAULT_MAX,     NULL
    };
#elif defined (CONFIG_SC7710G2)
LDO_CTL_T ldo_ctl_data_sdio3 =//LDO_EMMIO
    {
        LDO_LDO_SDIO3,  ANA_LDO_PD_SET, BIT_10,  ANA_LDO_PD_RST, BIT_10,  ANA_LDO_VCTL4,  BIT_12, BIT_13,
        ANA_LDO_VCTL4,  BIT_14, BIT_15, NULL,   LDO_VOLT_LEVEL_FAULT_MAX,     NULL
    };
LDO_CTL_T ldo_ctl_data_vdd30 =//LDO_EMMCORE
    {
        LDO_LDO_VDD30,  ANA_LDO_PD_SET, BIT_9,  ANA_LDO_PD_RST, BIT_9,  ANA_LDO_VCTL4,  BIT_8,BIT_9,
        ANA_LDO_VCTL4,  BIT_10, BIT_11, NULL,   LDO_VOLT_LEVEL_FAULT_MAX,     NULL
    };
#else
LDO_CTL_T ldo_ctl_data_sim2 =
     {
        LDO_LDO_SIM2,  ANA_LDO_PD_CTL1, BIT_6,  BIT_7,  ANA_LDO_VCTL4,  BIT_8,  BIT_9,
        ANA_LDO_VCTL4,  BIT_10,  BIT_11,  NULL,   LDO_VOLT_LEVEL_FAULT_MAX,     NULL
    };

LDO_CTL_T ldo_ctl_data_sdio1 =
    {
        LDO_LDO_SDIO1,  ANA_LDO_PD_CTL1, BIT_0,  BIT_1,  ANA_LDO_VCTL3,  BIT_12,BIT_13,
        ANA_LDO_VCTL3,  BIT_14, BIT_15, NULL,   LDO_VOLT_LEVEL_FAULT_MAX,     NULL
    };
#endif

//SLP_LDO_CTL_T slp_ldo_ctl_data_sdio1=  {SLP_LDO_SDIO1,     ANA_LDO_SLP_CTL0,    BIT_15, SLP_BIT_SET,    TRUE,   NULL};
//SLP_LDO_CTL_T slp_ldo_ctl_data_sim2=  {SLP_LDO_SIM2,      ANA_LDO_SLP_CTL2,    BIT_2,  SLP_BIT_SET,    TRUE,   NULL};


#define NULL 0x0

static const CMD_CTL_FLG s_cmdDetail[] =
{
    // cmdindex,rsp,transmode
    //#define CMDname                       cmdindex    ,data int filter    +   (cmd int filter+)rsp(+cmd error filter) +   ,data error filter                      ,transmode
    {   CARD_CMD0_GO_IDLE_STATE                 ,0  ,NULL               |   CARD_SDIO_NO_RSP    |NULL                                                           ,NULL                                                                                                                               },
    //{   CARD_CMD0_GO_IDLE_STATE                 ,0  ,SIG_TRANS_CMP      |   CARD_SDIO_NO_RSP        |ERR_DATA_END|  ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,TRANS_MODE_MULTI_BLOCK|TRANS_MODE_READ                         |   TRANS_MODE_BLOCK_COUNT_EN|  TRANS_MODE_DMA_EN|  CMD_HAVE_DATA},
    {   CARD_CMD1_SEND_OP_COND /*MMC*/          ,1  ,NULL               |   CARD_SDIO_R3        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD2_ALL_SEND_CID                  ,2  ,NULL               |   CARD_SDIO_R2        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD3_SET_RELATIVE_ADDR /*MMC*/     ,3  ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD7_SELECT_DESELECT_CARD          ,7  ,NULL               |   CARD_SDIO_R1       |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD12_STOP_TRANSMISSION            ,12 ,SIG_TRANS_CMP      |   CARD_SDIO_R1B       |NULL                                                           ,NULL                                                                                                                               },  //It is auto performed by Host
    {   CARD_CMD13_SEND_STATUS                  ,13 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD16_SET_BLOCKLEN                 ,16 ,NULL               |   CARD_SDIO_R1        |NULL                                                           ,NULL                                                                                                                               },
    {   CARD_CMD18_READ_MULTIPLE_BLOCK          ,18 ,SIG_TRANS_CMP      |   CARD_SDIO_R1        |ERR_DATA_END|  ERR_DATA_CRC|   ERR_DATA_TIMEOUT                ,TRANS_MODE_MULTI_BLOCK|TRANS_MODE_READ                         |   TRANS_MODE_BLOCK_COUNT_EN|  TRANS_MODE_DMA_EN|  CMD_HAVE_DATA   },
    {   CARD_ACMD6_SET_EXT_CSD     /*SD*/     ,6  ,NULL               |   CARD_SDIO_R1B        |NULL                                                           ,NULL                                                                                                                               },

    {   CARD_CMDMAX                             ,0  ,NULL               |   CARD_SDIO_NO_RSP    |NULL                                                           ,NULL                                                                                                                               }

};



typedef struct CARD_PORT_TAG* CARD_SDIO_HANDLE;


LOCAL volatile uint32 s_CardErrCode = 0;
CARD_PORT_T cadport ;
CARD_SDIO_HANDLE  emmc_handle = NULL;
LOCAL volatile uint32 s_CardEvent = 0;
LOCAL SDIO_CARD_PAL_Struct_T s_sdioCardPalHd[2] = {{0,0},{0,0}};

PUBLIC uint32 SDHOST_GetDmaAddr (SDHOST_HANDLE sdhost_handler)
{
    uint32 dmaAddr;
    dmaAddr = sdhost_handler->host_cfg->DMA_SYS_ADD;
    return dmaAddr;
}

PUBLIC void SDHOST_SetDmaAddr (SDHOST_HANDLE sdhost_handler, uint32 dmaAddr)
{
    sdhost_handler->host_cfg->DMA_SYS_ADD = dmaAddr;
}

LOCAL void _SetCardEvent (SDIO_CARD_PAL_HANDLE handle,uint32 EventId)
{
    s_CardEvent |= EventId;
}

PUBLIC uint32 SDHOST_GetNMLIntStatus (SDHOST_HANDLE sdhost_handler)
{
    volatile uint32 tmpReg;
    uint32 msg;

    tmpReg = sdhost_handler->host_cfg->INT_STA;
    msg = 0;

    if (0 != (tmpReg & BIT_15))
    {
        msg |= SIG_ERR;
    }

    if (0 != (tmpReg & BIT_8))
    {
        msg |= SIG_CARD_IN;
    }

    if (0 != (tmpReg & BIT_6))
    {
        msg |= SIG_CARD_INSERT;
    }

    if (0 != (tmpReg & BIT_7))
    {
        msg |= SIG_CARD_REMOVE;
    }

    if (0 != (tmpReg & BIT_5))
    {
        msg |= SIG_BUF_RD_RDY;
    }

    if (0 != (tmpReg & BIT_4))
    {
        msg |= SIG_BUF_WD_RDY;
    }

    if (0 != (tmpReg & BIT_3))
    {
        msg |= SIG_DMA_INT;
    }

    if (0 != (tmpReg & BIT_2))
    {
        msg |= SIG_BLK_CAP;
    }

    if (0 != (tmpReg & BIT_1))
    {
        msg |= SIG_TRANS_CMP;
    }

    if (0 != (tmpReg & BIT_0))
    {
        msg |= SIG_CMD_CMP;
    }

    return msg;
}

PUBLIC uint32 SDHOST_GetErrCode (SDHOST_HANDLE sdhost_handler)
{
    volatile uint32 tmpReg;
    uint32 err_msg;

    tmpReg = sdhost_handler->host_cfg->INT_STA;
    err_msg = 0;

    if (0 != (tmpReg & BIT_28))
    {
        err_msg |= ERR_RSP;
    }

    if (0 != (tmpReg & BIT_24))
    {
        err_msg |= ERR_CMD12;
    }

    if (0 != (tmpReg & BIT_23))
    {
        err_msg |= ERR_CUR_LIMIT;
    }

    if (0 != (tmpReg & BIT_22))
    {
        err_msg |= ERR_DATA_END;
    }

    if (0 != (tmpReg & BIT_21))
    {
        err_msg |= ERR_DATA_CRC;
    }

    if (0 != (tmpReg & BIT_20))
    {
        err_msg |= ERR_DATA_TIMEOUT;
    }

    if (0 != (tmpReg & BIT_19))
    {
        err_msg |= ERR_CMD_INDEX;
    }

    if (0 != (tmpReg & BIT_18))
    {
        err_msg |= ERR_CMD_END;
    }

    if (0 != (tmpReg & BIT_17))
    {
        err_msg |= ERR_CMD_CRC;
    }

    if (0 != (tmpReg & BIT_16))
    {
        err_msg |= ERR_CMD_TIMEOUT;
    }

    return err_msg;
}


PUBLIC ISR_EXE_T _SDHOST_IrqHandle (uint32 isrnum)
{
    ISR_Buffer_T buffer;
    SDHOST_HANDLE sdhost_handler;

    //buffer.slotNum = _GetIntSDHOSTSlotNum();
#if defined (CONFIG_TIGER) || defined (CONFIG_SC7710G2)
    buffer.slotNum =  SDHOST_SLOT_7;
#else
    buffer.slotNum =  SDHOST_SLOT_1;
#endif
    sdhost_handler = &sdio_port_ctl[buffer.slotNum];
    buffer.pSdhost_handler = &sdio_port_ctl[buffer.slotNum];

    buffer.msg = SDHOST_GetNMLIntStatus (sdhost_handler);
    buffer.errCode = SDHOST_GetErrCode (sdhost_handler);

    SDHOST_NML_IntSig_Dis (sdhost_handler, buffer.msg);
    SDHOST_NML_IntStatus_Dis (sdhost_handler, buffer.msg);
    SDHOST_NML_IntStatus_Clr (sdhost_handler, buffer.msg);
    sdhost_handler->sigCallBack (buffer.msg, buffer.errCode, buffer.slotNum);
    return CALL_HISR;
}


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
        nextAdd = SDHOST_GetDmaAddr (handle->sdio_port);
        SDHOST_SetDmaAddr (handle->sdio_port, nextAdd);
        SDHOST_NML_IntStatus_En (handle->sdio_port, SIG_DMA_INT);
        SDHOST_NML_IntSig_En (handle->sdio_port, SIG_DMA_INT);
    }

    if (0 != (SIG_CMD_CMP&msg))     // during the transmission ,the command has be confirmed by card
    {
        _SetCardEvent (handle,SIG_CMD_CMP);
    }
}

PUBLIC void SDHOST_SD_clk_On (SDHOST_HANDLE sdhost_handler)
{
                sdhost_handler->host_cfg->HOST_CTL1 |= BIT_0;
                while (0 == (sdhost_handler->host_cfg->HOST_CTL1 & BIT_1)) {}
                sdhost_handler->host_cfg->HOST_CTL1 |= BIT_2;
                while (0 == (sdhost_handler->host_cfg->HOST_CTL1 & BIT_1)) {} //maybe it is not nessarry
}

PUBLIC uint32 SDHOST_BaseClk_Set (uint32 sdio_base_clk)
{
    uint32 clk = 0;
    //Select the clk source of SDIO
#if defined (CONFIG_TIGER)
     REG32 (GR_CLK_GEN5) &= ~ (BIT_23|BIT_24);
     clk = SDIO_BASE_CLK_384M;
#elif defined (CONFIG_SC7710G2)
    REG32 (GR_CLK_GEN7) = (REG32 (GR_CLK_GEN7) & (~ (BIT_23|BIT_24))) | BIT_23;
    clk = SDIO_BASE_CLK_384M;
#else
    REG32 (GR_CLK_GEN5) &= ~ (BIT_17|BIT_18);
     clk = SDIO_BASE_CLK_48M;
     REG32 (GR_CLK_GEN5) |= (2<<19);
#endif
    //REG32 (GR_CLK_GEN5) |= (2<<17);
        
    return clk;
}

PUBLIC SDHOST_HANDLE SDHOST_Register (SDHOST_SLOT_NO slot_NO,SDIO_CALLBACK fun)
{
    uint32 status = 0, i = 0;

#if defined (CONFIG_TIGER)
     REG32 (AHB_CTL0)     |= BIT_23;
     REG32 (AHB_SOFT_RST) |= BIT_21;
     REG32 (AHB_SOFT_RST) &= ~BIT_21;
     sdio_port_ctl[slot_NO].open_flag = TRUE;
     sdio_port_ctl[slot_NO].baseClock = SDHOST_BaseClk_Set (SDIO_BASE_CLK_384M);
#elif defined(CONFIG_SC7710G2)
    REG32 (AHB_CTL6)     |= BIT_1;
    REG32 (AHB_SOFT2_RST) |= BIT_1;
    REG32 (AHB_SOFT2_RST) &= ~BIT_1;
    sdio_port_ctl[slot_NO].open_flag = TRUE;
    sdio_port_ctl[slot_NO].baseClock = SDHOST_BaseClk_Set (SDIO_BASE_CLK_384M);
#else
     REG32 (AHB_CTL0)     |= BIT_19;
     REG32 (AHB_SOFT_RST) |= BIT_16;
     REG32 (AHB_SOFT_RST) &= ~BIT_16;
     sdio_port_ctl[slot_NO].open_flag = TRUE;
     sdio_port_ctl[slot_NO].baseClock = SDHOST_BaseClk_Set (SDIO_BASE_CLK_48M);
#endif

    switch (slot_NO)
    {
        case SDHOST_SLOT_0:
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) SDIO0_BASE_ADDR);
            }
            break;

        case SDHOST_SLOT_1:
            {
#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) (SDIO0_BASE_ADDR+0x100) );
#else
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) SDIO1_BASE_ADDR);
#endif				
            }
            break;
#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
        case SDHOST_SLOT_2:
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) (SDIO0_BASE_ADDR+0x200) );
            }
            break;
        case SDHOST_SLOT_3:
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) SDIO1_BASE_ADDR );
            }
            break;
        case SDHOST_SLOT_4:
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) (SDIO1_BASE_ADDR+0x100) );
            }
            break;
        case SDHOST_SLOT_5:
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) (SDIO1_BASE_ADDR+0x200) );
            }
            break;
        case SDHOST_SLOT_6:
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) SDIO2_BASE_ADDR );
            }
            break;
        case SDHOST_SLOT_7:
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) EMMC_BASE_ADDR );
            }
            break;
#endif			

    }

    sdio_port_ctl[slot_NO].sigCallBack = fun;
    sdio_port_ctl[slot_NO].err_filter = 0;
    SDHOST_RST (&sdio_port_ctl[slot_NO],RST_ALL);
    SDHOST_internalClk_On (&sdio_port_ctl[slot_NO]);
    //_GetSDHOSTCapbility (&sdio_port_ctl[slot_NO],& (sdio_port_ctl[slot_NO].capbility));
    SDHOST_RST (&sdio_port_ctl[slot_NO],RST_ALL);

    return &sdio_port_ctl[slot_NO];
}



PUBLIC SDIO_CARD_PAL_HANDLE SDIO_Card_Pal_Open (SDIO_CARD_PAL_SLOT_E slotNo)
{
    s_sdioCardPalHd[slotNo].sdio_port = (SDHOST_HANDLE) SDHOST_Register ( (SDHOST_SLOT_NO) slotNo,_irqCardProc);
    s_sdioCardPalHd[slotNo].sdio_No = slotNo;
   * (volatile uint32 *) AHB_SDIO_CTL = (AHB_SDIO_CTRL_SLOT0); //select master1
    SDHOST_SD_clk_On(s_sdioCardPalHd[slotNo].sdio_port);
    return &s_sdioCardPalHd[slotNo];
}


LOCAL uint32 _WaitCardEvent (SDIO_CARD_PAL_HANDLE handle,uint32 EventId)
{
    if(EventId == (s_CardEvent&EventId))
    {
        return 0;
    }
    else
    {
	return 1;
    }
}


PUBLIC SDIO_CARD_PAL_ERROR_E SDIO_Card_Pal_SendCmd (
    /*IN*/SDIO_CARD_PAL_HANDLE handle,
    /*IN*/SDIO_CARD_PAL_CMD_E cmd,
    /*IN*/uint32 argument,
    /*IN*/CARD_DATA_PARAM_T *dataParam,
    /*OUT*/uint8 *rspBuf
)
{
    uint32 tmpIntFilter;
    uint32  isr_status;
	
   SDHOST_SD_clk_On(handle->sdio_port);

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

    *(volatile uint32 *)(INT_IRQ_EN) |= (0x1<<TB_SDIO1_INT);

    //_InitCardEvent (handle);
    s_CardErrCode = 0;
    s_CardEvent = 0;
    if (NULL != dataParam)
    {
        uint32 bufferSize = 0;
        bufferSize = dataParam->blkLen  *  (dataParam->blkNum);
        SDHOST_SetDmaAddr (handle->sdio_port, (uint32) (dataParam->databuf));
        SDHOST_SetDataParam (handle->sdio_port, dataParam->blkLen, dataParam->blkNum);
    }

    //SDHOST_SetCmdArg (handle->sdio_port,argument);
        handle->sdio_port->host_cfg->CMD_ARGUMENT = argument;
    SDHOST_SetCmd (handle->sdio_port,s_cmdDetail[cmd].cmdIndex,s_cmdDetail[cmd].transmode,CMD_TYPE_NORMAL, s_cmdDetail[cmd].Response);

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

    SDHOST_GetRspFromBuf (handle->sdio_port, s_cmdDetail[cmd].Response, rspBuf);

    return SDIO_CARD_PAL_ERR_NONE;

}

PUBLIC void SDHOST_NML_IntSig_Dis (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntSig_Dis (sdhost_handler,ERR_ALL);
        tmpReg |= (BIT_28|BIT_24|BIT_23|BIT_22|BIT_21|
                   BIT_20|BIT_19|BIT_18|BIT_17|BIT_16);

    }

    if (0 != (msg&SIG_CARD_IN))
    {
		tmpReg |= BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
		tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
		tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
		tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
		tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
		tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
		tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
		tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
		tmpReg |= BIT_0;
    }
	sdhost_handler->host_cfg->INT_SIG_EN &= ~tmpReg;

}


PUBLIC void SDHOST_NML_IntStatus_Dis (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntStatus_Dis (sdhost_handler,ERR_ALL);
        tmpReg |= (BIT_28|BIT_24|BIT_23|BIT_22|BIT_21|
                   BIT_20|BIT_19|BIT_18|BIT_17|BIT_16);

    }

    if (0 != (msg&SIG_CARD_IN))
    {
		tmpReg |= BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
		tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
		tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
		tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
		tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
		tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
		tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
		tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
		tmpReg |= BIT_0;
    }
    sdhost_handler->host_cfg->INT_STA_EN &= ~tmpReg;

}


PUBLIC void SDHOST_NML_IntStatus_Clr (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntStatus_Clr (sdhost_handler,ERR_ALL);
        tmpReg |= (BIT_28|BIT_24|BIT_23|BIT_22|BIT_21|
                   BIT_20|BIT_19|BIT_18|BIT_17|BIT_16);

    }

    if (0 != (msg&SIG_CARD_IN))
    {
        //sdhost_handler->host_cfg->INT_STA =BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
		tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
		tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
		tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
		tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
		tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
		tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
		tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
		tmpReg |= BIT_0;
    }
    sdhost_handler->host_cfg->INT_STA = tmpReg;

}

PUBLIC void SDHOST_SetDataTimeOutValue (SDHOST_HANDLE sdhost_handler,uint8 clk_cnt) // (2 ^ (clkcnt+13))*T_BSCLK
{
    volatile uint32 tmpReg,tmpIntReg;

    tmpIntReg = sdhost_handler->host_cfg->INT_STA_EN;
    // cfg the data timeout clk----------
    sdhost_handler->host_cfg->INT_STA_EN &= ~BIT_20;

    tmpReg = sdhost_handler->host_cfg->HOST_CTL1;
    tmpReg &= ~ (0xF << 16);
    tmpReg |= (clk_cnt << 16);
    sdhost_handler->host_cfg->HOST_CTL1 = tmpReg;

    sdhost_handler->host_cfg->INT_STA_EN = tmpIntReg;
}

PUBLIC void SDHOST_SetErrCodeFilter (SDHOST_HANDLE sdhost_handler,uint32 err_msg)
{
    sdhost_handler->err_filter = err_msg;
}

PUBLIC void SDHOST_NML_IntStatus_En (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;
    uint32 err_msg;

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntStatus_En (sdhost_handler,sdhost_handler->err_filter);
        err_msg = sdhost_handler->err_filter;
        if (0 != (err_msg&ERR_RSP))
        {
    		tmpReg |= BIT_28;
        }

        if (0 != (err_msg&ERR_CMD12))
        {
    		tmpReg |= BIT_24;
        }

        if (0 != (err_msg&ERR_CUR_LIMIT))
        {
    		tmpReg |= BIT_23;
        }

        if (0 != (err_msg&ERR_DATA_END))
        {
    		tmpReg |= BIT_22;
        }

        if (0 != (err_msg&ERR_DATA_CRC))
        {
    		tmpReg |= BIT_21;
        }

        if (0 != (err_msg&ERR_DATA_TIMEOUT))
        {
    		tmpReg |= BIT_20;
        }

        if (0 != (err_msg&ERR_CMD_INDEX))
        {
    		tmpReg |= BIT_19;
        }

        if (0 != (err_msg&ERR_CMD_END))
        {
    		tmpReg |= BIT_18;
        }

        if (0 != (err_msg&ERR_CMD_CRC))
        {
    		tmpReg |= BIT_17;
        }

        if (0 != (err_msg&ERR_CMD_TIMEOUT))
        {
    		tmpReg |= BIT_16;
        }

    }

    if (0 != (msg&SIG_CARD_IN))
    {
		tmpReg |= BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
		tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
		tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
		tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
		tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
		tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
		tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
		tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
		tmpReg |= BIT_0;
    }
    sdhost_handler->host_cfg->INT_STA_EN |= tmpReg;

}

PUBLIC void SDHOST_NML_IntSig_En (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;
    uint32 err_msg;

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntSig_En (sdhost_handler,sdhost_handler->err_filter);
        err_msg = sdhost_handler->err_filter;
        if (0 != (err_msg&ERR_RSP))
        {
    		tmpReg |= BIT_28;
        }

        if (0 != (err_msg&ERR_CMD12))
        {
    		tmpReg |= BIT_24;
        }

        if (0 != (err_msg&ERR_CUR_LIMIT))
        {
    		tmpReg |= BIT_23;
        }

        if (0 != (err_msg&ERR_DATA_END))
        {
    		tmpReg |= BIT_22;
        }

        if (0 != (err_msg&ERR_DATA_CRC))
        {
    		tmpReg |= BIT_21;
        }

        if (0 != (err_msg&ERR_DATA_TIMEOUT))
        {
    		tmpReg |= BIT_20;
        }

        if (0 != (err_msg&ERR_CMD_INDEX))
        {
    		tmpReg |= BIT_19;
        }

        if (0 != (err_msg&ERR_CMD_END))
        {
    		tmpReg |= BIT_18;
        }

        if (0 != (err_msg&ERR_CMD_CRC))
        {
    		tmpReg |= BIT_17;
        }

        if (0 != (err_msg&ERR_CMD_TIMEOUT))
        {
    		tmpReg |= BIT_16;
        }

    }

    if (0 != (msg&SIG_CARD_IN))
    {
		tmpReg |= BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
		tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
		tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
		tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
		tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
		tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
		tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
		tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
		tmpReg |= BIT_0;
    }
	sdhost_handler->host_cfg->INT_SIG_EN |= tmpReg;

}

PUBLIC void SDHOST_SetCmd (SDHOST_HANDLE sdhost_handler,uint32 cmdIndex,uint32 transmode,SDHOST_CMD_TYPE_E cmd_type,CMD_RSP_TYPE_E Response)
{
    volatile uint32 tmpReg;

    tmpReg = sdhost_handler->host_cfg->CMD_TRANSMODE;
    tmpReg &= (~ (0x7F|0x30000|0x3FF80000));

    if (0 != (TRANS_MODE_ATA_CMPLETE_SIG_EN&transmode))
    {
        tmpReg |= SDIO_TRANS_COMP_ATA;
    }

    if (0 != (TRANS_MODE_MULTI_BLOCK&transmode))
    {
        tmpReg |= SDIO_TRANS_MULTIBLK;
    }

    if (0 != (TRANS_MODE_READ&transmode))
    {
        tmpReg |= SDIO_TRANS_DIR_READ;
    }

    if (0 != (TRANS_MODE_CMD12_EN&transmode))
    {
        tmpReg |= SDIO_TRANS_AUTO_CMD12_EN;
    }

    if (0 != (TRANS_MODE_BLOCK_COUNT_EN&transmode))
    {
        tmpReg |= SDIO_TRANS_BLK_CNT_EN;
    }

    if (0 != (TRANS_MODE_DMA_EN&transmode))
    {
        tmpReg |= SDIO_TRANS_DMA_EN;
    }

    if (0 != (CMD_HAVE_DATA&transmode))
    {
        tmpReg |= SDIO_CMD_DATA_PRESENT;
    }

    switch (cmd_type)
    {
        case CMD_TYPE_NORMAL:
            {
                tmpReg |= SDIO_CMD_TYPE_NML;
            }
            break;

        case CMD_TYPE_SUSPEND:
            {
                tmpReg |= SDIO_CMD_TYPE_SUSPEND;
            }
            break;

        case CMD_TYPE_RESUME:
            {
                tmpReg |= SDIO_CMD_TYPE_RESUME;
            }
            break;

        case CMD_TYPE_ABORT:
            {
                tmpReg |= SDIO_CMD_TYPE_ABORT;
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

    switch (Response)
    {
        case CMD_NO_RSP:
            {
                tmpReg |= SDIO_NO_RSP;
            }
            break;

        case CMD_RSP_R1:
            {
                tmpReg |= SDIO_R1;
            }
            break;

        case CMD_RSP_R2:
            {
                tmpReg |= SDIO_R2;
            }
            break;

        case CMD_RSP_R3:
            {
                tmpReg |= SDIO_R3;
            }
            break;

        case CMD_RSP_R4:
            {
                tmpReg |= SDIO_R4;
            }
            break;

        case CMD_RSP_R5:
            {
                tmpReg |= SDIO_R5;
            }
            break;

        case CMD_RSP_R6:
            {
                tmpReg |= SDIO_R6;
            }
            break;

        case CMD_RSP_R7:
            {
                tmpReg |= SDIO_R7;
            }
            break;

        case CMD_RSP_R1B:
            {
                tmpReg |= SDIO_R1B;
            }
            break;

        case CMD_RSP_R5B:
            {
                tmpReg |= SDIO_R5B;
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;

    }

    tmpReg |= (cmdIndex<<24);

    sdhost_handler->host_cfg->CMD_TRANSMODE = tmpReg;

}

LOCAL void _Reset_DAT (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL1 |= BIT_26;

    while (0!= (sdhost_handler->host_cfg->HOST_CTL1 & BIT_26)) {}
}


LOCAL void _Reset_CMD (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL1 |= BIT_25;

    while (0!= (sdhost_handler->host_cfg->HOST_CTL1 & BIT_25)) {}
}

LOCAL void _Reset_ALL (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL1 |= BIT_24;

    while (0 != (sdhost_handler->host_cfg->HOST_CTL1 & BIT_24)) {}

}

LOCAL void _Reset_MODULE (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    CHIP_REG_OR(AHB_SOFT_RST, AHB_SDIO_SOFT_RST);
    CHIP_REG_AND(AHB_SOFT_RST, ~AHB_SDIO_SOFT_RST);

}

LOCAL void _Reset_DAT_CMD (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL1 |= (BIT_25|BIT_26);

    while (0!= (sdhost_handler->host_cfg->HOST_CTL1 & (BIT_25|BIT_26))) {}


}


PUBLIC void SDHOST_RST (SDHOST_HANDLE sdhost_handler,SDHOST_RST_TYPE_E rst_type)
{

    switch (rst_type)
    {
        case RST_CMD_LINE:
            {
                _Reset_CMD (sdhost_handler);
            }
            break;

        case RST_DAT_LINE:
            {
                _Reset_DAT (sdhost_handler);
            }
            break;

        case RST_CMD_DAT_LINE:
            {
                _Reset_DAT_CMD (sdhost_handler);
            }
            break;

        case RST_ALL:
            {
                _Reset_ALL (sdhost_handler);
            }
            break;
            
        case RST_MODULE:
            {
                _Reset_MODULE (sdhost_handler);
            }
            break;
        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }
}

PUBLIC void SDHOST_GetRspFromBuf (SDHOST_HANDLE sdhost_handler,CMD_RSP_TYPE_E Response,uint8 *rspBuf)
{
    uint32 tmpRspBuf[4];
    uint32 i;

    tmpRspBuf[0] = sdhost_handler->host_cfg->RSP0;
    tmpRspBuf[1] = sdhost_handler->host_cfg->RSP1;
    tmpRspBuf[2] = sdhost_handler->host_cfg->RSP2;
    tmpRspBuf[3] = sdhost_handler->host_cfg->RSP3;

    //  SDHOST_PRINT(("RSP %x %x %x %x",tmpRspBuf[0],tmpRspBuf[1],tmpRspBuf[2],tmpRspBuf[3]));

    for (i = 0; i < 4; i++)
    {
        rspBuf[0+ (i<<2) ] = (uint8) ( (tmpRspBuf[i]>>24) &0xFF);
        rspBuf[1+ (i<<2) ] = (uint8) ( (tmpRspBuf[i]>>16) &0xFF);
        rspBuf[2+ (i<<2) ] = (uint8) ( (tmpRspBuf[i]>>8) &0xFF);
        rspBuf[3+ (i<<2) ] = (uint8) (tmpRspBuf[i]&0xFF);
    }

    switch (Response)
    {
        case CMD_NO_RSP:
            break;

        case CMD_RSP_R1:
        case CMD_RSP_R1B:
        case CMD_RSP_R3:
        case CMD_RSP_R4:
        case CMD_RSP_R5:
        case CMD_RSP_R6:
        case CMD_RSP_R7:
        case CMD_RSP_R5B:
            {
                rspBuf[0] = (uint8) ( (tmpRspBuf[0]>>24) &0xFF);
                rspBuf[1] = (uint8) ( (tmpRspBuf[0]>>16) &0xFF);
                rspBuf[2] = (uint8) ( (tmpRspBuf[0]>>8) &0xFF);
                rspBuf[3] = (uint8) (tmpRspBuf[0]&0xFF);
            }
            break;

        case CMD_RSP_R2:
            {
                rspBuf[0] = (uint8) ( (tmpRspBuf[3]>>16) &0xFF);
                rspBuf[1] = (uint8) ( (tmpRspBuf[3]>>8) &0xFF);
                rspBuf[2] = (uint8) (tmpRspBuf[3]&0xFF);

                rspBuf[3] = (uint8) ( (tmpRspBuf[2]>>24) &0xFF);
                rspBuf[4] = (uint8) ( (tmpRspBuf[2]>>16) &0xFF);
                rspBuf[5] = (uint8) ( (tmpRspBuf[2]>>8) &0xFF);
                rspBuf[6] = (uint8) (tmpRspBuf[2]&0xFF);

                rspBuf[7] = (uint8) ( (tmpRspBuf[1]>>24) &0xFF);
                rspBuf[8] = (uint8) ( (tmpRspBuf[1]>>16) &0xFF);
                rspBuf[9] = (uint8) ( (tmpRspBuf[1]>>8) &0xFF);
                rspBuf[10] = (uint8) (tmpRspBuf[1]&0xFF);

                rspBuf[11] = (uint8) ( (tmpRspBuf[0]>>24) &0xFF);
                rspBuf[12] = (uint8) ( (tmpRspBuf[0]>>16) &0xFF);
                rspBuf[13] = (uint8) ( (tmpRspBuf[0]>>8) &0xFF);
                rspBuf[14] = (uint8) (tmpRspBuf[0]&0xFF);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

    return;
}

PUBLIC void SDHOST_internalClk_On (SDHOST_HANDLE sdhost_handler)
{
                //Enable internal clock
                sdhost_handler->host_cfg->HOST_CTL1 |= BIT_0;
                //Wait internal clock stable
                while (0 == (sdhost_handler->host_cfg->HOST_CTL1 & BIT_1)) {}
}

PUBLIC void SDHOST_SetDataParam (SDHOST_HANDLE sdhost_handler,uint32 block_size,uint32 block_cnt)
{
    volatile uint32 tmpReg;

    tmpReg = sdhost_handler->host_cfg->BLK_SIZE_COUNT;

    // Set Block Size
    tmpReg &= (~BIT_15);
    tmpReg &= (~ (0xFFF));
    if (0x1000 == block_size)
    {
        tmpReg |= BIT_15;
    }
    else
    {
        tmpReg |= block_size;
    }

    // Set Block Cnt
    tmpReg &= (~0xFFFF0000);
    tmpReg |= (block_cnt << 16);

    // Set DMA Buf Size
    tmpReg &= (~ (0x07<<12));
    // Set DMA DMAsize
    // tmpReg |= (0x06<<12);
   
    sdhost_handler->host_cfg->BLK_SIZE_COUNT = tmpReg;
}

#define SECTOR_MODE 0x40000000 
#define BYTE_MODE   0x00000000 

PUBLIC uint32 SDHOST_SD_Clk_Freq_Set (SDHOST_HANDLE sdhost_handler,uint32 sdio_clk)
{
    volatile uint32 tmpReg;
    uint32 clkDiv;

    sdhost_handler->host_cfg->HOST_CTL1 &= (~BIT_2);

    //SDCLK Frequency Select ,Configure SDCLK select
    clkDiv = sdhost_handler->baseClock/sdio_clk;

    if (0 != sdhost_handler->baseClock%sdio_clk)
    {
        clkDiv++;
    }

    tmpReg = sdhost_handler->host_cfg->HOST_CTL1;
    tmpReg &= (~ (0xff<<8));

    if (128 < clkDiv)
    {
        clkDiv = 256;
        tmpReg |= (0x80 << 8);
    }
    else if (64 < clkDiv)
    {
        clkDiv = 128;
        tmpReg |= (0x40<<8);
    }
    else if (32 < clkDiv)
    {
        clkDiv = 64;
        tmpReg |= (0x20<<8);
    }
    else if (16 < clkDiv)
    {
        clkDiv = 32;
        tmpReg |= (0x10<<8);
    }
    else if (8 < clkDiv)
    {
        clkDiv = 16;
        tmpReg |= (0x8<<8);
    }
    else if (4 < clkDiv)
    {
        clkDiv = 8;
        tmpReg |= (0x4<<8);
    }
    else if (2 < clkDiv)
    {
        clkDiv = 4;
        tmpReg |= (0x2<<8);
    }
    else if (1 < clkDiv)
    {
        clkDiv = 2;
        tmpReg |= (0x1<<8);
    }
    else if (0 < clkDiv)
    {
        clkDiv = 1;
        //nothing
    }
    else //if (0 == clkDiv)
    {
        SCI_ASSERT (0);/*assert to do*/
    }

    sdhost_handler->host_cfg->HOST_CTL1 = tmpReg;
    sdhost_handler->sdClock = sdhost_handler->baseClock/clkDiv;
    return sdhost_handler->sdClock;
}

PUBLIC BOOLEAN SDIO_Card_Pal_SetClk (SDIO_CARD_PAL_HANDLE handle)
{

	* (volatile uint32 *) AHB_SDIO_CTL = (AHB_SDIO_CTRL_SLOT0); //select master1
	SDHOST_SD_Clk_Freq_Set (handle->sdio_port,25000000);
	SDHOST_SD_clk_On (handle->sdio_port);

	return TRUE;
}

PUBLIC BOOLEAN CARD_SDIO_SetBlockLength(CARD_SDIO_HANDLE cardHandle,uint32 length)
{
	uint8 rspBuf[16];

	if(0 != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD16_SET_BLOCKLEN,length,NULL,rspBuf))
	{
		return FALSE;
	}
	cardHandle->BlockLen = length;
	return TRUE;
}

BOOLEAN CARD_SDIO_Select_CurPartition(CARD_SDIO_HANDLE cardHandle, CARD_EMMC_PARTITION_TPYE cardPartiton)
{
	uint8 rspBuf[16];
	uint16 RCA = cardHandle->RCA;
	uint32 argument = 0;
	
	argument = CMD6_ACCESS_MODE_WRITE_BYTE | ( EXT_CSD_PARTITION_CFG_INDEX<<CMD6_BIT_MODE_OFFSET_INDEX & CMD6_BIT_MODE_MASK_INDEX) | CMD6_CMD_SET;
	argument |= (PARTITION_CFG_BOOT_PARTITION_ENABLE_BOOT1 |PARTITION_CFG_BOOT_PARTITION_ACCESS_PARTITION_BOOT2)<<CMD6_BIT_MODE_OFFSET_VALUE;

	if(0 != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_ACMD6_SET_EXT_CSD,argument,NULL,rspBuf))
	{
		return FALSE;
	}
	return TRUE;
}

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

LOCAL BOOLEAN MMC_SWITCH(CARD_SDIO_HANDLE cardHandle, uint8 index, uint8 value)
{
	uint8 rspBuf[16];
	uint32 argument = 0;

	argument = CMD6_ACCESS_MODE_WRITE_BYTE | CMD6_CMD_SET;
	argument |= index<<CMD6_BIT_MODE_OFFSET_INDEX;
	argument |= value<<CMD6_BIT_MODE_OFFSET_VALUE;
	

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_ACMD6_SET_EXT_CSD,argument,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
}


PUBLIC BOOLEAN SDIO_Card_Pal_SetBusWidth (SDIO_CARD_PAL_HANDLE handle)
{
        uint32 tmpReg =  handle->sdio_port->host_cfg->HOST_CTL0;

    tmpReg &= (~ (0x1<<5));
    tmpReg &= (~ (0x1<<1));



            {
                tmpReg |= (0x1<<1);
            }


    handle->sdio_port->host_cfg->HOST_CTL0 = tmpReg;

    return TRUE;
}


LOCAL BOOLEAN _SetBusWidth(CARD_SDIO_HANDLE cardHandle,CARD_BUS_WIDTH_E width)
{
	uint8 rspBuf[16];
	uint16 RCA = cardHandle->RCA;
	uint32 argument = 0;

	argument = CMD6_ACCESS_MODE_WRITE_BYTE | ( EXT_CSD_BUS_WIDTH_INDEX<<CMD6_BIT_MODE_OFFSET_INDEX & CMD6_BIT_MODE_MASK_INDEX) ;



	//case CARD_WIDTH_4_BIT:
	{
		argument |=EXT_CSD_CARD_WIDTH_4_BIT<<CMD6_BIT_MODE_OFFSET_VALUE;
	}


	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_ACMD6_SET_EXT_CSD,argument,NULL,rspBuf))
	{
		return FALSE;
	}


	//case CARD_WIDTH_4_BIT:
	{
		SDIO_Card_Pal_SetBusWidth(cardHandle->sdioPalHd);
	}
	cardHandle->bus_width = width;

	return TRUE;
}


PUBLIC BOOLEAN CARD_SDIO_InitCard(CARD_SDIO_HANDLE cardHandle, CARD_SPEED_MODE speedmode)
{
	uint8 rspBuf[16];
	uint32 pre_tick, cur_tick;
	CARD_BUS_WIDTH_E busWidth = CARD_WIDTH_1_BIT;
	uint16 RCA;

	cardHandle->bus_width = CARD_WIDTH_1_BIT;
	cardHandle->BlockLen = 0;

	if(0 != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD0_GO_IDLE_STATE,0,NULL,rspBuf))
	{
		return FALSE;
	}
	pre_tick = SCI_GetTickCount(); /*set start tick value*/       
    	do
    	{
    		if(0 != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD1_SEND_OP_COND,0x00FF8000 | SECTOR_MODE,NULL,rspBuf))
    		{
    			continue;
    		}
    		if(0 != (rspBuf[0]&BIT_7))
    		{
    			break;
    		}

    		cur_tick = SCI_GetTickCount();
    		if( 5000 <  (cur_tick - pre_tick))
    		{
    			/*cmd time out, return false*/
    			return FALSE;
    		} 
            
    	}
    	while(1); /*lint !e506*/

	if(0 != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD2_ALL_SEND_CID,0,NULL,rspBuf))
	{
		return FALSE;
	}

	if(0 != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD3_SET_RELATIVE_ADDR,1<<16,NULL,rspBuf))
	{
		return FALSE;
	}
	cardHandle->RCA = 1;
	if(0 != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	{
		return FALSE;
	}
        
	if(0 != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD7_SELECT_DESELECT_CARD,  1<<16,NULL,rspBuf))
	{
		return FALSE;
	}
	if(0 != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	{
		return FALSE;
	}	 
	 //get Boot2 Capacity
	CARD_SDIO_Select_CurPartition(cardHandle, PARTITION_BOOT2); 	  
	  if(0 != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	{
		return FALSE;
	 }

       
	MMC_SWITCH(cardHandle, EXT_CSD_HS_TIMING_INDEX, 1);
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	{
		return FALSE;
	}
            
               //SDIO_Card_Pal_SetClk(cardHandle->sdioPalHd,SDIO_CARD_PAL_25MHz);
               
	SDIO_Card_Pal_SetClk(cardHandle->sdioPalHd);
	pre_tick = SCI_GetTickCount(); /*set start tick value*/  
 	cur_tick = pre_tick;
	while(cur_tick  -pre_tick < 100)
 	{
 	cur_tick = SCI_GetTickCount();
	}
        busWidth = CARD_WIDTH_4_BIT;
	if(FALSE == _SetBusWidth(cardHandle,CARD_WIDTH_4_BIT))
	{
		return FALSE;
	}
        
	if(FALSE == CARD_SDIO_SetBlockLength(cardHandle,512))
	{
		return FALSE;
	}
	return TRUE;
}


LDO_ERR_E LDO_SetVoltLevel(LDO_ID_E ldo_id, LDO_VOLT_LEVEL_E volt_level)
{
	unsigned int b0_mask,b1_mask;
	LDO_CTL_PTR  ctl = NULL;

	b0_mask = (volt_level & BIT_0)?~0:0;
	b1_mask = (volt_level & BIT_1)?~0:0;

#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
	if( LDO_LDO_SDIO3 == ldo_id )
		ctl = &ldo_ctl_data_sdio3;
	else
 		ctl = &ldo_ctl_data_vdd30;  
#else
if( LDO_LDO_SDIO1 == ldo_id )
	ctl = &ldo_ctl_data_sdio1;
else
 	ctl = &ldo_ctl_data_sim2;  
#endif
 
	if(ctl->level_reg_b0 == NULL)
	{
		return 0;
	}

	if(ctl->level_reg_b0 == ctl->level_reg_b1)
	{
		SET_LEVEL(ctl->level_reg_b0, b0_mask, b1_mask, ctl->b0, ctl->b0_rst, ctl->b1, ctl->b1_rst);
	}
	else
	{
		SET_LEVELBIT(ctl->level_reg_b0, b0_mask, ctl->b0, ctl->b0_rst);
		SET_LEVELBIT(ctl->level_reg_b1, b1_mask, ctl->b1, ctl->b1_rst);
	}

	return 0;
}

 LDO_ERR_E LDO_TurnOnLDO(LDO_ID_E ldo_id)
{
	LDO_CTL_PTR ctl = NULL;
                   uint32 reg_val;
#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
	if( LDO_LDO_SDIO3 == ldo_id )
		ctl = &ldo_ctl_data_sdio3;
	else
 		ctl = &ldo_ctl_data_vdd30;  
#else
if( LDO_LDO_SDIO1 == ldo_id )
	ctl = &ldo_ctl_data_sdio1;
else
 	ctl = &ldo_ctl_data_sim2;  
#endif

	if(ctl->ref == 0)
        #ifdef CONFIG_SC7710G2
		ANA_REG_SET(ctl->bp_rst_reg,ctl->bp_rst);   
        #else
		REG_SETCLRBIT(ctl->bp_reg, ctl->bp_rst, ctl->bp);
        #endif

	ctl->ref++;

	return 0;
}

 LDO_ERR_E LDO_TurnOffLDO(LDO_ID_E ldo_id)
{
	LDO_CTL_PTR ctl = NULL;
                   uint32 reg_val;
#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
	if( LDO_LDO_SDIO3 == ldo_id )
		ctl = &ldo_ctl_data_sdio3;
	else
 		ctl = &ldo_ctl_data_vdd30;  
#else
if( LDO_LDO_SDIO1 == ldo_id )
	ctl = &ldo_ctl_data_sdio1;
else
 	ctl = &ldo_ctl_data_sim2;  
#endif

   if(ctl->ref > 0)
            ctl->ref--;
	if(ctl->ref == 0)
        #ifdef CONFIG_SC7710G2
		ANA_REG_SET(ctl->bp_reg,ctl->bp);   
        #else
		REG_SETCLRBIT(ctl->bp_reg, ctl->bp, ctl->bp_rst);
        #endif
        
	return 0;
}

PUBLIC BOOLEAN SDIO_Card_Pal_Pwr (SDIO_CARD_PAL_HANDLE handle,SDIO_CARD_PAL_PWR_E onOrOff)
{
    int i;
    uint32 pre_tick, cur_tick;
    switch (onOrOff)
    {
        case SDIO_CARD_PAL_ON:
        {
            unsigned int tempreg;
             SDHOST_RST(handle->sdio_port, RST_MODULE);
#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
            LDO_SetVoltLevel (LDO_LDO_SDIO3, LDO_VOLT_LEVEL3);
            LDO_SetVoltLevel (LDO_LDO_VDD30, LDO_VOLT_LEVEL1); 
            LDO_TurnOnLDO(LDO_LDO_SDIO3);
            LDO_TurnOnLDO(LDO_LDO_VDD30);
#else
            LDO_SetVoltLevel (LDO_LDO_SDIO1, LDO_VOLT_LEVEL3);
            LDO_SetVoltLevel (LDO_LDO_SIM2, LDO_VOLT_LEVEL1); 
            tempreg = *(volatile unsigned int  *)0x8B000008; 
            *(volatile unsigned int  *)0x8B000008 = tempreg | (1<<5);
#ifdef CONFIG_SC8810_OPENPHONE
            tempreg = *(volatile unsigned int  *)0x8A000384;
            *(volatile unsigned int  *)0x8A000384 = tempreg | (1<<11);
            tempreg = *(volatile unsigned int  *)0x8A000388;
            *(volatile unsigned int  *)0x8A000388 = tempreg | (1<<11);
            tempreg = *(volatile unsigned int  *)0x8A000380;
            *(volatile unsigned int  *)0x8A000380 = tempreg | (1<<11);
#else
	   tempreg = *(volatile unsigned int  *)0x8A000184;
            *(volatile unsigned int  *)0x8A000184 = tempreg | (1<<7);
            tempreg = *(volatile unsigned int  *)0x8A000188;
            *(volatile unsigned int  *)0x8A000188 = tempreg | (1<<7);
            tempreg = *(volatile unsigned int  *)0x8A000180;
            *(volatile unsigned int  *)0x8A000180 = tempreg | (1<<7);
	/*		
	   for(i=0; i<10000;i++);
	   *(volatile unsigned int	*)0x8A000180 = tempreg & (~(1<<7));
	   for(i=0; i<10*10000;i++);
	   *(volatile unsigned int	*)0x8A000180 = tempreg | (1<<7);	   */
            LDO_TurnOnLDO(LDO_LDO_SDIO1);
            LDO_TurnOnLDO(LDO_LDO_SIM2);
#endif
#endif
             
            SDHOST_SD_Clk_Freq_Set (handle->sdio_port,400000);
            SDHOST_internalClk_On(handle->sdio_port);
            SDHOST_SD_clk_On(handle->sdio_port);
            pre_tick = SCI_GetTickCount(); /*set start tick value*/  
            cur_tick = pre_tick;
            while(cur_tick  -pre_tick < 100)
                {
                        cur_tick = SCI_GetTickCount();
                }
            //__udelay (100*1000);
        }
    break;

   case SDIO_CARD_PAL_OFF:
        {
#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
            LDO_TurnOffLDO (LDO_LDO_SDIO3);
            LDO_TurnOffLDO (LDO_LDO_VDD30);
#else
            LDO_TurnOffLDO (LDO_LDO_SDIO1);
            LDO_TurnOffLDO (LDO_LDO_SIM2);
#endif
            SDHOST_RST (handle->sdio_port,RST_ALL);
            //SDHOST_SD_clk_Off(handle->sdio_port,CLK_OFF);
            handle->sdio_port->host_cfg->HOST_CTL1 &= (~BIT_2);
            //SDHOST_internalClk_On(handle->sdio_port,CLK_OFF);
            handle->sdio_port->host_cfg->HOST_CTL1 &= (~BIT_0);
            pre_tick = SCI_GetTickCount(); /*set start tick value*/  
            cur_tick = pre_tick;
            while(cur_tick  -pre_tick < 100)
                {
                        cur_tick = SCI_GetTickCount();
                }
            //__udelay (250*1000);
            break;
        }
     default:
            break;

    }
    return TRUE;
}

PUBLIC void CARD_SDIO_PwrCtl(CARD_SDIO_HANDLE cardHandle,BOOLEAN switchCtl)
{
	if(TRUE == switchCtl)
	{
		SDIO_Card_Pal_Pwr(cardHandle->sdioPalHd,SDIO_CARD_PAL_ON);
	}
	else
	{
		SDIO_Card_Pal_Pwr(cardHandle->sdioPalHd,SDIO_CARD_PAL_OFF);
	}
}


LOCAL BOOLEAN _IsCardReady(CARD_SDIO_HANDLE cardHandle)
{
	uint32 pre_tick = 0, cur_tick = 0;
	uint8 rspBuf[16] ;
	uint32 argument = 0;

	pre_tick = SCI_GetTickCount(); /*set start tick value*/

	do
	{
                             if(0 != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
		{
			return FALSE;
		}
		
		if(0 != (rspBuf[2]&BIT_0))
		{
			return TRUE;
		}

		cur_tick = SCI_GetTickCount();
		if(5000 <  (cur_tick - pre_tick))
		{
			return FALSE;
		}
	}
	while(1); /*lint !e506*/

}


PUBLIC BOOLEAN Emmc_Init()
{
	uint32 ret = 0;
	
	//emmc_handle = CARD_SDIO_Open(CARD_SDIO_SLOT_1);
	emmc_handle = &cadport;
#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
	emmc_handle->sdioPalHd = SDIO_Card_Pal_Open(SDIO_CARD_PAL_SLOT_7);
#else	
	emmc_handle->sdioPalHd = SDIO_Card_Pal_Open(SDIO_CARD_PAL_SLOT_1);
#endif	
         //CARD_SDIO_PwrCtl(emmc_handle, FALSE);
	CARD_SDIO_PwrCtl(emmc_handle, TRUE);
	emmc_handle->BlockLen = 0;
	emmc_handle->RCA = 1;
	emmc_handle->bus_width = CARD_WIDTH_1_BIT;
	ret = CARD_SDIO_InitCard(emmc_handle, HIGH_SPEED_MODE);

	return ret;
}

PUBLIC BOOLEAN CARD_SDIO_ReadMultiBlock(CARD_EMMC_PARTITION_TPYE  cardPartiton, CARD_SDIO_HANDLE cardHandle,uint32 startBlock,uint32 num,uint8* buf)
{
	uint8 rspBuf[16];
	uint32 address = 0xFFFFFFFF;
	CARD_DATA_PARAM_T data;

	address = startBlock*cardHandle->BlockLen;

	data.blkLen = cardHandle->BlockLen;
	data.blkNum = num;
	data.databuf = buf;

	if(FALSE == _IsCardReady(cardHandle))
	{
		return FALSE;
	}

	if(0 != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD18_READ_MULTIPLE_BLOCK,address,&data,rspBuf))
	{
		SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD12_STOP_TRANSMISSION,NULL,NULL,rspBuf);
		return FALSE;
	}
	if(0 != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD12_STOP_TRANSMISSION,NULL,NULL,rspBuf))
	{
		return FALSE;
	}
	return TRUE;

}

PUBLIC BOOLEAN Emmc_Read(CARD_EMMC_PARTITION_TPYE  cardPartiton, uint32 startBlock,uint32 num,uint8* buf)
{
	uint32 ret = 0;
	uint8 rspBuf[16];

	 ret = CARD_SDIO_ReadMultiBlock(cardPartiton,emmc_handle, startBlock, num, buf);    
	return ret;

}




