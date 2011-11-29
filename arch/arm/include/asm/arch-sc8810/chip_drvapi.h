/******************************************************************************
 ** File Name:      chip_drvapi.h                                                    *
 ** Author:         Richard Yang                                              *
 ** DATE:           08/14/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This driver is for spreadtrum chip set.                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/14/2002     Richard.Yang     Create.                                   *
 ** 09/16/2003     Xueliang.Wang    Modify CR4013                 *
 ******************************************************************************/
#ifndef _CHIP_DRVAPI_H_
#define _CHIP_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Defines                                           *
 **---------------------------------------------------------------------------*/
//PLL clock
#define PLL_CLK_192M        192000000

#define CLK_DIV_1M            1000000

// ARM valid clock.
#define ARM_CLK_13M         13000000
#define ARM_CLK_20M         20000000
#define ARM_CLK_24M         24000000
#define ARM_CLK_26M         26000000
#define ARM_CLK_27M         27000000
#define ARM_CLK_28M         28800000
#define ARM_CLK_30M         30720000
#define ARM_CLK_32M         32000000
#define ARM_CLK_36M         36000000
#define ARM_CLK_39M         39000000
#define ARM_CLK_41M         41000000
#define ARM_CLK_48M         48000000
#define ARM_CLK_50M         50000000
#define ARM_CLK_52M         52000000
#define ARM_CLK_54M         54000000
#define ARM_CLK_60M         60000000
#define ARM_CLK_61M         61440000
#define ARM_CLK_64M         64000000
#define ARM_CLK_66M         66000000
#define ARM_CLK_78M         78000000
#define ARM_CLK_71M         71500000
#define ARM_CLK_72M         72000000
#define ARM_CLK_80M         80000000
#define ARM_CLK_82M         82000000
#define ARM_CLK_85M         85000000
#define ARM_CLK_96M         96000000
#define ARM_CLK_100M       100000000
#define ARM_CLK_102M       102000000
#define ARM_CLK_104M       104000000
#define ARM_CLK_109M       109000000
#define ARM_CLK_120M       120000000
#define ARM_CLK_128M       128000000
#define ARM_CLK_153M       153600000
#define ARM_CLK_164M       164000000
#define ARM_CLK_192M       192000000
#define ARM_CLK_200M       200000000
#define ARM_CLK_256M       256000000
#define ARM_CLK_384M       384000000
#define ARM_CLK_400M       400000000
#define ARM_CLK_512M       512000000

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
typedef enum chip_serial_tag
{
    SERIAL_SC6600I,
    SERIAL_SC6600R,
    SERIAL_SC6600H,
    SERIAL_SC6800D,
    SERIAL_SC8800D,
    SERIAL_SC6600L,
    SERIAL_SC8800H,
    SERIAL_SC6800H,
    SERIAL_SC8800G,
    CHIP_SERIAL_MAX
}
CHIP_SERIAL_E;

typedef enum chip_type_tag
{
    SC6600I_ID_BASE = SERIAL_SC6600I << 16,
    SC6600I,
    SC6600I_ID_MAX,
    SC6600R_ID_BASE = SERIAL_SC6600R << 16,
    SC6600R1,
    SC6600R2,
    SC6600R2A,
    SC6600R3,
    SC6600R3A,
    SC6600R_ID_MAX,
    SC6600H_ID_BASE = SERIAL_SC6600H << 16,
    SC6600H1,
    SC6600H2,
    SC6600H3,
    SC6600H_ID_MAX,
    SC6800D_ID_BASE = SERIAL_SC6800D << 16,
    SC6800D,
    SC6800D_ID_MAX,
    SC8800D_ID_BASE = SERIAL_SC8800D << 16,
    SC8800D,
    SC8800D_ID_MAX,
    SC6600L_ID_BASE = SERIAL_SC6600L << 16,
    SC6600L,
    SC6600L2,
    SC6600L7,
    SC6600H3A,
    SC6600W2,
    SC6600L_ID_MAX,
    SC8800H_ID_BASE = SERIAL_SC8800H << 16,
    SC8800H,
    SC8800S3B,
    /* modified by zhengfei.xiao for SC8800H5 */
    SC8800H5,
    SC8800S4_AC,
    SC8800H5_AC,
    SC8800H_ID_MAX,
    SC6800H_ID_BASE = SERIAL_SC6800H << 16,
    SC6800H,
    SC6800H_ID_MAX,
    SC8800G_ID_BASE = SERIAL_SC8800G << 16,
    SC8800G1,
    SC8800G2,
    SC8801G2,
    SC8802G2,
    SC8800G_ID_MAX,
    CHIP_TYPE_MAX = 0x7FFFFFFF
} CHIP_TYPE_E;

//define dsp rom code id
typedef enum chip_dsp_rom_id_tag
{
    CHIP_DSP_ROM_V1,
    CHIP_DSP_ROM_V2,
    CHIP_DSP_ROM_V3,
    CHIP_DSP_ROM_ID_MAX
} CHIP_DSP_ROM_ID_E;

typedef enum module_clk
{
    CLK_MCU = 16,
    CLK_APB,
    CLK_DSP,
    CLK_CAL,
    CLK_12M,
    CLK_48M,
    CLK_CCIR,
    CLK_DCAM,
    CLK_IIS,
    CLK_VBC,
    CLK_CDC,
    CLK_AUX0,
    CLK_AUX1
} MODULE_CLK_E;

typedef enum module_clk_src
{
    CLKPLL = 0,
    CLKUPLL,
    CLK13M,
    CLK32K
} MODULE_CLK_SRC_E;

typedef struct _SYS_CLK_CFG_INFO
{
    uint32 pll_mn;
    uint32 upll_mn;
    uint32 dsp_arm_div;
} SYS_CLK_CFG_INFO;

typedef struct  _CHIP_INFO
{
    CHIP_TYPE_E type;
    char info[16];
} CHIP_INFO;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function is used to return chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC  uint32 CHIP_GetHWChipID (void);  // return chip ID number.

/*****************************************************************************/
//  Description:    The function is used to detect the memory type
//  Global resource dependence:
//  Author:         Nick.Zhao
//  Return value:
//                1      CS0 connects to SDRAM
//                0      CS0 connects to FLASH
/*****************************************************************************/
PUBLIC uint32 CHIP_DetectMemType (void);

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetArmClk (void);
/*****************************************************************************/
// Description :    This function is used to get current AHB clock.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
uint32 CHIP_GetAhbClk (void);
/*****************************************************************************/
// Description :    This function is used to get current APB clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetAPBClk (void);
/*****************************************************************************/
// Description :    This function is used to get current PLL clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetPllClk (void);

/*****************************************************************************/
// Description :    This function is used to get current PLL clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetMPllClk (void);

/*****************************************************************************/
// Description :    This function is used to get current VPLL clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           Daniel.ding add it for SC6800 .
/*****************************************************************************/
uint32 CHIP_GetVPllClk (void);


/*****************************************************************************/
// Description :    This function is used to disable all fiqs on sc6600d
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           every bit indicates one fiq src
/*****************************************************************************/
void CHIP_DisableFIQ (void);


/*****************************************************************************/
// Description :    This function is used to turn off the power.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
void CHIP_TurnOffPower (void);

/*****************************************************************************/
// Description :    This function is used to reset MCU.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
void CHIP_ResetMCU (void);

/*****************************************************************************/
// Description :    This function is used to reset MCU from boot code.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
void CHIP_BootReset (void);


/*****************************************************************************/
// Description :    This function is used to get current DSP clock.
// Global resource dependence :
// Author :         Benjamin.Wang
// Note :
/*****************************************************************************/
uint32 CHIP_GetDspClk (void);

/*****************************************************************************/
// Description :    This function is used to get current VPLL clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           Aiguo.Miao update it for SC8800H .
/*****************************************************************************/
uint32 CHIP_GetTDPllClk (void);

/*********************************************************************************************/
//Description:   This function is used to initializtion system clk and other
//Author:
//Note:          Can't use trace and assert in this funciton ;
//Parameter:
/*********************************************************************************************/
void CHIP_Init (void);

/*****************************************************************************/
// Description :    This function is used get the chip type
// Global resource dependence : none
// Author :         yuhua.shi
// Note :
/*****************************************************************************/
PUBLIC CHIP_TYPE_E CHIP_GetChipType (void);


/*****************************************************************************/
// Description :    This function is used get the chip type
// Global resource dependence : none
// Author :         yuhua.shi
// Note :
/*****************************************************************************/
PUBLIC const char *CHIP_GetChipInfo (void);


/*****************************************************************************/
// Description :    This function is active/deactive share-memory clock
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC void CHIP_ActiveSMCLK (BOOLEAN is_active);
/*****************************************************************************/
// Description :    This function is Get Usb DM Value
// Global resource dependence : none
// Author :
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN CHIP_GetUsbDMValue (void);

/*****************************************************************************/
//  Description:    The function is used to get the chip ID (a uint32 number).
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:           It must be called before any hardware related settings
/*****************************************************************************/
PUBLIC void CHIP_InitHWChipID (void);

#ifdef PLATFORM_SC8800H
/*****************************************************************************/
//  Description:    The function is used to init TD Calibration clock
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_TDCalClk_Init (void);

/*****************************************************************************/
//  Description:    The function is used to init TD clock
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_CEVAClk_Init (uint8 dsp_clk_sel);

/*****************************************************************************/
//  Description:    The function is called by Layer1 when response to DSP sleep
//  Author:         Jeff.li
//  Note:           Close RFLDO/TD CAL/UART
/*****************************************************************************/
PUBLIC void CHIP_DSPSleep (void);

/*****************************************************************************/
//  Description:    The function is called by Layer1 when response to DSP wake up
//  Author:         Jeff.li
//  Note:           Open RFLDO/TD CAL/UART
/*****************************************************************************/
PUBLIC void CHIP_DSPWakeUp (void);

/*****************************************************************************/
//  Description:    The function is used to get to know wheather use UART1 for LOG or not
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHIP_GetUART1LogMode (void);

/*****************************************************************************/
//  Description:    The function is used to set wheather use UART1 for LOG or not
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
PUBLIC void CHIP_SetUART1LogMode (uint32 UART1LogEn);

/*****************************************************************************/
//  Description:    The function is called by Layer1 when response to DSP wake up
//  Author:         Jeff.li
//  Note:           Disable UART1
/*****************************************************************************/
PUBLIC void CHIP_DSPUART1Dis (void);

/*****************************************************************************/
//  Description:    The function is called by Layer1 when response to DSP wake up
//  Author:         Jeff.li
//  Note:           Open UART1
/*****************************************************************************/
PUBLIC void CHIP_DSPUART1En (void);

#endif

/*****************************************************************************/
//  Description:   This function is used to do something before assert.
//  Global resource dependence:
//  Author:         Zhengjiang.Lei
//  Note:
/*****************************************************************************/

PUBLIC void CHIP_BeforeAssert (void);

/*below are removed from driver_export.c, wait clean up*/
// The function is used to reset dsp when arm system enter assert mode
PUBLIC BOOLEAN CHIP_ResetDspInAssertMode (void);

// The function is used to reset and set dsp to output dsp assert log to pc by uart1
PUBLIC void CHIP_DspOutPutAssertInfo (void);

// The function is used to get the IRAM/SHARE-MEMORY buf to store assert information
// in the assert mode.
// The buffer size must be larger than size(IRAM_buf) structure
PUBLIC void *CHIP_GetAssertInfoBufBase (void);

/******************************************************************************/
// Description:   Enable MCU can access switched-ram and shared-ram.
// Dependence:    NONE
// Author:        Haifeng.Yang
// Note:          Beforce invoking this function, the DSP/ISP should be hold on!
/******************************************************************************/
PUBLIC uint32 Chip_SwitchIRAM2Arm (void);

/******************************************************************************/
// Description:   handle function when dsp exception
// Dependence:    NONE
// Author:        
// Note:          
/******************************************************************************/
PUBLIC void L1API_DSPExceptionHandle(void);

extern SYS_CLK_CFG_INFO *Get_system_clk_cfg (void);
extern  void EMC_Close (void);
extern void __tx_reset (void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _CHIP_DRVAPI_H_
