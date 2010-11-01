#include <config.h>
#include <asm/arch/bits.h>
#include <asm/arch/regs_global.h>
#include <linux/types.h>

#define MEM_REMAP_REG 0x20900218
void mem_remap(void)
{
	REG32(MEM_REMAP_REG)=0;
}
