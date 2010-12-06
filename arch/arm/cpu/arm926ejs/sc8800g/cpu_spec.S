#include <config.h>
#include <common.h>
#include <version.h>

#ifdef PLATFORM_SC8800G
#define REMAP 0x20900218
#define SVC_STACK_TEMP 0x40008000
#define GEN2_ADDR 0x8b00002c
#define ON_CHIP_RAM_EN 0xF
#endif

#ifdef PLATFORM_SC8800G
#define BIGEND_PROT_REG 0x20900290
#define AHB_CTRL5_REG 0x20900230
#endif

.globl cpu_spec
cpu_spec:
	LDR    R0, =REMAP
	LDR    R1, =0
	STR    R1, [R0]

	MRC p15,0,r0,c1,c0,0
#ifdef CHIP_ENDIAN_BIG
	ORR r0,r0,#0x80
#else
	BIC r0,r0,#0x80
#endif

#ifdef _LITTLE_ENDIAN
	BIC r0,r0,#1 /*disable MMU*/
	LDR r1,=0x1004
	BIC r0,r0,r1  /*disable cache*/
	LDR r1,=0x1000
	ORR r0,r0,r1
#endif
	MCR p15,0,r0,c1,c0,0

	/*Set Endian Regs of SC8800G*/
#if defined(CHIP_ENDIAN_BIG) && defined(PLATFORM_SC8800G)
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
	MOV    R10, #0

#if defined(PLATFORM_SC8800H) || defined(PLATFORM_SC8800G)
#else
	/*Enable on chip ram for ARM*/
	LDR     R0, =GEN2_ADDR
	LDR     R1, =ON_CHIP_RAM_EN
	LDR     R2, [R0]
	ORR     R1, R1, R2
	STR     R1, [R0]
#endif

	/*set up temp stack*/
	LDR    sp, =SVC_STACK_TEMP

	STMDB sp!,{lr} 
	bl lowlevel_init
	@bl MMU_Init
	/*Re-set up stack
	  The sp here must be in the reserved region
	  */
	LDMIA sp!, {lr}
	LDR     sp, =0x008f0000 

	mov	pc,lr
/*end of function cpu_spec*/
