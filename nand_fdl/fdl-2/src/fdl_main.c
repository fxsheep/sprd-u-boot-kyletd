#include "sci_types.h"
#include "fdl_main.h"
#include "cmd_def.h"
#include "packet.h"
#include "dl_engine.h"
#include "fdl_nand.h"
#include "sio_api.h"
#include "flash_command.h"
#include "mcu_command.h"
#include "usb_boot.h"
#include "dma_drv_fdl.h"
#ifdef CHIP_VER_8800H5
#include "sc8800h_reg_uart.h"
#endif
#include <config.h>
#include <common.h>
#include <serial.h>
#include <nand.h>

extern unsigned long _armboot_start;

extern  const unsigned char FDL2_signature[][24];

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

static uint32 s_fdl_bootmode;
#ifdef CHIP_VER_8800H5
LOCAL uint32 s_fdl_uart_addr_base	= ARM_UART0_BASE;
#endif

#if 0
static void error(void)
{
	sio_putstr("The second FDL failed!\r\n");
	while(1)/*Do nothing*/;
}
#endif

#ifndef NAND_FDL_SC8800D
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

#endif

DECLARE_GLOBAL_DATA_PTR;

int main(void)
{
	/* All hardware initialization has been done in the 1st FDL,
	 * so we don't do initialization stuff here.
	 * The UART has also been opened by the 1st FDL and the baudrate
	 * has been setted correctly.
	 */  
	int err;
	uint32 boot_mode; 
    uint32 ext_clk_26M;
	uint32 sigture_address;
	
	sigture_address = (uint32)FDL2_signature;

#if !defined(NAND_FDL_SC8800D)
#ifndef CHIP_VER_8800H5
    // Check current boot mode 
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
#endif
	
    packet_init();

	//add to migrate uboot nand flash wupport
	mem_malloc_init (_bss_end,
							CONFIG_SYS_MALLOC_LEN);
	timer_init();
#if (!defined(NAND_FDL_SC6800D)) && (!defined(NAND_FDL_SC8800H))
	dma_init();
#endif	

	do {
		/* Initialize NAND flash. */
		err = nand_flash_init();
		if ((NAND_SUCCESS != err) && (NAND_INCOMPATIBLE_PART != err)) {
   			send_ack_packet(convert_err(err));
        	break;
   		}
   		
   		/* Register command handler */
   		dl_init();

  		dl_reg(BSL_CMD_START_DATA,     data_start,         0);
   		dl_reg(BSL_CMD_MIDST_DATA,     data_midst,         0);
   		dl_reg(BSL_CMD_END_DATA,       data_end,           0);
   		dl_reg(BSL_CMD_READ_FLASH,     read_flash,         0);
   		dl_reg(BSL_ERASE_FLASH,        erase_flash,        0);
   		dl_reg(BSL_CMD_NORMAL_RESET,   mcu_reset_normal/*mcu_reset_boot*/,   0);
    	dl_reg(BSL_CMD_READ_CHIP_TYPE, mcu_read_chip_type, 0);  
    	dl_reg(BSL_REPARTITION,    	   format_flash,       0);	

#if !defined(NAND_FDL_SC8800D)
		switch(boot_mode){
		    case NAND_BOOT_MODE:
		        {
	                fdl_setbootmode(UART_BOOT_MODE);

		   	    	send_ack_packet(NAND_SUCCESS == err ? BSL_REP_ACK :
		   											  BSL_INCOMPATIBLE_PARTITION);
	  
	        		/* Start the download process. */
	   	        	dl_entry(DL_STAGE_CONNECTED);

		        	break;	    
	        	}
		    case USB_BOOT_MODE:
		    {
                	usb_varinit();

	    		/* Reply the EXEC cmd received in the 1st FDL. */
	   	    	send_ack_packet(NAND_SUCCESS == err ? BSL_REP_ACK :
	   											  BSL_INCOMPATIBLE_PARTITION);
  
        		/* Start the download process. */
   	        	dl_entry(DL_STAGE_CONNECTED);

	        	break;	    
		    }
		    case UART_BOOT_MODE:
		    {
	    		/* Reply the EXEC cmd received in the 1st FDL. */
	   	    	send_ack_packet(NAND_SUCCESS == err ? BSL_REP_ACK :
	   											  BSL_INCOMPATIBLE_PARTITION);
  
        		/* Start the download process. */
   	        	dl_entry(DL_STAGE_CONNECTED);

	        	break;	    
		    }
		    case USB_BOOT_MODE_2:
	    	default:
	        	break;
		}
   		
   	} while (0);
#else /* 8800d pattern */
		/* Reply the EXEC cmd received in the 1st FDL. */
   		send_ack_packet(NAND_SUCCESS == err ? BSL_REP_ACK :
   											  BSL_INCOMPATIBLE_PARTITION);
   		/* Start the download process. */
   		dl_entry(DL_STAGE_CONNECTED);
#endif
   	/* If we get here, there must be something wrong. */
   	error();
   	return 0;
}

