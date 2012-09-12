/******************************************************************************
 ** File Name:    uart_reg_v3.h                                            *
 ** Author:       hao.liu                                                 *
 ** DATE:         06/11/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/11/2010    hao.liu      Create.                                     *
 ******************************************************************************/
#ifndef _UART_REG_V3_H_
#define _UART_REG_V3_H_
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
#define UART_INT_STS_MASK     0xfff
#define UART0_INT_STS_MASK     0x7
#define UART1_INT_STS_MASK     (0x7<<3)
#define UART2_INT_STS_MASK     (0x7<<6)
#define UART3_INT_STS_MASK     (0x7<<9)

#define UART_INT_MASK       0xfff
#define UART_INT_BIT_MASK   0x7

#define UART_FIFO_SIZE          128

#define US1_RX_FIFOCNT     0xff
#define US1_TX_FIFOCNT     0xff00

#define UC_RX_WATERMARK    0x7f
#define UC_TX_WATERMARK    0x7f00
//-----------------------------------------------
//UART0, UART1 Registers
//ARM_UART0_BASE                        0x83000000
//ARM_UART1_BASE                        0x84000000
#define ARM_UART_TXD                    0x0000  //Write data to this address initiates a character transmission through tx fifo.
#define ARM_UART_RXD                    0x0004  //Reading this register retrieves the next data byte from the rx fifo.
#define ARM_UART_STS0                   0x0008
#define ARM_UART_STS1                   0x000C
#define ARM_UART_IEN                    0x0010
#define ARM_UART_ICLR                   0x0014
#define ARM_UART_CTL0                   0x0018
#define ARM_UART_CTL1                   0x001C
#define ARM_UART_CTL2                   0x0020
#define ARM_UART_CLKD0                  0x0024
#define ARM_UART_CLKD1                  0x0028
#define ARM_UART_STS2                   0x002C
#define ARM_UART_DSP_WAIT               0x0030
#define UARTDSLPTMR_PERIOD     1000    //ms
/* uart register effective bit definition */
#define REG_MASK_16         0x0000ffff
#define UART_STS0_MASK      0x3fff
#define UART_STS1_MASK      REG_MASK_16
#define UART_STS2_MASK      0x3fff
#define UART_IEN_MASK       0x20ff
#define UART_ICLR_MASK      0x20fC
#define UART_CKD0_MASK      REG_MASK_16
#define UART_CKD1_MASK      0x1f
#define UART_CTL0_MASK      0x3fff
#define UART_CTL1_MASK      0xffff

#define UART_TRANSFER_TIME_OUT 1000 //1000 ms

#define UART_BREAK_DETECT_TIME 0x1

/*
    sts0
    status (Interrupt control) Bits....
    Name:
    US_  : U(uart) + S(status) + '_' + describe.
*/
#define  US_RX_FULL        0x0001
#define  US_TX_EMPTY       0x0002
#define  US_PARITY         0x0004
#define  US_FRAMING        0x0008
#define  US_RXF_OVERRUN    0x0010
#define  US_DSR_CHANGE     0x0020
#define  US_CTS_CHANGE     0x0040
#define  US_BREAK          0x0080
#define  US_DSR            0x0100
#define  US_CTS            0x0200
#define  US_RTS            0x0400
#define  US_RXD            0x0800
#define  US_TXD            0x1000
#define  US_RXT_OUT        0x2000

/*
    ctl
    Config(control) value
*/
#define  UC_PARITY_ODD       1
#define  UC_PARITY_EVEN      0
#define  UC_PARITY_ENABLE  0x2
#define  UC_RTS            0x40
#define  UC_SEND_BREAK     0x80
#define  UC_DTR            0x100

#define  BYTE_SIZE_5       0
#define  BYTE_SIZE_6       (1u << 2)
#define  BYTE_SIZE_7       (2u << 2)
#define  BYTE_SIZE_8       (3u << 2)

#define  STOPBIT_NONE      0
#define  STOPBIT_1         (1u << 4)
#define  STOPBIT_1_5       (2u << 4)
#define  STOPBIT_2         (3u << 4)

/* UART ctrl0 register bit definition */
#define UC_PARITY          BIT_0
#define UC_BYTE_SIZE       (BIT_2 | BIT_3)
#define UC_STOP_BIT_NUBMER (BIT_4 | BIT_5)

/* UART ctrl1 register bit definition */
#define UC_SLEFLOOP_ENABLE   BIT_14
#define UC_DMA_ENABLE   BIT_15

/* UART sts1 register bit position definition */
#define US1_RX_FIFOCNT_SHIFT 0x0
#define US1_TX_FIFOCNT_SHIFT 0x8

/* UART ctrl1 register bit position definition */
#define UC_PARITY_SHIFT      0x0
#define UC_PARITY_ENABLE_SHIFT    0x1
#define UC_BYTE_SIZE_SHIFT   0x2
#define UC_STOP_BIT_NUBMER_SHIFT  0x4

/* UART ctrl2 register bit position definition */
#define UC_RX_WATERMARK_SHIFT 0x0
#define UC_TX_WATERMARK_SHIFT 0x8

//baudrate register has 21 bits: (2exp21 - 2) / 2
#define BAUDRATE_MAX       2097151

#define RECEIVE_TIMEOUT    0x3e00




/*  struct description */
typedef struct
{
    /* the data that writted to this address transfer through tx fifo  */
    volatile uint32       txd; /* bit 0--7  */
    /* read the reg retrieves the next data from the rx fifo */
    volatile uint32       rxd; /* bit 0--7  */

    /*  interrupt status and error status, line status  */
    volatile uint32       sts0;       /* bit 0--15  */

    volatile uint32       fifo_cnt;

    volatile uint32       ien;   /* uart int enable  */
    volatile uint32       iclr;  /* uart int clear   */

    volatile uint32       ctl;   /* control 0 */
    volatile uint32       flow_ctl;  /* control 1 */

    volatile uint32       watermark;

    /*  Clock dividor    */
    volatile uint32       clk0;  /*  Bit 0--15   */
    volatile uint32       clk1;  /*  Bit 0--4    */
    volatile uint32       masked_sts;
} UART_REG_S;

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
#endif //_UART_REG_V3_H_

