/*
 *  armboot - Startup Code for ARM926EJS CPU-core
 *
 *  Copyright (c) 2003  Texas Instruments
 *
 *  ----- Adapted for OMAP1610 OMAP730 from ARM925t code ------
 *
 *  Copyright (c) 2001	Marius Gröger <mag@sysgo.de>
 *  Copyright (c) 2002	Alex Züpke <azu@sysgo.de>
 *  Copyright (c) 2002	Gary Jennejohn <garyj@denx.de>
 *  Copyright (c) 2003	Richard Woodruff <r-woodruff2@ti.com>
 *  Copyright (c) 2003	Kshitij <kshitij@ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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
#include <version.h>

#if defined(CONFIG_OMAP1610)
#include <./configs/omap1510.h>
#elif defined(CONFIG_OMAP730)
#include <./configs/omap730.h>
#endif

#ifdef CONFIG_SC8825
#define BIGEND_PROT_REG 0x20900290
#define AHB_CTRL5_REG  	0x20900230
#define FDL2_STACK 	0x81000000
#endif
#ifdef PLATFORM_SC8800G
#define BIGEND_PROT_REG 0x20900290
#define AHB_CTRL5_REG  0x20900230
#define FDL2_STACK 	0x31000000
#endif
#ifdef CONFIG_SC8810
#define BIGEND_PROT_REG 0x20900290
#define AHB_CTRL5_REG  0x20900230
#define FDL2_STACK 	0x1000000
#endif

/*
 *************************************************************************
 *
 * Jump vector table as in table 3.1 in [1]
 *
 *************************************************************************
 */


.globl _start
_start:
	b	reset
/*
 *************************************************************************
 *
 * Startup Code (reset vector)
 *
 * do important init only if we don't start from memory!
 * setup Memory and board specific bits prior to relocation.
 * relocate armboot to ram
 * setup stack
 *
 *************************************************************************
 */

/*
 * These are defined in the board-specific linker script.
 */
.globl _bss_start
_bss_start:
	.word __bss_start

.globl _bss_end
_bss_end:
	.word _end

.global _armboot_start
_armboot_start:
	.word _start

/*
 * the actual reset code
 */

reset:
	/*
	 * set the cpu to SVC32 mode
	 */
#if 0
	ldr	r0, =0xffffff
time:	subs	r0,r0,#0x1
	bne	time
#endif

	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0,#0xd3
	msr	cpsr,r0

	MRC p15,0,r0,c1,c0,0
#ifdef CHIP_ENDIAN_BIG
	ORR r0,r0,#0x80
#else
	BIC r0,r0,#0x80
#endif

	BIC r0,r0,#1	/*disable MMU*/
	LDR r1,=0x1004
	BIC r0,r0,r1	/*disable cache*/
	/*LDR r1,=0x1000*/  /* use mmu.c and mmu_asm.S, so richard feng remove it */
	/*ORR r0,r0,r1*/    /* use mmu.c and mmu_asm.S, so richard feng remove it */

	MCR p15,0,r0,c1,c0,0

	/*set endian regs of sc8800g*/
#if defined(CHIP_ENDIAN_BIG) && (defined(PLATFORM_SC8800G) || defined(CONFIG_SC8810))
	LDR     R0, =BIGEND_PROT_REG
	MOV     R2, #0xD4
	ORR     R2, R2, #0xC300
	STR     R2,[R0]

	LDR     R2, =AHB_CTRL5_REG
	MOV     R1, #0xff
	ORR     R1, R1, #0x300

	STR     R1, [R2]
	MOV     R1, #0
	STR     R1, [R0]
#endif

	/*set stack limit to 0*/
	MOV R10, #0
	ldr sp, =FDL2_STACK	
clear_bss:
	ldr	r0, _bss_start		/* find start of bss segment        */
	ldr	r1, _bss_end		/* stop here                        */
	mov	r2, #0x00000000		/* clear                            */

clbss_l:str	r2, [r0]		/* clear loop...                    */
	add	r0, r0, #4
	cmp	r0, r1
	ble	clbss_l


	b main
