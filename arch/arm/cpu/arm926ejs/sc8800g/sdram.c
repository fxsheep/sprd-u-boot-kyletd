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

#if defined(PLATFORM_SC8800G)
uint32 g_emc_clk;
uint32 s_colum;
#endif //defined(PLATFORM_SC8800G) && defined(CHIP_VER_8800G2)

uint32 g_ahb_clk; 
SDRAM_TIMING_PARA_T_PTR sdram_parameters;

#ifdef PLATFORM_SC8800G
#define SDRAM_AUTODETECT_SUPPORT
#endif
#define _BL_NF_NBL_

#ifdef SDRAM_AUTODETECT_SUPPORT
#define MEM_REF_DATA0       0x12345678
#define MEM_REF_DATA1       0x55AA9889
#ifdef _BL_NF_NBL_
#define ZERO_ADDR           0x00000000UL
#else
#define ZERO_ADDR           0x30000000UL
#endif
#define STATE_SDRAM_TYPE    0UL
#define STATE_BIT_WIDTH     1UL
#define STATE_COLUM         2UL
#define STATE_ROW           3UL
#define STATE_REINIT        4UL
#define STATE_END           5UL

#define BYTE_OFFSET         3UL   // 1BYTE = 8BIT = 2^3
#define WIDTH16_OFFSET      4UL   // 16BIT = 2^4
#define WIDTH32_OFFSET      5UL   // 32BIT = 2^5
#define BANK_OFFSET         2UL   // 4BANK = 2^2

#define SDRAM_MIN_COLUM     8UL
#define SDRAM_MAX_COLUM     12UL
#define SDRAM_MIN_ROW       11UL
#define SDRAM_MAX_ROW       14UL

SDRAM_CFG_INFO_T s_sdram_raw_cfg;
#endif

#ifdef PLATFORM_SC8800G
#define INTERFACE_CLK_MAX   ARM_CLK_200M
typedef struct ARM_EMC_AHB_CLK_TAG 
{
    uint32 mcu_clk;
    uint32 arm_clk;
    uint32 emc_clk;
    uint32 ahb_clk;
} ARM_EMC_AHB_CLK_T;

typedef enum MCU_CLK_TYPE_TAG
{
    ARM400_EMC200_AHB100 = 0,   //SC8800G2
    ARM256_EMC200_AHB64,        //SC8801G2
    ARM192_EMC200_AHB96,        //SC8802G2
    ARM256_EMC192_AHB64,
    ARM256_EMC128_AHB64,
    ARM192_EMC192_AHB96,

    MCU_CLK_TYPE_MAX   
} MCU_CLK_TYPE_E;

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

//unsigned short ADI_Analogdie_reg_read (unsigned int addr);

//void ADI_Analogdie_reg_write (unsigned int addr, unsigned short data);

uint32 __GetMPllClk (void)
{
    return ( ARM_CLK_26M
               / ( (REG32(GR_MPLL_MN) & 0x003F0000) >>16)
               * (REG32(GR_MPLL_MN) & 0x00000FFF) );
}

LOCAL void __SetMPllClk (uint32 clk)
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

LOCAL void __ClkConfig(uint32 *emcclk, uint32 *ahbclk)
{
    uint32 tmp_clk, mcuclk, armclk;
    uint32 mcu_div, if_div;
    BOOLEAN is_mpll, is_async;
    MCU_CLK_TYPE_E clk_type = ARM400_EMC200_AHB100;

    ///*
    switch(REG32(GR_GEN3) & 0x3)
    {
    	case 0:
    		clk_type = ARM400_EMC200_AHB100; // 8800G2
    		break;
    	case 1:
    		clk_type = ARM256_EMC200_AHB64;  // 8801G1
    		break;
    	case 2:
    		clk_type = ARM192_EMC200_AHB96;  // 8802G1 
    		break;
    	default:
    		clk_type = 	ARM400_EMC200_AHB100;
    		break;
    }           
    //*/         
    
    mcuclk   = s_arm_emc_ahb_clk[clk_type].mcu_clk;
    armclk   = s_arm_emc_ahb_clk[clk_type].arm_clk;
    *emcclk  = s_arm_emc_ahb_clk[clk_type].emc_clk;
    *ahbclk  = s_arm_emc_ahb_clk[clk_type].ahb_clk;
    
    is_mpll  = (((0 == (REG32(GR_GEN3) & 0x3)) || (3 == (REG32(GR_GEN3) & 0x3))) ? (SCI_TRUE) : (SCI_FALSE));
    is_async = ((0 == (armclk%(*emcclk)))     ? (SCI_FALSE) : (SCI_TRUE));  

    mcu_div = 0;    
    if(is_mpll)
    {
        if(__GetMPllClk() != mcuclk)
        {
            __SetMPllClk(mcuclk);
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
LOCAL void SDRAM_GenMemCtlCfg (SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)
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
        for (;;){}
    }
    REG32 (EXT_MEM_CFG0) = (BIT_0|BIT_1|BIT_3|BIT_6|BIT_8|BIT_9|BIT_10|BIT_11); //software address mapping

#ifdef CHIP_ENDIAN_BIG
    REG32 (EXT_MEM_CFG0) |= BIT_12; /*lint !e718*/ //big endian
#endif

    //set dmem parameter
    /*lint -save -e506 -e774*/
    if ( (SDR_SDRAM == sdram_cfg_ptr->sdram_type))
    {
        REG32 (EXT_MEM_CFG1) = 0x00000008; //EMC phy set
    }
    else if (DDR_SDRAM == sdram_cfg_ptr->sdram_type)
    {
        REG32 (EXT_MEM_CFG0) |= (BIT_5);   // ddr select
        REG32 (EXT_MEM_CFG1) = 0x01080000; //EMC phy set
    }
    else
    {
        for (;;){}
    }

    REG32 (EXT_MEM_CFG0_CS0) = ( (0x1<<12) | (dburst_wlength<<8) | (dburst_rlength<<4) | (0x3)); 

    //config channel 0-15
    REG32 (EXT_MEM_CFG0_CH0)  = 0x0003c31c;//0x0001c31c;
    REG32 (EXT_MEM_CFG0_CH1)  = 0x0003c31c;
    REG32 (EXT_MEM_CFG0_CH2)  = 0x0003c31c;
    REG32 (EXT_MEM_CFG0_CH3)  = 0x0003c31c;
    REG32 (EXT_MEM_CFG0_CH4)  = 0x0003c31c;
    REG32 (EXT_MEM_CFG0_CH5)  = 0x0003c31c;

#if defined(PLATFORM_SC8800G)
    REG32 (EXT_MEM_CFG0_CH6)  = 0x0001c31c; //for sc8800g emc sleep bug.
    REG32 (EXT_MEM_CFG0_CH7)  = 0x0001c31c;
    REG32 (EXT_MEM_CFG0_CH8)  = 0x0001c31c;
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
LOCAL void SDRAM_DMemCtlCfg (uint32 sdram_clk,SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)
{
    //initialize dmem mode parameters
    uint32 row_mode       = sdram_cfg_ptr->row_mode;
    uint32 col_mode       = sdram_cfg_ptr->col_mode;
    uint32 data_width     = sdram_cfg_ptr->data_width;
    uint32 cas_latency    = sdram_cfg_ptr->cas_latency;
    uint32 sdram_cycle_ns = 1000/ (sdram_clk/1000000);
    uint32 row_num;
    
    uint32 t_ras = (sdram_parameters->ras_min/sdram_cycle_ns+1)>0xF ? 0xF : (sdram_parameters->ras_min/sdram_cycle_ns+1);
    uint32 t_xsr = (sdram_parameters->xsr_min/sdram_cycle_ns)>0xF ? 0xF : (sdram_parameters->xsr_min/sdram_cycle_ns);
    uint32 t_rfc = (sdram_parameters->rfc_min/sdram_cycle_ns)>0xF ? 0xF : (sdram_parameters->rfc_min/sdram_cycle_ns);
    uint32 t_wr  = sdram_parameters->wr_min/sdram_cycle_ns+2; //note: twr should add 2 for ddr
    uint32 t_rcd = (sdram_parameters->rcd_min/sdram_cycle_ns)>3 ? 3:(sdram_parameters->rcd_min/sdram_cycle_ns);
    uint32 t_rp  = (sdram_parameters->row_pre_min/sdram_cycle_ns)>3 ? 3:(sdram_parameters->row_pre_min/sdram_cycle_ns);
    uint32 t_rrd = (sdram_parameters->rrd_min/sdram_cycle_ns) >3 ? 3:(sdram_parameters->rrd_min/sdram_cycle_ns);
    uint32 t_mrd = (sdram_parameters->mrd_min) > 3 ? 3:(sdram_parameters->mrd_min);
    uint32 t_wtr = sdram_parameters->wtr_min;
    uint32 t_ref;
    uint32 t_rtw = sdram_cfg_ptr->cas_latency;

    switch(row_mode)
    {
    case ROW_MODE_14:
        row_num = 14;
        break;
    case ROW_MODE_13:
        row_num = 13;
        break;
    case ROW_MODE_12:
        row_num = 12;
        break;
    case ROW_MODE_11:
        row_num = 11;
        break;
    default:
        for (;;){}
    }
    
    t_ref = ((sdram_parameters->row_ref_max*6000)>>row_num) - 1; // t_ref*(1/6.5MHz)*2^row <= tREF

    //set data width mode
    if (data_width == DATA_WIDTH_32)
    {
        REG32 (EXT_MEM_DCFG0) = 0x0000BF00;
    }
    else
    {
        REG32 (EXT_MEM_DCFG0) = 0x0000BE00;
    }
        
    //set row mode
    REG32 (EXT_MEM_DCFG0) |= row_mode;/*lint !e737*/

    //set column mode
    REG32 (EXT_MEM_DCFG0) |= (col_mode<<4);/*lint !e737*/

    //set precharge bit
    REG32 (EXT_MEM_DCFG0) &= ~ (BIT_2|BIT_3); //A[10]

    //set t_rfc and t_ref
    REG32 (EXT_MEM_DCFG0) |= ( (t_rfc<<16) | (t_ref<<20)); /*lint !e737*/
    
    REG32(EXT_MEM_DCFG1) = (    (t_rp <<0) |       
                                (t_rcd<<2) |        
                                (t_rrd<<4) |        
                                (t_wr <<8) |        
                                (t_xsr<<12)|        
                                (t_ras<<16)|        
                                (t_rtw<<20)|
                                (t_mrd<<30)         
                            );
    
    if (SDR_SDRAM == sdram_cfg_ptr->sdram_type)
    {
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
            for (;;){}
        }
        
        //config delay lines.
        REG32 (EXT_MEM_DL24) = 0x2;
        REG32 (EXT_MEM_DL25) = 0x2;
        REG32 (EXT_MEM_DL26) = 0x2;
        REG32 (EXT_MEM_DL27) = 0x2;
    }
    else if (DDR_SDRAM == sdram_cfg_ptr->sdram_type)
    {
        // wtr is only for ddr
        REG32(EXT_MEM_DCFG1) |= (t_wtr<<24);
        
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
            for (;;){}
        }

        //config delay lines.
        REG32 (EXT_MEM_DL7)  = 0x6;
        REG32 (EXT_MEM_DL16) = 0x3;
        REG32 (EXT_MEM_DL17) = 0x3;
        REG32 (EXT_MEM_DL18) = 0x3;
        REG32 (EXT_MEM_DL19) = 0x3;
        REG32 (EXT_MEM_DL20) = 0x3;
        REG32 (EXT_MEM_DL21) = 0x3;
        REG32 (EXT_MEM_DL22) = 0x3;
        REG32 (EXT_MEM_DL23) = 0x3;
        REG32 (EXT_MEM_DL24) = 0x7;
        REG32 (EXT_MEM_DL25) = 0x7;
        REG32 (EXT_MEM_DL26) = 0x7;
        REG32 (EXT_MEM_DL27) = 0x7;
    }
    else
    {
        for (;;){}
    }

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
LOCAL void SDRAM_Device_Init (SDRAM_CFG_INFO_T_PTR sdram_cfg_ptr)
{
    uint8 mode_reg_bl = 0xFF;
    uint8 mode_reg_bt = MODE_REG_BT_SEQ;
    uint8 mode_reg_cl = 0xFF;
    uint8 mode_reg_opmode = MODE_REG_OPMODE;
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

    while ( (REG32 (EXT_MEM_DCFG3)) & BIT_16){}

    //Auto_ref
    REG32 (EXT_MEM_DCFG3) = 0x40020000;

    while ( (REG32 (EXT_MEM_DCFG3)) & BIT_17){}

    //Auto_ref
    REG32 (EXT_MEM_DCFG3) = 0x40020000;

    while ( (REG32 (EXT_MEM_DCFG3)) & BIT_17){}

    //mode register load.
    REG32 (EXT_MEM_DCFG3) &= ~ (0xFFFF);
    REG32 (EXT_MEM_DCFG3) |= (mode_reg | 0x40040000);

    while ( (REG32 (EXT_MEM_DCFG3)) & BIT_18){}

    //extended mode register load.
    if (ex_mode_reg != SDRAM_EXT_MODE_INVALID)
    {
        REG32 (EXT_MEM_DCFG3) &= ~ (0xFFFF);
        REG32 (EXT_MEM_DCFG3) |= (ex_mode_reg | 0x40040000);/*lint !e737*/

        while ( (REG32 (EXT_MEM_DCFG3)) & BIT_18){}
    }
    
}

LOCAL void __sdram_set_param(uint32 clk, SDRAM_CFG_INFO_T_PTR pCfg)
{
    volatile uint32 i;
    
    //step 1. Disable auto refresh.
    REG32 (EXT_MEM_DCFG0) &= ~ BIT_14;

    //step 2. sdram init: config registers, precharege all banks, auto ref for 2 times, load mode register.
    SDRAM_GenMemCtlCfg (pCfg);
    SDRAM_DMemCtlCfg (clk,pCfg);
    SDRAM_Device_Init (pCfg);

    //step3. enable auto refresh.
    REG32 (EXT_MEM_DCFG2) |= BIT_15; //clear refresh count.
    REG32 (EXT_MEM_DCFG0) |= BIT_14; //Enable auto refresh.
   
    for (i=0; i<1000; i++){}
}
#ifdef SDRAM_AUTODETECT_SUPPORT
LOCAL uint32 __colum_to_mode(uint32 colum)
{
    uint32 col_mode;
    switch(colum)
    {
        case 8:
            col_mode = COL_MODE_8;
            break;
        case 9:
            col_mode = COL_MODE_9;
            break;
        case 10:
            col_mode = COL_MODE_10;
            break;
        case 11:
            col_mode = COL_MODE_11;
            break;
        case 12:
            col_mode = COL_MODE_12;
            break;
        default:
            for( ; ; ) {}
            //break;          
    }
    return col_mode;
}
LOCAL uint32 __row_to_mode(uint32 row)
{
    uint32 row_mode;
    switch(row)
    {
        case 11:
            row_mode = ROW_MODE_11;
            break;
        case 12:
            row_mode = ROW_MODE_12;
            break;
        case 13:
            row_mode = ROW_MODE_13;
            break;
        case 14:
            row_mode = ROW_MODE_14;
            break;
        default:
            for( ; ; ) {}
            //break;          
    }
    return row_mode;
}

LOCAL BOOLEAN __is_rw_ok(uint32 addr, uint32 val)
{
    volatile uint32 i;
    
    REG32(addr) = val;
    REG32(addr+4) = (~val);
    
    for(i=0;i<0xff;i++) {}

    if((REG32(addr) == val) && (REG32(addr+4) == (~val)))
        return ((BOOLEAN)SCI_TRUE);
    else
        return ((BOOLEAN)SCI_FALSE);
    //return ((val == REG32(addr))? ((BOOLEAN)SCI_TRUE) : ((BOOLEAN)SCI_FALSE));
}

// ADDR: |-BANK -|------------ COLUM 12 ---------------|
//       |A13|A12|A11|A10|A9|A8|A7|A6|A5|A4|A3|A2|A1|A0|
//
//       |------------------- ROW 14-------------------|   
//       |A13|A12|A11|A10|A9|A8|A7|A6|A5|A4|A3|A2|A1|A0|
LOCAL uint32 __col_row_detect(BOOLEAN is_col, SDRAM_CFG_INFO_T_PTR pCfg)
{
    uint32 num, max, min;
    uint32 offset, addr;
    uint32 width_offset = (DATA_WIDTH_16 == pCfg->data_width)?(WIDTH16_OFFSET):(WIDTH32_OFFSET);

    if(is_col)
    {
        max = SDRAM_MAX_COLUM;
        min = SDRAM_MIN_COLUM;
    }
    else
    {
        max = SDRAM_MAX_ROW;
        min = SDRAM_MIN_ROW;
    }
    for(num = max; num >= min; num--)
    {
        // init 0 address
        REG32(ZERO_ADDR) = MEM_REF_DATA0;               /*lint !e413*/

        if(is_col)
        {
            offset = num + width_offset - BYTE_OFFSET;    // colum+width-byteoff
        }
        else
        {
            offset = num + BANK_OFFSET + (uint32)s_colum +  width_offset -  BYTE_OFFSET;    // row+bank+colum+width-byteoff
        }
        addr   = (1 << (offset - 1)) + ZERO_ADDR;
        if(__is_rw_ok(addr, MEM_REF_DATA1))
        {
            // if 0addr not changed, row is ok
            if(MEM_REF_DATA0 == REG32(ZERO_ADDR))       /*lint !e413*/
            {
                break;
            }
            // 0addr changed, row is too big
            else if(MEM_REF_DATA1 == REG32(ZERO_ADDR))  /*lint !e413*/
            {
                continue;
            }
        }
    }

    return (num); 
}

LOCAL void __sdram_detect(uint32 clk)
{
    SDRAM_CFG_INFO_T_PTR pCfg = &s_sdram_raw_cfg;
    uint32 state = STATE_SDRAM_TYPE;
    uint32 colum, row;

    pCfg->bank_mode    = BK_MODE_4;
    pCfg->row_mode     = ROW_MODE_14;
    pCfg->col_mode     = COL_MODE_12;
    pCfg->data_width   = DATA_WIDTH_16;
    pCfg->burst_length = BURST_LEN_2;
    pCfg->cas_latency  = CAS_LATENCY_3;
    pCfg->ext_mode_val = SDRAM_EXT_MODE_REG;
    pCfg->sdram_type   = SDR_SDRAM;
        
    while(STATE_END != state)
    {
        __sdram_set_param(clk, pCfg);
        switch(state)
        {
        case STATE_SDRAM_TYPE:
            if(__is_rw_ok(ZERO_ADDR, MEM_REF_DATA0))           // 16bit sdr/ddr detect ok, try 32bit
            {
                pCfg->data_width = DATA_WIDTH_32;
                state = STATE_BIT_WIDTH;
                //break;
            }
            else                        // 16bit sdr failed, try 16bit ddr
            {
                if(DDR_SDRAM == pCfg->sdram_type)   
                {
                    for( ; ; ) {}          // all failed
                }
                pCfg->sdram_type = DDR_SDRAM;
                state = STATE_SDRAM_TYPE;
                //break;
            }
            break;
        case STATE_BIT_WIDTH:
            if(__is_rw_ok(ZERO_ADDR, MEM_REF_DATA0))           // 32bit sdr/ddr detect ok, try colum
            {
                state = STATE_COLUM;
                //break;
            }
            else                        // 32bit sdr/ddr detect failed, fix 16bit and try colum
            {
                pCfg->data_width = DATA_WIDTH_16;
                state = STATE_COLUM;
                //break;
            }
            break;
        case STATE_COLUM:
            if(__is_rw_ok(ZERO_ADDR, MEM_REF_DATA0))           
            {
                colum = __col_row_detect(SCI_TRUE, pCfg);
                pCfg->col_mode = __colum_to_mode(colum);
                s_colum = colum;//lint !e63
                state = STATE_ROW;
                //break;
            }
            else                        
            {
                for( ; ; ) {}           // should not come here
            }
            break;
        case STATE_ROW:
            row = __col_row_detect(SCI_FALSE, pCfg);
            pCfg->row_mode = __row_to_mode(row);
            state = STATE_REINIT;
            break;
        case STATE_REINIT:
            state = STATE_END;  
            break;
        default:
            break;
        }
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
LOCAL void SDRAM_Init (uint32 clk)
{
    sdram_parameters = SDRAM_GetTimingPara();

    #ifdef SDRAM_AUTODETECT_SUPPORT
    __sdram_detect(clk);
    #else
    __sdram_set_param(clk, SDRAM_GetCfg());
    #endif

#if 1
    //ANA_REG_MSK_OR(ANA_DCDC_CTL, 4 << 6, 7 << 6);
    ANA_REG_MSK_OR(ANA_DCDC_CTL, 0x1c << 6, 0x3f << 6);
    {
	 /* wait until voltage is stable */
        volatile int32 i;
	 for (i=0; i<3000; ++i)
	 {
	 }
    }		

    *(volatile uint32 *)(GREG_BASE + 0x18) |= BIT_9;
    {
        uint32 tmp = *(volatile uint32 *)(GREG_BASE + 0x24);
	 tmp &= ~0x00000fff;
	 //tmp |= 0xe1; // 450M
	 //tmp |= 0x12c; // 600M
	 tmp |= 0x113; // 550M
	 //tmp |= 0xfa; // 500M
	 //tmp |= 0x104; // 520M
	 //tmp |= 0x145; // 650M
	 *(volatile uint32 *)(GREG_BASE + 0x24) = tmp;
    }
    *(volatile uint32 *)(GREG_BASE + 0x18) &= ~BIT_9;	
#endif

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
LOCAL void SDRAM_PinDrv_Set (void)
{
#if defined(PLATFORM_SC8800G)
	uint32 i = 0;
	uint32 clk_drv = 0x200;
	uint32 ctl_drv = 0x100;
	uint32 dat_drv = 0x100;
	
	REG32(PIN_CTL_REG) = 0X1FFF00; //set nf_rb keyin[0-7] wpus

	REG32(PINMAP_REG_BASE + 0X27c) = clk_drv;	
	REG32(PINMAP_REG_BASE + 0X280) = clk_drv;		
	
	for(i = 0; i<15; i++)
	{
		REG32(PINMAP_REG_BASE + 0x019c + i*4) = ctl_drv;
	}
	
	REG32(PINMAP_REG_BASE + 0X1d8) = ctl_drv;
	REG32(PINMAP_REG_BASE + 0X2a8) = ctl_drv;				
	REG32(PINMAP_REG_BASE + 0X1FC) = ctl_drv;
	REG32(PINMAP_REG_BASE + 0X200) = ctl_drv;
	REG32(PINMAP_REG_BASE + 0X224) = ctl_drv;
	REG32(PINMAP_REG_BASE + 0X228) = ctl_drv;
	REG32(PINMAP_REG_BASE + 0X24c) = ctl_drv;	
	REG32(PINMAP_REG_BASE + 0X250) = ctl_drv;		
	REG32(PINMAP_REG_BASE + 0X274) = ctl_drv;		
	REG32(PINMAP_REG_BASE + 0X278) = ctl_drv;		

	for(i = 0; i<9; i++)
		REG32(PINMAP_REG_BASE + 0x0284 + i*4) = ctl_drv;
	

	for(i = 0; i<8; i++)
	{	
		REG32(PINMAP_REG_BASE + 0x1DC + i*4) = dat_drv;
		REG32(PINMAP_REG_BASE + 0x204 + i*4) = dat_drv;
		REG32(PINMAP_REG_BASE + 0x22c + i*4) = dat_drv;
		REG32(PINMAP_REG_BASE + 0x254 + i*4) = dat_drv;
	}	

#endif
}


/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void Chip_ConfigClk(void)                                             *
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
LOCAL uint32 Chip_ConfigClk (void)
{
    volatile uint32 i,arm_ahb_clk;

#if defined(PLATFORM_SC6800H)
#ifdef _BL_NF_NBL_

    //support dual pll, mpll 400mhz, bpll 480mhz
    {
        volatile uint32 is_pll_done = 0;

        //clear the protect control
        CHIP_REG_SET(GR_MCU_PORT, PLL_MCU_PROT_VALUE);

        //disable pll
        CHIP_REG_AND(GR_MISC1, ~ MISC1_MCU_PLL_EN);

        //set mpll to 400MHz
        CHIP_REG_SET(GR_MPLL_MN, MPLL_MN_400M);
        //clk_48M source select to bpll
        CHIP_REG_OR(AHB_CLK_CFG1, AHB_CLK_CFG1_SRCSEL48M);

        //open bpll
        CHIP_REG_AND(GR_MISC1, ~MISC1_MCU_BPLL_FORCE_PD_EN);
        //select bpll controlled by hw and release bpll output
        CHIP_REG_OR(GR_MISC1, (MISC1_BPLL_SEL|MISC1_BPLL_CONT_DONE));

        //enable pll
        CHIP_REG_OR (GR_MISC1, MISC1_MCU_PLL_EN);

        //wait pll count done
        is_pll_done = CHIP_REG_GET(GR_STATUS) & ((uint32) APB_STATUS_PLL_CNT_DONE);

        while (0 == is_pll_done)
        {
            is_pll_done = CHIP_REG_GET(GR_STATUS) & ((uint32) APB_STATUS_PLL_CNT_DONE);
        }

        //set the protect control
        CHIP_REG_SET(GR_MCU_PORT, 0);
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

    for (i=0; i<100; i++){}

#elif defined(PLATFORM_SC8800G)
    __ClkConfig(&g_emc_clk, (uint32*)&arm_ahb_clk);
#endif

    //Delay some time
    for (i=0; i<1000; i++) {}

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
PUBLIC void Chip_Init (void) /*lint !e765 "Chip_Init" is used by init.s entry.s*/
{
    volatile uint32 i = 0;

    //step1, SDRAM pin set up
    SDRAM_PinDrv_Set();

    //step2, config AHB CLK and PLL clk
    g_ahb_clk = Chip_ConfigClk();

    //step3, initialize SDRAM init
    #ifdef PLATFORM_SC8800G
    SDRAM_Init (g_emc_clk/2);
    #else
    SDRAM_Init (g_ahb_clk);
    #endif

    for (i=0; i<5000; i++) {}
}
