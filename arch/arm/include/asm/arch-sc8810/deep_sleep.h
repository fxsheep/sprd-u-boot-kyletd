/******************************************************************************
 ** File Name:      deep_sleep.h                                               *
 ** Author:         Richard Yang                                              *
 ** DATE:           08/25/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DSP and MCU for deep sleep.                               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/25/2003     Richard.Yang     Create.                                   *
 ******************************************************************************/
#ifndef _DEEP_SLEEP_H_
#define _DEEP_SLEEP_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "com_drvapi.h"
#ifndef CONFIG_EMMC_BOOT
//#include "fiq_drvapi.h"
#endif
#include "chip.h"
#ifndef WIN32
#ifndef CONFIG_EMMC_BOOT
//#include "tx_api_thumb.h"
#endif
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#endif //WIN32
#endif // _DEEP_SLEEP_H_

