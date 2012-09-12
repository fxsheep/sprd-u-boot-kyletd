/******************************************************************************
 ** File Name:    spi_reg_v0.h                                            *
 ** Author:       Steve.Zhan                                                 *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.Zhan      Create.                                     *
 ******************************************************************************/
#ifndef _SPI_REG_V0_H_
#define _SPI_REG_V0_H_
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
/*----------SPI Interface Control Registers----------*/
#define SPI_TXD                         (SPI_BASE + 0x0000)
#define SPI_CLKD                        (SPI_BASE + 0x0004)
#define SPI_CTL0                        (SPI_BASE + 0x0008)
#define SPI_CTL1                        (SPI_BASE + 0x000c)
#define SPI_CTL2                        (SPI_BASE + 0x0010)
#define SPI_CTL3                        (SPI_BASE + 0x0014)
#define SPI_CTL4                        (SPI_BASE + 0x0018)
#define SPI_CTL5                        (SPI_BASE + 0x001c)
#define SPI_INT_IEN                     (SPI_BASE + 0x0020)
#define SPI_INT_CLR                     (SPI_BASE + 0x0024)
#define SPI_INT_RAW                     (SPI_BASE + 0x0028)
#define SPI_INT_STS                     (SPI_BASE + 0x002c)
#define SPI_STS1                        (SPI_BASE + 0x0030)
#define SPI_STS2                        (SPI_BASE + 0x0034)
#define SPI_DSP_WAIT                    (SPI_BASE + 0x0038)
#define SPI_STS3                        (SPI_BASE + 0x003c)
// SPI control register filed definitions
typedef struct
{
    VOLATILE uint32 data;               // data register
    VOLATILE uint32 clkd;               // clock dividor register
    VOLATILE uint32 ctl0;               // control register
    VOLATILE uint32 ctl1;               // Receive Data full threshold/Receive Data full threshold
    VOLATILE uint32 ctl2;               // 2-wire mode reigster
    VOLATILE uint32 ctl3;               // transmit data interval
    VOLATILE uint32 ctl4;               // transmit data interval
    VOLATILE uint32 ctl5;               // transmit data interval
    VOLATILE uint32 ien;                // interrutp enable register
    VOLATILE uint32 iclr;               // interrupt clear register
    VOLATILE uint32 iraw;               // interrupt clear register
    VOLATILE uint32 ists;               // interrupt clear register
    VOLATILE uint32 sts1;               // fifo cnt register, bit[5:0] for RX and [13:8] for TX
    VOLATILE uint32 sts2;               // masked interrupt status register
    VOLATILE uint32 dsp_wait;           // Used for DSP control
    VOLATILE uint32 sts3;               // Rx_cnt, working in only receive mode as master
    VOLATILE uint32 ctl6;
    VOLATILE uint32 sts4;
    VOLATILE uint32 fifo_rst;
} SPI_CTL_REG_T;

// Bit define for register STS2
#define SPI_RX_FIFO_FULL            BIT_0
#define SPI_RX_FIFO_EMPTY           BIT_1
#define SPI_TX_FIFO_FULL            BIT_2
#define SPI_TX_FIFO_EMPTY           BIT_3
#define SPI_RX_FIFO_REALLY_FULL     BIT_4
#define SPI_RX_FIFO_REALLY_EMPTY    BIT_5
#define SPI_TX_FIFO_REALLY_FULL     BIT_6
#define SPI_TX_FIFO_REALLY_EMPTY    BIT_7
#define SPI_TX_BUSY                 BIT_8
// Bit define for register ctr1
#define SPI_RX_MODE                 BIT_12
#define SPI_TX_MODE                 BIT_13
// Bit define for register ctr2
#define SPI_DMA_EN                  BIT_6
// Bit define for register ctr4
#define SPI_START_RX                BIT_9


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
