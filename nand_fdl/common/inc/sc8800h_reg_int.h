/********************************************************************************File name:    sc8800h_reg_int.h                                            *
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
 ** 01/24/2007    Aiguo.Miao      Port to SC8800H                             *
 ******************************************************************************/
#ifndef _SC8800H_REG_INT_
    #define _SC8800H_REG_INT_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
/*----------Interrupt Control Registers----------*/
#define INT_REG_BASE            		0x20a00000

#define INT_IRQ_BASE            		(INT_REG_BASE + 0x0000)
#define INT_FIQ_STS             		(INT_REG_BASE + 0x0000)
#define INT_IRQ_STS             		(INT_REG_BASE + 0x0004)
#define INT_IRQ_RAW_STS         		(INT_REG_BASE + 0x0008)
#define INT_IRQ_SEL         			(INT_REG_BASE + 0x000c)
#define INT_IRQ_EN              		(INT_REG_BASE + 0x0010)
#define INT_IRQ_DISABLE         		(INT_REG_BASE + 0x0014) 
#define INT_IRQ_SOFT            		(INT_REG_BASE + 0x0018)
#define INT_IRQ_SOFT_CLR           		(INT_REG_BASE + 0x001C)
#define INT_IRQ_PROTECT            		(INT_REG_BASE + 0x0020)
#define INT_VADDR                       (INT_REG_BASE + 0x0024)
#define INT_DEFVADDR                    (INT_REG_BASE + 0x0028)
#define INT_VCTL_BASE                   (INT_REG_BASE + 0x0030)
#define INT_VADDR_BASE                  (INT_REG_BASE + 0x006C)
#define INT_IRQFIQ_UARTSTS              (INT_REG_BASE + 0x00AC)
#define INT_PMASK                       (INT_REG_BASE + 0x00B0)
#define INT_VADDR2                      (INT_REG_BASE + 0x00B4)
#define INT_UART_INT_CFG                (INT_REG_BASE + 0x00B8)
#define INT_INTEN_DSP                   (INT_REG_BASE + 0x00BC)


//The corresponding bit of all INT_CTL registers.
#define INTCTL_UART_SLEEP_IRQ			(1 << 0)
#define INTCTL_SDIO_IRQ         		(1 << 1)
#define INTCTL_COMMRX           		(1 << 2)
#define INTCTL_COMMTX           		(1 << 3)
#define INTCTL_CNT1_IRQ         		(1 << 4)
#define INTCTL_CNT2_IRQ         		(1 << 5)
#define INTCTL_GPIO_IRQ         		(1 << 6)
#define INTCTL_RTC_IRQ          		(1 << 7)
#define INTCTL_KPD_IRQ          		(1 << 8)
#define INTCTL_I2C_IRQ          		(1 << 9)
#define INTCTL_SIM_IRQ          		(1 << 10)
#define INTCTL_CX_SEM_HINT_IRQ          (1 << 11)
#define INTCTL_CX_CR_HINT_IRQ        	(1 << 12)
#define INTCTL_DSP_IRQ          		(1 << 13)
#define INTCTL_ADC_IRQ          		(1 << 14)
#define INTCTL_GEA_POOL_IRQ     		(1 << 15)
#define INTCTL_SYST_IRQ         		(1 << 16)
#define INTCTL_RFT_IRQ         			(1 << 17)
#define INTCTL_UART2_3_IRQ        		(1 << 18)
#define INTCTL_UART2_IRQ        		(1 << 18)
#define INTCTL_UART3_IRQ        		(1 << 18)
#define INTCTL_DSP_INT_OR_IRQ        	(1 << 19)
#define INTCTL_DMA_IRQ        			(1 << 20)
#define INTCTL_VBC_IRQ        			(1 << 21)
#define INTCTL_MEA_IRQ        			(1 << 22)
#define INTCTL_DCT_IRQ        			(1 << 23)
#define INTCTL_UART0_1_IRQ        		(1 << 24)
#define INTCTL_UART0_IRQ        		(1 << 24)
#define INTCTL_UART1_IRQ        		(1 << 24)
#define INTCTL_USBD_IRQ        			(1 << 25)
#define INTCTL_ISP_IRQ        			(1 << 26)
#define INTCTL_NLC_IRQ        			(1 << 27)
#define INTCTL_LCDC_IRQ        			(1 << 28)
#define INTCTL_PCM_IRQ        			(1 << 29)
#define INTCTL_DRM_IRQ        			(1 << 30)
#define INTCTL_PBINT_IRQ       			(1 << 31)
#define INTCTL_ICLR_ALL         		0xFFFFFFFF


/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef union _INT_BIT_U{
	struct INT_BIT_MAP {
		volatile int	pb				:1; //bit 31
		volatile int	drm				:1;
		volatile int	pcm     		:1;
		volatile int	lcdc			:1;
		volatile int	nlc  			:1;
		volatile int	isp 			:1;
		volatile int	usb				:1;
		volatile int	uart_0_1		:1;
		volatile int	dct  			:1;
		volatile int	mea 			:1;
		volatile int	vbc				:1;
		volatile int	dma				:1;
		volatile int	dsp_int_or		:1;
		volatile int	uart_2_3		:1;
		volatile int	rft				:1;
		volatile int	counter			:1;
		volatile int	gea				:1;
		volatile int	adc				:1;
		volatile int	dsp				:1;
		volatile int	cx_cr_hint		:1;
		volatile int	cx_sem_hint		:1;
		volatile int	sim				:1;
		volatile int	i2c				:1;
		volatile int	kpd				:1;
		volatile int	rtc				:1;
		volatile int	gpio			:1;
		volatile int	timer2			:1;
		volatile int	timer1			:1;
		volatile int	commtx			:1;
		volatile int	commrx			:1;
		volatile int	sdio			:1;
		volatile int	uart_sleep		:1; //bit 0
	}mBits;
	volatile int dwValue ;
}INT_BIT_U;

typedef union _INT_IRQFIQ_UARTSTS_U{
	struct INT_IRQFIQ_UARTSTS_MAP {
	    volatile int    reserved        :9;
		volatile int	gpio		    :1; //bit 22    //spec no this bit
		volatile int	sdio		    :1; //bit 21    //spec no this bit
		volatile int	usbd_se0        :1; //bit 20    //spec no this bit
		volatile int	ctsn3			:1; //bit 19
		volatile int	dsrn3			:1; //bit 18
		volatile int	rxd3			:1; //bit 17
		volatile int	ctsn2			:1; //bit 16
		volatile int	dsrn2			:1; //bit 15
		volatile int	rxd2			:1; //bit 14
		volatile int	ctsn1			:1; //bit 13
		volatile int	dsrn1			:1; //bit 12
		volatile int	rxd1			:1; //bit 11
		volatile int	ctsn0			:1; //bit 10
		volatile int	dsrn0			:1; //bit 9
		volatile int	rxd0			:1; //bit 8
		volatile int	rsvd1			:6; //bit 2--bit7
	    volatile int	irq			    :1; //bit 1
		volatile int	fiq			    :1; //bit 0
	}mBits;
	volatile int dwValue ;
}INT_IRQFIQ_UARTSTS_U;


typedef union _INT_UART_CFG_U{
	struct INT_UART_CFG_MAP {
		volatile int    reserved_1      :1; //bit 31-32
		volatile int	gpio_pol		:1; //bit 30
		volatile int	sdio_pol		:1; //bit 29
		volatile int	usbd_se0_pol    :1; //bit 28
		volatile int	ctsn3_pol		:1; //bit 27
		volatile int	dsrn3_pol		:1; //bit 26
		volatile int	rxd3_pol		:1; //bit 25
		volatile int	ctsn2_pol		:1; //bit 24
		volatile int	dsrn2_pol		:1; //bit 23
		volatile int	rxd2_pol		:1; //bit 22
		volatile int	ctsn1_pol		:1; //bit 21
		volatile int	dsrn1_pol		:1; //bit 20
		volatile int	rxd1_pol		:1; //bit 19
		volatile int	ctsn0_pol		:1; //bit 18
		volatile int	dsrn0_pol		:1; //bit 17
		volatile int	rxd0_pol		:1; //bit 16
		volatile int    clr		        :1; //bit 15 clear interrupt
        volatile int    gpio_irq        :1; //bit 14
        volatile int    sdio_irq        :1; //bit 13
        volatile int    usbd_se0_wakeup :1; //bit 12
		volatile int	ctsn3_en		:1; //bit 11
		volatile int	dsrn3_en		:1; //bit 10
		volatile int	rxd3_en			:1; //bit 9
		volatile int	ctsn2_en		:1; //bit 8
		volatile int	dsrn2_en		:1; //bit 7
		volatile int	rxd2_en			:1; //bit 6
		volatile int	ctsn1_en		:1; //bit 5
		volatile int	dsrn1_en		:1; //bit 4
		volatile int	rxd1_en			:1; //bit 3
		volatile int	ctsn0_en		:1; //bit 2
		volatile int	dsrn0_en		:1; //bit 1
		volatile int	rxd0_en			:1; //bit 0
	}mBits;
	volatile int dwValue ;
}INT_UART_CFG_U;


#define INT_VADDR                       (INT_REG_BASE + 0x0024)
#define INT_DEFVADDR                    (INT_REG_BASE + 0x0028)
#define INT_VCTL_BASE                   (INT_REG_BASE + 0x0030)
#define INT_VADDR_BASE                  (INT_REG_BASE + 0x006C)
#define INT_IRQFIQ                      (INT_REG_BASE + 0x00AC)
#define INT_PMASK                       (INT_REG_BASE + 0x00B0)
#define INT_VADDR2                      (INT_REG_BASE + 0x00B4)
#define INT_UART_INT_CFG                (INT_REG_BASE + 0x00B8)
#define INT_INTEN_DSP                   (INT_REG_BASE + 0x00BC)

//IRQ Interrupt Control.
typedef struct INT_IRQ_S_
{
    volatile INT_BIT_U 		fiq_sts;
    volatile INT_BIT_U 		irq_sts;
    volatile INT_BIT_U 		raw;
    volatile INT_BIT_U 		int_sel;
    volatile INT_BIT_U 		en;
    volatile INT_BIT_U 		clr;
    volatile INT_BIT_U 		int_soft;
    volatile INT_BIT_U 		int_soft_clr;
	volatile INT_BIT_U 		int_prot;
//  vector int TBD
}INT_CTRL_T;

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

