/******************************************************************************
 ** File Name:    sc8810_reg_global.h                                        *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         11/13/2005                                                  *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 11/13/2005    Daniel.Ding     Create.                                     *
 ** 05/05/2010    Mingwei.Zhang   Modified for SC8800G                        *
 ******************************************************************************/
#ifndef _SC8810_REG_GLOBAL_H_
#define _SC8810_REG_GLOBAL_H_
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


/*----------Global Registers----------*/
//GREG_BASE     0x8B000000
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
#define POWCTL1_CONFIG                  0x7FFFF91E  // isolation number 1ms:30cycles


/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
#ifdef CHIP_ENDIAN_LITTLE
typedef union _gr_anatst_ctl_tag
{
    struct _gr_anatst_ctl_map
    {
        volatile unsigned int   start_en1u          :1;
        volatile unsigned int   start_en1u_rst      :1;
        volatile unsigned int   start_en2u          :1;
        volatile unsigned int   start_en2u_rst      :1;
        volatile unsigned int   start_en3u          :1;
        volatile unsigned int   start_en3u_rst      :1;
        volatile unsigned int   start_en6u          :1;
        volatile unsigned int   start_en6u_rst      :1;
        volatile unsigned int   Ldo_bpnf            :1;
        volatile unsigned int   Ldo_bpnf_rst        :1;
        volatile unsigned int   Ldo_bprf2           :1;
        volatile unsigned int   Ldo_bprf2_rst       :1;
        volatile unsigned int   Ldo_bpusb           :2; //USB
        volatile unsigned int   recharge            :1;
        volatile unsigned int   standby             :1;
        volatile unsigned int   Slp_usb_en          :1;
        volatile unsigned int   Reserved            :1;
        volatile unsigned int   Ldo_ldo3_b0         :1;
        volatile unsigned int   Ldo_ldo3_b0_rst     :1;
        volatile unsigned int   Ldo_ldo3_b1         :1;
        volatile unsigned int   Ldo_ldo3_b1_rst     :1;
        volatile unsigned int   Ldo_ldo2_b0         :1;
        volatile unsigned int   Ldo_ldo2_b0_rst     :1;
        volatile unsigned int   Adapter_en          :2;
        volatile unsigned int   Usb_500ma_en        :2;
        volatile unsigned int   Charger_ctl         :4;
    } mBits ;
    volatile unsigned int dwValue ;
} GR_ANATST_CTL_U;


typedef union _gr_nfc_mem_dly_tag
{
    struct _gr_nfc_mem_dly_map
    {
        volatile unsigned int nefc_cen_dly_sel      :3;
        volatile unsigned int nfc_cle_dly_sel       :3;
        volatile unsigned int nfc_ale_dly_sel       :3;
        volatile unsigned int nfc_wen_dly_sel       :3;
        volatile unsigned int nfc_ren_dly_sel       :3;
        volatile unsigned int nfc_wpn_dly_sel       :3;
        volatile unsigned int nfc_data0_dly_sel     :3;
        volatile unsigned int nfc_data8_dly_sel     :3;
        volatile unsigned int reserved              :8;//Reserved
    } mBits;
    volatile unsigned int dwValue;
} GR_NFC_MEM_DLY_U;

typedef union _gr_gen1_reg_tag
{
    struct _gr_gen1_reg_map
    {
        volatile unsigned int vlk_aux0_div          :7;
        volatile unsigned int reserved_2            :1;
        volatile unsigned int gea_eb2               :1;
        volatile unsigned int m_pllmn_we            :1;
        volatile unsigned int clk_aux0_en           :1;
        volatile unsigned int clk_aux1_en           :1;
        volatile unsigned int testmodep_mcu2        :1;
        volatile unsigned int syst_en3              :1;
        volatile unsigned int serclk_eb3            :1;
        volatile unsigned int clk_26mhz_en          :1;
        volatile unsigned int clk_aux0_sel          :2;
        volatile unsigned int clk_aux1_sel          :2;
        volatile unsigned int v_pllmn_we            :1;
        volatile unsigned int a_pllmn_we            :1;
        volatile unsigned int serclk_eb0            :1;
        volatile unsigned int serclk_eb1            :1;
        volatile unsigned int serclk_eb2            :1;
        volatile unsigned int arm_boot_md0          :1;
        volatile unsigned int arm_boot_md1          :1;
        volatile unsigned int arm_boot_md2          :1;
        volatile unsigned int arm_boot_md3          :1;
        volatile unsigned int arm_boot_md4          :1;
        volatile unsigned int arm_boot_md5          :1;
        volatile unsigned int reserved_1            :1;//Reserved
    } mBits;
    volatile unsigned int dwValue;
} GR_GEN1_REG_U;

typedef union _gr_glb_gen4_reg_tag
{
    struct _gr_glb_gen4_reg_map
    {
        volatile unsigned int clk_lcdc_div          :7;
        volatile unsigned int reserved_2            :1;
        volatile unsigned int clk_pll_source_sel    :8;
        volatile unsigned int reserved_1            :16;
    } mBits;
    volatile unsigned int dwValue;
} GR_GLB_GEN4_REG_U;
#else
typedef union _gr_anatst_ctl_tag
{
    struct _gr_anatst_ctl_map
    {
        volatile unsigned int   Charger_ctl         :4;
        volatile unsigned int   Usb_500ma_en        :2;
        volatile unsigned int   Adapter_en          :2;
        volatile unsigned int   Ldo_ldo2_b0_rst     :1;
        volatile unsigned int   Ldo_ldo2_b0         :1;
        volatile unsigned int   Ldo_ldo3_b1_rst     :1;
        volatile unsigned int   Ldo_ldo3_b1         :1;
        volatile unsigned int   Ldo_ldo3_b0_rst     :1;
        volatile unsigned int   Ldo_ldo3_b0         :1;
        volatile unsigned int   Reserved            :1;
        volatile unsigned int   Slp_usb_en          :1;
        volatile unsigned int   standby             :1;
        volatile unsigned int   recharge            :1;
        volatile unsigned int   Ldo_bpusb           :2; //USB
        volatile unsigned int   Ldo_bprf2_rst       :1;
        volatile unsigned int   Ldo_bprf2           :1;
        volatile unsigned int   Ldo_bpnf_rst        :1;
        volatile unsigned int   Ldo_bpnf            :1;
        volatile unsigned int   start_en6u_rst      :1;
        volatile unsigned int   start_en6u          :1;
        volatile unsigned int   start_en3u_rst      :1;
        volatile unsigned int   start_en3u          :1;
        volatile unsigned int   start_en2u_rst      :1;
        volatile unsigned int   start_en2u          :1;
        volatile unsigned int   start_en1u_rst      :1;
        volatile unsigned int   start_en1u          :1;
    } mBits ;
    volatile unsigned int dwValue ;
} GR_ANATST_CTL_U;


typedef union _gr_nfc_mem_dly_tag
{
    struct _gr_nfc_mem_dly_map
    {
        volatile unsigned int reserved              :8;//Reserved
        volatile unsigned int nfc_data8_dly_sel     :3;
        volatile unsigned int nfc_data0_dly_sel     :3;
        volatile unsigned int nfc_wpn_dly_sel       :3;
        volatile unsigned int nfc_ren_dly_sel       :3;
        volatile unsigned int nfc_wen_dly_sel       :3;
        volatile unsigned int nfc_ale_dly_sel       :3;
        volatile unsigned int nfc_cle_dly_sel       :3;
        volatile unsigned int nefc_cen_dly_sel      :3;
    } mBits;
    volatile unsigned int dwValue;
} GR_NFC_MEM_DLY_U;

typedef union _gr_gen1_reg_tag
{
    struct _gr_gen1_reg_map
    {
        volatile unsigned int reserved_1            :1;//Reserved
        volatile unsigned int arm_boot_md5          :1;
        volatile unsigned int arm_boot_md4          :1;
        volatile unsigned int arm_boot_md3          :1;
        volatile unsigned int arm_boot_md2          :1;
        volatile unsigned int arm_boot_md1          :1;
        volatile unsigned int arm_boot_md0          :1;
        volatile unsigned int serclk_eb2            :1;
        volatile unsigned int serclk_eb1            :1;
        volatile unsigned int serclk_eb0            :1;
        volatile unsigned int a_pllmn_we            :1;
        volatile unsigned int v_pllmn_we            :1;
        volatile unsigned int clk_aux1_sel          :2;
        volatile unsigned int clk_aux0_sel          :2;
        volatile unsigned int clk_26mhz_en          :1;
        volatile unsigned int serclk_eb3            :1;
        volatile unsigned int syst_en3              :1;
        volatile unsigned int testmodep_mcu2        :1;
        volatile unsigned int clk_aux1_en           :1;
        volatile unsigned int clk_aux0_en           :1;
        volatile unsigned int m_pllmn_we            :1;
        volatile unsigned int gea_eb2               :1;
        volatile unsigned int reserved_2            :1;
        volatile unsigned int vlk_aux0_div          :7;
    } mBits;
    volatile unsigned int dwValue;
} GR_GEN1_REG_U;

typedef union _gr_glb_gen4_reg_tag
{
    struct _gr_glb_gen4_reg_map
    {
        volatile unsigned int reserved_1            :16;
        volatile unsigned int clk_pll_source_sel    :8;
        volatile unsigned int reserved_2            :1;
        volatile unsigned int clk_lcdc_div          :7;
    } mBits;
    volatile unsigned int dwValue;
} GR_GLB_GEN4_REG_U;
#endif
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
#endif //_SC8810_REG_GLOBAL_H_
// End


