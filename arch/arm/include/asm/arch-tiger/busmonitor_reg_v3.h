/******************************************************************************
 ** File Name:      busmonitor_reg_v0.h                                                 *
 ** Author:         liuhao                                                   *
 ** DATE:           07/29/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/29/2010     liuhao     Create.                                   *
 ******************************************************************************/
#ifndef _BUSMONITOR_REG_V3_H_
#define _BUSMONITOR_REG_V3_H_
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
#define BM_ACCESS_BURST_Single      0
#define BM_ACCESS_BURST_INCR        1
#define BM_ACCESS_BURST_WRAP4       2
#define BM_ACCESS_BURST_INCR4       3
#define BM_ACCESS_BURST_WRAP8       4
#define BM_ACCESS_BURST_INCR8       5
#define BM_ACCESS_BURST_WRAP16      6
#define BM_ACCESS_BURST_INCR16      7

#define BM_CHANEL_ARM_DATA      0
#define BM_CHANEL_ARM_INSTRUCTION   1
#define BM_CHANEL_LCDC          0
#define BM_CHANEL_DCAM          1
#define BM_CHANEL_SD_DMA_INTERNAL   0
#define BM_CHANEL_SD_DMA_EXTERNAL   1

#define CHN_INT(x)                      (BUS_MONx_CTL_BASE + x*0x1000 + 0x0000)
#define CHN_CFG(x)                      (BUS_MONx_CTL_BASE + x*0x1000 + 0x0004)
#define ADDR_MIN(x)                     (BUS_MONx_CTL_BASE + x*0x1000 + 0x0008)
#define ADDR_MAX(x)                     (BUS_MONx_CTL_BASE + x*0x1000 + 0x000c)
#define DATA_MIN(x)                     (BUS_MONx_CTL_BASE + x*0x1000 + 0x0010)
#define DATA_MAX(x)                     (BUS_MONx_CTL_BASE + x*0x1000 + 0x0014)
#define DATA_MASK(x)                    (BUS_MONx_CTL_BASE + x*0x1000 + 0x0018)
#define MATCH_ADDR(x)                   (BUS_MONx_CTL_BASE + x*0x1000 + 0x001c)
#define MATCH_DATA(x)                   (BUS_MONx_CTL_BASE + x*0x1000 + 0x0020)
#define BUSMON_CNT(x)                   (BUS_MONx_CTL_BASE + x*0x1000 + 0x0024)

typedef struct _BM_POINT_REG
{
    volatile unsigned long bm_int;
    volatile unsigned long bm_cfg;
    volatile unsigned long bm_addr_min;
    volatile unsigned long bm_addr_max;
    volatile unsigned long bm_data_min;
    volatile unsigned long bm_data_max;
    volatile unsigned long bm_data_mask;
    volatile unsigned long bm_addr_match;
    volatile unsigned long bm_data_match;
    volatile unsigned long bm_cnt;
} BM_POINT_REG;

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
