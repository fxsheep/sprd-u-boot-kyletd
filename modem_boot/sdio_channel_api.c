#include <config.h>
#include <common.h>
#include "../drivers/sdio/sdio_api.h"
#define mdelay(x)	udelay(1000*x)
//#define __DEBUG__
SDIO_HANDLE	sdio_handle=NULL;
extern int  req_clk_status(void);
extern void req_clk_init(void);
void sdio_channel_open(void)
{
	int status;
	req_clk_init();
        status = req_clk_status();
        printf("Clock_Req(W) = %d \n",status);
 //       do{status = req_clk_status();}while(status==0);
        sdio_handle = sdio_open();
        
}
/******************************************************************************
**  Description:    This function is used to read data from MODEM by SPI BUS.
**  Author:         jiayong.yang
**  parameter:      buffer is used to save data from MODEM
**                  len is length of data to be read. 
******************************************************************************/
int sdio_channel_read(unsigned char *buffer,int len)
{
    unsigned long status;
    int ret;
    
    if(sdio_handle == NULL)
	return 0;
    status = req_clk_status();
    //printf("(R) = %d \n",status);
    do{status = req_clk_status();}while(status==0);
    
    ret = sdio_read (sdio_handle,buffer,len);
    //do{status = req_clk_status();}while(status);
    return ret;
}
/******************************************************************************
**  Description:    This function is used to send data to MODEM by SPI BUS.
**  Author:         jiayong.yang
**  parameter:      buffer is used to save data from MODEM
**                  len is length of data to be read. 
******************************************************************************/
int sdio_channel_write(unsigned char *buffer,int len)
{
    unsigned long status;
    int ret;
    
    
    if(sdio_handle == NULL)
	return 0;
    status = req_clk_status();
    //printf("(W) = %d \n",status);
    do{status = req_clk_status();}while(status==0);
    ret = sdio_write (sdio_handle,buffer,len);

    return ret;
}
void sdio_channel_close(void)
{
    if(sdio_handle!=NULL)
    {
    	sdio_close(sdio_handle);
        sdio_handle = NULL;
    }
}

