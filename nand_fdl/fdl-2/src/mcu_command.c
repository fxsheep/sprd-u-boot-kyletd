#include "mcu_command.h"
#include "packet.h"
#include "fdl_nand.h"

typedef void (*BOOT_ENTRY)(void);

/* Sorry, I don't know what this function is for.
 * 
 * Ming.Zhang 2005-09-22
 */
static void reset_mcu_clock(void)
{
#ifdef MCU_CLK_52M
#define REG(r)		(*((volatile unsigned int*)(r)))
		
    REG(0x8b000018) |= 1 << 9; // write pll enable
    REG(0x8b000024)  = 0x1E05; // M/N = 5, so clock = 78M
    REG(0x8b000018) &= ~(1 << 9); // write pll disable
    
    REG(GR_PCTL) = (unsigned int)(0x0A55);
#endif /* MCU_CLK_52M */
}

int mcu_reset_boot(PKT_HEADER *pakcet, void *arg)
{
	int i;
    BOOT_ENTRY boot_entry = (BOOT_ENTRY)0; /* The address of ROM Code */
    
    send_ack_packet(BSL_REP_ACK);
    
    /* Wait until all characters are sent out. */
    for (i=0; i<0x0A000; i++) {
    	/* Do nothing */;
    }
   
   	reset_mcu_clock();
   	
   	/* Jump to ROM code */
    (*boot_entry)();/*lint !e413*/
    
    /* We should not go here */
    return 0;
}

int mcu_reset_normal(PKT_HEADER *packet, void *arg)
{
    int i;
    BOOT_ENTRY boot_entry = (BOOT_ENTRY)0x40000000; /* Start of internal RAM */
#if 0    
    /* Copy NBL to internal RAM */
    if (NAND_SUCCESS != nand_read_NBL((void*)boot_entry)) {
    	send_ack_packet(BSL_REP_OPERATION_FAILED);
    	return 0;
    }
#endif    
    send_ack_packet(BSL_REP_ACK);
    
    /* Wait until all characters are sent out. */
    for (i=0; i<0x0A000; i++) {
    	/* Do nothing */;
    }
    
    reset_mcu_clock();
    
    /* Jump to NBL */
    (*boot_entry)();
    
    /* We should not go here */
    return 0;  
}

int mcu_read_chip_type(PKT_HEADER *packet, void *arg)
{
	unsigned int id;

#ifndef _SC6600C1
	id   =  *(unsigned int *)(0x4FFC);
	if (0x660000B6 != id)
	{
		// @Richard We should check if we want to support 6600C
		id = 0x6600b500;
	}
	else
	{
		id = 0x6600b700;
	}
#else
	id = 0x6600D100;	
#endif   
	
	packet->type = BSL_REP_READ_CHIP_TYPE;
    packet->size = 4;       
    memcpy(packet + 1, &id, sizeof(unsigned int));
    
    send_packet(packet);
    return 1;
}
