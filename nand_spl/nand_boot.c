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
#include <asm/arch/secure_boot.h>

#define CONFIG_SYS_NAND_READ_DELAY \
	{ volatile int dummy; int i; for (i=0; i<10000; i++) dummy = i; }

#define CONFIG_SYS_NAND_BAD_BLOCK_POS	0
#define CONFIG_SYS_NAND_5_ADDR_CYCLE	1
#define panic(x...) do{}while(0)
#define printf(x...) do{}while(0)

static int pageinblock = 0;

#if (2048 <= 512)
/*
 * NAND command for small page NAND devices (512)
 */
static int nand_command(struct mtd_info *mtd, int block, int page, int offs, u8 cmd)
{
	struct nand_chip *this = mtd->priv;
	int page_addr = page + block * pageinblock;

	if (this->dev_ready)
		while (!this->dev_ready(mtd))
			;
	else
		CONFIG_SYS_NAND_READ_DELAY;

	/* Begin command latch cycle */
	this->cmd_ctrl(mtd, cmd, NAND_CTRL_CLE | NAND_CTRL_CHANGE);
	/* Set ALE and clear CLE to start address cycle */
	/* Column address */
	this->cmd_ctrl(mtd, offs, NAND_CTRL_ALE | NAND_CTRL_CHANGE);
	this->cmd_ctrl(mtd, page_addr & 0xff, NAND_CTRL_ALE); /* A[16:9] */
	this->cmd_ctrl(mtd, (page_addr >> 8) & 0xff,
		       NAND_CTRL_ALE); /* A[24:17] */
#ifdef CONFIG_SYS_NAND_4_ADDR_CYCLE
	/* One more address cycle for devices > 32MiB */
	this->cmd_ctrl(mtd, (page_addr >> 16) & 0x0f,
		       NAND_CTRL_ALE); /* A[28:25] */
#endif
	/* Latch in address */
	this->cmd_ctrl(mtd, NAND_CMD_NONE, NAND_NCE | NAND_CTRL_CHANGE);

	/*
	 * Wait a while for the data to be ready
	 */
	if (this->dev_ready)
		while (!this->dev_ready(mtd))
			;
	else
		CONFIG_SYS_NAND_READ_DELAY;

	return 0;
}
#else
/*
 * NAND command for large page NAND devices (2k)
 */
static int nand_command(struct mtd_info *mtd, int block, int page, int offs, u8 cmd)
{
	struct nand_chip *this = mtd->priv;
	int page_addr = page + block * pageinblock;

	if (this->dev_ready)
		while (!this->dev_ready(mtd))
			;
	else
		CONFIG_SYS_NAND_READ_DELAY;

	/* Emulate NAND_CMD_READOOB */
	if (cmd == NAND_CMD_READOOB) {
		offs += mtd->writesize;
		cmd = NAND_CMD_READ0;
	}

	/* Begin command latch cycle */
	this->cmd_ctrl(mtd, cmd, NAND_CTRL_CLE | NAND_CTRL_CHANGE);
	/* Set ALE and clear CLE to start address cycle */
	/* Column address */
#ifdef CONFIG_MTD_NAND_SPRD
	this->cmd_ctrl(mtd, offs, NAND_CTRL_ALE | NAND_CTRL_CHANGE);
	//this->cmd_ctrl(mtd, offs>>1, NAND_CTRL_ALE | NAND_CTRL_CHANGE);
#else
	
	/* Adjust columns for 16 bit buswidth */
	if ( this->options & NAND_BUSWIDTH_16)
		offs >>= 1;

	this->cmd_ctrl(mtd, offs & 0xff,
		       NAND_CTRL_ALE | NAND_CTRL_CHANGE); /* A[7:0] */

	this->cmd_ctrl(mtd, (offs >> 8) & 0xff, NAND_CTRL_ALE); /* A[11:9] */
#endif
	/* Row address */
#ifdef CONFIG_MTD_NAND_SPRD
	this->cmd_ctrl(mtd, page_addr, NAND_CTRL_ALE); /* A[19:12] */
#else
	this->cmd_ctrl(mtd, (page_addr & 0xff), NAND_CTRL_ALE); /* A[19:12] */
	this->cmd_ctrl(mtd, ((page_addr >> 8) & 0xff),
		       NAND_CTRL_ALE); /* A[27:20] */
#endif
#ifdef CONFIG_SYS_NAND_5_ADDR_CYCLE
	/* One more address cycle for devices > 128MiB */
	this->cmd_ctrl(mtd, (page_addr >> 16) & 0x0f,
		       NAND_CTRL_ALE); /* A[31:28] */
#endif
	/* Latch in address */
	this->cmd_ctrl(mtd, NAND_CMD_READSTART,
		       NAND_CTRL_CLE | NAND_CTRL_CHANGE);
	this->cmd_ctrl(mtd, NAND_CMD_NONE, NAND_NCE | NAND_CTRL_CHANGE);

	/*
	 * Wait a while for the data to be ready
	 */
	if (this->dev_ready)
		while (!this->dev_ready(mtd))
			;
	else
		CONFIG_SYS_NAND_READ_DELAY;

	return 0;
}
#endif

static int nand_is_bad_block(struct mtd_info *mtd, int block)
{
	struct nand_chip *this = mtd->priv;

	nand_command(mtd, block, 0, CONFIG_SYS_NAND_BAD_BLOCK_POS, NAND_CMD_READOOB);

	/*
	 * Read one byte
	 */
	if (readb(this->IO_ADDR_R) != 0xff)
		return 1;

	return 0;
}

static int nand_read_page(struct mtd_info *mtd, int block, int page, uchar *dst)
{
	struct nand_chip *this = mtd->priv;
	u_char *ecc_calc;
	u_char *ecc_code;
	u_char *oob_data;
	int i;
	int eccsize = this->ecc.size;
	int eccbytes = this->ecc.bytes;
	int eccsteps = mtd->writesize / eccsize;	
	int ecctotal = eccbytes * eccsteps;	

	uint8_t *p = dst;
	int stat;

	nand_command(mtd, block, page, 0, NAND_CMD_READ0);

	/* No malloc available for now, just use some temporary locations
	 * in SDRAM
	 */
	ecc_calc = (u_char *)(CONFIG_SYS_SDRAM_BASE + 0x10000);
	ecc_code = ecc_calc + 0x100;
	oob_data = ecc_calc + 0x200;

	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		this->ecc.hwctl(mtd, NAND_ECC_READ);
		this->read_buf(mtd, p, eccsize);
		this->ecc.calculate(mtd, p, &ecc_calc[i]);
	}

	this->read_buf(mtd, oob_data, mtd->oobsize);

	/* Pick the ECC bytes out of the oob data */
	for (i = 0; i < ecctotal; i++)
		ecc_code[i] = oob_data[(this->ecc.layout)->eccpos[i]];

	p = dst;
	eccsteps = mtd->writesize / eccsize;
	for (i = 0 ; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		/* No chance to do something with the possible error message
		 * from correct_data(). We just hope that all possible errors
		 * are corrected by this routine.
		 */
		stat = this->ecc.correct(mtd, p, &ecc_code[i], &ecc_calc[i]);
	}

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

	/*
	 * Load U-Boot image from NAND into RAM
	 */
	pageinblock = nand_info.erasesize / nand_info.writesize;
	//nand_command(&nand_info, -1,-1,-1,NAND_CMD_RESET);
#ifdef SPRD_EVM_TAG_ON
	SPRD_EVM_TAG(2);
#endif
	nand_chip.cmd_ctrl(&nand_info, NAND_CMD_RESET, NAND_NCE|NAND_CLE|NAND_CTRL_CHANGE);
	nand_chip.cmd_ctrl(&nand_info, NAND_CMD_NONE, NAND_NCE|NAND_CTRL_CHANGE);
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
	secure_check(CONFIG_SYS_NAND_U_BOOT_START, 0, CONFIG_SYS_NAND_U_BOOT_START + CONFIG_SYS_NAND_U_BOOT_SIZE - VLR_INFO_OFF, INTER_RAM_BEGIN + CONFIG_SPL_LOAD_LEN - KEY_INFO_SIZ - CUSTOM_DATA_SIZ);
	(*uboot)();
}

