#include <config.h>
#include <asm/io.h>
#include <asm/arch/regs_global.h>
#include <asm/arch/bits.h>
#include <linux/types.h>
#include <asm/arch/regs_wdg.h>

void start_watchdog(uint32_t init_time_ms)
{
}

void stop_watchdog(void)
{
	
	//Enable watchdog programming
	REG32(GR_GEN0) |=GEN0_WDG_EN;
	REG32(GR_CLK_EN) &= ~0x04;
	//Unlock watchdog load rigiter
	REG32(WDG_LOCK) = 0x1ACCE551;
	//clear watchdog interrupt
	REG32(WDG_INT_CLR) = 1;

	//Disable watchdog timer clock and interrupt mode
	REG32(WDG_CTL) &= ~(BIT_1  | BIT_0);
	//lOCK WATCH DOG 
	REG32(WDG_LOCK) = 0x12345678;
	//Disable watchdog programming
	REG32(GR_GEN0) &= ~GEN0_WDG_EN;
}

void init_watchdog(void)
{

}

void feed_watchdog(void)
{
}

void load_watchdog(uint32_t time_ms)
{
}
void hw_watchdog_reset(void)
{
}
