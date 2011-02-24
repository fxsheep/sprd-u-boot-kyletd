/******************************************************************************
 ** File Name:      wdg_drvapi.h                                                  *
 ** Author:         Jie Dai                                                   *
 ** DATE:           08/02/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic input and output operations   *
 **                 on hardware, it can be treated as a hardware abstract     *
 **                 layer interface.                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/12/2010     Xuepeng.Zhang    Create.                                   *
 ******************************************************************************/

#ifndef _WDG_DRVAPI_H_
#define _WDG_DRVAPI_H_

/*----------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **------------------------------------------------------------------------- */


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
#define WDG_MAX_COUNT           (0xFFFFFFFF)

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function disable WDG timer
//  Author:         Jimmy.Jia
//  Note:           no rtc_clk to drive the counter
//
/*****************************************************************************/
PUBLIC void WDG_TimerStop (void);


/*****************************************************************************/
//  Description:    This function initialize and setup timer with given value
//  Author:         Jimmy.Jia
//  Note:           rtc_clk drive the counter
//                  load regiter is 32 bits wide so that don't check if param's valid
/*****************************************************************************/
PUBLIC void WDG_TimerStart (uint32 init_val);


/*****************************************************************************/
//  Description:    This function handle WatchDog Interrupt
//  Author:         Binggo Zhou
//  Note:
/*****************************************************************************/
PUBLIC void WDG_TimerHandler (uint32 int_num);
/*****************************************************************************/
//  Description:    This function init WatchDog
//  Author:         Binggo Zhou
//  Note:
/*****************************************************************************/
PUBLIC void WDG_TimerInit (void);

/*****************************************************************************/
//  Description:    This function Reset MCU
//  Author:         Younger.Yang
//  Note:
/*****************************************************************************/
PUBLIC void WDG_ResetMCU (void);

/*****************************************************************************/
//  Description:    This function Load WatchDog Timer Value
//  Author:         Binggo Zhou
//  Note:
/*****************************************************************************/
PUBLIC void WDG_TimerLoad (uint32 time_ms);
PUBLIC void WDG_ClockOn(void);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  // _WDG_DRVAPI_H_


