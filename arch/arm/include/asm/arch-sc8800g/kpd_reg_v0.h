/******************************************************************************
 ** File Name:    kpd_reg_v0.h                                            *
 ** Author:       hao.liu                                                 *
 ** DATE:         06/11/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/11/2010    hao.liu      Create.                                     *
 ******************************************************************************/
#ifndef _KPD_REG_V0_H_
#define _KPD_REG_V0_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
#include "sci_types.h"
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
//The corresponding bit of KPD_STS register.
#define KPD_INT_ALL                     (0xfff)

#define KPD_PRESS_INT0                  BIT_0
#define KPD_PRESS_INT1                  BIT_1
#define KPD_PRESS_INT2                  BIT_2
#define KPD_PRESS_INT3                  BIT_3

#define KPD_RELEASE_INT0                BIT_4
#define KPD_RELEASE_INT1                BIT_5
#define KPD_RELEASE_INT2                BIT_6
#define KPD_RELEASE_INT3                BIT_7

#define KPD_LONG_KEY_INT0               BIT_8
#define KPD_LONG_KEY_INT1               BIT_9
#define KPD_LONG_KEY_INT2               BIT_10
#define KPD_LONG_KEY_INT3               BIT_11

#define KPD_COL_CNT                     0x7
#define KPD_ROW_CNT                     0x70
#define KPD1_COL_CNT                    0x7
#define KPD1_ROW_CNT                    0x70
#define KPD2_COL_CNT                    0x700
#define KPD2_ROW_CNT                    0x7000
#define KPD3_COL_CNT                    0x70000
#define KPD3_ROW_CNT                    0x700000
#define KPD4_COL_CNT                    0x7000000
#define KPD4_ROW_CNT                    0x70000000

//The corresponding bit of KPD_POLARITY register.
#define KPDPOLARITY_ROW                 (0x00FF)    // Internal row output xor with this 
// value to generate row output.
#define KPDPOLARITY_COL                 (0xFF00)    // Column input xor with this value to
//The corresponding bit of KPD_CLK0.
#define KPDCLK0_CLK_DIV0                0xFFFF      //Clock dividor [15:0]
#define KPDCLK1_TIME_CNT                0xFFB0      //Time out counter value

// KEYPAD Interface Control.
typedef struct kpd_tag
{
    VOLATILE uint32    ctl;
    VOLATILE uint32    int_en;
    VOLATILE uint32    int_raw_status;
    VOLATILE uint32    int_mask_status;
    VOLATILE uint32    int_clr;
    VOLATILE uint32    reserved;    ///reserved
    VOLATILE uint32    polarity;
    VOLATILE uint32    debounce_cnt;
    VOLATILE uint32    long_key_cnt;
    VOLATILE uint32    sleep_cnt;
    VOLATILE uint32    clk_divide_cnt;
    VOLATILE uint32    key_status;
    VOLATILE uint32    sleep_status;
    VOLATILE uint32    debug_status_1;  //only for debug
    VOLATILE uint32    debug_status_2;  //only for debug
} kpd_s;

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

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
