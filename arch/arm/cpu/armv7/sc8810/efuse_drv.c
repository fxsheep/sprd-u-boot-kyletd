/*
 * Copyright (C) 2012 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

//#include <linux/kernel.h>
//#include <linux/init.h>
//#include <linux/io.h>
//#include <linux/module.h>

//#include <mach/hardware.h>
//#include <mach/regs_global.h>
#include <common.h>
#include <asm/io.h>

#include <asm/arch/regs_global.h>

#include "ctl_efuse.h"

#define CTL_EFUSE_BASE_PHYS			( 0x89000000 )
#define CTL_EFUSE_BASE				( CTL_EFUSE_BASE_PHYS )
#define SCI_ADDRESS(_b_, _o_)			( (u32)(_b_) + (_o_) )

#define SCI_D(reg)						( *(volatile u32 *)(reg) )

//static void __iomem *ctl_efuse_base = 0;
void sci_efuse_poweron(void)
{
	//ctl_efuse_base = ioremap(CTL_EFUSE_BASE_PHYS, PAGE_SIZE);
	SCI_D(GR_GEN0) |= GEN0_EFUSE_EN;
	SCI_D(REG_EFUSE_PGM_PARA) |= BIT_EFUSE_VDD_ON;
	SCI_D(REG_EFUSE_PGM_PARA) |= BIT_CLK_EFS_EN;
}

void sci_efuse_poweroff(void)
{
	SCI_D(REG_EFUSE_PGM_PARA) &= ~BIT_PGM_EN;
	SCI_D(REG_EFUSE_PGM_PARA) &= ~BIT_CLK_EFS_EN;
	SCI_D(REG_EFUSE_PGM_PARA) &= ~BIT_EFUSE_VDD_ON;
	SCI_D(GR_GEN0) &= ~GEN0_EFUSE_EN;
	//if (ctl_efuse_base) {
	//	iounmap(ctl_efuse_base);
	//	ctl_efuse_base = 0;
	//}
}

int sci_efuse_read(unsigned blk)
{
	int busy = 0;
	BUG_ON(blk > (MASK_READ_INDEX >> SHIFT_READ_INDEX));

	SCI_D(REG_EFUSE_BLOCK_INDEX) = BITS_READ_INDEX(blk);
	SCI_D(REG_EFUSE_MODE_CTRL) |= BIT_RD_START;

	do {
		//TODO: timeout
		busy = SCI_D(REG_EFUSE_STATUS) & BIT_READ_BUSY;
	} while (busy);

	return SCI_D(REG_EFUSE_DATA_RD);
}

int sci_efuse_program(unsigned blk, int data)
{
	return 0;
}

int sci_efuse_is_locked(unsigned blk)
{
	return 0;
}

/* low level */
int sci_efuse_raw_write(unsigned blk, int data, u32 magic)
{
	SCI_D(REG_EFUSE_MAGIC_NUMBER) = BITS_MAGIC_NUMBER(magic);
	return 0;
}

int sci_efuse_lock(unsigned blk)
{
	return 0;
}

#define CAL_DATA_BLK	7
#define BASE_ADC_P0   785   //3.6V
#define BASE_ADC_P1   917   //4.2V
#define VOL_P0        3600
#define VOL_P1        4200
#define ADC_DATA_OFFSET 128
int sci_efuse_calibration_get(unsigned int * p_cal_data)
{
	int data;
    unsigned int cal_temp;
    unsigned short adc_temp;

	sci_efuse_poweron();
	data = sci_efuse_read(CAL_DATA_BLK);
	sci_efuse_poweroff();

	data &= ~(1 << 31); 
    
//    data = (173 |(171 << 8));
    printf("sci_efuse_calibration data:%d\n",data);
	if((!data)||(p_cal_data == NULL))
	{
		return 0;
	}
    //adc 3.6V 
    adc_temp = ((data>>8) & 0x00FF) + BASE_ADC_P0 - ADC_DATA_OFFSET;
    p_cal_data[1] = (VOL_P0)|(adc_temp << 16);

    //adc 4.2V
	adc_temp = (data & 0x00FF) + BASE_ADC_P1 - ADC_DATA_OFFSET;
    p_cal_data[0] = (VOL_P1)|(adc_temp << 16);

	return 1;
}

