/*
 * (C) Copyright 2006-2008
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
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

#include <config.h>
#include <common.h>
#include <nand.h>
#include <asm/io.h>
#include "../drivers/mmc/card_sdio.h"

#include <asm/arch/secure_boot.h>

#if defined(CONFIG_ARM)
void board_init_f (ulong bootflag)
{
	relocate_code (CONFIG_SYS_TEXT_BASE - TOTAL_MALLOC_LEN, NULL,
		       CONFIG_SYS_TEXT_BASE);
}
#endif


void nand_boot(void)
{
	int ret;
                   int i,j;
	__attribute__((noreturn)) void (*uboot)(void);
#if 0
	unsigned int i = 0;
	for(i=0xffffffff;i>0;)
	  i--;
#endif
	/*
	 * Init board specific nand support
	 */
#ifdef SPRD_EVM_TAG_ON
#if 0
	unsigned long int * ptr = (unsigned long int*)SPRD_EVM_ADDR_START-8;
	int ijk = 0;
	for(ijk =0;ijk<28;ijk++){
		*(ptr++)=0x55555555;
	}
#endif
	SPRD_EVM_TAG(1);
#endif

	if(TRUE == Emmc_Init()){                           
                      Emmc_Read(PARTITION_BOOT2, 0, CONFIG_SYS_EMMC_U_BOOT_SECTOR_NUM, (uint8 *)CONFIG_SYS_NAND_U_BOOT_DST);    
        }   

	/*
	 * Jump to U-Boot image
	 */
#ifdef SPRD_EVM_TAG_ON
	SPRD_EVM_TAG(3);
#endif
	uboot = (void *)CONFIG_SYS_NAND_U_BOOT_START;
	secure_check(CONFIG_SYS_NAND_U_BOOT_START, 0, CONFIG_SYS_NAND_U_BOOT_START + CONFIG_SYS_NAND_U_BOOT_SIZE - VLR_INFO_OFF, INTER_RAM_BEGIN + CONFIG_SPL_LOAD_LEN - KEY_INFO_SIZ - CUSTOM_DATA_SIZ);
	(*uboot)();
}
