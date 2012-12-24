/******************************************************************************
 ** File Name:      sdio_api.h                                                *
 ** Author:         jiayong.yang                                              *
 ** DATE:           25/06/2012                                                *
 ** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    add sdio API for modem boot in u-boot by SDIO bus         *
 ******************************************************************************/
#ifndef _SDIO_API_H_
#define _SDIO_API_H_

typedef void* SDIO_HANDLE;


/*****************************************************************************/
//  Description: send data to sdio device
//  Author: Jason.wu
//  Param
//		handler: the handle of sdio device
//		buffer:	 data to be sent
//              size:    data size
//  Return:
//		size of data successfully sent
//  Note: 
/*****************************************************************************/
int sdio_write(SDIO_HANDLE handle,char *buffer,int size);
/*****************************************************************************/
//  Description: read data from sdio device
//  Author: Jason.wu
//  Param
//		handler: the handle of sdio device
//		buffer:	 data to be read
//              size:    data size
//  Return:
//		size of data successfully read
//  Note: 
/*****************************************************************************/
int sdio_read(SDIO_HANDLE handle,char *buffer,int size);
/*****************************************************************************/
//  Description: open the sdio device
//  Author: Jason.wu
//  Param
//		NONE
//  Return:
//		handler: the handle of sdio device
//  Note: 
/*****************************************************************************/
SDIO_HANDLE sdio_open(void);
/*****************************************************************************/
//  Description: close the sdio device
//  Author: Jason.wu
//  Param
//		handler: the handle of sdio device
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
void sdio_close(SDIO_HANDLE handle);

#endif
