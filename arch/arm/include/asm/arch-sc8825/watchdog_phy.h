/******************************************************************************
 ** File Name:      watchdog_phy.h                                                 *
 ** Author:         Jie Dai                                                   *
 ** DATE:           08/02/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the physical layer of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/02/2010     Jie Dai            Create.                                 *
 ******************************************************************************/

#ifndef __WATCHDOG_PHY_H__
#define __WATCHDOG_PHY_H__

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
#define     WDG_RESULT_OK       (int32)(0x00000000)
#define     WDG_RESULT_ERR      (int32)(0xFFFFFFFF)

typedef enum
{
    WDG_TIMER_STATE_STOP    = 0,    //Stop Watch Dog Timer
    WDG_TIMER_STATE_START,          //Start Watch Dog Timer
    WDG_TIMER_STATE_KEEP            //Don't change Watch Dog Timer State
}
WDG_TIMER_STATE;

typedef enum
{
    WDG_TIMEOUT_MODE_RESET = 0,     //Set Watch Dog to Reset Mode
    WDG_TIMEOUT_MODE_INT,           //Set Watch Dog to Interrupt Mode
    WDG_TIMEOUT_MODE_KEEP           //Don't change Watch Dog Mode
} WDG_TIMEOUT_MODE;

typedef struct
{
    WDG_TIMER_STATE state;
    WDG_TIMEOUT_MODE mode;
    uint32  val;                    //Set the Timer start val
} WDG_CONFIG_T;

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function config the watch dog module.
//  Dependency:     No
//  Author:         Jie.Dai
//  Note:
/*****************************************************************************/
PUBLIC int32 WDG_PHY_CONFIG (WDG_CONFIG_T *cfg);


/*****************************************************************************/
//  Description:    This function clear the watch dog interrupt
//  Dependency:     No
//  Author:         Jie.Dai
//  Note:
/*****************************************************************************/
PUBLIC int32 WDG_PHY_INT_CLR (void);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End


