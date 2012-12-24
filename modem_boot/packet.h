#ifndef __PACKET_H
#define __PACKET_H

//#define  __SPI_MODE__
#define	cpu2be16(wValue)  (((wValue & 0xFF)<<8) | ((wValue>>8)&0xFF))
#define	cpu2be32(dwValue) (((dwValue&0xFF)<<24) | (((dwValue>>8)&0xFF)<<16) | (((dwValue>>16)&0xFF)<<8) | (dwValue>>24))


extern int  uart_send_connect_message(void);
extern int  uart_send_start_message(int size,unsigned long addr);
extern int  uart_send_end_message(void);
extern int  uart_send_data_message(char *buffer,int data_size);
extern int  uart_send_exec_message(unsigned long addr);
extern int  uart_send_change_spi_mode_message(void);
extern unsigned short frm_chk (const unsigned short *src, int len);
extern unsigned short boot_checksum (const unsigned char *src, int len);
unsigned short calculate_crc (unsigned short crc, char const *buffer, int len);
unsigned int crc_16_l_calc (char *buf_ptr,unsigned int len);
#endif //__PACKET_H
