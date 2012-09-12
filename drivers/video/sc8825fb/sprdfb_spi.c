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


uint16_t sprdfb_spi_init(struct sprdfb_device *dev)
{
	return 1;
}

uint16_t sprdfb_spi_uninit(struct sprdfb_device *dev)
{
	return 1;
}

struct ops_spi sprdfb_spi_ops = {
};

