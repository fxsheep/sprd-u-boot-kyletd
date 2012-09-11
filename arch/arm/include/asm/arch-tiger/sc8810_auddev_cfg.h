/*name:      sc8800g_auddev_cfg.h                                      *
 ** Author:         Jeff Li                                                   *
 ** DATE:           16/09/2010                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    Configurations of audio device                            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 16/09/2010     Jeff.Li          Create.                                   *
 ******************************************************************************/
#ifndef _SC8810_AUDDEV_CFG_H_
#define _SC8810_AUDDEV_CFG_H_

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "adi_hal_internal.h"
#include "analog_reg_v3.h"
#include "sc8810_reg_global.h"
#include "chip_drv_common_io.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/
#define DOL_WRITE_BITS(_Reg, _Value, _Mask)     ANA_REG_MSK_OR(_Reg, _Value, _Mask)
#define DOL_REG_GET(_Reg)                       ANA_REG_GET(_Reg)
#define DOLPHIN_REG_BASE    ANA_DOLPHIN_BASE

#define VB_WRITE_BITS(_Reg, _Value, _Mask)          \
    {                                               \
        CHIP_REG_AND (_Reg, ~((_Mask)));            \
        CHIP_REG_OR (_Reg, ((_Value) & ( _Mask)));  \
    }

#define VB_CLK_CTL              GR_BUSCLK_ALM

#define VB_DA_INT_MASK          IRQ_VBCDA_IRQ
#define VB_AD_INT_MASK          IRQ_VBCAD_IRQ

#define VB_DA_INT_CLR_MASK      ICLR_VBCDA_IRQ_CLR
#define VB_AD_INT_CLR_MASK      ICLR_VBCAD_IRQ_CLR

static __inline void vb_switch_arm_ctl (BOOLEAN is_arm)
{
    if (is_arm)
    {
        ANA_REG_MSK_OR (ANA_AUDIO_CTL, (VBMCLK_ARM_EN | VBMCLK_ARM_ACC),
                        (VBMCLK_ARM_EN | VBMCLK_ARM_EN));
        REG32 (VB_CLK_CTL) |= ARM_VB_ACC;
    }
    else
    {
        ANA_REG_MSK_OR (ANA_AUDIO_CTL, VBMCLK_ARM_EN,
                        (VBMCLK_ARM_EN | VBMCLK_ARM_ACC));
        REG32 (VB_CLK_CTL) &= ~ARM_VB_ACC;
    }
}

#define VB_SWTICH_ARM_CTL(x)        vb_switch_arm_ctl(x)

#define DOL_DAC_GOD_SHIFT           (0)
#define DOL_DAC_GOD_MASK            (0xF << DOL_DAC_GOD_SHIFT)
#define DOL_DAC_GO_SHIFT            (4)
#define DOL_DAC_GO_MASK             (0x1F << 4)

#define DOL_LINEIN_GOB_SHIFT        (0)
#define DOL_LINEIN_GOB_MASK         (0x1F << 0)
#define DOL_LINEIN_GO_SHIFT         (5)
#define DOL_LINEIN_GO_MASK          (0x1F << 5)

#define DOL_ADC_GI_SHIFT            (0)
#define DOL_ADC_GI_MASK             (0xF << DOL_ADC_GI_SHIFT)
#define DOL_ADC_GIM_SHIFT           (4)
#define DOL_ADC_GIM_MASK            (0x1 << DOL_ADC_GIM_SHIFT)

#define DOL_SAMPLERATE_NUM_MAX      11

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif

