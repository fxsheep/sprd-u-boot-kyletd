#include <asm/arch/sci_types.h>

#define REG32(x)              (*((volatile uint32 *)(x)))

#define MEM_CS0_TYPE          MEM_CS_4096MB
#if defined CONFIG_GARDA && defined CONFIG_SC8825
#define MEM_CS1_TYPE          MEM_CS_2048MB
#else
#define MEM_CS1_TYPE          MEM_CS_0MB          //caution: if CS1 is not available, modify it to MEM_CS_0Mb
#endif
//#define MEMORY_TYPE           LPDDR1
#define MEMORY_TYPE           LPDDR2   //typedef enum int {LPDDR2,LPDDR1,DDR3} MEM_TYPE_ENUM;

#define DQSR_VALUE            4
#define GATE_TRAING_ON        1
#define GATE_EARLY_LATE       2
#ifndef SC8825_EMC_FREQ
#define SC8825_EMC_FREQ       400   //ddr clock
#endif

#if (MEMORY_TYPE == LPDDR1)
#define SC8825_EMC_DRV        40
#else
#define SC8825_EMC_DRV        48
#endif

#define ZQ_CALIBRE_DPSLEEP    0x0
#define SOFTWARE_ENABLE_VREF  0

#define ANALOG_DIE_REG_BASE   0x42000600
#define ANALOG_DCDC_CTRL_CAL  0x50
#define ANALOG_DCDC_CTRL_DS   0x4C
#define ANA_DDR2_BUF_CTRL1_DS 0x420006D0

//=====================================================================================
//CAUTIONS:
//There are some configuration restrictions for SNPS uMCTL/uPCTL controller
//refer to P287 section M1.7 for details. Also copied here for notification
//- Memory burst type sequential is not supported for mDDR and LPDDR2 with BL8.
//- Memory burst type interleaved is not supported for mDDR with BL16.
//Also, Must read M2.2.10.5 Considerations for Memory Initialization
//=====================================================================================
#define MEM_WIDTH             X32

#define BASE_ADDR_AHBREG      (0x20900200)
#define ADDR_AHBREG_ARMCLK    (BASE_ADDR_AHBREG+0x0024)
#define ADDR_AHBREG_AHB_CTRL0 (BASE_ADDR_AHBREG+0x0000)
#define GLB_REG_WR_REG_GEN1   (0x4B000018)
#define GLB_REG_DPLL_CTRL     (0x4B000040)
#define UMCTL_REG_BASE        0x60200000
#define PUBL_REG_BASE         0x60201000
#define EMC_MEM_BASE_ADDR     0x80000000
#define _LPDDR1_BL_           8 //2,4,8,16. seems must be 8 now
#define _LPDDR1_BT_           1 //0: sequential
//1: interleaving
#define _LPDDR1_CL_           3 //2,3,4
#define _LPDDR2_BL_           4 //4,8,16
#define _LPDDR2_BT_           0 //0: sequential
//1: interleaving
#define _LPDDR2_WC_           1 //0: wrap
//1: no wrap

typedef enum  { Init_mem = 0, Config = 1, Config_req = 2, Access = 3, Access_req = 4, Low_power = 5,
                Low_power_entry_req = 6, Low_power_exit_req = 7} uPCTL_STATE_ENUM;
typedef enum  {INIT = 0, CFG = 1, GO = 2, SLEEP = 3, WAKEUP = 4} uPCTL_STATE_CMD_ENUM;
typedef enum  {LPDDR2, LPDDR1, DDR2, DDR3} MEM_TYPE_ENUM;
typedef enum  {MEM_64Mb, MEM_128Mb, MEM_256Mb, MEM_512Mb, MEM_1024Mb, MEM_2048Mb, MEM_4096Mb, MEM_8192Mb} MEM_DENSITY_ENUM;
typedef enum  {X8, X16, X32} MEM_WIDTH_ENUM;
typedef enum  {LPDDR2_S2, LPDDR2_S4} MEM_COL_TYPE_ENUM;
typedef enum  {BL2 = 2, BL4 = 4, BL8 = 8, BL16 = 16} MEM_BL_ENUM;
typedef enum  {SEQ, INTLV} MEM_BT_ENUM;

typedef enum
{
	MEM_CS_8192MB,
	MEM_CS_4096MB,
	MEM_CS_2048MB,
	MEM_CS_1024MB,
	MEM_CS_512MB,
	MEM_CS_256MB,
	MEM_CS_128MB,
	MEM_CS_64MB,
	MEM_CS_0MB,
}MEM_CS_TYPE;

typedef enum
{
	SDLL_PHS_DLY_MIN = 0,
	SDLL_PHS_DLY_DEF = 0,
	SDLL_PHS_DLY_36  = 0,
	SDLL_PHS_DLY_54  = 1,
	SDLL_PHS_DLY_72  = 2,
	SDLL_PHS_DLY_90  = 3,
	SDLL_PHS_DLY_108 = 4,
	SDLL_PHS_DLY_126 = 5,
	SDLL_PHS_DLY_144 = 6,
	SDLL_PHS_DLY_MAX = 6,
}SDLL_PHS_DLY_E;

typedef enum
{
	DQS_PHS_DLY_MIN = 0,
	DQS_PHS_DLY_90  = 0,
	DQS_PHS_DLY_180 = 1,
	DQS_PHS_DLY_DEF = 1,
	DQS_PHS_DLY_270 = 2,
	DQS_PHS_DLY_360 = 3,
	DQS_PHS_DLY_MAX = 3,
}DQS_PHS_DLY_E;

typedef enum
{
	DXN_MIN = 0,
	DXN0    = 0,
	DXN1    = 1,
	DXN2    = 2,
	DXN3    = 3,
	DXN_MAX = 3,
}DXN_E;


//umctl/upctl registers declaration//{{{
#define UMCTL_CFG_ADD_SCFG            0x000
#define UMCTL_CFG_ADD_SCTL            0x004//moves the uPCTL from one state to another
#define UMCTL_CFG_ADD_STAT            0x008//provides information about the current state of the uPCTL
#define UMCTL_CFG_ADD_MCMD            0x040
#define UMCTL_CFG_ADD_POWCTL          0x044
#define UMCTL_CFG_ADD_POWSTAT         0x048
#define UMCTL_CFG_ADD_MCFG1           0x07C
#define UMCTL_CFG_ADD_MCFG            0x080
#define UMCTL_CFG_ADD_PPCFG           0x084
#define UMCTL_CFG_ADD_TOGCNT1U        0x0c0
#define UMCTL_CFG_ADD_TINIT           0x0c4
#define UMCTL_CFG_ADD_TRSTH           0x0c8
#define UMCTL_CFG_ADD_TOGCNT100N      0x0cc
#define UMCTL_CFG_ADD_TREFI           0x0d0
#define UMCTL_CFG_ADD_TMRD            0x0d4
#define UMCTL_CFG_ADD_TRFC            0x0d8
#define UMCTL_CFG_ADD_TRP             0x0dc
#define UMCTL_CFG_ADD_TRTW            0x0e0
#define UMCTL_CFG_ADD_TAL             0x0e4
#define UMCTL_CFG_ADD_TCL             0x0e8
#define UMCTL_CFG_ADD_TCWL            0x0ec
#define UMCTL_CFG_ADD_TRAS            0x0f0
#define UMCTL_CFG_ADD_TRC             0x0f4
#define UMCTL_CFG_ADD_TRCD            0x0f8
#define UMCTL_CFG_ADD_TRRD            0x0fc
#define UMCTL_CFG_ADD_TRTP            0x100
#define UMCTL_CFG_ADD_TWR             0x104
#define UMCTL_CFG_ADD_TWTR            0x108
#define UMCTL_CFG_ADD_TEXSR           0x10c
#define UMCTL_CFG_ADD_TXP             0x110
#define UMCTL_CFG_ADD_TXPDLL          0x114
#define UMCTL_CFG_ADD_TZQCS           0x118
#define UMCTL_CFG_ADD_TZQCSI          0x11C
#define UMCTL_CFG_ADD_TDQS            0x120
#define UMCTL_CFG_ADD_TCKSRE          0x124
#define UMCTL_CFG_ADD_TCKSRX          0x128
#define UMCTL_CFG_ADD_TCKE            0x12c
#define UMCTL_CFG_ADD_TMOD            0x130
#define UMCTL_CFG_ADD_TRSTL           0x134
#define UMCTL_CFG_ADD_TZQCL           0x138
#define UMCTL_CFG_ADD_TCKESR          0x13c
#define UMCTL_CFG_ADD_TDPD            0x140
#define UMCTL_CFG_ADD_DTUWACTL        0x200
#define UMCTL_CFG_ADD_DTURACTL        0x204
#define UMCTL_CFG_ADD_DTUCFG          0x208
#define UMCTL_CFG_ADD_DTUECTL         0x20C
#define UMCTL_CFG_ADD_DTUWD0          0x210
#define UMCTL_CFG_ADD_DTUWD1          0x214
#define UMCTL_CFG_ADD_DTUWD2          0x218
#define UMCTL_CFG_ADD_DTUWD3          0x21c
#define UMCTL_CFG_ADD_DTURD0          0x224
#define UMCTL_CFG_ADD_DTURD1          0x228
#define UMCTL_CFG_ADD_DTURD2          0x22C
#define UMCTL_CFG_ADD_DTURD3          0x230
#define UMCTL_CFG_ADD_DFITPHYWRDATA   0x250
#define UMCTL_CFG_ADD_DFITPHYWRLAT    0x254
#define UMCTL_CFG_ADD_DFITRDDATAEN    0x260
#define UMCTL_CFG_ADD_DFITPHYRDLAT    0x264
#define UMCTL_CFG_ADD_DFISTSTAT0      0x2c0
#define UMCTL_CFG_ADD_DFISTCFG0       0x2c4
#define UMCTL_CFG_ADD_DFISTCFG1       0x2c8
#define UMCTL_CFG_ADD_DFISTCFG2       0x2d8
#define UMCTL_CFG_ADD_DFILPCFG0       0x2f0
#define UMCTL_CFG_ADD_PCFG_0          0x400
#define UMCTL_CFG_ADD_PCFG_1          0x404
#define UMCTL_CFG_ADD_PCFG_2          0x408
#define UMCTL_CFG_ADD_PCFG_3          0x40c
#define UMCTL_CFG_ADD_PCFG_4          0x410
#define UMCTL_CFG_ADD_PCFG_5          0x414
#define UMCTL_CFG_ADD_PCFG_6          0x418
#define UMCTL_CFG_ADD_PCFG_7          0x41c
#define UMCTL_CFG_ADD_DCFG_CS0        0x484
#define UMCTL_CFG_ADD_DCFG_CS1        0x494
//}}}

//publ configure registers declaration.//{{{
//copied from PUBL FPGA cfg module. here shift them(<< 2)
#define PUBL_CFG_ADD_RIDR          (0x00 * 4) // R   - Revision Identification Register
#define PUBL_CFG_ADD_PIR           (0x01 * 4) // R/W - PHY Initialization Register
#define PUBL_CFG_ADD_PGCR          (0x02 * 4) // R/W - PHY General Configuration Register
#define PUBL_CFG_ADD_PGSR          (0x03 * 4) // R   - PHY General Status Register
#define PUBL_CFG_ADD_DLLGCR        (0x04 * 4) // R/W - DLL General Control Register
#define PUBL_CFG_ADD_ACDLLCR       (0x05 * 4) // R/W - AC DLL Control Register
#define PUBL_CFG_ADD_PTR0          (0x06 * 4) // R/W - PHY Timing Register 0
#define PUBL_CFG_ADD_PTR1          (0x07 * 4) // R/W - PHY Timing Register 1
#define PUBL_CFG_ADD_PTR2          (0x08 * 4) // R/W - PHY Timing Register 2
#define PUBL_CFG_ADD_ACIOCR        (0x09 * 4) // R/W - AC I/O Configuration Register
#define PUBL_CFG_ADD_DXCCR         (0x0A * 4) // R/W - DATX8 I/O Configuration Register
#define PUBL_CFG_ADD_DSGCR         (0x0B * 4) // R/W - DFI Configuration Register
#define PUBL_CFG_ADD_DCR           (0x0C * 4) // R/W - DRAM Configuration Register
#define PUBL_CFG_ADD_DTPR0         (0x0D * 4) // R/W - SDRAM Timing Parameters Register 0
#define PUBL_CFG_ADD_DTPR1         (0x0E * 4) // R/W - SDRAM Timing Parameters Register 1
#define PUBL_CFG_ADD_DTPR2         (0x0F * 4) // R/W - SDRAM Timing Parameters Register 2
#define PUBL_CFG_ADD_MR0           (0x10 * 4) // R/W - Mode Register
#define PUBL_CFG_ADD_MR1           (0x11 * 4) // R/W - Ext}ed Mode Register
#define PUBL_CFG_ADD_MR2           (0x12 * 4) // R/W - Ext}ed Mode Register 2
#define PUBL_CFG_ADD_MR3           (0x13 * 4) // R/W - Ext}ed Mode Register 3
#define PUBL_CFG_ADD_ODTCR         (0x14 * 4) // R/W - ODT Configuration Register
#define PUBL_CFG_ADD_DTAR          (0x15 * 4) // R/W - Data Training Address Register
#define PUBL_CFG_ADD_DTDR0         (0x16 * 4) // R/W - Data Training Data Register 0
#define PUBL_CFG_ADD_DTDR1         (0x17 * 4) // R/W - Data Training Data Register 1
#define PUBL_CFG_ADD_DCUAR         (0x30 * 4) // R/W - DCU Address Register
#define PUBL_CFG_ADD_DCUDR         (0x31 * 4) // R/W - DCU Data Register
#define PUBL_CFG_ADD_DCURR         (0x32 * 4) // R/W - DCU Run Register
#define PUBL_CFG_ADD_DCUSR0        (0x36 * 4) // R/W - DCU status register
#define PUBL_CFG_ADD_BISTRR        (0x40 * 4) // R/W - BIST run register
#define PUBL_CFG_ADD_BISTMSKR0     (0x41 * 4) // R/W - BIST Mask Register 0
#define PUBL_CFG_ADD_BISTMSKR1     (0x42 * 4) // R/W - BIST Mask Register 1
#define PUBL_CFG_ADD_BISTWCR       (0x43 * 4) // R/W - BIST Word Count Register
#define PUBL_CFG_ADD_BISTLSR       (0x44 * 4) // R/W - BIST LFSR Seed Register 
#define PUBL_CFG_ADD_BISTAR0       (0x45 * 4) // R/W - BIST Address Register 0
#define PUBL_CFG_ADD_BISTAR1       (0x46 * 4) // R/W - BIST Address Register 1
#define PUBL_CFG_ADD_BISTAR2       (0x47 * 4) // R/W - BIST Address Register 2
#define PUBL_CFG_ADD_BISTUDPR      (0x48 * 4) // R/W - BIST User Data Pattern Register
#define PUBL_CFG_ADD_BISTGSR       (0x49 * 4) // R/W - BIST General Status Register
#define PUBL_CFG_ADD_BISTWER       (0x4a * 4) // R/W - BIST Word Error Register
#define PUBL_CFG_ADD_BISTBER0      (0x4b * 4) // R/W - BIST Bit Error Register 0
#define PUBL_CFG_ADD_BISTBER1      (0x4c * 4) // R/W - BIST Bit Error Register 1
#define PUBL_CFG_ADD_BISTBER2      (0x4d * 4) // R/W - BIST Bit Error Register 2
#define PUBL_CFG_ADD_BISTWCSR      (0x4e * 4) // R/W - BIST Word Count Status Register
#define PUBL_CFG_ADD_BISTFWR0      (0x4f * 4) // R/W - BIST Fail Word Register 0
#define PUBL_CFG_ADD_BISTFWR1      (0x50 * 4) // R/W - BIST Fail Word Register 1
#define PUBL_CFG_ADD_ZQ0CR0        (0x60 * 4) // R/W - ZQ 0 Impedance Control Register 0
#define PUBL_CFG_ADD_ZQ0CR1        (0x61 * 4) // R/W - ZQ 0 Impedance Control Register 1
#define PUBL_CFG_ADD_ZQ0SR0        (0x62 * 4) // R/W - ZQ 0 Impedance Status Register 0
#define PUBL_CFG_ADD_ZQ0SR1        (0x63 * 4) // R/W - ZQ 0 Impedance Status Register 1

#define PUBL_CFG_ADD_DX0GCR        (0x70 * 4) // R/W - DATX8 0 General Configuration Register
#define PUBL_CFG_ADD_DX0GSR0       (0x71 * 4) // R   - DATX8 0 General Status Register 0
#define PUBL_CFG_ADD_DX0GSR1       (0x72 * 4) // R   - DATX8 0 General Status Register 1
#define PUBL_CFG_ADD_DX0DLLCR      (0x73 * 4) // R   - DATX8 0 DLL Control Register
#define PUBL_CFG_ADD_DX0DQTR       (0x74 * 4) // R/W - DATX8 0 DQ Timing Register
#define PUBL_CFG_ADD_DX0DQSTR      (0x75 * 4) // R/W

#define PUBL_CFG_ADD_DX1GCR        (0x80 * 4) // R/W - DATX8 1 General Configuration Register
#define PUBL_CFG_ADD_DX1GSR0       (0x81 * 4) // R   - DATX8 1 General Status Register 0
#define PUBL_CFG_ADD_DX1GSR1       (0x82 * 4) // R   - DATX8 1 General Status Register 1
#define PUBL_CFG_ADD_DX1DLLCR      (0x83 * 4) // R   - DATX8 1 DLL Control Register
#define PUBL_CFG_ADD_DX1DQTR       (0x84 * 4) // R   - DATX8 1 DLL Control Register
#define PUBL_CFG_ADD_DX1DQSTR      (0x85 * 4) // R/W

#define PUBL_CFG_ADD_DX2GCR        (0x90 * 4) // R/W - DATX8 2 General Configuration Register
#define PUBL_CFG_ADD_DX2GSR0       (0x91 * 4) // R   - DATX8 2 General Status Register 0
#define PUBL_CFG_ADD_DX2GSR1       (0x92 * 4) // R   - DATX8 2 General Status Register 1
#define PUBL_CFG_ADD_DX2DLLCR      (0x93 * 4) // R   - DATX8 2 DLL Control Register
#define PUBL_CFG_ADD_DX2DQTR       (0x94 * 4) // R   - DATX8 2 DLL Control Register
#define PUBL_CFG_ADD_DX2DQSTR      (0x95 * 4) // R/W

#define PUBL_CFG_ADD_DX3GCR        (0xa0 * 4) // R/W - DATX8 3 General Configuration Register
#define PUBL_CFG_ADD_DX3GSR0       (0xa1 * 4) // R   - DATX8 3 General Status Register 0
#define PUBL_CFG_ADD_DX3GSR1       (0xa2 * 4) // R   - DATX8 3 General Status Register 1
#define PUBL_CFG_ADD_DX3DLLCR      (0xa3 * 4) // R   - DATX8 3 DLL Control Register
#define PUBL_CFG_ADD_DX3DQTR       (0xa4 * 4) // R/W - DATX8 3 DQ Timing Register
#define PUBL_CFG_ADD_DX3DQSTR      (0xa5 * 4) // R/W

static void modify_reg_field(uint32 addr, uint32 start_bit, uint32 bit_num, uint32 value)
{
	uint32 temp, i;
	temp = REG32(addr);
	for (i=0; i<bit_num; i++)
	{
		temp &= ~(0x1<<(start_bit+i));
	}
	temp |= value<<start_bit;
	REG32(addr) = temp;
}

static void modify_adie_reg_field(uint32 addr, uint32 start_bit, uint32 bit_num, uint32 value)
{
	uint32 temp, i;
	temp = ADI_Analogdie_reg_read(addr);
	for (i=0; i<bit_num; i++)
	{
		temp &= ~(0x1<<(start_bit+i));
	}
	temp |= value<<start_bit;
	REG32(addr) = temp;
}

static uint32 polling_reg_bit_field(uint32 addr, uint32 start_bit, uint32 bit_num, uint32 value)
{
	uint32 temp, i;
	uint32 exp_value;
	uint32 mask;

	mask = 0;
	for (i=0; i<bit_num; i++)
	{
		mask |= 1<<(start_bit+i);
	}
	exp_value = value << start_bit;
	do {temp = REG32(addr);} while((temp & mask) != exp_value);
	return temp;
}

static void wait_n_pclk_cycle(uint32 num)
{
	volatile uint32 i;
	uint32          value_temp;
	for (i=0; i<num; i++)
	{
		value_temp = REG32(PUBL_REG_BASE+PUBL_CFG_ADD_PGSR);
	}
}

static void wait_100ns()
{
	uint32 i;
	for (i=0; i<100; i++);
}

static void wait_1us()
{
	uint32 i;
	for (i=0; i<1000; i++);
}

static void wait_us(uint32 us)
{
	uint32 i;
	for (i=0; i<us; i++)
	{
		wait_1us();
	}
}

static void write_upctl_state_cmd(uPCTL_STATE_CMD_ENUM cmd)
{
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_SCTL) = cmd;
}

static void poll_upctl_state (uPCTL_STATE_ENUM state)
{
	uPCTL_STATE_ENUM state_poll;
	uint32 value_temp;
	do
	{
		value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_STAT);
		state_poll = value_temp & 0x7;
	}
	while(state_poll != state);
	return;
}

static void move_upctl_state_to_initmem(void)
{
	uPCTL_STATE_ENUM upctl_state;
	uPCTL_STATE_CMD_ENUM  upctl_state_cmd;
	uint32 tmp_val ;
	//tmp_val = upctl_state ;
	tmp_val = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_STAT);
	//while(upctl_state!= Init_mem) {
	while((tmp_val & 0x7) != Init_mem)
	{
		switch((tmp_val & 0x7))
		{
			case Config:
				{
					write_upctl_state_cmd(INIT);
					poll_upctl_state(Init_mem);
					upctl_state = Init_mem;
					tmp_val = upctl_state ;
					break;
				}
			case Access:
				{
					write_upctl_state_cmd(CFG);
					poll_upctl_state(Config);
					upctl_state = Config;
					tmp_val = upctl_state ;
					break;
				}
			case Low_power:
				{
					write_upctl_state_cmd(WAKEUP);
					poll_upctl_state(Access);
					upctl_state = Access;
					tmp_val = upctl_state ;
					break;
				}
			default:   //transitional state
				{
					tmp_val = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_STAT);
					break;
				}
		}
	}
}

static void move_upctl_state_to_config(void)
{
	uPCTL_STATE_ENUM upctl_state;
	//uPCTL_STATE_CMD_ENUM  upctl_state_cmd;
	uint32  tmp_val ;
	tmp_val = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_STAT);
	upctl_state = tmp_val & 0x7;
	while(upctl_state != Config)
	{

		switch(upctl_state)
		{
			case Low_power:
				{
					write_upctl_state_cmd(WAKEUP);
					poll_upctl_state(Access);
					upctl_state = Access;
					break;
				}
			case Init_mem:
				{
					write_upctl_state_cmd(CFG);
					poll_upctl_state(Config);
					upctl_state = Config;
					break;
				}
			case Access:
				{
					write_upctl_state_cmd(CFG);
					poll_upctl_state(Config);
					upctl_state = Config;
					break;
				}
			default:   //transitional state
				{
					tmp_val = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_STAT);
					upctl_state = tmp_val & 0x7;
				}
		}
	}
}

static void move_upctl_state_to_low_power(void)
{
	uPCTL_STATE_ENUM upctl_state;
	//uPCTL_STATE_CMD_ENUM  upctl_state_cmd;
	uint32  tmp_val ;
	tmp_val = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_STAT);
	upctl_state = tmp_val & 0x7;
	while(upctl_state != Low_power)
	{
		switch(upctl_state)
		{
			case Access:
				{
					write_upctl_state_cmd(SLEEP);
					poll_upctl_state(Low_power);
					upctl_state = Low_power;
					break;
				}
			case Config:
				{
					write_upctl_state_cmd(GO);
					poll_upctl_state(Access);
					upctl_state = Access;
					break;
				}
			case Init_mem:
				{
					write_upctl_state_cmd(CFG);
					poll_upctl_state(Config);
					upctl_state = Config;
					break;
				}
			default:   //transitional state
				{
					tmp_val = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_STAT);
					upctl_state = tmp_val & 0x7;
				}
		}
	}
}


static void move_upctl_state_to_access(void)
{
	uPCTL_STATE_ENUM upctl_state;
	uint32  tmp_val ;
	tmp_val = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_STAT);
	upctl_state = tmp_val & 0x7;

	while(upctl_state != Access)
	{
		switch(upctl_state)
		{
			case Access:
				{
					break;
				}
			case Config:
				{
					write_upctl_state_cmd(GO);
					poll_upctl_state(Access);
					upctl_state = Access;
					break;
				}
			case Init_mem:
				{
					write_upctl_state_cmd(CFG);
					poll_upctl_state(Config);
					upctl_state = Config;
					break;
				}
			case Low_power:
				{
					write_upctl_state_cmd(WAKEUP);
					poll_upctl_state(Access);
					upctl_state = Access;
					break;
				}
			default:   //transitional state
				{
					tmp_val = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_STAT);
					upctl_state = tmp_val & 0x7;
				}
		}
	}
}

//return 1 if OK, 0 if fail
static uint32 ddr_rw_chk_single(uint32 offset, uint32 data)
{
	uint32 rd;
	*(volatile uint32 *)(EMC_MEM_BASE_ADDR + offset) = data;
	rd = *(volatile uint32 *)(EMC_MEM_BASE_ADDR + offset);
	if(rd == data)
		return 1;
	else
		return 0;
}
//return 1 if OK, 0 if fail
static uint32 ddr_rw_chk(uint32 offset)
{
	uint32 i;
	uint32 data;
	for(i = 0; i < 6; i++)
	{
		if(i == 0)
		{
			data = 0x00000000;
		}
		else if(i == 1)
		{
			data = 0xffffffff;
		}
		else if(i == 2)
		{
			data = 0x12345678;
		}
		else if(i == 3)
		{
			data = 0x87654321;
		}
		else if(i == 4)
		{
			data = 0x5a5a5a5a;
		}
		else if(i == 5)
		{
			data = 0xa5a5a5a5;
		}
		if(ddr_rw_chk_single(offset, data) == 0)
			return 0;
		if(ddr_rw_chk_single(offset + 0x4, data) == 0)
			return 0;
		if(ddr_rw_chk_single(offset + 0x8, data) == 0)
			return 0;
		if(ddr_rw_chk_single(offset + 0xc, data) == 0)
			return 0;
	}
	return 1;
}
static uint32 dq_training(uint32 offset)
{
	uint32 i;
	uint32 B0, B1, B2, B3;
	for(B0 = 0; B0 < 16; B0++)
	{
		*(volatile uint32 *)(PUBL_REG_BASE + PUBL_CFG_ADD_DX0DQSTR) = B0 | (B0 << 4);
		for(B1 = 0; B1 < 16; B1++)
		{
			*(volatile uint32 *)(PUBL_REG_BASE + PUBL_CFG_ADD_DX1DQSTR) = B1 | (B1 << 4);
			for(B2 = 0; B2 < 16; B2++)
			{
				*(volatile uint32 *)(PUBL_REG_BASE + PUBL_CFG_ADD_DX2DQSTR) = B2 | (B2 << 4);
				for(B3 = 0; B3 < 16; B3++)
				{
					*(volatile uint32 *)(PUBL_REG_BASE + PUBL_CFG_ADD_DX3DQSTR) = B3 | (B3 << 4);
					//for(j=0;j<100;j++); //wait some time after changing config register
					if(ddr_rw_chk(offset))
					{
						// *(volatile uint32 *)(EMC_MEM_BASE_ADDR+0x1000+i)=(0xBA55<<16)|(B3<<12)|(B2<<8)|(B1<<4)|B0;
						return 1;
					}
					else
					{
						//*(volatile uint32 *)(EMC_MEM_BASE_ADDR+0x1000+i)=(0xFA11<<16)|(B3<<12)|(B2<<8)|(B1<<4)|B0;
					}
					i = i + 4;
				}
			}
		}
	}
	//if not found, set as default value,and set rank2/3 as all one to flag the error
	*(volatile uint32 *)(PUBL_REG_BASE + PUBL_CFG_ADD_DX0DQSTR) = 0xffffffaa;
	*(volatile uint32 *)(PUBL_REG_BASE + PUBL_CFG_ADD_DX1DQSTR) = 0xffffffaa;
	*(volatile uint32 *)(PUBL_REG_BASE + PUBL_CFG_ADD_DX2DQSTR) = 0xffffffaa;
	*(volatile uint32 *)(PUBL_REG_BASE + PUBL_CFG_ADD_DX3DQSTR) = 0xffffffaa;
	return 0;
}

static void emc_publ_do_gate_training(void)
{
	uint32  value_temp, i;
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PIR) = 0x81;

	//check data training done
	//according to PUBL databook on PIR operation.
	//10 configuration clock cycle must be waited before polling PGSR
	//repeat(10)@(posedge `HIER_ARM_SYS.u_sys_wrap.u_DWC_ddr3phy_pub.pclk);
	//for(i = 0; i < 100; i++);
	wait_n_pclk_cycle(5);
	do
		value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PGSR);
	while((value_temp & 0x10) != 0x10);
	if((value_temp&(0x3 << 5)) != 0x0)
	{
		if((value_temp & 0x40) != 0)
		{
			while(1);
		}
		if((value_temp & 0x20) != 0)
		{
			while(1);
		}
	}
	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DX0GSR0);
	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DX1GSR0);
	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DX2GSR0);
	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DX3GSR0);
}

static void set_value_ddrphy_ret_en(uint32 value)
{
	if (value == 0x1)
	{
		REG32(0x4B001080) = 0x1;  //set ret_en to 1
	}
	else if (value == 0x0)
	{
		REG32(0x4B002080) = 0x1;  //clear set_en to 1
	}
}

static void publ_do_zq_calibration()
{
	uint32 value_temp, i;
	REG32(PUBL_REG_BASE+PUBL_CFG_ADD_ZQ0CR0) = 1<<30;

	value_temp = polling_reg_bit_field(PUBL_REG_BASE+PUBL_CFG_ADD_ZQ0SR0, 31, 1, 1);
	if ((value_temp & (0x1<<30)) == (0<<30))
	{

	}
	else
	{
//zq cal has error
		while (1);
	}
}

static void emc_init_common_reg(MEM_TYPE_ENUM mem_type_enum,
		MEM_WIDTH_ENUM mem_width_enum, uint32 mem_drv)
{
	uint32  value_temp;
	uint32  TOGCNT1U, TOGCNT100N;

	if (mem_type_enum == LPDDR1)
	{
		value_temp = (1<<31)|(1<<28)|(0xc<<5)|(0xc);
		REG32(PUBL_REG_BASE+PUBL_CFG_ADD_ZQ0CR0) = value_temp;
	}

	//program common registers for ASIC/FPGA
	//Memory Timing Configuration Registers
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_SCFG) = 0x00000420;
	value_temp = 0x00e60041; //no auto low power enabled
	value_temp &= ~(0x1 << 17);
	value_temp &= ~(0x3 << 22);
	value_temp &= ~(0x3 << 20);


	switch(mem_type_enum)
	{
		case LPDDR1:
			{
				value_temp |= (0x2 << 22); //LPDDR1 enabled
				value_temp |= (_LPDDR1_BL_ == 2  ? 0x0 :
						_LPDDR1_BL_ == 4  ? 0x1 :
						_LPDDR1_BL_ == 8  ? 0x2 :
						_LPDDR1_BL_ == 16 ? 0x3 : 0x0) << 20;
				value_temp |= (0x1 << 17); //pd_exit_mode
				break;
			}
		case LPDDR2:
			{
				value_temp |= 0x3 << 22; //LPDDR2 enabled
				value_temp |= (_LPDDR2_BL_ == 4  ? 0x1 :
						_LPDDR2_BL_ == 8  ? 0x2 :
						_LPDDR2_BL_ == 16 ? 0x3 : 0x2) << 20;
				value_temp |= (0x1 << 17); //pd_exit_mode
				break;
			}
	}
	value_temp &= ~(0xff << 24); //Clock stop idle period in n_clk cycles
	value_temp &= ~(0xff << 8); //Power-down idle period in n_clk cycles
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCFG) =        value_temp;

	//sr_idle
	value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCFG1);
	value_temp &= ~0xff; //
	value_temp &= ~(0xff << 16); //
	//value_temp[7:0] = 0x7D; //125*32=4000 cycles
	value_temp |= 0x00; //
	value_temp |= (0x00 << 16); //hw_idle
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCFG1) =       value_temp;

	value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PPCFG);
	value_temp &= ~0x1;
	switch(mem_width_enum)
	{
		case X16:
			{
				value_temp |= 0x1;    //enable partial populated memory
				break;
			}
		case X8:
		default:
			break;
	}
	REG32(UMCTL_REG_BASE+UMCTL_CFG_ADD_PPCFG) = value_temp;
	if (0)
	{
		TOGCNT100N = 100;
		TOGCNT1U   = 1000;
	}
	else
	{
		switch (SC8825_EMC_FREQ)
		{
			case 100:
				TOGCNT100N = 10;
				TOGCNT1U   = 100;
				break;
			case 200:
				TOGCNT100N = 20;
				TOGCNT1U   = 200;
				break;
			case 400:
				TOGCNT100N = 40;
				TOGCNT1U   = 400;
				break;
			default:
				while(1);
		}
	}

	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TOGCNT1U) =    TOGCNT1U;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TOGCNT100N) =  TOGCNT100N;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TRFC) =        0x00000034;

	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TINIT) =       0x000000C8;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TRSTH) =       0x00000000;

	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TREFI) =       0x00000027;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TMRD) =        0x00000005;


	value_temp = (mem_type_enum == LPDDR2 ) ? 0x00010008 : 00000004 ; //compenstate for clk jitter
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TRP) =         value_temp;

	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TRTW) =        0x00000001; //caution, 1 enough?, xiohui@2012-11-26
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TAL) =         0x00000000; //no al for lpddr1/lpddr2

	value_temp = (mem_type_enum == LPDDR2 ) ? 6 : 3;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TCL) =         value_temp;

	value_temp = (mem_type_enum == LPDDR2 ) ? 3 : 1;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TCWL) =        value_temp;


	value_temp = (mem_type_enum == LPDDR2 ) ? 0x00000011 : 0x00000008;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TRAS) =        value_temp;
	value_temp = (mem_type_enum == LPDDR2 ) ? 0x00000019 : 0x0000000c;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TRC) =         value_temp;

	value_temp = (mem_type_enum == LPDDR2 ) ? 0x00000006 : 0x00000003; //compenstate for clk jitter
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TRCD) =        value_temp;
	value_temp = (mem_type_enum == LPDDR2 ) ? 0x00000004 : 0x00000002;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TRRD) =        value_temp;
	value_temp = (mem_type_enum == LPDDR2 ) ? 0x00000003 : 0x00000001;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TRTP) =        value_temp;
	value_temp = (mem_type_enum == LPDDR2 ) ? 0x00000006 : 0x00000003;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TWR) =         value_temp;

	//xiaohui change from 3 to 4 due to tWTR=7.5ns whereas clk_emc=2.348ns
	value_temp = (mem_type_enum == LPDDR2 ) ? 0x00000003 : 0x00000002;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TWTR) =        value_temp;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TEXSR) =       0x00000038;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TXP) =         0x00000004;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TXPDLL) =      0x00000000;
	//xiaohui change from 0x24 to 0x27 due to tZQCS=90ns whereas clk_emc=2.348ns
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TZQCS) =       0x00000024;

	//value_temp = mem_type_enum == LPDDR2 ? 0x190A4 : 0x0; //assume 0.4s need one calibration, see samsung lpddr2 on page 140
	value_temp = 0x0; //assume 0.4s need one calibration, see samsung lpddr2 on page 140
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TZQCSI) =      value_temp;

	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TDQS) =        0x00000001;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TCKSRE) =      0x00000000;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TCKSRX) =      0x00000002;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TCKE) =        0x00000003;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TMOD) =        0x00000000;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TRSTL) =       0x00000002;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TZQCL) =       0x00000090;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TCKESR) =      0x00000006;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_TDPD) =        0x00000001;

	//default value not compatible with real condition
	value_temp = 0x00000011;
	value_temp |= 1 << 8; //addr_map: bank based
	value_temp &= ~(0xf << 2); //density
	value_temp |= 0x1 << 6;   //check:!!dram_type:1: LPDDR2 S4 or MDDR row width 13 and col width 10, 0:    LPDDR2 S2 or MDDR row width 14 and col width 9
	switch (MEM_CS0_TYPE)
	{
		case MEM_CS_8192MB:
			value_temp |= (0x7 << 2);
			break;
		case MEM_CS_4096MB:
			value_temp |= (0x6 << 2);
			break;
		case MEM_CS_2048MB:
			value_temp |= (0x5 << 2);
			break;
		case MEM_CS_1024MB:
			value_temp |= (0x4 << 2);
			break;
		case MEM_CS_512MB:
			value_temp |= (0x3 << 2);
			break;
		case MEM_CS_256MB:
			value_temp |= (0x2 << 2);
			break;
		case MEM_CS_128MB:
			value_temp |= (0x1 << 2);
			break;
		case MEM_CS_64MB:
			value_temp |= (0x0 << 2);
			break;
		default:
			value_temp |= (0x6 << 2);
	}
	value_temp |= (mem_width_enum == X32) ? 0x3 :
		(mem_width_enum == X16) ? 0x2 :
		0x0;  //dram_io_width
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DCFG_CS0) = value_temp;

	value_temp &= ~(0xf << 2); //density

	switch (MEM_CS1_TYPE)
	{
		case MEM_CS_8192MB:
			value_temp |= (0x7 << 2);
			break;
		case MEM_CS_4096MB:
			value_temp |= (0x6 << 2);
			break;
		case MEM_CS_2048MB:
			value_temp |= (0x5 << 2);
			break;
		case MEM_CS_1024MB:
			value_temp |= (0x4 << 2);
			break;
		case MEM_CS_512MB:
			value_temp |= (0x3 << 2);
			break;
		case MEM_CS_256MB:
			value_temp |= (0x2 << 2);
			break;
		case MEM_CS_128MB:
			value_temp |= (0x1 << 2);
			break;
		case MEM_CS_64MB:
			value_temp |= (0x0 << 2);
			break;
		default:
			value_temp |= (0x6 << 2);
	}

	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DCFG_CS1) = value_temp;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DFITPHYWRDATA) = 0x00000001;

	value_temp = (mem_type_enum == LPDDR2 ) ? 3 : 0; //WL-1, see PUBL P143
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DFITPHYWRLAT) = value_temp;

	switch(mem_type_enum)
	{
		case LPDDR2:
			{
				value_temp = 5;
				break;
			};//??? RL-2, see PUBL P143
		case LPDDR1:
			{
				value_temp = 1;
				break;
			};
		default:
			break;//$stop(2);
	}
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DFITRDDATAEN) = value_temp;

	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DFITPHYRDLAT) = 0x0000000f;

	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DFISTCFG0) =   0x00000007;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DFISTCFG1) =   0x00000003;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DFISTCFG2) =   0x00000003;
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DFILPCFG0) =   0x00078101; //dfi_lp_wakeup_sr=8

	//the priority settings is DSP > Disp > others > GPU
	// so DSP/Disp/Camera are set to LL and all others set to BE
	value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_0);
	value_temp &= ~0xff;
	value_temp &= ~(0xff<<16);
	value_temp |= (0x10 << 16); //quantum
	value_temp |= (0x0 << 7); //rdwr_ordered
	value_temp |= (0x1 << 6); //st_fw_en
	value_temp |= (0x1 << 5); //bp_rd_en, as per the coreconsultant
	value_temp |= (0x1 << 4); //bp_wr_en
	//value_temp[1:0] = 0x3; //qos dynamic mode by port signal
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_0) =      value_temp;

	value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_1);
	value_temp &= ~0xff;
	value_temp &= ~(0xff<<16);
	value_temp |= (0x10 << 16); //quantum
	value_temp |= (0x0 << 7); //rdwr_ordered
	value_temp |= (0x1 << 6); //st_fw_en
	value_temp |= (0x1 << 5); //bp_rd_en, as per the coreconsultant
	value_temp |= (0x1 << 4); //bp_wr_en
	//value_temp[1:0] = 0x3; //qos dynamic mode by port signal
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_1) =      value_temp;

	value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_2);
	value_temp &= ~0xff;
	value_temp &= ~(0xff<<16);
	value_temp |= (0x10 << 16); //quantum
	value_temp |= (0x1 << 7); //rdwr_ordered
	value_temp |= (0x1 << 6); //st_fw_en
	value_temp |= (0x1 << 5); //bp_rd_en, as per the coreconsultant
	value_temp |= (0x1 << 4); //bp_wr_en
	//value_temp[1:0] = 0x3; //qos dynamic mode by port signal
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_2) =      value_temp;

	value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_3);
	value_temp &= ~0xff;
	value_temp &= ~(0xff<<16);
	value_temp |= (0x10 << 16); //quantum
	value_temp |= (0x1 << 7); //rdwr_ordered
	value_temp |= (0x1 << 6); //st_fw_en
	value_temp |= (0x1 << 5); //bp_rd_en, as per the coreconsultant
	value_temp |= (0x1 << 4); //bp_wr_en
	value_temp |= 1;	//qos:LL
	//value_temp[1:0] = 0x3; //qos dynamic mode by port signal
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_3) =      value_temp;

	value_temp = REG32( UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_4);
	value_temp &= ~0xff;
	value_temp &= ~(0xff<<16);
	value_temp |= (0x10 << 16); //quantum
	value_temp |= (0x1 << 7); //rdwr_ordered
	value_temp |= (0x1 << 6); //st_fw_en
	value_temp |= (0x1 << 5); //bp_rd_en, as per the coreconsultant
	value_temp |= (0x1 << 4); //bp_wr_en
	value_temp |= 0x1;	//qos:LL
	//value_temp[1:0] = 0x3; //qos dynamic mode by port signal
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_4) =      value_temp;

	value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_5);
	value_temp &= ~0xff;
	value_temp &= ~(0xff<<16);
	value_temp |= (0x10 << 16); //quantum
	value_temp |= (0x0 << 7); //rdwr_ordered
	value_temp |= (0x1 << 6); //st_fw_en
	value_temp |= (0x1 << 5); //bp_rd_en, as per the coreconsultant
	value_temp |= (0x1 << 4); //bp_wr_en
	value_temp |= 1;	//qos:LL
	//value_temp[1:0] = 0x3; //qos dynamic mode by port signal
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_5) =      value_temp;

	value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_6);
	value_temp &= ~0xff;
	value_temp &= ~(0xff<<16);
	value_temp |= (0x10 << 16); //quantum
	value_temp |= (0x0 << 7); //rdwr_ordered
	value_temp |= (0x1 << 6); //st_fw_en
	value_temp |= (0x1 << 5); //bp_rd_en, as per the coreconsultant
	value_temp |= (0x1 << 4); //bp_wr_en
	value_temp |= 1;        //qos:LL
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_6) =      value_temp;

	value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_7);
	value_temp &= ~0xff;
	value_temp &= ~(0xff<<16);
	value_temp |= (0x10 << 16); //quantum
	value_temp |= (0x1 << 7); //rdwr_ordered
	value_temp |= (0x1 << 6); //st_fw_en
	value_temp |= (0x1 << 5); //bp_rd_en, as per the coreconsultant
	value_temp |= (0x1 << 4); //bp_wr_en
	//value_temp[1:0] = 0x3; //qos dynamic mode by port signal
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_PCFG_7) =      value_temp;

	//REG32(PUBL_REG_BASE+PUBL_CFG_ADD_DTAR) = (0x7<<28)|(0x3fff<<12)|(0x3f0<<0);
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DTAR) = (0x0 << 28) | (0x0 << 12) | (0x0 << 0);

	//ZQCR1
	value_temp = REG32(PUBL_REG_BASE+PUBL_CFG_ADD_ZQ0CR1);

	switch (MEMORY_TYPE)
	{
		case LPDDR2:
			value_temp &= ~(0xf<<4);
			break;
		case LPDDR1:
			value_temp &= ~(0xf<<4);
			break;
		default:
			while(1);
	}
	value_temp &= ~0xf;
	switch (mem_drv)
	{
		case 34:
			value_temp |= 0xD;  //output impdence divide select, 7:60ohm 9:48ohm 11:40ohm 13:34ohm
			break;
		case 40:
			value_temp |= 0xB;  //output impdence divide select, 7:60ohm 9:48ohm 11:40ohm 13:34ohm
			break;
		case 48:
			value_temp |= 0x9;  //output impdence divide select, 7:60ohm 9:48ohm 11:40ohm 13:34ohm
			break;
		case 60:
			value_temp |= 0x7;  //output impdence divide select, 7:60ohm 9:48ohm 11:40ohm 13:34ohm
			break;
		case 80:
			value_temp |= 0x5;  //output impdence divide select, 7:60ohm 9:48ohm 11:40ohm 13:34ohm
			break;
		default:
			while(1);
	}

	REG32(PUBL_REG_BASE+PUBL_CFG_ADD_ZQ0CR1) = value_temp;
	value_temp = (8<<18) | (2750<<6) |27;	//per 533MHz
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PTR0) = value_temp;

	//PTR1
	//value_temp[18:0] = count(200us/clk_emc_period);//CKE high time to first command (200 us)
	//value_temp[26:19] = don't care;//CKE low time with power and clock stable (100 ns) for lpddr2
	value_temp = (200 * 1000 * 10) / 25; //CKE high time to first command (200 us)
	value_temp |= (1000 / 25) << 19; //CKE low time with power and clock stable (100 ns) for lpddr2
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PTR1 ) = value_temp;

	//PTR2
	//value_temp[16:0] = count((200us_for_ddr3 or 11us_for_lpddr2) /clk_emc_period);
	//value_temp[26:17] = count(1us/clk_emc_period);
	switch(mem_type_enum)
	{
		case LPDDR1:
			{
				value_temp = 0;
				break;
			}
		case LPDDR2:
			{
				value_temp = (11 * 1000 * 10) / (25);
				break;
			}
		case DDR3:
			{
				value_temp = (200 * 1000 * 10) / (25);
				break;
			}
		default:
			break;
	}
	value_temp |= ((1 * 1000 * 10) / 25) << 17;
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PTR2 ) = value_temp;

	//ACIOCR
	value_temp = (mem_type_enum == LPDDR1)?0x1:0x0;
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_ACIOCR, 0, 1, value_temp);

	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DCR);
	value_temp &= ~(0x7);
	value_temp &= ~(0x1 << 3);
	value_temp &= ~(0x3 << 8);
	value_temp |= (mem_type_enum == LPDDR2 ) ? 0x4 : 0x0;
	value_temp |= ( (mem_type_enum == LPDDR2 ) ? 0x1 : 0x0) << 3;
	value_temp |= 0x00 << 8; //lpddr2-S4?????? xiaohui
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DCR) = value_temp;

	//MR0
	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_MR0);
	switch(mem_type_enum)
	{
		case LPDDR2:
			{
				value_temp = 0x0; //not applicable
				break;
			}
		case LPDDR1:
			{
				value_temp &= ~(0x7);
				value_temp &= ~(0x1 << 3);
				value_temp &= ~(0x7 << 4);
				value_temp &= ~(0x1 << 7);
				value_temp |= (_LPDDR1_BL_ == 2 ? 0x1 :
					_LPDDR1_BL_ == 4 ? 0x2 :
					_LPDDR1_BL_ == 8 ? 0x3 :
					_LPDDR1_BL_ == 16 ? 0x4 : 0x1);
				value_temp |= (_LPDDR1_BT_ << 3);
				value_temp |= (_LPDDR1_CL_ == 2 ? 0x2 :
					_LPDDR1_CL_ == 3 ? 0x3 :
					_LPDDR1_CL_ == 4 ? 0x4 : 0x3) << 4;
				value_temp |= 0x0 << 7; //normal operation
				break;
			}
		default:
			break;
	}
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_MR0) = value_temp;

	//MR1
	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_MR1);
	switch(mem_type_enum)
	{
		case LPDDR1: {break;}
		case LPDDR2:
			{
				value_temp &= ~0x7;
				value_temp &= ~(0x3 << 3);
				value_temp &= ~(0x7 << 5);
				value_temp |= (_LPDDR2_BL_ == 4 ? 0x2 :
					_LPDDR2_BL_ == 8 ? 0x3 :
					_LPDDR2_BL_ == 16 ? 0x4 : 0x3);
				value_temp |= _LPDDR2_BT_ << 3;
				value_temp |= _LPDDR2_WC_ << 4;
				value_temp |= (0x4 << 5); //nWR=6 //!check
				break;
			}
		default:
			break;
	}
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_MR1) = value_temp;

	//MR2
	switch(mem_type_enum)
	{
		case LPDDR1:
			{
				value_temp = 0x0;    //extend mode
				break;
			}
		case LPDDR2:
			{
				value_temp = 0x4;    //RL = 6 / WL = 3
				break;
			}
		default:
			break;
	}
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_MR2) = value_temp;

	//MR3
	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_MR3);
	value_temp &= ~0xf;
	value_temp |= 0x2;
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_MR3) = value_temp;

	//ODTCR. disable ODT for wrtie and read for LPDDR2/LPDDR1
	value_temp = REG32(PUBL_REG_BASE+PUBL_CFG_ADD_ODTCR);
	switch(MEMORY_TYPE)
	{
		case LPDDR1:
			value_temp &= ~0xff;
			value_temp &= ~(0xff<<16);
			break;
		case LPDDR2:
			value_temp &= ~0xff;
			value_temp &= ~(0xff<<16);
			break;
		default:
			while(1);
	}
	REG32(PUBL_REG_BASE+PUBL_CFG_ADD_ODTCR) = value_temp;

	//DTPR0
	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DTPR0);
	switch(mem_type_enum)
	{
		case LPDDR1:
			{
				//value_temp[1:0] = 2;    //tMRD
				//value_temp[4:2] = 2;    //tRTP
				//value_temp[7:5] = (mem_type_enum == LPDDR2 ) ? 0x00000004: 0x00000002;
				//value_temp[11:8] = 4;    //tRP
				//value_temp[15:12] = 4;    //tRCD
				//value_temp[20:16] = 8;    //tRAS
				//value_temp[24:21] = 2;    //tRRD
				//value_temp[30:25] = 12;    //tRC
				//value_temp[31] = 0x0;    //tCCD: BL/2
				value_temp = (0x0 << 31) | (12 << 25) | (2 << 21) | (8 << 16) | (4 << 12) | (4 << 8) | (2 << 2) | (2);
				value_temp |= 0x00000002 << 5; //tWTR
				break;
			}
		case LPDDR2:
			{
				value_temp = 0x36916a6d;//??? xiaohui
				break;
			}
		default:{while(1);}
	}
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DTPR0) = value_temp;

	//DTPR1
	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DTPR1);
	value_temp &= ~0x3;
	value_temp &= ~(0xff << 16);
	switch(mem_type_enum)
	{
		case LPDDR1:
			{
				value_temp |= 0x1; //same with lpddr2 to avoid additional code size
				value_temp |= (32 << 16);  //tRFC
				break;
			}
		case LPDDR2:
			{
				value_temp = 0x193400a0;//??? xiaohui
				break;
			}
		default:
			break;
	}
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DTPR1) = value_temp;

	//DTPR2
	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DTPR2);
	switch(mem_type_enum)
	{
		case LPDDR1:
				break;
		case LPDDR2:
			{
				value_temp = 0x1001a0c8; //actually is default value
				break;
			}
		default:
			break;
	}
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DTPR2) = value_temp;

	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DXCCR);
	value_temp &= ~(0x1 << 1);
	value_temp &= ~(0xf << 4);
	value_temp &= ~(0xf << 8);
	value_temp &= ~(0x1 << 14);
	value_temp |= ((mem_type_enum == LPDDR1 ) ? 0x1 : 0x0) << 1; //iom. 0:LPDDR1, 1: others
	value_temp |= (((mem_type_enum == LPDDR2)|(mem_type_enum==LPDDR1)) ?   (DQSR_VALUE) : 0x0) << 4; //pull down resistor for DQS
	value_temp |= (((mem_type_enum == LPDDR2)|(mem_type_enum==LPDDR1)) ? (8|DQSR_VALUE) : 0x0) << 8; //pull down resistor for DQS_N
	value_temp |= ((mem_type_enum == LPDDR2 ) ? 0x0 : 0x1) << 14; //DQS# Reset
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DXCCR) = value_temp;

	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DX0GCR);
	value_temp&=~((0x3<<9)|(1<<3)); //disable DQ/DQS Dynamic RTT Control
	value_temp|= ((mem_type_enum == LPDDR1)?0x1:0x0)<<3;
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DX0GCR) = value_temp;

	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DX1GCR);
	value_temp&=~((0x3<<9)|(1<<3));   //disable DQ/DQS Dynamic RIT Control
	value_temp|= ((mem_type_enum == LPDDR1)?0x1:0x0)<<3;
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DX1GCR) = value_temp;

	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DX2GCR);
	value_temp&=~((0x3<<9)|(1<<3));   //disable DQ/DQS Dynamic RIT Control
	value_temp|= ((mem_type_enum == LPDDR1)?0x1:0x0)<<3;
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DX2GCR) = value_temp;

	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DX3GCR);
	value_temp&=~((0x3<<9)|(1<<3));   //disable DQ/DQS Dynamic RIT Control
	value_temp|= ((mem_type_enum == LPDDR1)?0x1:0x0)<<3;
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DX3GCR) = value_temp;

	value_temp = (0 << 30) | (0x7fff << 13) | (0x7 << 10) | (0x300);
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DTUWACTL) = value_temp;
	value_temp = (0 << 30) | (0x7fff << 13) | (0x7 << 10) | (0x300);
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DTURACTL) = value_temp;

	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PGCR);
	value_temp &= ~0x7;
	value_temp &= ~(0xf << 18);
	value_temp |= (mem_type_enum == LPDDR2 ) ? 0x0 : 0x1; //0 = ITMS uses DQS and DQS#
	//1 = ITMS uses DQS only
	value_temp |= (0x1 << 1);
	if (MEM_CS1_TYPE == MEM_CS_0MB)
	{
		value_temp |= (0x1<<18); //only enable CS0 for data training
	}
	else
	{
		value_temp |= (0x3 << 18); //enable CS0/1 for data training
	}
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PGCR) = value_temp;

	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DSGCR);
	value_temp &= ~0xfff;  //only applicable for LPDDR

	//CAUTION:[7:5] DQSGX, [10:8] DQSGE
	value_temp |= (0x1f | ((GATE_EARLY_LATE)<<8) | ((GATE_EARLY_LATE)<<5));  //only applicable for LPDDR
	value_temp &= ~(0x1<<2); // zq Update Enable,CHECK!!!!
	value_temp &= ~(0x1<<4); // Low Power DLL Power Down
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_DSGCR) = value_temp;
} //emc_init_common_reg

//all cs are initialized simultaneously
void mem_init(MEM_TYPE_ENUM mem_type_enum, uint32 bl, MEM_BT_ENUM bt, uint32 zq)
{
	uint32  value_temp, i;
	uint32  mode_ba;
	uint32  mode_a;
	switch(mem_type_enum)
	{
		case LPDDR1:   //{{{
		{
			REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD) = 0x85f04001;
			do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
			while((value_temp & 0x80000000) != 0x0);

			for(i = 0; i < 2; i++)
			{
				REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD) = 0x85f04002;
				do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
				while((value_temp & 0x80000000) != 0x0);
			}

			if(bl == 8 & bt == SEQ)
			{
				while(1);
			}
			mode_ba = 0x0;
			mode_a = bl == 2 ? 0x1 :
				bl == 4 ? 0x2 :
				bl == 8 ? 0x3 :
				bl == 16 ? 0x4 : 0x1;
			mode_a |= bt << 3;
			mode_a |= (_LPDDR1_CL_ == 2 ? 0x2 :
				_LPDDR1_CL_ == 3 ? 0x3 :
				_LPDDR1_CL_ == 4 ? 0x4 : 0x3) << 4;
			REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD) = 0x85f00003 | (mode_ba << 17) | (mode_a << 4);
			do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
			while((value_temp & 0x80000000) != 0x0);

			mode_ba = 0x2;
			mode_a = 0x0;
			REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD) = 0x85f00003 | (mode_ba << 17) | (mode_a << 4);
			do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
			while((value_temp & 0x80000000) != 0x0);
			
			break;
		}
		case LPDDR2:
		{
			//This register provides software with a method to program memory commands to the memory devices for
			//initialization and mode register programming through the direct memory command channel. Writes to this
			//register are ignored when MCMD[31] is set to 0x1, poll bit[31] to determine when uPCTL is ready
			//to accept another command.
			//MR63: reset memory
			REG32(UMCTL_REG_BASE  + UMCTL_CFG_ADD_MCMD) = 0x89fee3f3;
			do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
			while((value_temp & 0x80000000) != 0x0);

			#if 1//must be followed in board test
			//for (i=0; i<11000; i++); //tINIT5+tINIT4, at least 11us
			wait_n_pclk_cycle(11*1000/25);
			#endif

			//MR10: I/O calibration
			REG32(UMCTL_REG_BASE  + UMCTL_CFG_ADD_MCMD) = 0x891ff0a3;
			do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
			while((value_temp & 0x80000000) != 0x0);
			wait_n_pclk_cycle(40);

			//MR10: I/O calibration
			REG32(UMCTL_REG_BASE  + UMCTL_CFG_ADD_MCMD) = 0x892ff0a3;
			do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
			while((value_temp & 0x80000000) != 0x0);
			//for (i=0; i<1000;i++);
			wait_n_pclk_cycle(40);

			if(bl == 8 && bt == SEQ)
			{
				while(1);
			}
			//MR01: Device feature 1
			value_temp = 0x80f00013;
			value_temp |= (bl == 4 ? 0x2 :
				bl == 8 ? 0x3 :
				bl == 16 ? 0x4 : 0x3) << 12;
			value_temp |= bt << 15;
			value_temp |= _LPDDR2_WC_ << 16;
			value_temp |= 4 << 17; //nWR=6 for Auto precharge
			REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD) = value_temp;
			do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
			while((value_temp & 0x80000000) != 0x0);

			//MR02: Device feature 2
			REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD) = 0x80f04023;
			do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
			while((value_temp & 0x80000000) != 0x0);

			//MR03: I/O config-1. DS: 40 ohm typical (default)
			value_temp = 0x80f02030;
			switch (zq)
			{
				case 34:
					value_temp |= 0x1<<12;
					break;
				case 40:
					value_temp |= 0x2<<12;
					break;
				case 48:
					value_temp |= 0x3<<12;
					break;
				case 60:
					value_temp |= 0x4<<12;
					break;
				case 80:
					value_temp |= 0x6<<12;
					break;
				default:
					while(1);
			}
			REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD) = value_temp;
			do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
			while((value_temp & 0x80000000) != 0x0);

			//refresh
			REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD) = 0x80f00002;
			do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
			while((value_temp & 0x80000000) != 0x0);
			
			break;
		}
		default:{while(1);}
	}
}

static void __emc_init(uint32 mem_drv)
{
	uint32 value_temp, i;
	MEM_TYPE_ENUM mem_type_enum;
	MEM_WIDTH_ENUM mem_width_enum;
	mem_type_enum = MEMORY_TYPE;
	mem_width_enum = MEM_WIDTH;

	if (1)
	{
		value_temp  = REG32(0x4B000080);
		value_temp |= (0x1<<1);
		REG32(0x4B000080) = value_temp;
	}

	//value_temp = REG32(0x2090_0308);
	//value_temp[9] = 0x1;
	//REG32_WR(0x2090_0308,value_temp);

	emc_init_common_reg(mem_type_enum, mem_width_enum, mem_drv);
#if SOFTWARE_ENABLE_VREF
	polling_reg_bit_field(PUBL_REG_BASE+PUBL_CFG_ADD_PGSR, 0, 3, 7);//confirm zqcal done, because later will trigger it
#endif
	//for LPDDR1, Vref is initially shut down, So zq calibration must be explicitly triggered by software
	//otherwise ouput impedance will have uncorrect value, thus marking DDR signals not toggling
	if (mem_type_enum == LPDDR1)
	{
#if 0
		{
			REG32(PUBL_REG_BASE+PUBL_CFG_ADD_ZQ0CR0) = (1<<30);
			for (i=0; i<=150; i++);
			polling_reg_bit_field(PUBL_REG_BASE+PUBL_CFG_ADD_ZQ0CR0, 30, 1, 0);
			value_temp = REG32(PUBL_REG_BASE+PUBL_CFG_ADD_ZQ0SR0);
			if ((value_temp&0x0000001f) == 0x00000000)
			{
				while(1);
			}
			if ((value_temp&0x000003e0) == 0x00000000)
			{
				while(1);
			}
		}
#endif
#if SOFTWARE_ENABLE_VREF
		{
			value_temp = 0x9;
			REG32(PUBL_REG_BASE+PUBL_CFG_ADD_PIR) = value_temp;
			//according to PUBL databook on PIR operation.
			//10 configuration clock cycle must be waited before polling PGSRi
			for (i=0; i<=150; i++);
			polling_reg_bit_field(PUBL_REG_BASE+PUBL_CFG_ADD_PGSR, 0, 1, 1); //init done
			polling_reg_bit_field(PUBL_REG_BASE+PUBL_CFG_ADD_PGSR, 2, 1, 1); //zcal done
			value_temp = REG32(PUBL_REG_BASE+PUBL_CFG_ADD_ZQ0SR0);
			if ((value_temp & 0x0000001f) == 0x00000000)
			{
				while(1); //zcal error must be 0
			}
			if ((value_temp & 0x000003e0) == 0x00000000)
			{
				while(1); //zcal error must be 0
			}
		}
#endif
	}

	value_temp = 0x9;
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PIR) = value_temp;
	wait_n_pclk_cycle(5);
	for (i=0; i<100; i++);
	do value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PGSR);
	while((value_temp & 0x1) == 0);

	value_temp = (0x1<<18)|0x1; //Controller DRAM Initialization
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PIR) = value_temp;

	//according to PUBL databook on PIR operation.
	//10 configuration clock cycle must be waited before polling PGSRi
	wait_n_pclk_cycle(5);
	do value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PGSR); 
	while((value_temp & 0x1) == 0);

	//check dfi init complete
	do
	{
		value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DFISTSTAT0);
	} while((value_temp & 0x1) == 0);

	//Start the memory power up sequence
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_POWCTL) = 0x00000001;

	//Returns the status of the memory power-up sequence
	do
	{
		value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_POWSTAT);
	} while((value_temp & 0x1) == 0);
	//mem_init(mem_type_enum); //OK
	switch(mem_type_enum)
	{
		case LPDDR1:
			mem_init(mem_type_enum, _LPDDR1_BL_, _LPDDR1_BT_, mem_drv);
			break;
		case LPDDR2:
			mem_init(mem_type_enum, _LPDDR2_BL_, _LPDDR2_BT_, mem_drv);
			break;
	}

	move_upctl_state_to_config();
#if GATE_TRAING_ON
	emc_publ_do_gate_training();
#endif
	//disable emc auto self-refresh
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_SCFG) = 0x00000421;
	move_upctl_state_to_access();
}

void emc_init_repowered(uint32 power_off)
{
	uint32  value_temp, i;
	uint32  value_temp1;
	MEM_TYPE_ENUM mem_type_enum;
	MEM_WIDTH_ENUM mem_width_enum;
	mem_width_enum = MEM_WIDTH;
	//MEM_DENSITY_ENUM mem_density_enum;
	mem_type_enum = MEMORY_TYPE;

	//cfg clk emc to 200MHz if lpddr1
	//it must use AHB to config

	value_temp = REG32(0x4B000080);
	value_temp |= 0x1 << 1;
	REG32(0x4B000080) = value_temp;

	move_upctl_state_to_config();

	//value_temp = REG32(0x2090_0308);
	//value_temp[9] = 0x1;
	//REG32(0x2090_0308) = value_temp;
	if (power_off)
	{
		emc_init_common_reg(mem_type_enum, mem_width_enum, SC8825_EMC_DRV);
	}

	do  value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PGSR);
	while((value_temp & 0x1) == 0);

	value_temp = 0x0;
	value_temp |= 1 << 30; //ZCALBYP
	value_temp |= 1 << 18; //Controller DRAM Initialization
	value_temp |= 1;
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PIR) = value_temp;

	//according to PUBL databook on PIR operation.
	//10 configuration clock cycle must be waited before polling PGSR
	wait_n_pclk_cycle(5);

	do value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PGSR);
	while((value_temp & 0x1) == 0);

	do value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_DFISTSTAT0);
	while((value_temp & 0x1) == 0);

	/*
	do
	{
		value_temp = REG32(UMCTL_REG_BASE+UMCTL_CFG_ADD_DFISTSTAT0);
	} while((value_temp&1)==0);
	REG32(UMCTL_REG_BASE+UMCTL_CFG_ADD_POWCTL)=1;
	do
	{
		value_temp = REG32(UMCTL_REG_BASE+UMCTL_CFG_ADD_POWSTAT);
	} while((value_temp&1)==0);
        */

	value_temp1 = REG32(0x20900260);  //read the pre-retention value
	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_ZQ0CR0);
	value_temp &= ~0xfffff;
	value_temp &= ~(0x1 << 28);
	value_temp |= 1 << 28;
	value_temp |= (value_temp1 & 0xfffff);
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_ZQ0CR0) = value_temp;

	if (power_off == 0x0)
	{
		move_upctl_state_to_low_power();
	}

	value_temp = REG32(0x4B000080);
	value_temp &= ~(0x1 << 2);
	value_temp |= 0x1 << 2;
	REG32(0x4B000080) = value_temp;
	value_temp &= ~(0x1 << 2);
	REG32(0x4B000080) = value_temp;

	value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_ZQ0CR0);
	value_temp   &= ~(0x1 << 28);
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_ZQ0CR0) = value_temp;

	if (ZQ_CALIBRE_DPSLEEP)
	{
		value_temp = 0x1 | (0x1 << 3);
		REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PIR) = value_temp;
		for(i = 0; i < 100; i++) {}
		do value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PGSR);
		while((value_temp & 0x1) == 0);
	}

	value_temp = 0x1 | (0x1 << 18); //Controller DRAM Initialization
	REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PIR) = value_temp;
	for(i = 0; i < 100; i++) {}
	do value_temp = REG32(PUBL_REG_BASE + PUBL_CFG_ADD_PGSR);
	while((value_temp & 0x1) == 0);

	//first move upctl to low power state before issuing wakeup command
	if (power_off)
	{
		move_upctl_state_to_low_power();
	}
	move_upctl_state_to_access();
	move_upctl_state_to_config();
	if (power_off)
		emc_publ_do_gate_training();

	//disable emc auto self-refresh
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_SCFG) = 0x00000421;
	move_upctl_state_to_access();
}

static void precharge_all_bank(void)
{
	//all cs be precharged
	REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD) = 0x85f00001;
}

static void load_mode(MEM_TYPE_ENUM mem_type_enum, uint32 bl, MEM_BT_ENUM bt)
{
	uint32  mode_ba, mode_a;
	uint32  value_temp;
	switch(mem_type_enum)
	{
		case LPDDR1:
		{
			if(bl == 8 & bt == SEQ)
			{
				while(1);
			}
			mode_ba = 0x0;
			mode_a = (bl == 2 ? 0x1 :
				bl == 4 ? 0x2 :
				bl == 8 ? 0x3 :
				bl == 16 ? 0x4 : 0x1);
			mode_a |= bt << 3;
			mode_a |= (_LPDDR1_CL_ == 2 ? 0x2 :
				_LPDDR1_CL_ == 3 ? 0x3 :
				_LPDDR1_CL_ == 4 ? 0x4 : 0x3) << 4;
			REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD) = 0x85f00003 | (mode_ba << 17) | (mode_a << 4);
			do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
			while((value_temp & 0x80000000) != 0x0);
			break;
		}
		case LPDDR2:
		{
			if(bl == 8 && bt == SEQ)
			{
				while(1);
			}
			//MR01: Device feature 1
			value_temp = 0x80f00013;
			value_temp = (bl == 4 ? 0x2 :
				bl == 8 ? 0x3 :
				bl == 16 ? 0x4 : 0x3) << 12;
			value_temp |= bt << 15;
			value_temp |= _LPDDR2_WC_ << 16;
			value_temp |= 4 << 17; //nWR=6 for Auto precharge
			REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD) = value_temp;
			do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
			while((value_temp & 0x80000000) != 0x0);

			//MR02: Device feature 2
			REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD) = 0x80f04023;
			do  value_temp = REG32(UMCTL_REG_BASE + UMCTL_CFG_ADD_MCMD);
			while((value_temp & 0x80000000) != 0x0);
			break;
		}
		default:
			break;
	}
}

static void disable_clk_emc()
{
	modify_reg_field(ADDR_AHBREG_AHB_CTRL0, 28, 1, 0);
}

static void enable_clk_emc()
{
	modify_reg_field(ADDR_AHBREG_AHB_CTRL0, 28, 1, 1);
}

static void assert_reset_acdll()
{
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_ACDLLCR, 30, 1, 0);
}

static void deassert_reset_acdll()
{
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_ACDLLCR, 30, 1, 1);
}

static void assert_reset_dxdll()
{
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX0DLLCR, 30, 1, 0);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX1DLLCR, 30, 1, 0);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX2DLLCR, 30, 1, 0);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX3DLLCR, 30, 1, 0);
}

static void deassert_reset_dxdll()
{
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX0DLLCR, 30, 1, 1);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX1DLLCR, 30, 1, 1);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX2DLLCR, 30, 1, 1);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX3DLLCR, 30, 1, 1);
}

static void assert_reset_ddrphy_dll()
{
	assert_reset_acdll();
	assert_reset_dxdll();
}

static void deassert_reset_ddrphy_dll()
{
	deassert_reset_acdll();
	deassert_reset_dxdll();
}

static void modify_dpll_freq(uint32 freq)
{
	uint32 temp;
	modify_reg_field(GLB_REG_WR_REG_GEN1, 9, 1, 1);
	temp = (freq >> 2);
	modify_reg_field(GLB_REG_DPLL_CTRL, 0, 11, temp);
	modify_reg_field(GLB_REG_WR_REG_GEN1, 9, 1, 0);
}


static void modify_emc_clk(uint32 freq)
{
	disable_clk_emc();
	assert_reset_acdll();
	assert_reset_dxdll();
	modify_dpll_freq(freq);

	modify_reg_field(ADDR_AHBREG_ARMCLK, 12, 2, 1);
	modify_reg_field(ADDR_AHBREG_ARMCLK, 8, 4, 0);	//clk emc div 0
	modify_reg_field(ADDR_AHBREG_ARMCLK, 3, 1, 1);	//clk emc sync
	
	wait_us(150);

	enable_clk_emc();
	deassert_reset_acdll();
	deassert_reset_dxdll();
	wait_us(10);
}

static void reset_ddrphy_dll()
{
	disable_clk_emc();
	assert_reset_acdll();
	assert_reset_dxdll();

	enable_clk_emc();
	deassert_reset_acdll();
	deassert_reset_dxdll();
	wait_us(10);
}

static void modify_dxndll_phase_trim(uint32 dxn, uint32 phase)
{
	uint32 phase_dec;
	uint32 publ_cfg_dxndllcr_addr, i;
	publ_cfg_dxndllcr_addr = PUBL_REG_BASE+PUBL_CFG_ADD_DX0DLLCR + (0x10*4*dxn);
	switch(phase)
	{
		case 36:
			phase_dec = 3;
			break; 
		case 54:
			phase_dec = 2;
			break; 
		case 72:
			phase_dec = 1;
			break; 
		case 90:
			phase_dec = 0;
			break; 
		case 108:
			phase_dec = 4;
			break; 
		case 126:
			phase_dec = 8;
			break; 
		case 144:
			phase_dec = 12;
			break; 
		default:
			while(1);
	}
	modify_reg_field(publ_cfg_dxndllcr_addr, 14, 4, phase_dec);
	modify_reg_field(publ_cfg_dxndllcr_addr, 30, 1, 0);
	
	wait_100ns();
	modify_reg_field(publ_cfg_dxndllcr_addr, 30, 1, 1);
	if (0)
	{
		wait_n_pclk_cycle(10*1000/25);
	}
	else
	{
		polling_reg_bit_field(PUBL_REG_BASE+PUBL_CFG_ADD_PGSR, 0, 1, 1);
		REG32(PUBL_REG_BASE+PUBL_CFG_ADD_PIR) = 0x5;
		wait_n_pclk_cycle(5);
		polling_reg_bit_field(PUBL_REG_BASE+PUBL_CFG_ADD_PGSR, 2, 1, 1);
	}
}

static void modify_dxndqstr_delay_trim(uint32 dxn, uint32 trim_value)
{
	uint32 publ_cfg_dxndllcr_addr, i;
	publ_cfg_dxndllcr_addr = PUBL_REG_BASE+PUBL_CFG_ADD_DX0DLLCR+(0x10*4*dxn);
	if (trim_value > 7)
		while(1);
	switch(dxn)
	{
		case 0:
			modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX0DQSTR, 20, 3, trim_value);
			modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX0DQSTR, 22, 3, trim_value);
			break;
		case 1:
			modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX1DQSTR, 20, 3, trim_value);
			modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX1DQSTR, 22, 3, trim_value);
			break;
		case 2:
			modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX2DQSTR, 20, 3, trim_value);
			modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX2DQSTR, 22, 3, trim_value);
			break;
		case 3:
			modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX3DQSTR, 20, 3, trim_value);
			modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_DX3DQSTR, 22, 3, trim_value);
			break;
		default:
			while(1);
	}
	modify_reg_field(publ_cfg_dxndllcr_addr, 30, 1, 0);
	wait_100ns();
	modify_reg_field(publ_cfg_dxndllcr_addr, 30, 1, 1);
	if (0)
	{
		wait_n_pclk_cycle(10*1000/25);
	}
	else
	{
		polling_reg_bit_field(PUBL_REG_BASE+PUBL_CFG_ADD_PGSR, 0, 1, 1);
		REG32(PUBL_REG_BASE+PUBL_CFG_ADD_PIR) = 0x5;
		wait_n_pclk_cycle(5);
		polling_reg_bit_field(PUBL_REG_BASE+PUBL_CFG_ADD_PGSR, 2, 1, 1);
	}
}

static void publ_do_itmsrst()
{
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_PIR, 4, 1, 1);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_PIR, 0, 1, 1);
	polling_reg_bit_field(PUBL_REG_BASE+PUBL_CFG_ADD_PIR, 0, 1, 0);
}

static void publ_do_bist_stop()
{
	uint32 i;
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR, 0, 3, 2);
	for (i=0; i<24; i++);
}

static void publ_do_bist_reset()
{
	uint32 i;
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR, 0, 3, 3);
	for (i=0; i<24; i++);
}

static void publ_record_bistgsr(uint32 result_addr)
{
	uint32 value_temp, addr;
	value_temp = 0;
	addr = result_addr + (1<<10);
	while ((value_temp & 0x1) == 0)
		value_temp = REG32(PUBL_REG_BASE+PUBL_CFG_ADD_BISTGSR);
	REG32(result_addr) = value_temp;

	if (value_temp == 0x1)
		REG32(addr) = 0x0000ba55;
	else
		REG32(addr) = 0x0000fa11;
}

static void publ_set_dqs_phase_trim_to_norminal(uint32 dxn)
{
	uint32 publ_dxn_cfg_dxndllcr_addr;
	uint32 publ_dxn_cfg_dxndqstr_addr;
	uint32 i;
	
	publ_dxn_cfg_dxndllcr_addr = PUBL_REG_BASE+PUBL_CFG_ADD_DX0DLLCR+(0x10*4*dxn);
	publ_dxn_cfg_dxndqstr_addr = PUBL_REG_BASE+PUBL_CFG_ADD_DX0DQSTR+(0x10*4*dxn);
	
	modify_reg_field(PUBL_REG_BASE+publ_dxn_cfg_dxndllcr_addr, 14, 4, 0);
	
	modify_reg_field(publ_dxn_cfg_dxndllcr_addr, 30, 1, 0);
	
	wait_n_pclk_cycle(100/25);
	
	modify_reg_field(publ_dxn_cfg_dxndllcr_addr, 30, 1, 1);
	if (0)
	{
		wait_n_pclk_cycle(10*1000/25);
	}
	else
	{
		polling_reg_bit_field(PUBL_REG_BASE+PUBL_CFG_ADD_PGSR, 0, 1, 1);
		REG32(PUBL_REG_BASE+PUBL_CFG_ADD_PIR) = 0x5;
		wait_n_pclk_cycle(5);
		polling_reg_bit_field(PUBL_REG_BASE+PUBL_CFG_ADD_PGSR, 2, 1, 1);
	}
	
	modify_reg_field(PUBL_REG_BASE+publ_dxn_cfg_dxndqstr_addr, 20, 3, 3);
	modify_reg_field(PUBL_REG_BASE+publ_dxn_cfg_dxndqstr_addr, 23, 3, 3);
}

static void publ_dram_bist(uint32 start_col, uint32 start_row, uint32 start_bank, uint32 start_cs,
			uint32 end_col, uint32 end_row, uint32 end_bank, uint32 end_cs,
			uint32 byte_lane,
			uint32 infinite_mode)
{
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTAR0, 0,  12, start_col);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTAR0, 12, 16, start_row);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTAR0, 28,  4, start_bank);

	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTAR1, 0,  2, start_cs);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTAR1, 2,  2, end_cs);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTAR1, 4, 12, 4);

	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTAR2, 0,  12, end_col);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTAR2, 12, 16, end_row);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTAR2, 28,  4, end_bank);

	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR,  3,  1, 1);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR,  4,  1, infinite_mode);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR,  5,  8, 1);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR, 13,  1, 1);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR, 14,  2, 1);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR, 17,  2, 2);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR, 19,  4, byte_lane);

	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTWCR, 0, 16, 32764);

	REG32(PUBL_REG_BASE+PUBL_CFG_ADD_BISTLSR) = 0x1234abcd;
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR, 0, 3, 1);
}

static void publ_phy_lb_bist(uint32 dxn, uint32 lbdqss, uint32 iolb)
{
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_PGCR, 19, 1, iolb);

	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_PGCR, 29, 1, lbdqss);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR, 14, 2, 1);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR, 17, 2, 2);
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR, 19, 4, dxn);
	
	REG32(PUBL_REG_BASE+PUBL_CFG_ADD_BISTLSR) = 0x1234abcd;
	modify_reg_field(PUBL_REG_BASE+PUBL_CFG_ADD_BISTRR, 0, 3, 1);
}

static void publ_disable_dxn(uint32 dxn)
{
	uint32 publ_cfg_add_dxngcr;
	publ_cfg_add_dxngcr = PUBL_CFG_ADD_DX0GCR+(0x10*4*dxn);
	modify_reg_field(PUBL_REG_BASE+publ_cfg_add_dxngcr, 0, 1, 0);
}

static void publ_enable_dxn(uint32 dxn)
{
	uint32 publ_cfg_add_dxngcr;
	publ_cfg_add_dxngcr = PUBL_CFG_ADD_DX0GCR+(0x10*4*dxn);
	modify_reg_field(PUBL_REG_BASE+publ_cfg_add_dxngcr, 0, 1, 1);
}

static void set_ddr_phy_vref(void)
{
	uint32 i;
	modify_adie_reg_field(ANA_DDR2_BUF_CTRL1_DS, 0, 4, 0xa);
	for (i=0; i<10; i++);
}

#if 0
uint32 cal_sdll_dly_param(SDLL_PHS_DLY_E sdll_phs_dly)
{
	switch (sdll_phs_dly)
	{
		case SDLL_PHS_DLY_36:  return 3;
		case SDLL_PHS_DLY_54:  return 2;
		case SDLL_PHS_DLY_72:  return 1;
		case SDLL_PHS_DLY_90:  return 0;
		case SDLL_PHS_DLY_108: return 4;
		case SDLL_PHS_DLY_126: return 8;
		case SDLL_PHS_DLY_144: return 12;
		default:               return 0;
	}
}

void set_dqs_pt_gsl_gps_dly(DXN_E dxn, DQS_PHS_DLY_E dqs_step_dly, SDLL_PHS_DLY_E sdll_phs_dly)
{
	uint32 dxndqstr_addr = PUBL_REG_BASE+PUBL_CFG_ADD_DX0DQSTR + 0x10*4*dxn;
	uint32 dxndllcr_addr = PUBL_REG_BASE+PUBL_CFG_ADD_DX0DLLCR + 0x10*4*dxn;

	modify_reg_field(dxndqstr_addr, 20, 3, dqs_step_dly);
	modify_reg_field(dxndqstr_addr, 22, 3, dqs_step_dly);
	modify_reg_field(dxndllcr_addr, 14, 4, cal_sdll_dly_param(sdll_phs_dly));
	modify_reg_field(dxndllcr_addr, 30, 1, 0);

	//wait some time for DLL reset
	wait_100ns();
	modify_reg_field(dxndllcr_addr, 30, 1, 1);
	wait_us(10);
}
void DMC_Init(const uint32 drv_strength, const uint32 sdll_phase, const uint32 dqs_step)
#else
void DMC_Init(uint32 emc_freq, uint32 mem_drv, uint32 sdll_phase, uint32 dqs_step)
#endif
{
	uint32 i=0;

#if SOFTWARE_ENABLE_VREF
	set_ddrphy_vref();
#endif

	if (emc_freq>=100 && emc_freq<=400)
		modify_emc_clk(emc_freq);
	else
		modify_emc_clk(SC8825_EMC_FREQ);

	for (i=0; i<1000; i++);
#if 0
	if (drv_strength >= 34 && drv_strength <=80)
		__emc_init(mem_drv);
	else
#endif
		__emc_init(SC8825_EMC_DRV);
#if 0
	if (drv_strength != 0)
	{
		set_dqs_pt_gsl_gps_dly(0, sdll_phase&0xff,       dqs_step&0xff); 
		set_dqs_pt_gsl_gps_dly(1, (sdll_phase>>8)&0xff,  (dqs_step>>8)&0xff);
		set_dqs_pt_gsl_gps_dly(2, (sdll_phase>>16)&0xff, (dqs_step>>16)&0xff);
		set_dqs_pt_gsl_gps_dly(3, (sdll_phase>>24)&0xff, (dqs_step>>24)&0xff);
	}
#endif
}

