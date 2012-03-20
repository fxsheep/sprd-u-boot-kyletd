/*
 * (C) Copyright 2008
 * Texas Instruments, <www.ti.com>
 * Sukumar Ghorai <s-ghorai@ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation's version 2 of
 * the License.
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
#include <mmc.h>
#include <asm/errno.h>
#include <part.h>
#include <asm/io.h>
#include <asm/arch/sc8810_reg_base.h>
#include <asm/arch/mfp.h>
#include <asm/arch/sc8810_reg_ahb.h>
//#include <asm/arch/mmc_host_def.h>
#include <asm/arch/ldo.h>
#include <asm/arch/sdio_reg_v3.h>
#include <asm/arch/chip_drv_common_io.h>
#include <asm/arch/sc8810_reg_global.h>
#include <asm/arch/sc8810_module_config.h>


#define MMCSD_SECTOR_SIZE 512


#define REG_AHB_CTL0		       		(*((volatile unsigned int *)(AHB_CTL0)))
#define REG_AHB_SOFT_RST			(*((volatile unsigned int *)(AHB_SOFT_RST)))
#define REG_AHB_SDIO_CTL			(*((volatile unsigned int *)(AHB_SDIO_CTL)))


SDIO_REG_CFG *mmc_base;
static struct mmc_cmd command;
static block_dev_desc_t sprd_mmc_dev;
static unsigned char mmc_rsp_buf[16];

block_dev_desc_t *mmc_get_dev(int dev)
{
	return ((block_dev_desc_t *) & sprd_mmc_dev);
}

static unsigned long mmc_bwrite(int dev_num, unsigned long blknr,
		lbaint_t blkcnt, void *dst)
{
	if(Emmc_Write(0, blknr, blkcnt, (uint8*)dst))
	{
		return blkcnt;
	}       
	else
	{
		return 0;
	}
}


static unsigned long mmc_bread(int dev_num, unsigned long blknr,
		lbaint_t blkcnt, void *dst)
{
	if(Emmc_Read(0, blknr, blkcnt, (uint8*)dst))
	{
		return blkcnt;
	}       
	else
	{
		return 0;
	}
}


int mmc_legacy_init(int dev)
{
	int retries, rc = -ENODEV;
	uint32_t cid_resp[4];
	uint32_t *resp;
	uint16_t rca = 0;

	if(TRUE == Emmc_Init())
	{       
		sprd_mmc_dev.if_type = IF_TYPE_UNKNOWN;
		sprd_mmc_dev.if_type = IF_TYPE_MMC;
		sprd_mmc_dev.part_type = PART_TYPE_EFI;
		sprd_mmc_dev.dev = 0;
		sprd_mmc_dev.lun = 0;
		sprd_mmc_dev.type = 0;

		/* FIXME fill in the correct size (is set to 32MByte) */
		sprd_mmc_dev.blksz = MMCSD_SECTOR_SIZE;
		
		sprd_mmc_dev.lba = Emmc_GetCapacity(0);;
		sprd_mmc_dev.removable = 0;
		sprd_mmc_dev.block_read = mmc_bread;
		sprd_mmc_dev.block_write = mmc_bwrite;			  
	}
	return rc;
}

