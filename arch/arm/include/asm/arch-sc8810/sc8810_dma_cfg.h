/*******************************************************************************
 ** File Name:      sc8810_dma_cfg.h                                          *
 ** Author:         andy.chen                                                  *
 ** Date:           08/23/2010                                                 *
 ** Copyright:      Copyright 2001-xxxx by Spreadtrum Communications,Inc.      *
 *                  All Rights Reserved.                                       *
 **                 This software is supplied under the terms of a license     *
 **                 agreement or non-disclosure agreement with Spreadtrum.     *
 **                 Passing on and copying of this document,and communication  *
 **                 of its contents is not permitted without prior written     *
 **                 authorization.                                             *
 ** Description:    This is the header file of defines signals of the sim      *
 **                 application layer                                          *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** ---------------------------------------------------------------------------*
 ** Date           Name             Description                                *
 **----------------------------------------------------------------------------*
 ** 2010.08        andy.chen         Create                                    *
 ** 20xx.xx        xxxx.xxxx         update                                    *
 ******************************************************************************/


#ifndef __SC8810_DMA_CFG_H__
#define __SC8810_DMA_CFG_H__

/*******************************************************************************
 **                        Dependencies                                        *
 ******************************************************************************/
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/

#ifdef   __cplusplus
extern   "C"
{
#endif

/*******************************************************************************
 **                        Mcaro Definitions                                   *
 ******************************************************************************/
// DMA MAX CH NUMBER
#define DMA_CH_NUM                      32
#define DMA_SOFT_BLOCK_WAITTIME         (0x0f)
#define DMA_HARD_BLOCK_WAITTIME         (0x0f)
#define DMA_MAX_TRANSSIZE               (1<<25)    //2^25byte
#define DMA_MAX_BURSTSIZE               (1<<16)    //2^16byte

// DMA user id
#define DMA_SOFT0                       0x00
#define DMA_UART0_TX                    0x01
#define DMA_UART0_RX                    0x02
#define DMA_UART1_TX                    0x03
#define DMA_UART1_RX                    0x04
#define DMA_UART2_TX                    0x05
#define DMA_UART2_RX                    0x06
#define DMA_IIS_TX                      0x07
#define DMA_IIS_RX                      0x08
#define DMA_EPT_IN                      0x09
#define DMA_EPT_OUT                     0x0A
#define DMA_VB_DA0                      0x0B
#define DMA_VB_DA1                      0x0C
#define DMA_VB_AD0                      0x0D
#define DMA_VB_AD1                      0x0E
#define DMA_SIM0_TX                     0x0F
#define DMA_SIM0_RX                     0x10
#define DMA_SIM1_TX                     0x11
#define DMA_SIM1_RX                     0x12
#define DMA_SPI_TX						0x13
#define DMA_SPI_RX						0x14
#define DMA_ROT                         0x15
#define DMA_DRM_RAW						0x1D
#define DMA_DRM_CPT						0x1E
#define DMA_VB_AD                       DMA_VB_AD0
#define DMA_USB_EP1						DMA_SOFT0
#define DMA_USB_EP3						DMA_SOFT0


// endian issue
#define DMA_ENDIAN_SELECT               DMA_CFG_Endian()
/*******************************************************************************
 **                        Structures Definitions                              *
 ******************************************************************************/
//dma channel type
typedef enum _DMA_CHN_TYPE
{
    DMA_CHN_TYPE_HARD = 0,
    DMA_CHN_TYPE_SOFT,
    DMA_CHN_TYPE_MAX
}
DMA_CHN_TYPE_E;

/*******************************************************************************
 **                        data prototype                                      *
 ******************************************************************************/

/*******************************************************************************
 **                        Function prototype                                  *
 ******************************************************************************/
/******************************************************************************/
//  Description:    get dma channel type
//  Global resource dependence:
//  Author:         andy.chen
//  Note:           channel:dma channel number,0-31
//  Return value:   DMA_CHN_TYPE_HARD/DMA_CHN_TYPE_SOFT
/******************************************************************************/
PUBLIC DMA_CHN_TYPE_E DMA_CFG_GetChnType (uint32 channel);

/******************************************************************************/
//  Description:    dma endian select
//  Global resource dependence:
//  Author:         andy.chen
//  Note:
//  Return value:
/******************************************************************************/
PUBLIC void DMA_CFG_Endian (void);

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif  //__SC8810_DMA_CFG_H__
// End


