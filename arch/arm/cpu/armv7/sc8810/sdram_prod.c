/******************************************************************************
 ** File Name:      sdram_prod.c                                              *
 ** Author:                                                                   *
 ** DATE:           04/18/2008                                                *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the product configurable parameters of  * 
 **                 SDRAM module                                              *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/18/2008     	                Create.                         		  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include <asm/arch/sci_types.h>
#include <asm/arch/sdram_cfg.h>

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
 


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/*********************************************************************************/
//             Macro Definition
/*********************************************************************************/
// Feature
#if defined (BB_DRAM_TYPE_32MB_16BIT)
#define SDRAM_ROW_MODE		ROW_MODE_13
#define SDRAM_COL_MODE		COL_MODE_9
#define SDRAM_DATA_WIDTH	DATA_WIDTH_16
#define SDRAM_SIZE			SDRAM_32M

#elif defined(BB_DRAM_TYPE_32MB_32BIT_13R)
#define SDRAM_ROW_MODE		ROW_MODE_13		
#define SDRAM_COL_MODE		COL_MODE_8		
#define SDRAM_DATA_WIDTH	DATA_WIDTH_32
#define SDRAM_SIZE			SDRAM_32M

#elif defined(BB_DRAM_TYPE_32MB_32BIT_12R)
#define SDRAM_ROW_MODE		ROW_MODE_12		
#define SDRAM_COL_MODE		COL_MODE_9		
#define SDRAM_DATA_WIDTH	DATA_WIDTH_32
#define SDRAM_SIZE			SDRAM_32M

#elif defined(BB_DRAM_TYPE_64MB_16BIT)
#define SDRAM_ROW_MODE		ROW_MODE_13
#define SDRAM_COL_MODE		COL_MODE_10
#define SDRAM_DATA_WIDTH	DATA_WIDTH_16
#define SDRAM_SIZE			SDRAM_64M

#elif defined(BB_DRAM_TYPE_64MB_32BIT)
#define SDRAM_ROW_MODE		ROW_MODE_13
#define SDRAM_COL_MODE		COL_MODE_9
#define SDRAM_DATA_WIDTH	DATA_WIDTH_32
#define SDRAM_SIZE			SDRAM_64M

#elif defined(BB_DRAM_TYPE_128MB_16BIT)
#define SDRAM_ROW_MODE		ROW_MODE_14
#define SDRAM_COL_MODE		COL_MODE_10
#define SDRAM_DATA_WIDTH	DATA_WIDTH_16
#define SDRAM_SIZE			SDRAM_128M

#elif defined(BB_DRAM_TYPE_128MB_32BIT)
#define SDRAM_ROW_MODE		ROW_MODE_13
#define SDRAM_COL_MODE		COL_MODE_10
#define SDRAM_DATA_WIDTH	DATA_WIDTH_32
#define SDRAM_SIZE			SDRAM_128M

#elif defined(BB_DRAM_TYPE_256MB_32BIT)
#define SDRAM_ROW_MODE		ROW_MODE_14
#define SDRAM_COL_MODE		COL_MODE_10
#define SDRAM_DATA_WIDTH	DATA_WIDTH_32
#define SDRAM_SIZE			SDRAM_256M

#else
#error do not select size and bit width for SDRAM
#endif

#define BK_MODE 		BK_MODE_4

#if defined(PLATFORM_SC8800H)
#define BURST_LEN 		BURST_LEN_8
#elif defined(PLATFORM_SC6800H)
#define BURST_LEN 		BURST_LEN_2
#elif defined(PLATFORM_SC8800G)
#define BURST_LEN 		BURST_LEN_2
#else
#error do not select burst len for SDRAM
#endif

#define CAS_LATENCY 	CAS_LATENCY_3

// Include Product config
#include "sdram_cfg.c"

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
/*******************************************************************************/
//   sdram_parameters[] is used to config SDRAM controller according to variable
//sdram. The unit is ns. The end of array is marked with 0xffffffff at the last element. 
/*******************************************************************************/
LOCAL CONST SDRAM_TIMING_PARA_T s_sdram_timing_param =
//  ms    ns   ns    ns            ns    ns       ns    ns  clk   clk
//  tREF, tRP, tRCD, tWR/tRDL/tDPL,tRFC,tXSR,     tRAS,tRRD,tMRD, tWTR(wtr is only for ddr)
    {64,   30,  30,   20,          110, 150,      60,  20,  2,    2   };

/*******************************************************************************/
//   sdram_cofig_info is used to config sdram controller when to init sdram
/*******************************************************************************/
LOCAL CONST SDRAM_CFG_INFO_T s_sdram_config_info =
{
	BK_MODE,
	SDRAM_ROW_MODE,
	SDRAM_COL_MODE,
	SDRAM_DATA_WIDTH,              
	BURST_LEN,
	CAS_LATENCY,
	SDRAM_EXT_MODE_VALUE,
	SDR_SDRAM
};

/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function get sdram config pointer
//  Author:         nick.zhao
//  Note:           
/*****************************************************************************/
PUBLIC SDRAM_CFG_INFO_T_PTR SDRAM_GetCfg(void)
{
  return (SDRAM_CFG_INFO_T_PTR)&s_sdram_config_info;
}

/*****************************************************************************/
//  Description:    This function get sdram timing parameter pointer
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC SDRAM_TIMING_PARA_T_PTR SDRAM_GetTimingPara(void)
{
	return (SDRAM_TIMING_PARA_T_PTR)&s_sdram_timing_param;
}

/*****************************************************************************/
//  Description:    This function can be defined by user if needed. It will be called
//                  before the SDRAM initialization.
//  Author:         nick.zhao
//  Note:           
/*****************************************************************************/
PUBLIC void Userdef_before_sdram_init(void)
{
   return;
}
/*****************************************************************************/
//  Description:    This function can be defined by user if needed. It will be called
//                  after the SDRAM initialization.
//  Author:         nick.zhao
//  Note:           
/*****************************************************************************/
PUBLIC void Userdef_after_sdram_init(void)
{
   return;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif


