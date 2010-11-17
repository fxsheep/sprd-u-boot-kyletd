/******************************************************************************
 ** File Name:      sp7100g_cfg.c                                             *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           08/23/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the config information of SP7100G       *
 **                 project.                                                  *     
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 08/23/2004      Xueliang.Wang   Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <config.h>
#include "asm/arch/sci_types.h"
#include "asm/arch/arm_reg.h"
#include "asm/arch/sc_reg.h"
#include "asm/arch/chip.h"
#include "asm/arch/emc_drv.h"
#include "asm/arch/dal_lcd.h"
#include <malloc.h>

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define SCI_ASSERT(_EXP)    assert(_EXP)
#define SCI_PASSERT(_EXP,PRINT_STR)     assert( _EXP );
//-----------------------------------------------------------------------------
//
//  The initialized frequency when system start up.
//

//-----------------------------------------------------------------------------
// MPLL = 78MHz,UPLL = 96MHZ, ARM = 78MHz, DSP = 96MHz when start up.
const uint32    g_pll_mn_value  = 0;
const uint32    g_upll_mn_value = 0;


//if use NOR+SRAM,you may set MCU_MAX_ECS0/ECS1_VALUE
//while will be used in system start up.
const uint32    g_clk_value     = 0;

//Global variable contain system clk config
SYS_CLK_CFG_INFO g_system_clk_cfg;

/*****************************************************************************/
//  Description:    This function get system clk config pointer
//  Author:         Younger.yang
//	Note:           
/*****************************************************************************/
SYS_CLK_CFG_INFO *Get_system_clk_cfg(void)
{
   
   g_system_clk_cfg.pll_mn       = g_pll_mn_value;
   g_system_clk_cfg.upll_mn      = g_upll_mn_value;
   g_system_clk_cfg.dsp_arm_div  = g_clk_value;
   
   return (SYS_CLK_CFG_INFO *)&g_system_clk_cfg;
}

/*****************************************************************************/
//  Description:    This function to  initialize customer config setting
//  Author:         liangwen.zhen
//	Note:           
/*****************************************************************************/
PUBLIC void PROD_CusCfgInit(void)
{
#ifndef FPGA_VERIFICATION
#if defined(CHIP_VER_8800G1)
    SCI_PASSERT(CHIP_GetChipType()== SC8800G1 ,("THIS SOFTWARTE ONLY FOR 8800G1"));
#elif defined(CHIP_VER_8800G2)
    SCI_PASSERT(CHIP_GetChipType()== SC8800G2 ,("THIS SOFTWARTE ONLY FOR 8800G2"));
#else
    #error please add macor in prj make file
#endif
#endif
}

/*****************************************************************************/
//  Description:    This function to  get address of the logo picture data
//  Author:         liangwen.zhen
//	Note:           
/*****************************************************************************/
PUBLIC uint8 * PROD_GetLogoAddr(LCD_ID_E id)
{
	return (uint8 *)0;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
