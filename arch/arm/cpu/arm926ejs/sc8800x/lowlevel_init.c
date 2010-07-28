#include <linux/types.h>
void lowlevel_init(void)
{
	uint32_t ahb_clk;
	ahb_clk = config_clk();
	emc_init(ahb_clk);
}
