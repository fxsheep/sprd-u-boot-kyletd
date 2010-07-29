#include <config.h>
#include <common.h>
#include <asm/arch/regs_global.h>
#include <asm/arch/regs_uart.h>
#include <linux/types.h>
#include <asm/arch/bits.h>

#define __REG(x)     (*((volatile u32 *)(x)))

#if defined (CONFIG_SYS_SC8800X_UART0)
#define UART_PHYS ARM_UART0_BASE
#elif defined (CONFIG_SYS_SC8800X_UART1)
#define UART_PHYS ARM_UART1_BASE
#elif defined (CONFIG_SYS_SC8800X_UART2)
#define UART_PHYS ARM_UART2_BASE
#elif defined (CONFIG_SYS_SC8800X_UART3)
#define UART_PHYS ARM_UART3_BASE
#else
#error "define CONFIG_SYS_SC88000X_UARTx in configs file to use UART driver"
#endif

#ifdef CONFIG_SERIAL_MULTI
#warning "SC8800X driver does not support MULTI serials."
#endif

DECLARE_GLOBAL_DATA_PTR;

void serial_setbrg(void)
{
	uint32_t clk = __REG(GR_GEN1);
	uint32_t brg_div;

	if(clk & GEN1_CLK_26MHZ_EN)
		brg_div = 26000000/CONFIG_BAUDRATE;
	else
		brg_div = 13000000/CONFIG_BAUDRATE;

	if (!gd->baudrate)
		gd->baudrate = CONFIG_BAUDRATE;

/* Set baud rate  */
    __REG( UART_PHYS + ARM_UART_CLKD0 ) = LWORD(brg_div);
    __REG( UART_PHYS + ARM_UART_CLKD1 ) = HWORD(brg_div);
}

int serial_getc(void)
{
	while(SIO_RX_READY( SIO_GET_RX_STATUS( UART_PHYS ) ));
	return SIO_GET_CHAR(UART_PHYS);
}

void serial_putc(const char c)
{
	while ( !SIO_TX_READY( SIO_GET_TX_STATUS(UART_PHYS)));
	SIO_PUT_CHAR(UART_PHYS,c);
	/* If \n, also do \r */
	if (c == '\n')
		serial_putc ('\r');
}

/*
 *  * Test whether a character is in the RX buffer
 *   */
int serial_tstc (void)
{
	/* If receive fifo is empty, return false */
	return SIO_RX_READY( SIO_GET_RX_STATUS( UART_PHYS ) ) ;
}

void serial_puts (const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}

/*
 *  * Initialise the serial port with the given baudrate. The settings
 *   * are always 8 data bits, no parity, 1 stop bit, no start bits.
 *    *
 *     */
int serial_init (void)
{
	__REG(GR_GEN1) &= ~(1<<23);

	/*Disable Interrupt */
	__REG(UART_PHYS + ARM_UART_IEN) = 0; 

	/* Enable UART*/
	__REG (GR_GEN1) |= (1<<23);

	serial_setbrg();

	/* Set port for 8 bit, one stop, no parity  */
	__REG( UART_PHYS + ARM_UART_CTL0 ) = UARTCTL_BL8BITS | UARTCTL_SL1BITS;    
	__REG( UART_PHYS + ARM_UART_CTL1 ) = 0;
	__REG( UART_PHYS + ARM_UART_CTL2 ) = 0;  
}
