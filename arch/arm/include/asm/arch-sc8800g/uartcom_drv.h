/******************************************************************************
 ** File Name:      uartcom_drv.h                                             *
 ** Author:         Zhengjiang.Lei                                            *
 ** DATE:           11/03/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                      serrial device. It manages create, send, receive     *
 **                      interface of serrial port.                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/03/2005     Zhengjiang.Lei     Create.                                 *
 ** 07/04/2007     David.Jia       CR 82866, add UART_GetRxFifoCnt.           *
 ** 02/09/2008     Tao.Zhou        driver arrangement                         *
 ******************************************************************************/
#ifndef _UARTCOM_DRV_H_
#define _UARTCOM_DRV_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sc_reg.h"
#include "isr_drvapi.h"
#include "rtc_drvapi.h"
#ifndef WIN32
#include "sio.h"
#endif

#include "timer_drvapi.h"
#include "adc_drvapi.h"
#include "chip.h"

#include "uart_drvapi.h"

#endif


