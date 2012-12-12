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


#if defined CONFIG_SC8825
#define FDL1_STACK 0x00028000
#elif defined CONFIG_SC7710G2
#define FDL1_STACK 0x40008000
#else
#define FDL1_STACK 0x31000000
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

#if defined (CONFIG_SC8825) || defined (CONFIG_SC7710G2)
.local _bss0_start
_bss0_start:
	.word __bss0_start
.local _bss0_end
_bss0_end:
	.word __bss0_end
.local _bss1_start
_bss1_start:
	.word __bss1_start
.local _bss1_end
_bss1_end:
	.word __bss1_end
#else
.local _bss_start
_bss_start:
	.word __bss_start
.local _bss_end
_bss_end:
	.word _end
#endif
.local _armboot_start
_armboot_start:
	.word _start

/*
 * the actual reset code
 */

reset:
	/*
	 * set the cpu to SVC32 mode
	 */
	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0,#0xd3
	msr	cpsr,r0

	MRC p15,0,r0,c1,c0,0

	BIC r0,r0,#0x80
	BIC r0,r0,#1	/*disable MMU*/
	LDR r1,=0x1004
	BIC r0,r0,r1	/*disable cache*/

	MCR p15,0,r0,c1,c0,0

	/*set stack limit to 0*/
	MOV R10, #0
	ldr sp, =FDL1_STACK
#if defined (CONFIG_SC8825) || defined (CONFIG_SC7710G2)
	ldr r0, _bss0_start
	ldr r1, _bss0_end
	bl  clear_bss
#endif

#if 0
	MOV R0, #0
	MOV R1, #0
	MOV R2, #0
	MOV R3, #0
	bl  DMC_Init
#else
	LDR R0, =400000000
	bl  DMC_Dev_Init
#endif

#if defined (CONFIG_SC8825) || defined (CONFIG_SC7710G2)
	ldr r0, _bss1_start
	ldr r1, _bss1_end
	bl  clear_bss
#else
	ldr r0, _bss_start
	ldr r1, _bss_end
	bl  clear_bss
#endif

	b   main

clear_bss:
	mov	r2, #0x00000000		/* clear                            */
clbss_l:str	r2, [r0]		/* clear loop...                    */
	add	r0, r0, #4
	cmp	r0, r1
	ble	clbss_l
	bx	r14

