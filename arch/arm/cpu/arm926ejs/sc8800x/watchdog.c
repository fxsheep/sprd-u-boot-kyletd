#include <config.h>
#include <asm/io.h>
#include <asm/arch/regs_global.h>
#include <asm/arch/bits.h>
#include <linux/types.h>

void enable_watchdog(void)
{

}

void disable_watchdog(void)
{
	uint32_t reg_config;
	
	//Enable watchdog programming
	reg_config = readl(GR_GEN0);
	reg_config |= GEN0_WDG_EN;
	writel(reg_config, GR_GEN0);

	//Unlock watchdog load register
	writel(WDG_UNLOCK_CODE, WDG_LOCK);

	//Disable watchdog timer clock and interrupt mode
	reg_config = readl(WDG_CTL);
	reg_config &= ~(BIT_1 | BIT_0);
	writel(reg_config, WDG_CTL);

	//Lock watchdog load register
	write(0x12345678, WDG_LOCK);

	//disable watchdog programming
	reg_config = readl(GR_GEN0);
	reg_config &= ~GEN0_WDG_EN;
	writel(reg_config, GR_GEN0);
}

void init_watchdog(void)
{

}

void feed_watchdog(void)
{
}
