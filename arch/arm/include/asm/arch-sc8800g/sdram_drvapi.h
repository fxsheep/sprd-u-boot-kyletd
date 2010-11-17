/******************************************************************************
 ** File Name:      sdram_drvapi.h                                         *
 ** Author:         Nick.zhao                                                 *
 ** DATE:           01/03/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
#ifndef _SDRAM_DRVAPI_H_
#define _SDRAM_DRVAPI_H_
/******************************************************************************
                          Macro define
******************************************************************************/

/******************************************************************************
                          Struct define
******************************************************************************/
/* SDRAM timeing parameter structure */
typedef struct
{
    uint32 row_ref_max;     //ROW_REFRESH_TIME,Refresh interval time , ns, tREF-max = 7800 ns
    uint32 row_pre_min;     //ROW_PRECHARGE_TIME , ns, tRP-min = 27 ns.
    uint32 row_cyc_min;     //ROW_CYCLE_TIME
    uint32 rcd_min;         // T_RCD,ACTIVE to READ or WRITE delay  , ns, tRCD-min = 27 ns
    uint32 wr_min;          // T_WR  ,WRITE recovery time  , ns, tWR-min = 15 ns.
    uint32 mrd_min;         //T_MRD , 2 cycles, tMRD-min = 2 cycles.
    uint32 rfc_min;         //T_RFC, AUTO REFRESH command period , ns, tRFC-min = 80 ns.
    uint32 xsr_min;         //T_XSR  , ns, tXSR-min = 120 ns.
    uint32 ras_min;         //T_RAS_MIN , row active time, ns, tRAS-min = 50ns
} SDRAM_TIMING_PARA_T, *SDRAM_TIMING_PARA_T_PTR;

typedef enum
{
    T_REF_MAX = 0,
    T_RP_MIN,
    T_RCD_MIN,
    T_WR_MIN,
    T_MRD_MIN,
    T_RFC_MIN,
    T_XSR_MIN,
    T_RAS_MIN,
    T_WTR,
    T_RTW
} SDRAM_TIMING_PARA_E;

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


#endif // _SDRAM_DRVAPI_H_
