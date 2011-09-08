#include <common.h>
#include <asm/io.h>

#include <asm/arch/regs_adi.h>
#include <asm/arch/adi_hal_internal.h>
#include <asm/arch-sc8800g/analog_reg_v3.h>
void CHG_TurnOn (void)
{
    ANA_REG_AND (ANA_CHGR_CTL0,~CHGR_PD_BIT);
}

void CHG_ShutDown (void)
{
    ANA_REG_OR (ANA_CHGR_CTL0,CHGR_PD_BIT);
}

