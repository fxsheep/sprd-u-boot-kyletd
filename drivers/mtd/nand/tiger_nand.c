/* linux/drivers/mtd/nand/tiger_nand.c
 *
 * Copyright (c) 2012 Spreadtrun.
 *
 * Spreadtrun NAND driver

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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <common.h>
#include <config.h>
#include <asm/arch/bits.h>
//#include <asm/arch/chip_drv_config_extern.h>
#include <asm/arch/sprd_nfc_reg_v2.h>
#include <asm/arch/regs_cpc.h>
#include <nand.h>
#include <asm/io.h>
#include <linux/mtd/nand.h>

//#define NAND_DBG
#ifdef CONFIG_NAND_SPL
#define printf(arg...) do{}while(0)
#endif
#ifndef NAND_DBG
#define printf(arg...) do{}while(0)
#endif

/* 2 bit correct, sc8810 support 1, 2, 4, 8, 12,14, 24 */
#define CONFIG_SYS_NAND_ECC_MODE	2
/* Number of ECC bytes per OOB - S3C6400 calculates 4 bytes ECC in 1-bit mode */
#define CONFIG_SYS_NAND_ECCBYTES	4
/* Size of the block protected by one OOB (Spare Area in Samsung terminology) */
#define CONFIG_SYS_NAND_ECCSIZE	512

#define CONFIG_SYS_NAND_5_ADDR_CYCLE	5


#define NFC_MC_ICMD_ID	(0xCD)
#define NFC_MC_ADDR_ID	(0x0A)
#define NFC_MC_WRB0_ID	(0xB0)
#define NFC_MC_WRB1_ID	(0xB1)
#define NFC_MC_MRDT_ID	(0xD0)
#define NFC_MC_MWDT_ID	(0xD1)
#define NFC_MC_SRDT_ID	(0xD2)
#define NFC_MC_SWDT_ID	(0xD3)
#define NFC_MC_IDST_ID	(0xDD)
#define NFC_MC_CSEN_ID	(0xCE)
#define NFC_MC_NOP_ID	(0xF0)
#define NFC_MC_DONE_ID	(0xFF)
#define NFC_MAX_CHIP	1
#define NFC_TIMEOUT_VAL		0x1000000

#define NAND_MC_CMD(x)  (uint16_t)(((x & 0xff) << 8) | NFC_MC_ICMD_ID)
#define NAND_MC_ADDR(x) (uint16_t)(((x & 0xff) << 8) | (NFC_MC_ADDR_ID << 4))
#define NAND_MC_WRB0(x) (uint16_t)(NFC_MC_WRB0_ID)
#define NAND_MC_MRDT	(uint16_t)(NFC_MC_MRDT_ID)
#define NAND_MC_MWDT	(uint16_t)(NFC_MC_MWDT_ID)
#define NAND_MC_SRDT	(uint16_t)(NFC_MC_SRDT_ID)
#define NAND_MC_SWDT	(uint16_t)(NFC_MC_SWDT_ID)
#define NAND_MC_IDST(x)	(uint16_t)((NFC_MC_IDST_ID) | ((x -1) << 8))

#define NAND_MC_BUFFER_SIZE (24)


#define mtd_to_tiger(m) (&g_tiger)
struct sprd_tiger_nand_param {
	uint8_t id[5];
	uint8_t bus_width;
	uint8_t a_cycles;
	uint8_t sct_pg; //sector per page
	uint8_t oob_size; /* oob size per sector*/
	uint8_t ecc_pos; /* oob size per sector*/
	uint8_t info_pos; /* oob size per sector*/
	uint8_t info_size; /* oob size per sector*/
	uint8_t eccbit; /* ecc level per eccsize */
	uint16_t eccsize; /*bytes per sector for ecc calcuate once time */
};
struct sprd_tiger_nand_info {
	struct mtd_info *mtd;
	struct nand_chip *nand;
	struct device *pdev;
	struct sprd_tiger_nand_param *param;
	uint32_t chip; //chip index
	uint32_t page; //page address
	uint16_t column; //column address
	uint16_t oob_size;
	uint16_t m_size; //main part size per sector
	uint16_t s_size; //oob size per sector
	uint8_t a_cycles;//address cycles, 3, 4, 5
	uint8_t sct_pg; //sector per page
	uint8_t info_pos;
	uint8_t info_size;
	uint8_t ecc_mode;//0-1bit, 1-2bit, 2-4bit, 3-8bit,4-12bit,5-16bit,6-24bit
	uint8_t ecc_pos; // ecc postion
	uint8_t wp_en; //write protect enable
	uint16_t write_size;
	uint16_t page_per_bl;//page per block
	uint16_t  buf_head;
	uint16_t _buf_tail;
	uint8_t ins_num;//instruction number
	uint32_t ins[NAND_MC_BUFFER_SIZE >> 1];	
};
static void sprd_tiger_nand_read_id(struct sprd_tiger_nand_info *tiger, uint32 *buf);
static void sprd_tiger_nand_reset(struct sprd_tiger_nand_info *tiger);
static int sprd_tiger_nand_wait_finish(struct sprd_tiger_nand_info *tiger);
//gloable variable 
static struct nand_ecclayout sprd_tiger_nand_oob_default = {
	.eccbytes = 0,
	.eccpos = {0},
	.oobfree = {
		{.offset = 2,
		 .length = 46}}
};
struct sprd_tiger_nand_info g_tiger = {0};
//save the data read by read_byte and read_word api interface functon
static __attribute__((aligned(4))) uint8_t  s_oob_data[NAND_MAX_OOBSIZE]; 
static uint32_t sprd_tiger_reg_read(uint32_t addr)
{
	return readl(addr);
}
static void sprd_tiger_reg_write(uint32_t addr, uint32_t val)
{
	writel(val, addr);
}
static void sprd_tiger_reg_or(uint32_t addr, uint32_t val)
{
	sprd_tiger_reg_write(addr, sprd_tiger_reg_read(addr) | val);
}
static void sprd_tiger_reg_and(uint32_t addr, uint32_t mask)
{
	sprd_tiger_reg_write(addr, sprd_tiger_reg_read(addr) & mask);
}
static void sprd_tiger_nand_int_clr(uint32_t bit_clear)
{
	sprd_tiger_reg_write(NFC_INT_REG, bit_clear);
}
unsigned int ecc_mode_convert(uint32_t mode)
{
	uint32_t mode_m;
	switch(mode)
	{
	case 1:
		mode_m = 0;
		break;
	case 2:
		mode_m = 1;
		break;
	case 4:
		mode_m = 2;
		break;
	case 8:
		mode_m = 3;
		break;
	case 12:
		mode_m = 4;
		break;
	case 16:
		mode_m = 5;
		break;
	case 24:
		mode_m = 6;
		break;
	default:
		mode_m = 0;
		break;
	}
	return mode_m;
}
/*spare info must be align to ecc pos, info_pos + info_size = ecc_pos, 
 *the hardware must be config info_size and info_pos when ecc enable,and the ecc_info size can't be zero,
 *to simplify the nand_param_tb, the info is align with ecc and ecc at the last postion in one sector
*/
static struct sprd_tiger_nand_param sprd_tiger_nand_param_tb[] = {
	{{0xec, 0xbc, 0x00,0x55, 0x54}, 	1, 	5, 	4, 	16, 	12, 	11, 	1, 	2, 	512},
	{{0xec, 0xbc, 0x00,0x6A, 0x56}, 	1, 	5, 	8, 	16, 	9, 	8, 	1, 	4, 	512},
};
#ifdef CONFIG_NAND_SPL
struct sprd_tiger_boot_header_info {
	uint32_t check_sum;
	uint32_t sct_size; //
	uint32_t acycle; // 3, 4, 5
	uint32_t bus_width; //0 ,1
	uint32_t spare_size; //spare part sise for one sector
	uint32_t ecc_mode; //0--1bit, 1--2bit,2--4bit,3--8bit,4--12bit, 5--16bit, 6--24bit
	uint32_t ecc_pos; // ecc postion at spare part
	uint32_t sct_per_page; //sector per page
	uint32_t info_pos;
	uint32_t info_size;
	uint32_t magic_num; //0xaa55a5a5	
	uint32_t ecc_value[11];
};
void boad_nand_param_init(struct sprd_tiger_nand_info *tiger, struct nand_chip *chip, uint8 *id)
{
	int extid;
	uint32_t writesize;
	uint32_t oobsize;
	uint32_t erasesize;
	uint32_t busw;
	
	/* The 4th id byte is the important one */
	extid = id[3];
	writesize = 1024 << (extid & 0x3);
	extid >>= 2;
	/* Calc oobsize */
	oobsize = (8 << (extid & 0x01)) * (writesize >> 9);
	extid >>= 2;
	/* Calc blocksize. Blocksize is multiples of 64KiB */
	erasesize = (64 * 1024) << (extid & 0x03);
	extid >>= 2;
	/* Get buswidth information */
	busw = (extid & 0x01) ? NAND_BUSWIDTH_16 : 0;
	tiger->write_size = writesize;
	tiger->m_size =CONFIG_SYS_NAND_ECCSIZE;
	tiger->sct_pg = writesize / CONFIG_SYS_NAND_ECCSIZE;
	tiger->s_size = oobsize / tiger->sct_pg;
	tiger->ecc_mode = ecc_mode_convert(CONFIG_SYS_NAND_ECC_MODE);
	tiger->ecc_pos = tiger->s_size - ((14 * CONFIG_SYS_NAND_ECC_MODE + 7) / 8);
	tiger->info_pos = tiger->ecc_pos - 1;
	tiger->info_size = 1;
	tiger->page_per_bl = erasesize / tiger->write_size;
	tiger->a_cycles = CONFIG_SYS_NAND_5_ADDR_CYCLE;
	if(NAND_BUSWIDTH_16 == busw)
	{
		chip->options |= NAND_BUSWIDTH_16;
	}
	else
	{
		chip->options &= ~NAND_BUSWIDTH_16;
	}
}
/*
 * because the tiger firmware use the nand identify process
 * and the data at the header of nand_spl is the nand param used at nand read and write,
 * so in nand_spl, don't need read the id or use the onfi spec to calculate the nand param,
 * just use the param at the nand_spl header instead of
 */
void nand_hardware_config(struct mtd_info *mtd,struct nand_chip *chip)
{
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);
	struct sprd_tiger_boot_header_info *header;
	int index;
	int array;
	struct sprd_tiger_nand_param * param;
	uint8 *id;
	sprd_tiger_nand_reset(tiger);
	sprd_tiger_nand_read_id(tiger, (uint32 *)s_oob_data);
	boad_nand_param_init(tiger, tiger->nand, s_oob_data);
	id = s_oob_data;
#if 1
	array = ARRAY_SIZE(sprd_tiger_nand_param_tb);
	for (index = 0; index < array; index ++) {
		param = sprd_tiger_nand_param_tb + index;
		if ((param->id[0] == id[0]) 
			&& (param->id[1] == id[1]) 
			&& (param->id[2] == id[2]) 
			&& (param->id[3] == id[3]) 
			&& (param->id[4] == id[4]))
			break;
	}
	if (index < array) {
		tiger->ecc_mode = ecc_mode_convert(param->eccbit);
		tiger->m_size = param->eccsize;
		tiger->s_size = param->oob_size;
		tiger->a_cycles = param->a_cycles;
		tiger->sct_pg = param->sct_pg;
		tiger->info_pos = param->info_pos;
		tiger->info_size = param->info_size;
		tiger->write_size = tiger->m_size * tiger->sct_pg;
		if(param->bus_width)
		{
			chip->options |= NAND_BUSWIDTH_16;
		}
		else
		{
			chip->options &= ~NAND_BUSWIDTH_16;
		}
		mtd->writesize = tiger->write_size;
		mtd->oobsize = tiger->s_size * tiger->sct_pg;
		tiger->nand=chip;
	}
	mtd->writesize = tiger->write_size;
	mtd->oobsize = tiger->s_size * tiger->sct_pg;
	mtd->erasesize = tiger->page_per_bl * tiger->write_size;
#else	
	header = (struct sprd_tiger_boot_header_info *)0x400000000;
	tiger->ecc_mode = header->ecc_mode;
	tiger->m_size = header->sct_size;
	tiger->s_size = header->spare_size;
	tiger->a_cycles = header->acycle;
	tiger->sct_pg = header->sct_per_page;
	tiger->info_pos = header->info_pos;
	tiger->info_size = header->info_size;
	tiger->write_size = tiger->m_size * tiger->sct_pg;
	if(header->bus_width)
	{
		chip->options |= NAND_BUSWIDTH_16;
	}
	else
	{
		chip->options &= ~NAND_BUSWIDTH_16;
	}
	mtd->writesize = tiger->write_size;
	mtd->oobsize = tiger->s_size * tiger->sct_pg;
#endif	
}
#else
static void sprd_tiger_nand_ecc_layout_gen(struct sprd_tiger_nand_info *tiger, struct sprd_tiger_nand_param *param, struct nand_ecclayout *layout)
{
	uint32_t sct = 0;
	uint32_t i = 0;
	uint32_t offset;
	uint32_t used_len ; //one sector ecc data size(byte)
	uint32_t eccbytes = 0; //one page ecc data size(byte)
	uint32_t oobfree_len = 0;
	used_len = (14 * param->eccbit + 7) / 8 + param->info_size;
	if(param->sct_pg > ARRAY_SIZE(layout->oobfree))
	{
		while(1);
	}
	for(sct = 0; sct < param->sct_pg; sct++)
	{
		//offset = (oob_size * sct) + ecc_pos;
		//for(i = 0; i < ecc_len; i++)
		offset = (param->oob_size * sct) + param->info_pos;
		for(i = 0; i < used_len; i++)
		{
			layout->eccpos[eccbytes++] = offset + i;
		}
		layout->oobfree[sct].offset = param->oob_size * sct;
		layout->oobfree[sct].length = param->oob_size - used_len ;
		oobfree_len += param->oob_size - used_len;
	}
	//for bad mark postion
	layout->oobfree[0].offset = 2;
	layout->oobfree[0].length = param->oob_size - used_len - 2;
	oobfree_len -= 2;
	layout->eccbytes = used_len * param->sct_pg;
}
void nand_hardware_config(struct mtd_info *mtd, struct nand_chip *chip, u8 id[5])
{
	int index;
	int array;
	struct sprd_tiger_nand_param * param;
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);
	
	array = ARRAY_SIZE(sprd_tiger_nand_param_tb);
	for (index = 0; index < array; index ++) {
		param = sprd_tiger_nand_param_tb + index;
		if ((param->id[0] == id[0]) 
			&& (param->id[1] == id[1]) 
			&& (param->id[2] == id[2]) 
			&& (param->id[3] == id[3]) 
			&& (param->id[4] == id[4]))
			break;
	}
	if (index < array) {
		//save the param config
		tiger->ecc_mode = ecc_mode_convert(param->eccbit);
		tiger->m_size = param->eccsize;
		tiger->s_size = param->oob_size;
		tiger->a_cycles = param->a_cycles;
		tiger->sct_pg = param->sct_pg;
		tiger->info_pos = param->info_pos;
		tiger->info_size = param->info_size;
		tiger->write_size = tiger->m_size * tiger->sct_pg;
		tiger->ecc_pos = param->ecc_pos;
		//tiger->bus_width = param->bus_width;
		if(param->bus_width)
		{
			chip->options |= NAND_BUSWIDTH_16;
		}
		else
		{
			chip->options &= ~NAND_BUSWIDTH_16;
		}
		tiger->param = param;
		//update the mtd and nand default param after nand scan
		mtd->writesize = tiger->write_size;
		mtd->oobsize = tiger->s_size * tiger->sct_pg;
		/* Calculate the address shift from the page size */
		chip->page_shift = ffs(mtd->writesize) - 1;
		/* Convert chipsize to number of pages per chip -1. */
		chip->pagemask = (chip->chipsize >> chip->page_shift) - 1;
		sprd_tiger_nand_ecc_layout_gen(tiger, param, &sprd_tiger_nand_oob_default);
		chip->ecc.layout = &sprd_tiger_nand_oob_default;
		tiger->mtd = mtd;
	}
	else {
		int steps;
		struct sprd_tiger_nand_param  param1;
		
		//save the param config
		steps = mtd->writesize / CONFIG_SYS_NAND_ECCSIZE;
		tiger->ecc_mode = ecc_mode_convert(CONFIG_SYS_NAND_ECC_MODE);
		tiger->m_size = CONFIG_SYS_NAND_ECCSIZE;
		tiger->s_size = mtd->oobsize / steps;
		tiger->a_cycles = mtd->writesize / CONFIG_SYS_NAND_ECCSIZE;
		tiger->sct_pg = steps;
		tiger->info_pos = tiger->s_size - CONFIG_SYS_NAND_ECCBYTES - 1;
		tiger->info_size = 1;
		tiger->write_size = mtd->writesize;
		tiger->ecc_pos = tiger->s_size - CONFIG_SYS_NAND_ECCBYTES;
		param1.sct_pg = tiger->sct_pg;
		param1.info_pos = tiger->info_pos;
		param1.info_size = tiger->info_size;
		param1.oob_size = tiger->s_size;
		param1.eccbit = CONFIG_SYS_NAND_ECC_MODE;
		param1.eccsize = tiger->s_size;
		
		if(chip->chipsize > (128 << 20)) {
			tiger->a_cycles = 5;
		}
		else {
			tiger->a_cycles = 4;
		}
		sprd_tiger_nand_ecc_layout_gen(tiger, &param1, &sprd_tiger_nand_oob_default);
		chip->ecc.layout = &sprd_tiger_nand_oob_default;
		tiger->mtd = mtd;
	}
	tiger->mtd = mtd;
}
#endif
#ifndef CONFIG_NAND_SPL
typedef struct {
	uint8_t *m_buf;
	uint8_t *s_buf;
	uint8_t m_sct;
	uint8_t s_sct;
	uint8_t dir; //if dir is 0, read dadta from NFC buffer, if 1, write data to NFC buffer
	uint16_t m_size;
	uint16_t s_size;
} sprd_tiger_nand_data_param;
static unsigned int sprd_tiger_data_trans(sprd_tiger_nand_data_param *param)
{
	uint32_t cfg0 = 0;
	uint32_t cfg1 = 0;
	uint32_t cfg2 = 0;
	cfg0 = NFC_ONLY_MST_MODE | MAIN_SPAR_APT | NFC_WPN;
	if(param->dir)
	{
		cfg0 |= NFC_RW;
	}
	if(param->m_sct != 0)
	{
		cfg0 |= (param->m_sct - 1) << SECTOR_NUM_OFFSET;
		cfg0 |= MAIN_USE;
		cfg1 |= (param->m_size - 1);
		sprd_tiger_reg_write(NFC_MAIN_ADDR_REG, (uint32_t)param->m_buf);
	}
	if(param->s_sct != 0)
	{
		cfg0 |= SPAR_USE;
		cfg1 |= (param->s_size - 1) << SPAR_SIZE_OFFSET;
		cfg2 |= (param->s_sct - 1) << SPAR_SECTOR_NUM_OFFSET;
		sprd_tiger_reg_write(NFC_SPAR_ADDR_REG, (uint32_t)param->s_buf);
	}
	sprd_tiger_reg_write(NFC_CFG0_REG, cfg0);
	sprd_tiger_reg_write(NFC_CFG1_REG, cfg1);
	sprd_tiger_reg_write(NFC_CFG2_REG, cfg2);
	sprd_tiger_nand_int_clr(INT_STSMCH_CLR | INT_WP_CLR | INT_TO_CLR | INT_DONE_CLR);//clear all interrupt
	sprd_tiger_reg_write(NFC_START_REG, NFC_START);
	sprd_tiger_nand_wait_finish(&g_tiger);
}
void sprd_ecc_ctrl(struct sprd_ecc_param *param, uint32_t dir)
{
	uint32_t cfg0 = 0;
	uint32_t cfg1 = 0;
	uint32_t cfg2 = 0;
	cfg0 = NFC_ONLY_ECC_MODE | MAIN_SPAR_APT;
	if(dir)
	{
		cfg0 |= NFC_RW;
	}
	cfg1 |=(param->sinfo_size - 1) << SPAR_INFO_SIZE_OFFSET;
	cfg1 |=(param->sp_size - 1) << SPAR_SIZE_OFFSET;
	cfg1 |= (param->m_size - 1);
	
	cfg2 |= (param->sinfo_pos)<< SPAR_INFO_POS_OFFSET;
	cfg2 |= ecc_mode_convert(param->mode) << ECC_MODE_OFFSET;
	cfg2 |= param->ecc_pos;
	sprd_tiger_reg_write(NFC_CFG0_REG, cfg0);
	sprd_tiger_reg_write(NFC_CFG1_REG, cfg1);
	sprd_tiger_reg_write(NFC_CFG2_REG, cfg2);
	sprd_tiger_nand_int_clr(INT_STSMCH_CLR | INT_WP_CLR | INT_TO_CLR | INT_DONE_CLR);//clear all interrupt
	sprd_tiger_reg_write(NFC_START_REG, NFC_START);
	sprd_tiger_nand_wait_finish(&g_tiger);
}
unsigned int sprd_ecc_encode(struct sprd_ecc_param *param)
{
	struct sprd_tiger_nand_info *tiger;
	sprd_tiger_nand_data_param d_param;

	tiger = &g_tiger;
	memset(&d_param, 0, sizeof(d_param));

	d_param.m_buf = param->p_mbuf;
	d_param.s_buf = param->p_sbuf;
	d_param.m_sct = param->ecc_num;
	d_param.s_sct = param->ecc_num;
	d_param.dir = 1;
	d_param.m_size = param->m_size;
	d_param.s_size = param->sp_size;
	sprd_tiger_data_trans(&d_param);
	sprd_ecc_ctrl(param, 1);
	d_param.dir = 0;
	d_param.m_sct = 0;
	sprd_tiger_data_trans(&d_param); //read the ecc value from nfc buffer
	return 0;
}
#endif
//add one macro instruction to nand controller
/*static */void sprd_tiger_nand_ins_init(struct sprd_tiger_nand_info *tiger)
{
	tiger->ins_num = 0;
}
/*static */void sprd_tiger_nand_ins_add(uint16_t ins, struct sprd_tiger_nand_info *tiger)
{
	uint16_t *buf = (uint16_t *)tiger->ins;
	if(tiger->ins_num >= NAND_MC_BUFFER_SIZE)
	{
		while(1);
	}
	*(buf + tiger->ins_num) = ins;
	tiger->ins_num++;
}
/*static */void sprd_tiger_nand_ins_exec(struct sprd_tiger_nand_info *tiger)
{
	uint32_t i;
	uint32_t cfg0;
	
	for(i = 0; i < ((tiger->ins_num + 1) >> 1); i++)
	{
		sprd_tiger_reg_write(NFC_INST0_REG + (i << 2), tiger->ins[i]);
	}
	cfg0 = sprd_tiger_reg_read(NFC_CFG0_REG);
	if(tiger->wp_en)
	{
		cfg0 &= ~NFC_WPN;
	}
	else
	{
		cfg0 |= NFC_WPN;
	}
	if(tiger->chip)
	{
		cfg0 |= CS_SEL;
	}
	else
	{
		cfg0 &= ~CS_SEL;
	}
	sprd_tiger_nand_int_clr(INT_STSMCH_CLR | INT_WP_CLR | INT_TO_CLR | INT_DONE_CLR);//clear all interrupt
	sprd_tiger_reg_write(NFC_CFG0_REG, cfg0);
	sprd_tiger_reg_write(NFC_START_REG, NFC_START);
}
static int sprd_tiger_nand_wait_finish(struct sprd_tiger_nand_info *tiger)
{
	unsigned int value;
	unsigned int counter = 0;
	while((counter < NFC_TIMEOUT_VAL/*time out*/))
	{
		value = sprd_tiger_reg_read(NFC_INT_REG);
		if(value & INT_DONE_RAW)
		{
			break;
		}
		counter ++;
	}
	sprd_tiger_reg_write(NFC_INT_REG, 0xf00); //clear all interrupt status
	if(counter > NFC_TIMEOUT_VAL)
	{
		while (1);
		return -1;
	}
	return 0;
}
static void sprd_tiger_nand_wp_en(struct sprd_tiger_nand_info *tiger, int en)
{
	if(en)
	{
		tiger->wp_en = 1;
	}
	else
	{
		tiger->wp_en = 0;
	}
}
static void sprd_tiger_select_chip(struct mtd_info *mtd, int chip)
{
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);
	if(chip < 0) { //for release caller
		return;
	}
	//printf("sprd_tiger_select_chip, %x\r\n", chip);
	tiger->chip = chip;
#ifdef CONFIG_NAND_SPL
	nand_hardware_config(mtd,tiger->nand);
#endif
}
static void sprd_tiger_nand_read_status(struct sprd_tiger_nand_info *tiger)
{
	uint32_t *buf;
	//printf("sprd_tiger_nand_read_status\r\n");
	sprd_tiger_nand_ins_init(tiger);
	sprd_tiger_nand_ins_add(NAND_MC_CMD(NAND_CMD_STATUS), tiger);
	sprd_tiger_nand_ins_add(NAND_MC_IDST(1), tiger);
	sprd_tiger_nand_ins_add(NFC_MC_DONE_ID, tiger);
	sprd_tiger_reg_write(NFC_CFG0_REG, NFC_ONLY_NAND_MODE);
	sprd_tiger_nand_ins_exec(tiger);
	sprd_tiger_nand_wait_finish(tiger);
	buf = (uint32_t *)s_oob_data;
	*buf = sprd_tiger_reg_read(NFC_STATUS0_REG);
	tiger->buf_head = 0;
	tiger->_buf_tail = 1;
	//printf("--sprd_tiger_nand_read_status--\r\n");
}
static void sprd_tiger_nand_read_id(struct sprd_tiger_nand_info *tiger, uint32 *buf)
{
	//printf("sprd_tiger_nand_read_id\r\n");
	sprd_tiger_nand_ins_init(tiger);
	sprd_tiger_nand_ins_add(NAND_MC_CMD(NAND_CMD_READID), tiger);
	sprd_tiger_nand_ins_add(NAND_MC_ADDR(0), tiger);
	sprd_tiger_nand_ins_add(NAND_MC_IDST(8), tiger);
	sprd_tiger_nand_ins_add(NFC_MC_DONE_ID, tiger);
	
	sprd_tiger_reg_write(NFC_CFG0_REG, NFC_ONLY_NAND_MODE);
	sprd_tiger_nand_ins_exec(tiger);
	sprd_tiger_nand_wait_finish(tiger);
	*buf = sprd_tiger_reg_read(NFC_STATUS0_REG);
	*(buf + 1) = sprd_tiger_reg_read(NFC_STATUS1_REG);
	tiger->buf_head = 0;
	tiger->_buf_tail = 8;
	//printf("--sprd_tiger_nand_read_id--\r\n");
}
static void sprd_tiger_nand_reset(struct sprd_tiger_nand_info *tiger)
{
	//printf("sprd_tiger_nand_reset\r\n");
	sprd_tiger_nand_ins_init(tiger);
	sprd_tiger_nand_ins_add(NAND_MC_CMD(NAND_CMD_RESET), tiger);
	sprd_tiger_nand_ins_add(NFC_MC_WRB0_ID, tiger); //wait rb
	sprd_tiger_nand_ins_add(NFC_MC_DONE_ID, tiger);
	//config register
	sprd_tiger_reg_write(NFC_CFG0_REG, NFC_ONLY_NAND_MODE);
	sprd_tiger_nand_ins_exec(tiger);
	sprd_tiger_nand_wait_finish(tiger);
	//printf("--sprd_tiger_nand_reset--\r\n");
}
static u32 sprd_tiger_get_decode_sts(u32 index)
{
	uint32_t err;
	uint32_t shift;
	uint32_t reg_addr;
	reg_addr = NFC_STATUS0_REG + (index & 0xfffffffc);
	shift = (index & 0x3) >> 3;
	err = sprd_tiger_reg_read(reg_addr);
	err >>= shift;
	if((err & ECC_ALL_FF))
	{
		err = 0;
	}
	else
	{
		err &= ERR_ERR_NUM0_MASK;
	}
	return err;
}
//read large page
static int sprd_tiger_nand_read_lp(struct mtd_info *mtd,uint8_t *mbuf, uint8_t *sbuf,uint32_t raw)
{
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);
	struct nand_chip *chip = tiger->nand;
	uint32_t column;
	uint32_t page_addr;
	uint32_t cfg0;
	uint32_t cfg1;
	uint32_t cfg2;
	uint32_t i;
	uint32_t err;
	page_addr = tiger->page;
	
	if(sbuf) {
		column = mtd->writesize;
	}
	else
	{
		column = 0;
	}
	if(chip->options & NAND_BUSWIDTH_16)
	{
		column >>= 1;
	}
	//printf("sprd_tiger_nand_read_lp,page_addr = %x,column = %x\r\n",page_addr, column);
	sprd_tiger_nand_ins_init(tiger);
	sprd_tiger_nand_ins_add(NAND_MC_CMD(NAND_CMD_READ0), tiger);
	sprd_tiger_nand_ins_add(NAND_MC_ADDR(column & 0xff), tiger);
	column >>= 8;
	sprd_tiger_nand_ins_add(NAND_MC_ADDR(column & 0xff), tiger);

	sprd_tiger_nand_ins_add(NAND_MC_ADDR(page_addr & 0xff), tiger);
	page_addr >>= 8;
	sprd_tiger_nand_ins_add(NAND_MC_ADDR(page_addr & 0xff), tiger);
	
	if (5 == tiger->a_cycles)// five address cycles
	{
		page_addr >>= 8;
		sprd_tiger_nand_ins_add(NAND_MC_ADDR(page_addr & 0xff), tiger);
	}
	sprd_tiger_nand_ins_add(NAND_MC_CMD(NAND_CMD_READSTART), tiger);
	
	sprd_tiger_nand_ins_add(NFC_MC_WRB0_ID, tiger); //wait rb
	if(mbuf && sbuf)
	{
		sprd_tiger_nand_ins_add(NAND_MC_SRDT, tiger);
		//switch to main part
		sprd_tiger_nand_ins_add(NAND_MC_CMD(NAND_CMD_RNDOUT), tiger);
		sprd_tiger_nand_ins_add(NAND_MC_ADDR(0), tiger);
		sprd_tiger_nand_ins_add(NAND_MC_ADDR(0), tiger);
		sprd_tiger_nand_ins_add(NAND_MC_CMD(NAND_CMD_RNDOUTSTART), tiger);
		sprd_tiger_nand_ins_add(NAND_MC_MRDT, tiger);
	}
	else
	{
		sprd_tiger_nand_ins_add(NAND_MC_MRDT, tiger);
	}
	sprd_tiger_nand_ins_add(NFC_MC_DONE_ID, tiger);
	//config registers 
	cfg0 = NFC_AUTO_MODE | MAIN_SPAR_APT | ((tiger->sct_pg - 1)<< SECTOR_NUM_OFFSET);
	if((!raw) && mbuf && sbuf)
	{
		cfg0 |= ECC_EN | DETECT_ALL_FF;
	}
	if(chip->options & NAND_BUSWIDTH_16)
	{
		cfg0 |= BUS_WIDTH;
	}
	cfg1 = (tiger->info_size - 1) << SPAR_INFO_SIZE_OFFSET;
	cfg2 = (tiger->ecc_mode << ECC_MODE_OFFSET) | (tiger->info_pos << SPAR_INFO_POS_OFFSET) | ((tiger->sct_pg - 1) << SPAR_SECTOR_NUM_OFFSET) | tiger->ecc_pos;

	if(mbuf && sbuf)
	{
		cfg1 |= (tiger->m_size - 1) | ((tiger->s_size  - 1)<< SPAR_SIZE_OFFSET);
		sprd_tiger_reg_write(NFC_MAIN_ADDR_REG, (uint32_t)mbuf);
		sprd_tiger_reg_write(NFC_SPAR_ADDR_REG, (uint32_t)sbuf);
		cfg0 |= MAIN_USE | SPAR_USE;
	}
	else
	{
		if(mbuf)
		{
			cfg1 |= (tiger->m_size - 1);
			sprd_tiger_reg_write(NFC_MAIN_ADDR_REG, (uint32_t)mbuf);
		}
		if(sbuf)
		{
			cfg1 |= (tiger->s_size - 1);
			sprd_tiger_reg_write(NFC_MAIN_ADDR_REG, (uint32_t)sbuf);
		}
		cfg0 |= MAIN_USE;
	}	
	sprd_tiger_reg_write(NFC_CFG0_REG, cfg0);
	sprd_tiger_reg_write(NFC_CFG1_REG, cfg1);
	sprd_tiger_reg_write(NFC_CFG2_REG, cfg2);
	
	sprd_tiger_nand_ins_exec(tiger);
	sprd_tiger_nand_wait_finish(tiger);
	if(!raw) {
		for(i = 0; i < tiger->sct_pg; i++) {
			err = sprd_tiger_get_decode_sts(i);
			if(err == ERR_ERR_NUM0_MASK) {
				mtd->ecc_stats.failed++;
			}
			else {
				mtd->ecc_stats.corrected += err;
			}
		}
	}
	
	return 0;
}
static int sprd_tiger_nand_read_sp(struct mtd_info *mtd,uint8_t *mbuf, uint8_t *sbuf,uint32_t raw)
{
	return 0;
}
static int sprd_tiger_nand_write_lp(struct mtd_info *mtd,const uint8_t *mbuf, uint8_t *sbuf,uint32_t raw)
{
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);
	struct nand_chip *chip = tiger->nand;
	uint32_t column;
	uint32_t page_addr;
	uint32_t cfg0;
	uint32_t cfg1;
	uint32_t cfg2;
	page_addr = tiger->page;
	if(mbuf) {
		column = 0;
	}
	else {
		column = mtd->writesize;
	}	
	if(chip->options & NAND_BUSWIDTH_16)
	{
		column >>= 1;
	}
	sprd_tiger_nand_ins_init(tiger);
	sprd_tiger_nand_ins_add(NAND_MC_CMD(NAND_CMD_SEQIN), tiger);
	sprd_tiger_nand_ins_add(NAND_MC_ADDR(column & 0xff), tiger);
	column >>= 8;
	sprd_tiger_nand_ins_add(NAND_MC_ADDR(column & 0xff), tiger);
	sprd_tiger_nand_ins_add(NAND_MC_ADDR(page_addr & 0xff), tiger);
	page_addr >>= 8;
	sprd_tiger_nand_ins_add(NAND_MC_ADDR(page_addr & 0xff), tiger);
	
	if (5 == tiger->a_cycles)// five address cycles
	{
		page_addr >>= 8;
		sprd_tiger_nand_ins_add(NAND_MC_ADDR(page_addr & 0xff), tiger);
	}
	
	sprd_tiger_nand_ins_add(NAND_MC_MWDT, tiger);
	if(mbuf && sbuf)
	{
		sprd_tiger_nand_ins_add(NAND_MC_SWDT, tiger);
	}
	sprd_tiger_nand_ins_add(NAND_MC_CMD(NAND_CMD_PAGEPROG), tiger);
	sprd_tiger_nand_ins_add(NFC_MC_WRB0_ID, tiger); //wait rb

	sprd_tiger_nand_ins_add(NFC_MC_DONE_ID, tiger);
	//config registers 
	cfg0 = NFC_AUTO_MODE | NFC_RW |  NFC_WPN | MAIN_SPAR_APT | ((tiger->sct_pg - 1)<< SECTOR_NUM_OFFSET);
	if((!raw) && mbuf && sbuf)
	{
		cfg0 |= ECC_EN;
	}
	if(chip->options & NAND_BUSWIDTH_16)
	{
		cfg0 |= BUS_WIDTH;
	}
	cfg1 = ((tiger->info_size - 1) << SPAR_INFO_SIZE_OFFSET);
	cfg2 = (tiger->ecc_mode << ECC_MODE_OFFSET) | (tiger->info_pos << SPAR_INFO_POS_OFFSET) | ((tiger->sct_pg - 1) << SPAR_SECTOR_NUM_OFFSET) | tiger->ecc_pos;
	if(mbuf && sbuf)
	{
		cfg0 |= MAIN_USE | SPAR_USE;
		cfg1 = (tiger->m_size - 1) | ((tiger->s_size - 1) << SPAR_SIZE_OFFSET);
		sprd_tiger_reg_write(NFC_MAIN_ADDR_REG, (uint32_t)mbuf);
		sprd_tiger_reg_write(NFC_SPAR_ADDR_REG, (uint32_t)sbuf);
	}
	else
	{
		cfg0 |= MAIN_USE;
		if(mbuf)
		{
			cfg1 |= tiger->m_size - 1;
			sprd_tiger_reg_write(NFC_MAIN_ADDR_REG, (uint32_t)mbuf);
		}
		else
		{
			cfg1 |= tiger->s_size - 1;
			sprd_tiger_reg_write(NFC_MAIN_ADDR_REG, (uint32_t)sbuf);
		}
	}
	sprd_tiger_reg_write(NFC_CFG0_REG, cfg0);
	sprd_tiger_reg_write(NFC_CFG1_REG, cfg1);
	sprd_tiger_reg_write(NFC_CFG2_REG, cfg2);
	sprd_tiger_nand_ins_exec(tiger);
	sprd_tiger_nand_wait_finish(tiger);
	return 0;
}
static int sprd_tiger_nand_write_sp(struct mtd_info *mtd,const uint8_t *mbuf, uint8_t *sbuf,uint32_t raw)
{
	return 0;
}
static void sprd_tiger_erase(struct mtd_info *mtd, int page_addr)
{
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);
	uint32_t cfg0 = 0;
	//printf("sprd_tiger_erase, %x\r\n", page_addr);
	sprd_tiger_nand_ins_init(tiger);
	sprd_tiger_nand_ins_add(NAND_MC_CMD(NAND_CMD_ERASE1), tiger);
	sprd_tiger_nand_ins_add(NAND_MC_ADDR(page_addr & 0xff), tiger);
	page_addr >>= 8;
	sprd_tiger_nand_ins_add(NAND_MC_ADDR(page_addr & 0xff), tiger);
	if((5 == tiger->a_cycles) || ((4 == tiger->a_cycles) && (512 == tiger->write_size)))
	{
		page_addr >>= 8;
		sprd_tiger_nand_ins_add(NAND_MC_ADDR(page_addr & 0xff), tiger);
	}
	sprd_tiger_nand_ins_add(NAND_MC_CMD(NAND_CMD_ERASE2), tiger);
	sprd_tiger_nand_ins_add(NFC_MC_WRB0_ID, tiger); //wait rb

	sprd_tiger_nand_ins_add(NFC_MC_DONE_ID, tiger);
	cfg0 = NFC_WPN | NFC_ONLY_NAND_MODE;
	sprd_tiger_reg_write(NFC_CFG0_REG, cfg0);
	sprd_tiger_nand_ins_exec(tiger);
	sprd_tiger_nand_wait_finish(tiger);
	//printf("--sprd_tiger_erase--\r\n");
}
static uint8_t sprd_tiger_read_byte(struct mtd_info *mtd)
{
	uint8_t ch = 0xff;
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);
	if(tiger->buf_head < tiger->_buf_tail)
	{
		ch = s_oob_data[tiger->buf_head ++];
	}
	return ch;
}
static uint16_t sprd_tiger_read_word(struct mtd_info *mtd)
{
	uint16_t data = 0xffff;
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);
	if(tiger->buf_head < (tiger->_buf_tail - 1))
	{
		data = s_oob_data[tiger->buf_head ++];
		data |= ((uint16_t)s_oob_data[tiger->buf_head ++]) << 8;
	}
	return data;
}
static int sprd_tiger_waitfunc(struct mtd_info *mtd, struct nand_chip *chip)
{
	return 0;
}
static int sprd_tiger_ecc_calculate(struct mtd_info *mtd, const uint8_t *data,
				uint8_t *ecc_code)
{
	return 0;
}
static int sprd_tiger_ecc_correct(struct mtd_info *mtd, uint8_t *data,
				uint8_t *read_ecc, uint8_t *calc_ecc)
{
	return 0;
}
static int sprd_tiger_read_page(struct mtd_info *mtd, struct nand_chip *chip,
			uint8_t *buf, int page)
{
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);
	tiger->page = page;
	if(512 == mtd->writesize)
	{
		sprd_tiger_nand_read_sp(mtd, buf, chip->oob_poi, 0);
	}
	else
	{
		sprd_tiger_nand_read_lp(mtd, buf, chip->oob_poi, 0);
	}
	return 0;
}
static int sprd_tiger_read_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
				uint8_t *buf, int page)
{
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);
	tiger->page = page;
	if(512 == mtd->writesize)
	{
		sprd_tiger_nand_read_sp(mtd, buf, chip->oob_poi, 1);
	}
	else
	{
		sprd_tiger_nand_read_lp(mtd, buf, chip->oob_poi, 1);
	}
	return 0;
}
static int sprd_tiger_read_oob(struct mtd_info *mtd, struct nand_chip *chip,
			   int page, int sndcmd)
{
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);
	tiger->page = page;
	if(512 == mtd->writesize)
	{
		sprd_tiger_nand_read_sp(mtd, 0, chip->oob_poi, 1);
	}
	else
	{
		sprd_tiger_nand_read_lp(mtd, 0, chip->oob_poi, 1);
	}
	return 0;
}
static void sprd_tiger_write_page(struct mtd_info *mtd, struct nand_chip *chip,
				const uint8_t *buf)
{
	if(512 == mtd->writesize)
	{
		sprd_tiger_nand_write_sp(mtd, buf, chip->oob_poi, 0);	
	}
	else
	{
		sprd_tiger_nand_write_lp(mtd, buf, chip->oob_poi, 0);	
	}

}
static void sprd_tiger_write_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
					const uint8_t *buf)
{
	if(512 == mtd->writesize)
	{
		sprd_tiger_nand_write_sp(mtd, buf, chip->oob_poi, 1);	
	}
	else
	{
		sprd_tiger_nand_write_lp(mtd, buf, chip->oob_poi, 1);	
	}
}
static int sprd_tiger_write_oob(struct mtd_info *mtd, struct nand_chip *chip,
				int page)
{
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);	
	tiger->page = page;
	if(512 == mtd->writesize)
	{
		sprd_tiger_nand_write_sp(mtd, 0, chip->oob_poi, 1);
	}
	else
	{
		sprd_tiger_nand_write_lp(mtd, 0, chip->oob_poi, 1);
	}
	return 0;
}

/**
 * nand_block_bad - [DEFAULT] Read bad block marker from the chip
 * @mtd:	MTD device structure
 * @ofs:	offset from device start
 * @getchip:	0, if the chip is already selected
 *
 * Check, if the block is bad.
 */
static int sprd_tiger_block_bad(struct mtd_info *mtd, loff_t ofs, int getchip)
{
	int page, chipnr, res = 0;
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);
	struct nand_chip *chip = mtd->priv;
	uint16_t bad;
	uint16_t *buf;

	page = (int)(ofs >> chip->page_shift) & chip->pagemask;

	if (getchip) {
		chipnr = (int)(ofs >> chip->chip_shift);
		/* Select the NAND device */
		chip->select_chip(mtd, chipnr);
	}

	chip->cmdfunc(mtd, NAND_CMD_READOOB, 0, page);
	if(512 == tiger->write_size) {
		sprd_tiger_nand_read_sp(mtd, 0, s_oob_data, 1);
	}
	else  {
		sprd_tiger_nand_read_lp(mtd, 0, s_oob_data, 1);
	}
	tiger->buf_head = 0;
	tiger->_buf_tail = mtd->oobsize;
	buf = (uint16_t *)(s_oob_data + chip->badblockpos);

	if (chip->options & NAND_BUSWIDTH_16) {
		bad = *(buf);
		if ((bad & 0xFF) != 0xff) {
			res = 1;
		}
	} else {
		bad = *(buf) & 0xff;
		if (bad != 0xff){
			res = 1;
		}
	}
	return res;
}
static void sprd_tiger_nand_cmdfunc(struct mtd_info *mtd, unsigned int command,
			    int column, int page_addr)
{
	struct sprd_tiger_nand_info *tiger = mtd_to_tiger(mtd);
	/* Emulate NAND_CMD_READOOB */
	if (command == NAND_CMD_READOOB) {
		column += mtd->writesize;
		command = NAND_CMD_READ0;
	}
	/*
	 * program and erase have their own busy handlers
	 * status, sequential in, and deplete1 need no delay
	 */
	switch (command) {
	case NAND_CMD_STATUS:
		sprd_tiger_nand_read_status(tiger);
		break;
	case NAND_CMD_READID:
		sprd_tiger_nand_read_id(tiger, (uint32 *)s_oob_data);
		break;
	case NAND_CMD_RESET:
		sprd_tiger_nand_reset(tiger);
		break;
	case NAND_CMD_ERASE1:
		sprd_tiger_erase(mtd, page_addr);
		break;
	case NAND_CMD_READ0:
	case NAND_CMD_SEQIN:
		tiger->column = column;
		tiger->page = page_addr;
	default:
		break;
	}
}
static void sprd_tiger_nand_hwecc_ctl(struct mtd_info *mtd, int mode)
{
	return; //do nothing
}
static void sprd_tiger_nand_hw_init(struct sprd_tiger_nand_info *tiger)
{
	int i = 0;
	uint32_t val;
	
	sprd_tiger_reg_or(0x20900200, BIT_8);
	
	val = sprd_tiger_reg_read(0x20900210);
	sprd_tiger_reg_or(0x20900210, BIT_5);
	for(i = 0; i < 0xffff; i++);
	sprd_tiger_reg_and(0x20900210, ~BIT_5);
	val |= (3)  | (4 << NFC_RWH_OFFSET) | (3 << NFC_RWE_OFFSET) | (3 << NFC_RWS_OFFSET) | (3 << NFC_ACE_OFFSET) | (3 << NFC_ACS_OFFSET);
	
	sprd_tiger_reg_write(NFC_TIMING_REG, val);
	sprd_tiger_reg_write(NFC_TIMEOUT_REG, 0x80400000);
	//close write protect
	sprd_tiger_nand_wp_en(tiger, 0);
}
int board_nand_init(struct nand_chip *chip)
{
	printf("board_nand_init\r\n");
	sprd_tiger_nand_hw_init(&g_tiger);
	chip->select_chip = sprd_tiger_select_chip;
	chip->cmdfunc = sprd_tiger_nand_cmdfunc;
	chip->read_byte = sprd_tiger_read_byte;
	chip->read_word	= sprd_tiger_read_word;
	chip->waitfunc = sprd_tiger_waitfunc;
	chip->ecc.mode = NAND_ECC_HW;
	chip->ecc.calculate = sprd_tiger_ecc_calculate;
	chip->ecc.hwctl = sprd_tiger_nand_hwecc_ctl;
	
	chip->ecc.correct = sprd_tiger_ecc_correct;
	chip->ecc.read_page = sprd_tiger_read_page;
	chip->ecc.read_page_raw = sprd_tiger_read_page_raw;
	chip->ecc.write_page = sprd_tiger_write_page;
	chip->ecc.write_page_raw = sprd_tiger_write_page_raw;
	chip->ecc.read_oob = sprd_tiger_read_oob;
	chip->ecc.write_oob = sprd_tiger_write_oob;
	chip->erase_cmd = sprd_tiger_erase;
	
	chip->ecc.bytes = CONFIG_SYS_NAND_ECCBYTES;
	g_tiger.ecc_mode = CONFIG_SYS_NAND_ECC_MODE;
	g_tiger.nand = chip;
	chip->eccbitmode = g_tiger.ecc_mode;
	chip->ecc.size = CONFIG_SYS_NAND_ECCSIZE;
	
	chip->options |= NAND_BUSWIDTH_16;

	return 0;
}
void read_chip_id(void)
{
}

#ifndef CONFIG_NAND_SPL
void McuReadNandType(unsigned char *array)
{

}
#endif

void nand_scan_patition(int blocks, int erasesize, int writesize)
{

}
