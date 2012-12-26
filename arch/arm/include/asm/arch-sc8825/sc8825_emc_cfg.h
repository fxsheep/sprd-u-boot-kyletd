/******************************************************************************
 ** File Name:      sc8825_emc_cfg.h                                          *
 ** Author:         Johnny.Wang                                               *
 ** DATE:           2012/12/04                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
#include "sci_types.h"
#include "sc_reg.h"


/******************************************************************************
                          junior Macro define
******************************************************************************/
#define UL_ONEBITS 0xffffffff
#define UL_LEN 32
#define ONE 0x00000001



/******************************************************************************
                            Enum define
******************************************************************************/
typedef enum 
{
	CTL_STATE_INIT_MEM,
	CTL_STATE_CONFIG,
	CTL_STATE_CONFIG_REQ,
	CTL_STATE_ACCESS,
	CTL_STATE_ACCESS_REQ,
	CTL_STATE_LOW_POWER,
	CTL_STATE_LOW_POWER_ENTRY_REQ,
	CTL_STATE_LOW_POWER_EXIT_REQ,
}EMC_CTL_STATE_E; 

typedef enum 
{
	CTL_CMD_INIT,
	CTL_CMD_CFG,
	CTL_CMD_GO,
	CTL_CMD_SLEEP,
	CTL_CMD_WAKEUP,
}EMC_CTL_CMD_E;


typedef enum
{
	DRAM_LPDDR1 	= 1,
	DRAM_LPDDR2_S2	= 2,
	DRAM_LPDDR2_S4	= 3,
	DRAM_LPDDR2		= 0x2,
	DRAM_DDR3		= 4
}DRAM_TYPE_E;

typedef enum
{
	DRAM_0BIT       = 0,
	DRAM_64MBIT		= 0x00800000,
	DRAM_128MBIT    = 0x01000000,
	DRAM_256MBIT    = 0x02000000,
	DRAM_512MBIT    = 0x04000000,
	DRAM_1GBIT      = 0x08000000,	
	DRAM_2GBIT      = 0x10000000,		
	DRAM_4GBIT      = 0x20000000,	
	DRAM_8GBIT      = 0x40000000,
	DRAM_16GBIT     = 0x80000000,	
}DRAM_DENSITY_E;

typedef enum
{
	ONE_CS 			= 1,
	TWO_CS 			= 2,
	FIRST_CS		= 1,
	SECOND_CS 		= 2,	
	THIRD_CS 		= 4,
	FORTH_CS 		= 8,
	ALL_TWO_CS 		= 3,
	ALL_THREE_CS 	= 7,
	ALL_FOUR_CS 	= 0XF,
}DRAM_CS_NUM_E;

typedef enum
{
	DRAM_BL2 = 0,
	DRAM_BL4 = 1,
	DRAM_BL8 = 2,
	DRAM_BL16 = 3
}DRAM_BL_E;

typedef enum
{
	DRAM_BT_SEQ = 0, //burst type = sequential(default)
	DRAM_BT_INT = 1  //burst type = interleaved
}DRAM_BT_E;

typedef enum
{
	DRAM_WRAP    = 0, //warp mode
	DRAM_NO_WRAP = 1  //no warp mode
}DRAM_WC_E;

typedef enum
{
	LPDDR1_CL0 = 0,
	LPDDR1_CL2 = 2,
	LPDDR1_CL3 = 3,
	LPDDR1_CL4 = 4,
	LPDDR2_RL3 = 3,
	LPDDR2_RL4 = 4,
	LPDDR2_RL5 = 5,
	LPDDR2_RL6 = 6,
	LPDDR2_RL7 = 7,
	LPDDR2_RL8 = 8,
	LPDDR2_WL1 = 1,
	LPDDR2_WL2 = 2,
	LPDDR2_WL3 = 3,
	LPDDR2_WL4 = 4
}DRAM_CL_E;

typedef enum
{
	IO_WIDTH_8  = 1,
	IO_WIDTH_16 = 2,
	IO_WIDTH_32 = 3
}IO_WIDTH_E;

typedef enum{
    CLK_24MHZ    = 24000000,
    CLK_26MHZ    = 26000000,
    CLK_38_4MHZ  = 38400000,
    CLK_48MHZ    = 48000000,
    CLK_64MHZ    = 64000000,
    CLK_76_8MHZ  = 76800000,
    CLK_96MHZ    = 96000000,
    CLK_100MHZ   = 100000000,    
    CLK_150MHZ   = 150000000,        
    CLK_153_6MHZ = 153600000,
    CLK_192MHZ   = 192000000,
	CLK_200MHZ   = 200000000,    
    CLK_333MHZ   = 333000000,
    CLK_400MHZ   = 400000000,
    CLK_427MHZ   = 427000000,
    CLK_450MHZ   = 450000000,
    CLK_500MHZ   = 500000000,
    CLK_525MHZ   = 525000000,
    CLK_537MHZ   = 537000000,
    CLK_540MHZ   = 540000000,
    CLK_550MHZ   = 550000000,
    CLK_800MHZ	 = 800000000,
    CLK_1000MHZ	 = 1000000000,
    EMC_CLK_400MHZ = 400000000,
    EMC_CLK_450MHZ = 450000000,
    EMC_CLK_500MHZ = 500000000
}CLK_TYPE_E;


typedef enum EMC_CS_NUM_TAG
{
    EMC_CS0 = 0,
    EMC_CS1
}
EMC_CS_NUM_E;

typedef enum EMC_PORT_NUM_TAG
{
    EMC_PORT_MIN    = 0,
    EMC_PORT0_AP    = 0,
    EMC_PORT1_GPU   = 1,
    EMC_PORT2_MST   = 2,
    EMC_PORT3_DSPP  = 3,
    EMC_PORT4_DSPD  = 4,
    EMC_PORT5_DISP  = 5,
    EMC_PORT6_MM    = 6,
    EMC_PORT7_CP    = 7,
	EMC_PORT_MAX    = 8
}
EMC_PORT_NUM_E;

typedef enum
{
	RANK_BANK_ROW_COL = 0x000,
	RANK_ROW_BANK_COL = 0x100,
	BANK_ROW_RANK_COL = 0x200
}EMC_ADDR_MAP_TYPE_E;

typedef enum
{
	EMC_PORT_BE = 0, //Best effot, low priority
	EMC_PORT_LL = 1, //Low latnecy,high priority
	EMC_PORT_DD = 2	 //dynamic determind,normal priority
}EMC_PORT_PRIORITY_E;

typedef enum
{
	PHY_ACT_INIT 	= BIT_0,	//trigger DDR system initialization,include PHY initialization,DRAM initialization,and PHY training
	PHY_ACT_DLLSRST	= BIT_1,	//dll soft reset
	PHY_ACT_DLLOCK 	= BIT_2,	//waite dll lock done
	PHY_ACT_ZCAL 	= BIT_3,	//impedance calibrate,perform PHY impedance calibration
	PHY_ACT_ITMSRST = BIT_4,	//interface timing module soft reset
	PHY_ACT_DDR3RST = BIT_5,	//ddr3 reset
	PHY_ACT_DRAMINIT= BIT_6,	//excute DRAM initialization
	PHY_ACT_DQSTRN	= BIT_7,	//excute dqs training routine
	PHY_ACT_EYETRN	= BIT_8,	//read data eye training,not support in this phy version
	PHY_ACT_ICPC	= BIT_16,	//initialization complete pin configuration
	PHY_ACT_DLLBYP	= BIT_17,	//DLL bypass
	PHY_ACT_CTLDINIT= BIT_18,	//Control DRAM initialization,if set DRAM initialization will be excute by controller,
	PHY_ACT_CLRSR	= BIT_28,	//Clear all status register,include PGSR and DXnGSR
	PHY_ACT_LOCKBYP = BIT_29,	//DLL lock bypass,if set,DLL lock wait will auto tigger after reset
	PHY_ACT_ZCALBYP = BIT_30,	//impedance calibration bypass, if set, impedance calibration will auto tigger after reset
	PHY_ACT_INITBYP = BIT_31 	//Initialization bypass
}EMC_PHY_ACT_E;

typedef enum
{
	PHY_STATE_INIT_DONE 	= 0x1,
	PHY_STATE_DLL_LOCK_DONE = 0x2,
	PHY_STATE_ZQCL_DONE 	= 0x4,
	PHY_STATE_DRAM_INIT_DONE= 0x8,
	PHY_STATE_DTDONE		= 0x10,
	PHY_STATE_DTERR			= 0x20,
	PHY_STATE_DTIERR		= 0x40,
	PHY_STATE_DRIFT_ERR		= 0x80,
	PHY_STATE_TQ			= 0x80000000,
}EMC_PHY_STATE_E;


typedef enum
{
	MEM_ACCESS_BYTE  = 1,
	MEM_ACCESS_HWORD = 2,
	MEM_ACCESS_WORD =  4
}MEM_ACCESS_TYPE_E;

typedef enum
{
	TWO_BANK   = 2,
	FOUR_BANK  = 4,
	EIGHT_BANK = 8
}MEM_BANK_NUM_E;

typedef enum
{
	DQS_STEP_DLY_MIN  = 0,
	DQS_STEP_DLY_SUB3 = 0,
	DQS_STEP_DLY_SUB2 = 1,
	DQS_STEP_DLY_SUB1 = 2,
	DQS_STEP_DLY_NOM  = 3,
	DQS_STEP_DLY_DEF  = 3,
	DQS_STEP_DLY_ADD1 = 4,
	DQS_STEP_DLY_ADD2 = 5,
	DQS_STEP_DLY_ADD3 = 6,
	DQS_STEP_DLY_ADD4 = 7,
	DQS_STEP_DLY_MAX  = 7
}DQS_STEP_DLY_E;

//DQS gating phase select
typedef enum
{
	DQS_PHS_DLY_MIN = 0,
	DQS_PHS_DLY_90  = 0,
	DQS_PHS_DLY_180 = 1,
	DQS_PHS_DLY_DEF = 1,
	DQS_PHS_DLY_270 = 2,
	DQS_PHS_DLY_360 = 3,
	DQS_PHS_DLY_MAX = 3
}DQS_PHS_DLY_E;

//DQS gating system latency
typedef enum
{
	DQS_CLK_DLY_MIN  = 0,
	DQS_CLK_DLY_DEF  = 0,
	DQS_CLK_DLY_1CLK = 1,
	DQS_CLK_DLY_2CLK = 2,
	DQS_CLK_DLY_3CLK = 3,
	DQS_CLK_DLY_4CLK = 4,
	DQS_CLK_DLY_5CLK = 5,
	DQS_CLK_DLY_MAX  = 5	
}DQS_CLK_DLY_E;

//slave dll phase trim
typedef enum
{
	SDLL_PHS_DLY_DEF  = 0x0,
	SDLL_PHS_DLY_36   = 0x3,
	SDLL_PHS_DLY_54   = 0x2,
	SDLL_PHS_DLY_72   = 0x1,
	SDLL_PHS_DLY_90   = 0x0,
	SDLL_PHS_DLY_108  = 0x4,
	SDLL_PHS_DLY_126  = 0x8,
	SDLL_PHS_DLY_144  = 0x12
}SDLL_PHS_DLY_E;

typedef enum
{
	LPDDR2_DS_34_OHM = 0xd,
	LPDDR2_DS_40_OHM = 0xb,
	LPDDR2_DS_48_OHM = 0x9,
	LPDDR2_DS_60_OHM = 0x7,
	LPDDR2_DS_80_OHM = 0x5
}LPDDR2_MEM_DS_T_E;

typedef enum
{
	LPDDR1_DS_33_OHM = 0xa,
	LPDDR1_DS_31_OHM = 0xb,
	LPDDR1_DS_48_OHM = 0xc,
	LPDDR1_DS_43_OHM = 0xd,
	LPDDR1_DS_39_OHM = 0xe,
	LPDDR1_DS_55_OHM = 0x5,
	LPDDR1_DS_64_OHM = 0x4
}LPDDR1_MEM_DS_T_E;

typedef enum
{
	DQS_PDU_MIN    = 1,
	DQS_PDU_688ohm = 1,
	DQS_PDU_611ohm = 2,
	DQS_PDU_550ohm = 3,
	DQS_PDU_500ohm = 4,
	DQS_PDU_DEF    = 4,
	DQS_PDU_458ohm = 5,
	DQS_PDU_393ohm = 6,
	DQS_PDU_344ohm = 7,	
	DQS_PDU_MAX    = 7
}DQS_PDU_E;

typedef enum
{
	CMD_NOP 	= 0,
	CMD_PREA	= 1,
	CMD_REF		= 2,
	CMD_MRS		= 3,
	CMD_ZQCS	= 4,
	CMD_ZQCL	= 5,
	CMD_RSTL	= 6,
	CMD_MRR		= 8,
	CMD_DPDE	= 9,
	CMD_ERR		= 0XA
}MEM_CMD_TYPE_E;

typedef enum
{
	CMD_MDR_NOT_EXIT = 0,
	CMD_MDR_RD_ONLY = 1,
	CMD_MDR_WR_ONLY = 2,
	CMD_MDR_NOP		= 3,
	CMD_MDR_SUCCESS = 4
}MEM_CMD_RESULT_E;


/******************************************************************************
                            Structure define
******************************************************************************/
typedef struct
{
	// timing for lpddr1 and lpddr2
	uint32 tREFI;	// average Refresh interval time between each row,normall = 7800 ns	
	uint32 tRAS;    // ACTIVE to PERCHARGE command period	
	uint32 tRC;     // ACTIVE to ACTIVE command period	
	uint32 tRFC;    // AUTO REFRESH to ACTIVE/AUTO REFRESH command period	
	uint32 tRCD;    // ACTIVE to READ/WRITE delay	
	uint32 tRP;		// PRECHARGE command period	
	uint32 tRRD;	// ACTIVE to ACTIVE delay	
	uint32 tWR;     // WRITE recovery time
	uint32 tWTR;	// internal write to read command delay	
	uint32 tXSR;    // Self Refresh Exit to next valid command delay	
	uint32 tXP;     // Exit Power Down to next valid command delay	
	// timing for lpddr2 and ddr3
	uint32 tMRR;	// MODE REGISTR READ command period
	uint32 tCKESR;	// CKE signal min pulse width during self-refresh
	uint32 tZQCS;	// ZQ Calibration short time
	uint32 tZQCL;	// ZQ Calibration long time
}DRAM_TIMING_INFO_T, *DRAM_TIMING_INFO_T_PTR;

typedef struct 
{
    DRAM_TYPE_E 	mem_type;	//dram type: lpddr1,lpddr2-s2,lpddr2-s4
	uint32 			cs_num;		//cs number summary,should be 1 or 2
    MEM_BANK_NUM_E	bank_num;	//bank number,lpddr1 and lpddr2 usually 4,ddr3 usually 8	
    DRAM_DENSITY_E 	cs0_cap;	//cs0 density
    DRAM_DENSITY_E 	cs1_cap;	//cs1 density

	IO_WIDTH_E		io_width;   //data io width, usually=16 or 32
	DRAM_BL_E		bl;			//burst lenght,usually=2,4,8,16
	DRAM_CL_E 		rl;  		//read cas latency, usually=1,2,3,4,5,6,7,8
	DRAM_CL_E 		wl;  		//write cas latency, usually=1,2,3,4,5,6,7,8	
} DRAM_MODE_INFO_T, *DRAM_MODE_INFO_T_PTR;


typedef struct
{
	char* chip_name;
	DRAM_TIMING_INFO_T_PTR time_info;
	DRAM_MODE_INFO_T_PTR mode_info;
}
DRAM_INFO_T, *DRAM_INFO_T_PTR;

#define RDWR_ORDER_OFF FALSE
#define RDWR_ORDER_ON  TRUE
typedef struct
{
	uint32 port_data_quantum;
	uint32 rdwr_order;
	EMC_PORT_PRIORITY_E port_priority;
}EMC_CHN_INFO_T, *EMC_CHN_INFO_T_PTR;


typedef struct
{
	BOOLEAN INIT_DONE;
	BOOLEAN DLL_LOCK_DONE;
	BOOLEAN ZC_DONE;
	BOOLEAN DRAM_INIT_DONE;
	BOOLEAN DATA_TR_DONE;
	BOOLEAN DATA_TR_ERR;
	BOOLEAN DATA_TR_INTER_ERR;
	BOOLEAN DQS_DRIFT_ERROR;
}EMC_PHY_STATUS_T,*EMC_PHY_STATUS_T_PTR;

typedef struct
{
//	MEM_DS_T_E 		mem_ds;
	SDLL_PHS_DLY_E 	sdll_phase_b0;
	SDLL_PHS_DLY_E 	sdll_phase_b1;
	SDLL_PHS_DLY_E 	sdll_phase_b2;
	SDLL_PHS_DLY_E 	sdll_phase_b3;	
	DQS_STEP_DLY_E 	dqs_step_b0;
	DQS_STEP_DLY_E 	dqs_step_b1;
	DQS_STEP_DLY_E 	dqs_step_b2;
	DQS_STEP_DLY_E 	dqs_step_b3;		
}ADJ_PAR_T,*ADJ_PAR_T_PTR;



/*******************************************************************************
                           Variable and Array definiation
*******************************************************************************/
#define DQS_GATE_EXTEN 2
#define DQS_GATE_EARLY 2	                      
#define DRAM_BURST_TYPE DRAM_BT_SEQ
#define DRAM_BURST_WRAP DRAM_NO_WRAP
#define NONE_MDR 0XFF
#define DQS_PDU_RES DQS_PDU_500ohm	//dqs pull up and pull down resist

#define EMC_SMALL_CODE_SIZE 
const EMC_CHN_INFO_T   EMC_CHN_INFO_ARRAY[EMC_PORT_MAX] = {{0x10,RDWR_ORDER_OFF,EMC_PORT_BE},//AP port set
											    {0x10,RDWR_ORDER_OFF,EMC_PORT_BE},//GPU port set
											    {0x10,RDWR_ORDER_ON, EMC_PORT_BE},//MST port set
											    {0x10,RDWR_ORDER_ON, EMC_PORT_LL},//DSPP port set
											    {0x10,RDWR_ORDER_ON, EMC_PORT_LL},//DSPD port set
											    {0x10,RDWR_ORDER_OFF,EMC_PORT_LL},//DISP port set
											    {0x10,RDWR_ORDER_OFF,EMC_PORT_LL},//MM port set
											    {0x10,RDWR_ORDER_ON, EMC_PORT_BE}};//CP port set
const char* DRAM_CHIP_NAME_INFO_ARRAY[] =
{
//	"NORMAL_LPDDR1_1CS_1G_32BIT",
	"NORMAL_LPDDR1_1CS_2G_32BIT",
	"NORMAL_LPDDR1_2CS_4G_32BIT",
	"NORMAL_LPDDR2_1CS_4G_32BIT",
	"NORMAL_LPDDR2_2CS_8G_32BIT"
//	"HYNIX_LPDDR1_H9DA4GH4JJAMCR4EM",
//	"SAMSUNG_LPDDR2_KMKJS000VM"
};

const DRAM_MODE_INFO_T DRAM_MODE_INFO_ARRAY[] =
{
//{DRAM_LPDDR1,   ONE_CS,FOUR_BANK,DRAM_1GBIT,DRAM_0BIT,  IO_WIDTH_32,DRAM_BL2,LPDDR1_CL3,LPDDR1_CL0},//NORMAL_LPDDR1_1CS_1G_32BIT
{DRAM_LPDDR1,   ONE_CS,FOUR_BANK,DRAM_2GBIT,DRAM_0BIT,  IO_WIDTH_32,DRAM_BL2,LPDDR1_CL3,LPDDR1_CL0},//NORMAL_LPDDR1_1CS_2G_32BIT    
{DRAM_LPDDR1,   TWO_CS,FOUR_BANK,DRAM_2GBIT,DRAM_2GBIT, IO_WIDTH_32,DRAM_BL2,LPDDR1_CL3,LPDDR1_CL0},//NORMAL_LPDDR1_2CS_4G_32BIT
{DRAM_LPDDR2_S4,ONE_CS,FOUR_BANK,DRAM_4GBIT,DRAM_0BIT,  IO_WIDTH_32,DRAM_BL4,LPDDR2_RL6,LPDDR2_WL3},//NORMAL_LPDDR2_1CS_4G_32BIT
{DRAM_LPDDR2_S4,TWO_CS,FOUR_BANK,DRAM_4GBIT,DRAM_4GBIT, IO_WIDTH_32,DRAM_BL4,LPDDR2_RL6,LPDDR2_WL3} //NORMAL_LPDDR2_2CS_8G_32BIT
//{DRAM_LPDDR1,   TWO_CS,FOUR_BANK,DRAM_2GBIT,DRAM_2GBIT, IO_WIDTH_32,DRAM_BL2,LPDDR1_CL3,LPDDR1_CL0},//HYNIX_LPDDR1_H9DA4GH4JJAMCR4EM
//{DRAM_LPDDR2_S4,TWO_CS,FOUR_BANK,DRAM_4GBIT,DRAM_4GBIT, IO_WIDTH_32,DRAM_BL4,LPDDR2_RL6,LPDDR2_WL3} //SAMSUNG_LPDDR2_KMKJS000VM
};


const DRAM_TIMING_INFO_T DRAM_TIMING_INFO_ARRAY[] =
{
//  ns	 ns	     ns  tRFC(ns) ns  tRP(ns) ns   ns  clk  ns   ns  clk  ns     ns    ns
//  tREFI tRAS    tRC  /tRFCab tRCD /tRPpb tRRD tWR tWTR tXSR tXP tMRR tCKESR tZQCS tZQCL
//	{7800, 50, 	80, 110, 	30,  30,    15,  15, 3,   140, 20, 0,  0, 		0, 	  0},//NORMAL_LPDDR1_1CS_1G_32BIT
	{7800, 50, 	80, 90,  	30,  30,    15,  15, 3,   140, 20, 0,  0, 		0, 	  0},//NORMAL_LPDDR1_1CS_2G_32BIT
	{7800, 50, 	80, 90,  	30,  30,    15,  15, 3,   140, 20, 0,  0, 		0,    0},//NORMAL_LPDDR1_2CS_4G_32BIT
	{3900, 50, 	80, 130,  	20,  30,    15,  15, 3,   140, 20, 2,  15,	   90,  360},//NORMAL_LPDDR2_1CS_4G_32BIT			
	{3900, 50, 	80, 130,  	20,  30,    15,  15, 3,   140, 20, 2,  15,	   90,  360} //NORMAL_LPDDR2_2CS_8G_32BIT				
//	{7800, 50, 	80, 90,  	30,  30,    15,  15, 3,   140, 20, 0,  0, 		0,    0},//HYNIX_LPDDR1_H9DA4GH4JJAMCR4EM
//	{3900, 50, 	80, 130,  	20,  30,    15,  15, 3,   140, 20, 2,  15,	   90,  360} //SAMSUNG_LPDDR2_KMKJS000VM	
};

/*******************************************************************************
                          Function declare
*******************************************************************************/

