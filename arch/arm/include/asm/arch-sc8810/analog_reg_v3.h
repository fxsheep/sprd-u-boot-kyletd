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
#include "sc8810_reg_base.h"
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
//#define   ANA_REG_BASE           0x82000600
#define   ANA_APB_CLK_EN        (ANA_REG_BASE + 0x00)
#define   ANA_APB_ARM_RST    	(ANA_REG_BASE + 0x04)
#define   ANA_LDO_PD_SET        (ANA_REG_BASE + 0x08)
#define   ANA_LDO_PD_RST        (ANA_REG_BASE + 0x0C)
#define   ANA_LDO_PD_CTL0       (ANA_REG_BASE + 0x10)
#define   ANA_LDO_PD_CTL1       (ANA_REG_BASE + 0x14)
#define   ANA_LDO_VCTL0         (ANA_REG_BASE + 0x18)
#define   ANA_LDO_VCTL1         (ANA_REG_BASE + 0x1C)
#define   ANA_LDO_VCTL2         (ANA_REG_BASE + 0x20)
#define   ANA_LDO_VCTL3         (ANA_REG_BASE + 0x24)
#define   ANA_LDO_VCTL4         (ANA_REG_BASE + 0x28)
#define   ANA_LDO_SLP_CTL0      (ANA_REG_BASE + 0x2C)
#define   ANA_LDO_SLP_CTL1      (ANA_REG_BASE + 0x30)
#define   ANA_LDO_SLP_CTL2      (ANA_REG_BASE + 0x34)
#define   ANA_DCDC_CTL          (ANA_REG_BASE + 0x38)
#define   ANA_DCDC_CTL_DS       (ANA_REG_BASE + 0x3C)
#define   ANA_DCDC_CTL_CAL      (ANA_REG_BASE + 0x40)
#define   ANA_DCDCARM_CTL       (ANA_REG_BASE + 0x44)
#define   ANA_DCDCARM_CTL_CAL   (ANA_REG_BASE + 0x48)
#define   ANA_PLL_CTL           (ANA_REG_BASE + 0x4C)
#define   ANA_APLLMN            (ANA_REG_BASE + 0x50)
#define   ANA_APLLWAIT          (ANA_REG_BASE + 0x54)
#define   ANA_RTC_CTL           (ANA_REG_BASE + 0x58)
#define   ANA_TRF_CTL           (ANA_REG_BASE + 0x5C)
#define   ANA_CHGR_CTL0         (ANA_REG_BASE + 0x60)
#define   ANA_CHGR_CTL1         (ANA_REG_BASE + 0x64)
#define   ANA_LED_CTL           (ANA_REG_BASE + 0x68)
#define   ANA_VIBRATOR_CTL0     (ANA_REG_BASE + 0x6C)
#define   ANA_VIBRATOR_CTL1     (ANA_REG_BASE + 0x70)
#define   ANA_AUDIO_CTL         (ANA_REG_BASE + 0x74)
#define   ANA_AUDIO_PA_CTL0     (ANA_REG_BASE + 0x78)
#define   ANA_AUDIO_PA_CTL1     (ANA_REG_BASE + 0x7C)
#define   ANA_MIXED_CTL         (ANA_REG_BASE + 0x80)
#define   ANA_STATUS            (ANA_REG_BASE + 0x84)
#define   ANA_HWRST_STATUS      (ANA_REG_BASE + 0x88)
#define   ANA_MCU_WR_PROT       (ANA_REG_BASE + 0x8C)
#define   ANA_VIBR_WR_PROT      (ANA_REG_BASE + 0x90)
#define   ANA_INT_GPI_DEBUG     (ANA_REG_BASE + 0x94)
#define   ANA_HWRST_RTC         (ANA_REG_BASE + 0x98)
#define   ANA_IF_SPR_CTRL       (ANA_REG_BASE + 0x9C)
#define   ANA_CHIP_ID_LOW       (ANA_REG_BASE + 0xFB)
#define   ANA_CHIP_ID_HIGH      (ANA_REG_BASE + 0xFC)

/*
  the APB_CLK_EN register bit
*/
//Reserved BIT_16-31
#define CHGRWDG_EB			BIT_15
#define CLK_AUXAD_EN		BIT_14
#define CLK_AUXADC_EN		BIT_13
#define RTC_TPC_EB			BIT_12
#define RTC_EIC_EB			BIT_11
#define RTC_WDG_EB			BIT_10
#define RTC_RTC_EB			BIT_9
#define RTC_ARCH_EB			BIT_8
#define PINREG_EB			BIT_7
#define AGEN_RTC_EN              	BIT_1
#define AGEN_RTC_RTC_EN       	BIT_9

#define GPIO_EB				BIT_6
#define ADC_EB				BIT_5
#define TPC_EB				BIT_4
#define	EIC_EB				BIT_3
#define	WDG_EB				BIT_2
#define	RTC_EB				BIT_1
#define	APB_ARCH_EB			BIT_0

/*
  the APB_ARM_RST register bit
*/
#define	GPIO_SOFT_RST	BIT_7
#define	EIC_SOFT_RST	BIT_6
#define TPC_SOFT_RST	BIT_5
#define ADC_SOFT_RST	BIT_4
#define WDG_SOFT_RST	BIT_3
#define CHGRWDG_SOFT_RST	BIT_2
#define VBMC_SOFT_RST	BIT_1
#define RTC_SOFT_RST	BIT_0
/*
  the LDO_PD_SET register bit
*/
#define DCDCAM_PD	BIT_9
#define LDO_BPVDD25	BIT_8
#define LDO_BPVDD18 BIT_7
#define LDO_BPVDD28	BIT_6
#define LDO_BPAVDDBB	BIT_5
#define LDO_BPRF1	BIT_4
#define	LDO_BPRF0	BIT_3
#define LDO_BPMEM	BIT_2
#define	DCDC_PD		BIT_1
#define	PDBG		BIT_0
///ANA_LDO_PD_SET
#define ANA_LDO_PD_SET_MSK  0x3FF
/*
  the LDO_PD_RST register bit
*/
#define DCDCAM_PD_RST	BIT_9
#define LDO_BPVDD25_RST	BIT_8
#define LDO_BPVDD18_RST BIT_7
#define LDO_BPVDD28_RST	BIT_6
#define LDO_BPAVDDBB_RST	BIT_5
#define LDO_BPRF1_RST	BIT_4
#define	LDO_BPRF0_RST	BIT_3
#define LDO_BPMEM_RST	BIT_2
#define	DCDC_PD_RST		BIT_1
#define	PDBG_RST		BIT_0
/*
  the LDO_PD_CTL0 register bit
*/
#define LDO_BPVB_RST	BIT_15
#define LDO_BPVB		BIT_14
#define LDO_BPCAMA_RST	BIT_13
#define LDO_BPCAMA		BIT_12
#define LDO_BPCMAD1_RST	BIT_11
#define LDO_BPCAMD1		BIT_10
#define LDO_BPCMAD0_RST	BIT_9
#define LDO_BPCAMD0		BIT_8
#define LDO_BPSIM1_RST	BIT_7
#define LDO_BPSIM1  	BIT_6
#define LDO_BPSIM0_RST	BIT_5
#define LDO_BPSIM0		BIT_4
#define LDO_BPSDIO0_RST	BIT_3
#define LDO_BPSDIO0		BIT_2
#define LDO_BPUSBH_RST	BIT_1
#define LDO_BPUSBH		BIT_0
#define ANA_LDO_PD_CTL0_MSK  0x5555

/*
  the LDO_PD_CTL1 register bit
*/
#define LDO_BPSIM3_RST	BIT_9
#define LDO_BPSIM3		BIT_8
#define LDO_BPSIM2_RST	BIT_7
#define LDO_BPSIM2		BIT_6
#define LDO_BPWIFI_RST	BIT_5
#define LDO_BPWIF1		BIT_4
#define LDO_BPWIF0_RST	BIT_3
#define LDO_BPWIF0		BIT_2
#define LDO_BPSDIO1_RST	BIT_1
#define LDO_BPSDIO1		BIT_0
#define ANA_LDO_PD_CTL1_MSK  0x155
/*
  the LDO_SLP_CTL0 register bit
*/
#define FSM_LDOSDIO1_BP_EN	BIT_15
#define FSM_LDOVDD25_BP_EN	BIT_13
#define FSM_LDOVDD18_BP_EN	BIT_12
#define FSM_LDOVDD28_BP_EN	BIT_11

#define FSM_LDOAVDDBB_BP_EN	BIT_10
#define FSM_LDOSDIO0_BP_EN	BIT_9
#define FSM_LDOVB_BP_EN		BIT_8
#define FSM_CAMA_BP_EN		BIT_7
#define FSM_CAMD1_BP_EN		BIT_6
#define FSM_CAMD0_BP_EN		BIT_5
#define FSM_USBH_BP_EN		BIT_4
#define FSM_SIM1_BP_EN		BIT_3
#define FSM_SIM0_BP_EN		BIT_2
#define FSM_RF1_BP_EN		BIT_1
#define FSM_RF0_BP_EN		BIT_0

/*
  the LDO_SLP_CTL1 register bit
*/
#define FSM_SLPPD_EN		BIT_15
#define FSM_DCDCARM_BP_EN	BIT_4
#define FSM_SIM3_BP_EN		BIT_3
#define FSM_SIM2_BP_EN		BIT_2
#define FSM_WF1_BP_EN		BIT_1
#define FSM_WF0_BP_EN		BIT_0
/*
  the DCDC_CTL register bit
*/
#define DCDC_RESERVE_RST	BIT_13
#define DCDC_RESERVE		BIT_12
#define DCDC_DEDTDELAY_RST	BIT_11
#define DCDC_DEDTDELAY		BIT_10
#define DCDC_DEDTDEN_RST	BIT_9
#define DCDC_DEDTDEN	BIT_8

///ANA_CHGR_CTL0
#define CHGR_ADAPTER_EN		BIT_0
#define CHGR_ADAPTER_EN_RST	BIT_1
#define CHGR_USB_500MA_EN		BIT_2
#define CHGR_USB_500MA_EN_RST	BIT_3

#define CHGR_USB_CHG_SHIFT              4
#define CHGR_USB_CHG_MSK                (3 << CHGR_USB_CHG_SHIFT)
#define CHGR_ADAPTER_CHG_SHIFT          6
#define CHGR_ADAPTER_CHG_MSK            (3 << CHGR_ADAPTER_CHG_SHIFT)
#define CHGR_PD_BIT                     			BIT_8
#define PA_LDO_EN_RST					BIT_9
#define CHGR_RECHG_BIT                  		BIT_12
#define CHGR_ADATPER_EN_BIT             	BIT_0
#define CHGR_ADATPER_EN_RST_BIT       	BIT_1
#define CHGR_USB_500MA_EN_BIT           	BIT_2
#define CHGR_USB_500MA_EN_RST_BIT       BIT_3
#define CHAR_ADAPTER_MODE_MSK           	(BIT_0|BIT_1|BIT_2|BIT_3)

///ANA_CHGR_CTL1
#define CHAR_SW_POINT_SHIFT     	0
#define CHAR_SW_POINT_MSK       		(0x1F << CHAR_SW_POINT_SHIFT)

/*
  the VIBRATOR_CTL0 register bit
*/
#define VIBR_STABLE_V_SHIFT 12
#define VIBR_STABLE_V_MSK   (0x0F << VIBR_STABLE_V_SHIFT)
#define VIBR_INIT_V_SHIFT   8
#define VIBR_INIT_V_MSK     (0x0F << VIBR_INIT_V_SHIFT)
#define VIBR_V_BP_SHIFT         4
#define VIBR_V_BP_MSK           (0x0F << VIBR_V_BP_SHIFT)
#define VIBR_PD_RST				BIT_3
#define VIBR_PD_SET					BIT_2
#define VIBR_BP_EN      BIT_1
#define VIBR_RTC_EN     BIT_0
/*
  the AUDIO_CTL register bit
*/
#define VB_ARM_SOFT_RST	BIT_15
#define HEADDETECT_PD	BIT_7
#define LININRE_EN		BIT_3
#define VBMCLK_SOURCE_SEL	BIT_2
#define VBMCLK_ARM_ACC	BIT_1
#define VBMCLK_ARM_EN	BIT_0
/*
  the AUDIO_PA_CTL0 register bit
*/
#define PA_OCP_I	BIT_12
#define PA_OTP_PD	BIT_11
#define PA_VCM_EN	BIT_3
#define PA_STOP_EN	BIT_2
#define PA_EN_RST	BIT_5
#define PA_EN		BIT_4
/*
  the AUDIO_PA_CTL1 register bit
*/
#define PA_ABOCP_PD	BIT_15
#define PA_DOCP_PD	BIT_14
#define PA_DEMI_EN	BIT_11
#define PA_D_EN		BIT_10
#define PA_LDO_EN_RST	BIT_9
#define PA_LDO_EN	BIT_8
#define PA_LDOOCP_PD	BIT_7
#define PA_SWOCP_PD	BIT_2
#define PA_SW_EN_RST	BIT_1
#define PA_SW_EN		BIT_0
/*
  the ANA_MIXED_CTRL register bit
*/
#define PTEST_PD_SET	BIT_15
#define VIBR_PWR_ERR_CLR	BIT_7
#define CLKBT_EN	BIT_6
//#define CLK26M_REGS0
#define UVH0_EN_RST	BIT_3
#define UVH0_EN		BIT_2
#define OTP_EN_RST	BIT_1
#define OTP_EN		BIT_0
/*
  the ANA_STATUS register bit
*/
#define VIBR_PWR_ERR	BIT_15
#define BONDOPT2		BIT_10
#define VIBR_PD			BIT_9
#define WHTLED_PD		BIT_8
#define CHGR_ON			BIT_3
#define CHGR_STDBY		BIT_2
#define BONDOPT1		BIT_1
#define BONDOPT0		BIT_0
/*
  the IF_SPR_CTL register bit
*/
#define IF_SPR_IN	BIT_2
#define	IF_SPR_OE	BIT_1
#define IF_SPR_OUT	BIT_0

//ANA_HWRST_STATUS
#define HWRST_STATUS_POWERON_MASK (0xf0)
#define HWRST_STATUS_RECOVERY (0x20)
#define HWRST_STATUS_FASTBOOT (0X30)
#define HWRST_STATUS_NORMAL (0X40)

//ryan:add for poweroff debug.
///ANA_LDO_PD_SET
#define ANA_LDO_PD_SET_MSK  0x3FF
///ANA_LDO_PD_CTL
#define ANA_LDO_PD_CTL_MSK  0x5555
///ANA_LDO_PD_RST
#define ANA_LDO_PD_RST_MSK 0x0000
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

