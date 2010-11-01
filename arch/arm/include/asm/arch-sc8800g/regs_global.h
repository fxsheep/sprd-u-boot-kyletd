#ifndef _SC8800H_REG_GLOBAL_H_
    #define _SC8800H_REG_GLOBAL_H_

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

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
 *   the GEN0 register bit  
 *   */
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
 *   the GEN1 register bit  
 *   */
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


/*MPLL related macro definition */
#define PLLMN_N_MAX	0xFFF
#define PLLMN_M_MAX	0xFFF
#define PLLMN_N_SHIFT	16
#define PLLMN_M_SHIFT   0
/*
 *   the PCMCIA ctl register bit
 *   */
#define PCM_EN							BIT_4

/*
 *   the BUSCLK ALM register bit
 *   */
#define ARM_VB_DA0ON                    BIT_2
#define ARM_VB_DA1ON					BIT_3
#define ARM_VB_AD0ON					BIT_4
#define ARM_VB_AD1ON					BIT_5
#define ARM_VB_ANAON					BIT_6
#define ARM_VB_ACC						BIT_7
#define ARM_VPLL_FORCE_PD               BIT_14
#define ARM_APLL_FORCE_PD               BIT_15


//UART0, UART1, UART2, UART3 registers
#define ARM_UART0_BASE        0x83000000 
#define ARM_UART1_BASE        0x84000000
#define ARM_UART2_BASE        0x8E000000
#define ARM_UART3_BASE        0x8F000000

//NAND flash controller 
#define ARM_NAND_BASE	      0x60000000

#define REG32(x) (*(volatile uint32_t *)(x))


#ifdef   __cplusplus
    }
#endif
#endif
// End
