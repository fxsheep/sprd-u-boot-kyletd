/******************************************************************************
 ** File Name:    sc8800_reg_lcm.h                                           *
 ** Author:       Jianping.Wang                                               *
 ** DATE:         01/21/2006                                                  *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 01/21/2006    Jianping.Wang   Create.                                     *
 ******************************************************************************/
#ifndef _SC8800H_REG_LCM_
#define _SC8800H_REG_LCM_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                          Compiler Flag                                    *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
**                               Macro Define                                **
**----------------------------------------------------------------------------*/
#if 0
#define LCM_REG_BASE			0x60001c40
#define LCM_PARAMETER0			(LCM_REG_BASE + 0x0000)
#define LCM_RSTN				(LCM_REG_BASE + 0x0004)
#define LCM_DATANUMBER          (LCM_REG_BASE + 0x0008)
#define LCM_PARAMETER1			(LCM_REG_BASE + 0x000c)
#define LCM_MODESEL				(LCM_REG_BASE + 0x0010)
#define LCM_RDDATA				(LCM_REG_BASE + 0x0014)
#define LCM_STATUS				(LCM_REG_BASE + 0x0018)
#define LCM_RGBMODE				(LCM_REG_BASE + 0x001c)
#define LCM_SELPIN				(LCM_REG_BASE + 0x0020)

#define LCM_CSN_BASE			0x60001400
#define LCM_CS0ADDR_BASE		(LCM_CSN_BASE + 0x0000)
#define LCM_CS1ADDR_BASE		(LCM_CSN_BASE + 0x0100)
#define LCM_CS2ADDR_BASE		(LCM_CSN_BASE + 0x0200)
#define LCM_CS3ADDR_BASE		(LCM_CSN_BASE + 0x0300)
#define LCM_CS4ADDR_BASE		(LCM_CSN_BASE + 0x0400)
#define LCM_CS5ADDR_BASE		(LCM_CSN_BASE + 0x0500)

/**---------------------------------------------------------------------------*
**                               Data Prototypt                              **
**----------------------------------------------------------------------------*/
typedef union _lcm_parameter0_tag {
	struct _lcm_paramter0_map {
		volatile unsigned int reserved		:12;//Reserved
		volatile unsigned int rcss			:2;
		volatile unsigned int rlpw			:4;
		volatile unsigned int rhpw			:4;
		volatile unsigned int wcss			:2;
		volatile unsigned int wlpw			:4;
		volatile unsigned int whpw			:4;
	}mBits;
	volatile unsigned int dwValue;
}LCM_PARAMTER0_U;

typedef union _lcm_parameter1_tag {
	struct _lcm_paramter1_map {
		volatile unsigned int reserved		:12;//Reserved
		volatile unsigned int rcss			:2;
		volatile unsigned int rlpw			:4;
		volatile unsigned int rhpw			:4;
		volatile unsigned int wcss			:2;
		volatile unsigned int wlpw			:4;
		volatile unsigned int whpw			:4;
	}mBits;
	volatile unsigned int dwValue;
}LCM_PARAMTER1_U;

typedef union _lcm_rstn_tag {
	struct _lcm_rstn_map {
		volatile unsigned int reserved 		:31;//Reserved
		volatile unsigned int rstn			:1;
	}mBits;
	volatile unsigned int dwValue;
}LCM_RSTN_U;

typedef union _lcm_modesel_tag {
	struct _lcm_modesel_map {
		volatile unsigned int reserved 		:26;//Reserved
		volatile unsigned int modesel5		:1;
		volatile unsigned int modesel4		:1;
		volatile unsigned int modesel3		:1;
		volatile unsigned int modesel2		:1;
		volatile unsigned int modesel1		:1;
		volatile unsigned int modesel0		:1;
	}mBits;
	volatile unsigned int dwValue;
}LCM_MODESEL_U;

typedef union _lcm_rddata_tag {
	struct _lcm_rddata_map {
		volatile unsigned int reserved		:14;//Reserved
		volatile unsigned int rd_data		:18;
	}mBits;
	volatile unsigned int dValue;
}LCM_RDDATA_U;

typedef union _lcm_status_tag {
	struct _lcm_status_map {
		volatile unsigned int reserved		:29;//Reserved
		volatile unsigned int fifo_empty	:1;
		volatile unsigned int fifo_full		:1;
		volatile unsigned int busy			:1;
	}mBits;
	volatile unsigned int dValue;
}LCM_STATUS_U;

typedef union _lcm_rgbmode_tag {
	struct _lcm_rgbmode_map {
		volatile unsigned int reserved		:20;//Reserved
		volatile unsigned int rgbmode5		:2;
		volatile unsigned int rgbmode4		:2;
		volatile unsigned int rgbmode3		:2;
		volatile unsigned int rgbmode2		:2;
		volatile unsigned int rgbmode1		:2;
		volatile unsigned int rgbmode0		:2;
	}mBits;
	volatile unsigned int dwValue;
}LCM_RGBMODE_U;

typedef union _lcm_selpin_tag {
	struct _lcm_selpin_map {
		volatile unsigned int reserved		:22;//Reserved
		volatile unsigned int cd5_data15	:1;//0:data15;1:cd5
		volatile unsigned int cd4_data14	:1;//0:data14;1:cd4
		volatile unsigned int cd3_data13	:1;//0:data13;1:cd3
		volatile unsigned int cd2_data12	:1;//0:data12;1:cd2
		volatile unsigned int cd1_data11	:1;//0:data11;1:cd1
		volatile unsigned int cd5_data17	:1;//0:data17;1:cd5
		volatile unsigned int cd4_data16	:1;//0:data16;1:cd4
		volatile unsigned int cd3_cs3		:1;//0:cs3;1:cd3
		volatile unsigned int cd2_cs4		:1;//0:cs4;1:cd2
		volatile unsigned int cd1_cs5		:1;//0:cs5;1:cd1
	}mBits;
	volatile unsigned int dwValue;
}LCM_SELPIN_U;

typedef struct lcm_dev_reg_tag
{
	volatile LCM_PARAMTER0_U	para0;
	volatile LCM_PARAMTER1_U	para1;
	volatile LCM_RSTN_U			rstn;
	volatile LCM_MODESEL_U  	mode_sel;
	volatile LCM_RDDATA_U   	rd_data;
	volatile LCM_STATUS_U   	status;
	volatile LCM_RGBMODE_U 		rgb_mode;
	volatile LCM_SELPIN_U		sel_pin;
}LCM_DEV_REG_T;
#endif
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 

