/* ============================================================================
@
@ MMU function for armV7
@ steve.zhan
@ Spreadtrum Communications Inc.
@
@ ============================================================================
*/

@AREA mmu_functions, CODE, READONLY
@    CODE32

#include <config.h>
@   Initialize the MMU
@
.globl g_mmu_page_table
@    IMPORT  mmu_create_page_table
.globl MMU_EnableIDCM
.globl MMU_InvalideDCACHEALL
.globl MMU_InvalideICACHEALL    
.globl Dcache_InvalRegion
.globl Dcache_CleanRegion


@============================================================================
@ Dcache_InvalRegion
@
@ PROTOTYPE
@ void Dcache_InvalRegion(void *addr, unsigned int length)
@
@ steve.zhan
@ DESCRIPTION
@ Invalid a memory region in the cache.
@============================================================================
Dcache_InvalRegion:
    stmfd sp!, {r0-r3, lr}
@    .word 0xf57ff04f @dsb
    add r1,r1,r0

    mrc p15, 1, r3, c0, c0, 0
    and r3, r3, #7
    mov r2, #16
    mov r2, r2, lsl r3
    
    sub r3, r2, #1
    tst r0, r3
    bic r0, r0, r3
    mcrne p15, 0, r0, c7, c14, 1

    tst r1, r3
    bic r1, r1, r3
    mcrne p15, 0, r1, c7, c14, 1

1:
    mcr p15, 0, r0, c7, c6, 1
    add r0, r0, r2
    cmp r0, r1
    blo 1b
    .word 0xf57ff04f

    ldmfd sp!, {r0-r3, lr}
    
@============================================================================
@ Dcache_CleanRegion
@
@ PROTOTYPE
@ void Dcache_CleanRegion(void *addr, unsigned int length)
@
@ steve.zhan
@ DESCRIPTION
@ clean a memory region in the cache.
@============================================================================
Dcache_CleanRegion:
	stmfd sp!, {r0-r3, lr}
@	.word 0xf57ff04f @dsb

	add r1,r1,r0
	mrc p15, 1, r3, c0, c0, 0
	and r3, r3, #7
	mov r2, #16
	mov r2, r2, lsl r3

	sub r3, r2, #1
	bic r0, r0, r3

1:
	mcr p15, 0, r0, c7, c10, 1 @p15, 0, r0, c7, c14, 1  is clean and invalid 
	add r0, r0, r2
	cmp r0, r1
	blo 1b

	.word 0xf57ff04f @dsb
	.word 0xf57ff05f @dmb
	ldmfd sp!, {r0-r3,pc}	

 
MMU_InvalideICACHEALL:
    STMFD   sp!, {a1-a4, lr}            @ save lr_USR and non-callee
    
    MOV     r0, #0    
    MCR     p15, 0, r0, c7, c5, 0       @Invalidate(flush)the ICache
    MCR     p15, 0, r0, c8, c5, 0       @flush ITLB only
    NOP                                               @next few instructions may be via cache.
    NOP
    NOP
    NOP
    NOP

    LDMFD   sp!, {a1-a4, PC}             @ restore registers
    

MMU_EnableIDCM:
@ save lr_USR and non-callee
	STMFD   sp!, {a1-a4, lr}               

@   Set the MMU page table address
    LDR     r2, =g_mmu_page_table
    LDR     r2, [r2]
    MCR     p15, 0, r2, c2, c0, 0
    
@   Domain Access Control: set all domains to manager
@
@   NOTE:
@   We must set domain access before enble MMU, otherwise Bus-Error will occur!
    LDR     r0, =0xFFFFFFFF
    MCR     p15, 0, r0, c3, c0, 0
    
@   Enable the ICache, DCache, write buffer, MMU
    MRC     p15, 0, r0, c1, c0, 0
    LDR     r1, =0x107D
    ORR     r0, r0, r1
    MCR     p15, 0, r0, c1, c0, 0 
    
@   Delay for the operations to finish
    NOP
    NOP
    NOP
    NOP
    NOP

@   Return
	LDMFD   sp!, {a1-a4, PC}             @ restore registers 

.globl	MMU_DisableIDCM
MMU_DisableIDCM:

@ save lr_USR and non-callee
	STMFD   sp!, {a1-a4, lr}               

@   Disable the ICache, DCache, write buffer, MMU
    MRC     p15, 0, r0, c1, c0, 0
    LDR     r1, =0x1005
    BIC     r0, r0, r1
    ORR     r0, r0, #0x8     @Bit_7 must be one when write c1@
    MCR     p15, 0, r0, c1, c0, 0

@write back data in data cache to memory system @
tcv_loop:
    MRC p15, 0, r15, c7, c14, 3 @ test clean and invalidate
	BNE tcv_loop

@   Invalidate the ICache and DCache
	MOV     r0, #0
    MCR     p15, 0, r0, c7, c7, 0

@   Delay for the operations to finish
    NOP
    NOP
    NOP

@   Return
	LDMFD   sp!, {a1-a4, PC}             @ restore registers 

