/*
 * (C) Copyright 2011 - 2012 Samsung Electronics
 * EXT4 filesystem implementation in Uboot by
 * Uma Shankar <uma.shankar@samsung.com>
 * Manjunatha C Achar <a.manjunatha@samsung.com>
 *
 * ext4ls and ext4load : Based on ext2 ls and load support in Uboot.
 *		       Ext4 read optimization taken from Open-Moko
 *		       Qi bootloader
 *
 * (C) Copyright 2004
 * esd gmbh <www.esd-electronics.com>
 * Reinhard Arlt <reinhard.arlt@esd-electronics.com>
 *
 * based on code from grub2 fs/ext2.c and fs/fshelp.c by
 * GRUB  --  GRand Unified Bootloader
 * Copyright (C) 2003, 2004  Free Software Foundation, Inc.
 *
 * ext4write : Based on generic ext4 protocol.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <common.h>
#include <malloc.h>
#include <ext_common.h>
#include <ext4fs.h>
#include <linux/stat.h>
#include <linux/time.h>
#include <asm/byteorder.h>
#include "ext4_common.h"

#include "make_ext4fs_main.h"

extern struct ext4_off_data ext4_pattern[];

/* 1 is in table; 0 is not in table */
static unsigned char in_table_data(int offset)
{
	int array;
	int ii;

	array = sizeof(ext4_pattern) / sizeof(struct ext4_off_data);

	for (ii = 0; ii < array; ii ++) {
		if (offset == ext4_pattern[ii].off)
			return ext4_pattern[ii].data;
	}
	
	return 0;
}

int make_ext4fs_main(unsigned char *buffer, int partlen)
{
        int ii;
	int array = sizeof(ext4_pattern) / sizeof(struct ext4_off_data);

	memset(buffer, 0, partlen);
	for (ii = 0; ii < array; ii ++)
		*(buffer + ext4_pattern[ii].off) = ext4_pattern[ii].data;

        return partlen;
}
