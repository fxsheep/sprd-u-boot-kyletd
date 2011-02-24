/******************************************************************************
 ** File Name:      boot_mode.c                                               *
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

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "sc_reg.h"
#include "chip_internal.h"
#include "mocor_boot_mode.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL BOOLEAN wdg_debug_mode = SCI_FALSE;

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Sets the different kinds of reset modes, used in normal p-
//                  ower up mode, watchdog reset mode and calibration mode etc.
//  Author:         Jeff.Li
//  Note:           
/*****************************************************************************/
PUBLIC void BOOT_SetRstMode (uint32 val)
{
    CHIP_PHY_SetRstMode (val);
}

/*****************************************************************************/
//  Description:    Gets the current reset mode.
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 BOOT_GetRstMode (void)
{
    return CHIP_PHY_GetRstMode ();
}

/*****************************************************************************/
//  Description:    After normal power on, the HW_RST flag should be reset in
//                  order to judge differrent reset conditions between normal
//                  power on reset and watchdog reset.
//  Author:         Jeff.Li
//  Note:           
/*****************************************************************************/
PUBLIC void BOOT_ResetHWFlag ()
{
    CHIP_PHY_ResetHWFlag (0x1FFF);
}

/*****************************************************************************/
//  Description:    Before watchdog reset, writting HW_RST flag is uesed to j-
//                  udge differrent watchdog reset conditions between MCU reset
//                  and system-halted.
//  Author:         Jeff.Li
//  Note:           
/*****************************************************************************/
PUBLIC void BOOT_SetWDGHWFlag (WDG_HW_FLAG_T type, uint32 val)
{
        CHIP_PHY_SetWDGHWFlag (type, val);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
