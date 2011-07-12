/******************************************************************************
 ** File Name:    sc8800g_reg_ahb.h                                            *
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
 ** 05/07/2010    Mingwei.zhang   Modify it for SC8800G.                      *
 ******************************************************************************/
#ifndef _SC8800G_REG_AHB_H_
#define _SC8800G_REG_AHB_H_
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
#define AHB_REG_BASE            0x20900200
#define CHIP_TYPE               0x209003FC

#define AHB_CTL0                (AHB_REG_BASE + 0x00)
#define AHB_CTL1                (AHB_REG_BASE + 0x04)
#define AHB_CTL2                (AHB_REG_BASE + 0x08)
#define AHB_CTL3                (AHB_REG_BASE + 0x0C)
#define AHB_SOFT_RST            (AHB_REG_BASE + 0x10)
#define AHB_PAUSE               (AHB_REG_BASE + 0x14)
#define AHB_REMAP               (AHB_REG_BASE + 0x18)
#define AHB_ARM_CLK             (AHB_REG_BASE + 0x24)
#define AHB_SDIO_CTL            (AHB_REG_BASE + 0x28)
#define AHB_CTL4                (AHB_REG_BASE + 0x2C)
#define AHB_ENDIAN_SEL          (AHB_REG_BASE + 0x30)
#define AHB_STS                 (AHB_REG_BASE + 0x34)
#define DSP_BOOT_EN             (AHB_REG_BASE + 0x84)
#define DSP_BOOT_VEC            (AHB_REG_BASE + 0x88)
#define DSP_RST                 (AHB_REG_BASE + 0x8C)
#define AHB_ENDIAN_EN           (AHB_REG_BASE + 0x90)
#define USB_PHY_CTRL            (AHB_REG_BASE + 0xA0)

#define CHIP_ID                 (AHB_REG_BASE + 0x1FC)

#define AHB_DSP_BOOT_EN             (AHB_REG_BASE + 0x84)
#define AHB_DSP_BOOT_VECTOR         (AHB_REG_BASE + 0x88)
#define AHB_DSP_RESET               (AHB_REG_BASE + 0x8C)
#define AHB_BIGEND_PROT_REG    (AHB_REG_BASE + 0x90)

#define AHB_CTL0_DCAM_EN   BIT_1
#define AHB_CTL0_CCIR_EN    BIT_2
#define AHB_CTL0_LCDC_EN    BIT_3
#define AHB_CTL0_SDIO_EN    BIT_4
#define AHB_CTL0_USBD_EN    BIT_5
#define AHB_CTL0_DMA_EN     BIT_6
#define AHB_CTL0_BM0_EN     (BIT_7)
#define AHB_CTL0_NFC_EN      BIT_8
#define AHB_CTL0_BM1_EN     (BIT_11)
#define AHB_CTL0_VSP_EN      BIT_13
#define AHB_CTL0_ROT_EN      BIT_14
#define AHB_CTL0_DRM_EN     BIT_18
#define AHB_CTL0_AHB_ARCH_EB   BIT_15
#define AHB_CTL0_EMC_EN   BIT_28

#define AHB_BIGENDIAN_DMA           BIT_0
#define AHB_BIGENDIAN_NFC						BIT_1
#define AHB_BIGENDIAN_LCDC					BIT_2
#define AHB_BIGENDIAN_SDIO					BIT_3
#define AHB_BIGENDIAN_DCAM					BIT_4
#define AHB_BIGENDIAN_VSP						BIT_5
#define AHB_BIGENDIAN_ROT						BIT_6
#define AHB_BIGENDIAN_BM0						BIT_7
#define AHB_BIGENDIAN_BM1						BIT_8
#define AHB_BIGENDIAN_SHARM					BIT_9

#define AHB_ENDIAN_OPEN             0xC3D4
// Bit define AHB_CTRL1
#define AHB_CTRL1_EMC_AUTO_GATE_EN BIT_8
#define AHB_CTRL1_EMC_CH_AUTO_GATE_EN  BIT_9
#define AHB_CTRL1_ARM_AUTO_GATE_EN  BIT_11
#define AHB_CTRL1_AHB_AUTO_GATE_EN  BIT_12
#define AHB_CTRL1_MCU_AUTO_GATE_EN  BIT_13
#define AHB_CTRL1_MSTMTX_AUTO_GATE_EN  BIT_14
#define AHB_CTRL1_ARMMTX_AUTO_GATE_EN  BIT_15
#define AHB_CTRL1_ARM_DAHB_SLEEP_EN  BIT_16

///USB_PHY_CTRL
#define USB_DM_PULLUP_BIT       BIT_19
#define USB_DP_PULLDOWN_BIT     BIT_20
#define USB_DM_PULLDOWN_BIT     BIT_21


/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
#ifdef CHIP_ENDIAN_LITTLE
typedef union _ahb_ctl0_reg_tag
{
    struct _ahb_ctl0_reg_map
    {
        volatile unsigned int    reserved               :1; ////apb_eb            :1;
        volatile unsigned int    dcam_eb                :1; //BIT_1
        volatile unsigned int    ccir_eb                :1; //BIT_2
        volatile unsigned int    lcdc_eb                :1; //BIT_3
        volatile unsigned int    sdio_eb                :1; //BIT_4
        volatile unsigned int    usbd_eb                :1; //BIT_5
        volatile unsigned int    dma_eb                 :1; //BIT_6
        volatile unsigned int    bm0_eb                 :1; //BIT_7
        volatile unsigned int    nfc_eb                 :1; //BIT_8
        volatile unsigned int    reserved0              :2; //BIT_9-10
        volatile unsigned int    bm1_eb                 :1; //BIT_11
        volatile unsigned int    reserved1              :1; //BIT_12
        volatile unsigned int    vsp_eb                 :1; //BIT_13
        volatile unsigned int    rot_eb                 :1; //BIT_14
        volatile unsigned int    reserved2              :3; //BIT_15-17
        volatile unsigned int    drm_eb                 :1; //BIT_18
        volatile unsigned int    reserved3              :8; //BIT_19-26
        volatile unsigned int    ahb_arch_en            :1; //BIT_27
        volatile unsigned int    emc_en                 :1; //BIT_28
        volatile unsigned int    reserved4              :3; //BIT_29-31
    } mBits ;
    volatile unsigned int dwValue ;
} AHB_CTL0_U;

typedef union _ahb_ctl1_reg_tag
{
    struct _ahb_ctl1_reg_map
    {
        volatile unsigned int    dcam_buf_sw            :1; //BIT_0
        volatile unsigned int    dcam_buf_status        :1; //BIT_1
        volatile unsigned int    reserved0              :6; //BIT_2-7
        volatile unsigned int    emc_auto_gate_en       :1; //BIT_8
        volatile unsigned int    emc_ch_auto_gate_en    :1; //BIT_9
        volatile unsigned int    reserved1              :1; //BIT_10
        volatile unsigned int    arm_auto_gate_en       :1; //BIT_11
        volatile unsigned int    ahb_auto_gate_en       :1; //BIT_12
        volatile unsigned int    mcu_auto_gate_en       :1; //BIT_13
        volatile unsigned int    mstmtx_auto_gate_en    :1; //BIT_14
        volatile unsigned int    armmtx_auto_gate_en    :1; //BIT_15
        volatile unsigned int    arm_dahb_sleep_en      :1; //BIT_16
        volatile unsigned int    reserved2              :15; //BIT_17-31
    } mBits ;
    volatile unsigned int dwValue ;
} AHB_CTL1_U;

typedef union _ahb_ctl2_reg_tag
{
    struct _ahb_ctl2_reg_map
    {
        volatile unsigned int    Reserved0              :3; //BIT_0-2
        volatile unsigned int    mcu_shm_ctrl           :2; //BIT_3-4
        volatile unsigned int    reserved1              :27; //BIT_5-31
    } mBits ;
    volatile unsigned int dwValue ;
} AHB_CTL2_U;

typedef union _ahb_ctl3_reg_tag
{
    struct _ahb_ctl3_reg_map
    {
        volatile unsigned int    clk_usb_ref_sel        :1; //BIT_0
        volatile unsigned int    usb_s_hbigendian       :1; //BIT_1
        volatile unsigned int    usb_m_hbigendian       :1; //BIT_2
        volatile unsigned int    reserved0              :1; //BIT_3
        volatile unsigned int    busmon_sel0            :1; //BIT_4
        volatile unsigned int    busmon_sel1            :1; //BIT_5
        volatile unsigned int    clk_usb_ref_en         :1; //BIT_6
        volatile unsigned int    clk_utmifs_en          :1; //BIT_7
        volatile unsigned int    utmifs_tx_en_inv       :1; //BIT_8
        volatile unsigned int    utmi_suspend_inv       :1; //BIT_9
        volatile unsigned int    clk_ulpi_en            :1; //BIT_10
        volatile unsigned int    reserved1              :21; //BIT_11-31
    } mBits ;
    volatile unsigned int dwValue ;
} AHB_CTL3_U;


typedef union _ahb_soft_rst_reg_tag
{
    struct _ahb_soft_rst_reg_map
    {
        volatile unsigned int    dma_rst                :1; //BIT_0
        volatile unsigned int    dcam_rst               :1; //BIT_1
        volatile unsigned int    ccir_rst               :1;//BIT_2
        volatile unsigned int    lcdc_rst               :1;//BIT_3
        volatile unsigned int    reserved0              :1;//Reserved
        volatile unsigned int    nfc_rst                :1;//BIT_5
        volatile unsigned int    usbd_utmi_rst          :1;//BIT_6
        volatile unsigned int    usbphy_rst             :1;//BIT_7
        volatile unsigned int    reserved1              :2;//Reserved
        volatile unsigned int    rot_rst                :1;//BIT_10
        volatile unsigned int    emc_rst                :1;//BIT_11
        volatile unsigned int    sd_rst                 :1;//BIT_12
        volatile unsigned int    drm_rst                :1;//BIT_13
        volatile unsigned int    adc_rst                :1;//BIT_14
        volatile unsigned int    vsp_rst                :1;//BIT_15
        volatile unsigned int    reserved2              :16;//Reserved
    } mBits;
    volatile unsigned int dwValue;
} AHB_SOFT_RST_U;
#else
typedef union _ahb_ctl0_reg_tag
{
    struct _ahb_ctl0_reg_map
    {
        volatile unsigned int    reserved4              :3; //BIT_29-31
        volatile unsigned int    emc_en                 :1; //BIT_28
        volatile unsigned int    ahb_arch_en            :1; //BIT_27
        volatile unsigned int    reserved3              :8; //BIT_19-26
        volatile unsigned int    drm_eb                 :1; //BIT_18
        volatile unsigned int    reserved2              :3; //BIT_15-17
        volatile unsigned int    rot_eb                 :1; //BIT_14
        volatile unsigned int    vsp_eb                 :1; //BIT_13
        volatile unsigned int    reserved1              :1; //BIT_12
        volatile unsigned int    bm1_eb                 :1; //BIT_11
        volatile unsigned int    reserved0              :2; //BIT_9-10
        volatile unsigned int    nfc_eb                 :1; //BIT_8
        volatile unsigned int    bm0_eb                 :1; //BIT_7
        volatile unsigned int    dma_eb                 :1; //BIT_6
        volatile unsigned int    usbd_eb                :1; //BIT_5
        volatile unsigned int    sdio_eb                :1; //BIT_4
        volatile unsigned int    lcdc_eb                :1; //BIT_3
        volatile unsigned int    ccir_eb                :1; //BIT_2
        volatile unsigned int    dcam_eb                :1; //BIT_1
        volatile unsigned int    reserved               :1; ////apb_eb            :1;
    } mBits ;
    volatile unsigned int dwValue ;
} AHB_CTL0_U;

typedef union _ahb_ctl1_reg_tag
{
    struct _ahb_ctl1_reg_map
    {
        volatile unsigned int    reserved2              :15; //BIT_17-31
        volatile unsigned int    arm_dahb_sleep_en      :1; //BIT_16
        volatile unsigned int    armmtx_auto_gate_en    :1; //BIT_15
        volatile unsigned int    mstmtx_auto_gate_en    :1; //BIT_14
        volatile unsigned int    mcu_auto_gate_en       :1; //BIT_13
        volatile unsigned int    ahb_auto_gate_en       :1; //BIT_12
        volatile unsigned int    arm_auto_gate_en       :1; //BIT_11
        volatile unsigned int    reserved1              :1; //BIT_10
        volatile unsigned int    emc_ch_auto_gate_en    :1; //BIT_9
        volatile unsigned int    emc_auto_gate_en       :1; //BIT_8
        volatile unsigned int    reserved0              :6; //BIT_2-7
        volatile unsigned int    dcam_buf_status        :1; //BIT_1
        volatile unsigned int    dcam_buf_sw            :1; //BIT_0
    } mBits ;
    volatile unsigned int dwValue ;
} AHB_CTL1_U;

typedef union _ahb_ctl2_reg_tag
{
    struct _ahb_ctl2_reg_map
    {
        volatile unsigned int    reserved1              :27; //BIT_5-31
        volatile unsigned int    mcu_shm_ctrl           :2; //BIT_3-4
        volatile unsigned int    Reserved0              :3; //BIT_0-2
    } mBits ;
    volatile unsigned int dwValue ;
} AHB_CTL2_U;

typedef union _ahb_ctl3_reg_tag
{
    struct _ahb_ctl3_reg_map
    {
        volatile unsigned int    reserved1              :21; //BIT_11-31
        volatile unsigned int    clk_ulpi_en            :1; //BIT_10
        volatile unsigned int    utmi_suspend_inv       :1; //BIT_9
        volatile unsigned int    utmifs_tx_en_inv       :1; //BIT_8
        volatile unsigned int    clk_utmifs_en          :1; //BIT_7
        volatile unsigned int    clk_usb_ref_en         :1; //BIT_6
        volatile unsigned int    busmon_sel1            :1; //BIT_5
        volatile unsigned int    busmon_sel0            :1; //BIT_4
        volatile unsigned int    reserved0              :1; //BIT_3
        volatile unsigned int    usb_m_hbigendian       :1; //BIT_2
        volatile unsigned int    usb_s_hbigendian       :1; //BIT_1
        volatile unsigned int    clk_usb_ref_sel        :1; //BIT_0
    } mBits ;
    volatile unsigned int dwValue ;
} AHB_CTL3_U;


typedef union _ahb_soft_rst_reg_tag
{
    struct _ahb_soft_rst_reg_map
    {
        volatile unsigned int    reserved2              :16;//Reserved
        volatile unsigned int    vsp_rst                :1;//BIT_15
        volatile unsigned int    adc_rst                :1;//BIT_14
        volatile unsigned int    drm_rst                :1;//BIT_13
        volatile unsigned int    sd_rst                 :1;//BIT_12
        volatile unsigned int    emc_rst                :1;//BIT_11
        volatile unsigned int    rot_rst                :1;//BIT_10
        volatile unsigned int    reserved1              :2;//Reserved
        volatile unsigned int    usbphy_rst             :1;//BIT_7
        volatile unsigned int    usbd_utmi_rst          :1;//BIT_6
        volatile unsigned int    nfc_rst                :1;//BIT_5
        volatile unsigned int    reserved0              :1;//Reserved
        volatile unsigned int    lcdc_rst               :1;//BIT_3
        volatile unsigned int    ccir_rst               :1;//BIT_2
        volatile unsigned int    dcam_rst               :1; //BIT_1
        volatile unsigned int    dma_rst                :1; //BIT_0
    } mBits;
    volatile unsigned int dwValue;
} AHB_SOFT_RST_U;
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
#endif
// End
