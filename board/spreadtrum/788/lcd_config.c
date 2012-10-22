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
	LCD_SetBackLightBrightness(value);

}
void FB_LDO_TurnOnLDO()
{

}
