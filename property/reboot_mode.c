#include <config.h>
#include <common.h>
#include <linux/types.h>
#include <asm/arch/bits.h>

#include <asm/arch/cmddef.h>
#include <boot_mode.h>
#include <asm/arch/sci_types.h>
#include <asm/arch/boot_drvapi.h>
#include <asm/arch/mocor_boot_mode.h>

static void ResetMCU(void)
{
    /* set watchdog reset flag */
    CHIP_PHY_ResetHWFlag(0x1FFF);
    CHIP_PHY_SetWDGHWFlag(TYPE_RESET, AUTO_TEST_MODE);
    /* reset the system via watchdog timeout */
    CHIP_ResetMCU();

    while (1);
}

void reboot_func(void)
{
	/* reboot example only for amazing */
	printf("\n--------%s %d-------\n", __FUNCTION__, __LINE__);
	ResetMCU();
}


