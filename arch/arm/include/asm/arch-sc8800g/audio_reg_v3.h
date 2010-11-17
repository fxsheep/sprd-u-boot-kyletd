/******************************************************************************
 ** File Name:    audio_reg_v3.h                                           *
 ** Author:       Mingwei.Zhang                                               *
 ** DATE:         05/13/2010                                                  *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 05/13/2010    Mingwei.Zhang   Create.                                     *
 ******************************************************************************/
#ifndef _AUDIO_REG_V3_H_
#define _AUDIO_REG_V3_H_
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

/*
  Voice Band Codec register
*/
//BASE ADDRESS: ARM_VBC_BASE           0x82003000
#define VBDA0                   (ARM_VBC_BASE+0x0000)                   // (0x0000)
#define VBDA1                   (ARM_VBC_BASE+0x0004)                    // (0x0004)
#define VBAD0                   (ARM_VBC_BASE+0x0008) // (0x0008)
#define VBAD1                   (ARM_VBC_BASE+0x000C) // (0x000C)
#define VBBUFFERSIZE            (ARM_VBC_BASE+0x0010) // (0x0010)
#define VBADBUFFDTA         (ARM_VBC_BASE+0x0014) // (0x0014)
#define VBDABUFFDTA         (ARM_VBC_BASE+0x0018) // (0x0018)
#define VBADCNT                 (ARM_VBC_BASE+0x001C) // (0x001c)
#define VBDACNT                 (ARM_VBC_BASE+0x0020) // (0x0020)
#define VBINTTYPE               (ARM_VBC_BASE+0x0034)
#define DAPATCHCTL              (ARM_VBC_BASE+0x0040)
#define DADGCTL                 (ARM_VBC_BASE+0x0044)
#define DAHPCTL                 (ARM_VBC_BASE+0x0048)
#define DAALCCTL0               (ARM_VBC_BASE+0x004C)
#define DAALCCTL1               (ARM_VBC_BASE+0x0050)
#define DAALCCTL2               (ARM_VBC_BASE+0x0054)
#define DAALCCTL3               (ARM_VBC_BASE+0x0058)
#define DAALCCTL4               (ARM_VBC_BASE+0x005C)
#define DAALCCTL5               (ARM_VBC_BASE+0x0060)
#define DAALCCTL6               (ARM_VBC_BASE+0x0064)
#define DAALCCTL7               (ARM_VBC_BASE+0x0068)
#define DAALCCTL8               (ARM_VBC_BASE+0x006C)
#define DAALCCTL9               (ARM_VBC_BASE+0x0070)
#define DAALCCTL10              (ARM_VBC_BASE+0x0074)
#define STCTL0                  (ARM_VBC_BASE+0x0078)
#define STCTL1                  (ARM_VBC_BASE+0x007C)
#define ADPATCHCTL              (ARM_VBC_BASE+0x0080)
#define ADDGCTL                 (ARM_VBC_BASE+0x0084)
#define HPCOEF0                 (ARM_VBC_BASE+0x0100)
#define HPCOEF1                 (ARM_VBC_BASE+0x0104)
#define HPCOEF2                 (ARM_VBC_BASE+0x0108)
#define HPCOEF3                 (ARM_VBC_BASE+0x010C)
#define HPCOEF4                 (ARM_VBC_BASE+0x0110)
#define HPCOEF5                 (ARM_VBC_BASE+0x0114)
#define HPCOEF6                 (ARM_VBC_BASE+0x0118)
#define HPCOEF7                 (ARM_VBC_BASE+0x011C)
#define HPCOEF8                 (ARM_VBC_BASE+0x0120)
#define HPCOEF9                 (ARM_VBC_BASE+0x0124)
#define HPCOEF10                (ARM_VBC_BASE+0x0128)
#define HPCOEF11                (ARM_VBC_BASE+0x012C)
#define HPCOEF12                (ARM_VBC_BASE+0x0130)
#define HPCOEF13                (ARM_VBC_BASE+0x0134)
#define HPCOEF14                (ARM_VBC_BASE+0x0138)
#define HPCOEF15                (ARM_VBC_BASE+0x013C)
#define HPCOEF16                (ARM_VBC_BASE+0x0140)
#define HPCOEF17                (ARM_VBC_BASE+0x0144)
#define HPCOEF18                (ARM_VBC_BASE+0x0148)
#define HPCOEF19                (ARM_VBC_BASE+0x014C)
#define HPCOEF20                (ARM_VBC_BASE+0x0150)
#define HPCOEF21                (ARM_VBC_BASE+0x0154)
#define HPCOEF22                (ARM_VBC_BASE+0x0158)
#define HPCOEF23                (ARM_VBC_BASE+0x015C)
#define HPCOEF24                (ARM_VBC_BASE+0x0160)
#define HPCOEF25                (ARM_VBC_BASE+0x0164)
#define HPCOEF26                (ARM_VBC_BASE+0x0168)
#define HPCOEF27                (ARM_VBC_BASE+0x016C)
#define HPCOEF28                (ARM_VBC_BASE+0x0170)
#define HPCOEF29                (ARM_VBC_BASE+0x0174)
#define HPCOEF30                (ARM_VBC_BASE+0x0178)
#define HPCOEF31                (ARM_VBC_BASE+0x017C)
#define HPCOEF32                (ARM_VBC_BASE+0x0180)
#define HPCOEF33                (ARM_VBC_BASE+0x0184)
#define HPCOEF34                (ARM_VBC_BASE+0x0188)
#define HPCOEF35                (ARM_VBC_BASE+0x018C)
#define HPCOEF36                (ARM_VBC_BASE+0x0190)
#define HPCOEF37                (ARM_VBC_BASE+0x0194)
#define HPCOEF38                (ARM_VBC_BASE+0x0198)
#define HPCOEF39                (ARM_VBC_BASE+0x019C)
#define HPCOEF40                (ARM_VBC_BASE+0x01A0)
#define HPCOEF41                (ARM_VBC_BASE+0x01A4)
#define HPCOEF42                (ARM_VBC_BASE+0x01A8)


//internal dolphin register
//#define ANA_DOLPHIN_BASE          0x82000100  //Analog die register
#define VBAICR                  (ANA_DOLPHIN_BASE + 0x00)
#define VBCR1                   (ANA_DOLPHIN_BASE + 0x04)
#define VBCR2                   (ANA_DOLPHIN_BASE + 0x08)
#define VBCCR1                  (ANA_DOLPHIN_BASE + 0x0C)
#define VBCCR2                  (ANA_DOLPHIN_BASE + 0x10)
#define VBPMR1                  (ANA_DOLPHIN_BASE + 0x14)
#define VBPMR2                  (ANA_DOLPHIN_BASE + 0x18)
#define VBCRR                   (ANA_DOLPHIN_BASE + 0x1C)
#define VBICR                   (ANA_DOLPHIN_BASE + 0x20)
#define VBIFR                   (ANA_DOLPHIN_BASE + 0x24)
#define VBCGR1                  (ANA_DOLPHIN_BASE + 0x28)
#define VBCGR2                  (ANA_DOLPHIN_BASE + 0x2C)
#define VBCGR3                  (ANA_DOLPHIN_BASE + 0x30)
///0x0134~0x0140    Reserved
#define VBCGR8                  (ANA_DOLPHIN_BASE + 0x44)
#define VBCGR9                  (ANA_DOLPHIN_BASE + 0x48)
#define VBCGR10                 (ANA_DOLPHIN_BASE + 0x4C)
#define VBTR1                   (ANA_DOLPHIN_BASE + 0x50)
#define VBTR2                   (ANA_DOLPHIN_BASE + 0x54)


#define VB_DA_BUF_SIZE_OFFSET          (8)
#define VB_AD_BUF_SIZE_OFFSET          (0)

#define VB_ENABLE_OFFSET               (15)


#define VB_RAMSW_EN_OFFSET             (10)
#define VB_RAMSW_NUMB_OFFSET           (9)

//VBAICR


//VBCR1
#define VBCR1_BTL_MUTE_SHIFT       (1)
#define VBCR1_BTL_MUTE_MASK        (1 << VBCR1_BTL_MUTE_SHIFT)
#define VBCR1_BTL_MUTE_DISABLE     (0 << VBCR1_BTL_MUTE_SHIFT)
#define VBCR1_BTL_MUTE_ENABLE      (1 << VBCR1_BTL_MUTE_SHIFT)

#define VBCR1_BYPASS_SHIFT         (2)
#define VBCR1_BYPASS_MASK          (1 << VBCR1_BYPASS_SHIFT)
#define VBCR1_BYPASS_IGNORED       (0 << VBCR1_BYPASS_SHIFT)
#define VBCR1_BYPASS_SELECTED      (1 << VBCR1_BYPASS_SHIFT)

#define VBCR1_DACSEL_SHIFT         (3)
#define VBCR1_DACSEL_MASK          (1 << VBCR1_DACSEL_SHIFT)
#define VBCR1_DACSEL_IGNORED       (0 << VBCR1_DACSEL_SHIFT)
#define VBCR1_DACSEL_SELECTED      (1 << VBCR1_DACSEL_SHIFT)

#define VBCR1_HP_DIS_SHIFT         (4)
#define VBCR1_HP_DIS_MASK          (1 << VBCR1_HP_DIS_SHIFT)
#define VBCR1_HP_DIS_DISABLE       (0 << VBCR1_HP_DIS_SHIFT)
#define VBCR1_HP_DIS_ENABLE        (1 << VBCR1_HP_DIS_SHIFT)

#define VBCR1_DAC_MUTE_SHIFT       (5)
#define VBCR1_DAC_MUTE_MASK        (1 << VBCR1_DAC_MUTE_SHIFT)
#define VBCR1_DAC_MUTE_DISABLE     (0 << VBCR1_DAC_MUTE_SHIFT)
#define VBCR1_DAC_MUTE_ENABLE      (1 << VBCR1_DAC_MUTE_SHIFT)

#define VBCR1_MONO_SHIFT           (6)
#define VBCR1_MONO_MASK            (1 << VBCR1_MONO_SHIFT)
#define VBCR1_MONO_DISABLE         (0 << VBCR1_MONO_SHIFT)
#define VBCR1_MONO_ENABLE          (1 << VBCR1_MONO_SHIFT)

#define VBCR1_SB_MICBIAS_SHIFT     (7)
#define VBCR1_SB_MICBIAS_MASK      (1 << VBCR1_SB_MICBIAS_SHIFT)
#define VBCR1_SB_MICBIAS_ENABLE    (0 << VBCR1_SB_MICBIAS_SHIFT)
#define VBCR1_SB_MICBIAS_DISABLE   (1 << VBCR1_SB_MICBIAS_SHIFT)

//VBCR2
#define VBCR2_MICSEL_SHIFT         (1)
#define VBCR2_MICSEL_MASK          (1 << VBCR2_MICSEL_SHIFT)
#define VBCR2_MICSEL_MIC1          (0 << VBCR2_MICSEL_SHIFT)
#define VBCR2_MICSEL_MIC2          (1 << VBCR2_MICSEL_SHIFT)

#define VBCR2_ADCHPF_SHIFT         (2)
#define VBCR2_ADCHPF_MASK          (1 << VBCR2_ADCHPF_SHIFT)
#define VBCR2_ADCHPF_DIS           (0 << VBCR2_ADCHPF_SHIFT)
#define VBCR2_ADCHPF_EN            (1 << VBCR2_ADCHPF_SHIFT)

#define VBCR2_ADCADWL_SHIFT        (3)
#define VBCR2_ADCADWL_MASK         (0x3<<VBCR2_ADCADWL_SHIFT)

#define VBCR2_DACADWL_SHIFT        (5)
#define VBCR2_DACADWL_MASK         (0x3<<VBCR2_DACADWL_SHIFT)

#define VBCR2_DACDEEMP_SHIFT       (7)
//ADI_VBC_CCR
#define VBCCR_ADC_FS_SHIFT         (0)
#define VBCCR_ADC_FS_MASK          (0xF << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_96000         (0 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_48000         (1 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_44100         (2 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_32000         (3 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_24000         (4 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_22050         (5 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_16000         (6 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_12000         (7 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_11025         (8 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_9600          (9 << VBCCR_ADC_FS_SHIFT)
#define VBCCR_ADC_FS_8000          (10<< VBCCR_ADC_FS_SHIFT)

#define VBCCR_DAC_FS_SHIFT         (4)
#define VBCCR_DAC_FS_MASK          (0xF << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_96000         (0 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_48000         (1 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_44100         (2 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_32000         (3 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_24000         (4 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_22050         (5 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_16000         (6 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_12000         (7 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_11025         (8 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_9600          (9 << VBCCR_DAC_FS_SHIFT)
#define VBCCR_DAC_FS_8000          (10 << VBCCR_DAC_FS_SHIFT)


//ADI_VBC_PMR1
#define VBPMR1_SB_LOUT_SHIFT       (1)
#define VBPMR1_SB_LOUT_MASK        (1 << VBPMR1_SB_LOUT_SHIFT)
#define VBPMR1_SB_LOUT_POWER_ON    (0 << VBPMR1_SB_LOUT_SHIFT)
#define VBPMR1_SB_LOUT_POWER_DOWN  (1 << VBPMR1_SB_LOUT_SHIFT)

#define VBPMR1_SB_BTL_SHIFT        (2)
#define VBPMR1_SB_BTL_MASK         (1 << VBPMR1_SB_BTL_SHIFT)
#define VBPMR1_SB_BTL_POWER_ON     (0 << VBPMR1_SB_BTL_SHIFT)
#define VBPMR1_SB_BTL_POWER_DOWN   (1 << VBPMR1_SB_BTL_SHIFT)

#define VBPMR1_SB_LIN_SHIFT        (3)
#define VBPMR1_SB_LIN_MASK         (1 << VBPMR1_SB_LIN_SHIFT)
#define VBPMR1_SB_LIN_POWER_ON     (0 << VBPMR1_SB_LIN_SHIFT)
#define VBPMR1_SB_LIN_POWER_DOWN   (1 << VBPMR1_SB_LIN_SHIFT)

#define VBPMR1_SB_ADC_SHIFT        (4)
#define VBPMR1_SB_ADC_MASK         (1 << VBPMR1_SB_ADC_SHIFT)
#define VBPMR1_SB_ADC_POWER_ON     (0 << VBPMR1_SB_ADC_SHIFT)
#define VBPMR1_SB_ADC_POWER_DOWN   (1 << VBPMR1_SB_ADC_SHIFT)

#define VBPMR1_SB_MIX_SHIFT         (5)
#define VBPMR1_SB_MIX_MASK          (1 << VBPMR1_SB_MIX_SHIFT)
#define VBPMR1_SB_MIX_POWER_ON      (0 << VBPMR1_SB_MIX_SHIFT)
#define VBPMR1_SB_MIX_POWER_DOWN    (1 << VBPMR1_SB_MIX_SHIFT)

#define VBPMR1_SB_OUT_SHIFT        (6)
#define VBPMR1_SB_OUT_MASK         (1 << VBPMR1_SB_OUT_SHIFT)
#define VBPMR1_SB_OUT_POWER_ON     (0 << VBPMR1_SB_OUT_SHIFT)
#define VBPMR1_SB_OUT_POWER_DOWN   (1 << VBPMR1_SB_OUT_SHIFT)

#define VBPMR1_SB_DAC_SHIFT        (7)
#define VBPMR1_SB_DAC_MASK         (1 << VBPMR1_SB_DAC_SHIFT)
#define VBPMR1_SB_DAC_POWER_ON     (0 << VBPMR1_SB_DAC_SHIFT)
#define VBPMR1_SB_DAC_POWER_DOWN   (1 << VBPMR1_SB_DAC_SHIFT)

//ADI_VBC_PMR2
#define VBPMR2_SB_SLEEP_SHIFT       (0)
#define VBPMR2_SB_SLEEP_MASK        (1 << VBPMR2_SB_SLEEP_SHIFT)
#define VBPMR2_SB_SLEEP_POWER_ON    (0 << VBPMR2_SB_SLEEP_SHIFT)
#define VBPMR2_SB_SLEEP_POWER_DOWN  (1 << VBPMR2_SB_SLEEP_SHIFT)

#define VBPMR2_SB_SHIFT             (1)
#define VBPMR2_SB_MASK              (1 << VBPMR2_SB_SHIFT)
#define VBPMR2_SB_POWER_ON          (0 << VBPMR2_SB_SHIFT)
#define VBPMR2_SB_POWER_DOWN        (1 << VBPMR2_SB_SHIFT)

#define VBPMR2_SB_MC_SHIFT          (2)
#define VBPMR2_SB_MC_MASK           (1 << VBPMR2_SB_MC_SHIFT)
#define VBPMR2_SB_MC_POWER_ON       (0 << VBPMR2_SB_MC_SHIFT)
#define VBPMR2_SB_MC_POWER_DOWN     (1 << VBPMR2_SB_MC_SHIFT)


#define VBPMR2_GIM_SHIFT            (3)
#define VBPMR2_GIM_MASK             (1 << VBPMR2_GIM_SHIFT)
#define VBPMR2_GIM_POWER_ON         (0 << VBPMR2_GIM_SHIFT)
#define VBPMR2_GIM_POWER_DOWN       (1 << VBPMR2_GIM_SHIFT)

#define VBPMR2_RLGOD_SHIFT            (4)
#define VBPMR2_RLGOD_MASK             (0x3 << VBPMR2_RLGOD_SHIFT)


//VBIFR
#define VBIFR_GAIN_DOWN_DONE_SHIFT            (0)
#define VBIFR_GAIN_DOWN_DONE_MASK             (1 << VBIFR_GAIN_DOWN_DONE_SHIFT)

#define VBIFR_GAIN_UP_DONE_SHIFT              (1)
#define VBIFR_GAIN_UP_DONE_MASK               (1 << VBIFR_GAIN_UP_DONE_SHIFT)

#define VBIFR_RAMP_DOWN_DONE_SHIFT            (2)
#define VBIFR_RAMP_DOWN_DONE_MASK             (1 << VBIFR_RAMP_DOWN_DONE_SHIFT)

#define VBIFR_RAMP_UP_DONE_SHIFT              (3)
#define VBIFR_RAMP_UP_DONE_MASK               (1 << VBIFR_RAMP_UP_DONE_SHIFT)

//VBCGR1
#define VBCGR1_GODL_SHIFT           (0)
#define VBCGR1_GODL_MASK            (0xF << VBCGR1_GODL_SHIFT)

#define VBCGR1_GODR_SHIFT           (4)
#define VBCGR1_GODR_MASK            (0xF << VBCGR1_GODR_SHIFT)

//VBCGR2
#define VBCGR2_GOBL_SHIFT           (0)
#define VBCGR2_GOBL_MASK            (0x1F << VBCGR2_GOBL_SHIFT)

#define VBCGR2_LRGOB_SHIFT           (6)
#define VBCGR2_LRGOB_MASK            (0x3 << VBCGR2_LRGOB_SHIFT)

//VBCGR3
#define VBCGR3_GOBR_SHIFT           (0)
#define VBCGR3_GOBR_MASK            (0x1F << VBCGR3_GOBR_SHIFT)

//VBCGR8
#define VBCGR8_GOL_SHIFT            (0)
#define VBCGR8_GOL_MASK             (0x1F << VBCGR8_GOL_SHIFT)

#define VBCGR8_LRGO_SHIFT           (6)
#define VBCGR8_LRGO_MASK            (0x3 << VBCGR8_LRGO_SHIFT)

//VBCGR9
#define VBCGR9_GOR_SHIFT            (0)
#define VBCGR9_GOR_MASK             (0x1F << VBCGR9_GOR_SHIFT)

//VBCGR10
#define VBCGR10_GI_SHIFT             (4)
#define VBCGR10_GI_MASK              (0xF << VBCGR10_GI_SHIFT)

//VBBUFFSIZE
#define VBADBUFFSIZE_SHIFT          (0)
#define VBDABUFFSIZE_SHIFT          (8)
//VBADBUFFDTA
#define VBPCM_MODE_SHIFT            (14)
//VBDABUFFDTA
#define VBRAMSW_NUMBER_SHIFT          (9)
#define VBRAMSW_EN_SHIFT              (10)
#define VBENABLE_SHIFT                (15)
#define VBAD0DMA_EN_SHIFT        (11)
#define VBDA0DMA_EN_SHIFT        (13)
#define VBDA1DMA_EN_SHIFT        (14)

#define  VBHP_LIMIT_SHIFT     (0)
#define  VBHP_LIMIT_MASK      (0xFF << VBHP_LIMIT_SHIFT)
#define  VBHP_WID_SEL_SHIFT   (8)
#define  VBHP_WID_SEL_MASK    (1<<VBHP_WID_SEL_SHIFT)
#define  VBHP_WID_SEL_16BITS  (0<<VBHP_WID_SEL_SHIFT)
#define  VBHP_WID_SEL_24BITS  (1<<VBHP_WID_SEL_SHIFT)
#define  VBHP_REG_CLR_SHIFT   (9)
#define  VBHP_REG_CLR_MASK    (1<<VBHP_REG_CLR_SHIFT)
#define  VBHP_REG_CLR_ON      (1<<VBHP_REG_CLR_SHIFT)
#define  VBHP_REG_CLR_OFF     (0<<VBHP_REG_CLR_SHIFT)
#define  VBHP_EN_SHIFT        (10)
#define  VBHP_EN_MASK         (1<<VBHP_EN_SHIFT)
#define  VBHP_EN_ENABLE       (1<<VBHP_EN_SHIFT)
#define  VBHP_EN_DISABLE      (0<<VBHP_EN_SHIFT)
// Global INT BIT
#define VB_DA_INT_SHIFT                 (BIT_6)
#define VB_AD_INT_SHIFT                 (BIT_5)

#define INT_FLAG_TIMEOUT               0x80000



#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_AUDIO_REG_V3_H_


