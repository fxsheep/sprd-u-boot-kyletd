/*
 *  linux/arch/arm/mach-sc8800s/mfp-sprd.c
 *
 *  Spreadtrum SoC multi-function pin configuration support
 *
 *  The GPIOs on SoC can be configured as one of many alternate
 *  functions,
 *
 *  Author:	Yingchun Li(yingchun.li@spreadtrum.com)
 *  Created:	March 10, 2010
 *  Copyright:	Spreadtrum Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
*/
//#define DEBUG

//#include <linux/module.h>
//#include <linux/kernel.h>
//#include <asm/io.h>
//#include <mach/mfp.h>
//#include <mach/regs_cpc.h>
#include <asm/arch/mfp.h>
#include <asm/arch/sc8810_reg_base.h>
#include <asm/arch/sc8810_cpc.h>
#include <asm/io.h>

/*
NOTE: pin to gpio's map, you should check it from your chip's spec
	carefully.
	in the map table, pin as the index, for mostly we use
	MFP_PIN_TO_GPIO to get the gpio from the pin

const static unsigned long pin_gpio_map[MFP_PIN_MAX] = {
	[MFP_PIN_MAX - 1] = 0xffff
};

*/

/*
	we cann't find a easy way to map pin to gpio, so drop it.
*/
unsigned long mfp_to_gpio(int pin)
{
	BUG_ON(1);
}

static  int __mfp_validate(unsigned long c)
{
	return 1;
}

static unsigned long __mfp_get_pin_reg(int pin_offset)
{
	if (!(pin_offset & A_DIE_PIN)) {
		return (unsigned long)PIN_CTL_BASE + pin_offset;
	} else {
		pin_offset &= ~A_DIE_PIN;
		return (unsigned long)ANA_PIN_CTL_BASE + pin_offset;
	}
}

#ifdef DEBUG
#define MFP_DBG(fmt...) pr_debug(fmt)
static unsigned long __mfp_get_physical(int pin_offset)
{
	if (!(pin_offset & A_DIE_PIN)) {
		return (unsigned long)SPRD_CPC_PHYS + pin_offset;
	} else {
		pin_offset &= ~A_DIE_PIN;
		return (unsigned long)SPRD_MISC_PHYS + 0x180 + pin_offset;
	}
}
#else
#define	MFP_DBG(fmt...)
#endif

static int __mfp_config_pin(unsigned long c)
{
	unsigned long flags;
	unsigned long pin_reg;
	unsigned long pin_cfg;
	int pin_offset;

	pin_offset = MFP_CFG_TO_REG_OFFS(c);
	pin_reg = __mfp_get_pin_reg(pin_offset);

	MFP_DBG("register is :0x%x, old config is %x\r\n",
		__mfp_get_physical(pin_offset),
		__raw_readl(pin_reg));

	//local_irq_save(flags);
	pin_cfg =__raw_readl(pin_reg);
	if (c & MFP_IO_SET) {
		pin_cfg = (pin_cfg & ~MFP_IO_MASK) | (c & MFP_IO_MASK);
	}

	if (c & MFP_S_PULL_SET) {
		pin_cfg = (pin_cfg & ~MFP_S_PULL_MASK) | (c & MFP_S_PULL_MASK);
	}

	if (c & MFP_AF_SET) {
		pin_cfg = (pin_cfg & ~MFP_AF_MASK) | (c & MFP_AF_MASK);
	}

	if (c & MFP_F_PULL_SET) {
		pin_cfg = (pin_cfg & ~MFP_F_PULL_MASK) | (c & MFP_F_PULL_MASK);
	}

	if (c & MFP_DS_SET) {
		pin_cfg = (pin_cfg & ~MFP_DS_MASK) | (c & MFP_DS_MASK);
	}

	__raw_writel(pin_cfg, pin_reg);
//	local_irq_restore(flags);

	MFP_DBG("new config is :%x\r\n", (int)pin_cfg);

	return 0;
}

static unsigned long spi_cs1_gpio_cfg = MFP_CFG_X(SPI_CSN0, GPIO, DS1, F_PULL_NONE, S_PULL_DOWN, IO_NONE);

void sprd_mfp_config(unsigned long *mfp_cfgs, int num)
{
	unsigned long flags;
	unsigned long *c;
	int res;
	int i;

	for (i = 0, c = mfp_cfgs; i < num; i++, c++) {

		res = __mfp_validate((*c));
		if (res < 0)
			continue;

	//	local_irq_save(flags);

		__mfp_config_pin(*c);

	//	local_irq_restore(flags);
	}
}
//EXPORT_SYMBOL_GPL(sprd_mfp_config);
