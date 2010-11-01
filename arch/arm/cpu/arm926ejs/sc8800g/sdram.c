#include <config.h>
#include <asm/arch/regs_global.h>
#include <asm/arch/regs_emc.h>
#include <asm/arch/sdram.h>
#include <asm/io.h>
#include <linux/types.h>
#include <asm/arch/regs_cpc.h>
#include <asm/arch/sdram_cfg.h>
#include <asm/arch/bits.h>
#include <asm/arch/sdram.h>

/********************************************
 * SDRAM pin configuration, include direction
 * driver strength.
 * ******************************************/
static void pin_driver_set(void)
{
	uint32_t i;
	uint32_t reg_config;
	for(i = CPC_EMA0_REG; i<= CPC_EMD15_REG; i+=4)
		writel(0x31, i); //sdram config, direction:output, driver strength:3

	//use SDRAM/DDR, set the bit on
	reg_config = readl(CPC_EMBA1_REG);
	reg_config |= BIT_6;
	writel(reg_config, CPC_EMBA1_REG);
}

/*
static void on_chip_ram_en(void)
{
	REG32(GEN2_ADDR)= ON_CHIP_RAM_EN;
}
*/
//file the sdram_config_info structure
static void get_config(sdram_cfg_t *sdram_config_info)
{
	sdram_config_info->bank_mode = CONFIG_SYS_SDRAM_BANK_CNT;
	sdram_config_info->row_mode = CONFIG_SYS_SDRAM_ROW_CNT;
	sdram_config_info->col_mode = CONFIG_SYS_SDRAM_COL_CNT;
	sdram_config_info->data_width = CONFIG_SYS_SDRAM_DATA_WIDTH;
	sdram_config_info->burst_length = CONFIG_SYS_SDRAM_BURST_LENGTH;
	sdram_config_info->cas_latency = CONFIG_SYS_SDRAM_CAS_LATENCY;
	sdram_config_info->ext_mode_val = CONFIG_SYS_SDRAM_EXT_MODE;
	sdram_config_info->sdram_size = CONFIG_SYS_SDRAM_SIZE_M;
	sdram_config_info->clk_dly = CONFIG_SYS_SDRAM_CLK_DELAY;
}

//fill the sdram_timing_param structure
static void get_timing_param(sdram_timing_t *sdram_timing_param)
{
	sdram_timing_param->row_ref_max = CONFIG_SYS_SDRAM_ROW_REFRESH_MAX;
	sdram_timing_param->row_pre_min = CONFIG_SYS_SDRAM_ROW_PRECHARGE_MIN;
	sdram_timing_param->row_cyc_min = CONFIG_SYS_SDRAM_ROW_CYCLE_MIN;
	sdram_timing_param->rcd_min = CONFIG_SYS_SDRAM_TRCD_MIN;
	sdram_timing_param->wr_min = CONFIG_SYS_SDRAM_TWR_MIN;
	sdram_timing_param->mrd_min = CONFIG_SYS_SDRAM_TMRD_MIN;
	sdram_timing_param->rfc_min = CONFIG_SYS_SDRAM_TRFC_MIN;
	sdram_timing_param->xsr_min = CONFIG_SYS_SDRAM_TXSR_MIN;
	sdram_timing_param->ras_min = CONFIG_SYS_SDRAM_TRAS_MIN;
}

void general_ctl_cfg(sdram_cfg_t *sdram_cfg)
{
	uint32_t reg_config;
	uint32_t rburst_length = sdram_cfg->burst_length;
	uint32_t wburst_length = sdram_cfg->burst_length;
	uint32_t data_width = sdram_cfg->data_width;

	rburst_length &= 0x3;
	wburst_length &= 0x3;

	//Config channel2
	//    //Enable ch2, no switch
	REG32(EXT_MEM_CFG0) |= (BIT_2|BIT_12|BIT_13);

	//Config channel0 big endian
	REG32(EXT_MEM_CFG0) |= BIT_4;

	//Disable all channel eburst_hit_en
	REG32(EXT_MEM_CFG0) &= ~(BIT_20|BIT_21|BIT_22|BIT_23);

	//DMEM enable, (CS0)-->DMEM
	REG32(EXT_MEM_CFG1) |= (BIT_0|BIT_10);    //DMEM enable
	REG32(EXT_MEM_CFG1) &=~BIT_4;	//kewang add for 128M sdram
	REG32(EXT_MEM_CFG1) |=BIT_5;// kewang

	REG32(EXT_MEM_CFG1) &= ~(BIT_12);    //Clear smem_only_en

	REG32(EXT_MEM_CFG1) &= ~(BIT_9);     //SDRAM mode

	if(data_width == DATA_WIDTH_16){
		//Config read/write latency for cs0 here...
		 if(rburst_length == BURST_LEN_8) {
			rburst_length = BURST_LEN_4;
		}else if(rburst_length == BURST_LEN_4){
			rburst_length = BURST_LEN_2;
		}
		if(wburst_length == BURST_LEN_8){
			wburst_length = BURST_LEN_4;
		}else if(wburst_length == BURST_LEN_4){
			wburst_length = BURST_LEN_2;
		}
		
		reg_config = REG32(EXT_MEM_CFG2);
		reg_config &= ~(0x3 | (0x3<<8));
		reg_config |= (rburst_length | (wburst_length<<8));
		REG32(EXT_MEM_CFG2) = reg_config; 
	}else if(data_width == DATA_WIDTH_32){
		//Config read/write latency for cs0 here...
		reg_config = REG32(EXT_MEM_CFG2);
		reg_config &= ~(0x3 | (0x3<<8));
		reg_config |= (rburst_length | (wburst_length<<8));
		REG32(EXT_MEM_CFG2) = reg_config; 
	}else {
		while(1);
	}
}

void dmem_ctl_cfg(uint32_t ahb_clk, const sdram_cfg_t *sdram_cfg_ptr, const sdram_timing_t * sdram_parameters)
{
	uint32_t t_rtw =0;

	uint32_t row_mode = sdram_cfg_ptr->row_mode;
	uint32_t col_mode = sdram_cfg_ptr->col_mode;
	uint32_t data_width = sdram_cfg_ptr->data_width;
	uint32_t cas_latency = sdram_cfg_ptr->cas_latency;
	uint32_t write_latency = 0;

	uint32_t sdram_cycle_ns = 1000/(ahb_clk); //1000000000/(clk*1000000),this value can be set a little bigger.
	uint32_t row_number = 0xFFFFFFFF;
             
	uint32_t t_ras = sdram_parameters->ras_min/sdram_cycle_ns;//sdram_parameters[T_RAS_MIN]/sdram_cycle_ns ;
	uint32_t t_xsr = sdram_parameters->xsr_min/sdram_cycle_ns;
	uint32_t t_rfc = sdram_parameters->rfc_min/sdram_cycle_ns ;
	uint32_t t_mrd = sdram_parameters->mrd_min/sdram_cycle_ns ;
	uint32_t t_wr  = sdram_parameters->wr_min/sdram_cycle_ns ;
	uint32_t t_rcd = sdram_parameters->rcd_min/sdram_cycle_ns ;
	uint32_t t_rp  = sdram_parameters->row_pre_min/sdram_cycle_ns ;
	uint32_t t_ref = sdram_parameters->row_ref_max/sdram_cycle_ns ;    
	uint32_t t_wtr = 0xf;

	if(cas_latency == 3)
	    	t_rtw = 4;
	else if(cas_latency == 2)
		t_rtw = 2;
	else 
		while(1);

	if (row_mode == ROW_MODE_11)
	{
		row_number = 11;
	}
	else if (row_mode == ROW_MODE_12)
	{
		row_number = 12;
	}
	else if (row_mode == ROW_MODE_13)
	{
		row_number = 13;
	}
	else
	{
		row_number = 13;
	}

	if(data_width == DATA_WIDTH_16)
	{
		data_width = 0;
	}else if(data_width == DATA_WIDTH_32)
	{
		data_width = 1;
	}

	REG32(EXT_MEM_DCFG0) = ( \
	                         DCFG0_BKPOS_HADDR_24_23 |       \
				(data_width<<3) |               \
				(row_mode<<4) |                 \
				DCFG0_AUTO_PRE_POSITION_A10 |   \
				(col_mode<<8) |                 \
				DCFG0_CLKDMEM_OUT_EN |          \
				DCFG0_ALTERNATIVE_EN |          \
				DCFG0_ROWHIT_EN |               \
				(t_ref<<20)                     \
		);
		
		REG32(EXT_MEM_DCFG1) = (                        \
				(t_rp <<0) |        \
				(t_rcd<<2) |        \
				(t_wr <<4) |        \
				(t_rfc<<8) |        \
				(t_xsr<<12)|        \
				(t_ras<<16)|        \
				(t_rtw<<20)|        \
				(t_wtr<<24)|        \
				(t_mrd<<28)         \
			);
	REG32(EXT_MEM_DCFG2) = ( (1<<row_number)|DCFG2_REF_CNT_RST );
	//rdm/wdm latency should be cfg relatively...
	 REG32(EXT_MEM_DCFG4) = ( cas_latency | (write_latency<<4) );
	
	REG32(EXT_MEM_CFG1)  &= ~BIT_14;
	REG32(EXT_MEM_DCFG0) |= BIT_14;
	if(cas_latency == 3)
	{
		REG32(EXT_MEM_DCFG4) = 0x00800209;
		REG32(EXT_MEM_DCFG6) = 0x00400100;
	}
	else if (cas_latency == 2)
	{
		REG32(EXT_MEM_DCFG4) = 0x00600007;
		REG32(EXT_MEM_DCFG6) = 0x00100100;
	}
	else 
	{
		while(1);
	}
}

void sdram_device_init(sdram_cfg_t *sdram_cfg_ptr)
{
	uint8_t mode_reg_bl = 0xFF;
	uint8_t mode_reg_bt = MODE_REG_BT_SEQ;   //sequencial mode burst.
	uint8_t mode_reg_cl = 0xFF;
	uint8_t mode_reg_opmode = MODE_REG_OPMODE;
	uint8_t mode_reg_wb = MODE_REG_WB_PRORAM;  //Programming burst length for write.

	uint32_t ex_mode_reg = 0;
	uint16_t mode_reg = 0;
	uint8_t dsoft_cs = 0; // command for CS0

	// since DMEM suport different r/w burst len, how to cfg sdram deviec burst len?
	switch (sdram_cfg_ptr->burst_length)
	{
	case BURST_LEN_2:
		mode_reg_bl = MODE_REG_BL_2;
		break;
	case BURST_LEN_4:
		mode_reg_bl = MODE_REG_BL_4;
		break;
	case BURST_LEN_8:
		mode_reg_bl = MODE_REG_BL_8;
		break;
	default:
		mode_reg_bl = MODE_REG_BL_8;
		break;
	}

	switch (sdram_cfg_ptr->cas_latency)
	{
	case CAS_LATENCY_1:
		mode_reg_cl = MODE_REG_CL_1;
		break;
	case CAS_LATENCY_2:
		mode_reg_cl = MODE_REG_CL_2;
		break;
	case CAS_LATENCY_3:
		mode_reg_cl = MODE_REG_CL_3;
		break;
	default:
		mode_reg_cl = MODE_REG_CL_3;
		break;
	}

	mode_reg = ((mode_reg_wb<<9) | (mode_reg_opmode<<7) 
	       | (mode_reg_cl<<4) | (mode_reg_bt<<3) | mode_reg_bl); 

	ex_mode_reg = sdram_cfg_ptr->ext_mode_val;

	REG32(EXT_MEM_DCFG3) |= BIT_16 | (dsoft_cs<<28);// Precharge all banks.
	while ((REG32(EXT_MEM_DCFG3)) & BIT_16);

	REG32(EXT_MEM_DCFG3) |= BIT_17 | (dsoft_cs<<28);//Auto_ref
	while ((REG32(EXT_MEM_DCFG3)) & BIT_17);

	REG32(EXT_MEM_DCFG3) |= BIT_17 | (dsoft_cs<<28);//Auto_ref
	while ((REG32(EXT_MEM_DCFG3)) & BIT_17);

	//mode register load.
	REG32(EXT_MEM_DCFG3) &= ~(0xFFFF);
	REG32(EXT_MEM_DCFG3) |= (mode_reg | BIT_18 | (dsoft_cs<<28));
	while ((REG32(EXT_MEM_DCFG3)) & BIT_18);

	//extended mode register load.
	if (ex_mode_reg != SDRAM_EXT_MODE_INVALID)	
	{
		REG32(EXT_MEM_DCFG3) &= ~(0xFFFF);
		REG32(EXT_MEM_DCFG3) |= (ex_mode_reg | BIT_18 | (dsoft_cs<<28));
		while ((REG32(EXT_MEM_DCFG3)) & BIT_18);
	}  
}

void emc_init(uint32_t ahb_clk)
{
	uint32_t reg_config;

	sdram_cfg_t sdram_config_info;
	sdram_timing_t sdram_timing_param;

	pin_driver_set();
//	on_chip_ram_en();
	get_config(&sdram_config_info);
	get_timing_param(&sdram_timing_param);

	//disable sdram auto refresh
	reg_config = readl(EXT_MEM_DCFG0);
	reg_config &= ~(DCFG0_AUTOREF_EN);
	writel(reg_config, EXT_MEM_DCFG0);

	general_ctl_cfg(&sdram_config_info);
	dmem_ctl_cfg(ahb_clk, &sdram_config_info, &sdram_timing_param);

	sdram_device_init(&sdram_config_info);

	reg_config = readl(EXT_MEM_DCFG2);
	reg_config |= DCFG2_REF_CNT_RST;
	writel(reg_config, EXT_MEM_DCFG2);

	//enable auto refresh
	reg_config = readl(EXT_MEM_DCFG0);
	reg_config |= DCFG0_AUTOREF_EN;
	writel(reg_config, EXT_MEM_DCFG0);

}
