#include <config.h>
#include <asm/io.h>
#include <asm/arch/bits.h>
#include <asm/arch/regs_global.h>
#include <asm/arch/regs_ahb.h>
#include <linux/types.h>

/*************************************************
 * determine the clock input, by the strap pin
 * the strap bit shows the crytal input clock
 * 1 : 26Mhz
 * 0 : 13Mhz
 *
 * return value is frequency in Mhz, 26 or 13
 * ***********************************************/
uint32_t get_input_clk(void)
{
	uint32_t reg_content;
	reg_content = readl(GR_GEN1);
	
	if(reg_content & GEN1_CLK_26MHZ_EN){
		return 26;
	}else {
		return 13;
	}
}

/****************************************************************
 * calculate the greatest common divisor for two input integer
 * the two input value should be non-zero integer
 *
 * return is the greatest common divisor
 * 0 is failed
 * *************************************************************/
uint32_t cal_g_divisor(uint32_t val_one, uint32_t val_two)
{
	if(val_one == 0 || val_two == 0)
		return 0;

	uint32_t div;
	
	uint32_t big_val, sml_val;
	
	if(val_one >= val_two) {
		big_val = val_one;
		sml_val = val_two;
	}else{
		big_val = val_two;
		sml_val = val_one;
	}

	div = big_val%sml_val;
	while(div != 0) {
		big_val = sml_val;
		sml_val = div;
		div = big_val%sml_val;
	}
	return sml_val;
}
/*************************************************
 * config pll, AHB, AMR,EMC clock
 * the config value is get from board specific
 * config file
 *
 * return is ahb clock for further configuration
 * ************************************************/
uint32_t config_clk(void)
{
	uint32_t input_clk, ahb_clk, pll_clk, ahb_div, arm_div, emc_div, xahb_div;
	uint32_t pll_div_m, pll_div_n;
	uint32_t reg_config;
	uint32_t great_divisor;
	int i;

	// get the input clock
	input_clk = get_input_clk();	
	pll_clk = CONFIG_SYS_PLL_MHZ;
	if(pll_clk <= 0)
		pll_clk = input_clk;  // cpu run frequency should not below input clk

	//calculate the divisor
	great_divisor = cal_g_divisor(pll_clk, input_clk);
	if(great_divisor <= 0)
		return 0;

	pll_div_m = input_clk / great_divisor;
	if(pll_div_m <= 0)
		pll_div_m = 1;
	else if(pll_div_m > PLLMN_M_MAX)
		pll_div_m = PLLMN_M_MAX;

	pll_div_n = pll_clk / great_divisor;
	if(pll_div_n > PLLMN_N_MAX){
		pll_div_n = PLLMN_N_MAX;
		pll_div_m = (input_clk * pll_div_n)/pll_clk;
	}else if(pll_div_n <= 0)
		pll_div_n = 1;
	
	reg_config = readl(GR_GEN1);  //enable mpll write
	reg_config |= GEN1_MPLLMN_WN;
	writel(reg_config,GR_GEN1);
	reg_config = (pll_div_m <<PLLMN_M_SHIFT) | (pll_div_n << PLLMN_N_SHIFT);
	writel(reg_config, GR_MPLL_MN); //config mpll register
	reg_config = readl(GR_GEN1);   //disable mpll write
	reg_config &= ~(GEN1_MPLLMN_WN);
	writel(reg_config, GR_GEN1);
	//delay some time
	for(i = 0; i<100; i++);

	ahb_div = CONFIG_SYS_AHB_DIV;
	if(ahb_div <= 0)
		ahb_div = 1;
	else if(ahb_div > AHB_CLK_DIV_MAX)
		ahb_div = AHB_CLK_DIV_MAX;

	arm_div = CONFIG_SYS_ARM_DIV;
	if(arm_div <= 0)
		arm_div = 1;
	else if (arm_div > ARM_CLK_DIV_MAX)
		arm_div = ARM_CLK_DIV_MAX;

	emc_div = CONFIG_SYS_EMC_DIV;
	if(emc_div <= 0)
		emc_div = 1;
	else if (emc_div > EMC_CLK_DIV_MAX)
		emc_div = EMC_CLK_DIV_MAX;

	xahb_div = ahb_div/arm_div;

	//config AHB ARM CLK register
	reg_config = ((ahb_div-1) << AHB_CLK_DIV_SHIFT) | ((arm_div-1) << ARM_CLK_DIV_SHIFT) | ((emc_div-1) << EMC_CLK_DIV_SHIFT) | ((xahb_div-1) << XAHB_CLK_DIV_SHIFT);
	writel(reg_config, AHB_AHB_ARM_CLK);

	//delay some time
	for(i=0; i<100; i++);

	ahb_clk = pll_clk/ahb_div;
	return ahb_clk;
}
