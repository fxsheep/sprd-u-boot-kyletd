/******************************************************************************
 ** File Name:      os_param.h                                                *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           08/27/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contain some OS parameter config.               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/27/2003     Xueliang.Wang    Create.                                   *
 ******************************************************************************/
#ifndef _OS_PARAM_H
#define _OS_PARAM_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h" 

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Defines                                           *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
// Assert mode value.
// If it is in debug mode, print assert information when assert; 
// else reset.
#define SCI_ASSERT_DEBUG_MODE       1
/* @Jim.zhang CR:MS12298 2004-08-26 */
#define SCI_ASSERT_RELEASE_MODE     0x35
/* end CR:MS12298 */

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function initialize os parameters which on NVItem.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           It must be called before use the os parameters, 
//                  and after flash could access.
/*****************************************************************************/
PUBLIC void OSPARAM_InitOsParam(void);

/*****************************************************************************/
//  Description:    The function gets static thread log table.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           Size of log_table should be not less than 
//                  uint32[SCI_MAX_STATIC_THREAD_ID].
/*****************************************************************************/
PUBLIC uint32 OSPARAM_GetThreadLogTable(// If set successfully, return 
                                        // SCI_SUCCESS; else return SCI_ERROR
    uint32  *log_table_ptr  // Pointer to a uint32 arry, 
                            // which used to save log table.
    );

/*****************************************************************************/
//  Description:    The function sets static thread log table.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           Size of log_table should be equal
//                  uint32[SCI_MAX_STATIC_THREAD_ID].
/*****************************************************************************/
PUBLIC uint32 OSPARAM_SetThreadLogTable(// If set successfully, return 
                                        // SCI_SUCCESS; else return SCI_ERROR
    uint32  *log_table_ptr  // Pointer to log table which used to set.
    );

/*****************************************************************************/
//  Description:    The function gets assert mode.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           The valid value should be:
//                      SCI_ASSERT_DEBUG_MODE  or SCI_ASSERT_RELEASE_MODE
/*****************************************************************************/
PUBLIC uint32 OSPARAM_GetAssertMode(// If set successfully, return SCI_SUCCESS; 
                                    // else return SCI_ERROR
    uint8  *assert_mode_ptr     // Pointer to a uint8, 
                                // which used to save assert mode.
    );

/*****************************************************************************/
//  Description:    The function sets assert mode.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           The valid value should be:
//                      SCI_ASSERT_DEBUG_MODE  or SCI_ASSERT_RELEASE_MODE
/*****************************************************************************/
PUBLIC uint32 OSPARAM_SetAssertMode(// If set successfully, return SCI_SUCCESS; 
                                    // else return SCI_ERROR
    uint8  assert_mode              // Assert mode value used to set.
    );
    
/*****************************************************************************/
//  Description:    The function get record assert infor to flash flag.
//	Global resource dependence: 
//  Author:         Johnson.sun
//	Note:           
/*****************************************************************************/
PUBLIC uint8 OSPARAM_GetRecordAssertInforToFlashFlag( void );    
    
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // _OS_PARAM_H