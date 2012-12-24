#include <common.h>
#include <linux/types.h>

#define SPI_PORT	0
extern int spi_channel_read (unsigned long dev_id,unsigned char *buffer_ptr,int len);
extern int spi_channel_write (unsigned long dev_id,unsigned char *buffer_ptr,int len);
extern void spi_channel_open (unsigned long phy_id,unsigned long mode,unsigned long tx_bit_length);
extern void dump_spi_registers(unsigned long phy_id);
extern int  spi_channel_status(void);

void SPI_channel_open(void)
{
	spi_channel_open(0,1,32);
	dump_spi_registers(0);
	req_clk_init();
}
/******************************************************************************
**  Description:    This function is used to read data from MODEM by SPI BUS.
**  Author:         jiayong.yang
**  parameter:      buffer is used to save data from MODEM
**                  len is length of data to be read. 
******************************************************************************/
int SPI_channel_read(unsigned char *buffer,int len)
{
    unsigned long status;
    int ret;
    do{status = req_clk_status();}while(status==0);
    ret = spi_channel_read (SPI_PORT,buffer,len);
    do{status = req_clk_status();}while(status);
    return ret;
}
/******************************************************************************
**  Description:    This function is used to send data to MODEM by SPI BUS.
**  Author:         jiayong.yang
**  parameter:      buffer is used to save data from MODEM
**                  len is length of data to be read. 
******************************************************************************/
int SPI_channel_write(unsigned char *buffer,int len)
{
    unsigned long status;
    int ret;
    do{status = req_clk_status();}while(status==0);
    ret = spi_channel_write (SPI_PORT,buffer,len);
    do{status = req_clk_status();}while(status);
    return ret;
}
void SPI_channel_close(void)
{
    return;
}
