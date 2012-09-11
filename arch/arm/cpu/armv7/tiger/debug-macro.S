/* linux/arch/arm/mach-sc8800g/include/mach/debug-macro.S
 *
 * Copyright (C) 2010 Spreadtrum
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <asm/arch/sc8810_reg_base.h>

.globl addruart
addruart:
	@ see if the MMU is enabled and select appropriate base address
	mrc	p15, 0, r3, c1, c0
	tst	r3, #1
	@ldreq	\rx, =SPRD_SERIAL1_PHYS
	ldrne	r3, =ARM_UART1_BASE
	mov pc,lr

.globl senduart	
senduart:
	and	r1,r1,#0xFF
	str	r1, [r3, #0x00]
	mov pc,lr

.globl waituart	
waituart:
	@ wait for TX_READY
1:	ldr	r2, [r3, #0x0C]
	mov	r2,r2,lsr #8
	and	r2,r2,#0xFF
	teq	r2, #0x00
	bne	1b
	mov pc,lr	

.globl busyuart	
busyuart:
	@ wait for TX_READY
2:	ldr	r2, [r3, #0x0C]
	mov	r2,r2,lsr #8
	and	r2,r2,#0xFF
	teq	r2, #0x00
	bne	2b
	mov pc,lr
