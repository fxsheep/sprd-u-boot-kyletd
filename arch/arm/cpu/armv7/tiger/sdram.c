/******************************************************************************
    David.Jia   2007.10.29      share_version_union

    TCC -fpu None -O2 -bi -g+ -apcs /interwork -D__RUN_IN_SDRAM sdram_init.c
SC6600R    -D_BL_NF_SC6600R_
SC6600H    -D_BL_NF_SC6600H_
SC6600I    -D_BL_NF_SC6600I_
SC6800     -gtp -cpu ARM926EJ-S -D_REF_SC6800_ -D_BL_NF_SC6800_
******************************************************************************/

#include <common.h>
#include <asm/arch/sci_types.h>
#include <asm/arch/arm_reg.h>
#include <asm/arch/sdram_cfg.h>
#include <asm/arch/chng_freq.h>
#include <asm/arch/sc_reg.h>
#include <asm/arch/sdram.h>
#include <asm/arch/chip.h>

/*lint -e760 -e547 ,because pclint error e63 e26 with REG32()*/
#define REG32(x)   (*((volatile uint32 *)(x)))
/*lint +e760 +e547 ,because pclint error e63 e26 with REG32()*/

/*lint -e765*/

#define  SDRAM_EXT_INVALID     0xffffffff       //@David.Jia 2008.1.7
#define  ROW_MODE_TO_NUM(_m)	(_m+11)

uint32 single_chk(uint32 addr, uint32 data)
{
	REG32(addr) = data;
	
	if(REG32(addr) == data)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint32 ddr_rw_chk(uint32 addr)
{
	uint32 i    = 0;
	uint32 data = 0;
	
	for(i = 0; i < 8; i++)
	{
		switch(i)
		{
			case 0: data = 0x00000000; break;
			case 1: data = 0xffffffff; break;
			case 2: data = 0x55555555; break;
			case 3: data = 0xaaaaaaaa; break;
			case 4: data = 0x01234567; break;
			case 5: data = 0x76543210; break;
			case 6: data = 0x89abcdef; break;
			case 7: data = 0xfedcba98; break;
		}
		
		if(single_chk(addr,    data) == 0 ||
		   single_chk(addr+0x4,data) == 0 ||
		   single_chk(addr+0x8,data) == 0 ||
		   single_chk(addr+0xc,data) == 0)
		{
			return 0;
		}
	}

	return 1;
}

uint32 dq_training(uint32 addr)
{
	const uint32 TR_MAX = 16;
	uint32 i = 0;
	uint32 tr0,tr1,tr2,tr3;

	for(tr0 = 0; tr0 < TR_MAX; tr0++)
	{
		PUBL_REG(PUBL_CFG_DX0DQSTR) = tr0;
		for(tr1 = 0; tr1 < TR_MAX; tr1++)
		{
			PUBL_REG(PUBL_CFG_DX1DQSTR) = tr1;
			for(tr2 = 0; tr2 < TR_MAX; tr2++)
			{
				PUBL_REG(PUBL_CFG_DX2DQSTR) = tr2;
        			for(tr3 = 0; tr3 < TR_MAX; tr3++)
				{
					PUBL_REG(PUBL_CFG_DX3DQSTR) = tr3;
					if(ddr_rw_chk(addr))
					{
						REG32(EMC_MEM_BASE_ADDR+i*4)=(0xBA550000|(tr3<<12)|(tr2<<8)|(tr1<<4)|tr0);
						return 1;
					}
					else
					{
					
						REG32(EMC_MEM_BASE_ADDR+i*4)=(0xFA110000|(tr3<<12)|(tr2<<8)|(tr1<<4)|tr0);
					}
					i++;
				}
			}
		}
	}

	//if not found correct dqs training value, set as default value;
	PUBL_REG(PUBL_CFG_DX0DQSTR)	= 0xaa;
	PUBL_REG(PUBL_CFG_DX1DQSTR)	= 0xaa;
	PUBL_REG(PUBL_CFG_DX2DQSTR)	= 0xaa;
	PUBL_REG(PUBL_CFG_DX3DQSTR)	= 0xaa;
    return 0;
}



void write_upctl_state_cmd(UPCTL_CMD_E cmd)
{
	UMCTL_REG(UMCTL_CFG_SCTL) = cmd;
}

void poll_upctl_state(UPCTL_STATE_E state)
{
	UPCTL_STATE_E state_poll;

	do{state_poll = REG32(UMCTL_REG_BASE + UMCTL_CFG_STAT);}
	while(state_poll != state);
}
void move_upctl_state_to_access(void)
{
	UPCTL_STATE_E upctl_state;
	upctl_state = UMCTL_REG(UMCTL_CFG_STAT);

	while(upctl_state != STATE_ACCESS)
	{
		switch(upctl_state)
		{
			case STATE_CONFIG:
			{
				write_upctl_state_cmd(CMD_GO);
				poll_upctl_state(STATE_ACCESS);
				upctl_state = UMCTL_REG(UMCTL_CFG_STAT); 		//hs add
				break;
			}
			case STATE_INIT_MEM:
			{
				write_upctl_state_cmd(CMD_CFG);
				poll_upctl_state(STATE_CONFIG);
				upctl_state = STATE_CONFIG;
				break;
			}
			case STATE_LOW_POWER:
			{
				write_upctl_state_cmd(CMD_WAKEUP);
				poll_upctl_state(STATE_ACCESS);
				upctl_state = UMCTL_REG(UMCTL_CFG_STAT);		//hs add
				break;
			}
			default:
			{				
	                 	upctl_state = UMCTL_REG(UMCTL_CFG_STAT);
				break;
			}

		}
	}
}

void DMC_Init(uint32 lpddr_type)
{
	uint32 value_temp,i;

	//memory timing configuration registers
	UMCTL_REG(UMCTL_CFG_SCFG) = 0x00000420;
	//UMCTL_REG(UMCTL_CFG_MCFG) = (lpddr_type<<22)|(DRAM_BL<<20)|(DRAM_TYPE<<6);
	UMCTL_REG(UMCTL_CFG_MCFG) = 0x00A60041;   //hs change

	//memory basic timing configuration	
	UMCTL_REG(UMCTL_CFG_TOGCNT1U) 	= 0x23;
	UMCTL_REG(UMCTL_CFG_TINIT) 		= 0x01;
	UMCTL_REG(UMCTL_CFG_TRSTH) 		= 0x00;
	UMCTL_REG(UMCTL_CFG_TOGCNT100N) = 0x28;
	UMCTL_REG(UMCTL_CFG_TREFI) 	= 0x4e;
	UMCTL_REG(UMCTL_CFG_TMRD) 	= 0x05;
	UMCTL_REG(UMCTL_CFG_TRFC) 	= 0x34;
	UMCTL_REG(UMCTL_CFG_TRP) 	= (lpddr_type==LPDDR2_SDRAM) ? 0x0001000A:0x00000004;
	UMCTL_REG(UMCTL_CFG_TRTW) 	= 0x03;
	UMCTL_REG(UMCTL_CFG_TAL) 	= 0x00;
	UMCTL_REG(UMCTL_CFG_TCL) 	= (lpddr_type==LPDDR2_SDRAM) ? 0x06:0x03; 
	UMCTL_REG(UMCTL_CFG_TCWL) 	= (lpddr_type==LPDDR2_SDRAM) ? 0x03:0x01;
	UMCTL_REG(UMCTL_CFG_TRAS) 	= (lpddr_type==LPDDR2_SDRAM) ? 0x11:0x08;
	UMCTL_REG(UMCTL_CFG_TRC) 	= (lpddr_type==LPDDR2_SDRAM) ? 0x1b:0x0c;
	UMCTL_REG(UMCTL_CFG_TRCD) 	= (lpddr_type==LPDDR2_SDRAM) ? 0x06:0x04;
	UMCTL_REG(UMCTL_CFG_TRRD) 	= (lpddr_type==LPDDR2_SDRAM) ? 0x04:0x02;
	UMCTL_REG(UMCTL_CFG_TRTP) 	= (lpddr_type==LPDDR2_SDRAM) ? 0x03:0x01;
	UMCTL_REG(UMCTL_CFG_TWR) 	= (lpddr_type==LPDDR2_SDRAM) ? 0x06:0x03;

	//xiaohui change from 3 to 4 due to tWTR=7.5ns whereas clk_emc=2.348ns
	UMCTL_REG(UMCTL_CFG_TWTR)	= (lpddr_type==LPDDR2_SDRAM) ? 0x04:0x01; 
	UMCTL_REG(UMCTL_CFG_TEXSR)	= 0x38;
	UMCTL_REG(UMCTL_CFG_TXP)	= 0x04;
	UMCTL_REG(UMCTL_CFG_TXPDLL)	= 0x00;

	//xiaohui change from 0x24 to 0x27 due to tZQCS=90ns whereas clk_emc=2.348ns
	UMCTL_REG(UMCTL_CFG_TZQCS)	= 0x27;
	UMCTL_REG(UMCTL_CFG_TZQCSI)	= 0x05;
	UMCTL_REG(UMCTL_CFG_TDQS)	= 0x01;
	UMCTL_REG(UMCTL_CFG_TCKSRE)	= 0x00;
	UMCTL_REG(UMCTL_CFG_TCKSRX)	= 0x02;
	UMCTL_REG(UMCTL_CFG_TCKE)	= 0x03;
	UMCTL_REG(UMCTL_CFG_TMOD)	= 0x00;
	UMCTL_REG(UMCTL_CFG_TRSTL) 	= 0x02;
	UMCTL_REG(UMCTL_CFG_TZQCL)	= 0x90;
	UMCTL_REG(UMCTL_CFG_TCKESR)	= 0x06; 
	UMCTL_REG(UMCTL_CFG_TDPD)	= 0x01; 

	//default value not compatible with real condition
	UMCTL_REG(UMCTL_CFG_DCFG)			=(DRAM_ADDR_MAP<<8)|(DRAM_TYPE<<6)|(DRAM_DENSITY<<2)|DRAM_IO_WIDTH;
	UMCTL_REG(UMCTL_CFG_DFITPHYWRDATA)	= 0X01;
	UMCTL_REG(UMCTL_CFG_DFITPHYWRLAT)	= (lpddr_type==LPDDR2_SDRAM) ? 3:0;
	UMCTL_REG(UMCTL_CFG_DFITRDDATAEN)	= (lpddr_type==LPDDR2_SDRAM) ? 5:1;
	UMCTL_REG(UMCTL_CFG_DFITPHYRDLAT)	= 0x0f;
	UMCTL_REG(UMCTL_CFG_DFISTCFG0)		= 0x07;
	UMCTL_REG(UMCTL_CFG_DFISTCFG1)		= 0x03;
	UMCTL_REG(UMCTL_CFG_DFISTCFG2)		= 0x03;
	UMCTL_REG(UMCTL_CFG_DFILPCFG0)		= 0X00070101;

	for(i = 0; i < 8; i++)
	{
		UMCTL_REG(UMCTL_CFG_PCFG0+i*4) |= 0X70; //st_fw_en,bp_rd_en,bp_wr_en
	}

	//hs add start
	UMCTL_REG(UMCTL_CFG_PCFG0)		= 0X50;
	UMCTL_REG(UMCTL_CFG_PCFG1)		= 0X50;
	UMCTL_REG(UMCTL_CFG_PCFG3)		= 0X50;
	UMCTL_REG(UMCTL_CFG_PCFG4)		= 0X50;
	UMCTL_REG(UMCTL_CFG_PCFG5)		= 0X50;
	UMCTL_REG(UMCTL_CFG_PCFG6)		= 0X50;
	//hs add end

	//===============================================================================
	//now,set the PHY registers
	PUBL_REG(PUBL_CFG_DTAR) = (0<<28)|(0<<12)|0;	
	PUBL_REG(PUBL_CFG_PTR0) = 0X0020051B;

	value_temp = PUBL_REG(PUBL_CFG_DCR);
	value_temp &= ~0x30f;
	value_temp |= (lpddr_type == LPDDR2_SDRAM) ? 0XC:0X0;
	value_temp |= ((DRAM_TYPE==0)?1:0)<<8;//lpddr2_s4???
	PUBL_REG(PUBL_CFG_DCR) = value_temp;

	//MR0
	value_temp = PUBL_REG(PUBL_CFG_MR0);
	if(lpddr_type == LPDDR1_SDRAM)
	{
		value_temp &= ~0xff;
		value_temp |= ((DRAM_CL<<4)|(DRAM_BT<<3)|DRAM_BL);
	}
	//PUBL_REG(PUBL_CFG_MR0) = value_temp;
	PUBL_REG(PUBL_CFG_MR0) = 0x00000A3B;	//hs modify	
	//MR1
	value_temp = PUBL_REG(PUBL_CFG_MR1);
	if(lpddr_type == LPDDR2_SDRAM)
	{
		value_temp &= ~0xff;
		value_temp |= ((0X3<<5)|(DRAM_WC<<4)|(DRAM_BT<<3)|DRAM_BL);
	}
	PUBL_REG(PUBL_CFG_MR1) = value_temp;

	//MR2
	if(lpddr_type == LPDDR2_SDRAM)
	{
		PUBL_REG(PUBL_CFG_MR2) = 0X4;
	}

	//MR3
	if(lpddr_type == LPDDR2_SDRAM)
	{
		value_temp = PUBL_REG(PUBL_CFG_MR3);
		value_temp &= ~0xf;
		value_temp |= 0x2;
		PUBL_REG(PUBL_CFG_MR3) = value_temp;
	}
	
	//DTPR0
	if(lpddr_type == LPDDR1_SDRAM)
	{
		//            tCCD   tRC       tRRD    tRAS   tRCD    tRP    tWTR   tRTP  tMRD
		value_temp = ((0<<31)|(12<<25)|(2<<21)|(8<<16)|(4<<12)|(4<<8)|(1<<5)|(2<<2)|2);
	}
	else if(lpddr_type == LPDDR2_SDRAM)
	{
		value_temp = 0x36916a6d;
	}
	PUBL_REG(PUBL_CFG_DTPR0) = value_temp;

	//DTPR1
	value_temp = PUBL_REG(PUBL_CFG_DTPR1);
	if(lpddr_type == LPDDR1_SDRAM)
	{
		value_temp &= ~(0x3);
		value_temp &= ~(0xff<<16);
		value_temp |= 0x1;
		value_temp |= 32<<16;
	}
	else if(lpddr_type == LPDDR2_SDRAM)
	{
		value_temp = 0x193400a0;
	}
	PUBL_REG(PUBL_CFG_DTPR1) = value_temp;

	//DTPR2
	if(lpddr_type == LPDDR2_SDRAM)
	{
		PUBL_REG(PUBL_CFG_DTPR2) = 0X1001A0C8;
	}

	//DXCCR
	value_temp = PUBL_REG(PUBL_CFG_DXCCR);
	value_temp &= ~0xff0;
	value_temp |= (lpddr_type == LPDDR2_SDRAM) ? 0XC40:0X0;
	PUBL_REG(PUBL_CFG_DXCCR) = value_temp;

	//PIR
	PUBL_REG(PUBL_CFG_PIR) = 0x00020000;
	
	//PGCR
	if(lpddr_type == LPDDR1_SDRAM)
	{
		PUBL_REG(PUBL_CFG_PGCR) = 0X018C2E03;
	}
	else
	{
		value_temp = PUBL_REG(PUBL_CFG_PGCR);
		value_temp &= ~0xc0006;
		value_temp |= 0xc0002;
		PUBL_REG(PUBL_CFG_PGCR) = value_temp;
	}	

	//PTR1
	PUBL_REG(PUBL_CFG_PTR1) = ((200*1000)/20)&0X7FFFF;
	//PTR2
	PUBL_REG(PUBL_CFG_PTR2) = ((200*1000)/20)&0X1FFFF;
	
	//waite for PHY initialization done and DLL lock done
	do{value_temp = PUBL_REG(PUBL_CFG_PGSR);}
	while((value_temp&0x3) != 0x3);

	//start memory power up sequence
	UMCTL_REG(UMCTL_CFG_POWCTL) = 0X1;
	do{value_temp = UMCTL_REG(0X44);}
	while((value_temp&0x1) != 0x0);

	//excute DRAM initialization sequence
	PUBL_REG(PUBL_CFG_PIR) = 0X00020041;
	for(i = 0; i <= 100; i++);

	//waite for initialize done
	do{value_temp = PUBL_REG(PUBL_CFG_PGSR);}
	while((value_temp&0x1) != 0x1);

	//waite for dll lock done
	do{value_temp = PUBL_REG(PUBL_CFG_PGSR);}
	while((value_temp&0x2) != 0x2);

	move_upctl_state_to_access();

	//manually configure the Q-valid window and phase
	PUBL_REG(PUBL_CFG_DX0DQSTR) = 0Xaa;
	PUBL_REG(PUBL_CFG_DX1DQSTR) = 0Xaa;
	PUBL_REG(PUBL_CFG_DX2DQSTR) = 0Xaa;
	PUBL_REG(PUBL_CFG_DX3DQSTR) = 0Xaa;

	if(lpddr_type == LPDDR1_SDRAM)
	{
		dq_training(EMC_MEM_BASE_ADDR);
	}
	
	for(i = 0; i < 1000; i++);
}


PUBLIC void Chip_Init (void) /*lint !e765 "Chip_Init" is used by init.s entry.s*/
{
    DMC_Init(LPDDR1_SDRAM);

}



















