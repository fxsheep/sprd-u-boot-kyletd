#include "sci_types.h"
#include "fdl_command.h"
#include "fdl_main.h"
#include "fdl_stdio.h"
#include "fdl_conf.h"
#include "packet.h"
#include "sio_api.h"
#include "usb_boot.h"

extern void JumpToTarget(unsigned long addr);

typedef struct _DL_FILE_STATUS {	
	unsigned long start_address;
	unsigned long total_size;
	unsigned long recv_size;
	unsigned long next_address;
} DL_FILE_STATUS, *PDL_FILE_STATUS;

static DL_FILE_STATUS g_file;


int sys_connect(PKT_HEADER *packet, void *arg)
{
	send_ack_packet(BSL_REP_ACK);
	return 1;
}

int data_start(PKT_HEADER *packet, void *arg)
{
	unsigned long *data = (unsigned long*)(packet + 1);
    unsigned long start_addr = *data;
    unsigned long file_size = *(data + 1);

    if ((start_addr < MEMORY_START) || (start_addr >= MEMORY_START + MEMORY_SIZE)) {
        send_ack_packet(BSL_REP_DOWN_DEST_ERROR);
        return 0;
    }
    
    if ((start_addr + file_size) > (MEMORY_START + MEMORY_SIZE)) {
        send_ack_packet(BSL_REP_DOWN_SIZE_ERROR);
        return 0;
    }

	g_file.start_address = start_addr;
	g_file.total_size = file_size;
	g_file.recv_size = 0;
	g_file.next_address = start_addr;   

	FDL_memset((void*)start_addr, 0, file_size);
    send_ack_packet(BSL_REP_ACK);
    return 1;
}

int data_midst(PKT_HEADER *packet, void *arg)
{
    unsigned short data_len = packet->size;

	if ((g_file.recv_size + data_len) > g_file.total_size) {
        send_ack_packet(BSL_REP_DOWN_SIZE_ERROR);
        return 0;
    }
    
    FDL_memcpy((void *)g_file.next_address, (const void*)(packet + 1), data_len);   
  	g_file.next_address += data_len;
   	g_file.recv_size += data_len;   

   	send_ack_packet(BSL_REP_ACK);
   	return 1;
}

int data_end(PKT_HEADER *packet, void *arg)
{
	send_ack_packet(BSL_REP_ACK);
	return 1;
}

int data_exec(PKT_HEADER *packet, void *arg)
{

  	//BSL_REP_ACK is sent in the begin of FDL2
    	
	JumpToTarget(g_file.start_address);
	
	return 0;
}

int set_baudrate(PKT_HEADER *packet, void *arg)
{
    unsigned long baudrate = *(unsigned long*)(packet + 1);
    int			  i;
    
    send_ack_packet(BSL_REP_ACK); 
    
    //modify the baudrate ONLY in uart boot mode
    if(fdl_isuartboot())
    {   
	    /* Wait until all characters are sent out */
	    for (i=0; i<1000; ++i) {
	    	/* Do nothing */
	    }
	    
	    sio_set_baudrate(baudrate);
    }
    return 1;
}

