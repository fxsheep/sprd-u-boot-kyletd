#include <config.h>
#include <common.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#include <linux/string.h>
#include <android_bootimg.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <nand.h>
#include <android_boot.h>
#include <environment.h>
#include <jffs2/jffs2.h>
#include <boot_mode.h>
#include <malloc.h>
#include "cmd_def.h"
#include "packet.h"

//#define __DEBUG__

#ifdef __DEBUG__
#define boot_debug printf
#else
#define boot_debug 
#endif

typedef enum _DL_PACKET_TYPE{
	BSL_UART_PACKET,
	BSL_SPI_PACKET
}BSL_PACKET_E;

#define SPI_PORT	0

#ifndef __SPI_MODE__
#define hs_channel_write	sdio_channel_write
#define hs_channel_read		sdio_channel_read
#else
#define hs_channel_write	SPI_channel_write
#define hs_channel_read		SPI_channel_read
#endif
#define TEST_LENGTH  (1024*32)
char    test_buffer[TEST_LENGTH]={0};

extern int hs_channel_write(unsigned char *buffer,int len);
extern int hs_channel_read(unsigned char *buffer,int len);
extern int uart_write(char *buffer,int size);
extern int uart_read(char *buffer,int size);
extern void req_clk_init(void);
static unsigned long send_buffer[1024*8+64]={0};

/******************************************************************************
**  Description:    This function scan the data in src buffer ,add 0x7e as begin
**                  and end of src,at the same time replace 0x7E,0x7D with rules
**                  below: 
**                        0x7e is replaced by 0x7d and 0x5e;
**                        0x7D is replace 0x7d and 0x5d
**                  the translated data will be save in dest buffer
**  Author:         jiayong.yang
**  parameter:      dest is buffer result of process
**                  src is buffer where data is saved
**                  size is size of src data. 
******************************************************************************/
static int translate_packet(char *dest,char *src,int size)
{
	int i;
	int translated_size = 0;
	
	dest[translated_size++] = 0x7E;
	
	for(i=0;i<size;i++){
		if(src[i] == 0x7E){
			dest[translated_size++] = 0x7D;
			dest[translated_size++] = 0x5E;
		} else if(src[i] == 0x7D) {
			dest[translated_size++] = 0x7D;
			dest[translated_size++] = 0x5D;			
		} else 
			dest[translated_size++] = src[i];
	}	
	dest[translated_size++] = 0x7E;
	return translated_size; 
}
/******************************************************************************
**  Description:    This function scan the data in src buffer ,add 0x7e as begin
**                  and end of src,at the same time replace 0x7E,0x7D with rules
**                  below: 
**                        0x7e is replaced by 0x7d and 0x5e;
**                        0x7D is replace 0x7d and 0x5d
**                  the translated data will be save in dest buffer
**  Author:         jiayong.yang
**  parameter:      dest is buffer result of process
**                  src is buffer where data is saved
**                  size is size of src data. 
******************************************************************************/
static int untranslate_packet(char *dest,char *src,int size)
{
	int i;
	int translated_size = 0;
	int status = 0;
	for(i=0;i<size;i++){
		switch(status){
			case 0:
				if(src[i] == 0x7e)
					status = 1;
			break;
			case 1:
				if(src[i] != 0x7e)
				{
					status = 2;
					dest[translated_size++] = src[i];
				}
			break;
			case 2:
				if(src[i] == 0x7E){
					unsigned short crc;
					crc = crc_16_l_calc((char const *)dest,translated_size-2);
					return translated_size;
				}else if(dest[translated_size-1] == 0x7D){
					if(src[i] == 0x5E){
						dest[translated_size-1] = 0x7E;
					} else if(src[i] == 0x5D) {
						dest[translated_size-1] = 0x7D;
					}
				}else {
					dest[translated_size++] = src[i];
				}
			break;
		}
	}
	return translated_size;
}
/******************************************************************************
**  Description:    This function setup the download protocol packet
**  Author:         jiayong.yang
**  parameter:      msg : msg type
**                  buffer: where packet is saved
**                  data_size: length of message body
**                  packet_type: UART message or SPI message. 
******************************************************************************/
static int setup_packet(CMD_TYPE msg,char *buffer,int offset,int data_size,BSL_PACKET_E packet_type)
{
	struct pkt_header_tag *head;
	int length = sizeof(struct pkt_header_tag)+data_size;
	int total_size = 0;
	unsigned short crc16;
	
	head = (struct pkt_header_tag *)&buffer[4];
	switch(msg)
	{
		case BSL_CMD_CONNECT:
			if(packet_type==BSL_UART_PACKET){
				head->type = cpu2be16((unsigned short)msg);
				head->length = cpu2be16((unsigned short)data_size);
				*((unsigned short *)&buffer[length+4]) = 0;
				total_size = length + 2;
			}else{

				*((unsigned short *)&buffer[0]) = data_size;
				*((unsigned short *)&buffer[2]) = (unsigned short)msg;
				*((unsigned int *)&buffer[8]) = 0;
				*((unsigned short *)&buffer[4]) = boot_checksum((const unsigned char *)buffer+offset,data_size);
				*((unsigned short *)&buffer[6]) = boot_checksum((const unsigned char *)buffer,6);
				
				total_size = 12;
			}
		break;
		case BSL_CMD_START_DATA:
			if(packet_type==BSL_UART_PACKET){
				head->type = cpu2be16((unsigned short)msg);
				head->length = cpu2be16((unsigned short)data_size);
				crc16 = crc_16_l_calc((char const *)head,length);
				*((unsigned short *)&buffer[length+4]) = cpu2be16(crc16);
				total_size = length + 2;
			}else{

				*((unsigned short *)&buffer[0]) =  data_size;
				*((unsigned short *)&buffer[2]) = (unsigned short)msg;
				*((unsigned short *)&buffer[4]) = boot_checksum((const unsigned char *)buffer+offset,data_size);
				*((unsigned short *)&buffer[6]) = boot_checksum((const unsigned char *)buffer,6);
				total_size = data_size + 8;
			}
		break;
		case BSL_CMD_MIDST_DATA:
			if(packet_type==BSL_UART_PACKET){
				head->type =  cpu2be16(((unsigned short)msg));
				head->length = cpu2be16(((unsigned short)data_size));
				crc16 = crc_16_l_calc((char const *)head,length);
				*((unsigned short *)&buffer[length+4]) = cpu2be16(crc16);
				total_size = length + 2;
			}else{
				if(data_size == 0)
					*((unsigned short *)&buffer[0]) =  4;
				else
					*((unsigned short *)&buffer[0]) =  data_size;
				*((unsigned short *)&buffer[2]) = (unsigned short)msg;
				*((unsigned short *)&buffer[4]) = boot_checksum((const unsigned char *)(buffer+offset),data_size);
				*((unsigned short *)&buffer[6]) = boot_checksum((const unsigned char *)buffer,6);
				total_size = data_size + 8;
			}		
		break;
		case BSL_CMD_END_DATA:
			if(packet_type==BSL_UART_PACKET){
				head->type =  cpu2be16((unsigned short)msg);
				head->length = cpu2be16((unsigned short)data_size);
				crc16 = crc_16_l_calc((char const *)head,length);
				*((unsigned short *)&buffer[length+4]) = cpu2be16(crc16);
				total_size = length + 2;
			}else{

				*((unsigned short *)&buffer[0]) =  data_size;
				*((unsigned short *)&buffer[2]) = (unsigned short)msg;
				*((unsigned int *)&buffer[8]) = 0;
				*((unsigned short *)&buffer[4]) = boot_checksum((const unsigned char *)(buffer+offset),data_size);
				*((unsigned short *)&buffer[6]) = boot_checksum((const unsigned char *)buffer,6);
				
				total_size = 12;
			}		
		break;	
		case BSL_CMD_EXEC_DATA:
			if(packet_type==BSL_UART_PACKET){
				head->type =  cpu2be16((unsigned short)msg);
				head->length = cpu2be16((unsigned short)data_size);
				crc16 = crc_16_l_calc((char const *)head,length);
				*((unsigned short *)&buffer[length+4]) = cpu2be16(crc16);
				total_size = length + 2;
			}else{

				*((unsigned short *)&buffer[0]) =  data_size;
				*((unsigned short *)&buffer[2]) = (unsigned short)msg;
				*((unsigned short *)&buffer[4]) = boot_checksum((const unsigned char *)(buffer+offset),data_size);
				*((unsigned short *)&buffer[6]) = boot_checksum((const unsigned char *)buffer,6);
				total_size = data_size + 8;
			}		 
		break;
		case BSL_CMD_SWITCH_MODE:
			if(packet_type==BSL_UART_PACKET){
				head->type =  cpu2be16((unsigned short)msg);
				head->length = cpu2be16((unsigned short)data_size);
				crc16 = frm_chk((const unsigned short *)head,length);
				*((unsigned short *)&buffer[length+4]) = (crc16);
				total_size = length + 2;
			}else{

				*((unsigned short *)&buffer[0]) =  4;
				*((unsigned short *)&buffer[2]) = (unsigned short)msg;
				*((unsigned short *)&buffer[4]) = boot_checksum((const unsigned char *)(buffer+offset),data_size);
				*((unsigned short *)&buffer[6]) = boot_checksum((const unsigned char *)buffer,6);
				*((unsigned int *)&buffer[8]) = 0;
				total_size = 12;
			}
		break;
		default:
		break;					
	}
	return total_size;
}
/******************************************************************************
**  Description:    This function setup connect message by uart
**  Author:         jiayong.yang
**  parameter:      none 
******************************************************************************/
int  uart_send_connect_message(void)
{
	char raw_buffer[32] = {0};
	char *data= raw_buffer;
	int size;
	int translated_size;
	int offset;
	int retval;
	struct pkt_header_tag head;
	
	size = setup_packet(BSL_CMD_CONNECT,raw_buffer,8,0,BSL_UART_PACKET);
	translated_size = translate_packet((char *)send_buffer,(char *)&raw_buffer[4],size);
	retval = uart_write((char *)send_buffer,translated_size);
	if(retval > 0)
	{
		size = 8;
		offset = 0;
		do{
			retval = uart_read((char *)&raw_buffer[offset],size);
			if(retval > 0)
			{
				offset += retval;
				size -= retval;
			} else {
				break;
			}
		}while(size!=0);
	}
	if(retval > 0){
		data = (char *)send_buffer;	
		retval = untranslate_packet(data,(char *)raw_buffer,offset);	
		head.type = (data[0]<<8)|data[1];
		if(head.type == BSL_REP_ACK)
			return 0;
	} 
        printf("Modem connect failed\n");
	return -1;
}
/******************************************************************************
**  Description:    This function setup data start message by uart
**  Author:         jiayong.yang
**  parameter:      size : size of image to be sent
**                  addr : address where image to be saved in MODEM 
******************************************************************************/
int  uart_send_start_message(int size,unsigned long addr)
{
	char raw_buffer[32] = {0};
	char *data = raw_buffer;
	int translated_size;
	int offset;
	int retval;
	struct pkt_header_tag head;
	
	*(unsigned long *)&raw_buffer[8] = cpu2be32(addr);
	*(unsigned long *)&raw_buffer[12] = cpu2be32(size);
	size = setup_packet(BSL_CMD_START_DATA,raw_buffer,8,8,BSL_UART_PACKET);
	translated_size = translate_packet((char *)send_buffer,(char *)&raw_buffer[4],size);
	retval = uart_write((char *)send_buffer,translated_size);

	if(retval > 0)
	{
		size = 8;
		offset = 0;
		do{
			retval = uart_read((char *)&raw_buffer[offset],size);
			if(retval > 0)
			{
				offset += retval;
				size -= retval;
			} else {
				break;
			}
		}while(size!=0);
	}
	if(retval > 0){
		data = (char *)send_buffer;
		untranslate_packet(data,(char *)raw_buffer,offset);
		head.type = (data[0]<<8)|data[1];
		
		if(head.type == BSL_REP_ACK)
			return 0;
	} 
	printf("\nData START failed\n");
	return -1;
}
/******************************************************************************
**  Description:    This function setup data end message by uart
**  Author:         jiayong.yang
**  parameter:      none 
******************************************************************************/
int  uart_send_end_message(void)
{
	char raw_buffer[32] = {0};
	char *data = raw_buffer;
	int size;
	int translated_size;
	int offset;
	int retval;
	struct pkt_header_tag head;
	
	size = setup_packet(BSL_CMD_END_DATA,raw_buffer,8,0,BSL_UART_PACKET);
	translated_size = translate_packet((char *)send_buffer,(char *)&raw_buffer[4],size);
	retval = uart_write((char *)send_buffer,translated_size);

	if(retval >0)
	{
		size = 8;
		offset = 0;
		do{
			retval = uart_read((char *)&raw_buffer[offset],size);
			if(retval > 0)
			{
				offset += retval;
				size -= retval;
			} else {
				break;
			}
		}while(size!=0);
	}
	if(retval > 0){
		data = (char *)send_buffer;
		untranslate_packet(data,(char *)raw_buffer,offset);
		head.type = (data[0]<<8)|data[1];
		if(head.type == BSL_REP_ACK)
			return 0;
	} 
	printf("\nData START failed\n");
	return -1;
}
/******************************************************************************
**  Description:    This function setup data message by uart
**  Author:         jiayong.yang
**  parameter:      none 
******************************************************************************/
int  uart_send_data_message(char *buffer,int data_size)
{
	char raw_buffer[32] = {0};
	char	uart_buffer[300]={0};
	char *data = raw_buffer;
	int size;
	int translated_size;
	int offset;
	int retval;
	struct pkt_header_tag head;
	
	memcpy(&uart_buffer[8],buffer,data_size);	
	size = setup_packet(BSL_CMD_MIDST_DATA,uart_buffer,8,data_size,BSL_UART_PACKET);
	translated_size = translate_packet((char *)send_buffer,(char *)&uart_buffer[4],size);
	retval = uart_write((char *)send_buffer,translated_size);

	if(retval >0)
	{
		size = 8;
		offset = 0;
		do{
			retval = uart_read((char *)&raw_buffer[offset],size);
			if(retval > 0)
			{
				offset += retval;
				size -= retval;
			} else {
				break;
			}
		}while(size!=0);
	}
	if(retval > 0){
		data = (char *)send_buffer;
		untranslate_packet(data,(char *)raw_buffer,offset);
		head.type = (data[0]<<8)|data[1];
		
		if(head.type == BSL_REP_ACK){
			printf(".");
			return 0;
		} else {
			printf("E");
		}
	} 
	return -1;
}
/******************************************************************************
**  Description:    This function setup execute message by uart
**  Author:         jiayong.yang
**  parameter:      addr: address MODEM start to run.
******************************************************************************/
int  uart_send_exec_message(unsigned long addr)
{
	char raw_buffer[32] = {0};
	char *data = raw_buffer;
	int size;
	int translated_size;
	int offset;
	int retval;
	struct pkt_header_tag head;
	
	*(unsigned long *)&raw_buffer[8] = cpu2be32(addr);
	size = setup_packet(BSL_CMD_EXEC_DATA,raw_buffer,8,4,BSL_UART_PACKET);
	translated_size = translate_packet((char *)send_buffer,(char *)&raw_buffer[4],size);
	retval = uart_write((char *)send_buffer,translated_size);

	if(retval >0)
	{
		size = 8;
		offset = 0;
		do{
			retval = uart_read((char *)&raw_buffer[offset],size);
			if(retval >= 0)
			{
				offset += retval;
				size -= retval;
			} else {
				break;
			}
		}while(size!=0);
	}
	if(retval > 0){
		data = (char *)send_buffer;
		untranslate_packet(data,(char *)raw_buffer,offset);
		head.type = (data[0]<<8)|data[1];
		
		if(head.type == BSL_REP_ACK)
			return 0;
	} 
        printf("FDL failed to run!!!");
	return -1;
}
/******************************************************************************
**  Description:    This function change the transfer  to SPI bus
**  Author:         jiayong.yang
**  parameter:      none
******************************************************************************/
int  uart_send_change_spi_mode_message(void)
{
	char raw_buffer[32] = {0};
	char *data = raw_buffer;
	int size;
	int translated_size;
	int offset;
	int retval;
	struct pkt_header_tag head;
	
	size = setup_packet(BSL_CMD_SWITCH_MODE,raw_buffer,8,0,BSL_UART_PACKET);
	translated_size = translate_packet((char *)send_buffer,(char *)&raw_buffer[4],size);
	retval = uart_write((char *)send_buffer,translated_size);

	if(retval >0)
	{
		size = 8;
		offset = 0;
		do{
			retval = uart_read((char *)&raw_buffer[offset],size);
			if(retval > 0)
			{
				offset += retval;
				size -= retval;
			} else {
				break;
			}
		}while(size!=0);
	}
	if(retval > 0){
		data = (char *)send_buffer;
		untranslate_packet(data,(char *)raw_buffer,offset);
		head.type = (data[0]<<8)|data[1];
		
		if(head.type == BSL_REP_ACK)
			return 0;
		
	}
	printf("ChangeMode failed\n");
	return -1;
}
/******************************************************************************
**  Description:    This function send data start message by SPI
**  Author:         jiayong.yang
**  parameter:      img_address : address where image to be saved in MODEM
**                  img_size :  size of image to be sent
******************************************************************************/
#define SPI_ACK_SIZE	8
int hs_channel_send_start_message(unsigned long img_address,unsigned long img_size)
{
	unsigned char read_buffer[16]={0};
	unsigned char buffer[32]={0};
	unsigned long data_size = 8;
	struct pkt_header_tag *head;
	unsigned long data_offset=8;
	unsigned long *data = (unsigned long *)buffer;
	int status;

	data[2] = cpu2be32(img_address);
	data[3] = cpu2be32(img_size);
	setup_packet(BSL_CMD_START_DATA,buffer,data_offset,data_size,BSL_SPI_PACKET);
	hs_channel_write(buffer,8);
	hs_channel_write(buffer+data_offset,data_size);
	hs_channel_read(read_buffer,SPI_ACK_SIZE);
	head = (struct pkt_header_tag *)read_buffer;
	if(cpu2be16(head->type) == BSL_REP_ACK);
		return 0;
	printf("data start failed\n");
	return -1;
}
/******************************************************************************
**  Description:    This function send data message by SPI
**  Author:         jiayong.yang
**  parameter:      buffer: address of data to be sent
**                  data_size :  size of data to be sent
******************************************************************************/
int hs_channel_send_data_message(char *buffer,int data_size)
{
	unsigned char read_buffer[16]={0};
	unsigned long *data = (unsigned long *)send_buffer;
	struct pkt_header_tag *head;
	unsigned long data_offset=8;
	int status;

	memcpy((char *)send_buffer+data_offset,buffer,data_size);
	setup_packet(BSL_CMD_MIDST_DATA,send_buffer,data_offset,data_size,BSL_SPI_PACKET);

	hs_channel_write(send_buffer,8);
	hs_channel_write((char *)send_buffer+data_offset,data_size);
	hs_channel_read(read_buffer,SPI_ACK_SIZE);

	head = (struct pkt_header_tag *)read_buffer;

	if(cpu2be16(head->type) == BSL_REP_ACK){
		printf(".");
		return 0;
	}
	printf("E");
	return -1;
		
}
/******************************************************************************
**  Description:    This function send data end message by SPI
**  Author:         jiayong.yang
**  parameter:      none
******************************************************************************/
int hs_channel_send_data_end(void)
{
	unsigned char read_buffer[16]={0};
	unsigned char buffer[16]={0};
	unsigned long data_size = 4;
	unsigned long *data = (unsigned long *)buffer;
	struct pkt_header_tag *head;
	unsigned long data_offset=8;
	int status;

	setup_packet(BSL_CMD_END_DATA,buffer,data_offset,data_size,BSL_SPI_PACKET);
	hs_channel_write(buffer,8);
	hs_channel_write(buffer+data_offset,data_size);
	hs_channel_read(read_buffer,SPI_ACK_SIZE);

	head = (struct pkt_header_tag *)read_buffer;
	if(cpu2be16(head->type) == BSL_REP_ACK)
		return 0;
	printf("data end failed 0x%x\n",cpu2be16(head->type));
	return -1;
}
/******************************************************************************
**  Description:    This function setup execute message by SPI
**  Author:         jiayong.yang
**  parameter:      img_address : where modem start to run
******************************************************************************/
int hs_channel_send_exec(unsigned long address)
{
	unsigned char read_buffer[16]={0};
	unsigned char buffer[16]={0};
	unsigned long data_size = 4;
	unsigned long *data = (unsigned long *)buffer;
	struct pkt_header_tag *head;
	unsigned long data_offset=8;
	int status;

	data[2] = cpu2be32(address);
		
	setup_packet(BSL_CMD_EXEC_DATA,buffer,data_offset,data_size,BSL_SPI_PACKET);

	hs_channel_write(buffer,8);
	hs_channel_write(buffer+data_offset,data_size);
	hs_channel_read(read_buffer,SPI_ACK_SIZE);

	head = (struct pkt_header_tag *)read_buffer;
	if(cpu2be16(head->type) == BSL_REP_ACK)
		return 0;
	printf("MODEM run failed...\n");
	return -1;
}
/******************************************************************************
**  Description:    This function download modem images by SPI
**  Author:         jiayong.yang
**  parameter:      none
******************************************************************************/
struct modem_image_info {
        unsigned long *buffer;
        int image_size;
        unsigned long address;
};
struct modem_image_info download_images_info[]={
#if 1
        {
                0x00480000,
                0x00020000,
                0x00000400,
        },
#endif
#if 1
        {
                0x00480000,
                0x00020000,
                0x02100000,
        },
#endif
        {
                0x00020000,	//source address in AP memory
                0x003e0000,     //image size
                0x00020000,     //dest address in CP memory
        },
        {
                0x01600000,
                0x009F8000,
                0x00400000,
        },
};
void hs_download_image(struct modem_image_info *info)
{
        extern unsigned long get_timer_masked(void);
        unsigned long *img_data=NULL;
        int ret,i;

        printf("\nDload MODEM image: size = 0x%08x, address = 0x%08x\n",info->image_size,info->address);
	hs_channel_send_start_message(info->address,info->image_size);
	img_data = info->buffer;
        printf("\nstart_time = %dms\n",(unsigned int)get_timer_masked());
	for(i=0;i<(info->image_size/TEST_LENGTH);){
                ret = hs_channel_send_data_message(img_data,(int)TEST_LENGTH);
        	//printf("\nret = %d address = 0x%08x :0x%08x 0x%08x \n",ret,(int)img_data ,img_data[0],img_data[1]);
		if(ret == 0){
			img_data += (TEST_LENGTH/4);
			i++;
		} else {
			while(1);
		}
	}
        printf("\nend_time = %dms\n",(unsigned int)get_timer_masked());
}
void hs_download_proc(void)
{
	
        int i,j = 0;

	j = sizeof(download_images_info)/sizeof(download_images_info[0]);
        for(i=0;i<j;i++)
	     	hs_download_image(&download_images_info[i]);
	hs_channel_send_exec(0x400000);
}
