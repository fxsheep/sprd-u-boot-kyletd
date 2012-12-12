#include <asm/arch/sc8810_lcd.h>
#include <asm/io.h>
#include <asm/arch/ldo.h>

static void __raw_bits_or(unsigned int v, unsigned int a)
{
        __raw_writel((__raw_readl(a) | v), a);
}
void set_backlight(uint32_t value)
{
    __raw_bits_or((1<<5),  0x8B000008);
    __raw_bits_or((1<<10), 0x8A000384);
    __raw_bits_or((1<<10), 0x8A000388);
    __raw_bits_or((1<<10), 0x8A000380);
}
void FB_LDO_TurnOnLDO()
{
    LDO_SetVoltLevel(LDO_LDO_SIM3, LDO_VOLT_LEVEL1);
    LDO_TurnOnLDO(LDO_LDO_SIM3);
    LDO_SetVoltLevel(LDO_LDO_VDD28, LDO_VOLT_LEVEL3);
    LDO_TurnOnLDO(LDO_LDO_VDD28);
}
