/******************************************************************************
 ** File Name:      pin_sleep.h                                               *
 ** Author:         jiexia.yu                                                 *
 ** DATE:           07/28/2007                                                *
 ** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic configuration for pin         *
 **                 management in deepsleep condition.                        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/28/2007     jiexia.yu        Create.                                   *
 ******************************************************************************/
#ifndef _PIN_SLEEP_H_
#define _PIN_SLEEP_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "arm_reg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Defines                                           *
 **---------------------------------------------------------------------------*/
//PIN sleep struct define
typedef struct pin_sleep_tag
{
    uint32 reg_addr;    //register address
    uint32 bit_mask;    //register data mask
    uint32 bit_set;     //register bit that want to be setup
    uint32 reg_save;    //temp variable for save original register value
}PIN_SLEEP_T;


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:  setup pin config when system enters deepsleep
//	Global resource dependence:  misc_pin_sleep_cfg[] 
//  Author:  jiexia.yu         
//	Note:           
/*****************************************************************************/
PUBLIC void PIN_EnterDeepSleep(void);

/*****************************************************************************/
//  Description:  restore pin config when system leaves deepsleep
//	Global resource dependence:  misc_pin_sleep_cfg[]
//  Author:  jiexia.yu         
//	Note:           
/*****************************************************************************/
PUBLIC void PIN_LeaveDeepSleep(void);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif // _PIN_SLEEP_H_

