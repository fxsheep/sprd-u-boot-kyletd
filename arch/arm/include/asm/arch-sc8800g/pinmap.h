/******************************************************************************
 ** File Name:      pinmap.h                                                  *
 ** Author:         Richard.Yang                                              *
 ** DATE:           03/08/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the structure of pin map.               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/08/2004     Richard.Yang     Create.                                   *
 ******************************************************************************/

#ifndef _PINMAP_H_
#define _PINMAP_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
#define PM_INVALID_VAL        0xffffffff
#define PM_INVALID_SHORT_VAL  0xffff

typedef struct PM_PINFUNC_tag
{
    uint32 addr;
    uint32 value;
} PM_PINFUNC_T;

typedef enum
{
    PM_OUTPUT,
    PM_INPUT,
    PM_INVALID_DIR
} PM_DIR_E;

typedef enum
{
    PM_LEVEL,
    PM_RISING_EDGE,
    PM_FALLING_EDGE,
    PM_BOTH_EDGE,
    PM_NO_INT,
    PM_INVALID_INT
} PM_IS_E;

typedef struct GPIO_CTL_tag
{
    uint16   gpio_num;
    uint16   default_val;
    PM_DIR_E dir;
    PM_IS_E  int_sense;
} PM_GPIO_CTL_T;

extern const PM_PINFUNC_T   pm_func[];
extern const PM_PINFUNC_T   pm_default_global_map[];
extern const PM_GPIO_CTL_T  pm_gpio_default_map[];
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function initialize chip function select.
//  Global resource dependence:
//  Author: Richard.Yang
//  Note:
/*****************************************************************************/
PUBLIC void PM_Init (void);

/*****************************************************************************/
//  Description:    Return the INT sense type according to GPIO number in
//                  pm_gpio_default_map[]
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC PM_IS_E PM_GetGPIOIntType (uint32 gpio_num);

#endif // _PINMAP_H

//  End of pinmap.h