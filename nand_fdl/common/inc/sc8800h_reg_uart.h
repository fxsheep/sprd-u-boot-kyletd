/******************************************************************************
 ** File Name:    sc8800h_reg_uart.h                                          *
 ** Author:       Aiguo.Miao	                                              *
 ** DATE:         08/28/2007                                                  *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 08/28/2007    Aiguo.Miao      Create.                                     *
 ******************************************************************************/
#ifndef _SC8800H_REG_UART_H_
    #define _SC8800H_REG_UART_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
#define ARM_UART0_BASE          		0x83000000
#define ARM_UART1_BASE          		0x84000000
#define ARM_UART2_BASE          		0x8E000000
#define ARM_UART3_BASE             		0x8F000000
#define PCM_BASE                		0x8F000000

#define ARM_UART_TXD            		0x0000  //Write data to this address initiates a character transmission through tx fifo.
#define ARM_UART_RXD            		0x0004  //Reading this register retrieves the next data byte from the rx fifo.
#define ARM_UART_STS0           		0x0008
#define ARM_UART_STS1           		0x000C
#define ARM_UART_IEN            		0x0010
#define ARM_UART_ICLR           		0x0014
#define ARM_UART_CTL0           		0x0018
#define ARM_UART_CTL1           		0x001C
#define ARM_UART_CTL2           		0x0020
#define ARM_UART_CLKD0          		0x0024
#define ARM_UART_CLKD1          		0x0028
#define ARM_UART_STS2           		0x002C

#define DSP_UART1_Base          		0xBE00
#define DSP_UART2_Base          		0xBE80
#define DSP_UART_TXD            		0xBE00  //Write data to this address initiates a character transmission through tx fifo.
#define DSP_UART_RXD            		0xBE01  //Reading this register retrieves the next data byte from the rx fifo.
#define DSP_UART_STS0           		0xBE02
#define DSP_UART_STS1           		0xBE03
#define DSP_UART_IEN            		0xBE04
#define DSP_UART_ICLR           		0xBE05
#define DSP_UART_CTL0           		0xBE06
#define DSP_UART_CTL1           		0xBE07
#define DSP_UART_CTL2           		0xBE08
#define DSP_UART_CLKD0          		0xBE09
#define DSP_UART_CLKD1          		0xBE0A
#define DSP_UART_STS2           		0xBE0B


//ARM UART.
typedef struct arm_uart_tag
{
    VOLATILE uint32 txd;
    VOLATILE uint32 rxd;
    VOLATILE uint32 sts0;
    VOLATILE uint32 sts1;
    VOLATILE uint32 ie;
    VOLATILE uint32 iclr;
    VOLATILE uint32 ctl0;
    VOLATILE uint32 ctl1;
    VOLATILE uint32 ctl2;
    VOLATILE uint32 clkd0;
    VOLATILE uint32 clkd1;
    VOLATILE uint32 sts2;
}arm_uart_s;

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

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

