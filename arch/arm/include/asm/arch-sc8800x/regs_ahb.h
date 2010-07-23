#ifndef _SC8800H_REG_AHB_H_
    #define _SC8800H_REG_AHB_H_

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define AHB_REG_BASE                0x20900200

#define AHB_CTL0                	(AHB_REG_BASE + 0x00)
#define AHB_CTL1                   	(AHB_REG_BASE + 0x04)
#define AHB_CTL2                   	(AHB_REG_BASE + 0x08)
#define AHB_RESERVED            	(AHB_REG_BASE + 0x0c)
#define AHB_SOFT_RST               	(AHB_REG_BASE + 0x10)
#define AHB_STOP_CTL               	(AHB_REG_BASE + 0x14)
#define AHB_REMAP               	(AHB_REG_BASE + 0x18)
#define AHB_INT_STS               	(AHB_REG_BASE + 0x1c)
#define AHB_INT_CLR               	(AHB_REG_BASE + 0x20)
#define AHB_AHB_ARM_CLK            	(AHB_REG_BASE + 0x24)
#define AHB_BOND_OPT               	(AHB_REG_BASE + 0x28)
#define AHB_TD_CLK                  (AHB_REG_BASE + 0x2c)
#define AHB_RFT_CLK                 (AHB_REG_BASE + 0x30)
#define AHB_DSP_WAKEUP              (AHB_REG_BASE + 0x80)
#define AHB_DSP_BOOT_EN             (AHB_REG_BASE + 0x84)
#define AHB_DSP_BOOT_VECTOR         (AHB_REG_BASE + 0x88)
#define AHB_DSP_RESET               (AHB_REG_BASE + 0x8C)
#define AHB_ARM_POWERDOWN_EN        (AHB_REG_BASE + 0x40)

#define AHB_DMA_SOFT_RST		BIT_0

#define CHIP_TYPE                   0x209003FC

/* AHB_AHB_ARM_CLK related macro definition */
#define AHB_CLK_DIV_MAX		0x1F
#define ARM_CLK_DIV_MAX		0x1F
#define EMC_CLK_DIV_MAX		0x1F
#define XAHB_CLK_DIV_MAX	0x7
#define AHB_CLK_DIV_SHIFT	5
#define ARM_CLK_DIV_SHIFT	0
#define EMC_CLK_DIV_SHIFT	10
#define XAHB_CLK_DIV_SHIFT	15


typedef union _ahb_ctl0_reg_tag {
	struct _ahb_ctl0_reg_map {
        volatile unsigned int	clk_apb_div         :5; //BIT_31-27
        volatile unsigned int	reserved1           :7; //BIT_26-20
        volatile unsigned int   usb_sdio_priority   :1; //BIT_19
        volatile unsigned int	drm_eb              :1; //BIT_18
        volatile unsigned int	em_no_tr            :1; //BIT_17
        volatile unsigned int	ahb_sleep_bypass    :1; //BIT_16
        volatile unsigned int	reserved2           :1; //BIT_15
        volatile unsigned int	rot_eb              :1; //BIT_14
        volatile unsigned int	mea_eb              :1; //BIT_13
        volatile unsigned int	dct_eb              :1; //BIT_12
        volatile unsigned int	mon1_eb             :1; //BIT_11
        volatile unsigned int	tic_eb              :1; //BIT_10
        volatile unsigned int	lcm_eb              :1; //BIT_9
        volatile unsigned int	nfc_eb              :1; //BIT_8
        volatile unsigned int	mon0_eb             :1; //BIT_7
        volatile unsigned int	dma_eb              :1; //BIT_6
        volatile unsigned int	usbd_eb             :1; //BIT_5
        volatile unsigned int   tve_eb              :1; //BIT_4
        volatile unsigned int   lcdc_eb             :1; //BIT_3
        volatile unsigned int   ccir_eb             :1; //BIT_2
        volatile unsigned int	isp_eb              :1; //BIT_1
        volatile unsigned int	apb_stop            :1; //BIT_0
	}mBits ;
	volatile unsigned int dwValue ;
}AHB_CTL0_U;

typedef union _ahb_soft_rst_reg_tag {
	struct _ahb_soft_rst_reg_map {
		volatile unsigned int reserved1 			:16;//reserved
    	volatile unsigned int mea_soft_rst			:1; //BIT_15
		volatile unsigned int dct_soft_rst			:1; //BIT_14
		volatile unsigned int drm_soft_rst			:1; //BIT_13
		volatile unsigned int sd_soft_rst			:1; //BIT_12
		volatile unsigned int emc_soft_rst			:1; //BIT_11
		volatile unsigned int rot_soft_rst			:1; //BIT_10
		volatile unsigned int reserved	            :2; //BIT_8/9
		volatile unsigned int usbd48m_soft_rst		:1; //BIT_7
		volatile unsigned int usbd12m_soft_rst		:1; //BIT_6
		volatile unsigned int nfc_soft_rst			:1; //BIT_5
		volatile unsigned int tve_soft_rst			:1; //BIT_4
		volatile unsigned int lcdc_soft_rst			:1; //BIT_3
		volatile unsigned int ccir_soft_rst			:1; //BIT_2
		volatile unsigned int isp_soft_rst			:1; //BIT_1
		volatile unsigned int dma_soft_rst			:1; //BIT_0
	}mBits;
	volatile unsigned int dwValue;
}AHB_SOFT_RST_U;

#ifdef   __cplusplus
    }
#endif
#endif

