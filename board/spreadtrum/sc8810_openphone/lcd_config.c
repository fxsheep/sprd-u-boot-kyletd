#include <asm/arch/sc8810_lcd.h>
#include <asm/arch/adi_hal_internal.h>
#include <asm/arch/analog_reg_v3.h>

#define WHTLED_CTL              ANA_LED_CTL
#define WHTLED_PD_SET           BIT_0
#define WHTLED_PD_RST           BIT_1
#define WHTLED_V_SHIFT          2
#define WHTLED_V_MSK            (0x1F << WHTLED_V_SHIFT)

void set_backlight(uint32_t value)
{
	ANA_REG_AND(WHTLED_CTL, ~(WHTLED_PD_SET | WHTLED_PD_RST));
	ANA_REG_OR(WHTLED_CTL,  WHTLED_PD_RST);
	ANA_REG_MSK_OR (WHTLED_CTL, ( (value << WHTLED_V_SHIFT) &WHTLED_V_MSK), WHTLED_V_MSK);
}
void FB_LDO_TurnOnLDO()
{

}
