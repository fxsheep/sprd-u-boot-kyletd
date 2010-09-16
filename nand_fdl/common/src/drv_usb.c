/******************************************************************************
 ** File Name:    usb.c                                                     *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  This file is usb driver file .It include all driver funciton*
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
#include "drv_usb.h"
#include "virtual_com.h"
#include "sci_types.h"
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
#define DATA_OUT	1
#define DATA_IN		0
#define DWORD_ENDIAN_SWAP(DWord) \
			(((DWord & 0x0FF00)<< 8)|((DWord & 0x0FF)<<24) |\
			((DWord & 0xFF000000)>>24)|((DWord & 0xFF0000)>>8))
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
/*--------------------------- Local Data ------------------------------------*/

/*--------------------------- Global Data -----------------------------------*/
/*--------------------------- External Data ---------------------------------*/
USB_rx_buf_T 	buf_manager ;
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
LOCAL __attribute__((aligned(4))) volatile unsigned char usb_ep2_buf [64];
BOOLEAN USB_DWordBufEndianSwap (uint32 * src ,uint32 * des ,uint32 len);
/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
BOOLEAN USB_EP0Config  (void)
{
    USB_EP0_REG_T * ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
	//config DMA's USB channel ;
	//Set max packet size is 8 Byte ;
	ctl_ep0->ctrl.mBits.pack_size   = 0x8 ;

	ctl_ep0->int_clr.mBits.setup_end  = 1 ;
	ctl_ep0->int_ctrl.mBits.setup_end = 1 ;

	//Set buffer ready ;	
	ctl_ep0->ctrl.mBits.buf_ready = 1 ;
	
	return TRUE;
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
BOOLEAN USB_EP1Config  (void)
{
	USB_EPx_REG_T * ctl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;
	
	//Set max packet size is 8 Byte ;
	ctl_ep1->ctrl.mBits.pack_size   			= 0xa ;
	ctl_ep1->transfer_size.mBits.num 		= 0xa ;
	
	//ctl_ep1->ctrl.bits.data_ready 			= 1 ;	 //Set data ready ;

	ctl_ep1->int_clr.mBits.transaction_end	= 1 ;
	ctl_ep1->int_ctrl.mBits.transaction_end	= 1 ;
	
	ctl_ep1->ctrl.mBits.ep_en   				= 1 ;
	
	return TRUE;
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
BOOLEAN USB_EP2Config  (void)
{
	USB_EPx_REG_T * ctl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;
	
	//Set max packet size is 8 Byte ;
	ctl_ep2->ctrl.mBits.pack_size   			= 0x40 ;
	ctl_ep2->receive_num.mBits.num  			= 0x2000 ;
	
	ctl_ep2->int_clr.mBits.transaction_end	= 1 ;	//Clear
	ctl_ep2->int_ctrl.mBits.transaction_end	= 1 ;	//Enable interrupt
	ctl_ep2->ctrl.mBits.ep_en   				= 1 ;	//Enable ep2
	
	ctl_ep2->ctrl.mBits.buf_ready 			= 1 ;	 //Set data ready ;
	
	return TRUE;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
BOOLEAN USB_EP3Config  (void)
{
	USB_EPx_REG_T * ctl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
	
	//Set max packet size is 8 Byte ;
	ctl_ep3->ctrl.mBits.pack_size   		= 0x40 ;
	ctl_ep3->transfer_size.mBits.num    	= 0x40 ;
	
	ctl_ep3->int_clr.mBits.transfer_end	= 1 ;
	ctl_ep3->int_ctrl.mBits.transfer_end	= 1 ;
	
	ctl_ep3->ctrl.mBits.ep_en   			= 1 ;
	
	return TRUE;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
LOCAL void data_proc (unsigned int * src ,unsigned int *des ,int direct ,int len ,int ep_id)
{
	int i;
	if (direct)
	{
		if (ep_id == USB_EP2){
			for (i=0;i< (len);i+=2){
				* (des + i) = (* (volatile unsigned int *) src) ;
				* (des + i+1) = (* (volatile unsigned int *) src) ;
			}
		}
		else{
			for (i=0;i< (len);i++){
				* (des + i) = DWSwapHL (* (volatile unsigned int *) src) ;
			}
		}
	}
	else
	{
		//Send datat to FIFO ;
		for (i=0;i< (len);i++){
			* (volatile unsigned int *) des = DWSwapHL (*(src + i));
		}
	}
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
int USB_EPxSendData  (char ep_id ,unsigned int * pBuf,short len)
{
	int size = ((len + 3 )>>2) & 0x3FF ;

	switch (ep_id){ 
	case USB_EP0_IN:
		{
			USB_EP0_REG_T * ctl_Ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
			ctl_Ep0->ctrl.mBits.pack_size	   	= len ;
			ctl_Ep0->transfer_size_in.mBits.num	= len ;
			data_proc (pBuf , (unsigned int *) USB_EP0_IN_FIFO, DATA_IN,size,USB_EP0_IN);
			ctl_Ep0->ctrl.mBits.data_ready 		= 1 ;	 //Set data ready ;
		}
		break;
	case USB_EP1:
		{
			USB_EPx_REG_T * ctl_Ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;	
			ctl_Ep1->ctrl.mBits.pack_size   	  	= len ;
			ctl_Ep1->transfer_size.mBits.num   	= len ;
			data_proc (pBuf , (unsigned int *) USB_EP1_FIFO, DATA_IN,size,USB_EP1);
			ctl_Ep1->ctrl.mBits.data_ready 		= 1 ;	 //Set data ready ;
		}
		break;
	case USB_EP3:
		{
			USB_EPx_REG_T * ctl_Ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;	
			ctl_Ep3->ctrl.mBits.pack_size   	  	= len ;
			ctl_Ep3->transfer_size.mBits.num   	= len ;
			data_proc (pBuf , (unsigned int *) USB_EP3_FIFO, DATA_IN,size,USB_EP3);
			ctl_Ep3->ctrl.mBits.data_ready 		= 1 ;	 //Set data ready ;
			while (!(ctl_Ep3->int_sts.mBits.transfer_end)){//wait until transfer end
			}
            //clear the int status of ep3's tranfer end 
            ctl_Ep3->int_clr.mBits.transfer_end = 1;  
		}
		break;
	default:
	    break ;
	}
	
	return TRUE;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
int USB_EPxReceiveData  (char ep_id ,unsigned int * pBuf,short len)
{
	volatile int size = ((len + 3 )>>2) & 0xFF ;

	switch (ep_id){
	case USB_EP0_OUT:
		{
			USB_EP0_REG_T * ctl_Ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
			data_proc ((unsigned int *) USB_EP0_OUT_FIFO,(unsigned int *)pBuf , DATA_OUT,size,USB_EP0_OUT);
			ctl_Ep0->ctrl.mBits.buf_ready 		= 1 ;	 //Set data ready ;
		}
		break;
	case USB_EP2:
		{
			USB_EPx_REG_T * ctl_Ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;	
			data_proc ((unsigned int *) USB_EP2_FIFO, (unsigned int *)pBuf , DATA_OUT,size,USB_EP2);
			ctl_Ep2->ctrl.mBits.buf_ready 		= 1 ;	 //Set data ready ;
		}
		break;
	default:
	    break ;	
	}
	
	return TRUE;
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
uint32 DWSwapHL (uint32 DWord)
{
#if defined (NAND_FDL_SC6600I)
	return (((DWord & 0x0FF00)<< 8)|((DWord & 0x0FF)<<24) |\
			((DWord & 0xFF000000)>>24)|((DWord & 0xFF0000)>>8));
#else
    return DWord;
#endif
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void USB_Ep0_handler  (void)
{
	USB_EP0_REG_T * ep0_ctrl = (USB_EP0_REG_T *)USB_EP0_BASE ;
	USB_EPx_INT_U status ;
	
	status.dwValue = ep0_ctrl->int_sts.dwValue ;

	if (status.mBits.setup_end){
        switch (ep0_ctrl->setup_low.mBits.type){
            case USB_REQ_STANDARD:
                switch (ep0_ctrl->setup_low.mBits.recipient){
                    case USB_REC_DEVICE:
                        VCOM_GetDevDescriptor (\
                                    (ep0_ctrl->setup_low.mBits.direction),\
                                    (ep0_ctrl->setup_low.mBits.value),\
                                    (ep0_ctrl->setup_high.mBits.index),\
                                    (ep0_ctrl->setup_high.mBits.length));
                        break;
                    case USB_REC_INTERFACE:
                        break;
                    default :
                        break;                
                }

                break;
            case USB_REQ_CLASS:
                if(USB_REC_INTERFACE == ep0_ctrl->setup_low.mBits.recipient)
                {
	                switch (ep0_ctrl->setup_low.mBits.request){
	                    case 0x20:
	                    case 0x21:
	                    case 0x22:
	                        VCOM_ClassCommand (\
	                                    (ep0_ctrl->setup_low.mBits.direction),\
	                                    (ep0_ctrl->setup_low.mBits.value),\
	                                    (ep0_ctrl->setup_high.mBits.index),\
	                                    (ep0_ctrl->setup_high.mBits.length));
	                        break;
	                    default :
	                        break;                
	                }
                }
                break;
            case USB_REQ_VENDOR:
                if(USB_REC_DEVICE == ep0_ctrl->setup_low.mBits.recipient)
                {
	                switch (ep0_ctrl->setup_low.mBits.request){
	                    case 0x01:
	                    case 0x03:
	                        VCOM_VendorCommand (\
	                                    (ep0_ctrl->setup_low.mBits.direction),\
	                                    (ep0_ctrl->setup_low.mBits.value),\
	                                    (ep0_ctrl->setup_high.mBits.index),\
	                                    (ep0_ctrl->setup_high.mBits.length));
	                        break;
	                    default :
	                        break;                
	                }
                }
                break;
            default :
                break;
        }
	}
	
	ep0_ctrl->int_clr.dwValue =0x3fff;
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void USB_Ep1_handler  (void)
{
	USB_EPx_REG_T * ctl = (USB_EPx_REG_T *)USB_EP1_BASE ;
	USB_EPx_INT_U status ;
	
	status.dwValue = ctl->int_sts.dwValue ;
	if (status.mBits.transfer_end)
	{
		//clear transfer end interrupt so that controler can send zero to host .
		ctl->int_clr.mBits.transfer_end = 1;
	}
	ctl->int_clr.dwValue = 0x3FFF; //Clear all interrupt ;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void USB_Ep2_handler  (void)
{
	USB_EPx_REG_T * ctl_ep2 = (USB_EPx_REG_T *)USB_EP2_BASE ;
	USB_EPx_INT_U status ;
	int num = 0;
	int i ;
	
	status.dwValue = ctl_ep2->int_sts.dwValue ;

	if (status.mBits.transaction_end){

		num = ctl_ep2->ctrl.mBits.p_num ;

		USB_EPxReceiveData (USB_EP2,(unsigned int *)usb_ep2_buf,num);
		USB_DWordBufEndianSwap ((uint32 *)usb_ep2_buf ,\
								(uint32 *)usb_ep2_buf,\
								16);
		for (i=0;i<num;i++)
		{
			buf_manager.usb_rx_buf[buf_manager.write++] = usb_ep2_buf[i];
			if (buf_manager.write >= USB_RECV_LIMIT ){
				buf_manager.write = 0;
			}
		}
		ctl_ep2->ctrl.mBits.buf_ready = 1 ;	 	//Set buf ready ;
	}
	ctl_ep2->int_clr.dwValue = 0x3FFF; 			//Clear all interrupt ;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void USB_Ep3_handler  (void)
{
	USB_EPx_REG_T * ctl_ep3 = (USB_EPx_REG_T *)USB_EP3_BASE ;
	USB_EPx_INT_U status ;
	
	status.dwValue = ctl_ep3->int_sts.dwValue ;
	ctl_ep3->int_clr.dwValue = 0x3FFF; //Clear all interrupt ;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void USB_Ep4_handler  (void)
{
	USB_EPx_REG_T * ctl = (USB_EPx_REG_T *)USB_EP4_BASE ;
	USB_EPx_INT_U status ;
	
	status.dwValue = ctl->int_sts.dwValue ;
	ctl->int_clr.dwValue = 0x3FFF; //Clear all interrupt ;
}

BOOLEAN USB_DWordBufEndianSwap (uint32 * src ,uint32 * des ,uint32 len)
{
#if defined (NAND_FDL_SC6600I)
	int i;
	uint32 *pSrc = src,*pDes = des;
	
	for (i=0;i<len ;i++){
		__asm{
	        ldr      r5,[pSrc]
	        and      r6,r5,#0xff00
	        mov      r6,r6,lsl #8
	        orr      r6,r6,r5,lsl #24
	        orr      r6,r6,r5,lsr #24
	        bic      r5,r5,#0xff000000
	        bic      r5,r5,#0xff00
	        orr      r6,r6,r5,lsr #8
	        str      r6,[pDes]
	    }
	    pSrc++;
	    pDes++;
	}
#endif
	return TRUE ;
}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
