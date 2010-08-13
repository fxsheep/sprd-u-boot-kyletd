#include <common.h>
#include <asm/io.h>
DECLARE_GLOBAL_DATA_PTR;

int board_init()
{
	gd->bd->bi_arch_number = MACH_TYPE_BIGPHONE;
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;
	return 0;
}

int dram_init(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = get_ram_size((volatile void *)PHYS_SDRAM_1,
			PHYS_SDRAM_1_SIZE);
#if CONFIG_NR_DRAM_BANKS > 1
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size = get_ram_size((volatile void *)PHYS_SDRAM_2,
			PHYS_SDRAM_2_SIZE);
#endif

	return 0;

}
