/******************************************************************************
 ** File Name:    emc_reg_v3.h                                           *
 ** Author:       haiyang.hu                                                  *
 ** DATE:         05/31/2009                                                  *
 ** Copyright:    2009 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/15/2007    Daniel.Ding     Create.                                     *
 ** 05/07/2010    Mingwei.zhang   Modify it for SC8800G.                      *
 ******************************************************************************/
#ifndef _EMC_REG_V3_H_
#define _EMC_REG_V3_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
#define BIT_RANGE(_bit_h,_bit_l)        ((_bit_h)-(_bit_l)+1)

/*----------external memory cotnrol registers----------*/

#define EXT_MEM_CFG0                    (EXT_MEM_CTL_BASE + 0x0000)
#define EXT_MEM_CFG1                    (EXT_MEM_CTL_BASE + 0x0004)
//temp for nor_fdl and driver_export and asm_cvt
#define EXT_MEM_CTL0                    (EXT_MEM_CTL_BASE + 0x0000)
#define EXT_MEM_CTL1                    (EXT_MEM_CTL_BASE + 0x0004)

#define EMC_REG_STS0_ADDR           (EXT_MEM_CTL_BASE + 0x0008)

#define EXT_MEM_CFG0_CS0               (EXT_MEM_CTL_BASE + 0x0010)
#define EXT_MEM_CFG0_CS1               (EXT_MEM_CTL_BASE + 0x0014)
#define EXT_MEM_CFG0_CS2               (EXT_MEM_CTL_BASE + 0x0018)
#define EXT_MEM_CFG0_CS3               (EXT_MEM_CTL_BASE + 0x001c)

#define EXT_MEM_INI                     (EXT_MEM_CTL_BASE + 0x0020)

#define EXT_MEM_CFG0_CH0               (EXT_MEM_CTL_BASE + 0x0030)
#define EXT_MEM_CFG0_CH1               (EXT_MEM_CTL_BASE + 0x0034)
#define EXT_MEM_CFG0_CH2               (EXT_MEM_CTL_BASE + 0x0038)
#define EXT_MEM_CFG0_CH3               (EXT_MEM_CTL_BASE + 0x003c)
#define EXT_MEM_CFG0_CH4               (EXT_MEM_CTL_BASE + 0x0040)
#define EXT_MEM_CFG0_CH5               (EXT_MEM_CTL_BASE + 0x0044)
#define EXT_MEM_CFG0_CH6               (EXT_MEM_CTL_BASE + 0x0048)
#define EXT_MEM_CFG0_CH7               (EXT_MEM_CTL_BASE + 0x004c)
#define EXT_MEM_CFG0_CH8               (EXT_MEM_CTL_BASE + 0x0050)
#define EXT_MEM_CFG0_CH9               (EXT_MEM_CTL_BASE + 0x0054)
#define EXT_MEM_CFG0_CH10              (EXT_MEM_CTL_BASE + 0x0058)
#define EXT_MEM_CFG0_CH11              (EXT_MEM_CTL_BASE + 0x005c)
#define EXT_MEM_CFG0_CH12              (EXT_MEM_CTL_BASE + 0x0060)
#define EXT_MEM_CFG0_CH13              (EXT_MEM_CTL_BASE + 0x0064)
#define EXT_MEM_CFG0_CH14              (EXT_MEM_CTL_BASE + 0x0068)
#define EXT_MEM_CFG0_CH15              (EXT_MEM_CTL_BASE + 0x006c)


#define EXT_MEM_STS2                    (EXT_MEM_CTL_BASE + 0x0078)
#define EXT_MEM_STS3                    (EXT_MEM_CTL_BASE + 0x007c)



#define EXT_MEM_DL0                     (EXT_MEM_CTL_BASE + 0x00c0)
#define EXT_MEM_DL1                     (EXT_MEM_CTL_BASE + 0x00c4)
#define EXT_MEM_DL2                     (EXT_MEM_CTL_BASE + 0x00c8)
#define EXT_MEM_DL3                     (EXT_MEM_CTL_BASE + 0x00cC)
#define EXT_MEM_DL4                     (EXT_MEM_CTL_BASE + 0x00d0)
#define EXT_MEM_DL5                     (EXT_MEM_CTL_BASE + 0x00d4)
#define EXT_MEM_DL6                     (EXT_MEM_CTL_BASE + 0x00d8)
#define EXT_MEM_DL7                     (EXT_MEM_CTL_BASE + 0x00dC)

#define EXT_MEM_DL16                    (EXT_MEM_CTL_BASE + 0x0100)
#define EXT_MEM_DL17                    (EXT_MEM_CTL_BASE + 0x0104)
#define EXT_MEM_DL18                    (EXT_MEM_CTL_BASE + 0x0108)
#define EXT_MEM_DL19                    (EXT_MEM_CTL_BASE + 0x010C)
#define EXT_MEM_DL20                    (EXT_MEM_CTL_BASE + 0x0110)
#define EXT_MEM_DL21                    (EXT_MEM_CTL_BASE + 0x0114)
#define EXT_MEM_DL22                    (EXT_MEM_CTL_BASE + 0x0118)
#define EXT_MEM_DL23                    (EXT_MEM_CTL_BASE + 0x011c)
#define EXT_MEM_DL24                    (EXT_MEM_CTL_BASE + 0x0120)
#define EXT_MEM_DL25                    (EXT_MEM_CTL_BASE + 0x0124)
#define EXT_MEM_DL26                    (EXT_MEM_CTL_BASE + 0x0128)
#define EXT_MEM_DL27                    (EXT_MEM_CTL_BASE + 0x012c)



#define EXT_MEM_DCFG0                   (EXT_MEM_CTL_BASE + 0x0140)
#define EXT_MEM_DCFG1                   (EXT_MEM_CTL_BASE + 0x0144)
#define EXT_MEM_DCFG2                   (EXT_MEM_CTL_BASE + 0x0148)
#define EXT_MEM_DCFG3                   (EXT_MEM_CTL_BASE + 0x014c)
#define EXT_MEM_DCFG4                   (EXT_MEM_CTL_BASE + 0x0150)
#define EXT_MEM_DCFG5                   (EXT_MEM_CTL_BASE + 0x0154)
#define EXT_MEM_DCFG6                   (EXT_MEM_CTL_BASE + 0x0158)
#define EXT_MEM_DCFG7                   (EXT_MEM_CTL_BASE + 0x015c)



#define EXT_MEM_SCFG0                   (EXT_MEM_CTL_BASE + 0x0170)
#define EXT_MEM_SCFG1                   (EXT_MEM_CTL_BASE + 0x0174)

#define EXT_MEM_SCFG0_CS0               (EXT_MEM_CTL_BASE + 0x0180)
#define EXT_MEM_SCFG0_CS1               (EXT_MEM_CTL_BASE + 0x0190)
#define EXT_MEM_SCFG0_CS2               (EXT_MEM_CTL_BASE + 0x01A0)
#define EXT_MEM_SCFG0_CS3               (EXT_MEM_CTL_BASE + 0x01B0)

#define EXT_MEM_SCFG1_CS0               (EXT_MEM_CTL_BASE + 0x0184)
#define EXT_MEM_SCFG1_CS1               (EXT_MEM_CTL_BASE + 0x0194)
#define EXT_MEM_SCFG1_CS2               (EXT_MEM_CTL_BASE + 0x01A4)
#define EXT_MEM_SCFG1_CS3               (EXT_MEM_CTL_BASE + 0x01B4)

#define EXT_MEM_SCFG2_CS0               (EXT_MEM_CTL_BASE + 0x0188)
#define EXT_MEM_SCFG2_CS1               (EXT_MEM_CTL_BASE + 0x0198)
#define EXT_MEM_SCFG2_CS2               (EXT_MEM_CTL_BASE + 0x01A8)
#define EXT_MEM_SCFG2_CS3               (EXT_MEM_CTL_BASE + 0x01B8)

#define EXT_MEM_SCFG3_CS0               (EXT_MEM_CTL_BASE + 0x018C)
#define EXT_MEM_SCFG3_CS1               (EXT_MEM_CTL_BASE + 0x019C)
#define EXT_MEM_SCFG3_CS2               (EXT_MEM_CTL_BASE + 0x01AC)
#define EXT_MEM_SCFG3_CS3               (EXT_MEM_CTL_BASE + 0x01BC)

#define EMC_REG_CFG0_BASE_VALUE     (0x00000141)
#define EMC_REG_CFG1_BASE_VALUE     (0x0000000C)
#define EMC_REG_SCFG0_BASE_VALUE    (0x00FF0000)
#define EMC_REG_SCFG1_BASE_VALUE    (0x00001111)
#define EMC_REG_CS_CFG_BASE_VALUE   (0x00000000)
#define EMC_REG_CS_SCFG0_BASE_VALUE (0x00100030)
#define EMC_REG_CS_SCFG1_BASE_VALUE (0x00007F7F)
#define EMC_REG_CS_SCFG2_BASE_VALUE (0x00A0744F)
#define EMC_REG_CS_SCFG3_BASE_VALUE (0x01224112)
#define EMC_CS_ADDR_SIZE                (0x4000000)
#define EMC_DELAY_IN_CHIP               (10)
#define EMC_CLK_MAX                 (192)    /*MHZ*/
#define EMC_CS_NUM_MAX_SUPPORT      (EMC_CS4)   /*3 is temp code for iram limit */


/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
#if defined(CHIP_ENDIAN_LITTLE)
typedef union
{
    struct
    {
volatile  uint   mode_r             :
        BIT_RANGE (1,0);
volatile  uint   mode_w             :
        BIT_RANGE (3,2);
volatile  uint   single_split_r     :
        BIT_RANGE (4,4);
volatile  uint   single_split_w     :
        BIT_RANGE (5,5);
volatile  uint   clk_mode           :
        BIT_RANGE (6,6);
volatile  uint   reserved1          :
        BIT_RANGE (11,7);
volatile  uint   wait_en_r          :
        BIT_RANGE (12,12);
volatile  uint   wait_en_w          :
        BIT_RANGE (13,13);
volatile  uint   wait_pol           :
        BIT_RANGE (14,14);
volatile  uint   cre_pol            :
        BIT_RANGE (15,15);
volatile  uint   cre_mode           :
        BIT_RANGE (16,16);
volatile  uint   cre_en             :
        BIT_RANGE (17,17);
volatile  uint   row_detect_en_r    :
        BIT_RANGE (18,18);
volatile  uint   row_detect_en_w    :
        BIT_RANGE (19,19);
volatile  uint   row_length         :
        BIT_RANGE (22,20);
volatile  uint   admux_en           :
        BIT_RANGE (23,23);
volatile  uint   reserved2          :
        BIT_RANGE (31,24);
    } bits_map;
    volatile  uint32   reg_value;
} EMC_REG_CS_SCFG0;

typedef union
{
    struct
    {
volatile  uint   t_first_r          :
        BIT_RANGE (3,0);
volatile  uint   t_next_r           :
        BIT_RANGE (6,4);
volatile  uint   reserved1          :
        BIT_RANGE (7,7);
volatile  uint   t_first_w          :
        BIT_RANGE (11,8);
volatile  uint   t_next_w           :
        BIT_RANGE (14,12);
volatile  uint   reserved2          :
        BIT_RANGE (31,15);
    } bits_map;
    volatile  uint32   reg_value;
} EMC_REG_CS_SCFG1;

typedef union
{
    struct
    {
volatile  uint   t_adswt_r          :
        BIT_RANGE (1,0);
volatile  uint   t_adswt_w          :
        BIT_RANGE (3,2);
volatile  uint   t_oe_stp_r         :
        BIT_RANGE (6,4);
volatile  uint   reserved1          :
        BIT_RANGE (7,7);
volatile  uint   t_we_stp_w         :
        BIT_RANGE (10,8);
volatile  uint   reserved2          :
        BIT_RANGE (11,11);
volatile  uint   t_we_wdt_w         :
        BIT_RANGE (15,12);
volatile  uint   t_ce_stp_r         :
        BIT_RANGE (16,16);
volatile  uint   t_ce_stp_w         :
        BIT_RANGE (17,17);
volatile  uint   t_adv_stp_r        :
        BIT_RANGE (18,18);
volatile  uint   t_adv_stp_w        :
        BIT_RANGE (19,19);
volatile  uint   t_adv_wdt_r        :
        BIT_RANGE (21,20);
volatile  uint   t_adv_wdt_w        :
        BIT_RANGE (23,22);
volatile  uint   t_adv_mode_r       :
        BIT_RANGE (24,24);
volatile  uint   t_adv_mode_w       :
        BIT_RANGE (25,25);
volatile  uint   reserved3          :
        BIT_RANGE (31,26);
    } bits_map;
    volatile  uint32   reg_value;
} EMC_REG_CS_SCFG2;

typedef union
{
    struct
    {
volatile  uint   t_sample_stp       :
        BIT_RANGE (3,0);
volatile  uint   t_sample_dly_r     :
        BIT_RANGE (6,4);
volatile  uint   t_sample_phase_r   :
        BIT_RANGE (7,7);
volatile  uint   t_sample_dly_w     :
        BIT_RANGE (10,8);
volatile  uint   t_sample_phase_w   :
        BIT_RANGE (11,11);
volatile  uint   t_wait_ie_stp      :
        BIT_RANGE (15,12);
volatile  uint   t_wait_mask_dly_r  :
        BIT_RANGE (18,16);
volatile  uint   reserved1          :
        BIT_RANGE (19,19);
volatile  uint   t_wait_mask_dly_w  :
        BIT_RANGE (22,20);
volatile  uint   reserved2          :
        BIT_RANGE (23,23);
volatile  uint   t_wait_dly_r       :
        BIT_RANGE (25,24);
volatile  uint   t_wait_dly_w       :
        BIT_RANGE (27,26);
volatile  uint   t_valid_phase_r    :
        BIT_RANGE (29,28);
volatile  uint   t_valid_phase_w    :
        BIT_RANGE (31,30);
    } bits_map;
    volatile  uint32   reg_value;
} EMC_REG_CS_SCFG3;

typedef union
{
    struct
    {
volatile  uint   hburst_ren         :
        BIT_RANGE (0,0);
volatile  uint   hburst_wen         :
        BIT_RANGE (1,1);
volatile  uint   dburst_rmode       :
        BIT_RANGE (2,2);
volatile  uint   dburst_wmode       :
        BIT_RANGE (3,3);
volatile  uint   dburst_rlength     :
        BIT_RANGE (6,4);
volatile  uint   reserved1          :
        BIT_RANGE (7,7);
volatile  uint   dburst_wlength     :
        BIT_RANGE (10,8);
volatile  uint   reserved2          :
        BIT_RANGE (11,11);
volatile  uint   mem_sel            :
        BIT_RANGE (12,12);
volatile  uint   reserved3          :
        BIT_RANGE (31,13);
    } bits_map;
    volatile  uint32   reg_value;
} EMC_REG_CS_CFG;

typedef union
{
    struct
    {
volatile  uint   clksmem0_out_en        :
        BIT_RANGE (0,0);
volatile  uint   clksmem1_out_en        :
        BIT_RANGE (1,1);
volatile  uint   clksmem0_out_mode      :
        BIT_RANGE (2,2);
volatile  uint   clksmem1_out_mode      :
        BIT_RANGE (3,3);
volatile  uint   clksmem0_out_sel       :
        BIT_RANGE (5,4);
volatile  uint   clksmem1_out_sel       :
        BIT_RANGE (7,6);
volatile  uint   clksmem0_out_pol       :
        BIT_RANGE (8,8);
volatile  uint   clksmem1_out_pol       :
        BIT_RANGE (9,9);
volatile  uint   clksmem0_out_gate_en   :
        BIT_RANGE (10,10);
volatile  uint   clksmem1_out_gate_en   :
        BIT_RANGE (11,11);
volatile  uint   clksmem0_out_gate_mode :
        BIT_RANGE (12,12);
volatile  uint   clksmem1_out_gate_mode :
        BIT_RANGE (13,13);
volatile  uint   sample_auto_rst_en     :
        BIT_RANGE (14,14);
volatile  uint   sample_rst             :
        BIT_RANGE (15,15);
volatile  uint   wait_timeout_thr       :
        BIT_RANGE (23,16);
volatile  uint   wait_timeout_en        :
        BIT_RANGE (24,24);
volatile  uint   smem_wait_timeout_reg  :
        BIT_RANGE (25,25);
volatile  uint   reserved               :
        BIT_RANGE (31,26);
    } bits_map;
    volatile  uint32   reg_value;
} EMC_REG_SCFG0;
#else   //big endian
typedef union
{
    struct
    {
volatile  uint   reserved2          :
        BIT_RANGE (31,24);
volatile  uint   admux_en           :
        BIT_RANGE (23,23);
volatile  uint   row_length         :
        BIT_RANGE (22,20);
volatile  uint   row_detect_en_w    :
        BIT_RANGE (19,19);
volatile  uint   row_detect_en_r    :
        BIT_RANGE (18,18);
volatile  uint   cre_en             :
        BIT_RANGE (17,17);
volatile  uint   cre_mode           :
        BIT_RANGE (16,16);
volatile  uint   cre_pol            :
        BIT_RANGE (15,15);
volatile  uint   wait_pol           :
        BIT_RANGE (14,14);
volatile  uint   wait_en_w          :
        BIT_RANGE (13,13);
volatile  uint   wait_en_r          :
        BIT_RANGE (12,12);
volatile  uint   reserved1          :
        BIT_RANGE (11,7);
volatile  uint   clk_mode           :
        BIT_RANGE (6,6);
volatile  uint   single_split_w     :
        BIT_RANGE (5,5);
volatile  uint   single_split_r     :
        BIT_RANGE (4,4);
volatile  uint   mode_w             :
        BIT_RANGE (3,2);
volatile  uint   mode_r             :
        BIT_RANGE (1,0);
    } bits_map;
    volatile  uint32   reg_value;
} EMC_REG_CS_SCFG0;

typedef union
{
    struct
    {
volatile  uint   reserved2          :
        BIT_RANGE (31,15);
volatile  uint   t_next_w           :
        BIT_RANGE (14,12);
volatile  uint   t_first_w          :
        BIT_RANGE (11,8);
volatile  uint   reserved1          :
        BIT_RANGE (7,7);
volatile  uint   t_next_r           :
        BIT_RANGE (6,4);
volatile  uint   t_first_r          :
        BIT_RANGE (3,0);
    } bits_map;
    volatile  uint32   reg_value;
} EMC_REG_CS_SCFG1;

typedef union
{
    struct
    {
volatile  uint   reserved3          :
        BIT_RANGE (31,26);
volatile  uint   t_adv_mode_w       :
        BIT_RANGE (25,25);
volatile  uint   t_adv_mode_r       :
        BIT_RANGE (24,24);
volatile  uint   t_adv_wdt_w        :
        BIT_RANGE (23,22);
volatile  uint   t_adv_wdt_r        :
        BIT_RANGE (21,20);
volatile  uint   t_adv_stp_w        :
        BIT_RANGE (19,19);
volatile  uint   t_adv_stp_r        :
        BIT_RANGE (18,18);
volatile  uint   t_ce_stp_w         :
        BIT_RANGE (17,17);
volatile  uint   t_ce_stp_r         :
        BIT_RANGE (16,16);
volatile  uint   t_we_wdt_w         :
        BIT_RANGE (15,12);
volatile  uint   reserved2          :
        BIT_RANGE (11,11);
volatile  uint   t_we_stp_w         :
        BIT_RANGE (10,8);
volatile  uint   reserved1          :
        BIT_RANGE (7,7);
volatile  uint   t_oe_stp_r         :
        BIT_RANGE (6,4);
volatile  uint   t_adswt_w          :
        BIT_RANGE (3,2);
volatile  uint   t_adswt_r          :
        BIT_RANGE (1,0);
    } bits_map;
    volatile  uint32   reg_value;
} EMC_REG_CS_SCFG2;

typedef union
{
    struct
    {
volatile  uint   t_valid_phase_w    :
        BIT_RANGE (31,30);
volatile  uint   t_valid_phase_r    :
        BIT_RANGE (29,28);
volatile  uint   t_wait_dly_w       :
        BIT_RANGE (27,26);
volatile  uint   t_wait_dly_r       :
        BIT_RANGE (25,24);
volatile  uint   reserved2          :
        BIT_RANGE (23,23);
volatile  uint   t_wait_mask_dly_w  :
        BIT_RANGE (22,20);
volatile  uint   reserved1          :
        BIT_RANGE (19,19);
volatile  uint   t_wait_mask_dly_r  :
        BIT_RANGE (18,16);
volatile  uint   t_wait_ie_stp      :
        BIT_RANGE (15,12);
volatile  uint   t_sample_phase_w   :
        BIT_RANGE (11,11);
volatile  uint   t_sample_dly_w     :
        BIT_RANGE (10,8);
volatile  uint   t_sample_phase_r   :
        BIT_RANGE (7,7);
volatile  uint   t_sample_dly_r     :
        BIT_RANGE (6,4);
volatile  uint   t_sample_stp       :
        BIT_RANGE (3,0);
    } bits_map;
    volatile  uint32   reg_value;
} EMC_REG_CS_SCFG3;

typedef union
{
    struct
    {
volatile  uint   reserved3          :
        BIT_RANGE (31,13);
volatile  uint   mem_sel            :
        BIT_RANGE (12,12);
volatile  uint   reserved2          :
        BIT_RANGE (11,11);
volatile  uint   dburst_wlength     :
        BIT_RANGE (10,8);
volatile  uint   reserved1          :
        BIT_RANGE (7,7);
volatile  uint   dburst_rlength     :
        BIT_RANGE (6,4);
volatile  uint   dburst_wmode       :
        BIT_RANGE (3,3);
volatile  uint   dburst_rmode       :
        BIT_RANGE (2,2);
volatile  uint   hburst_wen         :
        BIT_RANGE (1,1);
volatile  uint   hburst_ren         :
        BIT_RANGE (0,0);
    } bits_map;
    volatile  uint32   reg_value;
} EMC_REG_CS_CFG;

typedef union
{
    struct
    {
volatile  uint   reserved           :
        BIT_RANGE (31,26);
volatile  uint   smem_wait_timeout_reg      :
        BIT_RANGE (25,25);
volatile  uint   wait_timeout_en            :
        BIT_RANGE (24,24);
volatile  uint   wait_timeout_thr       :
        BIT_RANGE (23,16);
volatile  uint   sample_rst     :
        BIT_RANGE (15,15);
volatile  uint   sample_auto_rst_en     :
        BIT_RANGE (14,14);
volatile  uint   clksmem1_out_gate_mode         :
        BIT_RANGE (13,13);
volatile  uint   clksmem0_out_gate_mode         :
        BIT_RANGE (12,12);
volatile  uint   clksmem1_out_gate_en           :
        BIT_RANGE (11,11);
volatile  uint   clksmem0_out_gate_en           :
        BIT_RANGE (10,10);
volatile  uint   clksmem1_out_pol           :
        BIT_RANGE (9,9);
volatile  uint   clksmem0_out_pol       :
        BIT_RANGE (8,8);
volatile  uint   clksmem1_out_sel           :
        BIT_RANGE (7,6);
volatile  uint   clksmem0_out_sel       :
        BIT_RANGE (5,4);
volatile  uint   clksmem1_out_mode      :
        BIT_RANGE (3,3);
volatile  uint   clksmem0_out_mode      :
        BIT_RANGE (2,2);
volatile  uint   clksmem1_out_en            :
        BIT_RANGE (1,1);
volatile  uint   clksmem0_out_en            :
        BIT_RANGE (0,0);
    } bits_map;
    volatile  uint32   reg_value;
} EMC_REG_SCFG0;
#endif

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
#define EMC_WAIT_WRITEBUFFER_DONE   while(REG32(EMC_REG_STS0_ADDR) & BIT_0);
/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_EMC_REG_V3_H_


