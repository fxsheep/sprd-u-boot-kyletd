#include "sio_api.h"
#include "sio_drv.h"

static unsigned int hw_baudrate(unsigned int baudrate);
static void sio_hw_open(unsigned int baudrate);

void sio_open(unsigned int baudrate)
{
    sio_hw_open(hw_baudrate(baudrate));
}

void sio_set_baudrate(unsigned int baudrate)
{
	sio_hw_open(hw_baudrate(baudrate));
}

int sio_write(const void * buf, int len)
{
	const unsigned char * pstart = (const unsigned char*)buf;
	const unsigned char * pend = pstart + len;
    while (pstart < pend) {
		/* Check if tx port is ready.*/
		/*lint -save -e506 -e731*/
#ifndef CHIP_VER_8800H5
		while (!SIO_TX_READY(SIO_GET_TX_STATUS(ARM_UART0_Base))) {
#else		
		while (!SIO_TX_READY(SIO_GET_TX_STATUS(FDL_GetUARTBaseAddr()))) {
#endif

            /* Do nothing */
        }      
#ifndef CHIP_VER_8800H5
		SIO_PUT_CHAR(ARM_UART0_Base, *pstart);
#else		
		SIO_PUT_CHAR(FDL_GetUARTBaseAddr(), *pstart);
#endif
		++pstart;
	}	
    
    /* Ensure the last byte is written successfully */
	while (!SIO_TX_READY(SIO_GET_TX_STATUS(ARM_UART0_Base))) {
    	/* Do nothing */
    }
	
    return pstart - (const unsigned char*)buf;
}

int sio_read(void * buf, int len)
{
	unsigned char * pstart = (unsigned char*)buf;
	const unsigned char * pend = pstart + len;
	while ((pstart < pend) 
#ifndef CHIP_VER_8800H5
		&& SIO_RX_READY(SIO_GET_RX_STATUS(ARM_UART0_Base))) {
		*pstart++ = SIO_GET_CHAR(ARM_UART0_Base);
	}
#else		
		&& SIO_RX_READY(SIO_GET_RX_STATUS(FDL_GetUARTBaseAddr()))) {
		*pstart++ = SIO_GET_CHAR(FDL_GetUARTBaseAddr());
	}
#endif
	return pstart - (unsigned char*)buf;
}

int sio_putstr(const char * str)
{
	const char * p = str;
	while ('\0' != *p) {
		/* Check if tx port is ready.*/
#ifndef CHIP_VER_8800H5
		while (!SIO_TX_READY(SIO_GET_TX_STATUS(ARM_UART0_Base))) {
            /* Do nothing */
        }      
#else		
		while (!SIO_TX_READY(SIO_GET_TX_STATUS(FDL_GetUARTBaseAddr()))) {
            /* Do nothing */
        }      
#endif
		SIO_PUT_CHAR(ARM_UART0_Base, *p);
		++p;
	}	
    /* Ensure the last byte is written successfully */
#ifndef CHIP_VER_8800H5
	while (!SIO_TX_READY(SIO_GET_TX_STATUS(ARM_UART0_Base))) {
    	/* Do nothing *//*lint -restore*/
    }
#else		
	while (!SIO_TX_READY(SIO_GET_TX_STATUS(FDL_GetUARTBaseAddr()))) {
    	/* Do nothing *//*lint -restore*/
    }
#endif
    return p - str;
}

void sio_hw_open(unsigned int baudrate)
{
#ifndef NAND_FDL_SC8800H
    /* Disable UART*/
    *((volatile unsigned int*) (GR_GEN0)) &= (unsigned int)(~GEN0_UART0);
    
    /*Disable Interrupt */

    *(volatile unsigned int*) (ARM_UART0_Base + ARM_UART_IEN) = 0;



    
    /* Enable UART*/
    *((volatile unsigned int*) (GR_GEN0)) |= GEN0_UART0;
#else
    /* Disable UART*/
    *(volatile unsigned int*)0x8b000018 &= ~(0x00400000);
    
    /*Disable Interrupt */
#ifndef CHIP_VER_8800H5
    *(volatile unsigned int*) ( ARM_UART0_Base + ARM_UART_IEN) = 0;
#else		
    *(volatile unsigned int*) ( FDL_GetUARTBaseAddr() + ARM_UART_IEN) = 0;
#endif
    
    /* Enable UART*/
    *(volatile unsigned int*)0x8b000018 |= (0x00400000);
#endif
    /* Set baud rate  */
#ifndef CHIP_VER_8800H5
    *(volatile unsigned int*) (ARM_UART0_Base + ARM_UART_CLKD0) = LWORD(baudrate);
    *(volatile unsigned int*) (ARM_UART0_Base + ARM_UART_CLKD1) = HWORD(baudrate);
    
    
    /* Set port for 8 bit, one stop, no parity  */
    *(volatile unsigned int*) (ARM_UART0_Base + ARM_UART_CTL0) = UARTCTL_BL8BITS | UARTCTL_SL1BITS;    
    *(volatile unsigned int*) (ARM_UART0_Base + ARM_UART_CTL1) = 0;
    *(volatile unsigned int*) (ARM_UART0_Base + ARM_UART_CTL2) = 0;
#else		
    *(volatile unsigned int*) (FDL_GetUARTBaseAddr() + ARM_UART_CLKD0) = LWORD(baudrate);
    *(volatile unsigned int*) (FDL_GetUARTBaseAddr() + ARM_UART_CLKD1) = HWORD(baudrate);
    
    
    /* Set port for 8 bit, one stop, no parity  */
    *(volatile unsigned int*) (FDL_GetUARTBaseAddr() + ARM_UART_CTL0) = UARTCTL_BL8BITS | UARTCTL_SL1BITS;    
    *(volatile unsigned int*) (FDL_GetUARTBaseAddr() + ARM_UART_CTL1) = 0;
    *(volatile unsigned int*) (FDL_GetUARTBaseAddr() + ARM_UART_CTL2) = 0;
#endif
}

unsigned int hw_baudrate(unsigned int baudrate)
{
    unsigned long ext_clk,apb_clk;
    
    ext_clk=*(volatile unsigned long *)0x8b000018;
    
    if(ext_clk &(0x1<<15))
    {
        //APB_clk = 26Mhz
        apb_clk = 12998000 * 2;
    }
    else
    {
        //APB_clk = 13Mhz
        apb_clk = 12998000 * 1;
    }

	return (unsigned int)((apb_clk + baudrate / 2) / baudrate);
}
void sio_put_char(unsigned char c)
{
#ifndef CHIP_VER_8800H5
	while (!SIO_TX_READY(SIO_GET_TX_STATUS(ARM_UART0_Base))) {
        /* Do nothing */
    }      
	SIO_PUT_CHAR(ARM_UART0_Base, c);

    /* Ensure the last byte is written successfully */
	while (!SIO_TX_READY(SIO_GET_TX_STATUS(ARM_UART0_Base))) {
		/* Do nothing */
	}	
#else
	while (!SIO_TX_READY(SIO_GET_TX_STATUS(FDL_GetUARTBaseAddr()))) {
        /* Do nothing */
    }      
	SIO_PUT_CHAR(FDL_GetUARTBaseAddr(), c);

    /* Ensure the last byte is written successfully */
	while (!SIO_TX_READY(SIO_GET_TX_STATUS(FDL_GetUARTBaseAddr()))) {
		/* Do nothing */
	}	
#endif
}

unsigned char sio_get_char(void)
{  
#ifndef CHIP_VER_8800H5
    while (!SIO_RX_READY(SIO_GET_RX_STATUS(ARM_UART0_Base))) {
        /* Do nothing */
    }
    return SIO_GET_CHAR(ARM_UART0_Base);
#else
    while (!SIO_RX_READY(SIO_GET_RX_STATUS(FDL_GetUARTBaseAddr()))) {
        /* Do nothing */
    }
    return SIO_GET_CHAR(FDL_GetUARTBaseAddr());
#endif
}
