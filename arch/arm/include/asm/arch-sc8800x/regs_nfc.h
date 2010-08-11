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

#ifndef _SC8800H_REG_NFC_H_
#define _SC8800H_REG_NFC_H_

#include <asm/arch/regs_global.h>
#include <asm/arch/bits.h>

#define NFC_REG_BASE    	        ARM_NAND_BASE

#define NFC_MBUF			(NFC_REG_BASE + 0x0000)
#define NFC_SBUF			(NFC_REG_BASE + 0x0c00)

#define NFC_CMD     	           	(NFC_REG_BASE + 0x1c00)
#define NFC_STR0                   	(NFC_REG_BASE + 0x1c04)
#define NFC_STR1                   	(NFC_REG_BASE + 0x1c08)
#define NFC_STR2	            	(NFC_REG_BASE + 0x1c0c)
#define NFC_STR3	               	(NFC_REG_BASE + 0x1c10)
#define NFC_END0			(NFC_REG_BASE + 0x1c14)
#define NFC_END1			(NFC_REG_BASE + 0x1c18)
#define NFC_END2			(NFC_REG_BASE + 0x1c1c)
#define NFC_END3			(NFC_REG_BASE + 0x1c20)
#define NFC_PARA			(NFC_REG_BASE + 0x1c24)

#define NFC_PAGEECC0			(NFC_REG_BASE + 0x1c28)
#define NFC_PAGEECC1			(NFC_REG_BASE + 0x1c2c)
#define NFC_PAGEECC2			(NFC_REG_BASE + 0x1c30)
#define NFC_PAGEECC3			(NFC_REG_BASE + 0x1c34)

#define NFC_CMDCLR			(NFC_REG_BASE + 0x1c48)
#define NFC_RB				(NFC_REG_BASE + 0x1c80)

#define NFC_INTSRC			(NFC_REG_BASE + 0x1c84)
#define NFC_INTEN			(NFC_REG_BASE + 0x1c88)
#define NFC_INTMSK			(NFC_REG_BASE + 0x1c8c)

#define NFC_WPN				(NFC_REG_BASE + 0x1c38)
#define NFC_IDSTATUS			(NFC_REG_BASE + 0x1c3c)
#define NFC_ECCEN			(NFC_REG_BASE + 0x1d00)
#define NFC_TIMEOUT			(NFC_REG_BASE + 0x1d18) 

#define REG_NFC_MBUF                    (*((volatile unsigned int *)(NFC_MBUF)))
#define REG_NFC_SBUF                    (*((volatile unsigned int *)(NFC_SBUF)))
#define REG_NFC_CMD                     (*((volatile unsigned int *)(NFC_CMD)))
#define REG_NFC_STR0                    (*((volatile unsigned int *)(NFC_STR0)))
#define REG_NFC_STR1                    (*((volatile unsigned int *)(NFC_STR1)))
#define REG_NFC_STR2                    (*((volatile unsigned int *)(NFC_STR2)))
#define REG_NFC_STR3                    (*((volatile unsigned int *)(NFC_STR3)))
#define REG_NFC_END0                    (*((volatile unsigned int *)(NFC_END0)))
#define REG_NFC_END1                    (*((volatile unsigned int *)(NFC_END1)))
#define REG_NFC_END2                    (*((volatile unsigned int *)(NFC_END2)))
#define REG_NFC_END3                    (*((volatile unsigned int *)(NFC_END3)))
#define REG_NFC_PARA                    (*((volatile unsigned int *)(NFC_PARA)))

#define REG_NFC_PAGEECC0                (*((volatile unsigned int *)(NFC_PAGEECC0)))
#define REG_NFC_PAGEECC1                (*((volatile unsigned int *)(NFC_PAGEECC1)))
#define REG_NFC_PAGEECC2                (*((volatile unsigned int *)(NFC_PAGEECC2)))
#define REG_NFC_PAGEECC3                (*((volatile unsigned int *)(NFC_PAGEECC3)))

#define REG_NFC_WPN			(*((volatile unsigned int *)(NFC_WPN)))
#define REG_NFC_ECCEN			(*((volatile unsigned int *)(NFC_ECCEN)))
#define REG_NFC_INTSRC			(*((volatile unsigned int *)(NFC_INTSRC)))
#define REG_NFC_IDSTATUS		(*((volatile unsigned int *)(NFC_IDSTATUS)))

#endif 

