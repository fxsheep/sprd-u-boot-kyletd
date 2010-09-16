/******************************************************************************
 ** File Name:    sc8800h_reg_gpio.h                                          *
 ** Author:       Aiguo.Miao	                                              *
 ** DATE:         08/28/2007                                                  *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 08/28/2007    Aiguo.Miao      Create.                                     *
 ******************************************************************************/
#ifndef _SC8800H_REG_GPIO_H_
    #define _SC8800H_REG_GPIO_H_
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
#define GPIO_BASE               		0x8A000000


#define GPIO_PG0_BASE           		(GPIO_BASE + 0*0x80)    //GPIO pin15~0 Register Base Address
#define GPIO_PG1_BASE           		(GPIO_BASE + 1*0x80)    //GPIO pin31~16 Register Base Address
#define GPIO_PG2_BASE           		(GPIO_BASE + 2*0x80)    //GPIO pin47~32 Register Base Address
#define GPIO_PG3_BASE           		(GPIO_BASE + 3*0x80)    //GPIO pin63~48 Register Base Address
#define GPIO_PG4_BASE           		(GPIO_BASE + 4*0x80)    //GPIO pin79~64 Register Base Address  
#define GPIO_PG5_BASE           		(GPIO_BASE + 5*0x80)    //GPIO pin95~80 Register Base Address  
#define GPIO_PG6_BASE           		(GPIO_BASE + 6*0x80)    //GPIO pin111~96 Register Base Address     
#define GPIO_PG7_BASE           		(GPIO_BASE + 7*0x80)    //GPIO pin127~112 Register Base Address  
#define GPIO_PG8_BASE           		(GPIO_BASE + 8*0x80)    //GPIO pin143~128 Register Base Address  
#define GPIO_PG9_BASE           		(GPIO_BASE + 9*0x80)    //GPIO pin159~144 Register Base Address  
#define GPIO_PG10_BASE           		(GPIO_BASE + 0xA*0x80)  //GPIO pin161~160 Register Base Address

#define GPIO_DATA               		0x0000    //GPIO data register
#define GPIO_DMSK               		0x0004    //GPIO data mask register, GPIO pin can be read and write if the mask bit is "1"
#define GPIO_DIR                		0x0008    //"1" configure pin to be output"0" configure pin to be input
#define GPIO_IS                 		0x000C    //Interrupt sense register. "1" detect levels, "0" detect edges
#define GPIO_IBE                		0x0010    //Interrupt both edges register. "1" both edges trigger an interrupt, "0" interrupt generation event is controlled by GPIOIEN
#define GPIO_IEV                		0x0014    //Interrupt event register, "1" rising edges or high levels trigger interrupts, "0" falling edges or low levels trigger interrupts.
#define GPIO_IE                 		0x0018    //Interrupt mask register, "1" corresponding pin is not masked. "0" corresponding pin interrupt is masked
#define GPIO_RIS                		0x001C    //Row interrupt status, reflect the status of interrupts trigger conditions detection on pins (prior to masking). "1" interrupt condition met "0" condition not met
#define GPIO_MIS                		0x0020    //Masked interrupt status, "1" Interrupt active "0" interrupt not active
#define GPIO_IC                 		0x0024    //Interrupt clear, "1" clears edge detection interrupt. "0" has no effect.
#define GPIO_ITCR               		0x0040    //Integration test control register, "1" In integration test mode "0" Normal mode
#define GPIO_ITOP1              		0x0044    //Integration test input read/set register. In integration test mode, writes specify the value to be driven on the GPIOMIS lines. Read GPIOMIS checks interrupt active.
#define GPIO_ITOP2              		0x0048    //Integration test input read/set register. In integration test mode, reads return the value of GPIOINTR.


//GPIO ARM Control.
typedef struct gpio_ctl_tag
{
    VOLATILE uint32 data;
    VOLATILE uint32 dmsk;
    VOLATILE uint32 dir;
    VOLATILE uint32 is;
    VOLATILE uint32 ibe;
    VOLATILE uint32 iev;
    VOLATILE uint32 ie;
    VOLATILE uint32 ris;
    VOLATILE uint32 mis;
    VOLATILE uint32 ic;
    VOLATILE uint32 itcr;
    VOLATILE uint32 itop1;
    VOLATILE uint32 itop2;
}gpio_ctl_s;

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

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

