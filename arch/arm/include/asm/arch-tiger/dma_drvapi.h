/******************************************************************************
 ** File Name:      dma_drvapi.h                                                 *
 ** Author:         andy.chen                                                 *
 ** DATE:           07/28/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic interfaces of dma.             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/28/2010     andy.chen        Create.                                   *
 ******************************************************************************/
#ifndef _DMA_DRVAPI_H_
#define _DMA_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
**                          Micro Define
**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                      Function  Prototype
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    dma memcpy
//  Global resource dependence:
//  Author:         weihua.wang
//  input param     des_address: New buffer
//                  src_address: Buffer to copy from
//                  count      : Number of characters to copy in bytes
//  return value    zero       : param error or no available channel
//                  other value: writed in bytes
//  Note:           soft dma operation
/*****************************************************************************/
PUBLIC uint32 DMA_HAL_Memcpy (void *des, void *src, uint32 count);

/*****************************************************************************/
//  Description:    dma memset
//  Global resource dependence:
//  Author:         weihua.wang
//  input param     des_address: New buffer
//                  value      : value to set
//                  count      : Number of characters to copy in bytes
//  return value    zero       : param error or no available channel
//                  other value: writed in bytes
//  Note:           soft dma operation
/*****************************************************************************/
PUBLIC uint32 DMA_HAL_Memset (void *des, uint8 value, uint32 count);


#ifdef   __cplusplus
}
#endif

#endif //_DMA_DRVAPI_H_

/* End Of File  */


