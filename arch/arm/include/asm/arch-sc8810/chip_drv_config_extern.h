/******************************************************************************
 ** File Name:    chip_drv_config_extern.h                                            *
 ** Author:       steve.zhan                                                 *
 ** DATE:         11/13/2005                                                  *
 ** Copyright:    2010  Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.zhan      Create.                                     *
 ******************************************************************************/
#ifndef _CHIP_DRV_CONFIG_EXTERN_H_
#define _CHIP_DRV_CONFIG_EXTERN_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include <config.h>
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

//CHIP CONFIG
#if defined(PLATFORM_SC6800H)

//Special Chip platfrom io
#include "./sc6800h/sc6800h_plf_io.h"


//Chip module config header files.About some variant param. configs for the module.
#include "./sc6800h/sc6800h_module_config.h"
//end Chip Module config




//Chip Global Function(Reg., BASE, Address,)definitions
#include "./sc6800h/sc6800h_reg_base.h"
//All chip module reg.
#include "./sc6800h/sc6800h_module_ver_extern.h"

#define CHIP_PLATFORM_DEFINED

#endif


#if defined(PLATFORM_SC6600L)
//Special Chip platfrom io
#include "./sc6600l/sc6600l_plf_io.h"
//Chip module config header files.About some variant param. configs for the module.
#include "./sc6600l/sc6600l_module_config.h"
//end Chip Module config

//Chip Global Function(Reg., BASE, Address,)definitions
#include "./sc6600l/sc6600l_reg_base.h"
//All chip module reg.
#include "./sc6600l/sc6600l_module_ver_extern.h"

#define CHIP_PLATFORM_DEFINED
#endif

#if defined(PLATFORM_SC8800H)
//Special Chip platfrom io
#include "./sc8800h/sc8800h_plf_io.h"
//Chip module config header files.About some variant param. configs for the module.
#include "./sc8800h/sc8800h_module_config.h"
//end Chip Module config

//Chip Global Function(Reg., BASE, Address,)definitions
#include "./sc8800h/sc8800h_reg_base.h"
//All chip module reg.
#include "./sc8800h/sc8800h_module_ver_extern.h"

#define CHIP_PLATFORM_DEFINED
#endif

#if defined(PLATFORM_SC8800G)
//Special Chip platfrom io
#include "sc8800g_plf_io.h"
//Chip module config header files.About some variant param. configs for the module.
#include "sc8800g_module_config.h"
//end Chip Module config

//Chip Global Function(Reg., BASE, Address,)definitions
#include "sc8800g_reg_base.h"
//All chip module reg.
#include "sc8800g_module_ver_extern.h"

#define CHIP_PLATFORM_DEFINED
#endif


#if defined(CONFIG_SC8810)
//Special Chip platfrom io
#include "sc8810_plf_io.h"
//Chip module config header files.About some variant param. configs for the module.
#include "sc8810_module_config.h"
//end Chip Module config

//Chip Global Function(Reg., BASE, Address,)definitions
#include "sc8810_reg_base.h"
//All chip module reg.
#include "sc8810_module_ver_extern.h"

#define CHIP_PLATFORM_DEFINED
#endif


//End CHIP CONFIG




#if !defined(CHIP_PLATFORM_DEFINED)
//#error "chip_drv_config.h CONFIG ERROR"
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


