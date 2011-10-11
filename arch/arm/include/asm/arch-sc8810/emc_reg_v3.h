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
#define EMC_T_REF_CLK           6500 //t_ref reference clk, freqency = 6.5mhz
#define EMC_DLL_ON_OFF  	FALSE

#define EXT_MEM_CTL_BASE        		0x20000000


#define EXT_MEM_CFG0                    (EXT_MEM_CTL_BASE + 0x0000)
#define EXT_MEM_CFG1                    (EXT_MEM_CTL_BASE + 0x0004)


#define EXT_MEM_CFG0_CS0               (EXT_MEM_CTL_BASE + 0x0010)
#define EXT_MEM_CFG0_CS1               (EXT_MEM_CTL_BASE + 0x0014)
#define EXT_MEM_CFG0_CS2               (EXT_MEM_CTL_BASE + 0x0018)
#define EXT_MEM_CFG0_CS3               (EXT_MEM_CTL_BASE + 0x001c)


#define EXT_MEM_CFG0_ACH0               (EXT_MEM_CTL_BASE + 0x0020)
#define EXT_MEM_CFG1_ACH0               (EXT_MEM_CTL_BASE + 0x0024)
#define EXT_MEM_CFG0_ACH1               (EXT_MEM_CTL_BASE + 0x0028)
#define EXT_MEM_CFG1_ACH1               (EXT_MEM_CTL_BASE + 0x002C)
#define EXT_MEM_CFG0_HCH2               (EXT_MEM_CTL_BASE + 0x0030)
#define EXT_MEM_CFG1_HCH2               (EXT_MEM_CTL_BASE + 0x0034)
#define EXT_MEM_CFG0_HCH3               (EXT_MEM_CTL_BASE + 0x0038)
#define EXT_MEM_CFG1_HCH3               (EXT_MEM_CTL_BASE + 0x003C)
#define EXT_MEM_CFG0_HCH4               (EXT_MEM_CTL_BASE + 0x0040)
#define EXT_MEM_CFG1_HCH4               (EXT_MEM_CTL_BASE + 0x0044)
#define EXT_MEM_CFG0_HCH5               (EXT_MEM_CTL_BASE + 0x0048)
#define EXT_MEM_CFG1_HCH5               (EXT_MEM_CTL_BASE + 0x004C)
#define EXT_MEM_CFG0_HCH6               (EXT_MEM_CTL_BASE + 0x0050)
#define EXT_MEM_CFG1_HCH6               (EXT_MEM_CTL_BASE + 0x0054)
#define EXT_MEM_CFG0_HCH7               (EXT_MEM_CTL_BASE + 0x0058)
#define EXT_MEM_CFG1_HCH7               (EXT_MEM_CTL_BASE + 0x005C)
#define EXT_MEM_CFG0_HCH8               (EXT_MEM_CTL_BASE + 0x0060)
#define EXT_MEM_CFG1_HCH8               (EXT_MEM_CTL_BASE + 0x0064)

#define EXT_MEM_DL0                     (EXT_MEM_CTL_BASE + 0x0100)
#define EXT_MEM_DL1                     (EXT_MEM_CTL_BASE + 0x0104)
#define EXT_MEM_DL2                     (EXT_MEM_CTL_BASE + 0x0108)
#define EXT_MEM_DL3                     (EXT_MEM_CTL_BASE + 0x010C)
#define EXT_MEM_DL4                     (EXT_MEM_CTL_BASE + 0x0110)
#define EXT_MEM_DL5                     (EXT_MEM_CTL_BASE + 0x0114)
#define EXT_MEM_DL6                     (EXT_MEM_CTL_BASE + 0x0118)
#define EXT_MEM_DL7                     (EXT_MEM_CTL_BASE + 0x011C)
#define EXT_MEM_DL8                     (EXT_MEM_CTL_BASE + 0x0120)
#define EXT_MEM_DL9                     (EXT_MEM_CTL_BASE + 0x0124)
#define EXT_MEM_DL10                    (EXT_MEM_CTL_BASE + 0x0128)   	
#define EXT_MEM_DL11                    (EXT_MEM_CTL_BASE + 0x012c)
#define EXT_MEM_DL12                    (EXT_MEM_CTL_BASE + 0x0130)
#define EXT_MEM_DL13                    (EXT_MEM_CTL_BASE + 0x0134)
#define EXT_MEM_DL14                    (EXT_MEM_CTL_BASE + 0x0138)
#define EXT_MEM_DL15                    (EXT_MEM_CTL_BASE + 0x013c)
#define EXT_MEM_DL16                    (EXT_MEM_CTL_BASE + 0x0140)
#define EXT_MEM_DL17                    (EXT_MEM_CTL_BASE + 0x0144)
#define EXT_MEM_DL18                    (EXT_MEM_CTL_BASE + 0x0148)
#define EXT_MEM_DL19                    (EXT_MEM_CTL_BASE + 0x014c)    	

#define EXT_MEM_CFG0_DLL            (EXT_MEM_CTL_BASE + 0x0170)    	

#define EXT_MEM_STS3                    (EXT_MEM_CTL_BASE + 0x00AC)

#define EXT_MEM_DCFG0                   (EXT_MEM_CTL_BASE + 0x0180)
#define EXT_MEM_DCFG1                   (EXT_MEM_CTL_BASE + 0x0184)
#define EXT_MEM_DCFG2                   (EXT_MEM_CTL_BASE + 0x0188)
#define EXT_MEM_DCFG3                   (EXT_MEM_CTL_BASE + 0x018c)
#define EXT_MEM_DCFG4                   (EXT_MEM_CTL_BASE + 0x0190)
#define EXT_MEM_DCFG5                   (EXT_MEM_CTL_BASE + 0x0194)
#define EXT_MEM_DCFG6                   (EXT_MEM_CTL_BASE + 0x0198)
#define EXT_MEM_DCFG7                   (EXT_MEM_CTL_BASE + 0x019c)
#define EXT_MEM_DCFG8                   (EXT_MEM_CTL_BASE + 0x01A0)








/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
//External Memory Control.
typedef struct EMC_CFG_REG_TAG
{
    VOLATILE uint32 cfg0;
    VOLATILE uint32 cfg1;
    VOLATILE uint32 cfg0_cs0;
    VOLATILE uint32 cfg0_cs1;
    VOLATILE uint32 cfg0_cs2;
    VOLATILE uint32 cfg0_cs3;
    VOLATILE uint32 cfg0_ch0;
    VOLATILE uint32 cfg0_ch1;
    VOLATILE uint32 cfg0_ch2;
    VOLATILE uint32 cfg0_ch3;
    VOLATILE uint32 cfg0_ch4;
    VOLATILE uint32 cfg0_ch5;
    VOLATILE uint32 cfg0_ch6;
    VOLATILE uint32 cfg0_ch7;
    VOLATILE uint32 cfg0_ch8;
    VOLATILE uint32 cfg0_ch9;
    VOLATILE uint32 cfg0_ch10;
    VOLATILE uint32 cfg0_ch11;
    VOLATILE uint32 cfg0_ch12;
    VOLATILE uint32 cfg0_ch13;
    VOLATILE uint32 cfg0_ch14;
    VOLATILE uint32 cfg0_ch15;
}
EMC_CFG_REG_T, *EMC_CFG_REG_T_PTR;

typedef struct EMC_DCFG_REG_TAG
{
    VOLATILE uint32 dcfg0;
    VOLATILE uint32 dcfg1;
    VOLATILE uint32 dcfg2;
    VOLATILE uint32 dcfg3;
    VOLATILE uint32 dcfg4;
    VOLATILE uint32 dcfg5;
    VOLATILE uint32 dcfg6;
    VOLATILE uint32 dcfg7;
}
EMC_DCFG_REG_T, *EMC_DCFG_REG_T_PTR;
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


