/******************************************************************************
 ** File Name:      sc8810_sleep_cfg.h                                             *
 ** Author:         jiexia.yu                                                 *
 ** DATE:           07/09/2007                                                *
 ** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic function for ldo management.  *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/09/2007     jiexia.yu        Create.                                   *
 ******************************************************************************/
#ifndef _SC8810_SLEEP_CFG_H_
#define _SC8810_SLEEP_CFG_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip.h"

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
 #define SLEEP_MODULE_CONFIG_SLEEP_PIN_FEATURE 0

#define SLEEP_SUPPORT_BTCLK 0
#define SLEEP_SUPPORT_IVSP 1
#define SLEEP_SUPPORT_ISP 1
#define SLEEP_SUPPORT_DCAM 1
#define SLEEP_SUPPORT_LCDC 1
#define SLEEP_SUPPORT_SDIO 1
#define SLEEP_SUPPORT_BKLIGHT 1
#define SLEEP_SUPPORT_MMI 1
#define SLEEP_SUPPORT_USBD 0
#define SLEEP_SUPPORT_RETENTION 1

typedef void (*SLP_ENTER_FUNC) (uint8 slp_type);

typedef enum
{
    SLP_CFG_NULL  = 0,  //id for NULL
    SLP_CFG_COM,
    SLP_CFG_DIF,
    SLP_CFG_MAX
} SLP_CFG_TYPE_E;

typedef enum
{
    SLP_AHB_NULL = 0,   //id for NULL
    SLP_AHB_DCAM,       //id for AHB device dcam
    SLP_AHB_USBD,       //id for AHB device usbd
    SLP_AHB_EMC,        //id for AHB device emc
    SLP_AHB_DMA,        //id for AHB device dma
    SLP_AHB_BUS,        //id for AHB device bus
    SLP_AHB_DEV_MAX
} SLP_AHB_DEV_E;

typedef enum
{
    SLP_APB_NULL = 0,   //id for NULL
    SLP_APB_WDG,        //id for APB device watchdog
    SLP_APB_ADC,        //id for APB device adc
    SLP_APB_TMR,        //id for APB device timer
    SLP_APB_SIM,        //id for APB device sim
    SLP_APB_I2C,        //id for APB device i2c
    SLP_APB_TPC,        //id for APB device tpc
    SLP_APB_PWM,        //id for APB device pwm
    SLP_APB_KPD,        //id for APB device keypad
    SLP_APB_GPIO,       //id for APB device gpio
    SLP_APB_GEA,        //id for APB device gea
    SLP_APB_SYSTMR,     //id for APB device sys timer
    SLP_APB_UART0,      //id for APB device uart0
    SLP_APB_UART1,      //id for APB device uart1
    SLP_APB_SPI0,       //id for APB device spi0
    SLP_APB_IIS,        //id for APB device iis
    SLP_APB_SPI1,       //id for APB device spi1
    SLP_APB_PIN,        //id for APB device pin
    SLP_APB_DEV_MAX
} SLP_APB_DEV_E;


typedef enum
{
    SLP_CTL_NULL = 0,               //id for NULL
    SLP_CTRL_MCU_FORCE_STOP,    //id for arm and ahb ctrl
    SLP_CTRL_MCU_DMA_WAKEUP_EN,  //id for dma wake up ctrl
    SLP_CTRL_MCU_SYS_SLEEP_EN,  //id for sys  sleep ctrl
    SLP_CTRL_MCU_DEEP_SLEEP_EN, //id for deep sleep ctrl
    SLP_CTRL_APB_STOP,          //id for APB  sleep ctrl
    SLP_CTRL_APB_FORCE_ON,      //id for APB  sleep ctrl
    SLP_CTRL_APB_FORCE_SLEEP,   //id for APB  sleep ctrl
    SLP_CTRL_XTLEN,             //id for xtlen sleep ctrl
    SLP_CTRL_DMA_SLEEP_MOD,     //id for dma sleep mode ctrl
    SLP_CTRL_MCU_PLL_EN,    //id for mcu pll enable
    SLP_CTRL_XTL_ON_SLP,
    SLP_CTRL_MAX
} SLP_BIT_CTL_E;

typedef enum
{
    AHB_CAN_SLP_APB_CAN_SLP,
    AHB_CAN_SLP_APB_NO_SLP,
    AHB_NO_SLP_APB_CAN_SLP,
    AHB_NO_SLP_APB_NO_SLP,
    SLP_TYPE_MAX
} SLP_AHB_APB_TYPE_E;

typedef enum
{
    AHB_COULD_SLEEP,
    AHB_NO_SLEEP
} SLP_AHB_TYPE_E;

typedef enum
{
    APB_COULD_SLEEP,
    APB_NO_SLEEP
} SLP_APB_TYPE_E;

typedef enum
{
    SLP_BIT_CLR = 0,
    SLP_BIT_SET
} SLP_BIT_DEF_E;

typedef struct
{
    SLP_AHB_DEV_E  id;
    uint32         ahb_dev_reg;
    uint32         mask;
    SLP_BIT_DEF_E  value;
    BOOLEAN        valid;
    uint32         reserved;
} SLP_AHB_CTL_T, * SLP_AHB_CTL_PTR;

typedef struct
{
    SLP_APB_DEV_E  id;
    uint32         apb_dev_reg;
    uint32         mask;
    SLP_BIT_DEF_E  value;
    BOOLEAN        valid;
    uint32         reserved;
} SLP_APB_CTL_T, * SLP_APB_CTL_PTR;

typedef struct
{
    SLP_BIT_CTL_E  id;
    uint32         slp_bit_reg;
    uint32         mask;
    SLP_BIT_DEF_E  value;
    BOOLEAN        valid;
    uint32         reserved;
} SLP_BIT_CTL_T, * SLP_BIT_CTL_PTR;

typedef struct
{
    SLP_AHB_CTL_T  ahb_ctrl[SLP_AHB_DEV_MAX];
    SLP_APB_CTL_T  apb_ctrl[SLP_APB_DEV_MAX];
    SLP_BIT_CTL_T  slp_bit_ctrl[SLP_CTRL_MAX];
    SLP_ENTER_FUNC slp_handler;
} SLP_COM_CTL_T, *SLPCOMCTL_PTR;

typedef struct
{
    SLP_COM_CTL_T  slp_com_cfg;
} SLP_CTL_T, * SLPCTL_PTR;

typedef struct
{
    CHIP_TYPE_E    chip_type;
    SLPCTL_PTR     sleep_ctl;
} SLP_CFG_T, * SLP_CFG_PTR;

/*****************************************************************************/
//  Function name:  Slp_Get_Cfg
//  Description  :  this function get sleep table entry according to chip type
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC SLPCTL_PTR Slp_Get_Cfg (void);
/*****************************************************************************/
//  Function name:  Slp_Get_Apb_Status
//  Description  :  this function get chip sleep status according ahb and apb
//  bus status
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/

PUBLIC int     tx_enter_deep_sleep (uint32 level);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _SC8810_SLEEP_CFG_H_