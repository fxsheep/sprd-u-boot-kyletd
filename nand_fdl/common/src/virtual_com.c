/******************************************************************************
 ** File Name:    model.c                                                     *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2005     Daniel.Ding     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "common.h"
#include "drv_usb.h"
#include "virtual_com.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
/*--------------------------- Local Data ------------------------------------*/
/*--------------------------- Global Data -----------------------------------*/
__attribute__((aligned(4))) const char DeviceDescrSerial [] =
{
0x12, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, 0x40,
0x82, 0x17, 0x00, 0x4d, 0x02, 0x02, 0x00, 0x00, /*lint !e569 */
//0x7b, 0x06, 0x03, 0x23, 0x02, 0x02, 0x00, 0x00,
0x00, 0x01
};


__attribute__((aligned(4))) const uint8 ConfigDescrSerial[] =
{
0x09,0x02,0x27,0x00,0x01,0x01,0x00,0xc0,  
0x32,
0x09,0x04,0x00,0x00,0x03,0xff,0x00,0x00,
0x00,
0x07,0x05,0x81,0x03,0x0a,0x00,0x01, //Ep 1 In
0x07,0x05,0x02,0x02,0x40,0x00,0x00, //Ep 2 OUT
0x07,0x05,0x83,0x02,0x40,0x00,0x00, //Ep 3 In
0x00
};


__attribute__((aligned(4))) const char VenderRespond[] =
{
	0x00,0x00,0x00,0x00
};

char class_buf [64];


extern USB_rx_buf_T buf_manager ;
/*--------------------------- External Data ---------------------------------*/
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
char VCOM_GetChar(void)
{
	volatile unsigned char ch = 0;
	int condition ; 
    INT_CTRL_T * int_ctl_ptr = (INT_CTRL_T *)INT_REG_BASE;       
	USB_DEV_REG_T * ctl = (USB_DEV_REG_T *)USB_REG_BASE;
	USB_DEV_INT_STS_U status ;
    USB_rx_buf_T 	*buf_ptr = &buf_manager;

	//if no any char, loop in here ;
	condition = TRUE ;
	do{

		if (buf_ptr->read != buf_ptr->write)
		{
        	ch = buf_ptr->usb_rx_buf[buf_ptr->read];
			buf_ptr->read++;
			if (buf_ptr->read >= USB_RECV_LIMIT){
				buf_ptr->read = 0;
			}
				
			condition = FALSE ;
			return ch;
		}

		if( int_ctl_ptr->raw.dwValue  & (1<<25)){
			status.dwValue = ctl->int_sts.dwValue ;
			//dispatch handler message according to usb int status ;
			if (status.dwValue  & USB_INT_EPx_MASK )
			{
				//dispatch EPx handler ;
				if (status.mBits.ep2 )
				{
					USB_Ep2_handler();
				}
				if (status.mBits.ep3 )
				{
					USB_Ep3_handler();
				}
				if (status.mBits.ep0 )
				{
					USB_Ep0_handler();
				}
				if (status.mBits.ep1 )
				{
					USB_Ep1_handler();
				}	
			}
			//clear usb interrupt ;
			ctl->int_clr.dwValue = USB_INT_DEV_MASK ;
		}

	}while (condition);
	return (char)ch ;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
int VCOM_GetSingleChar(void)
{
	volatile int ch ;
    INT_CTRL_T * int_ctl_ptr = (INT_CTRL_T *)INT_REG_BASE;       
	USB_DEV_REG_T * ctl = (USB_DEV_REG_T *)USB_REG_BASE;
	USB_DEV_INT_STS_U status ;
    USB_rx_buf_T 	*buf_ptr = &buf_manager;

	ch = -1;

	if( int_ctl_ptr->raw.dwValue  & (1<<25)){
		status.dwValue = ctl->int_sts.dwValue ;
		//dispatch handler message according to usb int status ;
		if (status.dwValue  & USB_INT_EPx_MASK )
		{
			//dispatch EPx handler ;
			if (status.mBits.ep2 )
			{
				USB_Ep2_handler();
			}
			if (status.mBits.ep3 )
			{
				USB_Ep3_handler();
			}
			if (status.mBits.ep0 )
			{
				USB_Ep0_handler();
			}
			if (status.mBits.ep1 )
			{
				USB_Ep1_handler();
			}	
		}
		//clear usb interrupt ;
		ctl->int_clr.dwValue = USB_INT_DEV_MASK ;
	}

	if (buf_ptr->read != buf_ptr->write)
	{
    	ch = buf_ptr->usb_rx_buf[buf_ptr->read];
		buf_ptr->read++;
		if (buf_ptr->read >= USB_RECV_LIMIT){
			buf_ptr->read = 0;
		}
	}

	return ch ;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void VCOM_GetDevDescriptor (char bDirection,short wValue,short wIndex ,short wLen)
{
	volatile VALUE_U temp;
	short wResidue = 0;
	char *pData = 0;

	temp.wValue  = wValue;
	switch (temp.sDescriptor.type){
 	case USB_DEVICE_DESCRIPTOR_TYPE:
		if (wLen >= 0x040){
			wResidue = 18;			
		}
		else{
        	wResidue = wLen;
        }
        pData 	 = (char *)&DeviceDescrSerial;
        break;
    case USB_CONFIGURATION_DESCRIPTOR_TYPE:
		if (wLen >= 0x0ff){
			wResidue = 0x27;			
		}
		else{
			wResidue = wLen;
		} 
        pData    = (char *)&ConfigDescrSerial;
        break;
    case USB_STRING_DESCRIPTOR_TYPE:
        break;
    default:
        break;
	}
	//Write data to EP in & send data to host  ;
	USB_EPxSendData (USB_EP0_IN,(unsigned int *)pData,wResidue);
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void VCOM_VendorCommand (char bDirection,short wValue,short wIndex ,short wLen)
{
	char *pData = 0;
	if (bDirection){
		pData    = (char *)&VenderRespond;
		USB_EPxSendData (USB_EP0_IN,(unsigned int *)pData,wLen);
	}
	else {
		pData    = (char *)&VenderRespond;
		USB_EPxReceiveData  (USB_EP0_OUT,(unsigned int *)pData,NULL);
	}
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void VCOM_ClassCommand (char bDirection,short wValue,short wIndex ,short wLen)
{
	if ( bDirection ) {
		USB_EPxSendData (USB_EP0_IN,(unsigned int *)class_buf,wLen);	
	}
	else {
		USB_EPxReceiveData  (USB_EP0_OUT,(unsigned int *)class_buf,wLen);
	}
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void VCOM_SetAddress (char bDirection,short wValue,short wIndex ,short wLen)
{
	USB_EPxSendData (USB_EP0_IN,(unsigned int *)class_buf,NULL);
}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
