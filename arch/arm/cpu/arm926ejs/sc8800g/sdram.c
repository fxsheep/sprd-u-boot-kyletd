/******************************************************************************
    David.Jia   2007.10.29      share_version_union

    TCC -fpu None -O2 -bi -g+ -apcs /interwork -D__RUN_IN_SDRAM sdram_init.c
SC6600R    -D_BL_NF_SC6600R_
SC6600H    -D_BL_NF_SC6600H_
SC6600I    -D_BL_NF_SC6600I_
SC6800     -gtp -cpu ARM926EJ-S -D_REF_SC6800_ -D_BL_NF_SC6800_
******************************************************************************/

#include "asm/arch/sci_types.h"
#include "asm/arch/arm_reg.h"
#include "asm/arch/sdram_cfg.h"
#include "asm/arch/chng_freq.h"
#include "asm/arch/sc_reg.h"
#include "asm/arch/sdram.h"
#include "asm/arch/bl_trace.h"
#include "asm/arch/chip.h"
#include "asm/arch/bl_mmu.h"
#define  SDRAM_EXT_INVALID     0xffffffff       //@David.Jia 2008.1.7

uint32 g_ahb_clk;
unsigned int g_emc_clk;

extern SYS_CLK_CFG_INFO *Get_system_clk_cfg (void);

#ifdef CHIP_VER_8800G2
#define INTERFACE_CLK_MAX   ARM_CLK_200M
typedef struct ARM_EMC_AHB_CLK_TAG 
{
    uint32 mcu_clk;
    uint32 arm_clk;
    uint32 emc_clk;
    uint32 ahb_clk;
}
ARM_EMC_AHB_CLK_T;

typedef enum MCU_CLK_TYPE_TAG
{
    ARM400_EMC200_AHB100 = 0,   //SC8800G2
    ARM256_EMC200_AHB64,        //SC8801G2
    ARM192_EMC200_AHB96,        //SC8802G2
    ARM256_EMC192_AHB64,
    ARM256_EMC128_AHB64,
    ARM192_EMC192_AHB96,

    MCU_CLK_TYPE_MAX   
}
MCU_CLK_TYPE_E;

LOCAL CONST ARM_EMC_AHB_CLK_T s_arm_emc_ahb_clk[] = 
{
//     mcu_clk       arm_clk        emc_clk       ahb_clk
    {ARM_CLK_400M, ARM_CLK_400M, ARM_CLK_200M, ARM_CLK_100M},   //SC8800G2
    {ARM_CLK_256M, ARM_CLK_256M, ARM_CLK_200M, ARM_CLK_64M},    //SC8801G2
    {ARM_CLK_192M, ARM_CLK_192M, ARM_CLK_200M, ARM_CLK_96M},    //SC8802G2
    {ARM_CLK_512M, ARM_CLK_256M, ARM_CLK_192M, ARM_CLK_64M},
    {ARM_CLK_512M, ARM_CLK_256M, ARM_CLK_128M, ARM_CLK_64M},
    {ARM_CLK_384M, ARM_CLK_192M, ARM_CLK_192M, ARM_CLK_96M},
};

uint32 CHIP_GetMPllClk (void)
{
    return ( ARM_CLK_26M
               / ( (REG32(GR_MPLL_MN) & 0x003F0000) >>16)
               * (REG32(GR_MPLL_MN) & 0x00000FFF) );
}

void CHIP_SetMPllClk (uint32 clk)
{
    uint32 M, N, tmp_mn;

    M = 13; // M: fix to 13
    N = clk/2/1000000;
    
    tmp_mn  = (REG32(GR_MPLL_MN) & (~0x3FFFFF));
    tmp_mn |= (M << 16) | N;  
              
    REG32(GR_GEN1) |= BIT_9;        // MPLL Write En
    REG32(GR_MPLL_MN) = tmp_mn;
    REG32(GR_GEN1) &= ~BIT_9;       // MPLL Write Dis
}

void __ClkConfig(uint32 *emcclk, uint32 *ahbclk)
{
    uint32 tmp_clk, mcuclk, armclk;
    uint32 mcu_div, if_div;
    BOOLEAN is_mpll, is_async;
    MCU_CLK_TYPE_E clk_type = ARM400_EMC200_AHB100;

    ///*
    clk_type = (0 == (REG32(GR_GEN3) & 0x3)) ?      
               (ARM400_EMC200_AHB100) :             // 8800G2
               ((1 == (REG32(GR_GEN3) & 0x3)) ?     
               (ARM256_EMC200_AHB64) :              // 8801G1
               ((2 == (REG32(GR_GEN3) & 0x3)) ?
               (ARM192_EMC200_AHB96) :           // 8802G1
               (ARM400_EMC200_AHB100)));              // 6810 
    //*/         
    
    mcuclk   = s_arm_emc_ahb_clk[clk_type].mcu_clk;
    armclk   = s_arm_emc_ahb_clk[clk_type].arm_clk;
    *emcclk  = s_arm_emc_ahb_clk[clk_type].emc_clk;
    *ahbclk  = s_arm_emc_ahb_clk[clk_type].ahb_clk;
    
    is_mpll  = ((0 == (REG32(GR_GEN3) & 0x3)) ? (SCI_TRUE) : (SCI_FALSE));
    is_async = ((0 == (armclk%(*emcclk)))     ? (SCI_FALSE) : (SCI_TRUE));  

    mcu_div = 0;    
    if(is_mpll)
    {
        if(CHIP_GetMPllClk() != mcuclk)
        {
            CHIP_SetMPllClk(mcuclk);
        }
        if(armclk != mcuclk)
        {
            mcu_div = 1;
        }
    }

    if_div = 0;
    if(armclk > INTERFACE_CLK_MAX)
    {
        if_div = 1;
    }
    
    // step 1: config emc clk in dsp side in async mode
    if(is_async) 
    {
        REG32(AHB_DSP_BOOT_EN)  |= BIT_2;
        REG32(AHB_CTL1)         &= ~BIT_16;
        REG32(0x10130010)       |= BIT_28;

        REG32(0x1013000C)       |= (2<<10); //bit[11:10], DSP SIDE: 1:200M, 2:192M 3:26M
        
        REG32(0x10130010)       &= ~BIT_28;
        REG32(AHB_CTL1)         |= BIT_16;
        REG32(AHB_DSP_BOOT_EN)  &= ~BIT_2;
    }
    
    // step 2: first config divider 
    //         ifdiv / mcudiv /  ahbdiv
    //         MCU_26M / sync_mode / emc-async-sel
    tmp_clk = (BIT_23|BIT_24);
    tmp_clk |= (if_div  << 31)  |   // bit[31],    interface-div
               (mcu_div << 30)  |   // bit[30],    mcu-div
                                    // bit[29:25], read only
                                    // bit[24:23], mcu-sel, should config at step 2
                                    // bit[22:17], reserved
               (0 << 14)        |   // bit[16:14], emc-sync-div: 2^n
               (1 << 12)        |   // bit[13:12], emc-async-sel: 0:reserved, 1:200M, 2:192M 3:26M
               (0 << 8)         |   // bit[11:8],  emc-async-div: (n+1)
                                    // bit[7],     ARM_384M_SEL:0
               (1 << 4)         |   // bit[6:4],   ahb-div: (n+1)
               (1 << 3)  ;          // bit[3],     emc sync:1, async:0
                                    // bit[2:0],   arm-div, only for G1
               
    REG32(AHB_ARM_CLK) = tmp_clk;

    // step 3: config mcu-sel 
    tmp_clk &= ~(BIT_23|BIT_24); // mcu-sel: 26*N/M, this bit must be set after divider
    REG32(AHB_ARM_CLK) = tmp_clk;

    //step 4: switch to async mode at last
    if(is_async)
    {
        tmp_clk &= ~BIT_3;
        REG32(AHB_ARM_CLK) = tmp_clk;  
    }

    return;
}
#endif

#if defined(PLATFORM_SC8800H)


/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SDRAM_GenMemCtlCfg(SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)           *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     set emc channel parameters                                            *
 **                                                                           *
 ** INPUT                                                                     *
 **     sdram_cfg_ptr                                                         *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *
**---------------------------------------------------------------------------*/
void SDRAM_GenMemCtlCfg (SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)
{
    uint32 reg_val;
    uint32 rburst_length = sdram_cfg_ptr->burst_length;
    uint32 wburst_length = sdram_cfg_ptr->burst_length;
    uint32 data_width    = sdram_cfg_ptr->data_width;

    rburst_length &= 0x3;
    wburst_length &= 0x3;

    //Config channel2
    //Enable ch2, no switch
    REG32 (EXT_MEM_CFG0) |= (BIT_2|BIT_12|BIT_13);//lint !e718

    //Config channel0 big endian
    REG32 (EXT_MEM_CFG0) |= BIT_4;

    //Disable all channel eburst_hit_en
    REG32 (EXT_MEM_CFG0) &= ~ (BIT_20|BIT_21|BIT_22|BIT_23);

    //DMEM enable, (CS0)-->DMEM
    REG32 (EXT_MEM_CFG1) |= (BIT_0|BIT_10);   //DMEM enable
    REG32 (EXT_MEM_CFG1) &= ~ (BIT_12);       //Clear smem_only_en
    REG32 (EXT_MEM_CFG1) &= ~ (BIT_9);        //SDRAM mode

    // CS [1:0] map to HADDR [28:27]
#if defined (BB_DRAM_TYPE_128MB_32BIT)
    REG32 (EXT_MEM_CFG1) &= ~ (BIT_4 | BIT_5);
    REG32 (EXT_MEM_CFG1) |= BIT_5;
#endif

    if (data_width == DATA_WIDTH_16)
    {
        //Config read/write latency for cs0 here...
        if (rburst_length == BURST_LEN_8)
        {
            rburst_length = BURST_LEN_4;
        }
        else if (rburst_length == BURST_LEN_4)
        {
            rburst_length = BURST_LEN_2;
        }

        if (wburst_length == BURST_LEN_8)
        {
            wburst_length = BURST_LEN_4;
        }
        else if (wburst_length == BURST_LEN_4)
        {
            wburst_length = BURST_LEN_2;
        }

        reg_val = REG32 (EXT_MEM_CFG2);
        reg_val &= ~ (0x3 | (0x3<<8));
        reg_val |= (rburst_length | (wburst_length<<8));
        REG32 (EXT_MEM_CFG2) = reg_val;

    }
    else if (data_width == DATA_WIDTH_32)
    {
        //Config read/write latency for cs0 here...
        reg_val = REG32 (EXT_MEM_CFG2);
        reg_val &= ~ (0x3 | (0x3<<8));
        reg_val |= (rburst_length | (wburst_length<<8));
        REG32 (EXT_MEM_CFG2) = reg_val;
    }
    else
    {
        while (1);
    }
}


/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SDRAM_DMemCtlCfg(uint32 sdram_clk,SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)  *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     set emc dmem mode,timing parameters                                   *
 **                                                                           *
 ** INPUT                                                                     *
 **     sdram_clk,sdram_cfg_ptr                                               *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *
**---------------------------------------------------------------------------*/
void SDRAM_DMemCtlCfg (uint32 clk,SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)
{
    uint32 i=0,clk_liv=0,j=0;
    uint32 t_rtw = 0;

    //initialize dmem mode parameters
    uint32 row_mode      = sdram_cfg_ptr->row_mode;
    uint32 col_mode      = sdram_cfg_ptr->col_mode;
    uint32 data_width    = sdram_cfg_ptr->data_width;
    uint32 cas_latency   = sdram_cfg_ptr->cas_latency;
    uint32 write_latency = 0;
    uint32 sdram_cycle_ns = 1000/ (clk);
    uint32 row_number     = 0xFFFFFFFF;
    SDRAM_TIMING_PARA_T_PTR sdram_parameters = SDRAM_GetTimingPara();

    //initialize dmem timing parameters
    uint32 t_ras = sdram_parameters->ras_min    /sdram_cycle_ns;
    uint32 t_xsr = sdram_parameters->xsr_min    /sdram_cycle_ns;
    uint32 t_rfc = sdram_parameters->rfc_min    /sdram_cycle_ns;
    uint32 t_mrd = sdram_parameters->mrd_min    /sdram_cycle_ns;
    uint32 t_wr  = sdram_parameters->wr_min     /sdram_cycle_ns;
    uint32 t_rcd = sdram_parameters->rcd_min    /sdram_cycle_ns;
    uint32 t_rp  = sdram_parameters->row_pre_min/sdram_cycle_ns;
    uint32 t_ref = sdram_parameters->row_ref_max/sdram_cycle_ns;
    uint32 t_wtr = 0x0;

    //calculate t_rtw by cas_latnecy
    if (cas_latency == 3)
    {
        t_rtw = 3;
    }
    else if (cas_latency == 2)
    {
        t_rtw = 2;
    }
    else
        while (1);

    //calculate row_mode by row_mode
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

    //set dmem dcfg0 mode paramters
    REG32 (EXT_MEM_DCFG0) = (DCFG0_BKPOS_HADDR_24_23     |   \
                             (data_width<<3)             |   \
                             (row_mode<<4)               |   \
                             DCFG0_AUTO_PRE_POSITION_A10 |   \
                             (col_mode<<8)               |   \
                             DCFG0_CLKDMEM_OUT_EN        |   \
                             DCFG0_ALTERNATIVE_EN        |   \
                             DCFG0_ROWHIT_EN             |   \
                             (t_ref<<20)                     \
                            );

    //set dmem dcfg1 timing parameters
    REG32 (EXT_MEM_DCFG1) = ( (t_rp <<0) |        \
                              (t_rcd<<2) |        \
                              (t_wr <<4) |        \
                              (t_rfc<<8) |        \
                              (t_xsr<<12) |        \
                              (t_ras<<16) |        \
                              (t_rtw<<20) |        \
                              (t_wtr<<24) |        \
                              (t_mrd<<28)         \
                            );

    //set dmem dcfg2 refresh cnt parameters
    REG32 (EXT_MEM_DCFG2) = ( (1<<row_number) |DCFG2_REF_CNT_RST);
    REG32 (EXT_MEM_DCFG4) = (cas_latency | (write_latency<<4));
    REG32 (EXT_MEM_CFG1)  &= ~BIT_14;

    //enable emc clock out
    REG32 (EXT_MEM_DCFG0) |=  BIT_14;

    if (cas_latency == 3)
    {
        REG32 (EXT_MEM_DCFG4) = 0x00800209;
        REG32 (EXT_MEM_DCFG6) = 0x00400100;
    }
    else if (cas_latency == 2)
    {
        REG32 (EXT_MEM_DCFG4) = 0x00600007;
        REG32 (EXT_MEM_DCFG6) = 0x00100100;
    }
    else
    {
        while (1);
    }
}


/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SDRAM_Device_Init(SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)            *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     set emc dmem mode/ext mode register parameters                        *
 **                                                                           *
 ** INPUT                                                                     *
 **     sdram_cfg_ptr                                                         *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *
**---------------------------------------------------------------------------*/
void SDRAM_Device_Init (SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)
{
    uint8 mode_reg_bl     = 0xFF;
    uint8 mode_reg_bt     = MODE_REG_BT_SEQ;   //sequencial mode burst.
    uint8 mode_reg_cl     = 0xFF;
    uint8 mode_reg_opmode = MODE_REG_OPMODE;
    uint8 mode_reg_wb     = MODE_REG_WB_PRORAM;  //Programming burst length for write.
    uint32 ex_mode_reg    = 0;
    uint16 mode_reg       = 0;
    uint8  dsoft_cs       = 0; // command for CS0

    //calculate mode reg burst length
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

    //calculate mode reg cas latency
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

    //get mode reg parameter
    mode_reg = ( (mode_reg_wb<<9) | (mode_reg_opmode<<7)
                 | (mode_reg_cl<<4) | (mode_reg_bt<<3) | mode_reg_bl);

    //get ext-mode reg parameter
    ex_mode_reg = sdram_cfg_ptr->ext_mode_val;

    // Precharge all banks.
    REG32 (EXT_MEM_DCFG3) |= BIT_16 | (dsoft_cs<<28);

    while ( (REG32 (EXT_MEM_DCFG3)) & BIT_16);

    //Auto_ref
    REG32 (EXT_MEM_DCFG3) |= BIT_17 | (dsoft_cs<<28);

    while ( (REG32 (EXT_MEM_DCFG3)) & BIT_17);

    //Auto_ref again
    REG32 (EXT_MEM_DCFG3) |= BIT_17 | (dsoft_cs<<28);

    while ( (REG32 (EXT_MEM_DCFG3)) & BIT_17);

    //mode register load.
    REG32 (EXT_MEM_DCFG3) &= ~ (0xFFFF);
    REG32 (EXT_MEM_DCFG3) |= (mode_reg | BIT_18 | (dsoft_cs<<28));

    while ( (REG32 (EXT_MEM_DCFG3)) & BIT_18);

    //extended mode register load.
    if (ex_mode_reg != SDRAM_EXT_MODE_INVALID)
    {
        REG32 (EXT_MEM_DCFG3) &= ~ (0xFFFF);
        //REG32 (EXT_MEM_DCFG3) |= (uint32)(ex_mode_reg | BIT_18 | (dsoft_cs<<28));
        CHIP_REG_OR (EXT_MEM_DCFG3, (ex_mode_reg | BIT_18 | (dsoft_cs<<28))); //lint !e718

        while ( (REG32 (EXT_MEM_DCFG3)) & BIT_18);
    }
}





#elif defined(PLATFORM_SC6800H) || defined(PLATFORM_SC8800G)

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SDRAM_GenMemCtlCfg(SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)           *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     set emc channel parameters                                            *
 **                                                                           *
 ** INPUT                                                                     *
 **     sdram_cfg_ptr                                                         *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *
**---------------------------------------------------------------------------*/
void SDRAM_GenMemCtlCfg (SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)
{
    uint32 burst_length = sdram_cfg_ptr->burst_length;
    uint32 data_width   = sdram_cfg_ptr->data_width;
    uint32 dburst_rlength, dburst_wlength = 0;

    //cal burst length
    if (data_width == DATA_WIDTH_16)
    {
        switch (burst_length)
        {
            case BURST_LEN_1:
            case BURST_LEN_2:
                dburst_rlength = BURST_LEN_1;
                dburst_wlength = BURST_LEN_1;
                break;
            case BURST_LEN_4:
                dburst_rlength = BURST_LEN_2;
                dburst_wlength = BURST_LEN_2;
                break;
            case BURST_LEN_8:
                dburst_rlength = BURST_LEN_4;
                dburst_wlength = BURST_LEN_4;
                break;
            default:
                dburst_rlength = BURST_LEN_1;
                dburst_wlength = BURST_LEN_1;
                break;
        }
    }
    else if (data_width == DATA_WIDTH_32)
    {
        switch (burst_length)
        {
            case BURST_LEN_1:
                dburst_rlength = BURST_LEN_1;
                dburst_wlength = BURST_LEN_1;
                break;
            case BURST_LEN_2:
                dburst_rlength = BURST_LEN_2;
                dburst_wlength = BURST_LEN_2;
                break;
            case BURST_LEN_4:
                dburst_rlength = BURST_LEN_4;
                dburst_wlength = BURST_LEN_4;
                break;
            case BURST_LEN_8:
                dburst_rlength = BURST_LEN_8;
                dburst_wlength = BURST_LEN_8;
                break;
            default:
                dburst_rlength = BURST_LEN_1;
                dburst_wlength = BURST_LEN_1;
                break;
        }
    }
    else
    {
        while (1);
    }

    REG32 (EXT_MEM_CFG0) |= BIT_12; /*lint !e718*/ //big endian
    //set dmem parameter
    /*lint -save -e506 -e774*/
    if ( (SDR_SDRAM == DRAM_TYPE))
    {
        REG32 (EXT_MEM_CFG0) |= (BIT_1|BIT_3|BIT_6|BIT_9|BIT_10|BIT_11); //software address mapping
        REG32 (EXT_MEM_CFG1) = 0x00000008; //EMC phy set
        REG32 (EXT_MEM_CFG0_CS0) = ( (0x1<<12) | (dburst_wlength<<8) | (dburst_rlength<<4) | (0x3)); //EMC cs set
    }
    else if (DDR_SDRAM == DRAM_TYPE)
    {
        //REG32 (EXT_MEM_CFG0) |= (BIT_1|BIT_3|BIT_6|BIT_9|BIT_10|BIT_11); //software address mapping
        REG32(EXT_MEM_CFG0) |= (BIT_0|BIT_1|BIT_6|BIT_9|BIT_10|BIT_11); //software address mapping, 64M memory.
        REG32 (EXT_MEM_CFG0) |= (BIT_5);
        REG32 (EXT_MEM_CFG1) = 0x01080000; //EMC phy set
        REG32 (EXT_MEM_CFG0_CS0) = ( (0x1<<12) | (dburst_wlength<<8) | (dburst_rlength<<4) | (0x3)); //0x1113;
    }
    else
    {
        while (1);
    }

    //config channel 0-15
    REG32 (EXT_MEM_CFG0_CH0)  = 0x0003c31c;//0x0001c31c;
    REG32 (EXT_MEM_CFG0_CH1)  = 0x0003c31c;
    REG32 (EXT_MEM_CFG0_CH2)  = 0x0003c31c;
    REG32 (EXT_MEM_CFG0_CH3)  = 0x0003c31c;
    REG32 (EXT_MEM_CFG0_CH4)  = 0x0003c31c;
    REG32 (EXT_MEM_CFG0_CH5)  = 0x0003c31c;

#if defined(PLATFORM_SC8800G)
    REG32 (EXT_MEM_CFG0_CH5 + 4)  = 0x0001c31c; //for sc8800g emc sleep bug.
    REG32 (EXT_MEM_CFG0_CH5 + 8)  = 0x0001c31c;
    REG32 (EXT_MEM_CFG0_CH5 + 12)  = 0x0001c31c;
#endif
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SDRAM_DMemCtlCfg(uint32 sdram_clk,SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)  *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     set emc dmem mode,timing parameters                                   *
 **                                                                           *
 ** INPUT                                                                     *
 **     sdram_clk,sdram_cfg_ptr                                               *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *
**---------------------------------------------------------------------------*/
void SDRAM_DMemCtlCfg (uint32 sdram_clk,SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)
{
    //initialize dmem mode parameters
    uint32 row_mode       = sdram_cfg_ptr->row_mode;
    uint32 col_mode       = sdram_cfg_ptr->col_mode;
    uint32 data_width     = sdram_cfg_ptr->data_width;
    uint32 cas_latency    = sdram_cfg_ptr->cas_latency;
    uint32 sdram_cycle_ns = 1000/ (sdram_clk/1000000);
    uint32 row_number     = 0xFFFFFFFF;

    uint32 t_ref          = 0;
    uint32 t_rfc          = 0;
    uint32 t_rp           = 0;
    uint32 t_rcd          = 0;
    uint32 t_rrd          = 0;
    uint32 t_wr           = 0;
    uint32 t_xsr          = 0;
    uint32 t_ras          = 0;
    uint32 t_rtw          = 0;  //t_rtw is only for ddr
    uint32 t_wtr          = 0;  //t_wtr is only for ddr
    uint32 t_rtr          = 0;  //t_rtr is only for ddr
    uint32 t_mrd          = 0;

    SDRAM_TIMING_PARA_T_PTR sdram_parameters = SDRAM_GetTimingPara();


    //calculate row_mode by row_mode
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
    else if(row_mode == ROW_MODE_14)
    {
        row_number = 14;
    }
    else
    {
        while (1);
    }


    if (cas_latency == 3)
    {
        t_rtw = 3;
    }
    else if (cas_latency == 2)
    {
        t_rtw = 2;
    }
    else
    {
        while (1);
    }

    //set row_hit,clk_out,clk_sel,mode0_en,mode1_en,auto_ref_en,auto_allcs_en
    REG32 (EXT_MEM_DCFG0) = 0x0000FE00;

    //set data width mode
    if (data_width == DATA_WIDTH_32)
    {
        REG32 (EXT_MEM_DCFG0) = 0x0000FF00;
    }

    //set row mode
    REG32 (EXT_MEM_DCFG0) |= row_mode;/*lint !e737*/

    //set column mode
    REG32 (EXT_MEM_DCFG0) |= (col_mode<<4);/*lint !e737*/

    //set precharge bit
    REG32 (EXT_MEM_DCFG0) &= ~ (BIT_2|BIT_3); //A[10]

    #if (SDR_SDRAM == DRAM_TYPE)
    {
        //initialize dmem timing parameters
        t_ref = sdram_parameters->row_ref_max/ (1000/6); // t_ref*(1/6.5MHz)*8192 <= tREF
        t_rfc = sdram_parameters->rfc_min    /sdram_cycle_ns-1;
        t_rp  = sdram_parameters->row_pre_min/sdram_cycle_ns;
        t_rcd = sdram_parameters->rcd_min    /sdram_cycle_ns;
        t_rrd = 20/sdram_cycle_ns;//sdram_parameters->rrd_min    /sdram_cycle_ns;
        t_wr  = sdram_parameters->wr_min     /sdram_cycle_ns;
        t_xsr = sdram_parameters->xsr_min    /sdram_cycle_ns-1;
        t_ras = sdram_parameters->ras_min    /sdram_cycle_ns;
        t_wtr = 0;//sdram_parameters->wtr_min;
        t_mrd = sdram_parameters->mrd_min;

        //set t_rfc and t_ref
        REG32 (EXT_MEM_DCFG0) |= ( (t_rfc<<16) | (t_ref<<20)); /*lint !e737*/

        //set other timing parameters
        REG32 (EXT_MEM_DCFG1) = ( (t_rp <<0) |
                                  (t_rcd<<2) |
                                  (t_rrd<<4) |
                                  (t_wr <<8) |
                                  (t_xsr<<12) |
                                  (t_ras<<16) |
                                  (t_rtw<<20) |
                                  (t_wtr<<24) |
                                  (t_rtr<<28) |
                                  (t_mrd<<30)
                                );

        //read data and write data timing
        if (cas_latency == 3)
        {
            REG32 (EXT_MEM_DCFG4) = 0x00600208;
            REG32 (EXT_MEM_DCFG7) = 0x00400001;
        }
        else if (cas_latency == 2)
        {
            REG32 (EXT_MEM_DCFG4) = 0x00400006;
            REG32 (EXT_MEM_DCFG7) = 0x00100001;
        }
        else
        {
            while (1);
        }
    }
    #elif (DDR_SDRAM == DRAM_TYPE)
    {
        t_ras = sdram_parameters->ras_min/sdram_cycle_ns+1;//sdram_parameters[T_RAS_MIN]/sdram_cycle_ns ;
        t_xsr = sdram_parameters->xsr_min/sdram_cycle_ns+1;
        t_rfc = sdram_parameters->rfc_min/sdram_cycle_ns+1;
        t_wr  = sdram_parameters->wr_min/sdram_cycle_ns+1+2; //note: twr should add 2 for ddr
        t_rcd = (sdram_parameters->rcd_min/sdram_cycle_ns+1)>3 ? 3:(sdram_parameters->rcd_min/sdram_cycle_ns+1);
        t_rp  = (sdram_parameters->row_pre_min/sdram_cycle_ns+1)>3 ? 3:(sdram_parameters->row_pre_min/sdram_cycle_ns+1);
        t_rrd = (sdram_parameters->rrd_min/sdram_cycle_ns+1) >3 ? 3:(sdram_parameters->rrd_min/sdram_cycle_ns+1);
        t_mrd = (sdram_parameters->mrd_min+1) > 3 ? 3:(sdram_parameters->mrd_min+1);
        t_wtr = sdram_parameters->wtr_min+1;
        t_ref = sdram_parameters->row_ref_max*13/2*1000/(1<<row_number) - 1; // t_ref*(1/6.5MHz)*8192 <= tREF

        //set t_rfc and t_ref
        REG32 (EXT_MEM_DCFG0) |= ( (t_rfc<<16) | (t_ref<<20)); /*lint !e737*/

        REG32(EXT_MEM_DCFG1) = (    (t_rp <<0) |        \
                                    (t_rcd<<2) |        \
                                    (t_rrd<<4) |        \
                                    (t_wr <<8) |        \
                                    (t_xsr<<12)|        \
                                    (t_ras<<16)|        \
                                    (t_rtw<<20)|        \
                                    (t_wtr<<24)|        \
                                    (t_mrd<<30)         \
                                );
        
        //read data and write data timing
        if (cas_latency == 3)
        {
            REG32 (EXT_MEM_DCFG4) = 0x00622729;
            REG32 (EXT_MEM_DCFG5) = 0x00200010;
            REG32 (EXT_MEM_DCFG6) = 0x00F0000E;
            REG32 (EXT_MEM_DCFG7) = 0x00F0000E;
        }
        else if (cas_latency == 2)
        {
            REG32 (EXT_MEM_DCFG4) = 0x00422726;
            REG32 (EXT_MEM_DCFG5) = 0x00080004;
            REG32 (EXT_MEM_DCFG6) = 0x003C000E;
            REG32 (EXT_MEM_DCFG7) = 0x003C000E;
        }
        else
        {
            while (1);
        }

        //config delay lines.
        REG32 (EXT_MEM_DL0)  = 0;
        REG32 (EXT_MEM_DL1)  = 0;
        REG32 (EXT_MEM_DL2)  = 0;
        REG32 (EXT_MEM_DL3)  = 0;
        REG32 (EXT_MEM_DL4)  = 0;
        REG32 (EXT_MEM_DL5)  = 0;
        REG32 (EXT_MEM_DL6)  = 0;
        REG32 (EXT_MEM_DL7)  = 0x6;
        REG32 (EXT_MEM_DL16) = 0x4;
        REG32 (EXT_MEM_DL17) = 0x4;
        REG32 (EXT_MEM_DL18) = 0x4;
        REG32 (EXT_MEM_DL19) = 0x4;
        REG32 (EXT_MEM_DL20) = 0x4;
        REG32 (EXT_MEM_DL21) = 0x4;
        REG32 (EXT_MEM_DL22) = 0x4;
        REG32 (EXT_MEM_DL23) = 0x4;
        REG32 (EXT_MEM_DL24) = 0x6;
        REG32 (EXT_MEM_DL25) = 0x6;
        REG32 (EXT_MEM_DL26) = 0x6;
        REG32 (EXT_MEM_DL27) = 0x6;
    }
    #else
    {
        #error sdram type err
    }
    #endif

    return;
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SDRAM_Device_Init(SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)            *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     set emc dmem mode/ext mode register parameters                        *
 **                                                                           *
 ** INPUT                                                                     *
 **     sdram_cfg_ptr                                                         *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *
**---------------------------------------------------------------------------*/
void SDRAM_Device_Init (SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)
{
    uint8 mode_reg_bl = 0xFF;
    uint8 mode_reg_bt = MODE_REG_BT_SEQ;
    uint8 mode_reg_cl = 0xFF;
    uint8 mode_reg_opmode = MODE_REG_OPMODE;
    //    uint8 mode_reg_wb = MODE_REG_WB_PRORAM;  //Programming burst length for write.
    uint32 ex_mode_reg = 0;
    uint16 mode_reg = 0;

    //calculate mode reg burst length
    switch (sdram_cfg_ptr->burst_length)
    {
        case BURST_LEN_1:
            mode_reg_bl = MODE_REG_BL_1;
            break;
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
            mode_reg_bl = MODE_REG_BL_1;
            break;
    }

    //calculate mode reg cas latency
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

    //get mode reg parameter
    mode_reg = ( (mode_reg_opmode<<9) | (mode_reg_cl<<4) | (mode_reg_bt<<3) | mode_reg_bl);

    //get ext-mode reg parameter
    ex_mode_reg = sdram_cfg_ptr->ext_mode_val;

    // Precharge all banks.
    REG32 (EXT_MEM_DCFG3) = 0x40010000;

    while ( (REG32 (EXT_MEM_DCFG3)) & BIT_16);

    //Auto_ref
    REG32 (EXT_MEM_DCFG3) = 0x40020000;

    while ( (REG32 (EXT_MEM_DCFG3)) & BIT_17);

    //Auto_ref
    REG32 (EXT_MEM_DCFG3) = 0x40020000;

    while ( (REG32 (EXT_MEM_DCFG3)) & BIT_17);

    //mode register load.
    REG32 (EXT_MEM_DCFG3) &= ~ (0xFFFF);
    REG32 (EXT_MEM_DCFG3) |= (mode_reg | 0x40040000);

    while ( (REG32 (EXT_MEM_DCFG3)) & BIT_18);

    //extended mode register load.
    if (ex_mode_reg != SDRAM_EXT_MODE_INVALID)
    {
        REG32 (EXT_MEM_DCFG3) &= ~ (0xFFFF);
        REG32 (EXT_MEM_DCFG3) |= (ex_mode_reg | 0x40040000);/*lint !e737*/

        while ( (REG32 (EXT_MEM_DCFG3)) & BIT_18);
    }
}
#endif


/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SDRAM_Init(uint32 sdram_clk)                                     *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     initialize emc sdram module                                           *
 **                                                                           *
 ** INPUT                                                                     *
 **     sdram_clk                                                             *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *
**---------------------------------------------------------------------------*/
void SDRAM_Init (uint32 clk)
{
    SDRAM_CFG_INFO_T_PTR pCfg = NULL;

    Userdef_before_sdram_init();

#if defined(PLATFORM_SC8800H)
    clk = clk/1000000;
#endif
    pCfg = SDRAM_GetCfg();

    //step 1. Disable auto refresh.
    REG32 (EXT_MEM_DCFG0) &= ~ (DCFG0_AUTOREF_EN);

    //step 2. sdram init: config registers, precharege all banks, auto ref for 2 times, load mode register.
    SDRAM_GenMemCtlCfg (pCfg);
    SDRAM_DMemCtlCfg (clk,pCfg);
    SDRAM_Device_Init (pCfg);

    //step3. enable auto refresh.
    REG32 (EXT_MEM_DCFG2) |= DCFG2_REF_CNT_RST; //clear refresh count.
    REG32 (EXT_MEM_DCFG0) |= (DCFG0_AUTOREF_EN); //Enable auto refresh.

    Userdef_after_sdram_init();

}


/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SDRAM_PinDrv_Set(void)                                           *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     initialize emc pins                                                   *
 **                                                                           *
 ** INPUT                                                                     *
 **     none                                                                  *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *
**---------------------------------------------------------------------------*/
void SDRAM_PinDrv_Set (void)
{
    uint32 i;
#if defined(PLATFORM_SC8800H)

    for (i = 0x8C0000EC; i <= 0x8C0001F0; i+=4)
    {
        REG32 (i) = 0x31;       //sdram_mode
    }

    // If use SDRAM/DDR, EMBA[1]'S default function is NOR-FLASH'S reset£¬so we should change it
    REG32 (0x8C0001A0) |= BIT_6;

#endif
#if defined(PLATFORM_SC8800G)
    * (volatile uint32 *) PIN_CTL_REG = 0X1FFF00; //set nf_rb keyin[0-7] wpus
#endif
}


/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void Chip_ConfigClk(SYS_CLK_CFG_INFO* p_system_clk_cfg)               *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     set pll, arm clock,ahb clock,emc clock                                *
 **                                                                           *
 ** INPUT                                                                     *
 **     p_system_clk_cfg                                                      *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *
**---------------------------------------------------------------------------*/
uint32 Chip_ConfigClk (SYS_CLK_CFG_INFO *p_system_clk_cfg)
{
    volatile uint32 i,ext_clk,arm_ahb_clk;
    ext_clk=* (volatile unsigned long *) GR_GEN1;

#if defined (PLATFORM_SC8800H)

    if ( (ext_clk & (0x1<<15)) == 0x0)  //The external crystal is 13mhz
    {
        switch (p_system_clk_cfg->pll_mn)
        {

            default:
                BL_TRACE0 ("error1 r\n");

                for (;;)
                    ;
        }
    }
    else                                //The external crystal is 26mhz
    {
        switch (p_system_clk_cfg->pll_mn)
        {

            case MPLL_MN_160M_EX26M:
                arm_ahb_clk = ARM_CLK_80M;
                break;
            case MPLL_MN_164M_EX26M:
                arm_ahb_clk = 82000000;
                break;
            default:
                BL_TRACE0 ("error1 \r\n");

                while (1);
        }


        //enable pll_mn
        * (volatile uint32 *) GR_GEN1 |= (0x1<<9);
        //Set pll clk
        * (volatile uint32 *) GR_MPLL_MN = p_system_clk_cfg->pll_mn;
        //Disable pll_mn
        * (volatile uint32 *) GR_GEN1 &= ~ (0x1<<9);

        //Delay some time
        for (i=0; i<100; i++);

        //set arm,ahb,emc clock
        * (volatile uint32 *) AHB_AHB_ARM_CLK = p_system_clk_cfg->dsp_arm_div;
    }

#elif defined(PLATFORM_SC6800H)
#ifdef _BL_NF_NBL_

    //support dual pll, mpll 400mhz, bpll 480mhz
    {
        volatile uint32 is_pll_done = 0;

        //clear the protect control
        * (volatile uint32 *) (GR_MCU_PORT) = PLL_MCU_PROT_VALUE;

        //disable pll
        * (volatile uint32 *) (GR_MISC1) &= ~MISC1_MCU_PLL_EN;

        //set mpll to 400MHz
        * (volatile uint32 *) (GR_MPLL_MN) = MPLL_MN_400M;
        //clk_48M source select to bpll
        * (volatile uint32 *) (AHB_CLK_CFG1) |= AHB_CLK_CFG1_SRCSEL48M;

        //open bpll
        * (volatile uint32 *) (GR_MISC1) &= ~MISC1_MCU_BPLL_FORCE_PD_EN;
        //select bpll controlled by hw and release bpll output
        * (volatile uint32 *) (GR_MISC1) |= (MISC1_BPLL_SEL|MISC1_BPLL_CONT_DONE);

        //enable pll
        * (volatile uint32 *) (GR_MISC1) |= MISC1_MCU_PLL_EN;

        //wait pll count done
        is_pll_done = (* (volatile uint32 *) (GR_STATUS)) & APB_STATUS_PLL_CNT_DONE;

        while (0 == is_pll_done)
        {
            is_pll_done = (* (volatile uint32 *) (GR_STATUS)) & APB_STATUS_PLL_CNT_DONE;
        }

        //set the protect control
        * (volatile uint32 *) (GR_MCU_PORT) = 0;
    }
#endif

    arm_ahb_clk = ARM_CLK_100M;
    //MPLL is open already in RomCode
    i = REG32 (AHB_CLK_CFG0);
    //set CLK_EMC to 192MHz
    i &= ~ (BIT_7 | BIT_6 | BIT_5 | BIT_4);
    i |= 0xC << 4;               //Src_sel_emc:4'b1100 : MPLL_DIV2:
    i &= ~ (BIT_9 | BIT_8);      //Div_cfg_emc:0x0
    //set CLK_ARM to 192MHz
    i &= ~ (BIT_2 | BIT_1);      //Div_cfg_arm:0x01
    //set CLK_AHB to 96MHz
    i |= BIT_3;
    //set Src_sel_arm to  MPLL/2(192Mhz)
    i |= BIT_0;
    REG32 (AHB_CLK_CFG0) = i;

    for (i=0; i<100; i++);

#elif defined(PLATFORM_SC8800G)
    #ifdef CHIP_VER_8800G1
    arm_ahb_clk = (* (volatile uint32 *) (0x20900224));
    arm_ahb_clk &=  ~ (3 << 23);
    arm_ahb_clk |= (1 << 23);  //CLK_MCU_SEL set 192M

    arm_ahb_clk &=  ~ (3 << 12);
    arm_ahb_clk |= (1 << 12); //CLK_EMC_SEL set 192M

    arm_ahb_clk &=  ~ (0x7 << 14); //CLK_EMC_DIV set 0
    //arm_ahb_clk |= (1 << 14); //96 M

    arm_ahb_clk &=  ~ (7 << 4);
    arm_ahb_clk |= (1 << 4); //CLK_AHB_DIV set 1

    arm_ahb_clk &=  ~ (7 << 0); //CLK_ARM_DIV set 0

    (* (volatile uint32 *) (0x20900224)) = arm_ahb_clk;
    arm_ahb_clk = ARM_CLK_96M;
    
    #elif defined(CHIP_VER_8800G2)
    __ClkConfig(&g_emc_clk, &arm_ahb_clk);
    #endif
#endif

    //Delay some time
    for (i=0; i<1000; i++);

    return arm_ahb_clk;
}


/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void Chip_Init(void)                                                  *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     initialize chip setting                                               *
 **                                                                           *
 ** INPUT                                                                     *
 **     none                                                                  *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *
**---------------------------------------------------------------------------*/
void Chip_Init (void)
{
    uint32 ahb_clk;
    uint32 i = 0;
    SYS_CLK_CFG_INFO *pSysClkCfg = NULL;

    //step1, SDRAM pin set up
    SDRAM_PinDrv_Set();

    //step2, config AHB CLK and PLL clk
    pSysClkCfg = Get_system_clk_cfg();
    g_ahb_clk = Chip_ConfigClk (pSysClkCfg);

    //step3, initialize SDRAM init
    #ifdef CHIP_VER_8800G2
    SDRAM_Init (g_emc_clk/2);
    #else
    SDRAM_Init (g_ahb_clk);
    #endif

    for (i=0; i<5000; i++);
}
