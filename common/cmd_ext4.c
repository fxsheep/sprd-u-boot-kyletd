/*
 * (C) Copyright 2011 - 2012 Samsung Electronics
 * EXT4 filesystem implementation in Uboot by
 * Uma Shankar <uma.shankar@samsung.com>
 * Manjunatha C Achar <a.manjunatha@samsung.com>
 *
 * Ext4fs support
 * made from existing cmd_ext2.c file of Uboot
 *
 * (C) Copyright 2004
 * esd gmbh <www.esd-electronics.com>
 * Reinhard Arlt <reinhard.arlt@esd-electronics.com>
 *
 * made from cmd_reiserfs by
 *
 * (C) Copyright 2003 - 2004
 * Sysgo Real-Time Solutions, AG <www.elinos.com>
 * Pavel Bartusek <pba@sysgo.com>
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
 *
 */

/*
 * Changelog:
 *	0.1 - Newly created file for ext4fs support. Taken from cmd_ext2.c
 *	        file in uboot. Added ext4fs ls load and write support.
 */

#include <common.h>
#include <part.h>
#include <config.h>
#include <command.h>
#include <image.h>
#include <linux/ctype.h>
#include <asm/byteorder.h>
#include <ext_common.h>
#include <ext4fs.h>
#include <linux/stat.h>
#include <malloc.h>

#if defined(CONFIG_CMD_USB) && defined(CONFIG_USB_STORAGE)
#include <usb.h>
#endif

#if !defined(CONFIG_DOS_PARTITION) && !defined(CONFIG_EFI_PARTITION)
#error DOS or EFI partition support must be selected
#endif

static int do_ext4_load(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *interface = "mmc";
	int dev = 0;
	char *partname = "/fixnv";
	char *filename = "/fixnv/fixnv.bin";
	unsigned long ramaddress = 0x500000;
	unsigned long filesize ;

	printf("mount %s %d %s as ext4 filesystem\n", interface, dev, partname);
	/* mmc is interface; 0 is device in mmc slot */
	if (ext4fs_mount(interface, dev, partname) == -1) {
		printf("Bad ext4 partition %s %d:%lu\n", interface, dev, partname);
		goto fail;
	}

	filesize = ext4fs_open(filename);
	if (filesize < 0) {
		printf("File not found %s\n", filename);
		goto fail;
	}
	
	if (ext4fs_read((char *)ramaddress, filesize) != filesize) {
		printf("Unable to read %s from %s %d\n", filename, interface, dev);
		goto fail;
	}

	printf("%d bytes read\n", filesize);
	ext4fs_close();
	return 0;
fail:
	ext4fs_close();
	return 1;
}

static int do_ext4_ls(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *interface = "mmc";
	int dev = 0;
	char *partname = "/fixnv";
	char *filename = "/fixnv/fixnv.bin";

	printf("mount %s %d %s as ext4 filesystem\n", interface, dev, partname);
	/* mmc is interface; 0 is device in mmc slot */
	if (ext4fs_mount(interface, dev, partname) == -1) {
		printf("Bad ext4 partition %s %d:%lu\n", interface, dev, partname);
		ext4fs_close();
	}

	ext4fs_ls(filename);
	ext4fs_close();

	return 0;
}

#if defined(CONFIG_CMD_EXT4_WRITE)
static int do_ext4_write(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *interface = "mmc";
	int dev = 0;
	char *partname = "/fixnv";
	char *filename = "/fixnv/fixnv.bin";
	unsigned long ramaddress = 0x500000;
	unsigned long filesize = 65540;

	printf("mount %s %d %s as ext4 filesystem\n", interface, dev, partname);
	/* mmc is interface; 0 is device in mmc slot */
	if (ext4fs_mount(interface, dev, partname) == -1) {
		printf("Bad ext4 partition %s %d:%lu\n", interface, dev, partname);
		goto fail;
	}
	
	if (ext4fs_write(filename, (unsigned char *)ramaddress, filesize) == -1) {
		printf("Error ext4fs_write()\n");
		goto fail;
	}

	ext4fs_close();
	return 0;

fail:
	ext4fs_close();
	return 1;
}

static int do_ext4_format(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *interface = "mmc";
	int dev = 0;
	char *partname = "/fixnv";

	printf("Formating %s %d %s to ext4 filesystem\n", interface, dev, partname);
	/* mmc is interface; 0 is device in mmc slot */
	if (ext4fs_format(interface, dev, partname) == -1) {
		printf("Format failed!\n");
		goto fail;
	}

	printf("Format finished!\n");
	return 0;
fail:
	return 1;
}

U_BOOT_CMD(ext4write, 6, 1, do_ext4_write,
	"create a file in the root directory",
	"<interface> <dev[:part]> [Absolute filename path] [Address] [sizebytes]\n"
	"	  - create a file in / directory");
U_BOOT_CMD(ext4format, 6, 1, do_ext4_format,
	"format a partition to ext4 filesystem",
	"<interface> <dev[:part]>\n"
	"	  - format partition to ext4 filesystem");

#endif

U_BOOT_CMD(ext4ls, 4, 1, do_ext4_ls,
	   "list files in a directory (default /)",
	   "<interface> <dev[:part]> [directory]\n"
	   "	  - list files from 'dev' on 'interface' in a 'directory'");
U_BOOT_CMD(ext4load, 6, 0, do_ext4_load,
	   "load binary file from a Ext2 filesystem",
	   "<interface> <dev[:part]> [addr] [filename] [bytes]\n"
	   "	  - load binary file 'filename' from 'dev' on 'interface'\n"
	   "		 to address 'addr' from ext2 filesystem"
);
