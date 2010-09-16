/*
** Copyright (C) Spreadtrum Ltd, 2003. All rights reserved.
*/


/*
** This implements a 'retarget' layer for low-level IO.  Typically, this
** would contain your own target-dependent implementations of fputc(),
** ferror(), etc.
** 
** This example provides implementations of fputc(), ferror(),
** _sys_exit(), _ttywrch() and __user_initial_stackheap().
**
** Here, semihosting SWIs are used to display text onto the console 
** of the host debugger.  This mechanism is portable across ARMulator,
** Angel, Multi-ICE and EmbeddedICE.
**
** Alternatively, to output characters from the serial port of an 
** ARM Integrator Board (see serial.c), use:
**
**
*/

#include <stdio.h>
#include <rt_misc.h>

#ifdef __thumb
/* Thumb Semihosting SWI */
#define SemiSWI 0xAB
#else
/* ARM Semihosting SWI */
#define SemiSWI 0x123456
#endif


/* Write a character */ 
__swi(SemiSWI) void _WriteC(unsigned op, char *c);
#define WriteC(c) _WriteC (0x3,c)

/* Exit */
__swi(SemiSWI) void _Exit(unsigned op, unsigned except);
#define Exit() _Exit (0x18,0x20026)


struct __FILE { int handle;   /* Add whatever you need here */};
FILE __stdout;



/*
	write char through Tube
*/
/*extern void sio_senchar(char ch);
void sendchar( char * ch_ptr)
{
	
	
	//	if we don't send console, disable it
	
	//WriteC( ch_ptr );
	sio_sendchar(*ch_ptr);
}


int fputc(int ch, FILE *f)
{
    // Place your implementation of fputc here     
    // e.g. write a character to a UART, or to the 
    // debugger console with SWI WriteC            
    char tempch = ch;
#if 0
#ifdef USE_TUBE
    sendchar( &tempch);
#else
    WriteC( &tempch );
#endif

#endif // #if 0
	sendchar(&tempch);
    return ch;
}
*/

int ferror(FILE *f)
{  
 /* Your implementation of ferror */
    return EOF;
}


void _sys_exit(int return_code)
{
    Exit();         /* for debugging */

label:  goto label; /* endless loop */
}

/*
void _ttywrch(int ch)
{
#if 0
    char tempch = ch;
#ifdef USE_TUBE
    sendchar( &tempch );
#else
    WriteC( &tempch );
#endif
#endif
}
*/
// If RAM_LIMIT, this value MUST < RAM_Limit(defined in init.s)
//#define RAM_LIMIT   0x40100000

__value_in_regs struct __initial_stackheap __user_initial_stackheap(
        unsigned R0, unsigned SP, unsigned R2, unsigned SL)
{
    struct __initial_stackheap config;
    
        
//    config.heap_base = RAM_LIMIT - 16 * 1024; // Heap base placed at top of SRAM
    
    config.stack_base = SP;

    return config;
}

