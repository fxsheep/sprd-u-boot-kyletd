/******************************************************************************
 ** File Name:      dsp_sleep.h                                               *
 ** Author:         Richard Yang                                              *
 ** DATE:           08/25/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DSP and MCU for deep sleep.                               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/25/2003     Richard.Yang     Create.                                   *
 ******************************************************************************/
#ifndef _DEEP_SLEEP_V3_MODULE_H
#define _DEEP_SLEEP_V3_MODULE_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "arm_reg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#ifndef WIN32
#include "tx_api_thumb.h"
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define NEW_SLEEP_ROUTINE
#define SLEEP_DRV_NONE          0
#define SLEEP_DRV_L1            BIT_0
#define SLEEP_DRV_MMI           BIT_1
#define SLEEP_DRV_SIM           BIT_2
#define SLEEP_DRV_AUDIO         BIT_3
#define SLEEP_DRV_LCD           BIT_4
#define SLEEP_DRV_CHARGE        BIT_5
#define SLEEP_DRV_ENGINEER      BIT_6
#define SLEEP_DRV_BT            BIT_7
#define SLEEP_DRV_PCM           BIT_8
#define SLEEP_DRV_BTCLK         BIT_9
#define SLEEP_DRV_UPM           BIT_10
#define SLEEP_DRV_CALIBRATION   BIT_11
#define SLEEP_DRV_ATC           BIT_12
#define SLEEP_DRV_TV            BIT_13

#define MILLISECOND         180

#define ENABLE_DEEP_SLEEP       1
#define DISABLE_DEEP_SLEEP      0

#define ENABLE_AHB_SLEEP        1
#define DISABLE_AHB_SLEEP       0

//AHB_CTL0 bit define bit5,bit6 reserved
#define AHB_CTL0_DCAM_EB    BIT_1
#define AHB_CTL0_USBD_EB    BIT_2
#define AHB_CTL0_DMA_EB     BIT_3
#define AHB_DMA_SLEEP_MODE  BIT_4
#define AHB_CTL0_TIC_EB     BIT_7
#define AHB_CTL0_MON_EB     BIT_8
#define AHB_CTL0_EMC_EB     BIT_9
//APB REG related bit definition
#define MISC0_APB_FORCE_SLP  BIT_8
#define MISC0_APB_FORCE_ON   BIT_20
//Mcu_misc_en reg related bit definition
#define DSP_CLK_OUT_EN      BIT_15
#define MCU_MISC_HPASS      BIT_31

typedef enum
{
    SLP_LDOABB_AUTO_DIS,
    SLP_LDOVBO_AUTO_DIS,
    SLP_LDOAUX1_AUTO_DIS,
    SLP_LDOAUX2_AUTO_DIS,
    SLP_LDORF_AUTO_DIS,
    SLP_LDOUSB_AUTO_DIS,
    SLP_LDOCORE_AUTO_CHANGE,
    SLP_LDOALL_AUTO_DIS
} SLEEP_LDO_AUTO_CTRL_E;

typedef enum
{
    MCU_XTLEN_EN,
    MCU_PLL_EN,
    MCU_FORCE_STOP_EN,
    MCU_SYS_SLEEP_EN,
    MCU_DEEP_SLEEP_EN,
    DMA_AUTO_SLEEP_EN,
    APB_STOP_EN,
    APB_FORCE_ON_EN,
    APB_FORCE_SLEEP_EN
} SLEEP_CLK_AUTO_CTRL_E;

typedef enum
{
    MCU_SYS_SLEEP,
    MCU_CORE_SLEEP,
    MCU_DEEP_SLEEP,
    MCU_APB_SLEEP,
    MCU_DMA_SLEEP,
    MCU_NONE_SLEEP
} SLEEP_TYPE_E;


/*sleep_mode's bit map
-------------------------------------------------------------------------
|   31   |   30   |   29   |   28   |   27   |   26   |   25   |   24   |
-------------------------------------------------------------------------
|        |        |        |        |        |        |        |        |
-------------------------------------------------------------------------
-------------------------------------------------------------------------
|   15   |   14   |   13   |   12   |   11   |   10   |   9    |   8    |
-------------------------------------------------------------------------
|        |        |        |   ATC  | Calibra|   UPM  | BT_CLK |  PCM   |
-------------------------------------------------------------------------
|   7    |   6    |   5    |   4    |   3    |   2    |   1    |   0    |
-------------------------------------------------------------------------
|   BT   |ENGINEER|  CHR   |  LCD   |  AUD   |  SIM   |  MMI   |  L1    |
-------------------------------------------------------------------------*/
/* Note: Please remember to modify the sleep_mode initial value
 * in deep_sleep.c when you modify the struct definition!               */
typedef union _sleep_sturct
{
    struct _sleep
    {
        uint   reserved     :18;
        uint   tv           :1;
        uint   atc          :1;
        uint   calibration  :1;
        uint   upm          :1;
        uint   bt_clk       :1;
        uint   pcm          :1;
        uint   bt           :1;
        uint   engineer     :1;
        uint   charge       :1;
        uint   lcd          :1;
        uint   audio        :1;
        uint   sim          :1;
        uint   mmi          :1;
        uint   L1           :1;
    } mBits;
    uint   dwValue ;
} DRV_SLEEP_U;

#define SLEEP_DRV_NONE          0
#define SLEEP_DRV_L1            BIT_0
#define SLEEP_DRV_MMI           BIT_1
#define SLEEP_DRV_SIM           BIT_2
#define SLEEP_DRV_AUDIO         BIT_3
#define SLEEP_DRV_LCD           BIT_4
#define SLEEP_DRV_CHARGE        BIT_5
#define SLEEP_DRV_ENGINEER      BIT_6
#define SLEEP_DRV_BT            BIT_7
#define SLEEP_DRV_PCM           BIT_8
#define SLEEP_DRV_BTCLK         BIT_9
#define SLEEP_DRV_UPM           BIT_10
#define SLEEP_DRV_CALIBRATION   BIT_11
#define SLEEP_DRV_ATC           BIT_12
#define SLEEP_DRV_AP            BIT_13

/*ahb_sleep_mode's bit map
-------------------------------------------------------------------------
|   31   |   30   |   29   |   28   |   27   |   26   |   25   |   24   |
-------------------------------------------------------------------------
|       |        |        |        |        |        |        |        |
-------------------------------------------------------------------------
-------------------------------------------------------------------------
|   15   |   14   |   13   |   12   |   11   |   10   |   9    |   8    |
-------------------------------------------------------------------------
|        |        |        |        |        | Arch_eb| emc_eb |Busmon_eb|
-------------------------------------------------------------------------
|   7    |   6    |   5    |   4    |   3    |   2    |   1    |   0    |
-------------------------------------------------------------------------
|Tic_eb  |RESERVED|RESERVED|dma_s_mod|  dma_eb| Usbd_en| Dcam_eb|        |
-------------------------------------------------------------------------*/
/* Note: Please remember to modify the g_ahb_sleep_mode initial value
   in deep_sleep.c when you modify the struct definition!*/
typedef union _ahb_sleep_sturct
{
    struct _ahb_sleep
    {
        uint   reserved     :27;
        uint   sdio         :1;
        uint   dma          :1;
        uint   dcam         :1;
        uint   isp          :1;
        uint   lcdc         :1;
    } mBits;
    uint   dwValue ;
} DRV_AHB_SLEEP_U;

/*apb_sleep_mode's bit map*/

/*GEN0*/
/*-------------------------------------------------------------------------
|   31   |   30   |   29   |   28   |   27   |   26   |   25   |   24   |
-------------------------------------------------------------------------
|       |        |        |        |        |        |        |        |
-------------------------------------------------------------------------
-------------------------------------------------------------------------
|   15   |   14   |   13   |   12   |   11   |   10   |   9    |   8    |
-------------------------------------------------------------------------
|        |        |        |        |        |        |        |        |
-----------------------------------------------------------------------------------
|   7    |   6    |   5    |   4    |   3      |   2       |   1      |   0       |
-----------------------------------------------------------------------------------
|        |  pwm   |  tpc    |  i2c_eb| sim_eb   |           |   adc_eb|
----------------------------------------------------------------------------------*/
/*GEN1*/
/*-------------------------------------------------------------------------
|   31   |   30   |   29   |   28   |   27   |   26   |   25   |   24  | 23   | 22 |
-------------------------------------------------------------------------
|       |        |        |         |  pin   |   spi1  |   iis  |   spi0| uart1 | uart0 |
-------------------------------------------------------------------------
-------------------------------------------------------------------------
|   15   |   14   |   13   |   12   |   11   |   10   |   9    |   8    |
-------------------------------------------------------------------------
|        |        |        |        |        |        |        | gea_eb |
-----------------------------------------------------------------------------------
|   7    |   6    |   5    |   4    |   3      |   2       |   1      |   0       |
-----------------------------------------------------------------------------------
|        |        |        |        |          |           |          |           |
----------------------------------------------------------------------------------*/
/* Note: Please remember to modify the g_apb_sleep_mode initial value
   in deep_sleep.c when you modify the struct definition!               */
typedef union _apb_sleep_sturct
{
    struct _apb_sleep
    {
        uint   reserved    :16;
        uint   pwma         :1;
        uint   pwmb         :1;	
        uint   pwmc         :1;
        uint   pwmd         :1;
        uint   tpc         :1;
        uint   i2c         :1;
        uint   sim         :1;
        uint   adc         :1;
        uint   pin         :1;
        uint   spi1        :1;
        uint   iis         :1;
        uint   spi0        :1;
        uint   uart2       :1;
        uint   uart1       :1;
        uint   uart0       :1;
        uint   gea         :1;
    } mBits;
    uint   dwValue ;
} DRV_APB_SLEEP_U;

#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif //
#endif // End of _DEEP_SLEEP_V0_MODULE_H

