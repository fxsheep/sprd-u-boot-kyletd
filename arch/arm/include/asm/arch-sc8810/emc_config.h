#ifndef _EMC_CONFIG_H_
#define _EMC_CONFIG_H_


#include "sdram_sc7710g2.h"

/******************************************************************************
                           Macro define
******************************************************************************/
    

#define DLL_OFF FALSE
#define DLL_ON TRUE

#define FPGA_TEST	
#ifdef FPGA_TEST
#define EMC_CLK             CLK_48MHZ //CLK_333MHZ
#define EMC_T_REF_CLK       6500000 //t_ref reference clk, freqency = 6.5mhz
#define EMC_DLL_ON_OFF  	DLL_OFF
#else
#define EMC_CLK             CLK_333MHZ
#define EMC_T_REF_CLK       6500000 //t_ref reference clk, freqency = 6.5mhz
#define EMC_DLL_ON_OFF  	DLL_ON
#endif

//only one chip maro definition can equal to 1, others should equal to 0
//#define CHIP0_HYNIX_DDR_H8BCS0RJ0MCP      	
//#define CHIP1_TOSHIBA_SDR_TY9000A800JFGP40	
//#define CHIP2_ST_SDR_M65K                	
//#define CHIP3_SAMSUNG_SDR_K5D1G13DCA     	
//#define CHIP4_SAMSUNG_SDR_K5D5657DCBD090 	
//#define CHIP5_HYNIX_SDR_HYC0SEH0AF3P     	
//#define CHIP6_SAMSUNG_SDR_K5D1257ACFD090 	
//#define CHIP7_HYNIX_SDR_H8ACUOCEOBBR     	
//#define CHIP8_HYNIX_SDR_H8ACS0EJ0MCP     	
//#define CHIP9_HYNIX_SDR_H8AES0SQ0MCP     	
//#define CHIP10_HYNIX_SDR_HYC0SEH0AF3P    	
//#define CHIP11_MICRON_SDR_MT48H          	
#define CHIP12_HYNIX_DDR_H9DA4GH4JJAMCR4EM	
//#define CHIP13_HYNIX_SDR_H8ACS0PH0MCP    	
//#define CHIP14_HYNIX_DDR_H9DA4GH2GJAMCR  	
//#define CHIP15_SAMSUNG_DDR_K522H1HACF    	

#define SDRAM_AUTODETECT_SUPPORT

/*******************************************************************************
                          Parameter declare
*******************************************************************************/
   


extern CONST EMC_PHY_L1_TIMING_T EMC_PHY_TIMING_L1_INFO[EMC_PHYL1_TIMING_MATRIX_MAX];
extern CONST EMC_PHY_L2_TIMING_T EMC_PHY_TIMING_L2_INFO[EMC_PHYL2_TIMING_MATRIX_MAX];


/*******************************************************************************
                          Function declare
*******************************************************************************/

extern SDRAM_CFG_INFO_T_PTR SDRAM_GetCfg(void);
extern SDRAM_TIMING_PARA_T_PTR SDRAM_GetTimingPara(void);
extern SDRAM_CHIP_FEATURE_T_PTR SDRAM_GetFeature(void);
extern uint32 SDRAM_GetCap(SDRAM_CFG_INFO_T_PTR mem_info);
extern EMC_PHY_L1_TIMING_T_PTR EMC_GetPHYL1_Timing(DMEM_TYPE_E mem_type, uint32 cas_latency);
extern void EMC_PHY_Latency_Set(SDRAM_CFG_INFO_T_PTR mem_info);
extern void EMC_PHY_Timing_Set(SDRAM_CFG_INFO_T_PTR mem_info,
                        EMC_PHY_L1_TIMING_T_PTR emc_phy_l1_timing,
                        EMC_PHY_L2_TIMING_T_PTR emc_phy_l2_timing);
extern EMC_PHY_L2_TIMING_T_PTR EMC_GetPHYL2_Timing(void);

#endif

