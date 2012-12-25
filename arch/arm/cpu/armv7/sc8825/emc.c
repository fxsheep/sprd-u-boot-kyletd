/******************************************************************************
 ** File Name:        emc.c                                                                            
 ** Author:           Johnny Wang                                                                                    
 ** DATE:             2012/12/04
 ** Copyright:        2007 Spreatrum, Incoporated. All Rights Reserved.                          
 ** Description:                                                                              
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                                                         
 **-------------------------------------------------------------------------    
 ** DATE          NAME            DESCRIPTION                                                         
 ** 2012/12/04                    Create.                                                                 
 ******************************************************************************/
#include <asm/arch/os_api.h>
#include <asm/arch/arm_reg.h>
#include <asm/arch/sc_reg.h>
#include <asm/arch/sc8825_emc_cfg.h>
#include <asm/arch/emc_reg_v250a.h>
#include <asm/arch/sc8810_reg_ahb.h>
#include <asm/arch/sc8810_reg_global.h>
#include <asm/arch/adi_hal_internal.h>
#include <asm/arch/analog_reg_v3.h>


#ifdef   __cplusplus
    extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*
**                          Test Cases Prototype                              *
**----------------------------------------------------------------------------*/


uint32 LPDDR1_MEM_DS = LPDDR1_DS_39_OHM; //lpddr1 driver strength,refer to multiPHY p155
uint32 LPDDR2_MEM_DS = LPDDR2_DS_40_OHM; //lpddr1 driver strength,

uint32 B0_DQS_STEP_DLY = DQS_STEP_DLY_DEF; //byte0 dqs step delay
uint32 B1_DQS_STEP_DLY = DQS_STEP_DLY_DEF; //byte1 dqs step delay
uint32 B2_DQS_STEP_DLY = DQS_STEP_DLY_DEF; //byte2 dqs step delay
uint32 B3_DQS_STEP_DLY = DQS_STEP_DLY_DEF; //byte3 dqs step delay

uint32 B0_SDLL_PHS_DLY = SDLL_PHS_DLY_DEF; //byte0 sll dll phase delay 
uint32 B1_SDLL_PHS_DLY = SDLL_PHS_DLY_DEF; //byte1 sll dll phase delay 
uint32 B2_SDLL_PHS_DLY = SDLL_PHS_DLY_DEF; //byte2 sll dll phase delay 
uint32 B3_SDLL_PHS_DLY = SDLL_PHS_DLY_DEF; //byte3 sll dll phase delay 

DRAM_INFO_T static_dram_info={0};


int mem_name_cmp(char *str1,char *str2)
{
	while((*str1==*str2)&& *str1!='\0')
	{
		str1++;
		str2++;
	}

	if(*str1=='\0'&&*str2=='\0')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static BOOLEAN __is_bond_lpddr2()
{
    
    if( ((ADI_Analogdie_reg_read(ANA_STATUS))&BIT_6) == BIT_6 )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}    


static void set_reg_fld(uint32 addr, uint32 start_bit, uint32 bit_num, uint32 value) {
	uint32 temp, i;
	temp = REG32(addr) ;
	for(i=0;i<bit_num;i++) {
		temp &= ~(0x1<<(start_bit+i));
	}
	temp |= value<<start_bit;
	REG32(addr) = temp;
}

DRAM_INFO_T_PTR get_dram_info(char* dram_chip_name)
{
	uint32 i = 0;
	for(i = 0; i < sizeof(DRAM_MODE_INFO_ARRAY)/sizeof(DRAM_MODE_INFO_T);i++)
	{
		if(mem_name_cmp(DRAM_CHIP_NAME_INFO_ARRAY[i],dram_chip_name)==TRUE)
		{
			static_dram_info.chip_name = DRAM_CHIP_NAME_INFO_ARRAY[i];
			static_dram_info.time_info = &DRAM_TIMING_INFO_ARRAY[i];
			static_dram_info.mode_info = &DRAM_MODE_INFO_ARRAY[i];

			break;
		}
	}


	return &static_dram_info;
}

void set_reg_val_min_max(uint32 reg,uint32 val,uint32 min,uint32 max)
{
	if(val>=min && val<=max)
	{
		REG32(reg) = val;
	}
	else if(val<min)
	{
		REG32(reg) = min;
	}
	else
	{
		REG32(reg) = max;
	}
}

uint32 cal_cs_val(DRAM_DENSITY_E density)
{
	switch(density)
	{
		case DRAM_64MBIT: return 0;
		case DRAM_128MBIT:return 1;
		case DRAM_256MBIT:return 2;
		case DRAM_512MBIT:return 3;
		case DRAM_1GBIT  :return 4;
		case DRAM_2GBIT  :return 5;
		case DRAM_4GBIT  :return 6;
		case DRAM_8GBIT  :return 7;
		default          :return 6;
	}
}


void wait_us(uint32 us) 
{
	volatile uint32 i;
	volatile uint32 j;
	for(i=0;i<us;i++) 
	{
	    for(j=0;j<1000;j++);
	}
}

void wait_pclk(uint32 n)
{
    volatile uint32 i;
    volatile value;

    for(i = 0; i < n; i++)
    {
        value = REG32(PUBL_CFG_PGSR);
    }
    value = value;
}
void modify_dpll_freq (CLK_TYPE_E emc_clk) 
{
	uint32 temp;
	
	//step1: enable register write
	REG32(GR_GEN1_SET) = BIT_9;
	
	//step2: modify dpll parameter N
	temp = (emc_clk/4000000);
    set_reg_fld(GR_DPLL_MN,0,11,temp);	
	 
	//step3: disable register write
	REG32(GR_GEN1_CLR) = BIT_9;
}

void set_emc_clk(CLK_TYPE_E emc_clk) 
{
	REG32(GR_GEN1_SET) = BIT_9;

	//ensure emc is in idle state
	//step 1, disable emc clk
	set_reg_fld(AHB_CTL0,28,1,0);

	//step 2,assert address,command,data dll
	set_reg_fld(PUBL_CFG_ACDLLCR,30,1,0);
	set_reg_fld(PUBL_CFG_DX0DLLCR,30,1,0);
	set_reg_fld(PUBL_CFG_DX1DLLCR,30,1,0);
	set_reg_fld(PUBL_CFG_DX2DLLCR,30,1,0);
	set_reg_fld(PUBL_CFG_DX3DLLCR,30,1,0);	


	//step3,modify dpll 
	modify_dpll_freq(emc_clk);		
	set_reg_fld(AHB_ARM_CLK,12,2,1);
	set_reg_fld(AHB_ARM_CLK,8,4,0);	 //clk_emc_div=0
	set_reg_fld(AHB_ARM_CLK,3,1,1);	 //clk_emc_sync	
	wait_us(150); //PLL lock time. 150us should be enough.  please define this function by yourself

	//step4,enable emc clk
	set_reg_fld(AHB_CTL0,28,1,1);

	//step 2,deassert address,command,data dll
	set_reg_fld(PUBL_CFG_ACDLLCR,30,1,1);
	set_reg_fld(PUBL_CFG_DX0DLLCR,30,1,1);
	set_reg_fld(PUBL_CFG_DX1DLLCR,30,1,1);
	set_reg_fld(PUBL_CFG_DX2DLLCR,30,1,1);
	set_reg_fld(PUBL_CFG_DX3DLLCR,30,1,1);	
	wait_us(10); //for DLL to lock, spec is 5.12us. we reserve more cycles. please define this function by yourself

	REG32(GR_GEN1_CLR) = BIT_9;
}

void EMC_CTL_State_Move(EMC_CTL_STATE_E next_state)
{
	
	switch(next_state)
	{
		case CTL_STATE_INIT_MEM :
		{
		    while(CTL_CURRENT_STATE != CTL_STATE_INIT_MEM) 
			{
				switch(CTL_CURRENT_STATE)
				{
					case CTL_STATE_CONFIG:
					{
						REG32(UMCTL_CFG_SCTL) = CTL_CMD_INIT;
						while(CTL_CURRENT_STATE != CTL_STATE_INIT_MEM);
						break;
					}
					case CTL_STATE_ACCESS:
					{
						REG32(UMCTL_CFG_SCTL) = CTL_CMD_CFG;
						while(CTL_CURRENT_STATE != CTL_STATE_CONFIG);
						break;
					}
					case CTL_STATE_LOW_POWER:
					{
						REG32(UMCTL_CFG_SCTL) = CTL_CMD_WAKEUP;
						while(CTL_CURRENT_STATE != CTL_STATE_ACCESS);
						break;
					}
					default : break;
				}
		    }
		break;	
		}

		case CTL_STATE_CONFIG:
		{
		    while(CTL_CURRENT_STATE != CTL_STATE_CONFIG) 
			{
				switch(CTL_CURRENT_STATE)
				{
					case CTL_STATE_LOW_POWER:
					{
						REG32(UMCTL_CFG_SCTL) = CTL_CMD_WAKEUP;
						while(CTL_CURRENT_STATE != CTL_STATE_ACCESS);
						break;
					}
					case CTL_STATE_INIT_MEM:
					{
						REG32(UMCTL_CFG_SCTL) = CTL_CMD_CFG;
						while(CTL_CURRENT_STATE != CTL_STATE_CONFIG);
						break;
					}
					case CTL_STATE_ACCESS:
					{
						REG32(UMCTL_CFG_SCTL) = CTL_CMD_CFG;
						while(CTL_CURRENT_STATE != CTL_STATE_CONFIG);
						break;
					}
					default :break;
				}
		    }
		break;	
		}

		case CTL_STATE_LOW_POWER:
		{
		    while(CTL_CURRENT_STATE != CTL_STATE_LOW_POWER) 
			{
				switch(CTL_CURRENT_STATE)
				{
					case CTL_STATE_ACCESS:
					{
						REG32(UMCTL_CFG_SCTL) = CTL_CMD_SLEEP;
						while(CTL_CURRENT_STATE != CTL_STATE_LOW_POWER);
						break;
					}
					case CTL_STATE_CONFIG:
					{
						REG32(UMCTL_CFG_SCTL) = CTL_CMD_GO;
						while(CTL_CURRENT_STATE != CTL_STATE_ACCESS);
						break;
					}
					case CTL_STATE_INIT_MEM:
					{
						REG32(UMCTL_CFG_SCTL) = CTL_CMD_CFG;
						while(CTL_CURRENT_STATE != CTL_STATE_CONFIG);
						break;
					}
					default :break;
				}
		    }
		break;	
		}
		
		case CTL_STATE_ACCESS:
		{
		    while(CTL_CURRENT_STATE != CTL_STATE_ACCESS) 
			{
				switch(CTL_CURRENT_STATE)
				{
					case CTL_STATE_CONFIG:
					{
						REG32(UMCTL_CFG_SCTL) = CTL_CMD_GO;
						while(CTL_CURRENT_STATE != CTL_STATE_ACCESS);
						break;
					}
					case CTL_STATE_INIT_MEM:
					{
						REG32(UMCTL_CFG_SCTL) = CTL_CMD_CFG;
						while(CTL_CURRENT_STATE != CTL_STATE_CONFIG);
						break;
					}	
					case CTL_STATE_LOW_POWER:
					{
						REG32(UMCTL_CFG_SCTL) = CTL_CMD_WAKEUP;
						while(CTL_CURRENT_STATE != CTL_STATE_ACCESS);
						break;
					}
					default :break;
				}
		    }
		break;	
		}
		
		default :break;		
	}	
}


void EMC_MEM_Mode_set(DRAM_INFO_T_PTR dram_info)
{

	uint32 mddr_lpddr2_en = 0;

	if(dram_info->mode_info->mem_type == DRAM_LPDDR1)
	{
		mddr_lpddr2_en = 2;
	}
	else if(((dram_info->mode_info->mem_type)&DRAM_LPDDR2) == DRAM_LPDDR2)
	{
		mddr_lpddr2_en = 3;

	}
	else//DDR3
	{
		mddr_lpddr2_en = 0;	
	}
	#ifdef EMC_SMALL_CODE_SIZE
        REG32(UMCTL_CFG_MCFG) = 0x60010;
        REG32(UMCTL_CFG_MCFG) |=((mddr_lpddr2_en<<22)|
                                 (dram_info->mode_info->bl<<20)|
                                 ((dram_info->mode_info->mem_type ==DRAM_LPDDR2_S4)? 0x40:0));

        REG32(UMCTL_CFG_MCFG1) = 0;

    #else
    REG32(UMCTL_CFG_MCFG) = ( 
		(0x0<<24)						|	//mddr_lpddr2_clk_stop_idle, 0:disable
		(mddr_lpddr2_en<<22) 			|	// 2'b00 = mDDR/LPDDR2 Disabled, 2'b10 = mDDR Enabled, 2'b11 = LPDDR2 Enabled
		(dram_info->mode_info->bl<<20) 	| 	
		(1<<18)							|	//tFAW to be 4,5,or 6 times tRRD when=0,1,2
		(1<<17)							|	//power down exit mode,0:slow exit 1:fast exit
		(0<<16)							|	//power down type,0:percharge power down,1:active power down
		(0x0<<8)						|	//Power-down idle period in n_clk cycles. Memories are placed into power-down modeif the NIF is idle for pd_idle n_clk cycles
		((dram_info->mode_info->mem_type ==DRAM_LPDDR2_S4)? 0x40:0)  	| //lpddr2_s4 enable
		((dram_info->mode_info->mem_type ==DRAM_DDR3)? 0x20:0)  		| //ddr3 enable
		(1<<4)							|	// stagger cs_n,0:dis 1:enable
		(0<<3)  						| 	// 2T timing for memory command,0:dis 1:enable
		(0<<2)							|   // Setting this bit enables the BL8 interrupt function of DDR2		
		//(1<<1)							|   // This bit is intended to be set for 4-rank RDIMMs, which have a 2-bit CKE input
		(dram_info->mode_info->bl==DRAM_BL8?1:0) );// The BL setting in DDR2 / DDR3 must be consistent with the value programmed into the BL field of MR0.
							            	// 1'b0 = BL4, Burst length of 4 (MR0.BL=3'b010, DDR2 only)
							            	// 1'b1 = BL8, Burst length of 8 (MR0.BL=3'b011 for DDR2, MR0.BL=2'b00 for DDR3)
								
	REG32(UMCTL_CFG_MCFG1) =  ((0<<31) 	| 	//When this bit is programmed to 1¡¯b1 the c_active_in pin can be used to exit from the automatic clock stop , power down or self-refresh modes.
							(0x0<<16)	|	//Hardware idle period. The c_active output is driven high if the NIF is idle in Access state for hw_idle * 32 * n_clk cycles
							(0<<8)		|	//tfaw_cfg_offset
							(0x0<<0) );		//Self Refresh idle period. Memories are placed into Self-Refresh mode if the NIF is idle in Access state for sr_idle * 32 * n_clk cycles

     #endif                            
	if(dram_info->mode_info->io_width == IO_WIDTH_32)//x32
	{
	}
	else if(dram_info->mode_info->io_width == IO_WIDTH_16)//x16
	{
        	#ifdef EMC_SMALL_CODE_SIZE
         REG32(UMCTL_CFG_PPCFG) = 0x18;
         #else   
		REG32(UMCTL_CFG_PPCFG) =(1<<0) |	//Reduced Population Enable
		                        (1<<3) |	//byte2 disable
		                        (1<<4); 	//byte3 disable
        #endif		                        
	}
	else//x8
	{
         #ifdef EMC_SMALL_CODE_SIZE
         REG32(UMCTL_CFG_PPCFG) = 0x1c;
         #else
		REG32(UMCTL_CFG_PPCFG) =(1<<0) |	//Reduced Population Enable
		                        (1<<2) |	//byte1 disable		
		                        (1<<3) |	//byte2 disable
		                        (1<<4);  	//byte3 disable
        #endif		                        
	}

	REG32(UMCTL_CFG_LPDDR2ZQCFG) = 0xAB0A560A;//[31:24]:zqcl_op	 [23:16]:zqcl_ma  [15:8]:zqcs_op  [7:0]:zqcs_ma

}


void EMC_MEM_Timing_Set(CLK_TYPE_E emc_clk,DRAM_INFO_T_PTR dram_info)
{
	DRAM_TIMING_INFO_T_PTR time_info = dram_info->time_info;
	DRAM_MODE_INFO_T_PTR   mode_info = dram_info->mode_info;	

	uint32 mem_t = 0;

	mem_t = (1000000000/emc_clk);
		
	REG32(UMCTL_CFG_TOGCNT1U) 	= emc_clk/1000000+1;		
	REG32(UMCTL_CFG_TINIT) 	= 200; //200us
	#ifndef EMC_SMALL_CODE_SIZE
	REG32(UMCTL_CFG_TRSTH) 	= dram_info->mode_info->mem_type==DRAM_DDR3? 500:0; //500us when ddr3,others 0
	#else
	REG32(UMCTL_CFG_TRSTH) 	= 0; //500us when ddr3,others 0
    #endif
	REG32(UMCTL_CFG_TOGCNT100N) = emc_clk/10000000+1;
	
    REG32(UMCTL_CFG_TREFI)	= time_info->tREFI/100;				//unit is TOGCNT100N
    	#ifndef EMC_SMALL_CODE_SIZE
    REG32(UMCTL_CFG_TMRD) 	= (mode_info->mem_type==DRAM_LPDDR1)? 2: ((mode_info->mem_type==DRAM_DDR3)?4:5);
    #else
    REG32(UMCTL_CFG_TMRD) 	= (mode_info->mem_type==DRAM_LPDDR1)? 2:5;
    #endif
    
    REG32(UMCTL_CFG_TRTW) 	= 0x00000006; //modify by johnnywang
    REG32(UMCTL_CFG_TAL) 	= 0x00000000; //no al for lpddr1/lpddr2,ddr3:0, CL-1, CL-2 (depending on AL=0,1,2 in MR1)
    REG32(UMCTL_CFG_TCL) 	= mode_info->rl;
    REG32(UMCTL_CFG_TCWL) 	= (mode_info->mem_type == DRAM_LPDDR1) ? 1 : mode_info->wl;
    REG32(UMCTL_CFG_TDPD) 	= (((mode_info->mem_type)&DRAM_LPDDR2)==DRAM_LPDDR2) ? 500 : 0;

    REG32(UMCTL_CFG_TXP) 	= time_info->tXP;	//tXPmin=7.5ns,fast exit	

    REG32(UMCTL_CFG_TCKE) 	= (mode_info->mem_type==DRAM_LPDDR1)? 2:3;
    //REG32(UMCTL_CFG_TZQCSI)= (mode_info->mem_type==DRAM_LPDDR1)? 0:0;	
    REG32(UMCTL_CFG_TZQCSI)= 0;	
    REG32(UMCTL_CFG_TZQCL)	= (mode_info->mem_type==DRAM_LPDDR1)? 0:(time_info->tZQCL/mem_t+1);	//???

    	#ifndef EMC_SMALL_CODE_SIZE
    REG32(UMCTL_CFG_TMOD)= (mode_info->mem_type==DRAM_DDR3)? 12:0;	//???
    #else
    REG32(UMCTL_CFG_TMOD) 	= 0;	//???
    #endif
    
    	#ifndef EMC_SMALL_CODE_SIZE
    REG32(UMCTL_CFG_TRSTL) = (mode_info->mem_type==DRAM_DDR3)? (100/mem_t+1):0;	//???
    #else
    REG32(UMCTL_CFG_TRSTL) = 0;	//???
    #endif
	REG32(UMCTL_CFG_TMRR) 	= 2;	// default value,don't need set    
	
	if(mode_info->mem_type==DRAM_LPDDR1)
	{
		set_reg_val_min_max(UMCTL_CFG_TRFC, (time_info->tRFC/mem_t+1),7,28);
		REG32(UMCTL_CFG_TRP)    = 3;
		set_reg_val_min_max(UMCTL_CFG_TRAS, (time_info->tRAS/mem_t+1),4,8);		
		set_reg_val_min_max(UMCTL_CFG_TRC,  (time_info->tRC/mem_t+1),5,11);				
		REG32(UMCTL_CFG_TRCD)   = 3;
		REG32(UMCTL_CFG_TRRD)   = 2;
		REG32(UMCTL_CFG_TRTP)   = 0;
		REG32(UMCTL_CFG_TWR)    = 3;
		REG32(UMCTL_CFG_TZQCS)	= 0;

		set_reg_val_min_max(UMCTL_CFG_TWTR, time_info->tWTR,1,2);				
	    set_reg_val_min_max(UMCTL_CFG_TEXSR, time_info->tXSR/mem_t+1,17,40);
	    REG32(UMCTL_CFG_TXPDLL) = 0;	//slow exit should be 0???		
		REG32(UMCTL_CFG_TDQS) 	= 2;
    	REG32(UMCTL_CFG_TCKSRE) = 0;
    	REG32(UMCTL_CFG_TCKSRX) = 0;		
    	REG32(UMCTL_CFG_TZQCL)  = 0;				
	    REG32(UMCTL_CFG_TCKESR) = 0;					
	}
	else if(((mode_info->mem_type)&DRAM_LPDDR2)==DRAM_LPDDR2)
	{
		set_reg_val_min_max(UMCTL_CFG_TRFC, (time_info->tRFC/mem_t+1),15,112);
		set_reg_val_min_max(UMCTL_CFG_TRP,  (0x10000)+(time_info->tRP/mem_t+1), 0x10003, 0x1000d);		
		set_reg_val_min_max(UMCTL_CFG_TRAS, (time_info->tRAS/mem_t+1),7,23);
		set_reg_val_min_max(UMCTL_CFG_TRC,  (time_info->tRC/mem_t+1),10,36);			
		set_reg_val_min_max(UMCTL_CFG_TRCD, (time_info->tRCD/mem_t+1),3,13);			
		set_reg_val_min_max(UMCTL_CFG_TRRD, (time_info->tRRD/mem_t+1),2,6);			
		REG32(UMCTL_CFG_TRTP) = 4;
		set_reg_val_min_max(UMCTL_CFG_TWR,  (time_info->tWR/mem_t+1),3,8);		
		//REG32(UMCTL_CFG_TWR)  = 6;
		set_reg_val_min_max(UMCTL_CFG_TWTR, time_info->tWTR,2,4);										
	    REG32(UMCTL_CFG_TEXSR) 	= 512;
	    REG32(UMCTL_CFG_TXPDLL) = 0;
		set_reg_val_min_max(UMCTL_CFG_TZQCS, (time_info->tZQCS/mem_t+1),15,48);			
		REG32(UMCTL_CFG_TDQS) 	= 2;		    
    	REG32(UMCTL_CFG_TCKSRE) = 0;			
    	REG32(UMCTL_CFG_TCKSRX) = 0;	
	    set_reg_val_min_max(UMCTL_CFG_TZQCL, time_info->tZQCL/mem_t+1,60,192);
	    set_reg_val_min_max(UMCTL_CFG_TCKESR, time_info->tCKESR/mem_t+1,3,8);
			
	}		
	else //DRAM_DDR3
	{
        	#ifndef EMC_SMALL_CODE_SIZE
		set_reg_val_min_max(UMCTL_CFG_TRFC, (time_info->tRFC/mem_t+1),36,374);	
		set_reg_val_min_max(UMCTL_CFG_TRP,  (1<<16)+(time_info->tRP/mem_t+1), 5, 14);
		set_reg_val_min_max(UMCTL_CFG_TRAS, (time_info->tRAS/mem_t+1),15,38);
		set_reg_val_min_max(UMCTL_CFG_TRC,  (time_info->tRC/mem_t+1),20,52);			
		set_reg_val_min_max(UMCTL_CFG_TRCD, (time_info->tRCD/mem_t+1),5,14);						
		set_reg_val_min_max(UMCTL_CFG_TRRD, (time_info->tRRD/mem_t+1),4,8);			
		REG32(UMCTL_CFG_TRTP) = 4;
		set_reg_val_min_max(UMCTL_CFG_TWR,  (time_info->tWR/mem_t+1),6,16);			
		set_reg_val_min_max(UMCTL_CFG_TWTR, time_info->tWTR,3,8);							
	    set_reg_val_min_max(UMCTL_CFG_TEXSR, time_info->tXSR/mem_t+1,17,117);	
	    REG32(UMCTL_CFG_TXPDLL) = 10;
		REG32(UMCTL_CFG_TZQCS) 	= 64;
		REG32(UMCTL_CFG_TDQS) 	= 4;		    
    	REG32(UMCTL_CFG_TCKSRE) = 5;
    	REG32(UMCTL_CFG_TCKSRX) = 5;
	    set_reg_val_min_max(UMCTL_CFG_TZQCL, time_info->tZQCL/mem_t+1,0,1023);	
		REG32(UMCTL_CFG_TCKESR) = REG32(UMCTL_CFG_TCKE)+1;
        #endif
	}
}

void EMC_MEM_Power_Up()
{
	volatile uint32 i = 0;
	REG32(UMCTL_CFG_POWCTL) = 1;

	//wait unitl power up finished
	do{i = (REG32(UMCTL_CFG_POWSTAT)&0X1);}
	while(i == 0);

	wait_us(10);
}



void EMC_CTL_Mode_Set(DRAM_INFO_T_PTR dram_info)
{
	uint32 dram_type = 1;
	uint32 dram_dsty_cs0 = cal_cs_val(dram_info->mode_info->cs0_cap);
	uint32 dram_dsty_cs1 = cal_cs_val(dram_info->mode_info->cs1_cap);
	uint32 dram_io_width = dram_info->mode_info->io_width;

	if(dram_info->mode_info->mem_type==DRAM_LPDDR2_S4)
	{
		dram_type = 1;
	}
	else if(dram_info->mode_info->mem_type==DRAM_LPDDR2_S2)
	{
		dram_type = 0;
	}
	else if(dram_info->mode_info->mem_type==DRAM_LPDDR1)
	{
		if( (dram_info->mode_info->cs0_cap == DRAM_1GBIT   && dram_info->mode_info->io_width==IO_WIDTH_32) ||
			(dram_info->mode_info->cs0_cap == DRAM_512MBIT && dram_info->mode_info->io_width==IO_WIDTH_16) )
		{
			dram_type = 1;
		}
		else
		{
			dram_type = 0;
		}
	}

    REG32(UMCTL_CFG_DCFG_CS0) = RANK_ROW_BANK_COL|(dram_type<<6)|(dram_dsty_cs0<<2)|dram_io_width;
    REG32(UMCTL_CFG_DCFG_CS1) = RANK_ROW_BANK_COL|(dram_type<<6)|(dram_dsty_cs1<<2)|dram_io_width;

    REG32(UMCTL_CFG_SCFG) = 0X420;	

	REG32(UMCTL_REG_MRRCFG0) = 1;//set byte1 to store mode register read result
}

void EMC_CTL_CHN_Set(EMC_CHN_INFO_T_PTR emc_chn_info)
{
	volatile uint32 i = 0;
	for(i=EMC_PORT_MIN; i <= EMC_PORT_MAX; i++)
	{
		REG32(UMCTL_CFG_PCFG_0 +4*i) = ((emc_chn_info+i)->port_data_quantum<<16)	|
									   ((emc_chn_info+i)->rdwr_order<<7)			|
									   (0x1<<6)	| //store_forward enable
									   (0x1<<5)	| //back_pressure_read enable
									   (0x1<<4)	| //back_pressure_write enable
									   ((emc_chn_info+i)->port_priority);
	}
}


MEM_CMD_RESULT_E EMC_CTL_MDR_Issue(DRAM_INFO_T_PTR dram_info,DRAM_CS_NUM_E cs_num,MEM_CMD_TYPE_E cmd,uint8 mdr)
{
	volatile uint32 temp;
	DRAM_BL_E bl;
	DRAM_CL_E wl;
	DRAM_CL_E rl;	
	uint32 rl_wl = 0;
	uint32 nwr;
	uint32 mem_ds;
	DRAM_TYPE_E mem_type;

	bl = dram_info->mode_info->bl+1;
	wl = dram_info->mode_info->wl;
	rl = dram_info->mode_info->rl;	
	nwr = REG32(UMCTL_CFG_TWR)-2;
	mem_ds = LPDDR2_MEM_DS + 1;
	mem_type = dram_info->mode_info->mem_type&DRAM_LPDDR2;


	//caluate rl&wr parameter which should be set in lpddr2 MR2
		
	if(mem_type ==DRAM_LPDDR2)
	{
		if((rl == LPDDR2_RL3) && (wl == LPDDR2_WL1))
		{
			rl_wl = 1;
		}
		else if(rl == LPDDR2_RL4 && wl == LPDDR2_WL2)
		{
			rl_wl = 2;
		}
		else if(rl == LPDDR2_RL5 && wl == LPDDR2_WL2)
		{
			rl_wl = 3;
		}
		else if(rl == LPDDR2_RL6 && wl == LPDDR2_WL3)
		{
			rl_wl = 4;
		}
		else if(rl == LPDDR2_RL7 && wl == LPDDR2_WL4)
		{
			rl_wl = 5;
		}
		else if(rl == LPDDR2_RL8 && wl == LPDDR2_WL4)
		{
			rl_wl = 6;
		}
		else
		{
			rl_wl = 4;//default value,rl=6,wl=3
		}		
	}	

	switch(cmd)
	{
		case CMD_NOP:
		case CMD_PREA:
		case CMD_REF:
		case CMD_ZQCL:
		case CMD_ZQCS:
		case CMD_RSTL:
		case CMD_DPDE: REG32(UMCTL_CFG_MCMD) = 0X89000000+cmd+(cs_num<<20);break;
		case CMD_MRR:
		{
			if(mem_type ==DRAM_LPDDR2)
			{

				switch(mdr)
				{
					case 0:	
					case 4:
					case 5:	
					case 6:	
					case 7:	
					case 8:	
					case 32:	
					case 40: REG32(UMCTL_CFG_MCMD) = 0X89000008+(mdr<<4)+(cs_num<<20);break;
					case 1:	
					case 2:
					case 3:		
					case 9:		
					case 10:		
					case 63: return CMD_MDR_WR_ONLY;break;
					default: return CMD_MDR_NOT_EXIT;break;
				}
				
			}
			else
			{
				return CMD_MDR_NOT_EXIT;//only lpddr2 support mode resiger read
			}
			break;
		}
		case CMD_MRS:
		{
			if(mem_type ==DRAM_LPDDR2)//lpddr2
			{
				switch(mdr)
				{
					case 1:	REG32(UMCTL_CFG_MCMD) = 0X89000013|(bl<<12)|(DRAM_BURST_TYPE<<15)|(DRAM_BURST_WRAP<<16)|(nwr<<17)|(cs_num<<20);break;
					case 2: REG32(UMCTL_CFG_MCMD) = 0X89000023|(rl_wl<<12)|(cs_num<<20);break;
					case 3:	REG32(UMCTL_CFG_MCMD) = 0X89000033|(mem_ds<<12)|(cs_num<<20);break;	
					case 9:	return CMD_MDR_NOP;break;
					case 10: REG32(UMCTL_CFG_MCMD) = 0X890ff0a3|(cs_num<<20);break;//0xff:calb after init, 0xab:long calb, 0x56: short calb, 0xc3:zq reset
					case 63: REG32(UMCTL_CFG_MCMD) = 0x890ee3f3|(cs_num<<20);break;//mdr reset,will reset all mdr to default
				
					case 0:	
					case 4:
					case 5:	
					case 6:	
					case 7:	
					case 8:	
					case 32:	
					case 40: return CMD_MDR_RD_ONLY;break;
					default: return CMD_MDR_NOT_EXIT;break; 					
				}
			}
			else //lpddr1
			{
				switch(mdr)
				{                                             //ba1,ba0//
					case 0: REG32(UMCTL_CFG_MCMD) = 0x89000003|(0x0<<17)|((bl|(rl<<4))<<4)|(cs_num<<20);break;//lpddr1 mode resigter
					case 1: REG32(UMCTL_CFG_MCMD) = 0x89000003|(0x2<<17)|(0<<4)|(cs_num<<20);break;    	      //lpddr1 extern mode resigter
					default:return CMD_MDR_NOT_EXIT;break;
				}
			}
			break;	
		}

		default: return CMD_ERR;
	}


	//wait memory cmd finished
	do{temp = (REG32(UMCTL_CFG_MCMD))&0x80000000;}
	while( temp != 0x00000000);
    wait_us(10);

	//return
	return CMD_MDR_SUCCESS;	
}

BOOLEAN EMC_MEM_Init(DRAM_INFO_T_PTR dram_info)
{
	volatile uint32 value_temp;
	volatile uint32 i = 0;
	DRAM_BL_E bl = dram_info->mode_info->bl;
	DRAM_TYPE_E mem_type = dram_info->mode_info->mem_type;

	
#if 1
	if(mem_type==DRAM_LPDDR1)//lpddr1
	{
		//prechage all
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_PREA, NONE_MDR);

		//two auto refresh
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_REF, NONE_MDR);
		//EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_NOP, 0xff);
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_REF, NONE_MDR);
		//EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_NOP, 0xff);

		//set lpddr1 mode register 0
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_MRS, 0);
		//EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_NOP, 0xff);


		//set lpddr1 external mode register
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_MRS, 1);
		//EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_NOP, 0xff);				

	}
	else if((mem_type&DRAM_LPDDR2) == DRAM_LPDDR2)	//lpddr2
	{
        #if 0
		//memory reset
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_MRS, 63);
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_NOP, 0xff);
		wait_us(20);

		//zqcl to cs0
		EMC_CTL_MDR_Issue(dram_info,FIRST_CS, CMD_MRS, 10);
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_NOP, 0xff);
		wait_us(1);
		
		//zqcl to cs1
		EMC_CTL_MDR_Issue(dram_info,SECOND_CS, CMD_MRS, 10);
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_NOP, 0xff);
		wait_us(1);		
		
		//set lpddr2 mode register 1
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_MRS, 1);
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_NOP, 0xff);

		//set lpddr2 mode mode register 2
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_MRS, 2);
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_NOP, 0xff);				

		//set lpddr2 mode mode register 3
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_MRS, 3);
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_NOP, 0xff);

		//refresh
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_REF, 0xff);
		#else
		//memory reset
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_MRS, 63);
         wait_us(11);
         
		//zqcl to cs0
		EMC_CTL_MDR_Issue(dram_info,FIRST_CS, CMD_MRS, 10);
		
		//zqcl to cs1
		EMC_CTL_MDR_Issue(dram_info,SECOND_CS, CMD_MRS, 10);

        //Memory burst type sequential is not supported for mDDR and LPDDR2 with BL8        
        if(bl==DRAM_BL8 && DRAM_BURST_TYPE==DRAM_BT_SEQ) 
        {
            while(1);
        }
        
		//set lpddr2 mode register 1
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_MRS, 1);

		//set lpddr2 mode mode register 2
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_MRS, 2);

		//set lpddr2 mode mode register 3
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_MRS, 3);

		//refresh
		EMC_CTL_MDR_Issue(dram_info,ALL_TWO_CS, CMD_REF, NONE_MDR);
        
        #endif
	}
	else //ddr3 not support in tiger
	{
		while(1);
	}

#else 

    //MR63: reset memory	
    REG32(UMCTL_CFG_MCMD) = 0x89fee3f3;
    do  value_temp = REG32(UMCTL_CFG_MCMD);
    while((value_temp&0x80000000) != 0x0);

    #if 1 //must be followed in board test
      //#(10*1000); //tINIT5+tINIT4, at least 11us
      for(i=0;i<11000;i++);
    #endif

    //$display("%t,lpddr2 initialization: I/O calibration on CS0...",$time);
    //MR10: I/O calibration
    REG32(UMCTL_CFG_MCMD) = 0x891ff0a3;
    do  value_temp = REG32(UMCTL_CFG_MCMD);
    while((value_temp&0x80000000) != 0x0);
    for(i=0;i<1000;i++);

    //$display("%t,lpddr2 initialization: I/O calibration on CS1...",$time);
    //MR10: I/O calibration
    REG32(UMCTL_CFG_MCMD) = 0x892ff0a3;
    do  value_temp = REG32(UMCTL_CFG_MCMD);
    while((value_temp&0x80000000) != 0x0);
    for(i=0;i<1000;i++);

    //$display("%t,lpddr2 initialization: Device Feature 1...",$time);
    if(bl==DRAM_BL8 && DRAM_BURST_TYPE==DRAM_BT_SEQ) {
      //$display("%t,{%m}: Memory burst type sequential is not supported for mDDR and LPDDR2 with BL8",$time);
      //$finish(2);
      while(1);
    }
    //MR01: Device feature 1
    value_temp = 0x80f00013;
    value_temp |= (bl == DRAM_BL4  ? 0x2:
                   bl == DRAM_BL8  ? 0x3:
                   bl == DRAM_BL16 ? 0x4: 0x3)<<12;
    value_temp |= DRAM_BURST_TYPE<<15;
    value_temp |= DRAM_BURST_WRAP<<16;
    value_temp |= 4<<17; //nWR=6 for Auto precharge
    REG32(UMCTL_CFG_MCMD) = value_temp;
    do  value_temp = REG32(UMCTL_CFG_MCMD);
    while((value_temp&0x80000000) != 0x0);

    //$display("%t,lpddr2 initialization: Device Feature 2...",$time);
    //MR02: Device feature 2
    REG32(UMCTL_CFG_MCMD) = 0x80f04023;
    do  value_temp = REG32(UMCTL_CFG_MCMD);
    while((value_temp&0x80000000) != 0x0);

    //$display("%t,lpddr2 initialization: set I/O DS...",$time);
    //MR03: I/O config-1. DS: 48 ohm typical (default)
    //value_temp = 0x80f02030;
    value_temp = 0x80f00033;
	//value_temp |= (0x1<<12); //34ohm
	value_temp |= (0x2<<12); //40ohm
	//value_temp |= (0x3<<12); //48ohm
	//value_temp |= (0x4<<12); //60ohm
	//value_temp |= (0x6<<12); //80
    REG32(UMCTL_CFG_MCMD) = value_temp;
    do  value_temp = REG32(UMCTL_CFG_MCMD);
    while((value_temp&0x80000000) != 0x0);

    //$display("%t,lpddr2 initialization: refresh...",$time);
    //refresh
    REG32(UMCTL_CFG_MCMD) = 0x80f00002;
    do  value_temp = REG32(UMCTL_CFG_MCMD);
    while((value_temp&0x80000000) != 0x0);
#endif

}
void EMC_DFI_Set(DRAM_INFO_T_PTR dram_info)
{
	volatile uint32 temp = 0;
    //REG32(UMCTL_CFG_DFITCTRLDELAY) = 0x2; //fixed value,see PUBL P143,default value,don't need to set
    //REG32(UMCTL_CFG_DFITPHYWRDATA) = 0x1; //fixed value,see PUBL P143,default value,don't need to set

	#if 0//must check here!!!!!!!!!!!!!!!!!!!
	if(((dram_info->mode_info->mem_type)&DRAM_LPDDR2) == DRAM_LPDDR2)
    {
       	REG32(UMCTL_CFG_DFITPHYWRLAT)  = 3;
    	REG32(UMCTL_CFG_DFITRDDATAEN)  = 5;

	}
	else
	#endif	
	{
        	if((dram_info->mode_info->mem_type&DRAM_LPDDR2) == DRAM_LPDDR2)
        	{
            	REG32(UMCTL_CFG_DFITPHYWRLAT)  = dram_info->mode_info->wl;
            	REG32(UMCTL_CFG_DFITRDDATAEN)  = dram_info->mode_info->rl-1;
        	}    	
         else
         {
            	REG32(UMCTL_CFG_DFITPHYWRLAT)  = (dram_info->mode_info->wl>=1)? (dram_info->mode_info->wl-1):0; //WL-1, see PUBL P143
            	REG32(UMCTL_CFG_DFITRDDATAEN)  = (dram_info->mode_info->rl>=2)? (dram_info->mode_info->rl-2):0; //RL-2, see PUBL P143
         }
        
	}		
    
    REG32(UMCTL_CFG_DFITPHYRDLAT)  = 0xf; //fixed value,see PUBL P143
//    REG32(UMCTL_CFG_DFITPHYUPDTYPE0)  = 0x1; //fixed value,see PUBL P143
//    REG32(UMCTL_CFG_DFITPHYUPDTYPE1)  = 0x1; //fixed value,see PUBL P143
//    REG32(UMCTL_CFG_DFITPHYUPDTYPE2)  = 0x1; //fixed value,see PUBL P143
//    REG32(UMCTL_CFG_DFITPHYUPDTYPE3)  = 0x1; //fixed value,see PUBL P143
//    REG32(UMCTL_CFG_DFITCTRLUPDMIN)   = 0x10;//???
//    REG32(UMCTL_CFG_DFITCTRLUPDMAX)   = 0x10;//???
//    REG32(UMCTL_CFG_DFITCTRLUPDDLY)   = 0x10;//???
//    REG32(UMCTL_CFG_DFIUPDCFG)   		= 0x3;//???
//    REG32(UMCTL_CFG_DFITREFMSKI)   	= 0x0;//???
//    REG32(UMCTL_CFG_DFITCTRLUPDI)   	= 0x0;//???
    REG32(UMCTL_CFG_DFISTCFG0) 	  = 0x7;
    REG32(UMCTL_CFG_DFISTCFG1) 	  = 0x3;
    REG32(UMCTL_CFG_DFISTCFG2) 	  = 0x3;	
//	REG32(UMCTL_CFG_DFITDRAMCLKEN)= 0x2; //fixed value,see PUBL P143
//	REG32(UMCTL_CFG_DFITDRAMCLKDIS)= 0x2; //fixed value,see PUBL P143

    REG32(UMCTL_CFG_DFILPCFG0) 	  = 0x00078101;				


	do temp = REG32(UMCTL_CFG_DFISTSTAT0);
	while((temp&0x1) == 0);

}


void EMC_PHY_Timing_Set(CLK_TYPE_E emc_clk,DRAM_INFO_T_PTR dram_info)
{
	uint32 mem_t = 0;

	mem_t = (1000000000/emc_clk);

		
    //PTR0, to set tDLLSRST, tDLLLOCK, tITMSRST
    {
		//DLL Soft Reset Time: Number of controller clock cycles that the DLL soft reset pin
		//must remain asserted when the soft reset is triggered through the PHY Initialization
		//Register (PIR). This must correspond to a value that is equal to or more than 50ns
		//or 8 controller clock cycles, whichever is bigger
    	uint32 tDLLSRST = 50; //ns
    	uint32 tDLLSRST_T = ((tDLLSRST/mem_t +1<8)? 8:(tDLLSRST/mem_t +1));
				
		//DLL Lock Time: Number of clock cycles for the DLL to stabilize and lock, i.e. number
		//of clock cycles from when the DLL reset pin is de-asserted to when the DLL has
		//locked and is ready for use. Refer to the PHY databook for the DLL lock time.
		//Default value corresponds to 5.12us at 533MHz.		
    	uint32 tDLLLOCK = 5120;//ns
    	uint32 tDLLLOCK_T = tDLLLOCK/mem_t;

		//ITM Soft Reset Time: Number of controller clock cycles that the ITM soft reset pin
		//must remain asserted when the soft reset is applied to the ITMs. This must
		//correspond to a value that is equal to or more than 8 controller clock cycles. Default
		//value corresponds to 8 controller clock cycles
    	uint32 tITMSRST = 8;//CLK
	    REG32(PUBL_CFG_PTR0) = (tITMSRST<<18)|(tDLLLOCK_T<<6)|tDLLSRST_T;
	}
	
    //PTR1, to set tINT0, tINT1
    {
    	    uint32 tINT0 = 0;
    	    uint32 tINT0_T = 0;
		uint32 tINT1 = 0;
		uint32 tINT1_T = 0;
		
        	tINT0 = 200*1000; //ns, CKE high time to first command,lpddr2
        	#ifndef EMC_SMALL_CODE_SIZE
        	if(dram_info->mode_info->mem_type==DRAM_DDR3)
    	    {
    			tINT0 = 500*1000;
    	    }
         #endif    			
        	tINT0_T = tINT0/mem_t;

		tINT1 = 100;	//ns, CKE low time with power and clock stable
		#ifndef EMC_SMALL_CODE_SIZE
        	if(dram_info->mode_info->mem_type==DRAM_DDR3)
        	{
    			tINT1 = 360;
        	}		
         #endif
		tINT1_T = tINT1/mem_t;
		
		REG32(PUBL_CFG_PTR1 ) = tINT1_T<<19 | tINT0_T;
		
    }
	
    //PTR2, to set tINT2,tINT3
    {
    	uint32 tINT2 = 0;
    	uint32 tINT2_T = 0;
		uint32 tINT3 = 0;
		uint32 tINT3_T = 0;
	
		tINT2 = 11*1000;	//ns, time for reset command to end of auto initialization
		#ifndef EMC_SMALL_CODE_SIZE
		if(dram_info->mode_info->mem_type==DRAM_DDR3)
		{
			tINT2 = 200*1000;
		}
        #endif
		tINT2_T = tINT2/mem_t;
		

		tINT3 = 1000;	//ns, time for ZQ initialization command to first command
		tINT3_T = tINT3/mem_t;		    	
    
    	REG32(PUBL_CFG_PTR2 ) = tINT3_T<<17 | tINT2_T;
    }
	
	//DTPR0, to set tMRD,tRTP,tWTR,tRP,tRCD,tRAS,tRRD,tRC,tCCD,
	//only used in PUBL DCU unite,I suppose
	if(dram_info->mode_info->mem_type==DRAM_LPDDR1)
	{ 
    	REG32(PUBL_CFG_DTPR0) = 0x3088444a;
	}
	else
	{
    	REG32(PUBL_CFG_DTPR0) = 0x36916a6d;
	}


	//DTPR1, to set tRFC,
	//only used in PUBL DCU unite,I suppose
	if(dram_info->mode_info->mem_type==DRAM_LPDDR1)
	{ 
    	REG32(PUBL_CFG_DTPR1) &= ~0xff0003;
		REG32(PUBL_CFG_DTPR1) |=  0x200001;
	}
	else
	{
    	REG32(PUBL_CFG_DTPR1) = 0x193400a0;
	}
	
	//DTPR2, to set tXS,tXP,tCKE,tDLLK
	//only used in PUBL DCU unite,I suppose
	//don't need to set,use the default value
	
}

void EMC_PHY_Mode_Set(DRAM_INFO_T_PTR dram_info)
{
	volatile uint32 temp = 0;
	DRAM_TYPE_E mem_type = dram_info->mode_info->mem_type;
	DRAM_BL_E bl = dram_info->mode_info->bl;
	uint32 cs_num = dram_info->mode_info->cs_num;	

	//ZQ0CR0
	if(mem_type == DRAM_LPDDR1)
	{
		//when lpddr1,zq power down, override,0xc: 48ohm typical,refer to P155 of multiPHY databook	
	    REG32(PUBL_CFG_ZQ0CR0) = (1<<31)|(1<<28)|(LPDDR1_MEM_DS<<5)|(LPDDR1_MEM_DS); 
	}

	//ZQ0CR1
	REG32(PUBL_CFG_ZQ0CR1) &= ~0xff; 	//diable On-die termination impedance calibration
	REG32(PUBL_CFG_ZQ0CR1) |= LPDDR2_MEM_DS;
	
	//PGCR
    temp = REG32(PUBL_CFG_PGCR);
	temp &= ~0x380007;
	temp |= ((mem_type==DRAM_LPDDR1)? 1:0);
	temp |= (1<<1); //dqs gating mode, 0:active windows mode 1:passive windows mode
	temp |= (cs_num ==2)? (0x3<<18):(0x1<<18);
//	temp |= (0x3<<18);
	REG32(PUBL_CFG_PGCR) = temp;

	//DXnDLLCR
	if(B0_SDLL_PHS_DLY != SDLL_PHS_DLY_DEF)
	{
		set_reg_fld(PUBL_CFG_DX0DLLCR,14,4,B0_SDLL_PHS_DLY);
	}
	if(B1_SDLL_PHS_DLY != SDLL_PHS_DLY_DEF)
	{
		set_reg_fld(PUBL_CFG_DX1DLLCR,14,4,B1_SDLL_PHS_DLY);
	}	
	if(B2_SDLL_PHS_DLY != SDLL_PHS_DLY_DEF)
	{	
		set_reg_fld(PUBL_CFG_DX2DLLCR,14,4,B2_SDLL_PHS_DLY);
	}	
	if(B3_SDLL_PHS_DLY != SDLL_PHS_DLY_DEF)
	{	
		set_reg_fld(PUBL_CFG_DX3DLLCR,14,4,B3_SDLL_PHS_DLY);
	}
	
	//DXnDLLCR
	if(B0_DQS_STEP_DLY != DQS_STEP_DLY_DEF)
	{
		set_reg_fld(PUBL_CFG_DX0DQSTR,20,3,B0_DQS_STEP_DLY);
		set_reg_fld(PUBL_CFG_DX0DQSTR,23,3,B0_DQS_STEP_DLY);
	}	
	if(B1_DQS_STEP_DLY != DQS_STEP_DLY_DEF)
	{
		set_reg_fld(PUBL_CFG_DX1DQSTR,20,3,B1_DQS_STEP_DLY);
		set_reg_fld(PUBL_CFG_DX1DQSTR,23,3,B1_DQS_STEP_DLY);
	}	
	if(B2_DQS_STEP_DLY != DQS_STEP_DLY_DEF)
	{	
		set_reg_fld(PUBL_CFG_DX2DQSTR,20,3,B2_DQS_STEP_DLY);
		set_reg_fld(PUBL_CFG_DX2DQSTR,23,3,B2_DQS_STEP_DLY);
	}		
	if(B3_DQS_STEP_DLY != DQS_STEP_DLY_DEF)
	{
		set_reg_fld(PUBL_CFG_DX3DQSTR,20,3,B3_DQS_STEP_DLY);	
		set_reg_fld(PUBL_CFG_DX3DQSTR,23,3,B3_DQS_STEP_DLY);		
	}
	
	//DLLGCR
	//???don't need to set

	//ACDLLCR
	//???don't need to set

	//ACIOCR
	if(mem_type==DRAM_LPDDR1)
	{
		REG32(PUBL_CFG_ACIOCR) |= 1; //aciom
	}
	else
	{
		REG32(PUBL_CFG_ACIOCR) &= ~1; //aciom
	}
		
	//DXCCR, DATX8 common configuration register,to set data io,qds pin mode and pullup/pulldown resister
	REG32(PUBL_CFG_DXCCR) &= ~((1<<14)|(0xff<<4)|(1<<1));
	REG32(PUBL_CFG_DXCCR) = ((mem_type==DRAM_LPDDR1 ? 0x1:0x0)<<1)  	|	//iom,0:SSTL mode 1:CMOS mode
							(   DQS_PDU_RES<<4)  	|	//dqs resistor,0:open 1:688ohm 2:611ohm 3:550ohm 4:500ohm 5:458ohm 6:393ohm 7:344ohm
							((8|DQS_PDU_RES)<<8)    |	//dqs# resistor,same as dqs resistor
							((mem_type==DRAM_LPDDR1) ? 1:0<<14); 			//dqs# reset,see PUBL page61 for detials

	//DSGCR
    temp = REG32(PUBL_CFG_DSGCR);
    temp &= ~0xfff; // only applicable for LPDDR    
    temp |= (0xB|(DQS_GATE_EARLY<<8)|(DQS_GATE_EXTEN<<5)); 
    REG32(PUBL_CFG_DSGCR) = temp;
	
	//DCR
	{	
		uint32 ddr_mode = 0;
		uint32 ddr_8bank = 0;
		uint32 ddr_type = 0;
		DRAM_TYPE_E mem_type = dram_info->mode_info->mem_type;

		if(mem_type==DRAM_LPDDR1)
		{
			ddr_mode = 0;
		}
		else if(mem_type&DRAM_LPDDR2 == DRAM_LPDDR2)
		{
			ddr_mode = 4;
		}
		else
		{
			ddr_mode = 3;
		}

		ddr_8bank = (dram_info->mode_info->bank_num==8)?1:0;
		ddr_type  = (mem_type ==DRAM_LPDDR2_S4)? 0:1;

		REG32(PUBL_CFG_DCR) &= ~0x3ff;
		REG32(PUBL_CFG_DCR) |= (ddr_mode|(ddr_8bank<<1)|(ddr_type<<8));
	}

	//DXnGCR,DATX8 General Configuration Register
    REG32(PUBL_CFG_DX0GCR) &= ~(0x3<<9);//disable DQ/DQS Dynamic RTT Control
    REG32(PUBL_CFG_DX1GCR) &= ~(0x3<<9);//disable DQ/DQS Dynamic RTT Control    
    REG32(PUBL_CFG_DX2GCR) &= ~(0x3<<9);//disable DQ/DQS Dynamic RTT Control    
    REG32(PUBL_CFG_DX3GCR) &= ~(0x3<<9);//disable DQ/DQS Dynamic RTT Control        

    REG32(PUBL_CFG_ODTCR) &= ~0xff00ff;//disable ODT both for read and write

	if(mem_type!=DRAM_LPDDR1)
	{
		//trigger zqcl
		wait_pclk(50);
		REG32(PUBL_CFG_PIR) = 0x9; 
		wait_pclk(50);
		//wait trigger zqcl done
		do temp = REG32(PUBL_CFG_PGSR);
		while((temp&0x1) == 0);
	}	

	//Controller DRAM Initialization
	wait_pclk(50);
	REG32(PUBL_CFG_PIR) = 0x40001; 
	wait_pclk(50);    
	//wait done
	do {temp = REG32(PUBL_CFG_PGSR);}
	while((temp&0x1) == 0);
	
}


void EMC_PHY_MDR_Set(CLK_TYPE_E emc_clk,DRAM_INFO_T_PTR dram_info)
{
#ifndef CONFIG_MEM_LPDDR1
	uint32 temp = 0;
	uint32 mem_t = 0;

	mem_t = (1000000000/emc_clk);

	switch(dram_info->mode_info->mem_type)
	{
		case DRAM_LPDDR1:
		{
		    //MR0
		    //lpddr1 mode register
	    	temp = REG32(PUBL_CFG_MR0);	
			temp &= ~0XFF;
			temp |=((0<<7)							|	//operation mode, 0:normal_mode 1:test_mode
					(dram_info->mode_info->rl<<4)	|	//cas latency	
					(0<<3)							|	//burst type,0:sequential 1:interleaved
					(dram_info->mode_info->bl+1));	  		  	  		   		  
		    REG32(PUBL_CFG_MR0) = temp;		

			//MR2
			//lpddr1 extended mode register
			REG32(PUBL_CFG_MR2) = 0;
	    }break;
		
		case DRAM_LPDDR2_S2:
		case DRAM_LPDDR2_S4:
		{
			//MR0
			//lpddr2 don't use
			REG32(PUBL_CFG_MR0) = 0;			
			
			//MR1
			//lpddr2 mode register 1
		    temp = REG32(PUBL_CFG_MR1);
	        temp &= ~0xff;
	        temp |= ((0x4<<5)|		//write recovery,1:3 2:4 3:5 4:6 5:7 6:8,???fixed?????have some problem i think
					 (0x0<<4)|		//wrap control, 0:wrap 1:no_wrap
					 (0x0<<3)|		//burst type, 0:sequential 1:interleaved
					 dram_info->mode_info->bl+1);    
			REG32(PUBL_CFG_MR1) = temp;
			//MR2
			//lpddr2 mode register 2
			if(dram_info->mode_info->rl==3 &&dram_info->mode_info->wl==1)
			{
				REG32(PUBL_CFG_MR2) = 1;
			}
			else if(dram_info->mode_info->rl==4 &&dram_info->mode_info->wl==2)
			{
				REG32(PUBL_CFG_MR2) = 2;
			}
			else if(dram_info->mode_info->rl==5 &&dram_info->mode_info->wl==2)
			{
				REG32(PUBL_CFG_MR2) = 3;
			}
			else if(dram_info->mode_info->rl==6 &&dram_info->mode_info->wl==3)
			{
				REG32(PUBL_CFG_MR2) = 4;
			}
			else if(dram_info->mode_info->rl==7 &&dram_info->mode_info->wl==4)
			{
				REG32(PUBL_CFG_MR2) = 5;
			}
			else if(dram_info->mode_info->rl==8 &&dram_info->mode_info->wl==4)
			{
				REG32(PUBL_CFG_MR2) = 6;
			}
			else
			{
				REG32(PUBL_CFG_MR2) = 4;
			}

			//MR3
			//lpddr2 mode register 3
			REG32(PUBL_CFG_MR3) = 2;// 1=34.3-ohm, 2=40-ohm, 3=48-ohm, 4=60-ohm, 6=80-ohm, 7=120-ohm
		}break;
		case DRAM_DDR3:
		{
			//MR0
			//ddr3 mode register 0
			{
				uint32 bl=0;
				uint32 cl=1;
				uint32 wr=1;
				bl = (dram_info->mode_info->bl == DRAM_BL4)?2:0;
				cl = (dram_info->mode_info->rl <5)?1:(dram_info->mode_info->rl-4);
				switch(dram_info->time_info->tWR/mem_t+1)
				{
					case 5: wr=1;break;	
					case 6: wr=2;break;
					case 7: wr=3;break;				
					case 8: wr=4;break;								
					case 10: wr=5;break;
					case 12: wr=6;break;
					default: wr=1;
				}
					
			    REG32(PUBL_CFG_MR0) = bl 	|
										(0<<3)	| // Burst Type: 0:sequentia  1:interleaved
										(cl<<4)	|
										(wr<<9) |
										(1<<12);  // Power-Down Control, 0 = Slow exit (DLL off),1 = Fast exit (DLL on)
			}			
			
			//MR1
			//ddr3 mode register 1
		    REG32(PUBL_CFG_MR1) = 0x81;	//bit0:DLL Enable/Disable  bit7:Write Leveling Enable

			//MR2
			//ddr3 mode register 2
			REG32(PUBL_CFG_MR2) = 0x4;	//bit6:Auto Self-Refresh 1:eanble 0:disable

			//MR3
			//ddr3 mode register 3
			REG32(PUBL_CFG_MR3) = 0x0;	//bit0~1:Multi-Purpose Register (MPR) Location
											//bit2:  Multi-Purpose Register Enable
		}break;

		default: break;
	}
#endif	
}

BOOLEAN EMC_PHY_Training()
{
	uint32 i = 0;
	
	//while((PHY_CURRENT_STATE&PHY_STATE_INIT_DONE)    !=PHY_STATE_INIT_DONE);
	//while((PHY_CURRENT_STATE&PHY_STATE_DLL_LOCK_DONE)!=PHY_STATE_DLL_LOCK_DONE);
	//for(i = 0; i < 1000; i++);	

	//set umctl do memory init, phy wouldn't do
	//REG32(PUBL_CFG_PIR) |= (PHY_ACT_INIT|PHY_ACT_CTLDINIT);
	//while((PHY_CURRENT_STATE&PHY_STATE_INIT_DONE) !=PHY_STATE_INIT_DONE);
	//for(i = 0; i < 1000; i++);	
	
	//do dqs training
	wait_pclk(50);	
	REG32(PUBL_CFG_PIR) |= (PHY_ACT_INIT|PHY_ACT_DQSTRN);
	wait_pclk(50);	

	//wait PHY dqs training finished
	while((PHY_CURRENT_STATE&PHY_STATE_INIT_DONE) !=PHY_STATE_INIT_DONE);
	while((PHY_CURRENT_STATE&PHY_STATE_DTDONE)	  !=PHY_STATE_DTDONE);
	wait_us(50);	

	if((PHY_CURRENT_STATE&PHY_STATE_DTERR)||
   	   (PHY_CURRENT_STATE&PHY_STATE_DTIERR))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}		
}



void __emc_low_power_set()
{
	uint32 i = 0;
	//disable emc hardware low power enable
	i = REG32(0x20900308);
	i &= ~(1 << 8);
	REG32(0x20900308) = i;
	REG32(UMCTL_CFG_SCFG) = 0x00000421;
}

void EMC_Common_Reg_Set(CLK_TYPE_E emc_clk,EMC_CHN_INFO_T_PTR emc_chn_info,DRAM_INFO_T_PTR dram_info)
{
	EMC_CTL_Mode_Set(dram_info);
	EMC_CTL_CHN_Set(emc_chn_info);		
	
	EMC_MEM_Mode_set(dram_info);
	EMC_MEM_Timing_Set(emc_clk,dram_info);
	
	EMC_DFI_Set(dram_info);
	
	EMC_PHY_Mode_Set(dram_info);
	EMC_PHY_Timing_Set(emc_clk,dram_info);
	EMC_PHY_MDR_Set(emc_clk,dram_info);
}

void __clr_ddr_phy_ret_flg()
{
	REG32(GR_DDR_PHY_RETENTION_SET) = 0x2;
}

uint32 __reorder_mem_info(uint32 source_data)
{
	uint32 temp = 0;
	uint32 reg_info = 0;

	temp = source_data&0xff;
	//reorder mdr info which get from lpddr2 by mrr cmd
	//          bit7(d2)         bit6(d1)         bit5(d0)          bit4(d3)          bit3(d5)          bit2(d4)          bit1(d6)          bit0(d7) 
	reg_info = ((temp&BIT_2)<<5)+((temp&BIT_1)<<5)+((temp&BIT_0)<<5)+((temp&BIT_3)<<1)+((temp&BIT_5)>>2)+((temp&BIT_4)>>2)+((temp&BIT_6)>>5)+((temp&BIT_7)>>7);
	
	return reg_info;
}

DRAM_DENSITY_E __cal_mem_dsy(uint32 dsy_val)
{
	switch(dsy_val)
	{
		case 0: return DRAM_64MBIT;
		case 1: return DRAM_128MBIT;
		case 2: return DRAM_256MBIT;
		case 3: return DRAM_512MBIT;
		case 4: return DRAM_1GBIT;
		case 5: return DRAM_2GBIT;
		case 6: return DRAM_4GBIT;
		case 7: return DRAM_8GBIT;
		case 8: return DRAM_16GBIT;
		default: DRAM_4GBIT;
	}
}

BOOLEAN __detect_mem_info(DRAM_INFO_T_PTR dram_info)
{
	uint32 cs0_mdr_info = 0;
	uint32 cs1_mdr_info = 0;
	if(__is_bond_lpddr2())
	{
		//read lpddr2 mdr8 to get io_width,density and type
		EMC_CTL_MDR_Issue(dram_info,SECOND_CS, CMD_MRR, 8);
		cs1_mdr_info = __reorder_mem_info(REG32(UMCTL_REG_MRRSTAT0));
		
		EMC_CTL_MDR_Issue(dram_info,FIRST_CS,  CMD_MRR, 8);				
		cs0_mdr_info = __reorder_mem_info(REG32(UMCTL_REG_MRRSTAT0));

		if(cs1_mdr_info != 0)
		{
			dram_info->mode_info->cs_num = TWO_CS;

			dram_info->mode_info->cs1_cap = __cal_mem_dsy((cs1_mdr_info&0x3c)>>2);			
		}
		else
		{
			dram_info->mode_info->cs_num = ONE_CS;

			dram_info->mode_info->cs1_cap = DRAM_0BIT;			
		}
		dram_info->mode_info->cs0_cap = __cal_mem_dsy((cs0_mdr_info&0x3c)>>2);

		//cal memory data io width
		if((cs0_mdr_info&0xc0) == 0)
		{
			dram_info->mode_info->io_width = IO_WIDTH_32;
		}
		else if((cs0_mdr_info&0xc0) == 0x40)
		{
			dram_info->mode_info->io_width = IO_WIDTH_16;
		}
		else  if((cs0_mdr_info&0xc0) == 0x80)
		{
			dram_info->mode_info->io_width = IO_WIDTH_8;
		}	
		else
		{
			dram_info->mode_info->io_width = IO_WIDTH_32;
		}

		//cal memmory type
		if((cs0_mdr_info&0x3) == 0)
		{
			dram_info->mode_info->mem_type == DRAM_LPDDR2_S4;
		}
		else if((cs0_mdr_info&0x3) == 1)
		{
			dram_info->mode_info->mem_type == DRAM_LPDDR2_S2;
		}
		else
		{
			dram_info->mode_info->mem_type == DRAM_LPDDR2_S4;
		}				

        return TRUE;
	}
    else//note!!! don't support lpddr1 type detection now
    {
        return FALSE;
    }
}



BOOLEAN EMC_Init(CLK_TYPE_E emc_clk,EMC_CHN_INFO_T_PTR emc_chn_info,DRAM_INFO_T_PTR dram_info)
{
	__clr_ddr_phy_ret_flg();
	
	EMC_CTL_State_Move(CTL_STATE_INIT_MEM);

	EMC_Common_Reg_Set(emc_clk, emc_chn_info, dram_info);

	EMC_MEM_Power_Up();
		
	EMC_MEM_Init(dram_info);

    if((dram_info->mode_info->mem_type&DRAM_LPDDR2) == DRAM_LPDDR2)
    {
        	__detect_mem_info(dram_info);

        	EMC_Common_Reg_Set(emc_clk, emc_chn_info, dram_info);
    }
    
	EMC_CTL_State_Move(CTL_STATE_CONFIG);

    if((dram_info->mode_info->mem_type&DRAM_LPDDR2) == DRAM_LPDDR2)
    {
        	if(!EMC_PHY_Training())
        	{
        		while(1);
        	}
    }
	__emc_low_power_set();

	EMC_CTL_State_Move(CTL_STATE_ACCESS);
	
}
/**---------------------------------------------------------------------------*
 **                     Static Function Prototypes                            *
 **---------------------------------------------------------------------------*/

PUBLIC void DMC_Dev_Init(CLK_TYPE_E emc_clk)
{
	DRAM_INFO_T_PTR dram_info;
	char* dram_chip_name = NULL;

    
    ADI_init();
    
	if(__is_bond_lpddr2())
	{
		dram_chip_name ="NORMAL_LPDDR2_1CS_4G_32BIT";
        emc_clk = CLK_400MHZ;
	}
	else
	{
	    dram_chip_name ="NORMAL_LPDDR1_2CS_4G_32BIT";
        emc_clk = CLK_200MHZ;
	}	
		
	dram_info = get_dram_info(dram_chip_name);

	set_emc_clk(emc_clk);	
		
	EMC_Init(emc_clk, EMC_CHN_INFO_ARRAY,dram_info); 

}

#ifdef   __cplusplus
    }
#endif


 
