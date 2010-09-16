/******************************************************************************
 ** File Name:    usb_boot.c                                                  *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         4/25/2005                                                   *
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
#include "fdl_crc.h"
#include "usb_boot.h"
#include "drv_usb.h"
#include "virtual_com.h"
#include "fdl_main.h"
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
#define USB_BUFF_SIZE  0x4000
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/

/*--------------------------- Local Data ------------------------------------*/
LOCAL __attribute__((aligned(4))) uint8 s_usb_snd_buff[USB_BUFF_SIZE];
/*--------------------------- Global Data -----------------------------------*/
/*--------------------------- External Data ---------------------------------*/
extern USB_rx_buf_T 	buf_manager ;
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
void usb_write (char *write_buf,int write_len);
void usb_init(uint32 ext_clk26M);
/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void usb_boot (uint32 ext_clk26M)
{
	buf_manager.read 	= 0 ;
	buf_manager.write 	= 0 ;

	usb_init(ext_clk26M);
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void usb_varinit (void)
{
	int i;

	buf_manager.read 	= 0 ;
	buf_manager.write 	= 0 ;

}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void usb_init(uint32 ext_clk26M)
{
	int i;
    USB_DEV_REG_T 		* ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;

#if defined (NAND_FDL_SC6600I)
    if (ext_clk26M){
		//Do nothing;    
    }
    else{
	    //PLL change enable ;
	    *(volatile unsigned long *) 0x8b000018 |= 1<<9;
		//Set PLL is 144M ;
		*(volatile unsigned long *) 0x8b000024 = 0x0090000D;
	    //PLL change disable ;
	    *(volatile unsigned long *) 0x8b000018 &= ~(1<<9);

        *(volatile uint32 *)0x8b00000c = 0x11;     // mcu_clk set to 72mhz

		//Wait for a moment;
		system_delay(20);
	}

	//Close USB_LDO and pulldown 1.5K resister ; 
	//in this version, it's forbided,we don't want usb disconnect is detected by host

	//Enable USB AHB CLK ;
	*(volatile uint32 * )(0x20000100) |= 1<<2;
#elif defined (NAND_FDL_SC6800D)
    if (ext_clk26M){
		//Do nothing;    
    }
    else{
	    //PLL change enable ;
	    *(volatile unsigned long *) 0x8b000018 |= 1<<20;
		//Set PLL is 144M ;
		*(volatile unsigned long *) 0x8b000068 = 0x0090000D;

		//Wait for a moment;
		system_delay(20);
	}

	//Close USB_LDO and pulldown 1.5K resister ; 
	//in this version, it's forbided,we don't want usb disconnect is detected by host

	//Enable USB CLK ;
	*(volatile uint32 * )(0x20900200) |= 1<<5;
#elif defined (NAND_FDL_SC8800H)
       *(volatile uint32 * )(0x20900200) |= 1<<5;
#endif
	
	//Enable USB device ;
	ctl_usb->ctrl.mBits.en 	= ENABLE;

	USB_EP0Config ();
	USB_EP1Config ();
	USB_EP2Config ();
	USB_EP3Config ();
	
	//Open USB_LDO and pullup 1.5K resister;
    //handled in entry of init.s
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void usb_write (char *write_buf,int write_len)
{
	unsigned short crc;
	int i;
	int send_len;
	unsigned char curval;
	uint8 * usb_buff_ptr = NULL;

    // @Richard
    // NOTE(Important):
    // We do not do mask because the message including crc do not have mask word!
    // So if there is mask word, we have to modify below codes.
    // 
    crc = frm_chk((const unsigned short*)write_buf, write_len - 2);
    *(write_buf + write_len - 2) = (crc >> 8) & 0x0FF;
    *(write_buf + write_len - 1) = crc & 0x0FF;

    send_len = 1;
    usb_buff_ptr = &s_usb_snd_buff[1];
        
    for (i = 0; i < write_len; i++)
    {
        curval = *(write_buf + i);
		if ((HDLC_FLAG == curval) || (HDLC_ESCAPE == curval)) {
			*(usb_buff_ptr++) = HDLC_ESCAPE;
			*(usb_buff_ptr++) = ~HDLC_ESCAPE_MASK & curval;
			send_len++;
		} else {
			*(usb_buff_ptr++) = curval;
		}
		send_len++;
    }
    
    s_usb_snd_buff[0] = HDLC_FLAG ;
    s_usb_snd_buff[send_len++] = HDLC_FLAG;
    
    if(send_len == 0)
        return;
        
	//Ep3 max pack size is 64 bytes .
    if ((send_len >> 6) > 0){
    	for (i=0; i<(send_len >> 6); i++){
	    	USB_EPxSendData  (USB_EP3 ,(unsigned int * )(s_usb_snd_buff + (i<<6)),64);
    	}
    	if(send_len%0x40)
    	{
    	    USB_EPxSendData  (USB_EP3 ,(unsigned int * )(s_usb_snd_buff + (i<<6)),(send_len - (i<<6)));
    	}
    }
    else{
	    USB_EPxSendData  (USB_EP3 ,(unsigned int * )s_usb_snd_buff,send_len);
    } 

}

/*****************************************************************************/
//  Description:    turn off usb ldo 
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
uint8 usb_ldo_off(void)
{
	*(volatile unsigned long *) 0x8b00003C |= 1<<13;
	*(volatile unsigned long *) 0x8b00003C &= ~(1<<12);
	
	return TRUE;
}
//Add for USB 
void usb_irq (void)
{
    INT_CTRL_T * int_ctl_ptr = (INT_CTRL_T *)INT_REG_BASE;       
	USB_DEV_REG_T * ctl = (USB_DEV_REG_T *)USB_REG_BASE;
	USB_DEV_INT_STS_U status ;
	
	if( int_ctl_ptr->raw.dwValue  & (1<<25)){
		status.dwValue = ctl->int_sts.dwValue ;
		//dispatch handler message according to usb int status ;
		if (status.dwValue  & USB_INT_EPx_MASK )
		{
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
}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 

