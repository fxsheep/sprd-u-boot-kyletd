/******************************************************************************
 ** File Name:    sc8800h_reg_global.h                                        *
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
 ** 01/24/2007    Aiguo.Miao      Port to SC8800H                             *
 ******************************************************************************/
#ifndef _SC8800H_REG_GLOBAL_H_
    #define _SC8800H_REG_GLOBAL_H_
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
#define GREG_BASE               		0x8B000000



/*----------Global Registers----------*/
//GREG_BASE     0x8B000000
#define GR_ADCC                 		(GREG_BASE + 0x0000)
#define GR_ADCR                 		(GREG_BASE + 0x0004)
#define GR_GEN0                 		(GREG_BASE + 0x0008)
#define GR_PCTL                 		(GREG_BASE + 0x000C)
#define GR_IRQ                  		(GREG_BASE + 0x0010)
#define GR_ICLR                 		(GREG_BASE + 0x0014)
#define GR_GEN1                 		(GREG_BASE + 0x0018)
#define GR_GEN3			           		(GREG_BASE + 0x001C)
#define GR_HWRST			           		(GREG_BASE + 0x0020)
#define GR_HWRST1               		0x201007E0
#define GR_MPLL_MN               		(GREG_BASE + 0x0024)
#define GR_LDO_CTL0               		(GREG_BASE + 0x0028)
#define GR_GEN2                 		(GREG_BASE + 0x002C)
#define GR_ARM_BOOT_ADDR        		(GREG_BASE + 0x0030)
#define GR_STC_STSTE            		(GREG_BASE + 0x0034)
#define GR_LDO_CTL2             		(GREG_BASE + 0x0038)
#define GR_ANATST_CTL           		(GREG_BASE + 0x003C)
#define GR_SYS_ALM              		(GREG_BASE + 0x0040)
#define GR_BUSCLK_ALM           		(GREG_BASE + 0x0044)
#define GR_CLK_IIS1     				(GREG_BASE + 0x0048)
#define GR_SOFT_RST         		    (GREG_BASE + 0x004C)
#define GR_NFC_MEM_DLY					(GREG_BASE + 0x0058)
#define	GR_CLK_DLY						(GREG_BASE + 0x005C)
#define GR_GEN4                         (GREG_BASE + 0x0060)
#define GR_APLL_MN						(GREG_BASE + 0x0064)
#define GR_VPLL_MN						(GREG_BASE + 0x0068)
#define GR_LDO_CTL3						(GREG_BASE + 0x006C)
#define GR_PLL_SCR                      (GREG_BASE + 0x0070)
#define GR_CLK_EN                       (GREG_BASE + 0x0074)
#define GR_TDPLL_MN                     (GREG_BASE + 0x0078)
#define GR_CLK_GEN5                     (GREG_BASE + 0x007C)
#define GR_DCDC_CTL                     (GREG_BASE + 0x0080)
/*
  the GEN0 register bit  
*/
#define GEN0_WDG_EN		        		BIT_0
#define GEN0_PWMC	            		BIT_1
#define GEN0_TIMER_EN           		BIT_2
#define GEN0_SIM						BIT_3	//SIM module enable bit
#define GEN0_I2C                		BIT_4
#define GEN0_PWMB               		BIT_5
#define GEN0_PWMA               		BIT_6
#define GEN0_RTC                		BIT_7
#define GEN0_KPD                		BIT_8
#define GEN0_PWME	            		BIT_9
#define GEN0_MCU_DSP_RST        		BIT_10
#define GEN0_MCU_SOFT_RST       		BIT_11
#define GEN0_PLLPD_EN		      		BIT_12
#define GEN0_PWMD        				BIT_13
#define GEN0_CCIR_MCLK_EN	            BIT_14  //CCIR MCLK enable bit
#define GEN0_CCIR_MCLK_SEL     			BIT_15  //CCIR MCLK select bit 1:PLL
#define GEN0_EPT_EN                     BIT_15
#define GEN0_SIM1_EN                    BIT_16


/*
  the GEN1 register bit  
*/
#define GEN1_GEA_EN             		BIT_8
#define GEN1_MPLLMN_WN           		BIT_9
#define GEN1_CLK_AUX0_EN           		BIT_10
#define GEN1_CLK_AUX1_EN        		BIT_11
#define GEN1_TEST_MODEP_MCU     		BIT_12
#define GEN1_SYSCLK_EN          		BIT_13
#define GEN1_SERCLK_EB3            		BIT_14
#define GEN1_CLK_26MHZ_EN       		BIT_15
#define GEN1_TDPLL_MN_WE                BIT_19
#define GEN1_VPLLMN_WE                  BIT_20
#define GEN1_APLLMN_WE                  BIT_21
#define GEN1_SERCLK_EB0					BIT_22
#define GEN1_SERCLK_EB1					BIT_23
#define GEN1_SERCLK_EB2					BIT_24
#define GEN1_ARM_BOOT_MD0				BIT_25
#define GEN1_ARM_BOOT_MD1				BIT_26
#define GEN1_ARM_BOOT_MD2				BIT_27
#define GEN1_ARM_BOOT_MD3				BIT_28
#define GEN1_ARM_BOOT_MD4				BIT_29
#define GEN1_ARM_BOOT_MD5				BIT_30


/*
  the PCMCIA ctl register bit
*/
#define PCM_EN							BIT_4
#if defined(CHIP_VER_8800H5)
/*
  the AHB_MISC register bit
*/
#define ARM_VB_MCLKON                   BIT_3 
#define ARM_VB_DA0ON					BIT_4
#define ARM_VB_DA1ON					BIT_5
#define ARM_VB_ADCON					BIT_6
#define ARM_VB_ANAON					BIT_7
#define ARM_VB_ACC						BIT_8
#else
/*
  the BUSCLK ALM register bit
*/
#define ARM_VB_DA0ON                    BIT_2
#define ARM_VB_DA1ON					BIT_3
#define ARM_VB_AD0ON					BIT_4
#define ARM_VB_AD1ON					BIT_5
#define ARM_VB_ANAON					BIT_6
#define ARM_VB_ACC						BIT_7
//#define CLK_MCU_INV						BIT_14
//#define MCU_MN_EN						BIT_15
#endif
#define ARM_VPLL_FORCE_PD               BIT_14
#define ARM_APLL_FORCE_PD               BIT_15

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef union _gr_anatst_ctl_tag {
	struct _gr_anatst_ctl_map {
        volatile unsigned int	Reserved1           :2;
        volatile unsigned int   Ldo_ldo2_b1_rst     :1;
        volatile unsigned int   Ldo_ldo2_b1         :1;
        volatile unsigned int	Usb_500ma_en_rst	:1; //BIT_27
        volatile unsigned int	Usb_500ma_en		:1;
        volatile unsigned int	Adapter_en_rst      :1;
        volatile unsigned int	Adapter_en		    :1; //BIT_24
        volatile unsigned int	Ldo_ldo2_b0_rst     :1;
        volatile unsigned int	Ldo_ldo2_b0         :1;
        volatile unsigned int	Ldo_ldo3_b1_rst     :1;
        volatile unsigned int	Ldo_ldo3_b1         :1;
        volatile unsigned int	Ldo_ldo3_b0_rst     :1;
        volatile unsigned int	Ldo_ldo3_b0         :1;
        volatile unsigned int	Reserved            :1;
        volatile unsigned int	Slp_usb_en          :1;
        volatile unsigned int	standby             :1;
        volatile unsigned int	recharge            :1;
       volatile unsigned int	Ldo_bpusb        :2;   //BIT_13,  BIT_12
        volatile unsigned int	Ldo_bprf2_rst       :1;
        volatile unsigned int	Ldo_bprf2           :1;
        volatile unsigned int	Ldo_bpnf_rst        :1;
        volatile unsigned int	Ldo_bpnf            :1;
        volatile unsigned int	start_en6u_rst      :1;
        volatile unsigned int	start_en6u          :1;
        volatile unsigned int	start_en3u_rst      :1;
        volatile unsigned int	start_en3u          :1;
        volatile unsigned int	start_en2u_rst      :1;
        volatile unsigned int	start_en2u          :1;
        volatile unsigned int	start_en1u_rst      :1;
        volatile unsigned int	start_en1u          :1;
	}mBits ;
	volatile unsigned int dwValue ;
}GR_ANATST_CTL_U;


typedef union _gr_nfc_mem_dly_tag {
	struct _gr_nfc_mem_dly_map {
		volatile unsigned int reserved				:14;//Reserved
		volatile unsigned int nfc_ren_dly_sel		:3;
		volatile unsigned int nfc_wen_dly_sel		:3;
		volatile unsigned int nfc_ale_dly_sel		:3;
		volatile unsigned int nfc_cle_dly_sel		:3;
		volatile unsigned int nefc_cen_dly_sel		:3;
	}mBits;
	volatile unsigned int dwValue;
}GR_NFC_MEM_DLY_U;

typedef union _gr_gen1_reg_tag {
	struct _gr_gen1_reg_map {
		volatile unsigned int reserved_1			:1;//Reserved
		volatile unsigned int arm_boot_md5			:1;
		volatile unsigned int arm_boot_md4			:1;
		volatile unsigned int arm_boot_md3			:1;
		volatile unsigned int arm_boot_md2			:1;
		volatile unsigned int arm_boot_md1			:1;
		volatile unsigned int arm_boot_md0			:1;
		volatile unsigned int serclk_eb2			:1;
		volatile unsigned int serclk_eb1			:1;
		volatile unsigned int serclk_eb0			:1;
		volatile unsigned int a_pllmn_we			:1;
		volatile unsigned int v_pllmn_we			:1;
		volatile unsigned int td_pllmn_we			:1;
		volatile unsigned int reserved1			    :3;
		volatile unsigned int clk_26mhz_en			:1;
		volatile unsigned int serclk_eb3			:1;
		volatile unsigned int syst_en3				:1;
		volatile unsigned int testmodep_mcu2		:1;
		volatile unsigned int clk_aux1_en			:1;
		volatile unsigned int clk_aux0_en			:1;
		volatile unsigned int m_pllmn_we			:1;
		volatile unsigned int gea_eb2				:1;
		volatile unsigned int clk_aux0_div			:8;
	}mBits;
	volatile unsigned int dwValue;
}GR_GEN1_REG_U;

typedef union _gr_glb_gen4_reg_tag {
	struct _gr_glb_gen4_reg_map {
	    volatile unsigned int clk_ecc_div			:4;
		volatile unsigned int clk_dll_div			:4;
		volatile unsigned int clk_tdfir_div	        :8;
		volatile unsigned int clk_che_div			:8;
		volatile unsigned int clk_lcdc_div			:8;
	}mBits;
	volatile unsigned int dwValue;
}GR_GLB_GEN4_REG_U;

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

