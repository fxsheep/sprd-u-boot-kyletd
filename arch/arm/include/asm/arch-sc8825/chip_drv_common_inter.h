/******************************************************************************
 ** File Name:    chip_drv_common_inter.h                                     *
 ** Author:       Steve.Zhan                                                  *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE                NAME            DESCRIPTION                           *
 **  06/05/2010         Steve.Zhan       Create.                              *
 ******************************************************************************/
#ifndef _CHIP_DRV_COMMON_INTER_H_
#define _CHIP_DRV_COMMON_INTER_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/

#define DRV_INTERRUPT_FUNCTIONS_ENABLE

#include "int_hisr.h"

#ifdef DRV_INTERRUPT_FUNCTIONS_ENABLE

#define CHIPDRV_EnableIRQINT(int_num) _CHIPDRV_EnableIRQINT(int_num)
#define CHIPDRV_DisableIRQINT(int_num) _CHIPDRV_DisableIRQINT(int_num)
#define CHIPDRV_DisableIRQINTALL() _CHIPDRV_DisableIRQINTALL()
#define CHIPDRV_GetIRQStatus() _CHIPDRV_GetIRQStatus()

#define CHIPDRV_EnableFIQINT(int_num) _CHIPDRV_EnableFIQINT(int_num)
#define CHIPDRV_DisableFIQINT(int_num) _CHIPDRV_DisableFIQINT(int_num)
#define CHIPDRV_DisableFIQINTALL() _CHIPDRV_DisableFIQINTALL()

#define CHIPDRV_DisableINT() _CHIPDRV_DisableINT()

#else

#define CHIPDRV_EnableIRQINT(int_num) NULL
#define CHIPDRV_DisableIRQINT(int_num) NULL
#define CHIPDRV_DisableIRQINTALL() NULL
#define CHIPDRV_GetIRQStatus() NULL

#define CHIPDRV_EnableFIQINT(int_num) NULL
#define CHIPDRV_DisableFIQINT(int_num) NULL
#define CHIPDRV_DisableFIQINTALL() NULL

#define CHIPDRV_DisableINT() NULL


#endif


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

