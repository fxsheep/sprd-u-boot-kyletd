#include <config.h>
#include <asm/io.h>
#include <asm/arch/chip_drv_config_extern.h>
#include <asm/arch/bits.h>
#include <linux/types.h>
#include <asm/arch/regs_adi.h>
#include <asm/arch/adi_hal_internal.h>
#define VIBRATOR_LEVEL (3)
void set_vibrator(int on)
{
    if(on == 0){
        ANA_REG_AND(VIBR_CTL, ~(VIBR_PD_SET | VIBR_PD_RST));
        ANA_REG_OR(VIBR_CTL, VIBR_PD_SET);
    }else{
        ANA_REG_AND(VIBR_CTL, ~(VIBR_PD_SET | VIBR_PD_RST));
        ANA_REG_OR(VIBR_CTL, (VIBRATOR_LEVEL << VIBR_V_SHIFT) & VIBR_V_MSK);
        ANA_REG_OR(VIBR_CTL, VIBR_PD_RST);
    }
}

