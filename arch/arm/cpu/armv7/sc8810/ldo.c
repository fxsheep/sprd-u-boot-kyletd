/******************************************************************************
 ** File Name:      ldo_drv.c                                             *
 ** Author:         Yi.Qiu                                                 *
 ** DATE:           01/09/2009                                                *
 ** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic function for ldo management.  *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/09/2009     Yi.Qiu        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <common.h>
#include <asm/io.h>

#include <asm/arch/regs_global.h>
#include <asm/arch/bits.h>
#include <asm/arch/ldo.h>
#include <asm/arch/regs_ana.h>

#define LDO_INVALID_REG	0xFFFFFFFF
#define LDO_INVALID_BIT		0xFFFFFFFF

#define TRUE			1
#define FALSE		0

#define CHIP_REG_OR(reg_addr, value)    (*(volatile unsigned int *)(reg_addr) |= (unsigned int)(value))
#define CHIP_REG_AND(reg_addr, value)   (*(volatile unsigned int *)(reg_addr) &= (unsigned int)(value))
#define CHIP_REG_GET(reg_addr)          (*(volatile unsigned int *)(reg_addr))
#define CHIP_REG_SET(reg_addr, value)   (*(volatile unsigned int *)(reg_addr)  = (unsigned int)(value))

#undef NULL
#ifndef NULL
#define NULL 0x0
#endif

#define SCI_ASSERT(condition) BUG_ON(!(condition))  
#define SCI_PASSERT(condition, format...)  \
	do {		\
		if(!(condition)) { \
			printf("function :%s\r\n", __FUNCTION__);\
			BUG();	\
		} \
	}while(0)
	
typedef enum 
{
	SLP_BIT_CLR = 0,
	SLP_BIT_SET
}SLP_BIT_DEF_E;

typedef struct  
{
	LDO_ID_E id;
	unsigned int bp_reg;
	unsigned int bp;
	unsigned int bp_rst;
	unsigned int level_reg_b0;
	unsigned int b0;
	unsigned int b0_rst;
	unsigned int level_reg_b1;
	unsigned int b1;
	unsigned int b1_rst;
	unsigned int valid_time;
	unsigned int init_level;
	int ref;
}LDO_CTL_T, * LDO_CTL_PTR;

typedef struct  
{
	SLP_LDO_E id;
	unsigned int ldo_reg;
	unsigned int mask;
	SLP_BIT_DEF_E value;
	int valid;
	unsigned int reserved;
}SLP_LDO_CTL_T, * SLP_LDO_CTL_PTR;


LDO_CTL_T ldo_ctl_data[] =
{
    {
        LDO_LDO_ABB,   NULL,           NULL,   NULL,   ANA_LDO_VCTL0,  BIT_12, BIT_13,
        ANA_LDO_VCTL0,  BIT_14, BIT_15, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_RF1,   NULL,           NULL,   NULL,   ANA_LDO_VCTL0,  BIT_8,  BIT_9,
        ANA_LDO_VCTL0,  BIT_10, BIT_11, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_RF0,   NULL,           NULL,   NULL,   ANA_LDO_VCTL0,  BIT_4,  BIT_5,
        ANA_LDO_VCTL0,  BIT_6,  BIT_7,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_RTC,   NULL,           NULL,   NULL,   ANA_LDO_VCTL0,  BIT_0,  BIT_1,
        ANA_LDO_VCTL0,  BIT_2,  BIT_3,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_SDIO,  ANA_LDO_PD_CTL, BIT_2,  BIT_3,  ANA_LDO_VCTL1,  BIT_12,BIT_13,
        ANA_LDO_VCTL1,  BIT_14, BIT_15, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_AVBO,  ANA_LDO_PD_CTL, BIT_14, BIT_15, ANA_LDO_VCTL1,  BIT_8,  BIT_9,
        ANA_LDO_VCTL1,  BIT_10, BIT_11, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_SIM1,  ANA_LDO_PD_CTL, BIT_6,  BIT_7,  ANA_LDO_VCTL1,  BIT_4,  BIT_5,
        ANA_LDO_VCTL1,  BIT_6,  BIT_7,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_SIM0,  ANA_LDO_PD_CTL, BIT_4,  BIT_5,  ANA_LDO_VCTL1,  BIT_0,  BIT_1,
        ANA_LDO_VCTL1,  BIT_2,  BIT_3,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_USB,   ANA_LDO_PD_CTL, BIT_0,  BIT_1,  ANA_LDO_VCTL2,  BIT_12, BIT_13,
        ANA_LDO_VCTL2,  BIT_14, BIT_15, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_USBD,  NULL,           NULL,   NULL,   NULL,           NULL,   NULL,
        NULL,           NULL,   NULL,   NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_CAMA,  ANA_LDO_PD_CTL, BIT_12, BIT_13, ANA_LDO_VCTL2,  BIT_8,  BIT_9,
        ANA_LDO_VCTL2,  BIT_10, BIT_11, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_CAMD1, ANA_LDO_PD_CTL, BIT_10, BIT_11, ANA_LDO_VCTL2,  BIT_4,  BIT_5,
        ANA_LDO_VCTL2,  BIT_6,  BIT_7,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_CAMD0, ANA_LDO_PD_CTL, BIT_8,  BIT_9,  ANA_LDO_VCTL2,  BIT_0,  BIT_1,
        ANA_LDO_VCTL2,  BIT_2,  BIT_3,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_VDD25, NULL,           NULL,   NULL,   ANA_LDO_VCTL3,  BIT_8,  BIT_9,
        ANA_LDO_VCTL3,  BIT_10, BIT_11, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_VDD18, NULL,           NULL,   NULL,   ANA_LDO_VCTL3,  BIT_4,  BIT_5,
        ANA_LDO_VCTL3,  BIT_6,  BIT_7,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_VDD28, NULL,           NULL,   NULL,   ANA_LDO_VCTL3,  BIT_0,  BIT_1,
        ANA_LDO_VCTL3,  BIT_2,  BIT_3,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_MAX,   NULL,           NULL,   NULL,   NULL,           NULL,   NULL,
        NULL,           NULL,   NULL,   NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    }
};

SLP_LDO_CTL_T slp_ldo_ctl_data[] =
{
    {SLP_LDO_PA,        ANA_LDO_SLP,    BIT_15, SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_DVDD18,    ANA_LDO_SLP,    BIT_14, SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_VDD25,     ANA_LDO_SLP,    BIT_13, SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_VDD18,     ANA_LDO_SLP,    BIT_12, SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_VDD28,     ANA_LDO_SLP,    BIT_11, SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_ABB,       ANA_LDO_SLP,    BIT_10, SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_SDIO,      ANA_LDO_SLP,    BIT_9,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_VBO,       ANA_LDO_SLP,    BIT_8,  SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_CAMA,      ANA_LDO_SLP,    BIT_7,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_CAMD1,     ANA_LDO_SLP,    BIT_6,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_CAMD0,     ANA_LDO_SLP,    BIT_5,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_USB,       ANA_LDO_SLP,    BIT_4,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_SIM1,      ANA_LDO_SLP,    BIT_3,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_SIM0,      ANA_LDO_SLP,    BIT_2,  SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_RF1,       ANA_LDO_SLP,    BIT_1,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_RF0,       ANA_LDO_SLP,    BIT_0,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_MAX,       NULL,           NULL,   SLP_BIT_SET,    TRUE,   NULL}
};

static  LDO_CTL_PTR Ldo_Get_Cfg(void)
{
	return ldo_ctl_data;
}

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

LDO_CTL_PTR g_ldo_ctl_tab = NULL;


/*****************************************************************************/
//  Description:  Slp_Ldo_Get_Cfg
//	Global resource dependence: NONE
//  Author: 
//	Note:    Slp_Ldo_Get_Cfg
/*****************************************************************************/
static SLP_LDO_CTL_PTR Slp_Ldo_Get_Cfg(void)
{	
	return  slp_ldo_ctl_data;
}

/**---------------------------------------------------------------------------*
 **                         Function Declaration                              *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:  Turn on the LDO specified by input parameter ldo_id  
//	Global resource dependence: NONE
//  Author:  Tao.Feng && Yi.Qiu
//	Note:    return value = LDO_ERR_OK if operation is executed successfully           
/*****************************************************************************/
static  LDO_CTL_PTR LDO_GetLdoCtl(LDO_ID_E ldo_id)
{
	int i = 0;
	LDO_CTL_PTR ctl = NULL;
	
	SCI_ASSERT(NULL != g_ldo_ctl_tab);
	
	for(i=0; g_ldo_ctl_tab[i].id != LDO_LDO_MAX; i++)
    	{
    		if( g_ldo_ctl_tab[i].id == ldo_id)
    		{
    			ctl = &g_ldo_ctl_tab[i];
    			break;
    		}
    	}
		
	SCI_PASSERT(ctl != NULL, ("ldo_id = %d", ldo_id));

	return ctl;
}

/*****************************************************************************/
//  Description:  Turn on the LDO specified by input parameter ldo_id  
//	Global resource dependence: NONE
//  Author:  Tao.Feng && Yi.Qiu
//	Note:    return value = LDO_ERR_OK if operation is executed successfully           
/*****************************************************************************/
 LDO_ERR_E LDO_TurnOnLDO(LDO_ID_E ldo_id)
{
	unsigned int reg_val;	
	LDO_CTL_PTR ctl = NULL;
	unsigned long flags;
	
	ctl = LDO_GetLdoCtl(ldo_id);
	SCI_PASSERT(ctl != NULL, ("ldo_id = %d", ldo_id));

	if(ctl->bp_reg == NULL){
		if (LDO_LDO_USBD == ldo_id){
			local_irq_save(flags);
			CHIP_REG_AND(GR_CLK_GEN5, (~LDO_USB_PD));
			//__raw_bits_and((~LDO_USB_PD), GR_CLK_GEN5);
			local_irq_restore(flags);
		}
		return LDO_ERR_OK;
	}
	//SCI_DisableIRQ();
	local_irq_save(flags);
	
	SCI_PASSERT(ctl->ref >= 0, ("ctl->ref = %d", ctl->ref));
	if(ctl->ref == 0) 
		REG_SETCLRBIT(ctl->bp_reg, ctl->bp_rst, ctl->bp);

	ctl->ref++;
	
	//SCI_RestoreIRQ();
	local_irq_restore(flags);

	return LDO_ERR_OK;
} 

/*****************************************************************************/
//  Description:  Turo off the LDO specified by parameter ldo_id
//	Global resource dependence: NONE
//  Author: Tao.Feng && Yi.Qiu
//	Note:           
/*****************************************************************************/
 LDO_ERR_E LDO_TurnOffLDO(LDO_ID_E ldo_id)
{
	unsigned int reg_val;
	LDO_CTL_PTR ctl = NULL;
	unsigned long flags;
	
	ctl = LDO_GetLdoCtl(ldo_id);
	SCI_PASSERT(ctl != NULL, ("ldo_id = %d", ldo_id));

	if(ctl->bp_reg == NULL){
		if (LDO_LDO_USBD == ldo_id){
			local_irq_save(flags);
			CHIP_REG_OR(GR_CLK_GEN5, (~LDO_USB_PD));
			//__raw_bits_or((~LDO_USB_PD), GR_CLK_GEN5);
			local_irq_restore(flags);
		}
		return LDO_ERR_OK;
	}
	//SCI_DisableIRQ();
	local_irq_save(flags);
	
        if(ctl->ref > 0)
            ctl->ref--;
	
	if(ctl->ref == 0)
		REG_SETCLRBIT(ctl->bp_reg, ctl->bp, ctl->bp_rst);
	
	//SCI_RestoreIRQ();
	local_irq_restore(flags);
	
	return LDO_ERR_OK;
}

/*****************************************************************************/
//  Description: Find the LDO status -- ON or OFF
//	Global resource dependence: 
//  Author: Tao.Feng && Yi.Qiu
//	Note: return SCI_TRUE means LDO is ON, SCI_FALSE is OFF        
/*****************************************************************************/
 int LDO_IsLDOOn(LDO_ID_E ldo_id)
{
	unsigned int  masked_val = 0;
	LDO_CTL_PTR ctl = NULL;

	ctl = LDO_GetLdoCtl(ldo_id);
	SCI_PASSERT(ctl != NULL, ("ldo_id = %d", ldo_id));

	masked_val = (LDO_REG_GET(ctl->bp_reg) & ctl->bp);

	return (masked_val ? 0 : 1);
}

/*****************************************************************************/
//  Description:  change the LDO voltage level specified by parameter ldo_id
//	Global resource dependence: 
//  Author: Tao.Feng && Yi.Qiu   
//	Note:           
/*****************************************************************************/
 LDO_ERR_E LDO_SetVoltLevel(LDO_ID_E ldo_id, LDO_VOLT_LEVEL_E volt_level)

{
	unsigned int b0_mask,b1_mask;
	LDO_CTL_PTR  ctl = NULL;

	b0_mask = (volt_level & BIT_0)?~0:0;
	b1_mask = (volt_level & BIT_1)?~0:0;

	ctl = LDO_GetLdoCtl(ldo_id);
	SCI_PASSERT(ctl != NULL, ("ldo_id = %d", ldo_id));

	if(ctl->level_reg_b0 == NULL)
	{
		return LDO_ERR_ERR;
	}

	if(ctl->level_reg_b0 == ctl->level_reg_b1)
	{
		SET_LEVEL(ctl->level_reg_b0, b0_mask, b1_mask, ctl->b0, ctl->b0_rst, ctl->b1, ctl->b1_rst);
	}
	else
	{
		SET_LEVELBIT(ctl->level_reg_b0, b0_mask, ctl->b0, ctl->b0_rst);
		SET_LEVELBIT(ctl->level_reg_b1, b1_mask, ctl->b1, ctl->b1_rst);
	}
	
	return LDO_ERR_OK;
}

/*****************************************************************************/
//  Description: Get LDO voltage level
//	Global resource dependence: 
//  Author: Tao.Feng && Yi.Qiu   
//	Note:           
/*****************************************************************************/
 LDO_VOLT_LEVEL_E LDO_GetVoltLevel(LDO_ID_E ldo_id)
{
	unsigned int level_ret = 0;
	LDO_CTL_PTR ctl = NULL;

	ctl = LDO_GetLdoCtl(ldo_id);
	SCI_PASSERT(ctl != NULL, ("ldo_id = %d", ldo_id));

	if(ctl->level_reg_b0 == ctl->level_reg_b1)
	{
		GET_LEVEL(ctl->level_reg_b0, ctl->b0, ctl->b1, level_ret);
	}
	else
	{
		GET_LEVELBIT(ctl->level_reg_b0, ctl->b0, BIT_0, level_ret);
		GET_LEVELBIT(ctl->level_reg_b1, ctl->b1, BIT_1, level_ret);
	}

	return level_ret;
}


/*****************************************************************************/
//  Description:  Shut down any LDO that do not used when system enters deepsleep
//	Global resource dependence: s_ldo_reopen[]
//  Author: Tao.Feng && Yi.Qiu   
//	Note:           
/*****************************************************************************/
void LDO_DeepSleepInit(void)
{
	int i;
	SLP_LDO_CTL_PTR  slp_ldo_ctl_tab;
	
	slp_ldo_ctl_tab = Slp_Ldo_Get_Cfg();

	SCI_ASSERT(NULL != slp_ldo_ctl_tab);

	for(i=0; slp_ldo_ctl_tab[i].id != SLP_LDO_MAX; i++)
    	{
		if(slp_ldo_ctl_tab[i].value == SLP_BIT_SET)
			ANA_REG_OR(slp_ldo_ctl_tab[i].ldo_reg, slp_ldo_ctl_tab[i].mask);
			//__raw_bits_or(slp_ldo_ctl_tab[i].mask, slp_ldo_ctl_tab[i].ldo_reg);
		else
			ANA_REG_AND(slp_ldo_ctl_tab[i].ldo_reg, ~slp_ldo_ctl_tab[i].mask);
			//__raw_bits_and(~slp_ldo_ctl_tab[i].mask, slp_ldo_ctl_tab[i].ldo_reg);
    	}

}

/*****************************************************************************/
//  Description:    this function is used to initialize LDO voltage level.
//	Global resource dependence: 
//  Author: Tao.Feng && Yi.Qiu
//	Note:           
/*****************************************************************************/
int LDO_Init(void)
{
	int i;
	
	g_ldo_ctl_tab = Ldo_Get_Cfg();

	SCI_ASSERT(NULL != g_ldo_ctl_tab);

	for(i=0; g_ldo_ctl_tab[i].id != LDO_LDO_MAX; i++)
    	{
    		if( g_ldo_ctl_tab[i].init_level != LDO_VOLT_LEVEL_MAX)
    			LDO_SetVoltLevel(g_ldo_ctl_tab[i].id, g_ldo_ctl_tab[i].init_level);

		g_ldo_ctl_tab[i].ref = 0;
    	}

	//deepsleep init set for ldo
	LDO_DeepSleepInit();
	
	return LDO_ERR_OK;
}

/*****************************************************************************/
//  Description: turn off system core ldo
//  Global resource dependence:
//  Author: Mingwei.Zhang
//  Note:
/*****************************************************************************/
static void LDO_TurnOffCoreLDO (void)
{
    ANA_REG_SET (ANA_LDO_PD_SET, ANA_LDO_PD_SET_MSK);   /// turn off system core ldo
}

/*****************************************************************************/
//  Description: turn off all module ldo befor system core ldo
//  Global resource dependence:
//  Author: Mingwei.Zhang
//  Note:
/*****************************************************************************/

static void LDO_TurnOffAllModuleLDO (void)
{
    ANA_REG_SET (ANA_LDO_PD_CTL, ANA_LDO_PD_CTL_MSK);               ///turn off all module ldo
    ANA_REG_MSK_OR (ANA_PA_CTL, LDO_PA_SET, (LDO_PA_SET|LDO_PA_RST)); ///PA poweroff
}
/*****************************************************************************/
//  Description:  Shut down all LDO when system poweroff
//  Global resource dependence:
//  Author: Mingwei.Zhang
//  Note:
/*****************************************************************************/
void LDO_TurnOffAllLDO (void)
{
    LDO_TurnOffAllModuleLDO();
    LDO_TurnOffCoreLDO();
}
