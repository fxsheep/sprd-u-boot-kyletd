/******************************************************************************
 ** File Name:      gpio_drv.h                                                  *
 ** Author:         Xueppeng.Zhang                                             *
 ** DATE:           06/12/2010                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This is temporarily file for old interfaces of GPIO device*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/12/2010     Xuepeng.Zhang    Create.                                   *
 ******************************************************************************/

#ifndef _GPIO_DRV_H_
#define _GPIO_DRV_H_

#include "tb_drv.h"
#include "tb_hal.h"

//
// mocor\ms_code\ms_customize\source\product\driver\gpio\Gpio_prod.c
// mocor\ms_code\ms_customize\source\product\driver\ubot\isp1183\cfg\Hal4sys.c
//
#define  HAL_AddGPIOToCallbackTable  GPIO_AddCallbackToIntTable
#define  HAL_GetGPIOVal              GPIO_GetValue
#define  HAL_SetGPIOVal              GPIO_SetValue
#define HAL_SetGPIOInterruptSense      GPIO_SetInterruptSense

#define HAL_SetGPIODirection               GPIO_SetDirection
//
// mocor\ms_code\ms_customize\source\product\driver\gpio\Gpio_ext_drv.c
// mocor\ms_code\ms_customize\source\product\driver\gpio\Gpio_prod.c
//
#define HAL_EnableGPIOIntCtl               GPIO_EnableIntCtl
#define HAL_DisableGPIOIntCtl              GPIO_DisableIntCtl

#include "gpio_drvapi.h"
#endif  // _GPIO_DRV_H_

