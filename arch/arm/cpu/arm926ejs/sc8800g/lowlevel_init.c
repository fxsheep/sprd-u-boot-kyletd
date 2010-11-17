#include <linux/types.h>
#include <asm/arch/migrate.h>
#include <asm/arch/sdram_init.h>

void lowlevel_init(void)
{
	
	Chip_Init();
}
/*
 *  * Called in case of an exception.
 *   */
void hang(void)
{
	/* Loop forever */
	while (1) ;
}

