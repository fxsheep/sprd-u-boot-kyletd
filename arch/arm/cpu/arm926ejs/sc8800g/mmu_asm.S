/* ============================================================================
@
@ Enable the MMU function
@
@ @Liu Jun 2005-07-27
@ Spreadtrum Communications Inc.
@
@ NOTE:
@ 1. The ARM9 data cache must be used together with MMU, while the instruction
@    cache can be used independently.
@
@ Related Files:
@      mmu_c.c          Create the MMU page table
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

@============================================================================
@ Dcache_InvalRegion
@
@ PROTOTYPE
@   void Dcache_InvalRegion(void *addr, unsigned int length)@
@
@ steve.zhan
@ DESCRIPTION
@   Invalidate a memory region in the cache.
@============================================================================
Dcache_InvalRegion:
    STMFD sp!, {a1-a2, lr}               @ save lr_USR and non-callee
    ADD    r1, r0, r1                     @ End address
    BIC    r0, r0, #0x1f                  @ Align start address
loop_loop:
    CMP    r0, r1
    MCRLT  p15, 0x0, r0, c7, c6, 0x1      @ Invalidate cache line using MVA
    ADDLT  r0, r0, #0x20                  @ Add a cache line
    BLT    loop_loop
    LDMFD sp!, {a1-a2, PC}               @ restore registers    
    
MMU_InvalideDCACHEALL:
    STMFD sp!, {a1, lr}               @ save lr_USR and non-callee
tcv_a: 
    MRC p15, 0, r15, c7, c14, 3          @ test clean and invalidate
    BNE tcv_a
    
    MOV     r0, #0
    MCR     p15, 0, r0, c8, c7, 0        @flush i+d-TLBs    

    LDMFD sp!, {a1, PC}               @ restore registers    


MMU_InvalideICACHEALL:
    STMFD   sp!, {a1-a4, lr}            @ save lr_USR and non-callee
    
    MOV     r0, #0    
    MCR     p15, 0, r0, c7, c5, 0       @Invalidate(flush)the ICache
    MCR     p15, 0, r0, c8, c5, 0       @flush ITLB only
    NOP                                 @next few instructions may be via cache.
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

