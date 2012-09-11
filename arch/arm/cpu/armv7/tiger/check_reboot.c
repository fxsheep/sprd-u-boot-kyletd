#include <asm/arch/sci_types.h>
#include <asm/arch/sc_reg.h>
#include <asm/arch/adi_hal_internal.h>
#include <boot_mode.h>
#include <asm/arch/gpio.h>
#include <asm/arch/asm_generic_gpio.h>
#include <asm/arch/gpio_phy.h>
#include <asm/arch/rtc_reg_v3.h>
#include <asm/arch/mfp.h>

unsigned check_reboot_mode(void)
{
	unsigned rst_mode= 0;

	rst_mode = ANA_REG_GET(ANA_HWRST_STATUS);
	rst_mode &= HWRST_STATUS_POWERON_MASK;
	ANA_REG_SET(ANA_HWRST_STATUS, 0); //clear flag
	if(rst_mode == HWRST_STATUS_RECOVERY)
		return RECOVERY_MODE;
	else if(rst_mode == HWRST_STATUS_FASTBOOT)
		return FASTBOOT_MODE;
	else if(rst_mode == HWRST_STATUS_NORMAL)
		return NORMAL_MODE;
	else if(rst_mode == HWRST_STATUS_ALARM)
		return ALARM_MODE;
	else if(rst_mode == HWRST_STATUS_SLEEP)
		return SLEEP_MODE;
	else
		return 0;
}

void reboot_devices(unsigned reboot_mode)
{
    unsigned rst_mode = 0;
    if(reboot_mode == RECOVERY_MODE){
      rst_mode = HWRST_STATUS_RECOVERY;
    }
    else if(reboot_mode == FASTBOOT_MODE){
      rst_mode = HWRST_STATUS_FASTBOOT;
    }else if(reboot_mode == NORMAL_MODE){
      rst_mode = HWRST_STATUS_NORMAL;
    }else{
      rst_mode = 0;
    }

    ANA_REG_SET(ANA_HWRST_STATUS, rst_mode);
    reset_cpu(0);
#if 0
    asm volatile("ldr r1,=0x20900218"); //remap ROM to 0x0000_0000
    asm volatile("ldr r2,=1");
    asm volatile("str r2,[r1]");
    asm volatile("mov pc,#0");
#endif
}
void power_down_devices(unsigned pd_cmd)
{
    power_down_cpu(0);
}

#define POWER_BUTTON_GPIO_NUM 163
extern int sprd_gpio_get( struct gpio_chip * chip, unsigned offset);
extern int sprd_gpio_request(struct gpio_chip *chip, unsigned offset);
extern void sprd_gpio_init(void);
static unsigned long pwr_gpio_cfg =
    MFP_ANA_CFG_X(PBINT, AF0, DS1, F_PULL_UP,S_PULL_UP, IO_IE);
int power_button_pressed(void)
{
#if 0
    struct gpio_chip power_button_chip;
    sprd_gpio_init();
    sprd_mfp_config(&pwr_gpio_cfg, 1);
    sprd_gpio_request(&power_button_chip, POWER_BUTTON_GPIO_NUM);
    sprd_gpio_direction_input(&power_button_chip,POWER_BUTTON_GPIO_NUM); 
    return sprd_gpio_get(&power_button_chip, POWER_BUTTON_GPIO_NUM);
#else
	ANA_REG_OR(ANA_APB_CLK_EN, BIT_3|BIT_11);
	ANA_REG_SET(ADI_EIC_MASK, 0xff);
	udelay(3000);
	int status = ANA_REG_GET(ADI_EIC_DATA);
	//printf("eica status %x\n", status);
	return !!(status & (1 << 3)/*PBINT*/);//low level if pb hold
#endif
}

#define CHG_GPIO_NUM 162
static unsigned long chg_gpio_cfg =
    MFP_ANA_CFG_X(CHIP_RSTN, AF0, DS1, F_PULL_UP,S_PULL_UP, IO_IE);
    //MFP_ANA_CFG_X(CHIP_RSTN, AF0, DS1, F_PULL_DOWN,S_PULL_UP, IO_IE);
int charger_connected(void)
{
 #if  0	//mingwei
    struct gpio_chip chg_chip;
    sprd_mfp_config(&chg_gpio_cfg, 1);
    sprd_gpio_request(&chg_chip, CHG_GPIO_NUM);
    sprd_gpio_direction_input(&chg_chip,CHG_GPIO_NUM); 
    return sprd_gpio_get(&chg_chip, CHG_GPIO_NUM);
#else
	ANA_REG_OR(ANA_APB_CLK_EN, BIT_3|BIT_11);
	ANA_REG_SET(ADI_EIC_MASK, 0xff);
	udelay(3000);
	int status = ANA_REG_GET(ADI_EIC_DATA);
	//printf("charger_connected eica status %x\n", status);
	return !!(status & (1 << 2));
#endif
    
}

int alarm_triggered(void)
{
    printf("ANA_RTC_INT_RSTS is 0x%x\n", ANA_RTC_INT_RSTS);
    printf("value of it 0x%x\n", ANA_REG_GET(ANA_RTC_INT_RSTS));
    return ANA_REG_GET(ANA_RTC_INT_RSTS) & BIT_4;
}
