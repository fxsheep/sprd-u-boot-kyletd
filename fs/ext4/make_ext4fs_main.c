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
#include "make_ext4fs.h"

extern struct fs_info info;

/*********************** Richard Feng will delete the following code **********************/
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

int make_ext4fs_main_pattern(unsigned char *buffer, int partlen)
{
        int ii;
	int array = sizeof(ext4_pattern) / sizeof(struct ext4_off_data);

	memset(buffer, 0, partlen);
	for (ii = 0; ii < array; ii ++)
		*(buffer + ext4_pattern[ii].off) = ext4_pattern[ii].data;

        return partlen;
}
/*********************** Richard Feng will delete above code **********************/

int make_ext4fs_main(disk_partition_t *partinfo)
{
        const char *filename = "out.img";
        const char *directory = "abc";
        char *mountpoint = "";
        int android = 0;
        int gzip = 0;
        int sparse = 0;

	memset(&info, 0, sizeof(struct fs_info));

	printf("partition info : start = %d  size = %d  blksz = %d\n", partinfo->start, partinfo->size, partinfo->blksz);
	info.len = partinfo->size * partinfo->blksz;
	info.block_size = partinfo->blksz * 2;
	if (info.block_size != 1024)
		info.block_size = 1024; /* richardfeng note : info.block_size must be 1024 */
	info.no_journal = 1;  /* richardfeng note : info.no_journal must be 1 */

	printf("filename = %s  directory = %s  mountpoint = %s  android = %d  gzip = %d  sparse = %d no_journal = %d\n", filename, directory, mountpoint, android, gzip, sparse, info.no_journal);
	//printf("len = 0x%16Lx  block_size = %d, blocks_per_group = %d, inodes_per_group = %d, inode_size = %d, inodes = %d, journal_blocks = %d, feat_ro_compat = %d, feat_compat = %d, feat_incompat = %d, label = %s, no_journal = %d\n", info.len, info.block_size, info.blocks_per_group, info.inodes_per_group, info.inode_size, info.inodes, info.journal_blocks, info.feat_ro_compat, info.feat_compat, info.feat_incompat, info.label, info.no_journal);

        return make_ext4fs(filename, directory, mountpoint, android, gzip, sparse, partinfo->start, partinfo->blksz);
}

