/******************************************************************************
 ** File Name:    int_reg_v3.h                                           *
 ** Author:       Yuhua.Shi                                                   *
 ** DATE:         6/14/2008                                                   *
 ** Copyright:    2008 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/14/2008    Yuhua.Shi       Create.                                     *
 ** 05/05/2010    Mingwei.Zhang   Modified for SC8800G                        *
 ******************************************************************************/
#ifndef _INT_REG_V3_H_
#define _INT_REG_V3_H_
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
/*----------Interrupt Control Registers----------*/

#define INT_IRQ_BASE                    (INT_REG_BASE + 0x0000)
#define INT_IRQ_STS                     (INT_REG_BASE + 0x0000) //Interrupt status after masked by irq_enable.
#define INT_IRQ_RAW_STS                 (INT_REG_BASE + 0x0004) //Interrupt status from different interrupt source.
#define INT_IRQ_EN                      (INT_REG_BASE + 0x0008) //Enable bits  for the corresponding interrupt sources. 
#define INT_IRQ_DISABLE                 (INT_REG_BASE + 0x000C)
#define INT_IRQ_SOFT                    (INT_REG_BASE + 0x0010)
#define INT_IRQ_TEST_SRC                (INT_REG_BASE + 0x0014)
#define INT_IRQ_TEST_SEL                (INT_REG_BASE + 0x0018)
#define INT_IRQ_UINT_STS                (INT_REG_BASE + 0x001C)
#define INT_FIQ_STS                     (INT_REG_BASE + 0x0020)
#define INT_FIQ_RAW_STS                 (INT_REG_BASE + 0x0024)
#define INT_FIQ_EN                      (INT_REG_BASE + 0x0028)
#define INT_FIQ_DISABLE                 (INT_REG_BASE + 0x002C)
#define INT_FIQ_SOFT                    (INT_REG_BASE + 0x0030)
#define INT_FIQ_TEST_SRC                (INT_REG_BASE + 0x0034)
#define INT_FIQ_TEST_SEL                (INT_REG_BASE + 0x0038)
#define INT_UINT_CTL                    (INT_REG_BASE + 0x003C)


#define INTCTL_SPECIAL_LATCH_IRQ           BIT_0
#define INTCTL_SOFT_IRQ                     BIT_1
#define INTCTL_UART0_IRQ                    BIT_2
#define INTCTL_UART1_IRQ                    BIT_3
#define INTCTL_UART2_IRQ                    BIT_4
#define INTCTL_TIMER0_IRQ                   BIT_5
#define INTCTL_TIMER1_IRQ                   BIT_6
#define INTCTL_TIMER2_IRQ                   BIT_7
#define INTCTL_COMMTX                       BIT_7
#define INTCTL_GPIO_IRQ                     BIT_8
#define INTCTL_SPI_IRQ                          BIT_9
#define INTCTL_KPD_IRQ                      BIT_10
#define INTCTL_I2C_IRQ                          BIT_11
#define INTCTL_SIM0_IRQ                     BIT_12
#define INTCTL_SIM1_IRQ                     BIT_12
#define INTCTL_PIU_SER_INT_IRQ              BIT_13
#define INTCTL_PIU_CR_HINT_IRQ              BIT_14
#define INTCTL_DSP_IRQ0                     BIT_15
#define INTCTL_DSP_IRQ1                     BIT_16
#define INTCTL_SYST_IRQ                     BIT_17
#define INTCTL_EPT_IRQ                      BIT_18
#define INTCTL_IIS_IRQ                      BIT_19
#define INTCTL_DSP_INT_OR_IRQ               BIT_20
#define INTCTL_DMA_IRQ                      BIT_21
#define INTCTL_VBC_IRQ                      BIT_22
#define INTCTL_VSP_IRQ                      BIT_23
#define INTCTL_ANA_DIE_IRQ                  BIT_24
#define INTCTL_ADI_IRQ                      BIT_25
#define INTCTL_USB_IRQ                      BIT_26
#define INTCTL_DCAM_IRQ                 BIT_27
#define INTCTL_NFC_IRQ                      BIT_28
#define INTCTL_LCDC_IRQ                 BIT_29
#define INTCTL_DRM_IRQ                      BIT_30
#define INTCTL_SDIO_IRQ                 BIT_30
#define INTCTL_BUS_MON_IRQ              BIT_31
#define INTCTL_BUS_MON0_IRQ             BIT_31
#define INTCTL_BUS_MON1_IRQ             BIT_31
#define INTCTL_COMMRX_IRQ                   BIT_31


#define INTCTL_PCM_IRQ                  INTCTL_IIS_IRQ
#define INTCTL_ICLR_ALL                 0xFFFFFFFF


/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

#ifdef CHIP_ENDIAN_LITTLE
typedef union _INT_BIT_U
{
    struct INT_BIT_MAP
    {
        volatile int    special_latch       :1; //bit 0
        volatile int    soft                :1; //BIT_1
        volatile int    uart0               :1; //BIT_2
        volatile int    uart1               :1; //BIT_3
        volatile int    uart2               :1; //BIT_4
        volatile int    timer0              :1; //BIT_5
        volatile int    timer1              :1; //BIT_6
        volatile int    timer2_commtx       :1; //BIT_7
        volatile int    gpio                :1; //BIT_8
        volatile int    spi                 :1; //BIT_9
        volatile int    kpd                 :1; //BIT_10
        volatile int    i2c                 :1; //BIT_11
        volatile int    sim                 :1; //BIT_12
        volatile int    piu_ser_int         :1; //BIT_13
        volatile int    piu_cr_hint         :1; //BIT_14
        volatile int    dsp0                :1;  //BIT_15
        volatile int    dsp1                :1; //BIT_16
        volatile int    syst                :1; //BIT_17
        volatile int    ept                 :1; //BIT_18
        volatile int    iis                 :1; //BIT_19
        volatile int    dsp_int_or          :1; //BIT_20
        volatile int    dma                 :1; //BIT_21
        volatile int    vbc                 :1; //BIT_22
        volatile int    vsp                 :1; //BIT_23
        volatile int    ana_die             :1; //BIT_24
        volatile int    adi                 :1; //BIT_25
        volatile int    usb                 :1; //BIT_26
        volatile int    dcam                :1; //BIT_27
        volatile int    nfc                 :1; //BIT_28
        volatile int    lcdc                :1; //BIT_29
        volatile int    sdio_drm            :1; //BIT_30
        volatile int    mon_commrx          :1; //BIT_31
    } mBits;
    volatile int dwValue ;
} INT_BIT_U;



typedef union _INT_UART_RAW_U
{
    struct INT_UART_RAW_MAP
    {
        volatile int    rxd0            :1; //bit 0
        volatile int    ctsn0           :1; //bit 1
        volatile int    rxd1            :1; //bit 2
        volatile int    usbd_se0_wakeup :1; //bit 3
        volatile int    reserved        :28;
    } mBits;
    volatile int dwValue ;
} INT_UART_RAW_U;


typedef union _INT_UART_CFG_U
{
    struct INT_UART_CFG_MAP
    {
        volatile int    rxd0_en         :1; //bit 0
        volatile int    ctsn0_en        :1; //bit 1
        volatile int    rxd1_en         :1; //bit 2
        volatile int    usbd_se0_en     :1; //bit 3
        volatile int    reserved        :11; //bit 4-14
        volatile int    clr             :1; //bit 15
        volatile int    rxd0_pol        :1; //bit 16
        volatile int    ctsn0_pol       :1; //bit 17
        volatile int    rxd1_pol        :1; //bit 18
        volatile int    usbd_se0_pol    :1; //bit 19
        volatile int    reserved_1      :12; //bit 20-31
    } mBits;
    volatile int dwValue ;
} INT_UART_CFG_U;
#else
typedef union _INT_BIT_U
{
    struct INT_BIT_MAP
    {
        volatile int    mon_commrx          :1; //BIT_31
        volatile int    sdio_drm            :1; //BIT_30
        volatile int    lcdc                :1; //BIT_29
        volatile int    nfc                 :1; //BIT_28
        volatile int    dcam                :1; //BIT_27
        volatile int    usb                 :1; //BIT_26
        volatile int    adi                 :1; //BIT_25
        volatile int    ana_die             :1; //BIT_24
        volatile int    vsp                 :1; //BIT_23
        volatile int    vbc                 :1; //BIT_22
        volatile int    dma                 :1; //BIT_21
        volatile int    dsp_int_or          :1; //BIT_20
        volatile int    iis                 :1; //BIT_19
        volatile int    ept                 :1; //BIT_18
        volatile int    syst                :1; //BIT_17
        volatile int    dsp1                :1; //BIT_16
        volatile int    dsp0                :1;  //BIT_15
        volatile int    piu_cr_hint         :1; //BIT_14
        volatile int    piu_ser_int         :1; //BIT_13
        volatile int    sim                 :1; //BIT_12
        volatile int    i2c                 :1; //BIT_11
        volatile int    kpd                 :1; //BIT_10
        volatile int    spi                 :1; //BIT_9
        volatile int    gpio                :1; //BIT_8
        volatile int    timer2_commtx       :1; //BIT_7
        volatile int    timer1              :1; //BIT_6
        volatile int    timer0              :1; //BIT_5
        volatile int    uart2               :1; //BIT_4
        volatile int    uart1               :1; //BIT_3
        volatile int    uart0               :1; //BIT_2
        volatile int    soft                :1; //BIT_1
        volatile int    special_latch       :1; //bit 0
    } mBits;
    volatile int dwValue ;
} INT_BIT_U;



typedef union _INT_UART_RAW_U
{
    struct INT_UART_RAW_MAP
    {
        volatile int    reserved        :28;
        volatile int    usbd_se0_wakeup :1; //bit 3
        volatile int    rxd1            :1; //bit 2
        volatile int    ctsn0           :1; //bit 1
        volatile int    rxd0            :1; //bit 0
    } mBits;
    volatile int dwValue ;
} INT_UART_RAW_U;


typedef union _INT_UART_CFG_U
{
    struct INT_UART_CFG_MAP
    {
        volatile int    reserved_1      :12; //bit 20-31
        volatile int    usbd_se0_pol    :1; //bit 19
        volatile int    rxd1_pol        :1; //bit 18
        volatile int    ctsn0_pol       :1; //bit 17
        volatile int    rxd0_pol        :1; //bit 16
        volatile int    clr             :1; //bit 15
        volatile int    reserved        :11; //bit 4-14
        volatile int    usbd_se0_en     :1; //bit 3
        volatile int    rxd1_en         :1; //bit 2
        volatile int    ctsn0_en        :1; //bit 1
        volatile int    rxd0_en         :1; //bit 0
    } mBits;
    volatile int dwValue ;
} INT_UART_CFG_U;
#endif


//IRQ Interrupt Control.
typedef struct INT_IRQ_S_
{
    volatile INT_BIT_U      sts;
    volatile INT_BIT_U      raw;
    volatile INT_BIT_U      en;
    volatile INT_BIT_U      clr;
    volatile INT_BIT_U      soft;
    volatile INT_BIT_U      t_src;
    volatile INT_BIT_U      t_sel;
    volatile INT_UART_RAW_U uart_raw;
    volatile INT_BIT_U      f_sts;
    volatile INT_BIT_U      f_raw;
    volatile INT_BIT_U      f_en;
    volatile INT_BIT_U      f_dis;
    volatile INT_BIT_U      f_soft;
    volatile INT_BIT_U      ft_src;
    volatile INT_BIT_U      ft_sel;
    volatile INT_UART_CFG_U uart_cfg;
} INT_CTRL_T;

//IRQ Interrupt Control.
typedef struct int_irq_tag
{
    volatile int sts;
    volatile int raw_sts;
    volatile int en;
    volatile int clr;
    volatile int soft;
} int_irq_s;

typedef struct firq_tag
{
    volatile int sts;
    volatile int raw_sts;
    volatile int en;
    volatile int clr;
} firq_s;


/*----------Analog Die Interupt Status Registers----------*/

///#define ANA_INTC_BASE                0x82000380
#define ANA_INT_STATUS             (ANA_INTC_BASE + 0x00)
#define ANA_INT_RAW                (ANA_INTC_BASE + 0x04)
#define ANA_INT_EN                 (ANA_INTC_BASE + 0x08)

#define ANA_INT_STATUS_SYNC        (ANA_INTC_BASE + 0x0C)

#define ANA_MAX_INT_NUM         5
//#define ANA_INT_MSK             0x1F

#define ANA_TPC_IRQ         BIT_4
#define ANA_WDG_IRQ         BIT_3
#define ANA_RTC_IRQ         BIT_2
#define ANA_GPIO_IRQ        BIT_1
#define ANA_ADC_IRQ         BIT_0


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
#endif //_INT_REG_V3_H_
// End

