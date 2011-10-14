#include <asm/arch/gpio.h>
#include <asm/arch/mfp.h>

extern int sprd_gpio_request(struct gpio_chip *chip, unsigned offset);
extern void sprd_gpio_init(void);
static unsigned long pwr_gpio_cfg =
    MFP_ANA_CFG_X(PBINT, AF0, DS1, F_PULL_UP,S_PULL_UP, IO_IE);


static unsigned long chg_gpio_cfg =
    MFP_ANA_CFG_X(CHIP_RSTN, AF0, DS1, F_PULL_UP,S_PULL_UP, IO_IE);


int board_gpio_init(void)
{
	sprd_gpio_init();
	// config charger pin
	sprd_mfp_config(&chg_gpio_cfg, 1);
	sprd_gpio_request(NULL, CHG_GPIO_NUM);
	sprd_gpio_direction_input(NULL,CHG_GPIO_NUM); 

	// config power button 
	sprd_mfp_config(&pwr_gpio_cfg, 1);
	sprd_gpio_request(NULL, POWER_BUTTON_GPIO_NUM);
	sprd_gpio_direction_input(NULL,POWER_BUTTON_GPIO_NUM);
	return 0;
}
