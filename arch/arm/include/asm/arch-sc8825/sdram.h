#ifndef _SDRAM_H_
#define _SDRAM_H_

#if defined(CONFIG_SC8810)
typedef enum EMC_ENDIAN_SWITCH_TAG
{
    EMC_ENDIAN_SWITCH_NONE = 3,
    EMC_ENDIAN_SWITCH_BYTE = 0,
    EMC_ENDIAN_SWITCH_HALF = 1,
    EMC_ENDIAN_SWITCH_WORD = 2,    
}
EMC_ENDIAN_SWITCH_E;

typedef enum EMC_DVC_ENDIAN_TAG
{
    EMC_DVC_ENDIAN_DEFAULT = 0,
    EMC_DVC_ENDIAN_LITTLE = 0,
    EMC_DVC_ENDIAN_BIG = 1
}
EMC_DVC_ENAIDN_E;

typedef enum EMC_AUTO_GATE_TAG
{
    EMC_AUTO_GATE_DEFAULT = 0,
    EMC_AUTO_GATE_DIS = 0,
    EMC_AUTO_GATE_EN = 1
}
EMC_AUTO_GATE_E;

typedef enum EMC_AUTO_SLEEP_TAG
{
    EMC_AUTO_SLEEP_DEFAULT = 0,
    EMC_AUTO_SLEEP_DIS = 0,
    EMC_AUTO_SLEEP_EN = 1
}
EMC_AUTO_SLEEP_E;

typedef enum EMC_CMD_QUEUE_TAG
{
    EMC_2DB = 0,		// 2 stage device burst	
    EMC_2DB_1CB,		// 2-stage device burst and 1-stage channel burst
    EMC_2DB_2CB		// 2-stage device burst and 2-stage channel burst
}
EMC_CMD_QUEUE_E;

typedef enum EMC_CS_MODE_TAG
{
    EMC_CS_MODE_DEFAULT = 0,
    EMC_CS0_ENLARGE = 1,
    EMC_CS1_ENLARGE = 2
}
EMC_CS_MODE_E;

typedef enum EMC_CS_MAP_TAG
{
    EMC_CS_MAP_DEFAULT = 5,
    EMC_CS_MAP_32M = 0,		
    EMC_CS_MAP_64M = 1,
    EMC_CS_MAP_128M = 2,
    EMC_CS_MAP_256M = 3,
    EMC_CS_MAP_512M = 4,
    EMC_CS_MAP_1G = 5,
    EMC_CS_MAP_2G = 6,
    EMC_CS_MAP_4G = 7
}
EMC_CS_MAP_E;

typedef enum EMC_CS_NUM_TAG
{
    EMC_CS0 = 0,
    EMC_CS1
}
EMC_CS_NUM_E;

typedef enum EMC_BURST_MODE_TAG
{
    BURST_WRAP = 0,
    BURST_INCR
}
EMC_BURST_MODE_E;

typedef enum EMC_BURST_INVERT_TAG
{
    HBURST_TO_SINGLE = 0,
    HBURST_TO_BURST
}
EMC_BURST_INVERT_E;

typedef enum EMC_CHL_NUM_TAG
{
    EMC_AXI_ARM = 0,
    EMC_AXI_GPU = 1,
    EMC_AHB_MTX1 = 2,
    EMC_AHB_MTX2 = 3,
    EMC_DSP_CHL = 4,
    EMC_DSP_MTX = 5,
    EMC_DCAM_CHL = 6,
    EMC_LCD_CHL = 7,
    EMC_VSP_CHL = 8    
}
EMC_CHL_NUM_E;


typedef enum EMC_CLK_SYNC_TAG
{
    EMC_CLK_ASYNC = 0,
    EMC_CLK_SYNC
}
EMC_CLK_SYNC_E;

typedef enum EMC_REF_CS_TAG
{
    EMC_CS_REF_OBO = 0, //CSs auto-refresh one by one
    EMC_CS_REF_SAME	//CSs auto-refresh at same time
}
EMC_CS_REF_E;

typedef enum EMC_PRE_BIT_TAG
{
    EMC_PRE_BIT_A10 = 0,
    EMC_PRE_BIT_A11,
    EMC_PRE_BIT_A12,
    EMC_PRE_BIT_A13    
}
EMC_PRE_BIT_E;

typedef enum EMC_DL_SWTICH_TAG
{
    EMC_DL_OFF = 0,
    EMC_DL_ON
}
EMC_DL_SWTICH_E;

typedef enum EMC_CKE_SEL_TAG
{
    EMC_CKE_SEL_DEFAULT = 0,
    EMC_CKE_CS0 = 0,
    EMC_CKE_CS1 = 1,
    EMC_CKE_ALL_CS = 2
}
EMC_CKE_SEL_E;

typedef enum EMC_DQS_GATE_LOOP_TAG
{
    EMC_DQS_GATE_DEFAULT = 0,
    EMC_DQS_GATE_DL = 0,
    EMC_DQS_GATE_DL_LB = 1,
    EMC_DQS_GATE_LB = 2
}
EMC_DQS_GATE_LOOP_E;

typedef enum EMC_DQS_GATE_MODE_TAG
{
    EMC_DQS_GATE_MODE_DEFAULT = 0,
    EMC_DQS_GATE_MODE0 = 0,
    EMC_DQS_GATE_MODE1 = 1
}
EMC_DQS_GATE_MODE_E;

typedef enum EMC_PHY_TIMING_NUM_TAG
{
    EMC_PHY_TIMING_SDRAM_LATENCY2 = 0,
    EMC_PHY_TIMING_SDRAM_LATENCY3,
    EMC_PHY_TIMING_DDRAM_LATENCY2,
    EMC_PHY_TIMING_DDRAM_LATENCY3,
    EMC_PHY_TIMING_MATRIX_MAX
}EMC_PHY_TIMING_NUM_E;

typedef enum EMC_DLL_NUM_TAG
{
	EMC_DMEM_DL0 = 0,
	EMC_DMEM_DL1,
	EMC_DMEM_DL2,
	EMC_DMEM_DL3,
	EMC_DMEM_DL4,
	EMC_DMEM_DL5,
	EMC_DMEM_DL7,
	EMC_DMEM_DL8,
	EMC_DMEM_DL9,
	EMC_DMEM_DL10,
	EMC_DMEM_DL11,
	EMC_DMEM_DL12,
	EMC_DMEM_DL13,
	EMC_DMEM_DL14,
	EMC_DMEM_DL15,
	EMC_DMEM_DL16,
	EMC_DMEM_DL17,
	EMC_DMEM_DL18,
	EMC_DMEM_DL19,
	EMC_DMEM_MAX
}EMC_DLL_NUM_E;

typedef struct EMC_PHY_L1_TIMING_TAG 
{
	uint32 data_pad_ie_delay;
	uint32 data_pad_oe_delay;		
	uint32 dqs_gate_pst_delay;
	uint32 dqs_gate_pre_delay;
	uint32 dqs_ie_delay;
	uint32 dqs_oe_delay;		
}EMC_PHY_L1_TIMING_T,*EMC_PHY_L1_TIMING_T_PTR;

typedef struct EMC_PHY_L2_TIMING_TAG 
{	
	uint32 clkdmem_out_dl;
	uint32 dqs_ie_dl;
	uint32 dqs_out_dl;
	uint32 clkwr_dl;
	uint32 dqs_gate_pre_dl_0;
	uint32 dqs_gate_pre_dl_1;
	uint32 dqs_gate_pre_dl_2;
	uint32 dqs_gate_pre_dl_3;
	uint32 dqs_gate_pst_dl_0;
	uint32 dqs_gate_pst_dl_1;
	uint32 dqs_gate_pst_dl_2;
	uint32 dqs_gate_pst_dl_3;
	uint32 dqs_in_pos_dl_0;
	uint32 dqs_in_pos_dl_1;	
	uint32 dqs_in_pos_dl_2;	
	uint32 dqs_in_pos_dl_3;	
	uint32 dqs_in_neg_dl_0;	
	uint32 dqs_in_neg_dl_1;		
	uint32 dqs_in_neg_dl_2;		
	uint32 dqs_in_neg_dl_3;			
}EMC_PHY_L2_TIMING_T,*EMC_PHY_L2_TIMING_T_PTR;

/*
#define EMC_BASEADDR 0x20000000
#define EMC_CFG0  0x20000000

#define DMEM_CFG0   (EMC_BASEADDR + 0x40)
*/
#define DCFG0_DQM_MODE_LOW             0           //DQM low in deactive
#define DCFG0_DQM_MODE_W0R0            1           //DQM hihe in deactive,Write: 0 cycle delay; Read: 0 cycle delay;
#define DCFG0_DQM_MODE_W0R1            2           //DQM hihe in deactive,Write: 0 cycle delay; Read: 1 cycle delay;
#define DCFG0_DQM_MODE_W0R2            3           //DQM hihe in deactive,Write: 0 cycle delay; Read: 2 cycle delay;

#define DCFG0_DQM_TERM_EN              (1u << 2)
#define DCFG0_DQM_FORCE_HIGH           (1u << 3)

#define DCFG0_BKPOS_HADDR3             (0u << 4)
#define DCFG0_BKPOS_HADDR4             (1u << 4)
#define DCFG0_BKPOS_HADDR5             (2u << 4)
#define DCFG0_BKPOS_HADDR6             (3u << 4)
#define DCFG0_BKPOS_HADDR8             (4u << 4)
#define DCFG0_BKPOS_HADDR10             (5u << 4)
#define DCFG0_BKPOS_HADDR13             (6u << 4)
#define DCFG0_BKPOS_HADDR16             (7u << 4)
#define DCFG0_BKPOS_HADDR18             (8u << 4)
#define DCFG0_BKPOS_HADDR20             (9u << 4)
#define DCFG0_BKPOS_HADDR22             (10u << 4)
#define DCFG0_BKPOS_HADDR23             (11u << 4)
#define DCFG0_BKPOS_HADDR24             (12u << 4)
#define DCFG0_BKPOS_HADDR25             (13u << 4)
#define DCFG0_BKPOS_HADDR26             (14u << 4)
#define DCFG0_BKPOS_HADDR28             (15u << 4)

#define DCFG0_BKMODE_1                (0u << 8)
#define DCFG0_BKMODE_2                (1u << 8)
#define DCFG0_BKMODE_4                (2u << 8)
#define DCFG0_BKMODE_8                (3u << 8)

#define DCFG0_ROWMODE_11              (0u << 10)
#define DCFG0_ROWMODE_12              (1u << 10)
#define DCFG0_ROWMODE_13              (2u << 10)

#define DCFG0_COLMODE_8               (0u << 12)
#define DCFG0_COLMODE_9               (1u << 12)
#define DCFG0_COLMODE_10               (2u << 12)
#define DCFG0_COLMODE_11               (3u << 12)
#define DCFG0_COLMODE_12               (4u << 12)

#define DCFG0_DWIDTH_16               (0u << 15)
#define DCFG0_DWIDTH_32               (1u << 15)

#define DCFG0_BL_2                    (1u << 16)
#define DCFG0_BL_4                    (2u << 16)
#define DCFG0_BL_8                    (3u << 16)
#define DCFG0_BL_16                   (4u << 16)
#define DCFG0_BL_FULLPAGE             (7u << 16)

#define DCFG0_AUTOREF_ALLCS           (1u << 19)

#define DCFG0_RL_2                    (2u << 20)
#define DCFG0_RL_3                    (3u << 20)
#define DCFG0_RL_4                    (4u << 20)
#define DCFG0_RL_5                    (5u << 20)
#define DCFG0_RL_6                    (6u << 20)
#define DCFG0_RL_7                    (7u << 20)

#define DCFG0_T_RW_0                  (0u << 29)
#define DCFG0_T_RW_1                  (1u << 29)

#define DCFG0_ALTERNATIVE_EN          (1u << 30)
#define DCFG0_ROWHIT_EN               (1u << 31)
#define DCFG0_AUTOREF_EN             BIT_14

//define mode register domain..

#define MODE_REG_BL_1               (0)
#define MODE_REG_BL_2               (1)
#define MODE_REG_BL_4               (2)
#define MODE_REG_BL_8               (3)

#define MODE_REG_BT_SEQ               (0)
#define MODE_REG_BT_INT               (1)

#define MODE_REG_CL_1                 (1)
#define MODE_REG_CL_2                 (2)
#define MODE_REG_CL_3                 (3)

#define MODE_REG_OPMODE               (0)

#define MODE_REG_WB_PRORAM            (0)
#define MODE_REG_WB_SINGLE            (1)

//define extended mode register domain...
#define EX_MODE_REG_PASR_4_BANKS      (0)
#define EX_MODE_REG_PASR_2_BANKS      (1)
#define EX_MODE_REG_PASR_1_BANKS      (2)
#define EX_MODE_REG_PASR_HALF_BANK      (5)
#define EX_MODE_REG_PASR_QUART_BANK      (6)

#define EX_MODE_REG_DS_FULL           (0)
#define EX_MODE_REG_DS_HALF           (1)

#elif defined(CONFIG_TIGER)
typedef enum
{
	STATE_INIT_MEM,
	STATE_CONFIG,
	STATE_CONFIG_REQ,
	STATE_ACCESS,
	STATE_ACCESS_REQ,
	STATE_LOW_POWER,
	STATE_LOW_POWER_ENTRY_REQ,
	STATE_LOW_POWER_EXIT_REQ
}UPCTL_STATE_E;
typedef enum
{
	CMD_INIT,
	CMD_CFG,
	CMD_GO,
	CMD_SLEEP,
	CMD_WAKEUP
}UPCTL_CMD_E;

#define LPDDR1_SDRAM 1
#define LPDDR2_SDRAM 2


#define UMCTL_REG_BASE      0x60200000
#define PUBL_REG_BASE       0x60201000
#define EMC_MEM_BASE_ADDR   0x80000000
#define UMCTL_REG(X)        REG32(UMCTL_REG_BASE+X)
#define PUBL_REG(X)         REG32(PUBL_REG_BASE+X)

#define DRAM_TYPE       1 //0:LPDDR2_S2_OR_LPDDR1_ROW14_COL9 1:0:LPDDR2_S4_OR_LPDDR1_ROW13_COL10
#define DRAM_ADDR_MAP   1 //0:rank+bank+row+col 1:rank+row+bank+col 2:bank+row+rank+col
#define DRAM_DENSITY    5 //0:64MBIT 1:128MBIT 2:256MBIT 3:512MBIT 4:1GBIT 5:2GBIT 6:4GBIT 7:8GBIT
#define DRAM_IO_WIDTH   3 //1:X8 2:X16 3:X32
#define DRAM_BT         0 //0:sequential 1:interleaving
#define DRAM_BL         3 //1:BL2 2:BL4 3:BL8 4:BL16
                          //LPDDR1:2,4,8,16 seems must be 8 now
                          //LPDDR2:4,8,16
#define DRAM_CL         3 //2:CL2 3:CL3 4:CL4
#define DRAM_WC         1 //0:WRAP 1:NO_WRAP

//register
#define UMCTL_CFG_SCFG		0x000
#define UMCTL_CFG_SCTL		0x004
#define UMCTL_CFG_STAT		0x008
#define UMCTL_CFG_MCMD		0x040
#define UMCTL_CFG_POWCTL	0x044
#define UMCTL_CFG_POWSTAT	0x048
#define UMCTL_CFG_MCFG		0x080
#define UMCTL_CFG_TOGCNT1U	0x0c0
#define UMCTL_CFG_TINIT		0x0c4
#define UMCTL_CFG_TRSTH		0x0c8
#define UMCTL_CFG_TOGCNT100N	0x0cc
#define UMCTL_CFG_TREFI		0x0d0
#define UMCTL_CFG_TMRD		0x0d4
#define UMCTL_CFG_TRFC		0x0d8
#define UMCTL_CFG_TRP		0x0dc
#define UMCTL_CFG_TRTW		0x0e0
#define UMCTL_CFG_TAL		0x0e4
#define UMCTL_CFG_TCL		0x0e8
#define UMCTL_CFG_TCWL		0x0ec
#define UMCTL_CFG_TRAS		0x0f0
#define UMCTL_CFG_TRC		0x0f4
#define UMCTL_CFG_TRCD		0x0f8	
#define UMCTL_CFG_TRRD          0x0fc
#define UMCTL_CFG_TRTP          0x100
#define UMCTL_CFG_TWR           0x104
#define UMCTL_CFG_TWTR          0x108
#define UMCTL_CFG_TEXSR         0x10c
#define UMCTL_CFG_TXP           0x110
#define UMCTL_CFG_TXPDLL        0x114
#define UMCTL_CFG_TZQCS         0x118
#define UMCTL_CFG_TZQCSI        0x11c
#define UMCTL_CFG_TDQS          0x120
#define UMCTL_CFG_TCKSRE        0x124
#define UMCTL_CFG_TCKSRX	0x128
#define UMCTL_CFG_TCKE		0x12c
#define UMCTL_CFG_TMOD		0x130
#define UMCTL_CFG_TRSTL		0x134
#define UMCTL_CFG_TZQCL		0x138
#define UMCTL_CFG_TCKESR	0x13c
#define UMCTL_CFG_TDPD		0x140
#define UMCTL_CFG_DFITPHYWRDATA	0x250
#define UMCTL_CFG_DFITPHYWRLAT	0x254
#define UMCTL_CFG_DFITRDDATAEN	0x260
#define UMCTL_CFG_DFITPHYRDLAT	0x264
#define UMCTL_CFG_DFISTSTAT0	0x2c0
#define UMCTL_CFG_DFISTCFG0	0x2c4
#define UMCTL_CFG_DFISTCFG1	0x2c8
#define UMCTL_CFG_DFISTCFG2	0x2d8
#define UMCTL_CFG_DFILPCFG0	0x2f0
#define UMCTL_CFG_PCFG0		0x400
#define UMCTL_CFG_PCFG1		0x404
#define UMCTL_CFG_PCFG2		0x408
#define UMCTL_CFG_PCFG3		0x40c
#define UMCTL_CFG_PCFG4		0x410
#define UMCTL_CFG_PCFG5		0x414
#define UMCTL_CFG_PCFG6		0x418
#define UMCTL_CFG_PCFG7		0x41c
#define UMCTL_CFG_DCFG		0x484

#define PUBL_CFG_RIDR		(0x00*4) 
#define PUBL_CFG_PIR		(0x01*4)	
#define PUBL_CFG_PGCR		(0x02*4)	
#define PUBL_CFG_PGSR		(0x03*4)	
#define PUBL_CFG_DLLGCR		(0x04*4)	
#define PUBL_CFG_ACDLLCR	(0x05*4)		
#define PUBL_CFG_PTR0		(0x06*4)	
#define PUBL_CFG_PTR1		(0x07*4)	
#define PUBL_CFG_PTR2		(0x08*4)	
#define PUBL_CFG_ACIOCR		(0x09*4)	
#define PUBL_CFG_DXCCR		(0x0A*4)	
#define PUBL_CFG_DSGCR		(0x0B*4)	
#define PUBL_CFG_DCR		(0x0C*4)	
#define PUBL_CFG_DTPR0		(0x0D*4)	
#define PUBL_CFG_DTPR1		(0x0E*4)	
#define PUBL_CFG_DTPR2		(0x0F*4)	
#define PUBL_CFG_MR0		(0x10*4)	
#define PUBL_CFG_MR1		(0x11*4)	
#define PUBL_CFG_MR2		(0x12*4)	
#define PUBL_CFG_MR3		(0x13*4)	
#define PUBL_CFG_ODTCR		(0x14*4)	
#define PUBL_CFG_DTAR		(0x15*4)	
#define PUBL_CFG_DTDR0		(0x16*4)	
#define PUBL_CFG_DTDR1		(0x17*4)	
#define PUBL_CFG_DX0GSR0	(0x71*4)		
#define PUBL_CFG_DX0DQSTR	(0x75*4)		
#define PUBL_CFG_DX1GSR0	(0x81*4)		
#define PUBL_CFG_DX1DQSTR	(0x85*4)		
#define PUBL_CFG_DX2GSR0	(0x91*4)		
#define PUBL_CFG_DX2DQSTR	(0x95*4)		
#define PUBL_CFG_DX3GSR0	(0xa1*4)	
#define PUBL_CFG_DX3DQSTR	(0xa5*4) 
#endif

#endif//end of file
