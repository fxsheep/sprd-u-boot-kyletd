/*
 * arch/arm/mach-sc8800s/include/mach/regs_nfc.h
 *
 * Chip nand flash controller registers Definitions
 *
 * Copyright (C) 2010 Spreadtrum International Ltd.
 *
 * 2010-05-25: yingchun li <yingchun.li@spreadtrum.com>
 *            initial version
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#ifndef _SC8810_REG_NFC_H_
#define _SC8810_REG_NFC_H_

#include <asm/arch/chip_drv_config_extern.h>

#define NFC_REG_BASE	(0x60000000)

#define NFC_MBUF_ADDR	(NFC_REG_BASE + 0x2000)
#define NFC_SBUF_ADDR	(NFC_REG_BASE + 0x4000)

#define NFC_CMD		(NFC_REG_BASE + 0x0000)
#define NFC_CFG0	(NFC_REG_BASE + 0x0004)
#define NFC_CFG1	(NFC_REG_BASE + 0x0008)
#define NFC_TIMING	(NFC_REG_BASE + 0x0010)
#define NFC_TIEOUT	(NFC_REG_BASE + 0x0014)
#define NFC_ID_STS	(NFC_REG_BASE + 0x0018)
#define NFC_STS_EN	(NFC_REG_BASE + 0x0020)
#define NFC_CLR_RAW	(NFC_REG_BASE + 0x0024)

#define NFC_ECC_CFG0	(NFC_REG_BASE + 0x0030)
#define NFC_ECC_CFG1	(NFC_REG_BASE + 0x0034)

#define NFC_ECC_STS0	(NFC_REG_BASE + 0x0040)
#define NFC_ECC_STS1	(NFC_REG_BASE + 0x0044)
#define NFC_ECC_STS2	(NFC_REG_BASE + 0x0048)
#define NFC_ECC_STS3	(NFC_REG_BASE + 0x004C)

#define NFC_START_ADDR0	(NFC_REG_BASE + 0x0060)
#define NFC_START_ADDR1	(NFC_REG_BASE + 0x0064)
#define NFC_START_ADDR2	(NFC_REG_BASE + 0x0068)
#define NFC_START_ADDR3	(NFC_REG_BASE + 0x006C)
#define NFC_START_ADDR4	(NFC_REG_BASE + 0x0070)
#define NFC_START_ADDR5	(NFC_REG_BASE + 0x0074)
#define NFC_START_ADDR6	(NFC_REG_BASE + 0x0078)
#define NFC_START_ADDR7	(NFC_REG_BASE + 0x007C)

#define NFC_END_ADDR0	(NFC_REG_BASE + 0x0080)
#define NFC_END_ADDR1	(NFC_REG_BASE + 0x0084)
#define NFC_END_ADDR2	(NFC_REG_BASE + 0x0088)
#define NFC_END_ADDR3	(NFC_REG_BASE + 0x008c)
#define NFC_END_ADDR4	(NFC_REG_BASE + 0x0090)
#define NFC_END_ADDR5	(NFC_REG_BASE + 0x0094)
#define NFC_END_ADDR6	(NFC_REG_BASE + 0x0098)
#define NFC_END_ADDR7	(NFC_REG_BASE + 0x009C)



//bit define NFC_CMD
#define NFC_CMD_VALID		(1 << 31)
#define NFC_BLKNUM_OFFSET	(16)
//bit define NFC_CFG0
#define NFC_SP_SIZE_OFFSET	16
#define NFC_CMD_SET_OFFSET	15
#define NFC_ADVANCE		(1 << 14)
#define NFC_ADDR_3CYCLES	0
#define NFC_ADDR_4CYCLES	(1 << 12)
#define NFC_ADDR_5CYCLES	(2 << 12)
#define NFC_PAGE_TYPE_512	0
#define NFC_PAGE_TYPE_1K	(1 << 9)
#define NFC_PAGE_TYPE_2K	(2 << 9)
#define NFC_PAGE_TYPE_4K	(3 << 9)
#define NFC_PAGE_TYPE_8K	(4 << 9)

#define NFC_CS1_SEL		(1 << 6)
#define NFC_BUS_WIDTH_16	(1 << 5)
#define NFC_NFC_MEM_SWITCH	(1 << 4)
#define NFC_MEM_NFC_SWITCH	(1 << 3)
#define NFC_WPN			(1 << 2)
#define NFC_CMD_CLR		(1 << 1)
#define NFC_RBN			(1 << 0)
//bit define NFC_CFG1
#define NFC_DMA_RX_EN		(1 << 1)
#define NFC_DMA_TX_EN		(1 << 0)
//bit define NFC_TIMING
#define NFC_ACS_OFFSET		(0)
#define NFC_RWH_OFFSET		(5)
#define NFC_RWL_OFFSET		(10)
#define NFC_ACR_OFFSET		(16)
#define NFC_RR_OFFSET		(21)
#define NFC_CEH_OFFSET		(26)
//bit define NFC_INT_STS_EN
#define NFC_DONE_EN		(1 << 0)
#define NFC_ECC_DONE_EN		(1 << 1)
#define NFC_ERR_EN		(1 << 2)
#define NFC_WP_EN		(1 << 3)
#define NFC_TO_EN		(1 << 4)

#define NFC_DONE_STS		(1 << 16)
#define NFC_ECC_DONE_STS	(1 << 16)
#define NFC_ERR_STS		(1 << 16)
#define NFC_WP_STS		(1 << 16)
#define NFC_TO_STS		(1 << 16)

#define NFC_DONE_RAW		(1 << 0)
#define NFC_ECC_DONE_RAW	(1 << 1)
#define NFC_ERR_RAW		(1 << 2)
#define NFC_WP_RAW		(1 << 3)
#define NFC_TO_RAW		(1 << 4)

#define NFC_DONE_CLR		(1 << 16)
#define NFC_ECC_DONE_CLR	(1 << 17)
#define NFC_ERR_CLR		(1 << 18)
#define NFC_WP_CLR		(1 << 19)
#define NFC_TO_CLR		(1 << 20)

#define NFC_ECC_SP_POS_OFFSET	(24)
#define NFC_ECC_SP_SIZE_OFFSET	(16)
#define NFC_ECC_NUM_OFFSET	(8)
#define NFC_ECC_MODE_OFFSET	(4)
#define NFC_ECC_MODE_1BIT	(0 << 4)
#define NFC_ECC_MODE_2BIT	(1 << 4)
#define NFC_ECC_MODE_4BIT	(2 << 4)
#define NFC_ECC_MODE_8BIT	(3 << 4)
#define NFC_ECC_MODE_12BIT	(4 << 4)
#define NFC_ECC_MODE_16BIT	(5 << 4)
#define NFC_ECC_MODE_24BIT	(6 << 4)
#define NFC_ECC_SP_ENDIAN	(1 << 3)
#define NFC_ECC_DECODE		(1 << 2)
#define NFC_ECC_AUTO_EN		(1 << 1)
#define NFC_ECC_ACTIVE		(1 << 0)

//bit define NFC_ECC_CFG1
#define NFC_ECC_MAIN_ADDR_OFFSET	(16)
#define NFC_ECC_LOC_MAIN		(1 << 15)
#define NFC_ECC_ERR_NUM_MASK		(0x1f)
struct sc8810_ecc_param {
	u8 mode; //1, 2, 4, 8,12,16,24
	u8 ecc_num; //max is 8
	u8 sp_size; //spare part size
	u8 ecc_pos; //ecc postion
	u16 m_size;// main part size;
	u8 *p_mbuf;
	u8 *p_sbuf;
	u8 *sts;
};
unsigned int sc8810_ecc_encode(struct sc8810_ecc_param *param);
unsigned int ecc_mode_convert(u32 mode);

#endif 

