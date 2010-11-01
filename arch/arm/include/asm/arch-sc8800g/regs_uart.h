#ifndef _REGS_UART_H_
#define _REGS_UART_H_


#define ARM_UART_TXD    0x0000  //Write data to this address initiates a character transmission through tx fifo.
#define ARM_UART_RXD    0x0004  //Reading this register retrieves the next data byte from the rx fifo.
#define ARM_UART_STS0   0x0008
#define ARM_UART_STS1   0x000C
#define ARM_UART_IEN    0x0010
#define ARM_UART_ICLR   0x0014
#define ARM_UART_CTL0   0x0018
#define ARM_UART_CTL1   0x001C
#define ARM_UART_CTL2   0x0020
#define ARM_UART_CLKD0  0x0024
#define ARM_UART_CLKD1  0x0028
#define ARM_UART_STS2   0x002C  //Add for sc6600b

//The corresponding bit of UART_STS0 register.
#define UARTSTS_RX_FIFO_FULL_OUT        (1 << 0)        //Row Rx_fifo_full_tout.This bit is set when the number of rx fifo data bytes is larger than the rx interrupt watermark value or when receive time is out.  Auto cleared when the condition disappears.
#define UARTSTS_TX_FIFO_EMPTY           (1 << 1)        //Row Tx_fifo_emptyThis bit is set when the number of tx fifo data bytes is less than the tx interrupt watermark value.  Auto cleared when the condition disappears.
#define UARTSTS_PARITY_ERR              (1 << 2)        //Row parity_error
#define UARTSTS_FRAMING_ERR             (1 << 3)        //Row framing_error
#define UARTSTS_RXF_OVERRUN             (1 << 4)        //Row rxf_overrun
#define UARTSTS_DSR_CHANGE              (1 << 5)        //Row dsr_change
#define UARTSTS_CTS_CHANGE              (1 << 6)        //Row cts_change
#define UARTSTS_BREAK_DETECT            (1 << 7)        //Row break_detect
#define UARTSTS_DSR                     (1 << 8)        //DSR: Data set ready
#define UARTSTS_CTS                     (1 << 9)        //CTS: clear to send
#define UARTSTS_RTS                     (1 << 10)       //RTS: Request to send
#define UARTSTS_RXD                     (1 << 11)       //RXD: rx data in
#define UARTSTS_TXD                     (1 << 12)       //TXD: tx data out

//The corresponding bit of UART_STS1 register.
#define RX_FIFO_CNT_MSK 0x000F
#define TX_FIFO_CNT_MSK 0x00F0

//The corresponding bit of UART_IEN register.
#define UARTIEN_RX_FIFO_FULL_OUT        (1 << 0)        //Rx_fifo_full_tout interrupt enable
#define UARTIEN_TX_FIFO_EMPTY           (1 << 1)        //Tx_fifo_empty interrupt enable
#define UARTIEN_PARITY_ERR              (1 << 2)        //Parity_error interrupt enable
#define UARTIEN_FRAMING_ERR             (1 << 3)        //Framing_error interrupt enable
#define UARTIEN_RXF_OVERRUN             (1 << 4)        //Rxf_overrun interrupt enable
#define UARTIEN_DSR_CHANGE              (1 << 5)        //Dsr_change interrupt enable
#define UARTIEN_CTS_CHANGE              (1 << 6)        //Cts_change interrupt enable
#define UARTIEN_BREAK_DETECT            (1 << 7)        //Break_detect interrupt enable

//The corresponding bit of UART_ICLR register.
#define UARTICLR_PARITY_ERR             (1 << 2)        //Write "1" Clear parity_error interrupt
#define UARTICLR_FRAMING_ERR            (1 << 3)        //Write "1" Clear framing_error interrupt
#define UARTICLR_RXF_OVERRUN            (1 << 4)        //Write "1" Clear rx_fifo_overrun interrupt
#define UARTICLR_DSR_CHANGE             (1 << 5)        //Write "1" Clear dsr_change interrupt
#define UARTICLR_CTS_CHANGE             (1 << 6)        //Write "1" Clear cts_change interrupt
#define UARTICLR_BREAK_DETECT           (1 << 7)        //Write "1" Clear break_detect interrupt

//The corresponding bit of UART_CTL0 register.
#define UARTICLR_PARITY_ERR             (1 << 2)        //Write "1" Clear parity_error interrupt
#define UARTICLR_FRAMING_ERR            (1 << 3)        //Write "1" Clear framing_error interrupt
#define UARTICLR_RXF_OVERRUN            (1 << 4)        //Write "1" Clear rx_fifo_overrun interrupt
#define UARTICLR_DSR_CHANGE             (1 << 5)        //Write "1" Clear dsr_change interrupt
#define UARTICLR_CTS_CHANGE             (1 << 6)        //Write "1" Clear cts_change interrupt
#define UARTICLR_BREAK_DETECT           (1 << 7)        //Write "1" Clear break_detect interrupt

//The corresponding bit of UART_CTL0 register.
#define UARTCTL_PARITY_B                (1 << 0)
#define UARTCTL_PARITY_EVEN              0
#define UARTCTL_PARITY_ODD              (1 << 0)

#define UARTCTL_PARITY_EN               (1 << 1)

#define UARTCTL_BL_MSK                  (0x03 << 2)
#define UARTCTL_BL5BITS                  0
#define UARTCTL_BL6BITS                 (1 << 2)
#define UARTCTL_BL7BITS                 (2 << 2)
#define UARTCTL_BL8BITS                 (3 << 2)

#define UARTCTL_SL_MSK                  (0x03 << 4)
#define UARTCTL_SL0BITS                  0
#define UARTCTL_SL1BITS                 (1 << 4)
#define UARTCTL_SL1HBITS                (2 << 4)
#define UARTCTL_SL2BITS                 (3 << 4)

#define UARTCTL_RTS                     (1 << 6)        //Ready To SendWhen receive hardware flow control is not set, this bit controls the output of rtsn. When rhwf is set, this bit is controlled by the rx fifo level.
#define UARTCTL_TX_EMPTY                (1 << 7)        //Send BreakWhen tx fifo is empty and tx is idle, setting this bit forces the tx data output low.Need to be cleared by mcu.
#define UARTCTL_DTR                     (1 << 8)        //Data Terminal ReadyThis bit controls the dtrn output

//The corresponding bit of UART_STS2 register.
#define RX_FIFO_FULL_OUT_MSK            (1 << 0)        //Masked Rx_fifo_full_toutThis bit is set when the number of rx fifo data bytes is larger than the rx interrupt watermark value or when receive time is out. Auto cleared when the condition disappears.
#define TX_FIFO_EMPTY_MSK               (1 << 1)        //Masked Tx_fifo_emptyThis bit is set when the number of tx fifo data bytes is less than the tx interrupt watermark value. Auto cleared when the condition disappears.
#define PARITY_ERR_MSK                  (1 << 2)        //Masked Parity_error
#define FRAMING_ERR_MSK                 (1 << 3)        //Masked Framing_error
#define RXF_CHANGE_MSK                  (1 << 4)        //Masked Rxf_overrun
#define DSR_CHANGE_MSK                  (1 << 5)        //Masked Dsr_change
#define CTS_MSK                         (1 << 6)        //Masked Cts_change
#define BREAK_DETECT_MSK                (1 << 7)        //Masked Break_detect



//-----------------------------------------------
////Global Register 
////GREG_BASE     0x8B000000
////For Uart Enable
#define GEN0_UART0              (0x1)
#define GEN0_UART1              (0x2)
/* default baud rate value. clock frequency of 14.7456MHz */
/* Baud rate divisor = (Fuartclk / (16 * BaudRate)) - 1 */
#if 1 
/* Baud Rate. ( CLK = 12.998 MHz ) */
#define BAUD_1200_13M               0x2A50
#define BAUD_2400_13M               0x1528
#define BAUD_4800_13M               0x0A94
#define BAUD_9600_13M               0x054A
#define BAUD_19200_13M              0x02A5
#define BAUD_38400_13M              0x0152
#define BAUD_57600_13M              0x00E1
#define BAUD_115200_13M             0x0071
#define BAUD_230400_13M             0x0038
#define BAUD_460800_13M             0x001C
#define BAUD_921600_13M             0x000E
/* Baud Rate. ( CLK = 26 MHz ) */
#define BAUD_1200_26M               0x54A0
#define BAUD_2400_26M               0x2A50
#define BAUD_4800_26M               0x1528
#define BAUD_9600_26M               0x0A94
#define BAUD_19200_26M              0x054A
#define BAUD_38400_26M              0x02A5
#define BAUD_57600_26M              0x0152
#define BAUD_115200_26M             0x00E2
#define BAUD_230400_26M             0x0071
#define BAUD_460800_26M             0x0038
#define BAUD_921600_26M             0x001C


#define CPU_CLOCK              12998000
#else
/* Baud Rate. ( CLK = 26 MHz ) */
#define BAUD_4800               0x1528
#define BAUD_9600               0x0A94
#define BAUD_19200              0x054A
#define BAUD_38400              0x02A5
#define BAUD_57600              0x0152
#define BAUD_115200             0x00E2
#define BAUD_230400             0x0071
#define BAUD_460800             0x0038
#define BAUD_921600             0x001C

#define CPU_CLOCK              (12998000 << 1)
#endif /* #if 1 */


#define REG( r ) \
( *( ( volatile unsigned int* )( r ) ) )


/* UART primitives */
#define SIO_GET_RX_STATUS( p )		( REG( ( p ) + ARM_UART_STS1 ) & 0xFF )
#define SIO_RX_READY( s )    	    ( 0 != ( s ) ) 
#define SIO_GET_CHAR( p )		    ( unsigned char )( REG( ( p ) + ARM_UART_RXD ) & 0xFF )

#define SIO_GET_TX_STATUS(p)		( ( ( REG( ( p ) + ARM_UART_STS1 ) ) >> 8 ) && 0xFF )
#define SIO_TX_READY(s)    	    	( 0 == ( s ) )
#define SIO_PUT_CHAR(p,c)  	    	( REG( ( p ) + ARM_UART_TXD ) = ( unsigned int )( ( c ) & 0xFF ) )
		
#define COM1_DEBUG	(1)
#define COM0_USER	(0)
#define HWORD(p)    ((p >> 16) & 0x0FFFF)
#define LWORD(p)    (p & 0x0FFFF)
#define HBYTE(p)    ((p >> 8) & 0x0FF)
#define LBYTE(p)    (p & 0x0FF)
#define MSB(p)      ((p >> 4) & 0x0F)
#define LSB(p)      ((p) & 0x0F)

//Ending define For UART
#endif
