#include "sci_types.h"
#include "sc_fdl_reg.h"
#include "sdram.h"
#include "sdram_cfg.h"
#include "cmd_def.h"
#include "packet.h"

#if !defined (NAND_FDL_SC6800D)
#define EXTMEMCTL0 0x20000000
#define EXTMEMINI  0x20000020
//#define EXTMSCTL   0x20000024
#define CLK_DLY    0x8b00005c
#define SDRAM_EXT_INVALID     0xffffffff
#endif

#if defined (NAND_FDL_SC6800D)
/*
PLL_CLK_MAIN = 26M * MPLL_MN = 26 * (0x48/0xd) = 144 MHz;
ARM_CLK = ((Clk_arm_div_n+1)/(Clk_arm_div_m+1)) * PLL_CLK_MAIN = 144 * (1/5) = 28.8 MHz;
AHB_CLK = ((Clk_ahb_div_n+1)/(Clk_ahb_div_m+1)) * PLL_CLK_MAIN = 144 * (1/10) = 14.4 MHz;

note 1:
	ARM_CLK should not be smaller than AHB_CLK, and should be several times of(or 
	equal to) AHB_CLK,
note 2:
	(***_n+1)/(***_m+1) should be 1/* or 2/odd.
*/
void SDRAM_Clk_Dly_Cfg(uint32 dmem_clk,uint32 dclk_out_dly,uint32 dclk_in_dly)
{
    uint32 gen1,ahb_arm_clk_div;
    
    dclk_out_dly &= (0x1F);
    dclk_in_dly &= (0x1F);
    
    gen1 = *(volatile uint32 *)GR_GEN1;
    
    ahb_arm_clk_div = *(volatile uint32 *)AHB_AHB_ARM_CLK;
    
    switch (dmem_clk)
    {
    case 50:
    	if (gen1 & BIT_15)   // if 26MHz external crystal.
    	{
    		*(volatile uint32 *)GR_GEN1 |= BIT_9;	
    		*(volatile uint32 *)GR_MPLL_MN = 0xC8001A;      //mpll = 200 MHz
    		*(volatile uint32 *)GR_GEN1 &= ~BIT_9;
    		
    		ahb_arm_clk_div = 0x0301;      //arm_clk 2 div,ahb_clk 4 div.
    		*(volatile uint32 *)AHB_AHB_ARM_CLK = ahb_arm_clk_div;
    	}
    	else
    	{
    		*(volatile uint32 *)GR_GEN1 |= BIT_9;
    		*(volatile uint32 *)GR_MPLL_MN = 0xC8000D;      //mpll = 200 MHz
    		*(volatile uint32 *)GR_GEN1 &= ~BIT_9;
    		
    		ahb_arm_clk_div = 0x0301;      //arm_clk 2 div,ahb_clk 4 div.
    		*(volatile uint32 *)AHB_AHB_ARM_CLK = ahb_arm_clk_div;
    	}
    	*(volatile uint32 *)EXT_MEM_CFG1 = (dclk_out_dly) | (0x10 << 5) | (dclk_in_dly << 10) | (0x10 << 15) | (0x7 << 20);
    	break;
    	
    case 80:
    	if (gen1 & BIT_15)   // if 26MHz external crystal.
    	{
    		*(volatile uint32 *)GR_GEN1 |= BIT_9;	
    		*(volatile uint32 *)GR_MPLL_MN = 0xA0001A;      //mpll = 160 MHz
    		*(volatile uint32 *)GR_GEN1 &= ~BIT_9;
    		
    		ahb_arm_clk_div = 0x0100;      //arm_clk 1 div,ahb_clk 2 div.
    		*(volatile uint32 *)AHB_AHB_ARM_CLK = ahb_arm_clk_div;
    	}
    	else
    	{
    	    *(volatile uint32 *)GR_GEN1 |= BIT_9;
    		*(volatile uint32 *)GR_MPLL_MN = 0xA0000D;      //mpll = 160 MHz
    		*(volatile uint32 *)GR_GEN1 &= ~BIT_9;
    		
    		ahb_arm_clk_div = 0x0100;      //arm_clk 1 div,ahb_clk 2 div.
    		*(volatile uint32 *)AHB_AHB_ARM_CLK = ahb_arm_clk_div;
    	}
    	*(volatile uint32 *)EXT_MEM_CFG1 = (dclk_out_dly) | (0x10 << 5) | (dclk_in_dly << 10) | (0x10 << 15) | (0x7 << 20);
    	break;

    case 100:
        if (gen1 & BIT_15)   // if 26MHz external crystal.
    	{
    		*(volatile uint32 *)GR_GEN1 |= BIT_9;	
    		*(volatile uint32 *)GR_MPLL_MN = 0xC8001A;      //mpll = 200 MHz
    		*(volatile uint32 *)GR_GEN1 &= ~BIT_9;
    		
    		ahb_arm_clk_div = 0x0101;      //arm_clk 2 div,ahb_clk 2 div.
    		*(volatile uint32 *)AHB_AHB_ARM_CLK = ahb_arm_clk_div;
    	}
    	else
    	{
    		*(volatile uint32 *)GR_GEN1 |= BIT_9;
    		*(volatile uint32 *)GR_MPLL_MN = 0xC8000D;      //mpll = 200 MHz
    		*(volatile uint32 *)GR_GEN1 &= ~BIT_9;
    		
    		ahb_arm_clk_div = 0x0101;      //arm_clk 2 div,ahb_clk 2 div.
    		*(volatile uint32 *)AHB_AHB_ARM_CLK = ahb_arm_clk_div;
    	}

	    *(volatile uint32 *)EXT_MEM_CFG1 = (dclk_out_dly) | (0x10 << 5) | (dclk_in_dly << 10) | (0x10 << 15) | (0x7 << 20);

    	break;
    default:
    	break;
    }
    
    //  *(volatile uint32 *)EMC_DDLY0 = 0;
	//	*(volatile uint32 *)EMC_DDLY1 = 0;

}
    
void SDRAM_GenMemCtlCfg(void)
{
	uint32 temp;
	
	temp = *(volatile uint32 *)EXT_MEM_CFG0;
	temp |= BIT_8;   //CS0 select SDRAM. ******
	*(volatile uint32 *)EXT_MEM_CFG0 = temp;
}

void SDRAM_DMemCtlCfg(uint32 clk,SDRAM_CFG_INFO_T * sdram_cfg_ptr)
{
	uint32 bank_mode = sdram_cfg_ptr->bank_mode;
	uint32 row_mode = sdram_cfg_ptr->row_mode;
	uint32 col_mode = sdram_cfg_ptr->col_mode;
	uint32 data_width = sdram_cfg_ptr->data_width;
	uint32 burst_length = sdram_cfg_ptr->burst_length;
	uint32 cas_latency = sdram_cfg_ptr->cas_latency;
	SDRAM_TIMING_PARA_T_PTR  sdram_timing_ptr = SDRAM_GetTimingPara();
	
    uint32 sdram_cycle_ns = 1000/(clk);//1000000000/(clk*1000000),this value can be set a little bigger.
	uint32 row_number = 0xFFFFFFFF;
	uint32 read_latency = DCFG0_RL_4;
	uint32 t_ref_interval = 7800; // uint: ns
	
	if (cas_latency == CAS_LATENCY_1)
	{
		read_latency = DCFG0_RL_3;
	}
	else if (cas_latency == CAS_LATENCY_2)
	{
		read_latency = DCFG0_RL_4;
	}
	else if (cas_latency == CAS_LATENCY_3)
	{
		read_latency = DCFG0_RL_5;
	}
	else
	{
		read_latency = DCFG0_RL_5; // ???
	}
	
	if (row_mode == ROW_MODE_11)
	{
		row_number = 11;
		t_ref_interval = 31200;
	}
	else if (row_mode == ROW_MODE_12)
	{
		row_number = 12;
		t_ref_interval = 15600;
	}
	else if (row_mode == ROW_MODE_13)
	{
		row_number = 13;
		t_ref_interval = 7800;
	}
	else
	{
		row_number = 13;
		t_ref_interval = 7800;
	}
	
	
	*(volatile uint32 *)EXT_MEM_DCFG0 = ( DCFG0_DQM_MODE_W0R1 | DCFG0_DQM_TERM_EN 
									| DCFG0_DQM_FORCE_HIGH | DCFG0_BKPOS_HADDR23
									| (bank_mode << 8) | (row_mode << 10) | (col_mode << 12) | (data_width << 15) 
									| (burst_length << 16) | read_latency | DCFG0_T_RW_0
									| DCFG0_ALTERNATIVE_EN | DCFG0_ROWHIT_EN);
								
	*(volatile uint32 *)EXT_MEM_DCFG1 = (BIT_15| (1 << row_number));
/*	
	*(volatile uint32 *)EXT_MEM_DTIMING = (((50/sdram_cycle_ns)<<28) | ((100/sdram_cycle_ns)<<24)
									 | ((100/sdram_cycle_ns - 1)<<20) | ((2-1)<<18)
									 | ((15/sdram_cycle_ns)<<16) | ((19/sdram_cycle_ns)<<14)
									 | ((19/sdram_cycle_ns)<<12) | (t_ref_interval/sdram_cycle_ns)); 
*/
	*(volatile uint32 *)EXT_MEM_DTIMING = (((sdram_timing_ptr->ras_min/sdram_cycle_ns)<<28)
									 | ((sdram_timing_ptr->xsr_min/sdram_cycle_ns)<<24)
									 | ((sdram_timing_ptr->rfc_min/sdram_cycle_ns)<<20)
									 | ((sdram_timing_ptr->mrd_min/sdram_cycle_ns)<<18)
									 | ((sdram_timing_ptr->wr_min/sdram_cycle_ns)<<16)
									 | ((sdram_timing_ptr->rcd_min/sdram_cycle_ns)<<14)
									 | ((sdram_timing_ptr->row_pre_min/sdram_cycle_ns)<<12)
									 | ((sdram_timing_ptr->row_ref_max/sdram_cycle_ns)-1) );
}

void SDRAM_Device_Init(SDRAM_CFG_INFO_T * sdram_cfg_ptr)
{
   	uint8 mode_reg_bl = 0xFF;
    uint8 mode_reg_bt = MODE_REG_BT_SEQ;   //sequencial mode burst.
    uint8 mode_reg_cl = 0xFF;
    uint8 mode_reg_opmode = MODE_REG_OPMODE;
    uint8 mode_reg_wb = MODE_REG_WB_PRORAM;  //Programming burst length for write.
    
    uint16 ex_mode_reg = 0;
    uint16 mode_reg = 0;
    uint8 dsoft_cs = 0; // command for CS0
    
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
    
    *(volatile uint32 *)EXT_MEM_DCMD |= BIT_16 | (dsoft_cs<<28);// Precharge all banks.
    while ((*(volatile uint32 *)EXT_MEM_DCMD) & BIT_16)
        ;
        
    *(volatile uint32 *)EXT_MEM_DCMD |= BIT_17 | (dsoft_cs<<28);//Auto_ref
    while ((*(volatile uint32 *)EXT_MEM_DCMD) & BIT_17)
        ;
        
    *(volatile uint32 *)EXT_MEM_DCMD |= BIT_17 | (dsoft_cs<<28);//Auto_ref
    while ((*(volatile uint32 *)EXT_MEM_DCMD) & BIT_17)
        ;
        
    //mode register load.
    *(volatile uint32 *)EXT_MEM_DCMD &= ~(0xFFFF);
    *(volatile uint32 *)EXT_MEM_DCMD |= (mode_reg | BIT_18 | (dsoft_cs<<28));
    while ((*(volatile uint32 *)EXT_MEM_DCMD) & BIT_18)
        ;
    //extended mode register load.
	if (ex_mode_reg != SDRAM_EXT_MODE_INVALID)
	{
		*(volatile uint32 *)EXT_MEM_DCMD &= ~(0xFFFF);
    	*(volatile uint32 *)EXT_MEM_DCMD |= (ex_mode_reg | BIT_18 | (dsoft_cs<<28));
    	while ((*(volatile uint32 *)EXT_MEM_DCMD) & BIT_18)
        	;
	}       
}
#endif

#if defined (NAND_FDL_SC8800H)

/*---------------------------------MEM_DCFG4--MEM_DCFG5--MEM_DCFG5-0x0094--0x009c---0x00c0----0x00c4---0x00c8---0x00cc---0x00D0---0x00D4---0x00D8---0x00DC-----*/
const uint32 ext_mem_dcfg[2][5][13] = {	
/*cas2:setting*/
/*120Mhz*/						{	{0x632727, 0x380010, 0x380C0E, 0x10005, 0x10008, 0x1000c, 0x1000c, 0x1000c, 0x1000c, 0x10003, 0x10003, 0x10003, 0x10003,},\
/*100Mhz*/							{0x632727, 0x380038, 0x380F0E, 0x10005, 0x10007, 0x10005, 0x10005, 0x10005, 0x10005, 0x10001, 0x10001, 0x10001, 0x10003,},\
/*78Mhz*/							{0x632727, 0x180018, 0x380C0E, 0x10006, 0x1000F, 0x10005, 0x10005, 0x10005, 0x10005, 0x10006, 0x10006, 0x10006, 0x10007,},\
/*50Mhz*/							{0x632727, 0x180018, 0x380C0E, 0x10005, 0x10010, 0x10005, 0x10005, 0x10005, 0x10005, 0x10018, 0x10018, 0x10018, 0x10018,},\
/*25Mhz*/							{0x632727, 0x180018, 0x380C0E, 0x10005, 0x10024, 0x10005, 0x10005, 0x10005, 0x10005, 0x10038, 0x10038, 0x10038, 0x10038,},},\
/*cas3:setting*/
/*120Mhz*/						{	{0x632729, 0xE00040, 0x700C0E, 0x10005, 0x1000A, 0x10001, 0x10001, 0x10001, 0x10001, 0x10004, 0x10004, 0x10004, 0x10004,},\
/*100Mhz*/							{0x632729, 0xE00040, 0x700C0E, 0x10005, 0x1000C, 0x10005, 0x10005, 0x10005, 0x10005, 0x10001, 0x10001, 0x10001, 0x10003,},\
/*78Mhz*/							{0x632729, 0xE00040, 0x700C0E, 0x10005, 0x1000F, 0x10005, 0x10005, 0x10005, 0x10005, 0x10006, 0x10006, 0x10006, 0x10007,},\
/*50Mhz*/							{0x632729, 0xE00040, 0x700C0E, 0x10005, 0x10010, 0x10005, 0x10005, 0x10005, 0x10005, 0x10015, 0x10015, 0x10015, 0x10015,},\
/*25Mhz*/							{0x632729, 0xE00040, 0x700C0E, 0x10005, 0x10024, 0x10005, 0x10005, 0x10005, 0x10005, 0x10038, 0x10038, 0x10038, 0x10038,},}};


void SDRAM_GenMemCtlCfg(SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)
{
	uint32 reg_val;
	uint32 rburst_length = sdram_cfg_ptr->burst_length;
	uint32 wburst_length = sdram_cfg_ptr->burst_length;
	uint32 data_width    = sdram_cfg_ptr->data_width;
	
	rburst_length &= 0x3;
	wburst_length &= 0x3;
		
    //Config channel2
    //Enable ch2, no switch
    REG32(EXT_MEM_CFG0) |= (BIT_2|BIT_12|BIT_13);
    
    //Config channel0 big endian
    REG32(EXT_MEM_CFG0) |= BIT_4;
	
	//Disable all channel eburst_hit_en
	REG32(EXT_MEM_CFG0) &= ~(BIT_20|BIT_21|BIT_22|BIT_23);
        
        //DMEM enable, (CS0)-->DMEM
    REG32(EXT_MEM_CFG1) |= (BIT_0|BIT_10);    //DMEM enable
    REG32(EXT_MEM_CFG1) &= ~(BIT_12);                           //Clear smem_only_en

	REG32(EXT_MEM_CFG1) &= ~(BIT_9);                        //SDRAM mode
	
	if(data_width == DATA_WIDTH_16)
	{
	    //Config read/write latency for cs0 here...	
	    if(rburst_length == BURST_LEN_8)
	    {
	        rburst_length = BURST_LEN_4;
	    }
	    else if(rburst_length == BURST_LEN_4)
	    {
	        rburst_length = BURST_LEN_2;
	    }

	    if(wburst_length == BURST_LEN_8)
	    {
	        wburst_length = BURST_LEN_4;
	    }
	    else if(wburst_length == BURST_LEN_4)
	    {
	        wburst_length = BURST_LEN_2;
	    }
	    
    	reg_val = REG32(EXT_MEM_CFG2);
    	reg_val &= ~(0x3 | (0x3<<8));
    	reg_val |= (rburst_length | (wburst_length<<8));
    	REG32(EXT_MEM_CFG2) = reg_val; 	
	    
	}
	else if(data_width == DATA_WIDTH_32)
	{
    	//Config read/write latency for cs0 here...	
    	reg_val = REG32(EXT_MEM_CFG2);
    	reg_val &= ~(0x3 | (0x3<<8));
    	reg_val |= (rburst_length | (wburst_length<<8));
    	REG32(EXT_MEM_CFG2) = reg_val; 	
	}
	else
	{
	    while(1);
	}
}

void SDRAM_DMemCtlCfg(uint32 clk,SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)
{
	uint32 i=0,clk_liv=0,j=0;
	uint32 t_rtw = 0;
	
	uint32 row_mode = sdram_cfg_ptr->row_mode;
	uint32 col_mode = sdram_cfg_ptr->col_mode;
	uint32 data_width = sdram_cfg_ptr->data_width;
	uint32 cas_latency = sdram_cfg_ptr->cas_latency;
	uint32 write_latency = 0;
		
    uint32 sdram_cycle_ns = 1000/(clk); //1000000000/(clk*1000000),this value can be set a little bigger.
	uint32 row_number = 0xFFFFFFFF;
	SDRAM_TIMING_PARA_T_PTR sdram_parameters = SDRAM_GetTimingPara( );
	
	uint32 t_ras = sdram_parameters->ras_min/sdram_cycle_ns;//sdram_parameters[T_RAS_MIN]/sdram_cycle_ns ;
	uint32 t_xsr = sdram_parameters->xsr_min/sdram_cycle_ns;
    uint32 t_rfc = sdram_parameters->rfc_min/sdram_cycle_ns ;
    uint32 t_mrd = sdram_parameters->mrd_min/sdram_cycle_ns ;
    uint32 t_wr  = sdram_parameters->wr_min/sdram_cycle_ns ;
    uint32 t_rcd = sdram_parameters->rcd_min/sdram_cycle_ns ;
    uint32 t_rp  = sdram_parameters->row_pre_min/sdram_cycle_ns ;
    uint32 t_ref = sdram_parameters->row_ref_max/sdram_cycle_ns ;    
    uint32 t_wtr = 0xf;
    
    if(cas_latency == 3)
    	t_rtw = 3;
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

    
    REG32(EXT_MEM_DCFG0) = (                                    \
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

void SDRAM_Device_Init(SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)
{
  	uint8 mode_reg_bl = 0xFF;
    uint8 mode_reg_bt = MODE_REG_BT_SEQ;   //sequencial mode burst.
    uint8 mode_reg_cl = 0xFF;
    uint8 mode_reg_opmode = MODE_REG_OPMODE;
    uint8 mode_reg_wb = MODE_REG_WB_PRORAM;  //Programming burst length for write.
    
    uint32 ex_mode_reg = 0;
    uint16 mode_reg = 0;
    uint8 dsoft_cs = 0; // command for CS0
    
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
#endif
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         
//	Note:     After chip and SDRAM power up, software program boots from on-chip memory. 
//            Then software should do the following tasks in on-chip memory. 
/*****************************************************************************/	
void SDRAM_Init(uint32 clk)
{
    SDRAM_CFG_INFO_T *pCfg = NULL;
 #if !defined (NAND_FDL_SC6800D)   
 	volatile unsigned long i,emcs_ctl;
	uint32 ctl0_val;
 #endif
    //Call Userdef_before_sdram_init()
    Userdef_before_sdram_init();
    //Get SDRAM_cfg
    pCfg = SDRAM_GetCfg();

#if defined (NAND_FDL_SC6800D)
	SDRAM_Clk_Dly_Cfg(clk,(pCfg->clk_dly & 0xFFFF),((pCfg->clk_dly & 0xFFFF0000) >> 16));

	*(volatile uint32 *)EXT_MEM_DCFG0 &= ~(DCFG0_AUTOREF_EN);

	*(volatile uint32 *)EXT_MEM_CFG0 |= BIT_15;

	SDRAM_GenMemCtlCfg();

	SDRAM_DMemCtlCfg(clk,pCfg);
	
	SDRAM_Device_Init(pCfg);

	*(volatile uint32 *)EXT_MEM_DCFG1 |= BIT_15;

	*(volatile uint32 *)EXT_MEM_DCFG0 &= ~(BIT_3);

	*(volatile uint32 *)EXT_MEM_DCFG0 |= BIT_23;
#elif defined (NAND_FDL_SC8800H)



	REG32(EXT_MEM_DCFG0) &= ~(DCFG0_AUTOREF_EN);    //Disable auto refresh

	SDRAM_GenMemCtlCfg(pCfg);
	
	//SDRAM_Auto_DllCtlCfg();							//automatically config the sdram

	SDRAM_DMemCtlCfg(clk,pCfg);

	SDRAM_Device_Init(pCfg);
	

    REG32(EXT_MEM_DCFG2) |= DCFG2_REF_CNT_RST;


    REG32(EXT_MEM_DCFG0) |=  (DCFG0_AUTOREF_EN);    //Enable auto refresh	


#else
    //Check SDRAM configuration
    if(NULL == pCfg)
    {
        send_ack_packet(BSL_ILLEGAL_SDRAM);
        return;
    }
    if((pCfg->cas_latency!=0x2) && (pCfg->cas_latency!=0x3))
    {
        send_ack_packet(BSL_WRONG_SDRAM_PARAMETER);
        return;
    }
    if(pCfg->sdram_size > 0x3)
    {
        send_ack_packet(BSL_WRONG_SDRAM_PARAMETER);
        return;
    }
    if(SDRAM_EXT_INVALID!=pCfg->ext_mode_val)
    {
        if(pCfg->ext_mode_val>0x3fff)
        {
           send_ack_packet(BSL_WRONG_SDRAM_PARAMETER);
           return;
        }   
    }
#if defined(NAND_FDL_SC6600R)
    *(volatile uint32 *)(0x8b000050) = 0;
    *(volatile uint32 *)(0x8b000054) = 0;
    for(i=0;i<100;i++);
#endif    
    //Set clock delay
    *(volatile uint32 *)CLK_DLY = pCfg->clk_dly;
    /*Initialize the SDRAM*/    
    //Refresh time = a
    //Precharge time = a
    //Active to read/write time = 1
    //Write recovery time = 2
    //CAS Latency time = pCfg->cas_latency & 0x3
    //burst length = 0x11
    //the mapping of SDRAM address is bank address,row address, col address.
    //the size of SDRAM is pCfg->sdram_size & 0x3
    ctl0_val = 0x800591aa | (pCfg->cas_latency<<13) | (pCfg->sdram_size<<19);
    *(volatile uint32 *)EXTMEMCTL0=ctl0_val;       //0x8019f1a5  //0x8015d162
    *(volatile uint32 *)EXTMEMINI = 0x00d24018;
    //Wait initialization finish
    emcs_ctl = *(volatile uint32 *)(0x20000024);
    while((emcs_ctl & 0x00000002) != 0x00000002)
    {
    	emcs_ctl = *(volatile uint32 *)(0x20000024);
    }    
    //Load Extend mode regiter if needed
    if(SDRAM_EXT_INVALID!=pCfg->ext_mode_val)
    {
       *(volatile uint32 *)EXTMEMINI = pCfg->ext_mode_val|(0x5<<22);       
	   //Wait initialization finish
	   emcs_ctl = *(volatile uint32 *)(0x20000024);
	   while((emcs_ctl & 0x00000002) != 0x00000002)
	   {
	   	emcs_ctl = *(volatile uint32 *)(0x20000024);
	   }  
    }
    //Refresh interval value is 0x0
    //Refresh prescale value is 0x96
    *(volatile uint32 *)(0x20000020) = 0x0022c800;
    *(volatile uint32 *)(0x20000024) |= 1<<5;
	
    for(i=0;i<100;i++);
#endif
    
    //Call Userdef_after_sdram_init()
    Userdef_after_sdram_init();
    
}

