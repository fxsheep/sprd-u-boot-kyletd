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


/*
 * SPREADTRUM BIGPHONE board - SoC Configuration
 */
#define CONFIG_ARM926EJS			/* arm926ejs CPU core */
#define CONFIG_SC8800X
#define CONFIG_BIGPHONE
/*
#define CONFIG_MX25
#define CONFIG_TX25
#define CONFIG_MX25_CLK32		32000	*/  /* OSC32K frequency */
#define CONFIG_SYS_HZ			1000
#define CONFIG_SPRD_TIMER_CLK		32768

#define WATCHDOG_LOAD_VALUE	0x4000
#define CONFIG_SYS_STACK_SIZE	0x400

/*system clock config, it should be in range hardware support */
#define CONFIG_SYS_PLL_MHZ	328	
/* F(AHB)= F(PLL)/AHB_DIV */
#define CONFIG_SYS_AHB_DIV      4
/* F(ARM) = F(PLL)/ARM_DIV */
#define CONFIG_SYS_ARM_DIV	2
/* F(EMC) = F(PLL)/EMC_DIV */
#define CONFIG_SYS_EMC_DIV	2


#ifdef CONFIG_NAND_SPL
#define CONFIG_PRELOADER
#endif

#ifdef CONFIG_NAND_SPL
#define CONFIG_SYS_SDRAM_BANK_CNT   2
#define CONFIG_SYS_SDRAM_ROW_CNT    2
#define CONFIG_SYS_SDRAM_COL_CNT  1
#define CONFIG_SYS_SDRAM_DATA_WIDTH    32
#define CONFIG_SYS_SDRAM_BURST_LENGTH   3
#define CONFIG_SYS_SDRAM_CAS_LATENCY    3
#define CONFIG_SYS_SDRAM_EXT_MODE   0xffffffff
#define CONFIG_SYS_SDRAM_SIZE_M    64
#define CONFIG_SYS_SDRAM_CLK_DELAY   0x2120
 
#define CONFIG_SYS_SDRAM_ROW_REFRESH_MAX 7800
#define CONFIG_SYS_SDRAM_ROW_PRECHARGE_MIN  30
#define CONFIG_SYS_SDRAM_ROW_CYCLE_MIN  69
#define CONFIG_SYS_SDRAM_TRCD_MIN  30
#define CONFIG_SYS_SDRAM_TWR_MIN  30
#define CONFIG_SYS_SDRAM_TMRD_MIN  2
#define CONFIG_SYS_SDRAM_TRFC_MIN  110
#define CONFIG_SYS_SDRAM_TXSR_MIN  150
#define CONFIG_SYS_SDRAM_TRAS_MIN  90
#endif


#define	CONFIG_SYS_MONITOR_LEN		(256 << 10)	/* 256 kB for U-Boot */

/* NAND BOOT is the only boot method */
#define CONFIG_NAND_U_BOOT

#ifdef CONFIG_NAND_SPL
/* Start copying real U-boot from the second page */
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x800
#define CONFIG_SYS_NAND_U_BOOT_SIZE	0x30000
/* Load U-Boot to this address */
#define CONFIG_SYS_NAND_U_BOOT_DST	0x00f00000
#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_NAND_U_BOOT_DST

#define CONFIG_SYS_SDRAM_BASE 0x00

#define CONFIG_SYS_NAND_SPARE_SIZE	64
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128 * 1024)
#define CONFIG_SYS_NAND_PAGE_COUNT	64
#define CONFIG_SYS_NAND_SIZE		(128 * 1024 * 1024)
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	0
#else
#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_SKIP_RELOCATE_UBOOT
#endif

#define CONFIG_SYS_NAND_PAGE_SIZE	2048
/* Size of the block protected by one OOB (Spare Area in Samsung terminology) */
#define CONFIG_SYS_NAND_ECCSIZE	CONFIG_SYS_NAND_PAGE_SIZE
/* Number of ECC bytes per OOB - S3C6400 calculates 4 bytes ECC in 1-bit mode */
#define CONFIG_SYS_NAND_ECCBYTES	4
/* Number of ECC-blocks per NAND page */
#define CONFIG_SYS_NAND_ECCSTEPS	(CONFIG_SYS_NAND_PAGE_SIZE / CONFIG_SYS_NAND_ECCSIZE)
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
#define CONFIG_SYS_MALLOC_LEN		(1 << 20)	/* 1 MiB */
/* reserved for initial data */
#define CONFIG_SYS_GBL_DATA_SIZE	128
/*
 * Board has 2 32MB banks of DRAM but there is a bug when using
 * both so only the first is configured
 */
#define CONFIG_NR_DRAM_BANKS	1

#define PHYS_SDRAM_1		0x00000000
#define PHYS_SDRAM_1_SIZE	0x02000000
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
#define CONFIG_SYS_NO_FLASH	1
#define	CONFIG_ENV_IS_IN_NAND
#define	CONFIG_ENV_OFFSET	CONFIG_SYS_MONITOR_LEN
#define CONFIG_ENV_SIZE		(128 * 1024)	/* 128 kB NAND block size */
#define CONFIG_ENV_OFFSET_REDUND	(CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)

/* NAND */
#define CONFIG_NAND_SPRD
#define CONFIG_SPRD_NAND_REGS_BASE	(0x60000000)
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		(0x60000000)
//#define CONFIG_JFFS2_NAND
#define CONFIG_SPRD_NAND_HWECC
#define CONFIG_SYS_NAND_LARGEPAGE

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

/*
 * Ethernet
 */
//#define CONFIG_FEC_MXC
//#define CONFIG_FEC_MXC_PHYADDR		0x1f
//#define CONFIG_MII
//#define BOARD_LATE_INIT
#define CONFIG_ENV_OVERWRITE

#define CONFIG_BOOTDELAY	5

#define CONFIG_LOADADDR		0x01000000	/* loadaddr env var */
#define CONFIG_SYS_LOAD_ADDR	CONFIG_LOADADDR

#define xstr(s)	str(s)
#define str(s)	#s

#define	CONFIG_EXTRA_ENV_SETTINGS					\
	"netdev=eth0\0"							\
	"nfsargs=setenv bootargs root=/dev/nfs rw "			\
		"nfsroot=${serverip}:${rootpath}\0"			\
	"ramargs=setenv bootargs root=/dev/ram rw\0"			\
	"addip=setenv bootargs ${bootargs} "				\
		"ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}"	\
		":${hostname}:${netdev}:off panic=1\0"			\
	"addtty=setenv bootargs ${bootargs}"				\
		" console=ttymxc0,${baudrate}\0"			\
	"addmtd=setenv bootargs ${bootargs} ${mtdparts}\0"		\
	"addmisc=setenv bootargs ${bootargs}\0"				\
	"u-boot=tx25/u-boot.bin\0"					\
	"kernel_addr_r=" xstr(CONFIG_LOADADDR) "\0"			\
	"hostname=tx25\0"						\
	"bootfile=tx25/uImage\0"					\
	"rootpath=/opt/eldk/arm\0"					\
	"net_nfs=tftp ${kernel_addr_r} ${bootfile};"			\
		"run nfsargs addip addtty addmtd addmisc;"		\
		"bootm\0"						\
	"bootcmd=run net_nfs\0"						\
	"load=tftp ${loadaddr} ${u-boot}\0"				\
	"update=nand erase 0 40000;nand write ${loadaddr} 0 40000\0"	\
	"upd=run load update\0"						\

#endif /* __CONFIG_H */
