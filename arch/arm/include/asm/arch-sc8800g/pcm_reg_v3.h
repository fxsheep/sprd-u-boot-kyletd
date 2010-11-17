/******************************************************************************
 ** File Name:    pcm_reg_v3.h                                            *
 ** Author:       mingwei.zhang                                                 *
 ** DATE:         06/11/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/11/2010    mingwei.zhang   Create.                                     *
 ** 09/08/2010	  Zhonghe.Huang	  Modify for SC8800G
 ******************************************************************************/
#ifndef _PCM_REG_V3_H_
#define _PCM_REG_V3_H_
#include "sci_types.h"
#include "pcm_drvapi.h"
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
#define WORD_SIZE               (4)
#define HALF_WORD_SIZE          (2)
#define BYTE_SIZE               (1)

#define PCM_DATA_LEN            120

#define FIFO_FULL_WATERMARK             24
#define FIFO_EMPTY_WATERMARK            8
#define PCM_FIFO_SIZE_IN_WORD           32
#define DMA_PCM_RX              DMA_IIS_RX
#define DMA_PCM_TX              DMA_IIS_TX

// IIS_CTL0
#define IIS_CTL0_BIT_PER_CHN_SHIFT      (4)
#define IIS_CTL0_BIT_PER_CHN_MASK       (0x3 << IIS_CTL0_BIT_PER_CHN_SHIFT)
#define IIS_CTL0_BIT_PER_CHN_8          0
#define IIS_CTL0_BIT_PER_CHN_16         1
#define IIS_CTL0_BIT_PER_CHN_32         2

#define IIS_CTL0_SLAVE_MODE             BIT_3
#define IIS_CTL0_IISC_SHRTH             BIT_8
#define IIS_CTL0_SYNC_MODE              BIT_9
#define IIS_CTL0_PCM_SCK_REV            BIT_11
#define IIS_CTL0_PCM_DMA_EB             BIT_14
#define IIS_CTL0_PCM_EB                 BIT_15

// IIS_CTL2
#define IIS_CTL2_PCM_SLOT_SHIFT         (0)
#define IIS_CTL2_PCM_SLOT_MASK          (0x7 << IIS_CTL2_PCM_SLOT_SHIFT)

#define IIS_CTL2_PCM_CYCLE_SHIFT        (3)
#define IIS_CTL2_PCM_CYCLE_MASK         (0x7F << IIS_CTL2_PCM_CYCLE_SHIFT)

// IIS_CTL3
#define IIS_CTL3_RX_FULL_THRHLD_SHIFT   (0)
#define IIS_CTL3_RX_FULL_THRHLD_MASK    (0x1F << IIS_CTL3_RX_FULL_THRHLD_SHIFT)
#define IIS_CTL3_RX_EMPTY_THRHLD_SHIFT  (8)
#define IIS_CTL3_RX_EMPTY_THRHLD_MASK   (0x1F << IIS_CTL3_RX_EMPTY_THRHLD_SHIFT)

//IIS_CTL4
#define IIS_CTL4_TX_FULL_THRHLD_SHIFT   (0)
#define IIS_CTL4_TX_FULL_THRHLD_MASK    (0x1F << IIS_CTL4_TX_FULL_THRHLD_SHIFT)
#define IIS_CTL4_TX_EMPTY_THRHLD_SHIFT  (8)
#define IIS_CTL4_TX_EMPTY_THRHLD_MASK   (0x1F << IIS_CTL4_TX_EMPTY_THRHLD_SHIFT)

// IIS_INT_IEN
#define IIS_INT_IEN_RX_FIFO_FULL_EN     BIT_0
#define IIS_INT_IEN_RX_FIFO_EMPTY_EN    BIT_1
#define IIS_INT_IEN_TX_FIFO_FULL_EN     BIT_2
#define IIS_INT_IEN_TX_FIFO_EMPTY_EN    BIT_3
#define IIS_INT_IEN_RX_OVERRUN_EN       BIT_4
#define IIS_INT_IEN_SLAVE_TIMEOUT_EN    BIT_5
#define IIS_INT_IEN_RX_FULL_EN          BIT_6
#define IIS_INT_IEN_TX_EMPTY_EN         BIT_7

// IIS_INT_CLR
#define IIS_INT_IEN_RX_FIFO_FULL_CLR    BIT_0
#define IIS_INT_IEN_RX_FIFO_EMPTY_CLR   BIT_1
#define IIS_INT_IEN_TX_FIFO_FULL_CLR    BIT_2
#define IIS_INT_IEN_TX_FIFO_EMPTY_CLR   BIT_3
#define IIS_INT_IEN_RX_OVERRUN_CLR      BIT_4
#define IIS_INT_IEN_SLAVE_TIMEOUT_CLR   BIT_5

// IIS_INT_STS
#define IIS_INT_STS_RX_FIFO_FULL        BIT_6
#define IIS_INT_STS_TX_FIFO_FULL        BIT_2
#define IIS_INT_STS_TX_FIFO_EMPTY       BIT_7
#define IIS_INT_STS_SLAVE_TIMEOUT       BIT_5


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
// PCM control register filed definitions
typedef struct
{
    volatile uint32 iis_fifo;               // 0x00, fifo
    volatile uint32 iis_clkd;               // 0x04, clk_dividor
    volatile uint32 iis_ctl0;               // 0x08, ctl0--basic setting control register
    volatile uint32 iis_ctl1;               // 0x0c, ctl1
    volatile uint32 iis_ctl2;               // 0x10, ctl2--pcm slot relavent
    volatile uint32 iis_ctl3;               // 0x14, ctl3--bit[4:0]:rx_full_th; bit[12:8]:rx_empty_th
    volatile uint32 iis_ien;                // 0x18, ien
    volatile uint32 iis_iclr;               // 0x1c, iclr
    volatile uint32 iis_iraw;               // 0x20, iraw
    volatile uint32 iis_ists;               // 0x24, ists--interrupt masked sts
    volatile uint32 iis_sts1;               // 0x28, sts1--rx fifo R/W addr
    volatile uint32 iis_sts2;               // 0x2c, sts2--basic info of pcm module
    volatile uint32 iis_sts3;               // 0x30, sts3--lrck count
    volatile uint32 iis_dspwait;            // 0x34, dspwait
    volatile uint32 iis_ctl4;               // 0x38, ctl4--bit[4:0]:tx_full_th; bit[12:8]:tx_empty_th
    volatile uint32 iis_sts4;				// 0x3c, sts4--tx fifo R/W addr
} PCM_CTL_REG_T;

#define PCM_ENABLE_PCM_CLK() do { \
    REG32(GR_PLL_SCR) |= (BIT_8 | BIT_9);	\
    REG32(GR_GEN2)	  &= ~(0xF << 24); \
    REG32(GR_GEN2)	  |= (0x1 << 24);  \
} while (0)

#define PCM_ENABLE_MODULE() do { \
    volatile uint32 val = 0;  \
    val =  REG32 (GR_GEN0);  \
    val |= GEN0_I2S_EN;  \
    REG32 (GR_GEN0) = val;  \
  } while (0)

#define PCM_DISABLE_MODULE() do { \
    REG32 (GR_GEN0) &= ~GEN0_I2S_EN;  \
} while (0)

#define PCM_SET_RX_WATERMARK(param) do {  \
    volatile PCM_CTL_REG_T *reg_ptr = (volatile PCM_CTL_REG_T *) PCM_CTL_BASE; \
    reg_ptr->iis_ctl3 &= ~(IIS_CTL3_RX_FULL_THRHLD_MASK | IIS_CTL3_RX_EMPTY_THRHLD_MASK ); \
    reg_ptr->iis_ctl3 |=   ((param->rx_watermark << IIS_CTL3_RX_FULL_THRHLD_SHIFT) \
                                   | (param->tx_watermark << IIS_CTL3_RX_EMPTY_THRHLD_SHIFT)); \
} while (0)

#define PCM_SET_TX_WATERMARK(param) do {  \
	volatile PCM_CTL_REG_T *reg_ptr = (volatile PCM_CTL_REG_T *) PCM_CTL_BASE; \
	reg_ptr->iis_ctl4 &= ~(IIS_CTL4_TX_FULL_THRHLD_MASK | IIS_CTL4_TX_EMPTY_THRHLD_MASK ); \
	reg_ptr->iis_ctl4 |=   ((param->rx_watermark << IIS_CTL4_TX_FULL_THRHLD_SHIFT) \
		| (param->tx_watermark << IIS_CTL4_TX_EMPTY_THRHLD_SHIFT)); \
} while (0)

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/

#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_PCM_REG_V3_H_




