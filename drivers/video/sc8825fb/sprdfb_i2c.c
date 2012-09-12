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
#include <common.h>

#include <asm/arch/tiger_lcd.h>

#include "sprdfb.h"


uint16_t sprdfb_i2c_init(struct sprdfb_device *dev)
{

	return 1;
}

uint16_t sprdfb_i2c_uninit(struct sprdfb_device *dev)
{

	return 1;
}

/*write i2c, reg is 8 bit, val is 8 bit*/
static int32_t sprdfb_i2c_write_8bits(uint8_t reg, uint8_t val)
{
	return 0;
}

/*read i2c, reg is 8 bit, val is 8 bit*/
static int32_t sprdfb_i2c_read_8bits(uint8_t reg, uint8_t *val)
{
	return 0;
}


/*write i2c, reg is 8 or 16 bit, val is 8 or 16bit*/
static int32_t sprdfb_i2c_write_16bits(uint16_t reg, uint16_t reg_is_8bit, uint16_t val, uint16_t val_is_8bit)
{
	return 0;
}

/*read i2c, reg is 8 or 16 bit, val is 8 or 16bit*/
static int32_t sprdfb_i2c_read_16bits(uint16_t reg, uint16_t reg_is_8bit, uint16_t *val, uint16_t val_is_8bit)
{
	return 0;
}


/*write i2c with burst mode*/
static int32_t sprdfb_i2c_write_burst(uint8_t* buf, int num)
{
	return 0;
}

struct ops_i2c sprdfb_i2c_ops = {
	.i2c_write_8bits = sprdfb_i2c_write_8bits,
	.i2c_read_8bits = sprdfb_i2c_read_8bits,
	.i2c_write_16bits = sprdfb_i2c_write_16bits,
	.i2c_read_16bits = sprdfb_i2c_read_16bits,
	.i2c_write_burst = sprdfb_i2c_write_burst,
};

