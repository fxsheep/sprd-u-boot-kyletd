#include <common.h>
#include <asm/io.h>
DECLARE_GLOBAL_DATA_PTR;

extern void sprd_gpio_init(void);
extern void ADI_init (void);
extern int LDO_Init(void);

#include <asm/arch/regs_ana.h>
#include <asm/arch/adi_hal_internal.h>

#define PIN_CTL_REG 0x8C000000
static void chip_init(void)
{
    //ANA_REG_SET(ANA_ADIE_CHIP_ID,0);
    /* setup pins configration when LDO shutdown*/
    //__raw_writel(0x1fff00, PIN_CTL_REG);
     *(volatile unsigned int *)PIN_CTL_REG = 0x1fff00;
}
int board_init()
{
	gd->bd->bi_arch_number = MACH_TYPE_OPENPHONE;
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

    ADI_init();
    chip_init();
    LDO_Init();
    sprd_gpio_init();
//    board_gpio_init();
	return 0;
}

int dram_init(void)
{
	gd->ram_size = get_ram_size((volatile void *)PHYS_SDRAM_1,
			PHYS_SDRAM_1_SIZE);
	return 0;
}
