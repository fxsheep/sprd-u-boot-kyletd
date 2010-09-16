/******************************************************************************
 ** File Name:    sc8800h_reg_usb.h                                            *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         11/13/2005                                                  *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 11/13/2005    Daniel.Ding     Create.                                     *
 ******************************************************************************/
#ifndef _SC8800H_REG_USB_
    #define _SC8800H_REG_USB_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
/*----------USB Ctronl Registers----------*/
#define USB_REG_BASE            		0x20300000
#define USB_DEV_CTRL                	(USB_REG_BASE + 0x0000)
#define USB_DEV_STS                		(USB_REG_BASE + 0x0004)
#define USB_DEV_RESET              		(USB_REG_BASE + 0x0008)
#define USB_EPx_RESET              		(USB_REG_BASE + 0x000C)
#define USB_SIE_CTRL               		(USB_REG_BASE + 0x0010)
#define USB_INT_CTRL               		(USB_REG_BASE + 0x0014)
#define USB_INT_STS               		(USB_REG_BASE + 0x0018)
#define USB_INT_CLR               		(USB_REG_BASE + 0x001C)
#define USB_INT_RAW               		(USB_REG_BASE + 0x0020)
#define USB_FRAME_NUM              		(USB_REG_BASE + 0x0024)
#define USB_TIME_SET              		(USB_REG_BASE + 0x0028)

//Ep0
#define USB_EP0_BASE					(USB_REG_BASE + 0x40)
#define USB_TRANSF_SIZE_EP0_IN     		(USB_REG_BASE + 0x0040)
#define USB_TRANSF_SIZE_EP0_OUT    		(USB_REG_BASE + 0x0044)
#define USB_SEND_DATA_NUM_EP0	   		(USB_REG_BASE + 0x0048)
#define USB_RECEIVED_DATA_NUM_EP0  		(USB_REG_BASE + 0x004C)
#define USB_REMOTE				  		(USB_REG_BASE + 0x0050)
#define USB_CONFIG				  		(USB_REG_BASE + 0x0054)
#define USB_SETTING				  		(USB_REG_BASE + 0x0058)
#define USB_SETUP_LOW			  		(USB_REG_BASE + 0x005C)
#define USB_SETUP_HIGH			  		(USB_REG_BASE + 0x0060)
#define USB_EP0_CTRL			  		(USB_REG_BASE + 0x0064)
#define USB_EP0_INT_CTRL			 	(USB_REG_BASE + 0x0068)
#define USB_EP0_INT_STS			  		(USB_REG_BASE + 0x006C)
#define USB_EP0_INT_CLR			  		(USB_REG_BASE + 0x0070)
#define USB_EP0_INT_RAW			  		(USB_REG_BASE + 0x0074)
#define USB_EP0_DMA_IN_WAITE_TIME  		(USB_REG_BASE + 0x0078)
#define USB_EP0_DMA_OUT_WAITE_TIME 		(USB_REG_BASE + 0x007C)

//Epx
#define USB_EP_CTRL				  		(0x0000) // 0X64
#define USB_EP_SEND_DATA	    	 	(0x0004) // 0X68
#define USB_EP_RECEIVED_DATA	  		(0x0008) // 0X6C
#define USB_EP_INT_CTRL				 	(0x000C) // 0X68
#define USB_EP_INT_STS			  		(0x0010) // 0X6C
#define USB_EP_INT_CLR			  		(0x0014) // 0X70
#define USB_EP_INT_RAW			  		(0x0018) // 0X74
#define USB_EP_DMA_WAITE_TIME	  		(0x001C) // 0X78

#define USB_EPx_BASE					(USB_REG_BASE + 0xc0)

#define USB_EP1_BASE					(USB_EPx_BASE + 0x40*0)
#define USB_EP2_BASE					(USB_EPx_BASE + 0x40*1)
#define USB_EP3_BASE					(USB_EPx_BASE + 0x40*2)
#define USB_EP4_BASE					(USB_EPx_BASE + 0x40*3)

#define USB_EPx_CTRL(x)				  	(USB_EPx_BASE + 0x40*x + USB_EP_CTRL ) // 0X64
#define USB_EPx_SEND_DATA(x)	    	(USB_EPx_BASE + 0x40*x + USB_EP_SEND_DATA) // 0X68
#define USB_EPx_RECEIVED_DATA(x)	  	(USB_EPx_BASE + 0x40*x + USB_EP_RECEIVED_DATA) // 0X6C
#define USB_EPx_INT_CTRL(x)				(USB_EPx_BASE + 0x40*x + USB_EP_INT_CTRL) // 0X68
#define USB_EPx_INT_STS(x)			  	(USB_EPx_BASE + 0x40*x + USB_EP_INT_STS) // 0X6C
#define USB_EPx_INT_CLR(x)			  	(USB_EPx_BASE + 0x40*x + USB_EP_INT_CLR) // 0X70
#define USB_EPx_INT_RAW(x)			  	(USB_EPx_BASE + 0x40*x + USB_EP_INT_RAW) // 0X74
#define USB_EPx_DMA_WAITE_TIME(x)	  	(USB_EPx_BASE + 0x40*x + USB_EP_DMA_WAITE_TIME) // 0X78

#define USB_EP0_IN_FIFO					(USB_REG_BASE + 0x80000)
#define USB_EP1_FIFO					(USB_REG_BASE + 0x80004)
#define USB_EP3_FIFO					(USB_REG_BASE + 0x80008)
#define USB_EP0_OUT_FIFO				(USB_REG_BASE + 0x8000c)
#define USB_EP2_FIFO					(USB_REG_BASE + 0x80010)
#define USB_EP4_FIFO					(USB_REG_BASE + 0x80014)

#define USB_FIFO_MAX_WORD               16
#define USB_FIFO_MAX_BYTE               64
#define USB_MAX_TRANSFER_SIZE			(64*1024)

#define USB_CTRL_END	            		0x203FFFFF


// USB EPx ID 
#define USB_EP0_IN						0
#define USB_EP0_OUT						1
#define USB_EP1							2
#define USB_EP2							3
#define USB_EP3							4
#define USB_EP4							5
#define USB_EPx_NUMBER					6


#define USB_INT_SOF						(BIT_0)
#define USB_INT_SPEND					(BIT_1)
#define USB_INT_RESUM					(BIT_2)
#define USB_INT_RST						(BIT_3)
#define USB_INT_UNPID					(BIT_4)
#define USB_INT_EPID					(BIT_5)
#define USB_INT_CHANGE					(BIT_6)
#define USB_INT_DEVICE					(BIT_7)
#define USB_INT_EP0						(BIT_8)
#define USB_INT_EP1						(BIT_9)
#define USB_INT_EP2						(BIT_10)
#define USB_INT_EP3						(BIT_11)
#define USB_INT_EP4						(BIT_12)

#define USB_INT_DEV_MASK				0x7F
#define USB_INT_EPx_MASK				0x3FFF

#define USB_CLK_12M						12000000
#define USB_CLK_48M						48000000
/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef union _usb_dev_ctrl_tag {
	struct _usb_dev_ctrl_map {
		volatile unsigned int reserved       	:30;//Reserved ;
		volatile unsigned int power				:1;	//Slef powered ;
		volatile unsigned int en				:1;	//Device enable ;
	}mBits ;
	volatile unsigned int dwValue ;
}USB_DEV_CTRL_U;

typedef union usb_sts_tag {
	struct usb_sts_map{
		volatile unsigned int reserved 			:12;
		volatile unsigned int wk_en				:1;	//Remote wakeup enable ;
		volatile unsigned int ep_num				:4;	//Ep number what is using
		volatile unsigned int i_num				:5;	//Interface number;
		volatile unsigned int state				:3;	//Device current state ;
		volatile unsigned int address			:7;	//Device address;
	}mBits ; 
    volatile unsigned int dwValue ;
}USB_DEV_STS_U;


typedef union usb_dev_rst_tag {
	struct usb_dev_rst_map {
		volatile unsigned int reserved			:31;//Reserved ;
		volatile unsigned int reset				:1;	//Device reset ;
	}mBits;
	volatile unsigned int dwValue ;
}USB_DEV_RST_U;

typedef union usb_epx_rst_tag {
	struct usb_epx_rst_map {
		volatile unsigned int reserved			    :21;//Reserved ;
		volatile unsigned int ep4_toggle_rst	    :1;	//Data toggle reset for end point 4 ;
		volatile unsigned int ep3_toggle_rst	    :1;	//Data toggle reset for end point 3 ;
		volatile unsigned int ep2_toggle_rst	    :1;	//Data toggle reset for end point 2 ;
		volatile unsigned int ep1_toggle_rst	    :1;	//Data toggle reset for end point 1 ;
		volatile unsigned int ep0_toggle_rst	    :1;	//Data toggle reset for end point 0 ;
		volatile unsigned int ep4_fifo_rst  		:1;	//Fifo reset for end point 4 ;
		volatile unsigned int ep3_fifo_rst	    	:1;	//Fifo reset for end point 3 ;
		volatile unsigned int ep2_fifo_rst		    :1;	//Fifo reset for end point 2 ;
		volatile unsigned int ep1_fifo_rst		    :1;	//Fifo reset for end point 1 ;
		volatile unsigned int ep0_out_fifo_rst		:1;	//Fifo reset for end point 0 Out;
		volatile unsigned int ep0_in_fifo_rst	    :1;	//Fifo reset for end point 0 In ;
	}mBits ;
	volatile unsigned int dwValue ;
}USB_EPx_RST_U;

typedef union usb_sie_ctrl_tag {
	struct usb_sie_ctrl_map{
		volatile unsigned int reserved_0			:30;//Reserved ;
		volatile unsigned int timeout			    :1;	//SIE rx EOP signal timeout enable;
		volatile unsigned int mode				    :1;	//SIE rx detect sync mode ;
	}mBits ;
	volatile unsigned int dwValue ;
}USB_SIE_CTRL_U;

typedef union usb_dev_int_tag {
	struct usb_dev_int_map {
		volatile unsigned int reserved 			:25;//reserved 25 bits
	    volatile unsigned int state				:1;	//State of device changed
		volatile unsigned int error			    :1;	//Received a error PID ;
		volatile unsigned int unpid 			:1;	//Received unsupported PID ;
		volatile unsigned int host_rst			:1; //Reset from host ;
		volatile unsigned int resume 			:1;	//Detect a J to K state change when 
											        //the device is in the suspend state ;
		volatile unsigned int suspend			:1;	//Device into suspend state;
		volatile unsigned int sof				:1;	//Start of frame packet;
	}mBits;
	volatile unsigned int dwValue ;
}USB_DEV_INT_U;

typedef union usb_dev_int_sts_tag {
	struct usb_dev_int_sts_map {
		volatile unsigned int reserved 			:19;//reserved 19 bits
		volatile unsigned int ep4	 			:1;	//Select ep4 interrupt status register;
		volatile unsigned int ep3 				:1;	//Select ep3 interrupt status register;
		volatile unsigned int ep2 				:1;	//Select ep2 interrupt status register;
		volatile unsigned int ep1 				:1;	//Select ep1 interrupt status register;
		volatile unsigned int ep0 				:1;	//Select ep0 interrupt status register;
		volatile unsigned int device 			:1;	//Select device interrupt status register;
	    volatile unsigned int state				:1;	//State of device changed
		volatile unsigned int error			    :1;	//Received a error PID ;
		volatile unsigned int unpid 			:1;	//Received unsupported PID ;
		volatile unsigned int host_rst			:1; //Reset from host ;
		volatile unsigned int resume 			:1;	//Detect a J to K state change when 
											        //the device is in the suspend state ;
		volatile unsigned int suspend			:1;	//Device into suspend state;
		volatile unsigned int sof				:1;	//Start of frame packet;
	}mBits ;
	volatile unsigned int dwValue ;
}USB_DEV_INT_STS_U;

typedef union usb_frame_num_tag {
	struct usb_frame_num_map {
		volatile unsigned int reserved 			:21;//Reserved ;
		volatile unsigned int num				:11;//Frame number 
	}mBits;
	volatile unsigned int dwValue ;
}USB_FRAME_NUM_U;


typedef union usb_epx_data_tag {
	struct usb_epx_data_map {
		volatile unsigned int reserved 			:15;//Reserved ;
		volatile unsigned int num				:17;//Total size (byte number) in 
	}mBits;
	volatile unsigned int dwValue ;
}USB_EPx_DATA_U;

typedef union usb_wakeup_tag {
	struct usb_wakeup_map {
		volatile unsigned int reserved 			:31;//Reserved ;
		volatile unsigned int wakeup			:1;	//send remote wakeup to host ;
	}mBits;
	volatile unsigned int dwValue ;
}USB_WAKEUP_U;


typedef union usb_dev_cnf_tag {
	struct usb_dev_cfg_map {
		volatile unsigned int reserved 			:24;//Reserved ;
		volatile unsigned int wakeup			:8;	//Configuration value coming 
										        	//from host ;
	}mBits;
	volatile unsigned int dwValue ;
}USB_DEV_CNF_U;

typedef union usb_setup_low_tag{
	struct usb_setup_low_map {
		volatile unsigned int value				:16;//request value
		volatile unsigned int request 			:8;	//request type ;
		volatile unsigned int direction			:1;	//select data direction ;
		volatile unsigned int type 				:2;	//Specific request type
		volatile unsigned int recipient			:5;	//Request recipient ;
	}mBits;
	volatile unsigned int dwValue ;
}USB_SETUP_LOW_U;


typedef union usb_setup_high_tag {
	struct SETUP_HIGH_BIT_MAP {
		volatile unsigned int length 			:16;//request length ;
		volatile unsigned int index				:16;//request index
	}mBits;
	volatile unsigned int dwValue ;
}USB_SETUP_HIGH_U;

typedef union usb_ep0_ctrl_tag {
	struct usb_ep0_ctrl_map{
		volatile unsigned int reserved_2		:2;
		volatile unsigned int stall				:1;	//Device send STALL to host;
		volatile unsigned int buf_ready			:1;	//buffer ready of ep0;
		volatile unsigned int data_ready		:1;	//Data ready of ep0;
		volatile unsigned int reserved_1		:4;
		volatile unsigned int pack_size			:11;//max payload size in byte;
		volatile unsigned int s_int_en			:1;	//setup transaction interrupt en;
		volatile unsigned int p_num	 			:11;//show hte bytes number in current
											        //data packet received from host;
	}mBits;
	volatile unsigned int dwValue ;
}USB_EP0_CTRL_U;

typedef union usb_epx_int_tag {
	struct usb_epx_int_map {
		volatile unsigned int reserved			:18;//Reserved 18 bits
		volatile unsigned int data_not_ready	:1;	//Ep0 buffer not ready to send;
		volatile unsigned int buf_not_ready		:1;	//Ep0 buffer not ready to receive;
		volatile unsigned int dma_out 			:1;	//Ep0 out wait dam ack time out ;
		volatile unsigned int dma_in			:1;	//Ep0 in wati dma ack time out;
		volatile unsigned int transfer_end		:1;	//Transfer end of control ep 0
		volatile unsigned int setup_end			:1;	//Setup transaction end;
		volatile unsigned int buf_full			:1;	//Buffer full when receive data 
										        	//from host and sent to ARM
		volatile unsigned int buf_empty			:1;	//Buffer emtpy when receive data 
											        //from ARM and sent to host ;
		volatile unsigned int crc16_err			:1;	//CRC16 error;
		volatile unsigned int setup				:1;	//Setup wait data packet timeout;
		volatile unsigned int out_timeout		:1;	//Out wait data packet timeout;
		volatile unsigned int in_timeout		:1;	//In wait ack packet timeout ;
		volatile unsigned int tog_err			:1;	//Data toggle error;
		volatile unsigned int transaction_end	:1;	//Transaction end ;
	}mBits;
	volatile unsigned int dwValue ;
}USB_EPx_INT_U;

typedef union usb_epx_ctrl_tag {
	struct usb_epx_ctrl_map {
		volatile unsigned int reserved_2		:2;
		volatile unsigned int stall				:1;	//Device send STALL to host;
		volatile unsigned int buf_ready			:1;	//buffer ready of ep0;
		volatile unsigned int data_ready		:1; //Bit 27
		volatile unsigned int active			:1;	//transize epx register be loaded;
		volatile unsigned int ep_en				:1;	//Enable this ep
		volatile unsigned int type				:2;	//Transfer type
		volatile unsigned int pack_size			:11;//max payload size in byte;
		volatile unsigned int halt_en			:1;	//epx halt en signal coming from host;
		volatile unsigned int p_num	 			:11;//show hte bytes number in current
	}mBits;
	volatile unsigned int dwValue ;
}USB_EPx_CTRL_U;

typedef union usb_dam_time_out_tag {
	struct usb_dam_time_out_map {
		volatile unsigned int reserved			:16;//Reserved
		volatile unsigned int time	 			:16;//Time out
	}mBits;
	volatile unsigned int dwValue ;
}USB_TIME_OUT_U;


//USB_REG_BASE            		0x20300000
typedef struct usb_dev_reg_tag
{
	volatile USB_DEV_CTRL_U 	ctrl;
	volatile USB_DEV_STS_U 		sts;
	volatile USB_DEV_RST_U 		sys_rest;
	volatile USB_EPx_RST_U  	ep_rest;
	volatile USB_SIE_CTRL_U 	sie;
	volatile USB_DEV_INT_U 		int_ctrl;
	volatile USB_DEV_INT_STS_U 	int_sts;
	volatile USB_DEV_INT_U 		int_clr;
	volatile USB_DEV_INT_U 		int_raw;
	volatile USB_FRAME_NUM_U	f_num;
}USB_DEV_REG_T;

//USB_EP0_BASE					(USB_REG_BASE + 0x40)
typedef struct usb_ep0_ctrl_reg_tag {
	volatile USB_EPx_DATA_U		transfer_size_in;		//0x40
	volatile USB_EPx_DATA_U		transfer_size_out;		
	volatile USB_EPx_DATA_U		send_num;
	volatile USB_EPx_DATA_U		receive_num;		
	volatile USB_WAKEUP_U		wakeup;
	volatile USB_DEV_CNF_U		config;
	volatile unsigned int	    alternate;
	volatile USB_SETUP_LOW_U	setup_low ;
	volatile USB_SETUP_HIGH_U	setup_high;		//0x60
	volatile USB_EP0_CTRL_U		ctrl;
	volatile USB_EPx_INT_U		int_ctrl;
	volatile USB_EPx_INT_U		int_sts;
	volatile USB_EPx_INT_U		int_clr;		//0x70
	volatile USB_EPx_INT_U		int_raw;
	volatile USB_TIME_OUT_U		dma_in_time;
	volatile USB_TIME_OUT_U		dma_out_time;	//0x7c
}USB_EP0_REG_T;

//USB_EPx_BASE					(USB_REG_BASE + 0xc0)
typedef struct _usb_epx_reg_tag
{
	volatile USB_EPx_CTRL_U		ctrl;			//0xc0
	volatile USB_EPx_DATA_U		receive_num;	//Read only 
	volatile USB_EPx_DATA_U		transfer_size;  //Write only 
	volatile USB_EPx_INT_U		int_ctrl;
	volatile USB_EPx_INT_U		int_sts;		//d0
	volatile USB_EPx_INT_U		int_clr;
	volatile USB_EPx_INT_U		int_raw;
	volatile USB_TIME_OUT_U		dma_time;
}USB_EPx_REG_T;
/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 

