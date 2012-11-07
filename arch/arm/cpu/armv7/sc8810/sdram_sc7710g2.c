/******************************************************************************
 ** File Name:        EMC_test.c
 ** Author:           Johnny Wang
 ** DATE:             27/07/2010
 ** Copyright:        2007 Spreatrum, Incoporated. All Rights Reserved.
 ** Description:
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History
 **-------------------------------------------------------------------------
 ** DATE          NAME            DESCRIPTION
 ** 23/04/2009                    Create.
 ******************************************************************************/
#include <common.h>
#include <asm/arch/sci_types.h>
#include <asm/arch/arm_reg.h>
#include <asm/arch/sc_reg.h>

//#include "mem_bist.h"

//#include "dma.h"
//#include "dma_drv.h"

#include <asm/arch/sdram_sc7710g2.h>
#include <asm/arch/emc_config.h>

#if defined(EMC_TEST)
//#include "EMC_test.h"
#endif

#ifdef   __cplusplus
extern   "C"
{
#endif

/*----------------------------------------------------------------------------*
**                          Sub Function                                      *
**----------------------------------------------------------------------------*/
LOCAL void EMC_PHY_Mode_Set(SDRAM_CFG_INFO_T_PTR mem_info);
LOCAL void EMC_Base_Mode_Set(void);

uint32 DRAM_CAP;
uint32  SDRAM_BASE    =    	0x00000000;//(128*1024*1024/2)//0x30000000


#define ROW_MODE_MASK           0x3
#define COL_MODE_MASK           0x7
#define DATA_WIDTH_MASK         0x1
#define AUTO_PRECHARGE_MASK     0x3
#define CS_POSITION_MASK        0X3

#define MEM_REF_DATA0       0x12345678
#define MEM_REF_DATA1       0x55AA9889
#define ZERO_ADDR           0x00000000UL

#define BYTE_OFFSET         3UL   // 1BYTE = 8BIT = 2^3
#define WIDTH16_OFFSET      4UL   // 16BIT = 2^4
#define WIDTH32_OFFSET      5UL   // 32BIT = 2^5
#define BANK_OFFSET         2UL   // 4BANK = 2^2



/**---------------------------------------------------------------------------*
 **                     Static Function Prototypes                            *
 **---------------------------------------------------------------------------*/

__inline uint32 delay(uint32 k)
{
    uint32 i, j;

    for (i=0; i<k; i++)
    {
        for (j=0; j<1000; j++);
    }

    return k;
}

void EMC_AddrMode_Set(SDRAM_CFG_INFO_T_PTR mem_info)
{
    uint32 reg_val = 0;

    reg_val = REG32(EXT_MEM_DCFG0);
    reg_val &= ~((DATA_WIDTH_MASK<<8) | (COL_MODE_MASK<<4) | ROW_MODE_MASK);
    reg_val |= (((mem_info->data_width & DATA_WIDTH_MASK)<< 8)
                | ((mem_info->col_mode & COL_MODE_MASK) << 4)
                | (mem_info->row_mode & ROW_MODE_MASK));

    REG32(EXT_MEM_DCFG0) = reg_val;
}

void EMC_DataWidth_Set(SDRAM_CFG_INFO_T_PTR mem_info)
{
    uint32 reg_val = 0;

    reg_val = REG32(EXT_MEM_DCFG0);
    reg_val &= ~(DATA_WIDTH_MASK<<8);

    if (DATA_WIDTH_32 == mem_info->data_width)
    {
        reg_val |= ((mem_info->data_width & DATA_WIDTH_MASK)<< 8);
    }
    
    REG32(EXT_MEM_DCFG0) = reg_val;
}

void EMC_ColumnMode_Set(SDRAM_CFG_INFO_T_PTR mem_info)
{
    uint32 reg_val = 0;

    reg_val = REG32(EXT_MEM_DCFG0);
    reg_val &= ~(COL_MODE_MASK<<4);
    reg_val |= ((mem_info->col_mode & COL_MODE_MASK) << 4);

    REG32(EXT_MEM_DCFG0) = reg_val;
}

void EMC_RowMode_Set(SDRAM_CFG_INFO_T_PTR mem_info)
{
    uint32 reg_val = 0;

    reg_val = REG32(EXT_MEM_DCFG0);
    reg_val &= ~(ROW_MODE_MASK<<4);
    reg_val |= ((mem_info->row_mode & ROW_MODE_MASK) << 4);

    REG32(EXT_MEM_DCFG0) = reg_val;
}

LOCAL BOOLEAN SDRAM_Type_Set(SDRAM_CFG_INFO_T_PTR mem_info)
{
    uint32 err_line = 0;
    DMEM_TYPE_E sdram_mode = mem_info->sdram_type;

    

    REG32(EXT_MEM_DCFG0) &= ~ BIT_14;

    if (SDR_SDRAM == sdram_mode)
    {
        REG32(EXT_MEM_CFG1) = 0X2;
        REG32(EXT_MEM_CFG0_CS0) = 0X3;
        REG32(EXT_MEM_CFG0_CS1) = 0X3;

        //REG32(EXT_MEM_DCFG5) = 0X620209;
        EMC_PHY_Latency_Set(mem_info);
        
        REG32(EXT_MEM_DCFG6) = 0X400020;
        REG32(EXT_MEM_DCFG7) = 0XF0000E;
        REG32(EXT_MEM_DCFG8) = 0X400001;

        delay(100);
        REG32(EXT_MEM_DCFG4) = 0X40010000;

        delay(100);
        REG32(EXT_MEM_DCFG4) = 0X40020000;

        delay(100);
        REG32(EXT_MEM_DCFG4) = 0X40020000;

        delay(100);
        REG32(EXT_MEM_DCFG4) = 0X40040031;

        delay(100);

        REG32(EXT_MEM_DCFG0) &= ~(AUTO_PRECHARGE_MASK<<2);

    }
    else if (DDR_SDRAM == sdram_mode)
    {
        REG32(EXT_MEM_CFG1) = 0x00000049;

        REG32(EXT_MEM_DCFG0) &= ~(AUTO_PRECHARGE_MASK<<2);

//        REG32(EXT_MEM_DCFG0) |= BIT_8;
        REG32(EXT_MEM_DCFG1) = 0x03382434;
        REG32(EXT_MEM_DCFG2) = 0x1a261000;
        //REG32(EXT_MEM_DCFG5) = 0x0062272A;
        EMC_PHY_Latency_Set(mem_info);

        if (DATA_WIDTH_16 == mem_info->data_width)
        {
            if (CAS_LATENCY_2 == mem_info->cas_latency)
            {
                REG32(EXT_MEM_DCFG6) = 0x00080004;
            }
            else
            {
                REG32(EXT_MEM_DCFG6) = 0x00200010;
            }
        }
        else
        {
            if (CAS_LATENCY_2 == mem_info->cas_latency)
            {
                REG32(EXT_MEM_DCFG6) = 0x00200010;
            }
            else
            {
                REG32(EXT_MEM_DCFG6) = 0x00400020;
            }
        }
        
        REG32(EXT_MEM_DCFG7) = 0x00F0000E;
        REG32(EXT_MEM_DCFG8) = 0x00F0000E; 

#ifdef DLL_OPEN
        REG32(EXT_MEM_CFG0_DLL) = 0x21080; // open dll
        while (0==(REG32(0xA0000170))&BIT_14); //wait dll locked
        REG32(EXT_MEM_DL0) = 0x8020; //0x8040;
        REG32(EXT_MEM_DL1) = 0x8020; //0x8040;
        REG32(EXT_MEM_DL2) = 0x8020; //0x8040;
        REG32(EXT_MEM_DL3) = 0x8020; //0x8040;

        REG32(EXT_MEM_DL4) = 0x8020;
        REG32(EXT_MEM_DL5) = 0x8020;
        REG32(EXT_MEM_DL6) = 0x8020;
        REG32(EXT_MEM_DL7) = 0x8020;
        REG32(EXT_MEM_DL8) = 0x8020;
        REG32(EXT_MEM_DL9) = 0x8020;
        REG32(EXT_MEM_DL10) = 0x8020;
        REG32(EXT_MEM_DL11) = 0x8020;

        REG32(EXT_MEM_DL12) = 0x8040;
        REG32(EXT_MEM_DL13) = 0x8040;
        REG32(EXT_MEM_DL14) = 0x8040;
        REG32(EXT_MEM_DL15) = 0x8040;
        REG32(EXT_MEM_DL16) = 0x8040;
        REG32(EXT_MEM_DL17) = 0x8040;
        REG32(EXT_MEM_DL18) = 0x8040;
        REG32(EXT_MEM_DL19) = 0x8040;
        REG32(EXT_MEM_CFG0_DLL) |= BIT_10; //open dll cmp

#else
        REG32(EXT_MEM_DL0) = 0x0008;
        REG32(EXT_MEM_DL1) = 0x0008;
        REG32(EXT_MEM_DL2) = 0x0008;
        REG32(EXT_MEM_DL3) = 0x0008;

        REG32(EXT_MEM_DL4) = 0x0004;
        REG32(EXT_MEM_DL5) = 0x0004;
        REG32(EXT_MEM_DL6) = 0x0004;
        REG32(EXT_MEM_DL7) = 0x0004;
        REG32(EXT_MEM_DL8) = 0x0004;
        REG32(EXT_MEM_DL9) = 0x0004;
        REG32(EXT_MEM_DL10) = 0x0004;
        REG32(EXT_MEM_DL11) = 0x0004;

        REG32(EXT_MEM_DL12) = 0x0008;
        REG32(EXT_MEM_DL13) = 0x0008;
        REG32(EXT_MEM_DL14) = 0x0008;
        REG32(EXT_MEM_DL15) = 0x0008;
        REG32(EXT_MEM_DL16) = 0x0008;
        REG32(EXT_MEM_DL17) = 0x0008;
        REG32(EXT_MEM_DL18) = 0x0008;
        REG32(EXT_MEM_DL19) = 0x0008;

#endif

        REG32(EXT_MEM_DCFG4) = 0x40010000;
        delay(100);
        REG32(EXT_MEM_DCFG4) = 0x40020000;
        delay(100);

        REG32(EXT_MEM_DCFG4) = 0x40020000;
        delay(100);

        REG32(EXT_MEM_DCFG4) = 0x40040031;
        delay(100);

        REG32(EXT_MEM_DCFG4) = 0x40048000;
        delay(100);

    }
    else
    {
        err_line = __LINE__;
        goto err_print;
    }

    //enable auto refresh.
    REG32(EXT_MEM_DCFG3) |= BIT_15;  //clear refresh count.
    REG32(EXT_MEM_DCFG0) |= BIT_14;  //Enable auto refresh.

    return TRUE;

err_print:

    //SCI_TraceLow("\r\nERR! %s %d sdram_mode:0x%x", __func__, err_line, sdram_mode);

    return FALSE;
}


LOCAL void EMC_SoftReset(void)
{
    REG32(AHB_SOFT_RST) |= BIT_11;
    delay(10);
    REG32(AHB_SOFT_RST) &= (~BIT_11);
    delay(10);
}



PUBLIC uint32 SDRAM_GetCap(SDRAM_CFG_INFO_T_PTR mem_info)  // capability in bytes
{
    uint32 SDRAM_Cap;

#ifdef SDRAM_AUTODETECT_SUPPORT
    uint32 width_offset = (DATA_WIDTH_16 == mem_info->data_width)?(WIDTH16_OFFSET):(WIDTH32_OFFSET);

    SDRAM_Cap = 1 << (BANK_OFFSET + (mem_info->col_mode + ROW_LINE_MIN)
                      + (mem_info->row_mode + COLUMN_LINE_MIN) + width_offset - BYTE_OFFSET);

//    SDRAM_Cap = 0x10000000;
#else
    SDRAM_CHIP_FEATURE_T_PTR mem_feature = SDRAM_GetFeature();

    SDRAM_Cap = mem_feature->cap;
#endif

#ifdef _FAST_TEST_
    SDRAM_Cap = 0x10000000;
#endif

    return SDRAM_Cap;
}

PUBLIC EMC_PHY_L1_TIMING_T_PTR EMC_GetPHYL1_Timing(DMEM_TYPE_E mem_type, uint32 cas_latency)
{
    if (SDR_SDRAM == mem_type)
    {
        if (CAS_LATENCY_2 == cas_latency)
        {
            return (EMC_PHY_L1_TIMING_T_PTR)(&(EMC_PHY_TIMING_L1_INFO[EMC_PHYL1_TIMING_SDRAM_LATENCY2]));
        }
        else
        {
            return (EMC_PHY_L1_TIMING_T_PTR)(&(EMC_PHY_TIMING_L1_INFO[EMC_PHYL1_TIMING_SDRAM_LATENCY3]));
        }
    }
    else
    {
        if (CAS_LATENCY_2 == cas_latency)
        {
            return (EMC_PHY_L1_TIMING_T_PTR)(&(EMC_PHY_TIMING_L1_INFO[EMC_PHYL1_TIMING_DDRAM_LATENCY2]));
        }
        else
        {
            return (EMC_PHY_L1_TIMING_T_PTR)(&(EMC_PHY_TIMING_L1_INFO[EMC_PHYL1_TIMING_DDRAM_LATENCY3]));
        }
    }
}

PUBLIC EMC_PHY_L2_TIMING_T_PTR EMC_GetPHYL2_Timing(void)
{
    if (EMC_DLL_ON_OFF == DLL_OFF)
    {
        return (EMC_PHY_L2_TIMING_T_PTR)(&(EMC_PHY_TIMING_L2_INFO[EMC_PHYL2_TIMING_DLL_OFF]));
    }
    else
    {
        return (EMC_PHY_L2_TIMING_T_PTR)(&(EMC_PHY_TIMING_L2_INFO[EMC_PHYL2_TIMING_DLL_ON]));
    }
}


/*****************************************************************************/
//  Description:	EMC basic mode set function
//				set the base mode like:
//				EMC device endian
//				EMC auto gate en for power saving
//				EMC auto sleep en
//				EMC cmd queue mode
//  Global resource dependence:  NONE
//  Related register: EMC_CFG0
//  Author:		Johnny.Wang
//  Note:			The default cs map space is 4g, if dram capability is larger than 4g,
//				emc_cs_map parameter must adjust.
/*****************************************************************************/
LOCAL void EMC_Base_Mode_Set(void)
{
    uint32 i = 0;

    i = REG32(EXT_MEM_CFG0);
    i &= ~0x1fff;
    i |=(EMC_DVC_ENDIAN_DEFAULT <<12) 	|
        (EMC_AUTO_GATE_EN		<<11)	|
        (EMC_AUTO_SLEEP_EN		<<10)	|
        (EMC_2DB_1CB			<<6)	|
        (EMC_CS_MODE_DEFAULT	<<3)	|
        (EMC_TWO_CS_MAP_4GBIT   <<0)	;

    REG32(EXT_MEM_CFG0) = i;
}


LOCAL void EMC_CS_Mapping_Set(EMC_CS_MAP_E cs_position)
{
    uint32 i = 0;

    i = REG32(EXT_MEM_CFG0);
    i &= ~CS_POSITION_MASK;
    i |= cs_position;

    REG32(EXT_MEM_CFG0) = i;
}

/*****************************************************************************/
//  Description:	EMC each cs work mode set function
//				set each cs work mode parameter like:
//				memory write burst length
//				memory read burst length
//				memory write burst mode:wrap/increase
//				memory read burst mode:wrap/increase
//				AHB write busrt divided to single/busrt access
//				AHB read busrt divided to single/busrt access
//  Global resource dependence:  memory burst length type
//  Related register: EMC_CFG0_CSx
//  Author:		Johnny.Wang
//  Note:			There are two cs pin in sc8810 emc, we usuall use cs0 to control external memory
//
/*****************************************************************************/
PUBLIC void EMC_CSx_Burst_Set(EMC_CS_NUM_E emc_cs_num, SDRAM_CFG_INFO_T_PTR mem_info)
{
    uint32 i = 0;
    uint32 emc_cs_cfg = EXT_MEM_CFG0_CS0 + emc_cs_num*4;

    uint32 burst_len = 0;

    if (DATA_WIDTH_16 == mem_info ->data_width)
    {
        burst_len = mem_info->burst_length -1;
    }
    else
    {
        burst_len = mem_info->burst_length;
    }

    i = REG32(emc_cs_cfg);
    i &= ~((0x7<<8)|(0x7<<4)|(1<<1)|1);
    i |=((burst_len			<<8) | //write burst length
         (burst_len			<<4) | //read burst length
         (HBURST_TO_BURST	<<1) | //write hburst invert to mem burst
         (HBURST_TO_BURST	<<0));  //rrite hburst invert to mem burst

    REG32(emc_cs_cfg) = i;
}

/*****************************************************************************/
//  Description:	EMC AXI channel set function
//				set each axi channel parameter like:
//				axi channel en
//				axi channel auto sleep en
//				channel endian switch
//				channel priority
//  Global resource dependence:  NONE
//  Related register: EMC_CFG0_ACHx
//				  EMC_CFG1_ACHx
//  Author:		Johnny.Wang
//  Note:			There are two axi channel in sc8810 emc, one for A5,the other for GPU
//
/*****************************************************************************/
LOCAL void EMC_AXI_CHL_Set(EMC_CHL_NUM_E emc_axi_num)
{
    uint32 i = 0;
    uint32 emc_axi_cfg0 = EXT_MEM_CFG0_ACH0+ emc_axi_num*8;
    uint32 emc_axi_cfg1 = EXT_MEM_CFG1_ACH1+ emc_axi_num*8;

	
    i = REG32(emc_axi_cfg0);
    i &= ~0xf0;
    i |= (TRUE<<7) | //channel auto sleep en
         (TRUE<<6) | //channel en
         (EMC_ENDIAN_SWITCH_NONE<<4);
    REG32(emc_axi_cfg0) = i;

    i = REG32(emc_axi_cfg1);
    i &= ~BIT_4; //clear bit4
    i |= (EMC_CLK_ASYNC<<4); //emc clk async with axi clk
    i |= (1<<6); //axi channel response mode  0:at once  1:delay several clk
    REG32(emc_axi_cfg1) = i;
}
/*****************************************************************************/
//  Description:	EMC AHB channel set function
//  Global resource dependence:  NONE
//  Author:		Johnny.Wang
//  Note:			There are 7 ahb channel in sc8810 emc, but no one is relate with ARM,
//				so don't used temporarily
//
/*****************************************************************************/
LOCAL void EMC_AHB_CHL_Set(EMC_CHL_NUM_E emc_ahb_num,uint32 addr_offset)
{
    uint32 emc_ahb_cfg0 = EXT_MEM_CFG0_ACH0 + emc_ahb_num*8;
    uint32 emc_ahb_cfg1 = EXT_MEM_CFG1_ACH0 + emc_ahb_num*8;

    REG32(emc_ahb_cfg1) &= ~0x03ff0000;	//clear bit16~25
    REG32(emc_ahb_cfg1) |= addr_offset<<16;

}

/*****************************************************************************/
//  Description:	EMC Memroy all timing parameter set function
//				set all timing parameter of EMC when operate external memory like:
//				t_rtw,
//				t_ras,
//				t_xsr,
//				t_rfc,
//				t_wr,
//				t_rcd,
//				t_rp,
//				t_rrd,
//				t_mrd,
//				t_wtr,
//				t_ref,
//  Global resource dependence:  NONE
//  Author:		Johnny.Wang
//  Related register: EMC_DCFG1
//				  EMC_DCFG2
//  Note:			None
//
/*****************************************************************************/
LOCAL void EMC_MEM_Timing_Set(uint32 emc_freq,
                        SDRAM_CFG_INFO_T_PTR     mem_info,
                        SDRAM_TIMING_PARA_T_PTR  mem_timing)
{
    uint32 cycle_ns = (uint32)(2000000000/emc_freq);//2000/(clk); //device clock is half of emc clock.
    uint32 cycle_t_ref = 1000000000/EMC_T_REF_CLK;

    uint32 row_mode    = mem_info->row_mode + ROW_LINE_MIN;
    uint32 t_rtw       = mem_info->cas_latency;

    //round all timing parameter
    uint32  t_ras	= mem_timing->ras_min/cycle_ns;
    uint32  t_xsr 	= mem_timing->xsr_min/cycle_ns;
    uint32  t_rfc 	= mem_timing->rfc_min/cycle_ns;
    uint32  t_wr  	= mem_timing->wr_min/cycle_ns+2; //note: twr should add 2 for ddr
    uint32  t_rcd 	= mem_timing->rcd_min/cycle_ns;
    uint32  t_rp  	= mem_timing->row_pre_min/cycle_ns;
    uint32  t_rrd 	= mem_timing->rrd_min/cycle_ns;
    uint32  t_mrd	= mem_timing->mrd_min;
    uint32  t_wtr 	= mem_timing->wtr_min+1;
    uint32  t_ref 	= mem_timing->row_ref_max*1000000/cycle_t_ref/(1<<row_mode) -2;

    //prevent the maximun overflow of all timing parameter
    t_ras	= (t_ras >= 0xf) ? 0x7  : t_ras;
    t_xsr 	= (t_xsr >= 0xff) ? 0x26 : t_xsr;
    t_rfc 	= (t_rfc >= 0x3f) ? 0x1a : t_rfc;
    t_wr  	= (t_wr  >= 0xf) ? 0x4  : t_wr;
    t_rcd 	= (t_rcd >= 0xf) ? 0x3  : t_rcd;
    t_rp  	= (t_rp  >= 0xf) ? 0x3  : t_rp;
    t_rrd 	= (t_rrd >= 0xf) ? 0x2  : t_rrd;
    t_mrd	= (t_mrd >= 0xf) ? 0x0  : t_mrd;
    t_wtr 	= (t_wtr >= 0xf) ? 0x3  : t_wtr;
    t_ref 	= (t_ref >=0xfff) ? 0x100: t_ref ;

    //prevent the minmun value of all timing
    t_ras	= (t_ras <= 0) ? 0x7  : t_ras;
    t_xsr 	= (t_xsr <= 0) ? 0x26 : t_xsr;
    t_rfc 	= (t_rfc <= 0) ? 0x1a : t_rfc;
    t_wr  	= (t_wr  <= 0) ? 0x4  : t_wr;
    t_rcd 	= (t_rcd <= 0) ? 0x3  : t_rcd;
    t_rp  	= (t_rp  <= 0) ? 0x3  : t_rp;
    t_rrd 	= (t_rrd <= 0) ? 0x2  : t_rrd;
    t_mrd	= (t_mrd <= 0) ? 0x2  : t_mrd;
    t_wtr 	= (t_wtr <= 0) ? 0x3  : t_wtr;
    t_ref 	= (t_ref <=00) ? 0x100: t_ref ;



    REG32(EXT_MEM_DCFG1) =
        ((1<<28)	  |//read to read turn around time between different cs,default:0     2 cs or above:1
         (t_wtr << 24) |
         (t_rtw << 20) |
         (t_ras << 16) |
         (t_rrd << 12) |
         (t_wr  << 8)  |
         (t_rcd << 4)  |
         (t_rp  << 0));

    REG32(EXT_MEM_DCFG2) =
        ((t_rfc << 24) |
         (t_xsr << 16) |
         (t_ref << 4)  |
         (t_mrd << 0));
}


/*****************************************************************************/
//  Description:	EMC software command send function
//				this function will send software initialization command to external memory
//  Global resource dependence:  memory type
//  Author:		Johnny.Wang
//  Related register:
//  Note:			None
//
/*****************************************************************************/
void EMC_SCMD_Issue(SDRAM_CFG_INFO_T_PTR mem_info)
{
    uint32 i = 0;

    //shut down auto-refresh
    REG32(EXT_MEM_DCFG0) &= ~(DCFG0_AUTOREF_EN);

    //precharge all bank
    REG32(EXT_MEM_DCFG4) = 0x40010000;
    while (REG32(EXT_MEM_DCFG4) & BIT_16);
    for (i=0; i<=50; i++);

    //software auto refresh
    REG32(EXT_MEM_DCFG4) = 0x40020000;
    while (REG32(EXT_MEM_DCFG4) & BIT_17);
    for (i=0; i<=50; i++);

    //software auto refresh
    REG32(EXT_MEM_DCFG4) = 0x40020000;
    while (REG32(EXT_MEM_DCFG4) & BIT_17);
    for (i=0; i<=50; i++);

    //load nornal mode register
    REG32(EXT_MEM_DCFG4) = 0x40040000 | (mem_info->cas_latency<<4) | (mem_info->burst_length);
    while (REG32(EXT_MEM_DCFG4) & BIT_18);
    for (i=0; i<=50; i++);

    if (SDRAM_EXT_MODE_INVALID != mem_info->ext_mode_val)
    {
        //load external mode register
        REG32(EXT_MEM_DCFG4) = 0x40040000 | mem_info->ext_mode_val;
        while (REG32(EXT_MEM_DCFG4) & BIT_18);
        for (i=0; i<=50; i++);
    }

    //open auto-refresh
    REG32(EXT_MEM_DCFG0) |= (DCFG0_AUTOREF_EN);

}

#ifdef SDRAM_AUTODETECT_SUPPORT


LOCAL BOOLEAN __is_rw_ok(uint32 addr,uint32 val)
{
    volatile uint32 i;
    BOOLEAN ret = SCI_TRUE;

    REG32(addr) = 0;
    REG32(addr) 	= val;

    REG32(addr + 4) = 0;
    REG32(addr + 4) = (~val);

    delay(100);

    if ((REG32(addr) == val) && (REG32(addr + 4) == (~val)))
    {
        ret = SCI_TRUE;
    }
    else
    {
        ret = SCI_FALSE;
    }

    return ret;
}

#if 0
BOOLEAN dram_detect_cap(uint32 dram_total_cap)
{
    BOOLEAN ret = TRUE;
    uint32 i = 0;
    uint32 detect_block_size = CAP_2G_BIT;
    uint32 start_detect_addr_len = 0;
    uint32 start_detect_addr[] = {
        0x00000000,
        0x10000000,
        0x30000000,
        INVALIDE_VAL
    };

    switch (dram_total_cap)
    {
        case CAP_6G_BIT:
            break;
        case CAP_4G_BIT:
            start_detect_addr[2] = INVALIDE_VAL;
            break;
        case CAP_2G_BIT:
        default:
            start_detect_addr[1] = INVALIDE_VAL;
            start_detect_addr[2] = INVALIDE_VAL;
            break;
    }

    start_detect_addr_len = sizeof(start_detect_addr) / sizeof(start_detect_addr[0]);

    for(i = 0; i < start_detect_addr_len; i++)
    {
        if (start_detect_addr[i] != INVALIDE_VAL)
        {
            *(volatile uint32 *)start_detect_addr[i] = start_detect_addr[i];
        }
    }

    for(i = 0; i < start_detect_addr_len; i++)
    {
        if (start_detect_addr[i] != INVALIDE_VAL)
        {
            if (*(volatile uint32 *)start_detect_addr[i] != start_detect_addr[i])
            {
                ret = FALSE;
                break;
            }
        }  
    }
    
    return ret;
}


CONST SDRAM_MODE_INFO_T sdram_info[] =
{
    {CAP_6G_BIT, EMC_TWO_CS_MAP_4GBIT, ROW_MODE_14, ROW_MODE_15_6G,   DATA_WIDTH_32},
    {CAP_6G_BIT, EMC_TWO_CS_MAP_4GBIT, ROW_MODE_14, COL_MODE_11_6G,   DATA_WIDTH_32},
    {CAP_4G_BIT, EMC_TWO_CS_MAP_4GBIT, ROW_MODE_14, COL_MODE_10,      DATA_WIDTH_32},
    {CAP_2G_BIT, EMC_ONE_CS_MAP_2GBIT, ROW_MODE_14, COL_MODE_10,      DATA_WIDTH_32},
    {CAP_1G_BIT, EMC_ONE_CS_MAP_1GBIT, ROW_MODE_14, COL_MODE_9,       DATA_WIDTH_32},
    {CAP_1G_BIT, EMC_ONE_CS_MAP_1GBIT, ROW_MODE_13, COL_MODE_10,      DATA_WIDTH_32},

    {CAP_2G_BIT, EMC_ONE_CS_MAP_2GBIT, ROW_MODE_14, COL_MODE_11,      DATA_WIDTH_16},    
    {CAP_1G_BIT, EMC_ONE_CS_MAP_1GBIT, ROW_MODE_14, COL_MODE_10,      DATA_WIDTH_16},
};
#endif

LOCAL BOOLEAN __col_row_detect(SDRAM_CFG_INFO_T_PTR pCfg, uint32 mode, uint32 *num_get)
{
    uint32 num,max,min;
    uint32 offset,addr;
    uint32 width_offset = (DATA_WIDTH_16 == pCfg->data_width)?(WIDTH16_OFFSET):(WIDTH32_OFFSET);
    uint32 err_line;
    uint32 read_val = 0;

    if (mode == STATE_COLUM)
    {
        max = SDRAM_MAX_COLUMN;
        min = SDRAM_MIN_COLUMN;

        if (pCfg->col_mode != max)
        {
            pCfg->col_mode = max;
            EMC_AddrMode_Set(pCfg);
        }
    }
    else
    {
        max = SDRAM_MAX_ROW;
        min = SDRAM_MIN_ROW;

        if (pCfg->row_mode != max)
        {
            pCfg->row_mode = max;
            EMC_AddrMode_Set(pCfg);
        }
    }

    for (num = max; num >= min; num--)
    {
        REG32(ZERO_ADDR) = 0;
        REG32(ZERO_ADDR) = MEM_REF_DATA0;

        if (mode == STATE_COLUM)
        {
            offset = num + COLUMN_LINE_MIN + width_offset - BYTE_OFFSET;
        }
        else
        {
            offset = num + ROW_LINE_MIN + BANK_OFFSET + (uint32)pCfg->col_mode + width_offset - BYTE_OFFSET;
        }

        addr = (1 << (offset - 1)) + ZERO_ADDR;

        if (__is_rw_ok(addr, MEM_REF_DATA1))
        {
            read_val = REG32(ZERO_ADDR);

            if (MEM_REF_DATA0 == read_val)
            {
                break;
            }

            if (MEM_REF_DATA1 != read_val)
            {
                err_line = __LINE__;
                goto err_print;
            }
        }
    }

    if (num < min)
    {
        err_line = __LINE__;
        goto err_print;
    }

    *num_get = num;
    return TRUE;

err_print:

    //SCI_TraceLow("\r\nERR! %s %d read_val:0x%x mode:0x%x", __func__, err_line, read_val, mode);

    return FALSE;
}

//sdram_info

LOCAL BOOLEAN DRAM_Para_SelfAdapt(SDRAM_CFG_INFO_T_PTR pCfg)
{
    uint32 state = STATE_SDRAM_TYPE;
    BOOLEAN update_mode = TRUE;

    pCfg->row_mode     = ROW_MODE_14;
    pCfg->col_mode     = COL_MODE_12;
    pCfg->data_width   = DATA_WIDTH_32;
    pCfg->burst_length = BURST_LEN_2_WORD;
    pCfg->cas_latency  = CAS_LATENCY_3;
    pCfg->ext_mode_val = SDRAM_EXT_MODE_REG;
    pCfg->sdram_type   = DDR_SDRAM;

    EMC_PHY_Mode_Set(pCfg);
    EMC_Base_Mode_Set();
    EMC_AddrMode_Set(pCfg);
    SDRAM_Type_Set(pCfg);
    
    while (STATE_END != state)
    {        
        switch (state)
        {
        case STATE_SDRAM_TYPE:
            if (__is_rw_ok(ZERO_ADDR, MEM_REF_DATA0))
            {
                state = STATE_COLUM;
            }
            else
            {
                if((pCfg->data_width == DATA_WIDTH_32)
                        && (pCfg->sdram_type == DDR_SDRAM)
                    )
                {
                    pCfg->data_width = DATA_WIDTH_16;
                    EMC_DataWidth_Set(pCfg);
                    EMC_CSx_Burst_Set(EMC_CS0, pCfg);
                    EMC_CSx_Burst_Set(EMC_CS1, pCfg);
                }
                else if((pCfg->data_width == DATA_WIDTH_16)
                        && (pCfg->sdram_type == DDR_SDRAM)
                    )
                {
                    pCfg->data_width = DATA_WIDTH_32;
                    pCfg->sdram_type == SDR_SDRAM;
                    EMC_DataWidth_Set(pCfg);
                    EMC_PHY_Mode_Set(pCfg);
                    SDRAM_Type_Set(pCfg);
                }
                else if((pCfg->data_width == DATA_WIDTH_32)
                        && (pCfg->sdram_type == SDR_SDRAM)
                    )
                {
                    pCfg->data_width = DATA_WIDTH_16;
                    EMC_DataWidth_Set(pCfg);
                    EMC_CSx_Burst_Set(EMC_CS0, pCfg);
                    EMC_CSx_Burst_Set(EMC_CS1, pCfg);
                }
                else
                {
                    SCI_ASSERT(0);
                    while(1);
                }
            }
            break;
        case STATE_COLUM:
            if (__is_rw_ok(ZERO_ADDR, MEM_REF_DATA0))
            {
                state = STATE_COLUM;
            }
            else
            {
            }
            
            break;
        default:
            break;
        }    
    }

    return TRUE;
}

LOCAL BOOLEAN SDRAM_Mode_SelfAdapt(SDRAM_CFG_INFO_T_PTR	pCfg)
{
    uint32 state = STATE_SDRAM_TYPE;
    uint32 err_line = 0;
    BOOLEAN update_mode = TRUE;
    uint32 colum_mode, row_mode;

    pCfg->row_mode     = ROW_MODE_14;
    pCfg->col_mode     = COL_MODE_12;
    pCfg->data_width   = DATA_WIDTH_16;
    pCfg->burst_length = BURST_LEN_2_WORD;
    pCfg->cas_latency  = CAS_LATENCY_3;
    pCfg->ext_mode_val = SDRAM_EXT_MODE_REG;
    pCfg->sdram_type   = SDR_SDRAM;


    EMC_PHY_Mode_Set(pCfg);


    while (STATE_END != state)
    {
        if (update_mode)
        {
            EMC_AddrMode_Set(pCfg);
            //__sdram_set_param(EMC_CLK, pCfg);
            EMC_CSx_Burst_Set(EMC_CS0, pCfg);
            EMC_CSx_Burst_Set(EMC_CS1, pCfg);
        }

        switch (state)
        {
        case STATE_SDRAM_TYPE:
            if (__is_rw_ok(ZERO_ADDR, MEM_REF_DATA0))          // 16bit sdr/ddr detect ok, try 32bit
            {
                pCfg->data_width = DATA_WIDTH_32;
                state = STATE_BIT_WIDTH;
                update_mode = TRUE;
            }
            else                        // 16bit sdr failed, try 16bit ddr
            {
                if (DDR_SDRAM == pCfg->sdram_type)
                {
                    err_line = __LINE__;
                    goto err_print;
                }
                pCfg->sdram_type = DDR_SDRAM;
                state = STATE_SDRAM_TYPE;
                update_mode = TRUE;

                EMC_SoftReset();
                SDRAM_Type_Set(pCfg);
            }
            break;
        case STATE_BIT_WIDTH:
            if (__is_rw_ok(ZERO_ADDR, MEM_REF_DATA0))          // 32bit sdr/ddr detect ok, try colum
            {
                state = STATE_COLUM;
                update_mode = FALSE;
            }
            else                        // 32bit sdr/ddr detect failed, fix 16bit and try colum
            {
                pCfg->data_width = DATA_WIDTH_16;
                state = STATE_COLUM;
                update_mode = TRUE;
            }
            break;
        case STATE_COLUM:
            if (__is_rw_ok(ZERO_ADDR, MEM_REF_DATA0))
            {
                __col_row_detect(pCfg, STATE_COLUM, &colum_mode);
                state = STATE_ROW;

                if (pCfg->col_mode != colum_mode)
                {
                    pCfg->col_mode = colum_mode;
                    update_mode = TRUE;
                }
                else
                {
                    update_mode = FALSE;
                }
            }
            else
            {
                err_line = __LINE__;
                goto err_print;
            }
            break;
        case STATE_ROW:
            if (__is_rw_ok(ZERO_ADDR, MEM_REF_DATA0))
            {
                __col_row_detect(pCfg, STATE_ROW, &row_mode);
                state = STATE_REINIT;

                if (pCfg->row_mode != row_mode)
                {
                    pCfg->row_mode = row_mode;
                    update_mode = TRUE;
                }
                else
                {
                    update_mode = FALSE;
                }
            }
            else
            {
                err_line = __LINE__;
                goto err_print;
            }
            break;
        case STATE_REINIT:
            if (!__is_rw_ok(ZERO_ADDR, MEM_REF_DATA0))
            {
                err_line = __LINE__;
                goto err_print;
            }

            state = STATE_END;
            break;
        default:
            err_line = __LINE__;
            goto err_print;
            //break;
        }
    }

    return TRUE;

err_print:
#if 0
    SCI_TraceLow("\r\nERR! row_mode:0x%x col_mode:0x%x data_width:0x%x \
burst_length:%d cas_latency:0x%x ext_mode_val:0x%x sdram_type:0x%x state:0x%x %s %d",
                 pCfg->row_mode, pCfg->col_mode,
                 pCfg->data_width, pCfg->burst_length,
                 pCfg->cas_latency, pCfg->ext_mode_val,
                 pCfg->sdram_type, state, __func__, err_line);
#endif
    return FALSE;
}
#endif


#if 1
/*****************************************************************************/
//  Description:	EMC Memroy mode set function
//				set external memory work mode parameter like:
//				data width
//				column mode
//				row mode and so on
//  Global resource dependence:  NONE
//  Author:		Johnny.Wang
//  Related register: EMC_DCFG0
//
//  Note:			None
//
/*****************************************************************************/
LOCAL void EMC_MEM_Mode_Set(SDRAM_CFG_INFO_T_PTR mem_info)
{
    uint32 i = 0;

    i = REG32(EXT_MEM_DCFG0);
    i &= ~(0XFF00 | 0X7F);
    i |= (EMC_CS_AREF_ALL<<15) 		|
         (TRUE			<< 14) 		|//hardware auto-refresh en
         (TRUE			<< 13) 		|//mode1 en
         (TRUE			<< 12) 		|//mode0 en
         (TRUE			<< 11) 		|//dmem clock output phase
         (TRUE			<< 10) 		|//dmem clock output en
         (TRUE			<< 9)   	|//row hit en
         (mem_info->data_width<< 8) |
         (mem_info->col_mode << 4)  |
         (0<< 2)  					|//0:A10 1:A11 2:A12 3:A13
         (mem_info->row_mode - 11);

    REG32(EXT_MEM_DCFG0) = i;
}


/*****************************************************************************/
//  Description:	EMC phy latency set function
//				set parameter relate with cas latency like:
//				timing adjustment sample clock latency
//				DQS output latency
//				write dm latency
//				read dm latency
//				write data latency
//				read data latency
//  Global resource dependence:  dram type , cas_latency
//  Author:		Johnny.Wang
//  Related register: EMC_DCFG5
//
//  Note:			None
//
/*****************************************************************************/
PUBLIC void EMC_PHY_Latency_Set(SDRAM_CFG_INFO_T_PTR mem_info)
{
    if (SDR_SDRAM == mem_info->sdram_type)
    {
        if (DATA_WIDTH_16 == mem_info->data_width)
        {
            if (CAS_LATENCY_2 == mem_info->cas_latency)
            {
                REG32(EXT_MEM_DCFG5) = 0x00400007;
            }
            else
            {
                REG32(EXT_MEM_DCFG5) = 0x00600209;
            }
        }
        else
        {
            if (CAS_LATENCY_2 == mem_info->cas_latency)
            {
                REG32(EXT_MEM_DCFG5) = 0x00400007;
            }
            else
            {
                REG32(EXT_MEM_DCFG5) = 0x00600209;
            }
        }
    }
    else
    {
        if (DATA_WIDTH_16 == mem_info->data_width)
        {
            if (CAS_LATENCY_2 == mem_info->cas_latency)
            {
                REG32(EXT_MEM_DCFG5) = 0x00622728;
            }
            else
            {
                REG32(EXT_MEM_DCFG5) = 0x0062272A;
            }
        }
        else
        {
            if (CAS_LATENCY_2 == mem_info->cas_latency)
            {
                REG32(EXT_MEM_DCFG5) = 0x00622728;
            }
            else
            {
                REG32(EXT_MEM_DCFG5) = 0x00622728;
            }
        }
    }
}
/*****************************************************************************/
//  Description:	EMC phy mode set function
//				set parameter relate with emc phy work mode like:
//				cke map to cs0 or cs1
//				dqs gate delay,delay line or loopback
//				dqs gate mode,mode0 or mode1
//				DMEM data output mode,dff or dl
//				DMEM DDR DQS[3:0] output mode,dff or dl
//				DMEM DDR DQS PAD IE mode,dff or dl
//				DMEM sample clock mode,internal or from dqs
//				DMEM CK/CK# output mode,dff or dl
//				DMEM READ strobe clock loopback dis/en
//  Global resource dependence:  dll on or off, external memory type
//  Author:		Johnny.Wang
//  Related register: EMC_CFG1
//
//  Note:			None
//
/*****************************************************************************/
LOCAL void EMC_PHY_Mode_Set(SDRAM_CFG_INFO_T_PTR mem_info)
{
    uint32 i = 0;

    SCI_ASSERT(mem_info != NULL);

    i = REG32(EXT_MEM_CFG1);
    i &= ~((3<<14)|0x3ff);
    i |=(EMC_CKE_SEL_DEFAULT << 14) |
        (EMC_DQS_GATE_DEFAULT<< 8)	|
        (EMC_DQS_GATE_MODE_DEFAULT<< 7) |
        (mem_info->sdram_type<< 6) |//DMEM data output mode,0:dff 1:dl
        (0<<5) |//DMEM DDR DQS[3:0] output mode,0:dff 1:dl
        (0<<4) |//DMEM DDR DQS PAD IE mode,0:dff 1:dl
        (mem_info->sdram_type<<3) |//DMEM sample clock mode,0:internal 1:out-of-chip
        (0<<2) |//DMEM CK/CK# output mode,0:dff 1:dl
        (0<<1) |//DMEM READ strobe clock loopback 0:dis 1:en
        mem_info->sdram_type;

    REG32(EXT_MEM_CFG1) = i;

#ifdef FPGA_TEST
    //REG32(EXT_MEM_CFG1) |= BIT_6;
#endif
}



/*****************************************************************************/
//  Description:	EMC phy timing set function
//				set parameter relate with emc phy work mode like:
//				data pad ie delay
//				data pad oe delay
//				dqs pad ie delay
//				dqs pad oe delay
//				all delay line timing parameter
//  Global resource dependence:  dll on or off, external memory type
//  Author:		Johnny.Wang
//  Related register: EMC_DCFG6,7,8 and EMC_DMEM_DL0~DL19
//  Note:			None
//
/*****************************************************************************/
PUBLIC void EMC_PHY_Timing_Set(SDRAM_CFG_INFO_T_PTR mem_info,
                        EMC_PHY_L1_TIMING_T_PTR emc_phy_l1_timing,
                        EMC_PHY_L2_TIMING_T_PTR emc_phy_l2_timing)
{
    uint32 i = 0;

    SCI_ASSERT((mem_info != NULL) && (emc_phy_l1_timing != NULL) && (emc_phy_l2_timing != NULL));

    REG32(EXT_MEM_DCFG8) = ((emc_phy_l1_timing->data_pad_ie_delay & 0xffff) <<16) |
                           (emc_phy_l1_timing->data_pad_oe_delay & 0xff);


    if (DDR_SDRAM == mem_info->sdram_type)
    {
        REG32(EXT_MEM_DCFG6) = ((emc_phy_l1_timing->dqs_gate_pst_delay& 0xffff) <<16) |
                               (emc_phy_l1_timing->dqs_gate_pre_delay& 0xffff);

        REG32(EXT_MEM_DCFG7) = ((emc_phy_l1_timing->dqs_ie_delay& 0xffff) <<16) |
                               (emc_phy_l1_timing->dqs_oe_delay& 0xff);

#if EMC_DLL_ON_OFF
        {
            REG32(EXT_MEM_CFG0_DLL) = 0x11080; //DLL and compensation en

            WAIT_EMC_DLL_LOCK;
        }
#else
        {
            REG32(EXT_MEM_CFG0_DLL) = 0x0; //DLL disable
        }
#endif

        for (i = 0; i < 20; i++)
        {
            REG32(EXT_MEM_DL0 + i*4) = REG32((unsigned int)emc_phy_l2_timing + i * 4);
        }

#if (EMC_DLL_ON_OFF == DLL_ON)
        REG32(EXT_MEM_CFG0_DLL) |= DCFG0_DLL_COMPENSATION_EN;
#endif

    }
}

/*****************************************************************************/
//  Description:	EMC phy  set function
//				include these subfunction:
//				EMC_PHY_Latency_set(),
//				EMC_PHY_Mode_Set(),
//				EMC_PHY_Timing_Set()
//  Global resource dependence:  dll on or off, external memory type
//  Author:		Johnny.Wang
//  Related register:
//  Note:			None
//
/*****************************************************************************/
void EMC_PHY_Set(uint32 emc_freq,
					SDRAM_CFG_INFO_T_PTR mem_info,
					EMC_PHY_L1_TIMING_T_PTR emc_phy_l1_timing,
					EMC_PHY_L2_TIMING_T_PTR emc_phy_l2_timing)
{
	EMC_PHY_Latency_Set(mem_info);
	EMC_PHY_Mode_Set(mem_info);
	EMC_PHY_Timing_Set(mem_info, emc_phy_l1_timing, emc_phy_l2_timing);
}

void EMC_Init(uint32 emc_freq,
				EMC_CHL_NUM_E emc_axi_num,
				SDRAM_CFG_INFO_T_PTR mem_info,
				SDRAM_TIMING_PARA_T_PTR mem_timing,
				EMC_PHY_L1_TIMING_T_PTR emc_phy_l1_timing,
				EMC_PHY_L2_TIMING_T_PTR emc_phy_l2_timing)
{
	EMC_CHL_NUM_E i = 0;
	
	EMC_Base_Mode_Set();
	EMC_CSx_Burst_Set(EMC_CS0,mem_info);
	EMC_CSx_Burst_Set(EMC_CS1,mem_info);	
	
	if(emc_axi_num <= EMC_AHB_MAX)
	{
		for(i = 0; i <= EMC_AHB_MAX; i++)
		{
			EMC_AHB_CHL_Set(i,0);
		}
	}
	else
	{
		EMC_AXI_CHL_Set(emc_axi_num);
	}
	
	EMC_MEM_Mode_Set(mem_info);
	EMC_MEM_Timing_Set(emc_freq,mem_info,mem_timing);
	EMC_PHY_Set(emc_freq,mem_info,emc_phy_l1_timing,emc_phy_l2_timing);
 	EMC_SCMD_Issue(mem_info);
 	delay(100);
}

#endif



void set_emc_pad(uint32 dqs_drv,uint32 data_drv,uint32 ctl_drv, uint32 clk_drv)
{
    uint32 i = 0;
    //ckdp
    REG32(PINMAP_REG_BASE + 0X2e8) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X2e8) |= clk_drv<<8;

    //ckdm
    REG32(PINMAP_REG_BASE + 0X2ec) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X2ec) |= clk_drv<<8;

    //addr
    for (i = 0; i<14; i++)
    {
        REG32(PINMAP_REG_BASE + 0x20c + i*4) &= ~0x300;
        REG32(PINMAP_REG_BASE + 0x20c + i*4) |= ctl_drv<<8;
    }

    //cke1
    REG32(PINMAP_REG_BASE + 0x29c) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X29c) |= ctl_drv<<8;

    //rasn
    REG32(PINMAP_REG_BASE + 0x2f0) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X2f0) |= ctl_drv<<8;
    //casn
    REG32(PINMAP_REG_BASE + 0x2f4) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X2f4) |= ctl_drv<<8;
    //wen
    REG32(PINMAP_REG_BASE + 0x2f8) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X2f8) |= ctl_drv<<8;
    //csn0
    REG32(PINMAP_REG_BASE + 0x2fc) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X2fc) |= ctl_drv<<8;
    //csn1
    REG32(PINMAP_REG_BASE + 0x300) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X300) |= ctl_drv<<8;
    //gpre_loop
    REG32(PINMAP_REG_BASE + 0x304) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X304) |= ctl_drv<<8;
    //gpst_loop
    REG32(PINMAP_REG_BASE + 0x308) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X308) |= ctl_drv<<8;
    //bank0
    REG32(PINMAP_REG_BASE + 0x30c) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X30c) |= ctl_drv<<8;
    //bank1
    REG32(PINMAP_REG_BASE + 0x310) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X310) |= ctl_drv<<8;
    //cke0
    REG32(PINMAP_REG_BASE + 0x314) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X314) |= ctl_drv<<8;

    //data
    for (i = 0; i<8; i++)
    {
        REG32(PINMAP_REG_BASE + 0x244 + i*4) &= ~0x300;
        REG32(PINMAP_REG_BASE + 0x26c + i*4) &= ~0x300;
        REG32(PINMAP_REG_BASE + 0x2c0 + i*4) &= ~0x300;

        REG32(PINMAP_REG_BASE + 0x244 + i*4) |= data_drv<<8;
        REG32(PINMAP_REG_BASE + 0x26c + i*4) |= data_drv<<8;
        REG32(PINMAP_REG_BASE + 0x2c0 + i*4) |= data_drv<<8;
    }
    for (i = 0; i<2; i++)
    {
        REG32(PINMAP_REG_BASE + 0x294 + i*4) &= ~0x300;
    }
    for (i = 0; i<6; i++)
    {
        REG32(PINMAP_REG_BASE + 0x2a0 + i*4) &= ~0x300;
    }


    //dqs
    REG32(PINMAP_REG_BASE + 0X268) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X290) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X2bc) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X2e4) &= ~0x300;

    REG32(PINMAP_REG_BASE + 0X268) |= dqs_drv<<8;
    REG32(PINMAP_REG_BASE + 0X290) |= dqs_drv<<8;
    REG32(PINMAP_REG_BASE + 0X2bc) |= dqs_drv<<8;
    REG32(PINMAP_REG_BASE + 0X2e4) |= dqs_drv<<8;

    //dqm
    REG32(PINMAP_REG_BASE + 0X264) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X28c) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X2b8) &= ~0x300;
    REG32(PINMAP_REG_BASE + 0X2e0) &= ~0x300;

    REG32(PINMAP_REG_BASE + 0X264) |= data_drv<<8;
    REG32(PINMAP_REG_BASE + 0X28c) |= data_drv<<8;
    REG32(PINMAP_REG_BASE + 0X2b8) |= data_drv<<8;
    REG32(PINMAP_REG_BASE + 0X2e0) |= data_drv<<8;


}


void set_sc7702_clk(void)
{

    //CLK_ARM = 230.4MHZ  CLK_AHB=57.5MHZ CLK_EMC=96MHZ
    {
        uint32 tmp_clk = 0;

        //disable DSP affect clk_emc
        REG32(DSP_BOOT_EN)  |=  BIT_2;  // ARM access DSP ASHB bridge enable
        REG32(AHB_CTL1)     &= ~BIT_16; // ARM disable matrix to sleep
        REG32(0x10130010)   |=  BIT_28; // DSP Zbus 32bit access enable
        REG32(0x1013000C)   |= (0xf<<12);// bit[15:12], DSP SIDE clk_emc_drv
        REG32(0x1013000C)   |= (3<<10); //bit[11:10], DSP SIDE clk_emc_sel: 0:384M, 1:256M, 2:230M 3:26M

        REG32(0x10130010)   &= ~BIT_28; // DSP Zbus 32bit access disable
        REG32(AHB_CTL1)     |=  BIT_16; // ARM enable matrix to sleep
        REG32(DSP_BOOT_EN)  &= ~BIT_2;  // ARM access DSP ASHB bridge enable


        REG32(AHB_ARM_CLK) |= (BIT_23|BIT_24); //set clk_mcu =26MHz

        tmp_clk |=                      // bit[31],    reserved
            (0 << 30)        |   // bit[30],    clk_mcu div2 en
            // bit[29:25], read only
            (0 << 23)        |   // bit[24:23], clk_mcu select, 0:460.8M(MPLL),1:153.6M,2:64MHZ,3:26MHZ
            // bit[22:19], reserved
            (1 << 17)        |   // bit[18:17], clk_arm_if div, clk_mcu/(n+1)
            // bit[16:14], reserved
            (0 << 12)        |   // bit[13:12], clk_emc async sel: 0:384M, 1:256M, 2:230M 3:26M
            (1 << 8)         |   // bit[11:8],  clk_emc async div: (n+1)
            (0 << 0)         |   // bit[7],     clk_mcu highest freq set 0:460.8M 1:384M
            (1 << 4)         |   // bit[6:4],   clk_ahb div clk_arm_if/(n+1)
            (0 << 3)  ;          // bit[3],     emc sync:1, async:0
        // bit[2:0],   reserved

        REG32(AHB_ARM_CLK) = tmp_clk;

        REG32(AHB_ARM_CLK) &= ~(BIT_23|BIT_24);
    }
    return;

}

void EMC_CHL_Init(EMC_CHL_NUM_E emc_axi_num)
{
    int i;

    if ((emc_axi_num >= EMC_AHB_MIN) && (emc_axi_num < EMC_AHB_MAX))
    {
        for (i = EMC_AHB_MIN; i < EMC_AHB_MAX; i++)
        {
            EMC_AHB_CHL_Set(i, 0);
        }
    }
    else
    {
        EMC_AXI_CHL_Set(emc_axi_num);
    }
}


void DMC_Init(uint32 clk)
{
    uint32 emc_freq = clk;
    SDRAM_CFG_INFO_T_PTR    mem_info = SDRAM_GetCfg();
    SDRAM_TIMING_PARA_T_PTR mem_timing = SDRAM_GetTimingPara();
    EMC_PHY_L1_TIMING_T_PTR emc_phy_l1_timing = NULL;
    EMC_PHY_L2_TIMING_T_PTR emc_phy_l2_timing = EMC_GetPHYL2_Timing();
    BOOLEAN ret = TRUE;
//    set_emc_pad(1,1,0,2);
//    set_sc7702_clk();	//open only when chip test
//    emc_phy_l1_timing = EMC_GetPHYL1_Timing(mem_info->sdram_type, mem_info->cas_latency);
//    EMC_Init(EMC_CLK, EMC_AHB_ARM0, mem_info, mem_timing, emc_phy_l1_timing, emc_phy_l2_timing);
//    DRAM_CAP =  SDRAM_GetCap(mem_info); // get size

        EMC_CHL_Init(EMC_AXI_ARM);

#ifdef SDRAM_AUTODETECT_SUPPORT
        ret = SDRAM_Mode_SelfAdapt(mem_info);
#else
        EMC_AddrMode_Set(mem_info);
        EMC_CSx_Burst_Set(EMC_CS0, mem_info);
        EMC_CSx_Burst_Set(EMC_CS1, mem_info);
        SDRAM_Type_Set(mem_info);
#endif

        DRAM_CAP =  SDRAM_GetCap(mem_info); // get size

        EMC_PHY_Latency_Set(mem_info);
        EMC_PHY_Mode_Set(mem_info);

        emc_phy_l1_timing = EMC_GetPHYL1_Timing(mem_info->sdram_type, mem_info->cas_latency);
        EMC_PHY_Timing_Set(mem_info, emc_phy_l1_timing, emc_phy_l2_timing);

        EMC_MEM_Timing_Set(emc_freq, mem_info, mem_timing);

        EMC_SCMD_Issue(mem_info);
        delay(100);

    return;
}

PUBLIC void Chip_Init (void) /*lint !e765 "Chip_Init" is used by init.s entry.s*/
{
	DMC_Init(EMC_CLK);
}

#ifdef   __cplusplus
}
#endif




