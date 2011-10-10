/******************************************************************************
 ** File Name:    analog_reg_v3.h                                        *
 ** Author:       Tim.Luo                                                     *
 ** DATE:         03/03/2010                                                  *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 03/03/2010    Tim.Luo         Create.                                     *
 ** 05/07/2010    Mingwei.zhang   Modify it for SC8800G.                      *
 ******************************************************************************/

#ifndef _ANALOG_REG_V3_H_
#define _ANALOG_REG_V3_H_

#include "bits.h"
#include "sc8800g_reg_base.h"
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
///
//Analog die register define
///
//#define   ANA_REG_BASE           0x82000480
#define   ANA_AGEN              (ANA_REG_BASE + 0x00)
#define   ANA_MODULE_ARM_RST    (ANA_REG_BASE + 0x04)
#define   ANA_CLK_CTL           (ANA_REG_BASE + 0x08)
#define   ANA_LDO_PD_SET        (ANA_REG_BASE + 0x0C)
#define   ANA_LDO_PD_CTL        (ANA_REG_BASE + 0x10)
#define   ANA_LDO_VCTL0         (ANA_REG_BASE + 0x14)
#define   ANA_LDO_VCTL1         (ANA_REG_BASE + 0x18)
#define   ANA_LDO_VCTL2         (ANA_REG_BASE + 0x1C)
#define   ANA_LDO_VCTL3         (ANA_REG_BASE + 0x20)
#define   ANA_LDO_SLP           (ANA_REG_BASE + 0x24)
#define   ANA_ANA_CTL0          (ANA_REG_BASE + 0x28)
#define   ANA_DCDC_CTL          (ANA_REG_BASE + 0x2C)
#define   ANA_CHGR_CTL0         (ANA_REG_BASE + 0x30)
#define   ANA_CHGR_CTL1         (ANA_REG_BASE + 0x34)
#define   ANA_PLLMN             (ANA_REG_BASE + 0x38)
#define   ANA_PLLWAIT           (ANA_REG_BASE + 0x3C)
#define   ANA_LED_CTL           (ANA_REG_BASE + 0x40)
#define   ANA_PA_CTL            (ANA_REG_BASE + 0x44)
#define   ANA_HWRST_STATUS      (ANA_REG_BASE + 0x48)
#define   GR_HWRST                       ANA_HWRST_STATUS
#define   ANA_HWRST_RTC         (ANA_REG_BASE + 0x4C)
#define   ANA_STA               (ANA_REG_BASE + 0x50)
#define   ANA_INT_DEBUG         (ANA_REG_BASE + 0x54)
#define   ANA_LDO_PD_RST        (ANA_REG_BASE + 0x58)
#define   ANA_MCU_PROT          (ANA_REG_BASE + 0x5C)
#define   ANA_DCDC_CTRL_DS      (ANA_REG_BASE + 0x60)
#define   ANA_ADIE_CHIP_ID      (ANA_REG_BASE + 0x64)

/*
  the AGEN register bit
*/
//Reserved BIT_14-31
#define AGEN_PLL_FORCE_PD_EN    BIT_13
#define AGEN_RTC_ARCH_EN        BIT_12
#define AGEN_RTC_WDG_EN         BIT_11
#define AGEN_RTC_RTC_EN         BIT_10
#define AGEN_RTC_TPC_EN         BIT_9
#define AGEN_RTC_GPIO_EN        BIT_8
#define AGEN_APB_ARCH_EN        BIT_7
#define AGEN_TPC_EN             BIT_6
//Reserved BIT_5
#define AGEN_WDG_EN             BIT_4
#define AGEN_PINREG_EN          BIT_3
#define AGEN_ADC_EN             BIT_2
#define AGEN_RTC_EN             BIT_1
#define AGEN_GPIO_EN            BIT_0

///ANA_CLK_CTL BIT map
#define ACLK_CTL_AUXAD_EN       BIT_4
#define ACLK_CTL_AUXADC_EN  BIT_0
#define VBMCLK_ARM_EN           BIT_1
#define VBCTL_SEL               BIT_2

///ANA_LDO_PD_SET
#define ANA_LDO_PD_SET_MSK  0x3FF
///ANA_LDO_PD_CTL
#define ANA_LDO_PD_CTL_MSK  0x5555

///ANA_ANA_CTL0 BIT map
#define VIBR_CTL                    ANA_ANA_CTL0
#define VIBR_PD_SET             BIT_10
#define VIBR_PD_RST             BIT_11
#define VIBR_V_SHIFT            12
#define VIBR_V_MSK              (0x07 << VIBR_V_SHIFT)

///ANA_CHGR_CTL0
#define CHGR_USB_CHG_SHIFT              4
#define CHGR_USB_CHG_MSK                (3 << CHGR_USB_CHG_SHIFT)
#define CHGR_ADAPTER_CHG_SHIFT          6
#define CHGR_ADAPTER_CHG_MSK            (3 << CHGR_ADAPTER_CHG_SHIFT)
#define CHGR_PD_BIT                     BIT_8
#define CHGR_RECHG_BIT                  BIT_11
#define CHGR_ADATPER_EN_BIT             BIT_0
#define CHGR_ADATPER_EN_RST_BIT         BIT_1
#define CHGR_USB_500MA_EN_BIT           BIT_2
#define CHGR_USB_500MA_EN_RST_BIT       BIT_3
#define CHAR_ADAPTER_MODE_MSK           (BIT_0|BIT_1|BIT_2|BIT_3)

///ANA_CHGR_CTL1
#define CHAR_SW_POINT_SHIFT     0
#define CHAR_SW_POINT_MSK       (0x1F << CHAR_SW_POINT_SHIFT)

///ANA_LED_CTL BIT map
#define KPLED_CTL               ANA_LED_CTL
#define KPLED_PD_SET            BIT_7
#define KPLED_PD_RST            BIT_8
#define KPLED_V_SHIFT           9
#define KPLED_V_MSK             (0x07 << KPLED_V_SHIFT)

#define WHTLED_CTL              ANA_LED_CTL
#define WHTLED_PD_SET           BIT_0
#define WHTLED_PD_RST           BIT_1
#define WHTLED_V_SHIFT          2
#define WHTLED_V_MSK            (0x1F << WHTLED_V_SHIFT)

///ANA_PA_CTL
#define LDO_PA_SET              BIT_6
#define LDO_PA_RST              BIT_7

///ANA_ADIE_CHIP_ID
#define ANA_G1_CHIP_ID          ((uint16)0)
#define ANA_G2_CHIP_ID          ((uint16)1)

//ANA_HWRST_STATUS
#define HWRST_STATUS_POWERON_MASK (0xf0)
#define HWRST_STATUS_RECOVERY (0x20)
#define HWRST_STATUS_FASTBOOT (0X30)
#define HWRST_STATUS_NORMAL (0X40)
#define HWRST_STATUS_ALARM (0X50)
#define HWRST_STATUS_SLEEP (0X60)



/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/

#endif //_ANALOG_REG_V3_H_

