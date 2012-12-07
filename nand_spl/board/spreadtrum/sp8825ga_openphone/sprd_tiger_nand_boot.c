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

#define CONFIG_SYS_NAND_READ_DELAY \
	{ volatile int dummy; int i; for (i=0; i<10000; i++) dummy = i; }

#define CONFIG_SYS_NAND_BAD_BLOCK_POS	0
#define CONFIG_SYS_NAND_5_ADDR_CYCLE	1

static int pageinblock = 0;
static uchar s_oob_data[NAND_MAX_OOBSIZE];
static int nand_is_bad_block(struct mtd_info *mtd, int block)
{
	struct nand_chip *this = mtd->priv;
	int page_addr = CONFIG_SYS_NAND_BAD_BLOCK_POS + block * pageinblock;

	this->ecc.read_oob(mtd, this, page_addr, 1);
	/*
	 * compare one bytes
	 */
	if (this->oob_poi[0] != 0xff)
		return 1;

	return 0;
}

static int nand_read_page(struct mtd_info *mtd, int block, int page, uchar *dst)
{
	struct nand_chip *this = mtd->priv;
	int page_addr = page + block * pageinblock;
	int stat;

	this->cmdfunc(mtd, NAND_CMD_READ0, 0x00, page_addr);
	stat = this->ecc.read_page(mtd, this, dst,page_addr);
	return 0;
}

static int nand_load(struct mtd_info *mtd, unsigned int offs,
		     unsigned int uboot_size, uchar *dst)
{
	unsigned int block, lastblock;
	unsigned int page;

/*
	 * offs has to be aligned to a page address!
*/
	block = offs / mtd->erasesize;
	lastblock = (offs + uboot_size - 1) / mtd->erasesize;
	page = (offs % mtd->erasesize) / mtd->writesize;

	while (block <= lastblock) {
		if (!nand_is_bad_block(mtd, block)) {
			/*
			 * Skip bad blocks
			 */
			while (page < pageinblock) {
				nand_read_page(mtd, block, page, dst);
				dst += mtd->writesize;
				page++;
			}

			page = 0;
		} else {
			lastblock++;
		}

		block++;
	}

	return 0;
}

#if defined(CONFIG_ARM)
void board_init_f (ulong bootflag)
{
	relocate_code (CONFIG_SYS_TEXT_BASE - TOTAL_MALLOC_LEN, NULL,
		       CONFIG_SYS_TEXT_BASE);
}
#endif

/*
 * The main entry for NAND booting. It's necessary that SDRAM is already
 * configured and available since this code loads the main U-Boot image
 * from NAND into SDRAM and starts it from there.
 */
void nand_boot(void)
{
	struct nand_chip nand_chip;
	nand_info_t nand_info;
	int ret;
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
	nand_info.priv = &nand_chip;
	nand_chip.dev_ready = NULL;	/* preset to NULL */
	board_nand_init(&nand_chip);
	if (nand_chip.select_chip)
		nand_chip.select_chip(&nand_info, 0);
	nand_chip.oob_poi=s_oob_data; //init the oob data buffer
	/*
	 * Load U-Boot image from NAND into RAM
	 */
	pageinblock = nand_info.erasesize / nand_info.writesize;
	//nand_command(&nand_info, -1,-1,-1,NAND_CMD_RESET);
#ifdef SPRD_EVM_TAG_ON
	SPRD_EVM_TAG(2);
#endif
	nand_chip.cmdfunc(&nand_info, NAND_CMD_RESET, -1, -1);
	ret = nand_load(&nand_info, CONFIG_SYS_NAND_U_BOOT_OFFS, CONFIG_SYS_NAND_U_BOOT_SIZE,
			(uchar *)CONFIG_SYS_NAND_U_BOOT_DST);

#ifdef CONFIG_NAND_ENV_DST
	nand_load(&nand_info, CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE,
		  (uchar *)CONFIG_NAND_ENV_DST);

#ifdef CONFIG_ENV_OFFSET_REDUND
	nand_load(&nand_info, CONFIG_ENV_OFFSET_REDUND, CONFIG_ENV_SIZE,
		  (uchar *)CONFIG_NAND_ENV_DST + CONFIG_ENV_SIZE);
#endif
#endif

	if (nand_chip.select_chip)
		nand_chip.select_chip(&nand_info, -1);

	/*
	 * Jump to U-Boot image
	 */
#ifdef SPRD_EVM_TAG_ON
	SPRD_EVM_TAG(3);
#endif
	uboot = (void *)CONFIG_SYS_NAND_U_BOOT_START;
	(*uboot)();
}
