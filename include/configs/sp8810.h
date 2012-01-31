/*
 * (C) Copyright 2009 DENX Software Engineering
 * Author: John Rigby <jrigby@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H
//only used in fdl2 .in uart download, the debug infors  from  serial will break the download process.
#define CONFIG_FDL2_PRINT  0	
#define BOOT_NATIVE_LINUX (0)

#define CONFIG_SILENT_CONSOLE
#define CONFIG_GPIOLIB 1
//#define NAND_DEBUG  
//#define DEBUG
#define U_BOOT_SPRD_VER 1
/*#define SPRD_EVM_TAG_ON 1*/
#ifdef SPRD_EVM_TAG_ON
#define SPRD_EVM_ADDR_START 0x40006000
#define SPRD_EVM_TAG(_x) (*(((unsigned long *)SPRD_EVM_ADDR_START)+_x) = *(volatile unsigned long *)0x87003004)
#endif
#define CONFIG_L2_OFF			1

#define BOOT_DEBUG 1

#define CONFIG_YAFFS2 1

#define BOOT_PART "boot"
//#define BOOT_PART "kernel"
#define RECOVERY_PART "recovery"
/*
 * SPREADTRUM BIGPHONE board - SoC Configuration
 */

#define CONFIG_SC8810
#define CONFIG_SP8810

#ifdef CONFIG_SC8810
#define CHIP_ENDIAN_LITTLE
#define SC8800S_LITTLE_ENDIAN FALSE
#define _LITTLE_ENDIAN 1
#define EXT_MEM_TYPE_DDR 1
#endif

#define BB_DRAM_TYPE_256MB_32BIT
#define  CONFIG_MTD_NAND_SC8810 1

#define CONFIG_SYS_HZ			1000
#define CONFIG_SPRD_TIMER_CLK		1000 /*32768*/

//#define CONFIG_SYS_HUSH_PARSER

#ifdef CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2 "> "
#endif

/*#define CMDLINE_NEED_CONV */

#define WATCHDOG_LOAD_VALUE	0x4000
#define CONFIG_SYS_STACK_SIZE	0x400

//#define	CONFIG_SYS_MONITOR_LEN		(256 << 10)	/* 256 kB for U-Boot */

/* NAND BOOT is the only boot method */
#define CONFIG_NAND_U_BOOT
#define DYNAMIC_CRC_TABLE
/* Start copying real U-boot from the second page */
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x40000
#define CONFIG_SYS_NAND_U_BOOT_SIZE	0x60000

#define RAM_TYPPE_IS_SDRAM	0

#ifdef CONFIG_NAND_SPL
/* Load U-Boot to this address */
#define CONFIG_SYS_NAND_U_BOOT_DST	0x00f00000
#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_NAND_U_BOOT_DST

#define CONFIG_SYS_SDRAM_BASE 0x00000000
#define CONFIG_SYS_INIT_SP_ADDR     \
	(CONFIG_SYS_SDRAM_BASE + 0x4000)

#define CONFIG_SYS_NAND_SPARE_SIZE	64
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128 * 1024)
#define CONFIG_SYS_NAND_PAGE_COUNT	64
#define CONFIG_SYS_NAND_SIZE		(128 * 1024 * 1024)
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	0

#define CONFIG_SYS_NAND_5_ADDR_CYCLE

#else

#define CONFIG_MMU_TABLE_ADDR (0x40000000)

#define CONFIG_SYS_SDRAM_BASE 0x00000000
#define CONFIG_SYS_INIT_SP_ADDR     \
	(CONFIG_SYS_SDRAM_BASE + 0x1000 - GENERATED_GBL_DATA_SIZE)

#define CONFIG_SKIP_LOWLEVEL_INIT
#endif

#define CONFIG_SYS_NAND_PAGE_SIZE	2048
/* Size of the block protected by one OOB (Spare Area in Samsung terminology) */
#define CONFIG_SYS_NAND_ECCSIZE	512
/* Number of ECC bytes per OOB - S3C6400 calculates 4 bytes ECC in 1-bit mode */
#define CONFIG_SYS_NAND_ECCBYTES	4
/* Number of ECC-blocks per NAND page */
#define CONFIG_SYS_NAND_ECCSTEPS	(CONFIG_SYS_NAND_PAGE_SIZE / CONFIG_SYS_NAND_ECCSIZE)
/* 2 bit correct, sc8810 support 1, 2, 4, 8, 12,14, 24 */
#define CONFIG_SYS_NAND_ECC_MODE	2
#define CONFIG_SYS_SPL_ECC_POS		8
/* Size of a single OOB region */
#define CONFIG_SYS_NAND_OOBSIZE	64
/* Number of ECC bytes per page */
#define CONFIG_SYS_NAND_ECCTOTAL	(CONFIG_SYS_NAND_ECCBYTES * CONFIG_SYS_NAND_ECCSTEPS)
/* ECC byte positions */
#define CONFIG_SYS_NAND_ECCPOS		{40, 41, 42, 43, 44, 45, 46, 47, \
				 48, 49, 50, 51, 52, 53, 54, 55, \
				 56, 57, 58, 59, 60, 61, 62, 63}

#define CONFIG_HW_WATCHDOG

#define CONFIG_DISPLAY_CPUINFO

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1

/*
 * Memory Info
 */
/* malloc() len */
#define CONFIG_SYS_MALLOC_LEN		(2 << 20)	/* 1 MiB */
/*
 * Board has 2 32MB banks of DRAM but there is a bug when using
 * both so only the first is configured
 */
#define CONFIG_NR_DRAM_BANKS	1

#define PHYS_SDRAM_1		0x00000000
#define PHYS_SDRAM_1_SIZE	0x10000000
#if (CONFIG_NR_DRAM_BANKS == 2)
#define PHYS_SDRAM_2		0x90000000
#define PHYS_SDRAM_2_SIZE	0x02000000
#endif
/* 8MB DRAM test */
#define CONFIG_SYS_MEMTEST_START	PHYS_SDRAM_1
#define CONFIG_SYS_MEMTEST_END		(PHYS_SDRAM_1+0x0800000)
#define CONFIG_STACKSIZE	(256 * 1024)	/* regular stack */

/*
 * Serial Info
 */
#define CONFIG_SPRD_UART		1
#define CONFIG_SYS_SC8800X_UART1	1
#define CONFIG_CONS_INDEX	1	/* use UART0 for console */
#define CONFIG_BAUDRATE		115200	/* Default baud rate */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/*
 * Flash & Environment
 */
/* No NOR flash present */
#define CONFIG_SYS_MONITOR_LEN ((CONFIG_SYS_NAND_U_BOOT_OFFS)+(CONFIG_SYS_NAND_U_BOOT_SIZE))
#define CONFIG_SYS_NO_FLASH	1
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE		(128 * 1024)	
/*
#define	CONFIG_ENV_IS_IN_NAND
#define	CONFIG_ENV_OFFSET	CONFIG_SYS_MONITOR_LEN
#define CONFIG_ENV_OFFSET_REDUND	(CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)
*/

/* NAND */
#define CONFIG_NAND_SC8810
#define CONFIG_SPRD_NAND_REGS_BASE	(0x60000000)
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		(0x60000000)
//#define CONFIG_JFFS2_NAND
//#define CONFIG_SPRD_NAND_HWECC
#define CONFIG_SYS_NAND_HW_ECC
#define CONFIG_SYS_NAND_LARGEPAGE
//#define CONFIG_SYS_NAND_5_ADDR_CYCLE

#define CONFIG_SYS_64BIT_VSPRINTF

#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE
#define CONFIG_CMD_UBI
#define CONFIG_RBTREE

/* U-Boot general configuration */
#define CONFIG_SYS_PROMPT	"=> "	/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE	1024	/* Console I/O Buffer Size  */
/* Print buffer sz */
#define CONFIG_SYS_PBSIZE	(CONFIG_SYS_CBSIZE + \
		sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS	32	/* max number of command args */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_LONGHELP

/* support OS choose */
#undef CONFIG_BOOTM_NETBSD 
#undef CONFIG_BOOTM_RTEMS

/* U-Boot commands */
#include <config_cmd_default.h>
#define CONFIG_CMD_NAND
#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_LOADS
#undef CONFIG_CMD_NET
#undef CONFIG_CMD_NFS
#undef CONFIG_CMD_SETGETDCR

#define CONFIG_ENV_OVERWRITE

#ifdef SPRD_EVM_TAG_ON
#define CONFIG_BOOTDELAY	0
#else
#define CONFIG_BOOTDELAY	0
#define CONFIG_ZERO_BOOTDELAY_CHECK
#endif

#define CONFIG_LOADADDR		0x01000000	/* loadaddr env var */
#define CONFIG_SYS_LOAD_ADDR	CONFIG_LOADADDR

#define xstr(s)	str(s)
#define str(s)	#s

#define MTDIDS_DEFAULT "nand0=sprd-nand"
#ifdef CONFIG_G2PHONE
#define MTDPARTS_DEFAULT "mtdparts=sprd-nand:384k@256k(boot),256k(params),6m(kernel),6m(ramdisk),6m(recovery),70m(system),30m(userdata),7m(cache)"
#define CONFIG_BOOTARGS "mem=64M console=ttyS1,115200n8 init=/init "MTDPARTS_DEFAULT
#elif defined CONFIG_SP8810
/*#define MTDPARTS_DEFAULT "mtdparts=sprd-nand:256k(spl),384k(2ndbl),128k(params),512k(vmjaluna),6016k(modem),7680k(kernel),5120k(dsp),1280k(fixnv),2560k(runtimenv),6400k(recovery),100m(system),198m(userdata),1m(boot_logo),1m(fastboot_logo),2m(cache),256k(misc)"*/
//#define MTDPARTS_DEFAULT "mtdparts=sprd-nand:256k(spl),512k(2ndbl),128k(params),512k(vmjaluna),10m(modem),10m(boot),5120k(dsp),1280k(fixnv),3840k(backupfixnv),3840k(runtimenv),10m(recovery),150m(system),300m(userdata),1m(boot_logo),1m(fastboot_logo),2m(cache),256k(misc)"
//#define MTDPARTS_DEFAULT "mtdparts=sprd-nand:256k(spl),512k(2ndbl),128k(params),512k(vmjaluna),10m(modem),10m(boot)"
#define MTDPARTS_DEFAULT "mtdparts=sprd-nand:256k(spl),512k(2ndbl),128k(params),512k(vmjaluna),10m(modem),3840k(fixnv),3840k(backupfixnv),5120k(dsp),3840k(runtimenv),10m(boot),10m(recovery),200m(system),230m(userdata),20m(cache),256k(misc),1m(boot_logo),1m(fastboot_logo),2m(productinfo),512k(kpanic)"
#define CONFIG_BOOTARGS "mem=240M console=ttyS1,115200n8 init=/init " MTDPARTS_DEFAULT
#endif

#define COPY_LINUX_KERNEL_SIZE	(0x600000)
#define LINUX_INITRD_NAME	"modem"

#define CONFIG_BOOTCOMMAND "cboot normal"
#define	CONFIG_EXTRA_ENV_SETTINGS				""	

#ifdef CONFIG_CMD_NET
#define CONFIG_IPADDR 192.168.10.2
#define CONFIG_SERVERIP 192.168.10.5
#define CONFIG_NETMASK 255.255.255.0
#define CONFIG_USBNET_DEVADDR 26:03:ee:00:87:9f
#define CONFIG_USBNET_HOSTADDR 9a:04:c7:d6:30:d0


#define CONFIG_NET_MULTI
#define CONFIG_CMD_DNS
#define CONFIG_CMD_NFS
#define CONFIG_CMD_RARP
#define CONFIG_CMD_PING
/*#define CONFIG_CMD_SNTP */
#endif

#define CONFIG_USB_GADGET_SC8800G
#define CONFIG_USB_DWC
#define CONFIG_USB_GADGET_DUALSPEED
//#define CONFIG_USB_ETHER
#define CONFIG_CMD_FASTBOOT
#define SCRATCH_ADDR    0x1000000
#define FB_DOWNLOAD_BUF_SIZE (200*1024*1024)

#define CONFIG_MODEM_CALIBERATE
/*
#define CONFIG_UPDATE_TFTP
#define CONFIG_FIT
#define CONFIG_OF_LIBFDT
#define CONFIG_SYS_MAX_FLASH_BANKS 1
#define CONFIG_SYS_MAX_FLASH_SECT 128
*/
#define CONFIG_LCD
#ifdef CONFIG_LCD
#define CONFIG_SPLASH_SCREEN
#define LCD_BPP LCD_COLOR16
//#define CONFIG_LCD_HVGA   1
#define CONFIG_LCD_WVGA   1
//#define CONFIG_LCD_INFO
//#define LCD_TEST_PATTERN
//#define CONFIG_LCD_LOGO
#define CONFIG_SYS_WHITE_ON_BLACK
#ifdef LCD_TEST_PATTERN
#define CONSOLE_COLOR_RED 0xf800 
#define CONSOLE_COLOR_GREEN 0x07e0
#define CONSOLE_COLOR_YELLOW 0x07e0
#define CONSOLE_COLOR_BLUE 0x001f
#define CONSOLE_COLOR_MAGENTA 0x001f
#define CONSOLE_COLOR_CYAN 0x001f
#endif
#endif // CONFIG_LCD

#define CALIBRATE_ENUM_MS 15000
#define CALIBRATE_IO_MS 10000

#define LOW_BAT_ADC_LEVEL 782 /*phone battery adc value low than this value will not boot up*/
#define PWR_KEY_DETECT_CNT 12 /*this should match the count of boot_pwr_check() function */
#define ALARM_LEAD_SET_MS 40 /* time set for alarm boot in advancd */

#endif /* __CONFIG_H */
