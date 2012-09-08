/******************************************************************************
 ** File Name:      sc8810_freq_cfg.h                                                 *
 ** Author:         Yuhua.Shi                                                   *
 ** DATE:           08/17/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the config of frequency.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/17/2010     Yuhua.Shi     Create.                                   *
 ******************************************************************************/

#ifndef _SC8810_FREQ_CFG_H_
#define _SC8810_FREQ_CFG_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "sci_types.h"
#include "chng_freq.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Macro Define                                **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                 *
 **---------------------------------------------------------------------------*/
//define the arm/ahb/emc clk
typedef struct freq_clk_LVL_tag
{
    uint32 arm_clk;
    uint32 ahb_clk;
    uint32 emc_clk;
} FREQ_CLK_LVL_T, *FREQ_CLK_LVL_PTR;

//define the control infomation for change frequency
typedef struct freq_control_tag
{
    uint32             min_clk_lvl;
    uint32             max_clk_lvl;
    BOOLEAN        is_chng_freq_enable;
	BOOLEAN        is_ahbmaster_event_enable; 
} FREQ_CONTROL_T, *FREQ_CONTROL_PTR;

//define the frequency levle of application
typedef struct freq_app_clk_LVL_tag
{
    CHNG_FREQ_INDEX_E freq_index;
    uint32 clk_lvl;
} FREQ_APP_CLK_LVL_T, *FREQ_APP_CLK_LVL_PTR;

//define the config info for change frequency
typedef struct chng_freq_config_tag
{
    FREQ_CLK_LVL_PTR                clk_lvl_table_ptr;
    uint32                                  *clk_div_table_ptr;
    FREQ_APP_CLK_LVL_PTR        app_clk_lvl_table_ptr;
    FREQ_CONTROL_PTR              freq_control_info_ptr;
} CHNG_FREQ_CONFIG_T, *CHNG_FREQ_CONFIG_PTR;


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function is used to get the config info about change frequency
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC CHNG_FREQ_CONFIG_PTR FREQ_GetFreqCfg (
    void
);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
