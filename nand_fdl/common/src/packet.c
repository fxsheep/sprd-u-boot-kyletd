#include "sci_types.h"
#include "fdl_conf.h"
#include "fdl_main.h"
#include "packet.h"
#include "fdl_crc.h"
#include "sio_drv.h"  /* For efficiency, we access UART hardware directly. */
#include "usb_boot.h"
#include "nand_ext.h"

#define HDLC_FLAG               0x7E
#define HDLC_ESCAPE             0x7D
#define HDLC_ESCAPE_MASK        0x20

typedef enum
{
    PKT_NONE = 0,
    PKT_HEAD,
    PKT_GATHER,
    PKT_COMPLETED
} PKT_STATE; 

#define DATA_SIZE		(sizeof(PKT_HEADER) + MAX_PKT_SIZE + 2)
typedef struct _UNIT {
	union {
		struct {
    		struct _UNIT * next;    
    		int state;  /* used by recv handle, ref PKT_STATE. */
    		int size;
    	} s;
    	unsigned long algin;
    } header;
    unsigned char data[DATA_SIZE];
} UNIT, *PUNIT;


static UNIT   g_pool[MAX_UNIT_NUM];

static UNIT * g_freeptr = 0;
static UNIT * g_completedptr = 0;
static UNIT * g_currptr = 0;

static void write_packet(const void * buf, int len);
uint32 system_count_get(void);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
__inline unsigned char usb_get_char(void)
{
	return VCOM_GetChar();
}

void packet_init( void )
{
	int i;
	memset(g_pool, 0, sizeof g_pool);
    
    g_freeptr = &g_pool[0];
    g_completedptr = 0;
    g_currptr = 0;
    
    for (i=0; i<MAX_UNIT_NUM - 1; ++i) {/*lint !e681 */
    	g_pool[i].header.s.next = &g_pool[i+1];
    }
    g_pool[MAX_UNIT_NUM - 1].header.s.next = 0;
}


PKT_HEADER * malloc_packet(unsigned long size)
{
    if ((size <= MAX_PKT_SIZE) &&(0 != g_freeptr)) {
        UNIT * p = g_freeptr;
        g_freeptr = g_freeptr->header.s.next;
                
        p->header.s.next = 0;            
        p->header.s.state = PKT_NONE;
        p->header.s.size  = 0;
        
        return (PKT_HEADER*)p->data;
    }
    return 0;
}

void free_packet(PKT_HEADER * ptr)
{       
	UNIT * p = (UNIT*)((unsigned char*)ptr - (unsigned long)&((UNIT*)0)->data);/*lint !e413 */
    p->header.s.next = g_freeptr;
    g_freeptr = p;
}

PKT_HEADER * get_packet(void)
{
    UNIT * p;
    
    while (0 == g_completedptr)
        peek_packet();
    
    p = g_completedptr;        
    g_completedptr   = g_completedptr->header.s.next;
    p->header.s.next = 0;
    return (PKT_HEADER*)p->data;
}

void send_packet(PKT_HEADER * p)
{
	unsigned short size = p->size + sizeof(PKT_HEADER); 
    
    if(fdl_isuartboot())
    {
        unsigned short crc = frm_chk((unsigned short*)p, size);
	    unsigned char * pcrc = (unsigned char*)p + size;
	    *pcrc++ = (crc >> 8) & 0xFF;
	    *pcrc = crc & 0xFF;   
    }
    else{
        //do crc check in usb write
    }
    write_packet(p, size + sizeof(unsigned short));
}

void send_ack_packet(DLSTATUS status)
{
	/* An acknowledge packet just contains three short-wide fields:
	 * pkt_type, pkt_size, crc
	 *///total ack length is 8byte -- 7e xx xx xx xx xx xx 7e
	unsigned short ack_packet[5]; 
    PKT_HEADER * p = (PKT_HEADER*)ack_packet;
    p->type = status;
    p->size = 0;
    
    send_packet(p);
}

int peek_packet( void )
{
    unsigned char c;
    unsigned char * pdata = 0;
	UNIT *temptr = NULL;
	uint32 size = 0;
	
    if ( 0 == g_currptr ) {
        PKT_HEADER * tmp = malloc_packet(MAX_PKT_SIZE);
        if (0 == tmp)
        	return 0;
        /* It looks silly, but we need the UNIT header. */
        g_currptr = (UNIT*)((unsigned char*)tmp - (unsigned long)&((UNIT*)0)->data);/*lint !e413 */
    }

    pdata = g_currptr->data + g_currptr->header.s.size;
    temptr = g_currptr;

    //-------------uart boot mode ------------------------//
   	if(fdl_isuartboot())
   	{
   	    while (1) {
	    	if ((unsigned int)temptr->header.s.size >= DATA_SIZE) {

	    		/* An invalid packet whose size has exceeded the pre-defined
	    		 * maximum size.
	    		 * Drop it and notify the PC
	    		 */
	    		free_packet((PKT_HEADER*)temptr->data);
	    		g_currptr = 0;
	        	send_ack_packet(BSL_REP_VERIFY_ERROR);
	        	return -1;
	    	}
	    	
	    	c = sio_get_char();

	    	switch (temptr->header.s.state) {
		    	case PKT_NONE:
		        	if (HDLC_FLAG == c) {
		            	temptr->header.s.state = PKT_HEAD;
		            	temptr->header.s.size = 0;
		        	}
		       		break;
		       		
		    	case PKT_HEAD: /* Skip multiple HDLC_FLAG */
		        	if (HDLC_FLAG != c) {
		            	if (HDLC_ESCAPE == c)
		            	{           
		                	/* Try to get the "true" data. */
					    	c = sio_get_char()^ HDLC_ESCAPE_MASK;
		            	}
		            	temptr->header.s.state = PKT_GATHER;
		            	*pdata++ = c;
		            	++temptr->header.s.size;
		        	}
		        	break;
		        	
		    	case PKT_GATHER:
		        	if (HDLC_FLAG == c) {
		            	/* Compute the crc of the received packet. */
		            	unsigned short crc = frm_chk((unsigned short *)temptr->data, 
		            		temptr->header.s.size);
		            	if (0 != crc) {  
		                	/* crc error, so this packet is dropped. */
		                	free_packet((PKT_HEADER*)temptr->data);
		                	g_currptr = 0;     
		                	send_ack_packet(BSL_REP_VERIFY_ERROR);
		                	return -1;
		            	} else {
		                	/* A complete packet has been received. Add it to the tail of
		                	 * the completed list.
		                	 */
		                	temptr->header.s.next = 0;
		                	temptr->header.s.state = PKT_COMPLETED;
		                	if (0 == g_completedptr) {
		                    	g_completedptr = temptr;
		                	} else {
		                    	UNIT * p = g_completedptr;
		                    	while (0 != p->header.s.next)
		                    		p = p->header.s.next;
		                    	p->header.s.next = temptr;
		                	}
		                	g_currptr = 0;

		                	return 1;
		            	}
		        	} else {
		            	if (HDLC_ESCAPE == c)
		            	{   
					    	c = sio_get_char()^ HDLC_ESCAPE_MASK;
		            	}
		            	*pdata++ = c;
		            	++temptr->header.s.size;
		        	}
		        	break;
		        	
		    	default:
		    		break;
	    	}
    	}
    }
   	else if(fdl_isusbboot())
   	{
	    while (1) {
	    	if ((unsigned int)temptr->header.s.size >= DATA_SIZE) {

	    		/* An invalid packet whose size has exceeded the pre-defined
	    		 * maximum size.
	    		 * Drop it and notify the PC
	    		 */
	    		free_packet((PKT_HEADER*)temptr->data);
	    		g_currptr = 0;
	        	send_ack_packet(BSL_REP_VERIFY_ERROR);
	        	return -1;
	    	}
	    	
	        c = usb_get_char();

	    	switch (temptr->header.s.state) {
		    	case PKT_NONE:
		        	if (HDLC_FLAG == c) {
		            	temptr->header.s.state = PKT_HEAD;
		            	//temptr->header.s.size = 0;
		            	size = 0;
		        	}
		        	        	
    	       		c = usb_get_char();
		       		
		    		/*lint -save -e616 -e825*/	
		    	case PKT_HEAD: //* Skip multiple HDLC_FLAG */
		    	       /*lint -restore */
		        	if (HDLC_FLAG != c) {
		            	if (HDLC_ESCAPE == c) {           
		                	/* Try to get the "true" data. */
				            c = usb_get_char()^HDLC_ESCAPE_MASK;
		            	}
		            	temptr->header.s.state = PKT_GATHER;
		            	*pdata++ = c;
		            	size++;
		        	}
		        	
		        	c = usb_get_char();
			/*lint -save -e616 -e825*/
		    	case PKT_GATHER:
			/*lint -restore  */
		    	    //what we got is not the end flag
		    	    while(HDLC_FLAG != c)
		    	    {
		            	if(HDLC_ESCAPE == c)
		            	{   
				           c = usb_get_char()^HDLC_ESCAPE_MASK;
		            	}
		            	*pdata++ = c;
		            	size++;
		            	c = usb_get_char();
		    	    }
		    	    
		    	    //run here, indicated that we got the HDLC FLAG
		    	    {
		    	        unsigned short crc;

		    	        temptr->header.s.size = size;

		            	/* Compute the crc of the received packet. */
		            	crc = frm_chk((unsigned short *)temptr->data, 
		            		temptr->header.s.size);
		            	
		            	if (0 != crc) {  
		                	/* crc error, so this packet is dropped. */
		                	free_packet((PKT_HEADER*)temptr->data);
		                	g_currptr = 0;     
		                	send_ack_packet(BSL_REP_VERIFY_ERROR);
		                	return -1;
		            	} else {
		                	/* A complete packet has been received. Add it to the tail of
		                	 * the completed list.
		                	 */
		                	temptr->header.s.next = 0;
		                	temptr->header.s.state = PKT_COMPLETED;

		                    g_completedptr = temptr;
		                	g_currptr = 0;

		                	return 1;
		            	}
		    	    }
		
		        	break;/*lint !e527*/
		    	default:
		    		break;
	    	}
	    }
   	}
   	else
   	{
   	}
   	
   	return 1;
   	
}

/******************************************************************************
 * write_packet
 ******************************************************************************/
void write_packet(const void * buf, int len)
{
	const unsigned char * pstart = (const unsigned char*)buf;
	const unsigned char * pend = pstart + len;
		
	if(fdl_isuartboot() )
	{	
		sio_put_char(HDLC_FLAG);
		while (pstart<pend) {
			if ((HDLC_FLAG == *pstart) || (HDLC_ESCAPE == *pstart)) {
				sio_put_char(HDLC_ESCAPE);
				sio_put_char(~HDLC_ESCAPE_MASK & *pstart);
			} else {
				sio_put_char(*pstart);
			}
			++pstart;
		}
		sio_put_char(HDLC_FLAG);
	}
	else if(fdl_isusbboot())
	{
        usb_write ((char*)buf,len);
	}
	else
	{
	}
}


