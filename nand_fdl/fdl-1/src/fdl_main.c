/******************************************************************************
 ** File Name:      FDL_main.c                                                 *
 ** Author:         weihua.wang                                                  *
 ** DATE:           27/06/2005                                               *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "fdl_stdio.h"
#include "fdl_command.h"
#include "fdl_conf.h"
#include "fdl_main.h"
#include "cmd_def.h"
#include "packet.h"
#include "dl_engine.h"
#include "sio_api.h"
#include "sdram_init.h"
#include "usb_boot.h"
#include "chip_init.h"
#include "virtual_com.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define AHB_72MHZ        72000000
#define AHB_80MHZ        80000000
//BOOT mode Strapping Pin value
#if defined (NAND_FDL_SC6600I) || defined (NAND_FDL_SC6800D)
#define NAND_BOOT_MODE                0x0
#define USB_BOOT_MODE                 0x1
#define UART_BOOT_MODE  		      0x2
#define USB_BOOT_MODE_2               0x3
#define MAX_BOOT_MODE				  USB_BOOT_MODE_2	
#else
#define NAND_BOOT_MODE                0x3
#define USB_BOOT_MODE                 0x2
#define UART_BOOT_MODE  		      0x1
#define USB_BOOT_MODE_2               0x0
#define MAX_BOOT_MODE				  NAND_BOOT_MODE	
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
unsigned char stack_space[STACK_SIZE]={0};

const unsigned long stack_bottom = ((unsigned long)&stack_space + STACK_SIZE);

static const char VERSION_STR[] = {"Spreadtrum Boot Block version 1.1"};

#ifdef CHIP_VER_8800H5
LOCAL uint32 s_fdl_uart_addr_base	= ARM_UART0_BASE;
#endif
static uint32 s_fdl_bootmode;
/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/
static void Set_AHBClk(uint32 ahb_clk);

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
static void error(void)
{
	sio_putstr("The first FDL failed!\r\n");
	for (;;) /*Do nothing*/;
}

uint32 system_count_get(void)
{
	volatile uint32 clock;
	volatile uint32 clock_c;
	
	//read two times all the same to make sure
	clock   = *(volatile uint32 *)(0x8700001c);
	clock_c = *(volatile uint32 *)(0x8700001c);
	
	while (clock != clock_c)
	{
		clock   = *(volatile uint32 *)(0x8700001c);
		clock_c = *(volatile uint32 *)(0x8700001c);	
	}
	
	return (clock_c);
}

void system_delay(uint32 delay_ms)
{
	uint32 tmp;
	
	tmp = system_count_get() + delay_ms;
	
	while(tmp > system_count_get());
}

static void Set_AHBClk(uint32 ahb_clk)
{
#if defined (NAND_FDL_SC6600I)
    unsigned long ext_clk,baud_rate;
    
    ext_clk=*(volatile unsigned long *)0x8b000018;
    
    if(ext_clk &(0x1<<15))
    {
      if(ahb_clk == AHB_72MHZ)
        *(volatile uint32 *)0x8b00000c = 0x11;     // 144/2 = 72
    }
    else
    {
      if(ahb_clk == AHB_72MHZ)
       *(volatile uint32 *)0x8b00000c = 0x10;      // 72/1 = 72
    }
#elif defined (NAND_FDL_SC6600R)
    uint32 i;
    //usb 48MHZ/12MHZ div from upll
   *(volatile uint32 *)0x8b000044 |=  BIT_20|BIT_21;
    for(i=0;i<100;i++);
    *(volatile uint32 *)GR_GEN1 |= (0x1<<9);
    //Set pll clk
    *(volatile uint32 *)GR_MPLL_MN = 0x27000d;
    //Disable pll_mn
    *(volatile uint32 *)GR_GEN1 &= ~(0x1<<9);    
    //Delay some time
    for(i=0;i<100;i++);
    *(volatile uint32 *)0x8b00000c = 0x00;    
#elif defined (NAND_FDL_SC8800H)
     uint32 i;
	// Config MPLL clock
      *(volatile uint32 *)GR_GEN1 |= BIT_9;
    
      *(volatile uint32 *) GR_MPLL_MN = 0xA0000D;

    // Delay a little time
    for( i=0; i<100; i++);
        
      *(volatile uint32 *)GR_GEN1 &= ~BIT_9;
  
    *(volatile uint32 *)(AHB_AHB_ARM_CLK) = ( (1) | ((3)<<5) | ((1)<<10) | ((1)<<15));

    // Delay a little time
    for( i=0; i<100; i++);
#endif
}

BOOLEAN fdl_isuartboot(void)
{
    if(UART_BOOT_MODE == s_fdl_bootmode)
    {
		return TRUE;
    }
    else
    {
		return FALSE;
    }
}

BOOLEAN fdl_isusbboot(void)
{
    if(USB_BOOT_MODE == s_fdl_bootmode)
    {
		return TRUE;
    }
    else
    {
		return FALSE;
    }
}
void fdl_setbootmode(uint32 mode)
{
	if(mode > MAX_BOOT_MODE)
        return;
        
    s_fdl_bootmode = mode;
}

#ifdef CHIP_VER_8800H5
uint32 FDL_GetBootMode(void)
{	
    return s_fdl_bootmode;
}

void _FDL_UpdateBootMode(void)
{        
   	uint32 reg_value = 0;
   	
	// Check current boot mode
	//reg_value = (*(volatile uint32 *) GR_HWRST >> 8) & 0xff;//read HWRST reg bit[15:8]
	reg_value = (*(volatile uint32 *) (0x8B000020) >> 8) & 0xff;//read HWRST reg bit[15:8]
	if(reg_value == 0x5a)
	{
		s_fdl_bootmode = USB_BOOT_MODE;
	}
	else
	{
		s_fdl_bootmode = UART_BOOT_MODE;

		if(0x6A == reg_value)
		{
			s_fdl_uart_addr_base = ARM_UART1_BASE;
		}
	}

}

uint32 FDL_GetUARTBaseAddr(void)
{
	return s_fdl_uart_addr_base;
}
#endif

int main(void)
{	
	uint32 i;
	PKT_HEADER 		*packet;	
	uint32 boot_mode; 
    uint32  ext_clk_26M;
	
#if defined (NAND_FDL_SC6800D)
	*(volatile uint32 *)0x20900218 |= 0x1;	
	for(i =0; i< 100; i++);	
#elif defined (NAND_FDL_SC8800H)
	*(volatile uint32 *)0x20900218 |= 0x1;	
	for(i =0; i< 100; i++);	
    Set_AHBClk(AHB_80MHZ);
#else
	//Set AHB_clk 72Mhz
    Set_AHBClk(AHB_72MHZ);
#endif

    Chip_Init();

    // Check current boot mode 
#ifndef CHIP_VER_8800H5
	boot_mode = *(volatile unsigned long *) 0x8b000018;
	boot_mode = (boot_mode >> 25) & 0x3;
	if((*(volatile uint32 *)0x8b000020) == 0x5A)
	{
			boot_mode = USB_BOOT_MODE;
  }
    fdl_setbootmode(boot_mode);
#else
    _FDL_UpdateBootMode();

    // Check current boot mode 
    boot_mode = FDL_GetBootMode();
#endif

	packet_init();
	
	dl_init();
	dl_reg(BSL_CMD_CONNECT,    sys_connect,  0);
	dl_reg(BSL_CMD_START_DATA, data_start,   0);
   	dl_reg(BSL_CMD_MIDST_DATA, data_midst,   0);
   	dl_reg(BSL_CMD_EXEC_DATA,  data_exec,    0);
   	dl_reg(BSL_CMD_END_DATA,   data_end,     0);
   	dl_reg(BSL_SET_BAUDRATE,   set_baudrate, 0);
	
	switch(boot_mode){
	    case USB_BOOT_MODE:
	    {
			// Enable internal RAM for ARM
			*(volatile unsigned long *) 0x8b00002C |= 0x7;

        	//Open system counter
	        *(volatile uint32 *)0x8b000018|=(0x1<<13);

           //Read from strapping pin to know whether external clock is 26Mhz or 13Mhz
            ext_clk_26M = *(volatile unsigned long *)0x8b000018;
            ext_clk_26M = (ext_clk_26M>>15) & 0x1;	
	     
	        //for usb is ldo off, device is not ready, we can send nothing
            usb_boot (ext_clk_26M);
            //shake with pc,wait the sig of 0x7e,wait until 
            while(1)
            {
                if(0x7e == VCOM_GetChar())
                {
                    break;
                }
            }
	    	break ;
	    }
	    case NAND_BOOT_MODE:
            fdl_setbootmode(UART_BOOT_MODE);
	    {
            while(1)
            {    
#ifndef CHIP_VER_8800H5
                while (!(*(volatile uint32*)(0x8300000c)&0xff)) {
               /* Do nothing */
                }
                
                if(0x7e ==  (*(volatile uint32*)(0x83000004)&0xff))
                {
                    break;
                }
#else
                while (!(*(volatile uint32*)(FDL_GetUARTBaseAddr() +ARM_UART_STS1)&0xff)) {
               /* Do nothing */
                }
                
                if(0x7e ==  (*(volatile uint32*)(FDL_GetUARTBaseAddr()+ARM_UART_RXD)&0xff))
                {
                    // clear uart rx fifo
                    while((*(volatile uint32*)(FDL_GetUARTBaseAddr() +ARM_UART_STS1)&0xff))
                    {
                        uint32 value;
                        value = *(volatile uint32*)(FDL_GetUARTBaseAddr() +ARM_UART_RXD);
                    }
                    break;
                }
#endif
            }
        	break;	    
	    }
	    case UART_BOOT_MODE:
	    {
            while(1)
            {    
#ifndef CHIP_VER_8800H5
                while (!(*(volatile uint32*)(0x8300000c)&0xff)) {
               /* Do nothing */
                }
                
                if(0x7e ==  (*(volatile uint32*)(0x83000004)&0xff))
                {
                    break;
                }
#else
                while (!(*(volatile uint32*)(FDL_GetUARTBaseAddr() +ARM_UART_STS1)&0xff)) {
               /* Do nothing */
                }
                
                if(0x7e ==  (*(volatile uint32*)(FDL_GetUARTBaseAddr()+ARM_UART_RXD)&0xff))
                {
                    // clear uart rx fifo
                    while((*(volatile uint32*)(FDL_GetUARTBaseAddr() +ARM_UART_STS1)&0xff))
                    {
                        uint32 value;
                        value = *(volatile uint32*)(FDL_GetUARTBaseAddr() +ARM_UART_RXD);
                    }
                    break;
                }
#endif
            }
        	break;	    
	    }
	    case USB_BOOT_MODE_2:
			break;
    	default:
        	break;
	}

    //when got the signal of 0x7e ,send response to pc
	packet = malloc_packet(sizeof(VERSION_STR));
	packet->type = BSL_REP_VER;
	packet->size = sizeof(VERSION_STR);
	FDL_memcpy(packet + 1, VERSION_STR, sizeof(VERSION_STR));
	send_packet(packet);
	free_packet(packet);
    
	dl_entry(DL_STAGE_NONE);
   	
   	error();
   	return 1;
}

