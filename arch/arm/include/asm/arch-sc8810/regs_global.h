#ifndef REGS_GLOBAL_H
#define REGS_GLOBAL_H

/* arch/arm/mach-sc8800s/include/mach/regs_global.h
 *
 * Copyright (C) 2010 Spreadtrum
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
 #include <asm/arch/bits.h>
 
/*----------Global Registers----------*/
#define GREG_BASE     0x8B000000

#define GR_GEN0                         (GREG_BASE + 0x0008)
#define GR_PCTL                         (GREG_BASE + 0x000C)
#define GR_IRQ                          (GREG_BASE + 0x0010)
#define GR_ICLR                         (GREG_BASE + 0x0014)
#define GR_GEN1                         (GREG_BASE + 0x0018)
#define GR_GEN3                         (GREG_BASE + 0x001C)
//#define   GR_HWRST                        (GREG_BASE + 0x0020)
#define BOOT_FLAG                       (GREG_BASE + 0x0020)   //It's called GR_HWRST in other chip define
#define GR_MPLL_MN                      (GREG_BASE + 0x0024)
#define GR_PIN_CTL                      (GREG_BASE + 0x0028)
#define GR_GEN2                         (GREG_BASE + 0x002C)
#define GR_ARM_BOOT_ADDR                (GREG_BASE + 0x0030)
#define GR_STC_STATE                    (GREG_BASE + 0x0034)
//reserved
#define GR_BUSCLK                       (GREG_BASE + 0x0044)    ////GR_BUSCLK_ALM
#define GR_ARCH_CTL                     (GREG_BASE + 0x0048)
#define GR_SOFT_RST                     (GREG_BASE + 0x004C)
//reserved
#define GR_NFC_MEM_DLY                  (GREG_BASE + 0x0058)
#define GR_CLK_DLY                      (GREG_BASE + 0x005C)
#define GR_GEN4                         (GREG_BASE + 0x0060)
//reserved
#define GR_POWCTL0                  (GREG_BASE + 0x0068)
#define GR_POWCTL1                  (GREG_BASE + 0x006C)
#define GR_PLL_SCR                      (GREG_BASE + 0x0070)
#define GR_CLK_EN                       (GREG_BASE + 0x0074)
//reserved
#define GR_CLK_GEN5                     (GREG_BASE + 0x007C)
//reserved
#define GR_SWRST                        GR_SOFT_RST             ////mingweiflag GR_SOFT_RST or GR_SWRST?
#define GR_BUSCLK_ALM                   GR_BUSCLK
#define LDO_USB_PD BIT_9
/*
  the GEN0 register bit
*/
//reserved
#define GEN0_TIMER_EN                   BIT_2
#define GEN0_SIM0_EN                    BIT_3
#define GEN0_I2C_EN                     BIT_4
#define GEN0_GPIO_EN                    BIT_5
#define GEN0_ADI_EN                     BIT_6
#define GEN0_EFUSE_EN                   BIT_7
#define GEN0_KPD_EN                     BIT_8
//reserved
#define GEN0_MCU_DSP_RST                BIT_10
#define GEN0_MCU_SOFT_RST               BIT_11
#define GEN0_I2S_EN                     BIT_12
#define GEN0_PIN_EN                     BIT_13
#define GEN0_CCIR_MCLK_EN               BIT_14
#define GEN0_EPT_EN                     BIT_15
#define GEN0_SIM1_EN                    BIT_16
#define GEN0_SPI_EN                     BIT_17
//reserved
#define GEN0_SYST_EN                    BIT_19
#define GEN0_UART0_EN                   BIT_20
#define GEN0_UART1_EN                   BIT_21
#define GEN0_UART2_EN                   BIT_22
#define GEN0_VB_EN                      BIT_23
#define GEN0_GPIO_RTC_EN                BIT_24
//reserved
#define GEN0_KPD_RTC_EN                 BIT_26
#define GEN0_SYST_RTC_EN                BIT_27
#define GEN0_TMR_RTC_EN                 BIT_28
//reserved

/*
  the GEN1 register bit
*/
#define GEN1_MPLL_MN_EN                 BIT_9
#define GEN1_CLK_AUX0_EN                BIT_10
#define GEN1_CLK_AUX1_EN                BIT_11
//reserved
#define GEN1_RTC_ARCH_EN                BIT_18
//reserved

#define  MISC0_UART1_MUX_SEL         BIT_8

/*
  the APB Soft Reset register bit
*/
#define SWRST_I2C_RST                   BIT_0
#define SWRST_KPD_RST                   BIT_1
//reserved
#define SWRST_SIM0_RST                  BIT_5
#define SWRST_SIM1_RST                  BIT_6
//reserved
#define SWRST_TIMER_RST                 BIT_8
//reserved
#define SWRST_EPT_RST                   BIT_10
#define SWRST_UART0_RST                 BIT_11
#define SWRST_UART1_RST                 BIT_12
#define SWRST_UART2_RST                 BIT_13
#define SWRST_SPI_RST                   BIT_14
//reserved
#define SWRST_IIS_RST                   BIT_16
//reserved
#define SWRST_SYST_RST                  BIT_19
#define SWRST_PINREG_RST                BIT_20
#define SWRST_GPIO_RST                  BIT_21
#define ADI_SOFT_RST                    BIT_22
#define SWRST_VBC_RST                   BIT_23
#define SWRST_PWM0_RST                  BIT_24
#define SWRST_PWM1_RST                  BIT_25
#define SWRST_PWM2_RST                  BIT_26
#define SWRST_PWM3_RST                  BIT_27
#define SWRST_EFUSE_RST                 BIT_28
//reserved

/*
  the ARM VB CTRL register bit
*/
#define ARM_VB_IIS_SEL                  BIT_0
#define ARM_VB_MCLKON                   BIT_1
#define ARM_VB_DA0ON                    BIT_2
#define ARM_VB_DA1ON                    BIT_3
#define ARM_VB_AD0ON                    BIT_4
#define ARM_VB_AD1ON                    BIT_5
#define ARM_VB_ANAON                    BIT_6
#define ARM_VB_ACC                      BIT_7

#define ARM_VB_ADCON                    ARM_VB_AD0ON
//reserved

/*
  the Interrupt control register bit
*/
#define IRQ_MCU_IRQ0                    BIT_0
#define IRQ_MCU_FRQ0                    BIT_1
#define IRQ_MCU_IRQ1                    BIT_2
#define IRQ_MCU_FRQ1                    BIT_3
//reserved
#define IRQ_VBCAD_IRQ                   BIT_5
#define IRQ_VBCDA_IRQ                   BIT_6
//reserved
#define IRQ_RFT_INT                     BIT_12
//reserved
/*
  the Interrupt clear register bit
*/
#define ICLR_DSP_IRQ0_CLR               BIT_0
#define ICLR_DSP_FRQ0_CLR               BIT_1
#define ICLR_DSP_IRQ1_CLR               BIT_2
#define ICLR_DSP_FIQ1_CLR               BIT_3
//reserved
#define ICLR_VBCAD_IRQ_CLR              BIT_5
#define ICLR_VBCDA_IRQ_CLR              BIT_6
//reserved
#define ICLR_RFT_INT_CLR                BIT_12
//reserved

/*
  the Clock enable register bit
*/

#define CLK_PWM0_EN                     BIT_21
#define CLK_PWM1_EN                     BIT_22
#define CLK_PWM2_EN                     BIT_23
#define CLK_PWM3_EN                     BIT_24
#define CLK_PWM0_SEL                        BIT_25
#define CLK_PWM1_SEL                        BIT_26
#define CLK_PWM2_SEL                        BIT_27
#define CLK_PWM3_SEL                        BIT_28

// POWER CTL1
#define POWCTL1_CONFIG                  0x7FFFF903

#endif //_SC8800G_REG_GLOBAL_H_
// End


