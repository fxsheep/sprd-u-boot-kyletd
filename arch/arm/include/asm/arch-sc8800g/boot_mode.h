/******************************************************************************
 ** File Name:      boot_mode.h                                               *
 ** Author:         Jeff Li                                                   *
 ** DATE:           31/08/2010                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the logic interfaces called during boot,*
 **                 including reset mode setting, initialization etc.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 31/08/2010     Jeff.Li          Create.                                   *
 ******************************************************************************/
#ifndef _BOOT_MODE_H_
#define _BOOT_MODE_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Sets the different kinds of reset modes, used in normal p-
//                  ower up mode, watchdog reset mode and calibration mode etc.
//  Author:         Jeff.Li
//  Note:           
/*****************************************************************************/
PUBLIC void BOOT_SetRstMode (uint32 val);

/*****************************************************************************/
//  Description:    Gets the current reset mode.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 BOOT_GetRstMode (void);

/*****************************************************************************/
//  Description:    After normal power on, the HW_RST flag should be reset in
//                  order to judge differrent reset conditions between normal
//                  power on reset and watchdog reset.
//  Author:         Jeff.Li
//  Note:           
/*****************************************************************************/
PUBLIC void BOOT_ResetHWFlag ();

/*****************************************************************************/
//  Description:    Before watchdog reset, writting HW_RST flag is uesed to j-
//                  udge differrent watchdog reset conditions between MCU reset
//                  and system-halted.
//  Author:         Jeff.Li
//  Note:           
/*****************************************************************************/
PUBLIC void BOOT_SetWDGHWFlag (uint32 val);

/*****************************************************************************/
//  Description:    Gets the flag.
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GetWatchdogMode (void);

/*****************************************************************************/
//  Description:    Sets the flag when watchdog resets the system in debug mode.
//  Author:
/*****************************************************************************/
PUBLIC void SetWatchdogMode (BOOLEAN mode);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif

