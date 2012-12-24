#include "asm/arch/sci_types.h"
#include <asm/io.h>
#ifndef GPIO_BASE
#define 	GPIO_BASE 0x8A000000	//Sunny, for SC8810
#endif

static void __raw_bits_and(unsigned int v, unsigned int a)
{
	__raw_writel((__raw_readl(a) & v), a);
}

static void __raw_bits_or(unsigned int v, unsigned int a)
{
	  __raw_writel((__raw_readl(a) | v), a);
}

void gpio_direction_output(int PinNo, int OutIn)	//Sunny only for uBoot gpio ops
{
	int addr, maskbit, n;
	if(PinNo >=192){
		addr = 0x820004C0;
	}else if(PinNo >=176){
		addr = 0x82000480;
	}else{
		n = (PinNo - 16)/16;
		addr =GPIO_BASE +  0x0080 * n;
	}
	maskbit = PinNo%16;

	__raw_bits_or((1<<5),  0x8B000008);	//GPIO register Enable
	__raw_bits_or((1<<maskbit), addr + 4);	//Mask
	if(OutIn)
		__raw_bits_or((1<<maskbit), addr + 8);	//Dir out
	else	__raw_bits_and(~(1<<maskbit), addr+ 8);	//Dir in
}

void gpio_set_value(int PinNo, int Level)	//Sunny only for uBoot gpio ops
{
	int addr, maskbit, n;
	if(PinNo >=192){
		addr = 0x820004C0;
	}else if(PinNo >=176){
		addr = 0x82000480;
	}else{
		n = (PinNo - 16)/16;
		addr =GPIO_BASE +  0x0080 * n;
	}
	maskbit = PinNo%16;

	__raw_bits_or((1<<maskbit), addr + 4);	//Mask
	if(Level)
		__raw_bits_or((1<<maskbit), addr);	//High
	else	__raw_bits_and(~(1<<maskbit), addr);	//Low
}

int gpio_get_value(int PinNo)	//Sunny only for uBoot gpio ops
{
	int addr, maskbit, n;
	uint32 ret;
	if(PinNo >=192){
		addr = 0x820004C0;
	}else if(PinNo >=176){
		addr = 0x82000480;
	}else{
		n = (PinNo - 16)/16;
		addr =GPIO_BASE +  0x0080 * n;
	}
	maskbit = PinNo%16;

	ret = __raw_readl(addr);

	if(ret & (1<<maskbit))
		return 1;
	else	return 0;
}

