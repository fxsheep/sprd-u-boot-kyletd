/******************************************************************************
 ** File Name:      sdram_drvapi.h                                            *
 ** Author:         Nick.zhao                                                 *
 ** DATE:           01/03/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
#ifndef _SDRAM_DRVAPI_H_
#define _SDRAM_DRVAPI_H_
#ifdef   __cplusplus
extern   "C"
{
#endif
#include "sci_types.h"
/******************************************************************************
                          Macro define
******************************************************************************/
/******************************************************************************
                          Struct define
******************************************************************************/
typedef struct
{
    uint32 row_ref_max;     //ROW_REFRESH_TIME,Refresh interval time , ns, tREF-max = 7800 ns
    uint32 row_pre_min;     //ROW_PRECHARGE_TIME , ns, tRP-min = 27 ns.
    uint32 rcd_min;         //T_RCD,ACTIVE to READ or WRITE delay  , ns, tRCD-min = 27 ns
    uint32 wr_min;          //T_WR  ,WRITE recovery time  , ns, tWR-min = 15 ns.
    uint32 rfc_min;         //T_RFC, AUTO REFRESH command period , ns, tRFC-min = 80 ns.
    uint32 xsr_min;         //T_XSR  , ns, tXSR-min = 120 ns.
    uint32 ras_min;         //T_RAS_MIN , row active time, ns, tRAS-min = 50ns
    uint32 rrd_min;         //T_RRD
    uint32 mrd_min;         //T_MRD , 2 cycles, tMRD-min = 2 cycles.
    uint32 wtr_min;         //T_WTR , 1 cycle
} SDRAM_TIMING_PARA_T, *SDRAM_TIMING_PARA_T_PTR;

/* The SDRAM size parameters supports */
#define     SDRAM_8M        0
#define     SDRAM_16M       1
#define     SDRAM_32M       2
#define     SDRAM_64M       3
#define     SDRAM_128M      4
#define     SDRAM_256M      5


#define     BK_MODE_1               0           //1 bank sdram
#define     BK_MODE_2               1           //2 bank sdram          
#define     BK_MODE_4               2           //4 bank sdram
#define     BK_MODE_8               3           //8 bank sdram

#define     ROW_MODE_11             0           // 11 row sdram
#define     ROW_MODE_12             1           // 12 row sdram
#define     ROW_MODE_13             2           // 13 row sdram
#define     ROW_MODE_14             3           // 14 row sdram

#define     COL_MODE_8              0           //8 column sdram
#define     COL_MODE_9              1           //9 column sdram
#define     COL_MODE_10             2           //10 column sdram
#define     COL_MODE_11             3           //11 column sdram
#define     COL_MODE_12             4           //12 column sdram

#define     DATA_WIDTH_16           0           //16 bit sdram
#define     DATA_WIDTH_32           1           //32 bit sdram

#define     BURST_LEN_1             0           // 1 words burst
#define     BURST_LEN_2             1           // 2 words burst
#define     BURST_LEN_4             2           // 4 words burst
#define     BURST_LEN_8             3           // 8 words burst
#define     BURST_LEN_16            4           // 16 words burst
#define     BURST_LEN_FULLPAGE      7           // full page burst

#define     CAS_LATENCY_1           1           // 1 cycle cas latency
#define     CAS_LATENCY_2           2           // 2 cycle cas latency
#define     CAS_LATENCY_3           3           // 3 cycle cas latency

/* Extend mode register value supports*/
#define     SDRAM_EXT_MODE_INVALID     0xffffffff
#define     EXT_MODE_PASR_ALL            0
#define     EXT_MODE_DS_HALF             1
#define     EXT_MODE_DS_FULL             0
#define     EXT_MODE_DS_QUARTER          2
#define     EXT_MODE_FLAG                1
#define     SDRAM_EXT_MODE_REG           ((EXT_MODE_FLAG<<15) | (EXT_MODE_DS_FULL<<5) | EXT_MODE_PASR_ALL)

#define     SDR_SDRAM               0
#define     DDR_SDRAM               1

/* The sdram configuration struct */
typedef struct _SDRAM_CFG_INFO
{
    uint32  bank_mode;    //Can only be set as BK_MODE_1,BK_MODE_2,BK_MODE_4,BK_MODE_8
    uint32  row_mode;     //Can only be set as ROW_MODE_11,ROW_MODE_12,ROW_MODE_13
    uint32  col_mode;     //Can only be set as COL_MODE_8,COL_MODE_9,COL_MODE_10,COL_MODE_11,COL_MODE_12
    uint32  data_width;   //Can only be set as DATA_WIDTH_16,DATA_WIDTH_32
    uint32  burst_length; //Can only be set as BURST_LEN_2,BURST_LEN_4,BURST_LEN_8,BURST_LEN_16,BURST_LEN_FULLPAGE
    uint32  cas_latency;  //Can only be set as CAS_LATENCY_1,CAS_LATENCY_2,CAS_LATENCY_3
    uint32  ext_mode_val; //User can config extend mode register in SDRAM.
    uint32  sdram_type;   //DDR or SDR
} SDRAM_CFG_INFO_T, *SDRAM_CFG_INFO_T_PTR;

/******************************************************************************/
//  Description:   Intialize SDRAM parameters
//  Parameter:
//      pPara    the address of SDRAM parameters
//  Return:
//      NULL
/******************************************************************************/
PUBLIC void SDRAM_InitPara (SDRAM_TIMING_PARA_T_PTR timing_ptr);

/******************************************************************************/
//  Description:   Change SDRAM controller settings
//  Parameter:
//      ahb_clk    the AHB clk
//  Return:
//      NULL
/******************************************************************************/
void SDRAM_SetParam (uint32 ahb_clk);

/*****************************************************************************/
//  Description:    This function get sdram config pointer
//  Author:         nick.zhao
//  Note:
/*****************************************************************************/
PUBLIC SDRAM_CFG_INFO_T_PTR SDRAM_GetCfg (void);

/*****************************************************************************/
//  Description:    This function get sdram timing parameter pointer
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC SDRAM_TIMING_PARA_T_PTR SDRAM_GetTimingPara (void);

/*****************************************************************************/
//  Description:    This function can be defined by user if needed. It will be called
//                  before the SDRAM initialization.
//  Author:         nick.zhao
//  Note:
/*****************************************************************************/
PUBLIC void Userdef_before_sdram_init (void);

/*****************************************************************************/
//  Description:    This function can be defined by user if needed. It will be called
//                  after the SDRAM initialization.
//  Author:         nick.zhao
//  Note:
/*****************************************************************************/
PUBLIC void Userdef_after_sdram_init (void);

#ifdef   __cplusplus
}
#endif
#endif // _SDRAM_DRVAPI_H_

