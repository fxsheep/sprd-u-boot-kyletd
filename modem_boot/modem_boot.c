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
#include "packet.h"
#include "cmd_def.h"

//#define __TEST_SPI_ONLY__
//#define __DL_UART0__
#ifndef __SPI_MODE__
#define hs_channel_open	sdio_channel_open
#define hs_channel_close	sdio_channel_close
#else
#define hs_channel_open	SPI_channel_open
#define hs_channel_close	SPI_channel_close

#endif
#define mdelay(_ms) udelay(_ms*1000)

static int	boot_status = 0;


extern int serial0_init (void);
extern int serial0_getc(void);
extern int serial0_tstc (void);
extern void serial0_putc(const char c);

extern int serial_init (void);
extern int serial_getc(void);
extern int serial_tstc (void);
extern void serial_putc(const char c);
#ifdef __DL_UART0__
	#define uart_init serial0_init
	#define uart_getc serial0_getc
	#define uart_tstc serial0_tstc
	#define uart_putc serial0_putc
#else
	#define uart_init serial_init
	#define uart_getc serial_getc
	#define uart_tstc serial_tstc
	#define uart_putc serial_putc
#endif
#define		PIN_REG_SIMDA3		(0x8C000470)
#define		PIN_REG_SIMRST3		(0x8C000478)
#define		PIN_CTRL_REG     	(0x8b000028)
extern int __dl_log_share__;
void	print_message(int dir,char *buffer,int size)
{
	int i;
	if(size <= 0)
		return;
	if(dir)
		printf("Send(%d): ",size);
	else
		printf("Recv(%d): ",size);
	if(size > 16) size =16;
	for(i=0;i<size;i++)
		printf("0x%x ",buffer[i]);
	printf("\n");
}
int uart_write(char *buffer,int size)
{
	int write_len=0;
	if(size > 0)
	{
		do{
			uart_putc(*buffer++);
			size--;
                        write_len++;
		}while(size>0);
	}
	return  write_len;
}

int uart_read(char *buffer,int size)
{
	int read_len=0;
	char *data=buffer;
	if(size > 0)
	{
		do{
			if(uart_tstc())
			{
			    *buffer++ = uart_getc();
			    size --;
			    read_len++;
			}
		}while(size>0);
	}
	return read_len;
}

int	try_read_version_string(void)
{
	unsigned char version_string[64];
	int version_string_length = 0;
	unsigned char read_char;

	version_string_length = 0;
	read_char = uart_getc();
	if(read_char != 0x7e){
		return 1;
	}
	version_string[version_string_length++] = read_char;

	while(uart_tstc()){
		read_char = uart_getc();	
		version_string[version_string_length++] = read_char;
		if(read_char == 0x7e){
			return parse_version_string(version_string,version_string_length);
		}
	}
	mdelay(10);
	if(!uart_tstc())
		return 1;
	while(uart_tstc()){
		read_char = uart_getc();	
		version_string[version_string_length++] = read_char;
		if(read_char == 0x7e){
			return parse_version_string(version_string,version_string_length);
		}
	}
	return 1;
	//reset_modem_device();
}

static void download_fdl(char *data,int size)
{
	int retval;
	int i;
	int file_length = size;
	int sent_length = 0;
	int packet_size = 256;
	unsigned long fdl_run_address = 0x40000000;
	char	*buffer = data;
#ifndef __TEST_SPI_ONLY__	
	retval = uart_send_connect_message();
	
	retval = uart_send_start_message(file_length,fdl_run_address);
	do{
		if(sent_length + packet_size >= file_length)
			packet_size = file_length - sent_length;
		uart_send_data_message(buffer,packet_size);
		sent_length += packet_size;
		buffer += packet_size;
	}while(sent_length < file_length);
	uart_send_end_message();
        
	uart_send_exec_message(fdl_run_address);
#endif

	//printf("\ntry to connect with FDL... \n");
	do{
		mdelay(1);
		uart_putc(0x7E);
	}while(!uart_tstc());
	boot_status = 1;//uart and FDL 
	try_read_version_string();
        uart_send_change_spi_mode_message();
#if 0 //def __DL_UART0__
        *(volatile unsigned long *)PIN_REG_SIMDA3  |= 0x00000380;
	*(volatile unsigned long *)PIN_REG_SIMRST3 |= 0x00000380;
	*(volatile unsigned long *)PIN_CTRL_REG &= 0xffffffbf;
        
        uart_init();
        __dl_log_share__ = 0;
        printf("__dl_log_share__ = %d \n",__dl_log_share__);
#endif
	
	mdelay(2);
	{
		extern void hs_download_proc(void);
		extern void hs_channel_open(void);
		int status;
                
		hs_channel_open();
		hs_download_proc();
		hs_channel_close();
	}
	
}

int  parse_version_string(unsigned char *buffer,int length)
{
	int i;
	unsigned short temp_data[64];
	struct pkt_header_tag packet;
	unsigned short calculated_crc = 0;
	unsigned short received_crc = (buffer[length-3] << 8) | buffer[length-2]; 

	packet.type = (buffer[1]<<8)|buffer[2];
	packet.length = (buffer[3]<<8)|buffer[4];
	if((buffer[0] == 0x7e) && (buffer[length-1] == 0x7e))
	{
		if(boot_status == 0)
			calculated_crc = crc_16_l_calc(&buffer[1],length-4);
		else
		{
			memcpy(temp_data,&buffer[1],length-4);
			calculated_crc = frm_chk(&temp_data,length-4);
			calculated_crc = (calculated_crc>>8)|(calculated_crc<<8);
		}
	}

	//printf("type 0x%x size =%d r_crc = 0x%x c_crc 0x%x\n",packet.type,packet.length,received_crc,calculated_crc);
	if(calculated_crc == received_crc)
		return 0;
	return 0;
}



void   bootup_modem(char *data,int size)
{
	
#ifdef CONFIG_SP7702
	int i=0,delay=0;
	unsigned char hand_shake_flag = 0x7e;
	int is_empty=1;
	int retval=0;
	int retry_count;
#ifndef __DL_UART0__
        __dl_log_share__ = 1;
        uart_init();

        *(volatile unsigned long *)PIN_REG_SIMDA3  |= 0x000003a0;
	*(volatile unsigned long *)PIN_REG_SIMRST3 |= 0x000003a0;
	*(volatile unsigned long *)PIN_CTRL_REG |= 0x00000040;
#endif
modem_reset:
	retry_count = 0;
	{
		extern void modem_poweroff(void);
		
		modem_poweroff();
		mdelay(2000);

	}
	
	uart_putc(0x7E);
        uart_putc(0x7E); 
#ifdef __TEST_SPI_ONLY__
	download_fdl(data,size);
#endif
	{
		extern void modem_poweron(void);
		modem_poweron();
	}
	
	//printf("try to get version string....\n");
	for(;;)
	{
		is_empty = 1;
		while(uart_tstc()){
			uart_getc();
		};

		uart_putc(hand_shake_flag);

		mdelay(10);
		retry_count++;
		if(uart_tstc()){
			is_empty = 0;
			//printf("UART RX FIFO is not empty....\n");
		}

		if(is_empty == 0)
		{
			retval = try_read_version_string();
			if(retval == 0)
				break;
		}
		if(retry_count >200)
			goto modem_reset;
	}
	//printf("\nsuccessfully read version string !!!\n");
	download_fdl(data,size);
	boot_status = 2;
#endif
}

inline int modem_status(void) {
	return boot_status;
}
