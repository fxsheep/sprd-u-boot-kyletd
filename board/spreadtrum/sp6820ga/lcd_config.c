#include <asm/arch/sc8810_lcd.h>
#include <asm/io.h>

extern void LCD_SetBackLightBrightness( unsigned long  value);
static void __raw_bits_and(unsigned int v, unsigned int a)
{
        __raw_writel((__raw_readl(a) & v), a);

}

static void __raw_bits_or(unsigned int v, unsigned int a)
{
        __raw_writel((__raw_readl(a) | v), a);
}
void set_backlight(uint32_t value)
{
#ifndef CONFIG_SP8810EA
	LCD_SetBackLightBrightness(value);
#else
    __raw_writel(0x101, 0x8C0003e0);
    __raw_bits_or((1<<5),  0x8B000008);
    __raw_bits_or((1<<15), 0x8A000384);
    __raw_bits_or((1<<15), 0x8A000388);
    __raw_bits_or((1<<15), 0x8A000380);
#endif
}
void FB_LDO_TurnOnLDO()
{

}
