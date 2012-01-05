#include <config.h>
#include <asm/io.h>
#include <asm/arch/chip_drv_config_extern.h>
#include <asm/arch/bits.h>
#include <linux/types.h>
#include <asm/arch/regs_adi.h>
#include <asm/arch/adi_hal_internal.h>
#define VIBRATOR_REG_UNLOCK (0xA1B2)
#define VIBRATOR_REG_LOCK   (~VIBRATOR_REG_UNLOCK)
#define VIBRATOR_STABLE_LEVEL   (4)
#define VIBRATOR_INIT_LEVEL (11)    //init level must larger than stable level
#define VIBRATOR_INIT_STATE_CNT (2)
#define mdelay(_ms) udelay(_ms*1000)
void set_vibrator(int on)
{
	int i = 0;
	ANA_REG_SET(ANA_VIBR_WR_PROT, VIBRATOR_REG_UNLOCK); //unlock vibrator registor
	if(on == 0){
		ANA_REG_AND(ANA_VIBRATOR_CTL0, ~(VIBR_PD_SET | VIBR_PD_RST));
		ANA_REG_OR(ANA_VIBRATOR_CTL0, VIBR_PD_SET);
	}else{
		ANA_REG_AND(ANA_VIBRATOR_CTL0, ~(VIBR_PD_SET | VIBR_PD_RST));
		ANA_REG_OR(ANA_VIBRATOR_CTL0, VIBR_PD_RST);
	}
	ANA_REG_SET(ANA_VIBR_WR_PROT, VIBRATOR_REG_LOCK);   //lock vibrator registor
}
void vibrator_hw_init(void)
{
	ANA_REG_SET(ANA_VIBR_WR_PROT, VIBRATOR_REG_UNLOCK); //unlock vibrator registor

	ANA_REG_OR(ANA_VIBRATOR_CTL0, VIBR_RTC_EN);
	ANA_REG_AND(ANA_VIBRATOR_CTL0, ~VIBR_BP_EN);   //enable new version,so VIBR_V_BP is disable.

	ANA_REG_MSK_OR(ANA_VIBRATOR_CTL0, (VIBRATOR_INIT_LEVEL << VIBR_INIT_V_SHIFT), VIBR_INIT_V_MSK); //set init current level
	ANA_REG_MSK_OR(ANA_VIBRATOR_CTL0, (VIBRATOR_STABLE_LEVEL << VIBR_STABLE_V_SHIFT), VIBR_STABLE_V_MSK); //set stable current level
	ANA_REG_SET(ANA_VIBRATOR_CTL1, VIBRATOR_INIT_STATE_CNT);   //set convert count

	ANA_REG_SET(ANA_VIBR_WR_PROT, VIBRATOR_REG_LOCK);   //lock vibrator registor
}


