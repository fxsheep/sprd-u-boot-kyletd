#include "migrate.h"
#include <common.h>
#include <stdarg.h>
#include <malloc.h>
#include <asm/arch/bits.h>
#include <asm/arch/regs_global.h>
#include <asm/arch/regs_uart.h>

int printf(const char *fmt, ...)
{
#if 1
	va_list args;
	unsigned int i;
	char printbuffer[CONFIG_SYS_PBSIZE];

	va_start(args, fmt);
	i = vsprintf(printbuffer, fmt, args);
	serial_puts(printbuffer);
	return i;
#endif
	return 0;
}
void hang(void)
{
	sio_putstr("###ERROR####Please RESET the board###\n");
}

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

#define __REG(x)     (*((volatile u32 *)(x)))

void serial_setbrg(void)
{
	uint32_t clk = __REG(GR_GEN1);
	uint32_t brg_div;

	if(clk & GEN1_CLK_26MHZ_EN)
		brg_div = 26000000/CONFIG_BAUDRATE;
	else
		brg_div = 13000000/CONFIG_BAUDRATE;

	/* Set baud rate  */
	__REG( UART_PHYS + ARM_UART_CLKD0 ) = LWORD(brg_div);
	__REG( UART_PHYS + ARM_UART_CLKD1 ) = HWORD(brg_div);
}

void serial_putc(char c)
{
	while ( !SIO_TX_READY( SIO_GET_TX_STATUS(UART_PHYS)));
	SIO_PUT_CHAR(UART_PHYS,c);
	/* If \n, also do \r */
	if (c == '\n')
		serial_putc ('\r');
}

int serial_getc(void)
{
	while(!SIO_RX_READY( SIO_GET_RX_STATUS( UART_PHYS ) ));
	return SIO_GET_CHAR(UART_PHYS);
}

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
	/* clear input buffer */
	while(SIO_RX_READY( SIO_GET_RX_STATUS( UART_PHYS ) ))
		SIO_GET_CHAR(UART_PHYS);
	return 0;
}

int board_init(void)
{
	return 0;
}

int vprintf(const char *fmt, va_list args)
{
	uint i;
	char printbuffer[CONFIG_SYS_PBSIZE];

	/* For this to work, printbuffer must be larger than
	 *	 * anything we ever want to print.
	 *		 */
	i = vsprintf(printbuffer, fmt, args);

	/* Print the string */
	serial_puts(printbuffer);
	return i;
}

void putc(const char c)
{
	serial_putc(c);
}

int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	return 0;
}

void hw_watchdog_reset(void)
{
}

