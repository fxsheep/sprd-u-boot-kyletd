/*
 * (C) Copyright 2009 DENX Software Engineering
 * Author: John Rigby <jrigby@gmail.com>
 *
 * Based on mx27/generic.c:
 *  Copyright (c) 2008 Eric Jarrige <eric.jarrige@armadeus.org>
 *  Copyright (c) 2009 Ilya Yanok <yanok@emcraft.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <div64.h>
#include <netdev.h>
#include <asm/io.h>

unsigned long sprd_get_armclk(void)
{
	//return lldiv(CONFIG_SYS_PLL_MHZ,CONFIG_SYS_ARM_DIV);
	return 0;
}

unsigned long sprd_get_ahbclk(void)
{
	//return lldiv(CONFIG_SYS_PLL_MHZ,CONFIG_SYS_AHB_DIV);
	return 0;
}

unsigned long sprd_get_emcclk(void)
{
	//return lldiv(CONFIG_SYS_PLL_MHZ,CONFIG_SYS_EMC_DIV);
	return 0;
}

#if defined(CONFIG_DISPLAY_CPUINFO)
int print_cpuinfo (void)
{
	return 0;
}
#endif
