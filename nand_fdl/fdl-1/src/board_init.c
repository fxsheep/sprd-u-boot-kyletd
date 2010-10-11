#include "board_init.h"

#define REG(i) 		(*(volatile unsigned int*)(i))

void board_init(void)
{
	unsigned int i;
	for(i=0; i<41; ++i)
       REG(0x8c0001c8+i*4) = 0x0A;
   	
   REG(0x8c000274) = 0x0A;     /* EMCSN0_reg */
   REG(0x8c000284) = 0x0A;     /* EMOE_reg */
   REG(0x8c000288) = 0x0A;     /* EMWE_reg */
}

