#ifndef _SDRAM_H_
#define _SDRAM_H_

#ifdef CONFIG_SC8810
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

#endif
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

#endif//end of file
