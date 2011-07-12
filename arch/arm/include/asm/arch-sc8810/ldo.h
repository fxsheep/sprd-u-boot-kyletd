/******************************************************************************
 ** File Name:      ldo_manager_sc6600l.h                                             *
 ** Author:         jiexia.yu                                                 *
 ** DATE:           07/09/2007                                                *
 ** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic function for ldo management.  *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/09/2007     jiexia.yu        Create.                                   *
 ******************************************************************************/
#ifndef _LDO_DRV_H_
#define _LDO_DRV_H_

#include <asm/arch/adi_hal_internal.h>

/* set or clear the bit of register in reg_addr address */
#define REG_SETCLRBIT(_reg_addr, _set_bit, _clr_bit)	\
	do	\
	{	\
        reg_val = ANA_REG_GET(_reg_addr);   \
        	reg_val |= (_set_bit);	\
        	reg_val &= ~(_clr_bit);	\
        ANA_REG_SET(_reg_addr,reg_val);   \
        }while(0);
		
/* set the value of register bits corresponding with bit_mask */
#define REG_SETBIT(_reg_addr, _bit_mask, _bit_set) ANA_REG_MSK_OR(_reg_addr, _bit_set, _bit_mask);

/* macro used to get voltage level(one bit level) */
#define GET_LEVELBIT(_reg_addr, _bit_mask, _bit_set, _level_var)   \
	{	\
		(_level_var) |=	\
                        ((ANA_REG_GET(_reg_addr)&(_bit_mask))?(_bit_set):0);    \
	}

/* macro used to get voltage level */
#define GET_LEVEL(_reg_addr, _bit0_mask, _bit1_mask, _level_var)    \
        {	\
        	(_level_var) |=	\
                        (((ANA_REG_GET(_reg_addr)&(_bit0_mask))?(BIT_0):0)|    \
                         ((ANA_REG_GET(_reg_addr)&(_bit1_mask))?(BIT_1):0)); \
        }	

/* macro used to set voltage level according to bit field */
#define SET_LEVELBIT(_reg_addr, _bit_mask, _set_bit, _rst_bit)  \
	{	\
		REG_SETBIT(	\
			_reg_addr, 	\
			(_set_bit) | (_rst_bit),	\
			((_set_bit)&(_bit_mask)) | ((_rst_bit)&(~_bit_mask))	\
			)	\
	}

/* macro used to set voltage level according to two bits */
#define SET_LEVEL(_reg_addr, _bit0_mask, _bit1_mask, _set_bit0, 	\
		_rst_bit0, _set_bit1, _rst_bit1)    \
	{	\
		REG_SETBIT(	\
			(_reg_addr), 	\
			((_set_bit0)|(_rst_bit0) | (_set_bit1)|(_rst_bit1)),  \
			(((_set_bit0)&(_bit0_mask)) | ((_rst_bit0)&(~_bit0_mask))|	\
			((_set_bit1)&(_bit1_mask)) | ((_rst_bit1)&(~_bit1_mask)))	\
			)	\
	}

#define LDO_REG_OR ANA_REG_OR
#define LDO_REG_AND ANA_REG_AND
#define LDO_REG_GET  ANA_REG_GET

/**---------------------------------------------------------------------------*
 **                         Defines                                           *
 **---------------------------------------------------------------------------*/

typedef enum 
{
    LDO_LDO_NULL  = 0,   //id for NULL
    LDO_LDO_IO     ,       //VDDIO
    LDO_LDO_IO18     ,       //VDDIO18
    LDO_LDO_IO28     ,       //VDDIO28
    LDO_LDO_NF      ,      //LDONF
    LDO_LDO_MEM    ,       //VDDMEM
    LDO_LDO_CORE    ,      //VDDCORE
    LDO_LDO_CORE_SLP,      //VDDCORE Sleep
    LDO_LDO_AUX1    ,      //Sensor core voltage used by sc6600l
    LDO_LDO_AUX2    ,      //Sensor analog  voltage used by sc6600l
    LDO_LDO_SIM0    ,     //VDD SIM0
    LDO_LDO_SIM1    ,     //VDD SIM1
    LDO_LDO_USB     ,     //VDDUSB
    LDO_LDO_RF      ,      //LDORF (LDO1)
    LDO_LDO_LCD    ,      //LDOLCD
    LDO_LDO_VIDEO,
    LDO_LDO_RTC     ,      //VDDRTC
    LDO_LDO_ABB     ,  //AVDDBB
    LDO_LDO_PLL     ,  //VDDPLL
    LDO_LDO_AVB     ,  //LDOVB
    LDO_LDO_AVBO    ,  //LDOVBO
    LDO_LDO_BP18,
    LDO_LDO_D3V,
    LDO_LDO_DVD18,
    LDO_LDO_VREFRF  ,  //VREFRF
    LDO_LDO_VREF    ,  //VREF
    LDO_LDO_VREF27  ,  //VREF27
    LDO_LDO_SDIO  ,  //SDIO
    ///SC8800G
    LDO_LDO_CAMA    ,
    LDO_LDO_CAMD0   ,
    LDO_LDO_CAMD1   ,
    LDO_LDO_VDD18   ,
    LDO_LDO_VDD25   ,
    LDO_LDO_VDD28   ,
    LDO_LDO_RF0     ,
    LDO_LDO_RF1     ,
    LDO_LDO_USBD    ,
    ////end SC8800G
    LDO_LDO_MAX     //id for calculate LDO number only!
}LDO_ID_E;

#define LDO_LDO_LCM		LDO_LDO_AUX1
#define LDO_LDO_CMR	LDO_LDO_AUX2

typedef enum 
{
    SLP_LDO_NULL  = 0,  //id for NULL
    SLP_LDO_ABB,        //id for abb
    SLP_LDO_VBO,        //id for voice band
    SLP_LDO_AUX1,       //id for sensor core
    SLP_LDO_AUX2,       //id for sensor analog and io
    SLP_LDO_USB,        //id for USB
    SLP_LDO_RF,         //id for RF
    SLP_LDO_COREV,      //id for ldo core
    SLP_LDO_PLL,
    SLP_LDO_AFC,
    SLP_LDO_SDIO,       //id for ldo sdio
    SLP_LDO_IO18,       //id for vddio18
    SLP_LDO_IO28,       //id for vddio28
    SLP_LDO_BG,          //id for ldo BG
    ///SC8800G
    SLP_LDO_PA,
    SLP_LDO_DVDD18,
    SLP_LDO_VDD25,
    SLP_LDO_VDD18,
    SLP_LDO_VDD28,
    SLP_LDO_CAMA,
    SLP_LDO_CAMD1,
    SLP_LDO_CAMD0,
    SLP_LDO_SIM1,
    SLP_LDO_SIM0,
    SLP_LDO_RF1,
    SLP_LDO_RF0,
    ///END SC8800G
    SLP_LDO_MAX
}SLP_LDO_E;

typedef enum
{
	LDO_VOLT_LEVEL0 = 0,
	LDO_VOLT_LEVEL1,
	LDO_VOLT_LEVEL2,
	LDO_VOLT_LEVEL3,
	LDO_VOLT_LEVEL_MAX
}LDO_VOLT_LEVEL_E;

//LDO error flag definition
typedef enum
{
    LDO_ERR_OK,
    LDO_ERR_INVALID_HANDLE,
    LDO_ERR_INVALID_OPERATION,
    LDO_ERR_ERR
}LDO_ERR_E;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description:  Turn on the LDO specified by input parameter ldo_id  
//	Global resource dependence: NONE
//  Author:  jiexia.yu
//	Note:    return value = LDO_ERR_OK if operation is executed successfully           
/*****************************************************************************/
 LDO_ERR_E LDO_TurnOnLDO(LDO_ID_E ldo_id);

/*****************************************************************************/
//  Description:  Turo off the LDO specified by parameter ldo_id
//	Global resource dependence: NONE
//  Author: jiexia.yu
//	Note:           
/*****************************************************************************/
 LDO_ERR_E LDO_TurnOffLDO(LDO_ID_E ldo_id);

/*****************************************************************************/
//  Description: Find the LDO status -- ON or OFF
//	Global resource dependence: 
//  Author: jiexia.yu         
//	Note: return SCI_TRUE means LDO is ON, SCI_FALSE is OFF        
/*****************************************************************************/
 int LDO_IsLDOOn(LDO_ID_E ldo_id);

/*****************************************************************************/
//  Description: Get LDO voltage level
//	Global resource dependence: 
//  Author: jiexia.yu
//	Note:           
/*****************************************************************************/
 LDO_VOLT_LEVEL_E LDO_GetVoltLevel(LDO_ID_E ldo_id);

/*****************************************************************************/
//  Description:  This function could be invoked by customer for LDO voltage
//                level & referece voltage level initiation.
//	Global resource dependence: 
//  Author: jiexia.yu         
//	Note:           
/*****************************************************************************/
 LDO_ERR_E LDO_SetVoltLevel(LDO_ID_E ldo_id, LDO_VOLT_LEVEL_E level);

/*****************************************************************************/
//  Description:  Control The VCAM Domain PAD to avoid VCAM Domain PAD input not floating
//	Global resource dependence: NONE
//  Author:  Tao.Feng && Yi.Qiu
//	Note:    return value = LDO_ERR_OK if operation is executed successfully           
/*****************************************************************************/
 LDO_ERR_E LDO_ControVCAMPad(int pad_on);

/*****************************************************************************/
//  Description:  Shut down any LDO that do not used when system enters deepsleep
//	Global resource dependence: s_ldo_reopen[]
//  Author: jiexia.yu     
//	Note:           
/*****************************************************************************/
 void LDO_DeepSleepInit(void);
 void LDO_TurnOffAllLDO(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#endif // _LDO_MANAGER_H_


