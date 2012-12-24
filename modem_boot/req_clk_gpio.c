/******************************************************************************
 ** File Name:      spi_phy_v0.c                                                 *
 ** Author:         liuhao                                                   *
 ** DATE:           06/28/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the physical layer of SPI device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2010     liuhao     Create.  
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <config.h>
#include <common.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
//#define __DEBUG__
//#define __SPI_MODE__

#define REG32(x)                        (*(volatile unsigned long *)(x))
#define GPIO_PRINT(x)	printf x

#define CHIP_REG_OR(reg_addr, value)    (*(volatile unsigned long *)(reg_addr) |= (unsigned long)(value))
#define CHIP_REG_AND(reg_addr, value)   (*(volatile unsigned long *)(reg_addr) &= (unsigned long)(value))
#define CHIP_REG_GET(reg_addr)          (*(volatile unsigned long *)(reg_addr))
#define CHIP_REG_SET(reg_addr, value)   (*(volatile unsigned long *)(reg_addr)  = (unsigned long)(value))

#define GEN0_REG_ADDR	  (0x8B000008)
#define GEN0_EN_GPIO      (BIT_5)
#ifdef __SPI_MODE__
#define	PINREG_SPICLK_REQ	(0x8C000000 + 0x44C)
#define SPI_CLK_REQ	 (65-16) // GPIO_PROD_APCP_SRDY
#else
#define	PINREG_SPICLK_REQ	(0x8C000000 + 0x398)
#define SPI_CLK_REQ	 (98-16) // GPIO_PROD_APCP_SRDY
#endif
//define gpio func
#define PIN_DS_3          (BIT_8|BIT_9)
#define PIN_DS_2          (BIT_9)
#define PIN_DS_1          (BIT_8)
#define PIN_DS_0          
#define PIN_FPU_EN        (BIT_7)
#define PIN_FUNC_DEF      (0x00<<4)       //Function select,BIT4-5
#define PIN_FUNC_3        (0x03<<4)
#define PIN_FUNC_2        (0x02<<4)
#define PIN_SPU_EN        (BIT_3)
#define PIN_O_EN          (BIT_0)           // Output enable in sleep mode
#define PIN_I_EN          (BIT_1)           // Input enable in sleep mode

#define mdelay(n)	udelay((n) * 1000)

extern int modem_status(void);

static unsigned long gpio_base_for_cs = ((SPI_CLK_REQ>>4) * 0x80 + (unsigned long) 0x8A000000); 
static unsigned long gpio_bit_num_for_cs = (SPI_CLK_REQ & 0xF); 
int	req_clk_status(void)
{
	if(modem_status() == 2)  {
		mdelay(10);
		return 0;
	} else {
		return REG32(gpio_base_for_cs) & (1UL<<gpio_bit_num_for_cs);
	}
}
/*********************************************************************************************************
** Function name: 
** Descriptions: Perform byte reversal on an array of longword.
**               0xABCD => 0xDCBA, corresponding to DMA Full Switch Mode
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
void req_clk_init(void)
{
	unsigned long reg_val = 0;
	*(volatile unsigned long *)(GEN0_REG_ADDR) |= GEN0_EN_GPIO;

	*(volatile unsigned long *)(PINREG_SPICLK_REQ) = PIN_FUNC_3|PIN_DS_3|PIN_O_EN;

	reg_val =  REG32(gpio_base_for_cs+0x4);
	reg_val &= ~(1UL<<gpio_bit_num_for_cs);
	reg_val |= (1<<gpio_bit_num_for_cs);
	CHIP_REG_SET((gpio_base_for_cs+0x4),reg_val);

	reg_val =  REG32(gpio_base_for_cs+0x8);
	reg_val &= ~(1UL<<gpio_bit_num_for_cs);
	CHIP_REG_SET((gpio_base_for_cs+0x8),reg_val);

	reg_val =  REG32 (gpio_base_for_cs+0x8);
	reg_val &= ~(1UL<<gpio_bit_num_for_cs);

	CHIP_REG_SET((gpio_base_for_cs),reg_val);	

}
void dump_gpio_register(void)
{
	GPIO_PRINT(("GPIO_PIN(0x%x--%d)  = 0x%8x\n",PINREG_SPICLK_REQ,SPI_CLK_REQ,REG32(PINREG_SPICLK_REQ)));
	GPIO_PRINT(("GPIO(0x%x --0x%x)      = 0x%8x\n",gpio_base_for_cs+0x4,gpio_base_for_cs,REG32(gpio_base_for_cs+0x4)));
	GPIO_PRINT(("GPIO(0x%x -- %d)      = 0x%8x\n",gpio_base_for_cs+0x8,gpio_bit_num_for_cs,REG32(gpio_base_for_cs+0x8)));
}
