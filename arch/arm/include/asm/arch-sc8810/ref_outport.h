/******************************************************************************
 ** File Name:    ref_outport.h                                                    *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2006                                                    *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2006      Daniel.Ding     Create.                                     *
 ******************************************************************************/
#ifndef _REF_OUTPORT_H_
    #define _REF_OUTPORT_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "sci_types.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
//REF ERROR CODE
#define REF_AT_BASE         0X80000000
#define REF_AT_INVLID_PARA  REF_AT_BASE + 1
#define REF_AT_INVLID_CMD   REF_AT_BASE + 2

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function delay some ticks .
// Global resource dependence :
// Author :         Daniel.ding
// Note :           
/*****************************************************************************/
PUBLIC uint32 OS_TickDelay(uint32 ticks);

/*****************************************************************************/
//  Description:    OS_CheckStackRegion
//  Global resource dependence: 
//  Author:        Younger.yang
//  Note:           This function  provide system stack region
/*****************************************************************************/

BOOLEAN OS_CheckStackRegion(uint32 addr);

/*****************************************************************************/
//  Description:    OS_CheckCodeRegion
//  Global resource dependence: 
//  Author:        Younger.yang
//  Note:           This function  provide system code region
/*****************************************************************************/
BOOLEAN OS_CheckCodeRegion(uint32 addr);

/*****************************************************************************/

PUBLIC uint32 OS_TickDelay(uint32 ticks);

 /*****************************************************************************/
// Description : This function send a char to uart or usb virtual com at assert debug
// Global resource dependence :
// Author :      weihua.wang
// Note :           
/*****************************************************************************/
PUBLIC void OS_DebugPutChar(uint8 ch);

/******************************************************************************/
// Description:   Handler AT Command
// Dependence:    None
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
PUBLIC uint32 REF_HandleATCommand(uint8* at_string);

/*****************************************************************************/
// Description : This function send a char to uart or usb virtual com at assert debug
// Global resource dependence :
// Author :      weihua.wang
// Note :           
/*****************************************************************************/
PUBLIC void OS_DebugPutChar(uint8 ch);

/*****************************************************************************/
// Description : This function scanf a char from uart receive register at debug.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/
PUBLIC int OS_DebugGetChar(void);
 
/*****************************************************************************/
//  Description:    The OS_DumpPrint function is used to print string by 
//                  serial port.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC  void OS_DumpFlush(void);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
