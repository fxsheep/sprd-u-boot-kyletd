/******************************************************************************
 ** File Name:      chip_init_drvapi.h                                           *
 ** Author:         Jeff Li                                                   *
 ** DATE:           06/09/2010                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the interfaces called during system and *
 **                 driver initialization, which export to init.c of refbase. *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/09/2010     Jeff.Li          Create.                                   *
 ******************************************************************************/
#ifndef _CHIP_INIT_DRVAPI_H_
#define _CHIP_INIT_DRVAPI_H_

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
// Description :    Phase1 of chip driver intialization. It initializes criti-
//                  cal resource of chip before the refbase starts to boot, it
//                  contains global clock init and enable, pinmap config init etc.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void CHIPDRV_InitOnPhase1 ();

/*****************************************************************************/
// Description :    Phase2 of chip driver intialization. It initializes driver
//                  modules before refbase confirms powering on. it contains
//                  some necessary modules including RTC, timer, DMA, NLC etc.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void CHIPDRV_InitOnPhase2 ();

/*****************************************************************************/
// Description :    Phase3 of chip driver intialization. It initializes driver
//                  modules after refbase confirms powering on. it contains
//                  the modules including GPIO, watchdog, analog, charge etc.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void CHIPDRV_InitOnPhase3 ();

/*****************************************************************************/
// Description :    Phase4 of chip driver intialization. It initializes MMU
//                  after refbase has copied the kernel image.
// Author :         Jeff.Li
// Note :           Before phase4, the MMU and virtual memory mapping is still
//                  not initialized, so the kernel image coping of refbase sho-
//                  uld make sure that the operating address is physical.
/*****************************************************************************/
PUBLIC void CHIPDRV_InitOnPhase4 ();

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_CHIP_INIT_DRVAPI_H_