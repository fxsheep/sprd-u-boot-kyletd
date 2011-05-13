/******************************************************************************
 ** File Name:    gpio_reg_v3.h                                            *
 ** Author:       mingwei.zhang                                                 *
 ** DATE:         06/11/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/11/2010    mingwei.zhang   Create.                                     *
 ******************************************************************************/
#ifndef _GPIO_REG_V3_H_
#define _GPIO_REG_V3_H_
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

/*----------GPIO ARM Control Register----------*/
#define GPIO_PG_BASE                    GPIO_BASE
#define GPI_PG0_BASE                GPIO_BASE   //  GPI 5~0         Register Base Address       =GPIO_ID(15-0)
#define GPI_PG0_NUM                 0x08        //GPIO 2~0  

#define GPIO_PG0_BASE               (GPIO_PG_BASE + 0x0080) //  GPIO 15~0       Register Base Address   =GPIO_ID(15-0)
#define GPIO_PG0_NUM                      0x10  //  GPIO 15~0

#define GPIO_PG1_BASE               (GPIO_PG_BASE + 0x0100) //  GPIO 31~16  Register Base Address   =GPIO_ID(31-16)
#define GPIO_PG1_NUM                      0x10  //  GPIO 31~16

#define GPIO_PG2_BASE               (GPIO_PG_BASE + 0x0180) //  GPIO 47~32      Register Base Address   =GPIO_ID(47-32)
#define GPIO_PG2_NUM                      0x10  //  GPIO 47~32

#define GPIO_PG3_BASE               (GPIO_PG_BASE + 0x0200) //  GPIO 63~48  Register Base Address   =GPIO_ID(63-48)
#define GPIO_PG3_NUM                      0x10  //  GPIO 63~48

#define GPIO_PG4_BASE               (GPIO_PG_BASE + 0x0280) //  GPIO 79~64      Register Base Address   =GPIO_ID(79-64)
#define GPIO_PG4_NUM                      0x10  //  GPIO 79~64

#define GPIO_PG5_BASE               (GPIO_PG_BASE + 0x0300) //  GPIO 95~80  Register Base Address   =GPIO_ID(95-80)
#define GPIO_PG5_NUM                      0x10  //  GPIO 95~80

#define GPIO_PG6_BASE               (GPIO_PG_BASE + 0x0380) //  GPIO 111~96         Register Base Address   =GPIO_ID(111-96)
#define GPIO_PG6_NUM                      0x10  //  GPIO 111~96

#define GPIO_PG7_BASE               (GPIO_PG_BASE + 0x0400) //  GPIO 117~112    Register Base Address   =GPIO_ID(117-112)
#define GPIO_PG7_NUM                      0x06  //  GPIO 117~112



//GPIO_OFFSET
#define GPIO_DATA                               0x0000    //GPIO data register
#define GPIO_DMSK                               0x0004    //GPIO data mask register, GPIO pin can be read and write if the mask bit is "1"
#define GPIO_DIR                                0x0008    //"1" configure pin to be output"0" configure pin to be input
#define GPIO_IS                                 0x000C    //Interrupt sense register. "1" detect levels, "0" detect edges
#define GPIO_IBE                                0x0010    //Interrupt both edges register. "1" both edges trigger an interrupt, "0" interrupt generation event is controlled by GPIOIEN
#define GPIO_IEV                                0x0014    //Interrupt event register, "1" rising edges or high levels trigger interrupts, "0" falling edges or low levels trigger interrupts.
#define GPIO_IE                                 0x0018    //Interrupt mask register, "1" corresponding pin is not masked. "0" corresponding pin interrupt is masked
#define GPIO_RIS                                0x001C    //Row interrupt status, reflect the status of interrupts trigger conditions detection on pins (prior to masking). "1" interrupt condition met "0" condition not met
#define GPIO_MIS                                0x0020    //Masked interrupt status, "1" Interrupt active "0" interrupt not active
#define GPIO_IC                                 0x0024    //Interrupt clear, "1" clears edge detection interrupt. "0" has no effect.

//GPI_OFFSET
#define GPI_DATA                        0x0000    //GPI data register, original input signal, not through de-bounce path.
#define GPI_DMSK                        0x0004    //GPI data mask register. GPIDATA register can be read if the mask bit is "1"
#define GPI_IEV                         0x0014    //Interrupt event register, "1" high levels trigger interrupts, "0" low levels trigger interrupts.
#define GPI_IE                          0x0018    //Interrupt mask register, "1" corresponding pin is not masked. "0" corresponding pin interrupt is masked
#define GPI_RIS                         0x001C    //Row interrupt status, reflect the status of interrupts trigger conditions detection on pins (prior to masking). "1" interrupt condition met "0" condition not met
#define GPI_MIS                         0x0020    //Masked interrupt status, "1" Interrupt active "0" interrupt not active
#define GPI_IC                          0x0024    //Interrupt clear, "1" clears level detection interrupt. "0" has no effect.
#define GPI_0CTRL                       0x0028    //GPI0:...
#define GPI_1CTRL                       0x002C    //GPI1:...
#define GPI_2CTRL                       0x0030    //GPI2:...
#define GPI_3CTRL                       0x0034    //GPI3:...
#define GPI_4CTRL                       0x0038    //GPI4:...
#define GPI_5CTRL                       0x003C    //GPI5:...
#define GPI_6CTRL                       0x0040    //GPI5:...
#define GPI_7CTRL                       0x0044    //GPI5:...
#define GPI_TRIG                                0x0048

#define GPI_DEBOUNCE_BIT                 BIT_8
#define GPI_DEBOUNCE_PERIED                255

// Caculate the base addr of GPIO
#define GET_GPIO_BASE_ADDR(NUM)     ((((NUM)&0x7F)>>4)*0x80 + GPIO_PG0_BASE)

//GPIO ARM Control.
/*----------Analog Die GPIO----------*/
//--Analog Die GPIO
#define ANA_GPIO_PG_BASE            ANA_GPIO_BASE   //GPIO base address on analog die

#define ANA_GPIN_PG0_BASE           (ANA_GPIO_PG_BASE + 0x0000)  // analog GPIN 6-2,0
#define ANA_GPIN_PG0_NUM            0x08

#define ANA_GPIO_PG0_BASE           (ANA_GPIO_PG_BASE + 0x0080) // analog GPIO 15-0     Register Base Address   =GPIO_ID(143-128)
#define ANA_GPIO_PG0_NUM            0x10    //analog GPIO 15-0

#define ANA_GPIO_PG1_BASE           (ANA_GPIO_PG_BASE + 0x0100) // analog GPIO 28 ~ 16  Register Base Address   =GPIO_ID(159-144)
#define ANA_GPIO_PG1_NUM            0x0E    //analog GPIO 29-16

#define GET_ANA_GPIO_REG_ADDR(NUM)  ((((NUM)&0x1F)>>4)*0x80 + ANA_GPIO_PG0_BASE)
#define ANA_GPIO_BIT(NUM)           (1 << ((NUM)&0x0F))



/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/

#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_GPIO_REG_V3_H_
// End



