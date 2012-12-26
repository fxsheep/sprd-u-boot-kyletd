/******************************************************************************
 ** File Name:      sdhost_drv.c
 ** Author:         Jason.wu
 ** DATE:           09/17/2007
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.
 ** Description:    This file describe operation of SD host.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION
 ** 09/17/2007     Jason.wu        Create.
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "asm/arch/sci_types.h"
#include "asm/arch/os_api.h"
#include "asm/arch/chip_plf_export.h"
#include "sdhost_drv.h"
#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
#include <asm/arch/sdio_reg_v3.h>
#include <asm/arch/int_reg_v3.h>
#include <asm/arch/sys_timer_reg_v0.h>
#include <asm/arch/sc8810_module_config.h>
#endif
#include "asm/arch/ldo.h"

#if defined (CONFIG_SC8810) || defined (CONFIG_SC8825)
#define PLATFORM_SC8800G
#endif
#if defined CONFIG_SC8825 && defined CONFIG_UBOOT_DEBUG
#define SDHOST_PRINT(x) printf x
#else
#define SDHOST_PRINT(x) SCI_TRACE_LOW x
#endif
/*****************************************************************************/
//  Description:   Handle of sdhost
//  Author: Jason.wu
//  Param
//      host_cfg: start register of host controller
//      open_flag: Indicate whether this port has be used by other application
//      baseClock: the basic frequence that host work on
//      sdClock: the frequence that card work on
//      capbility: the function that canbe supported by host
//      err_filter: the error code that application want to watched
//      sigCallBack: the point of application that used to anounce that some signal happened
//  Note:
/*****************************************************************************/
typedef struct SDHOST_PORT_T_TAG
{
    volatile SDIO_REG_CFG *host_cfg;
    BOOLEAN open_flag;
    uint32 baseClock;
    uint32 sdClock;
    SDHOST_CAPBILIT_T capbility;
    uint32 err_filter;
    SDIO_CALLBACK sigCallBack;
} SDHOST_PORT_T;

typedef struct
{
    uint32 msg;
    uint32 errCode;
    SDHOST_SLOT_NO slotNum;
    SDHOST_HANDLE pSdhost_handler;

} ISR_Buffer_T;

INPUT_BUFFER_INIT (ISR_Buffer_T, SDHOST_SLOT_MAX_NUM)

LOCAL SDHOST_PORT_T sdio_port_ctl[SDHOST_SLOT_MAX_NUM];
#ifndef OS_NONE
LOCAL DEVICE_HANDLE s_dev_sdio = SCI_NULL;
#endif

PUBLIC ISR_EXE_T _SDHOST_IrqHandle (uint32 isrnum);
LOCAL void  SdhostHisrFunc (uint32 cnt, void *pData);


/*****************************************************************************/
//  Description:  To confirm whether the handle is valid
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      TRUE the handle is valid
//      FALSE:the handle is not valid
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _RegisterVerifyHOST (SDHOST_HANDLE sdhost_handler)
{
    uint32 index;

    for (index = 0; index < SDHOST_SLOT_MAX_NUM; index++)
    {
        if ( (sdhost_handler == &sdio_port_ctl[index]) && (TRUE == sdio_port_ctl[index].open_flag))
        {
            return TRUE;
        }
    }

    return FALSE;
}

PUBLIC uint32 SDHOST_GetPinState (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    return sdhost_handler->host_cfg->PRESENT_STAT;
}

//---Power Control Register---
/*****************************************************************************/
//  Description:  power on/off led
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      onoff:
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_Led (SDHOST_HANDLE sdhost_handler,SDHOST_LED_ONOFF_E onoff)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    //===
    switch (onoff)
    {
        case SDIO_LED_ON:
            {
                sdhost_handler->host_cfg->HOST_CTL0 |= 0x1;
            }
            break;

        case SDIO_LED_OFF:
            {
                sdhost_handler->host_cfg->HOST_CTL0 &= (~0x1);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

    //===
}

/*****************************************************************************/
//  Description:  set data bus width
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      width: data bus width,only 1bit ,4bit and 8bit canbe used
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_Cfg_BusWidth (SDHOST_HANDLE sdhost_handler,SDHOST_BIT_WIDTH_E width)
{
    uint32 tmpReg =  sdhost_handler->host_cfg->HOST_CTL0;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpReg &= (~ (0x1<<5));
    tmpReg &= (~ (0x1<<1));

    switch (width)
    {
        case SDIO_1BIT_WIDTH:
            {
                //do nothing
            }
            break;

        case SDIO_4BIT_WIDTH:
            {
                tmpReg |= (0x1<<1);
            }
            break;

        case SDIO_8BIT_WIDTH:
            {
                tmpReg |= (0x1<<5);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

    sdhost_handler->host_cfg->HOST_CTL0 = tmpReg;
}

/*****************************************************************************/
//  Description:  set bus speed mode
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      speed: speed mode ,only low speed mode and high speed mode canbe used
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
#if defined (CONFIG_SC8825) || defined(CONFIG_SC7710G2)
PUBLIC void SDHOST_Cfg_SpeedMode (SDHOST_HANDLE sdhost_handler,SDHOST_SPEED_E speed)
{
	uint32 tmpReg = sdhost_handler->host_cfg->HOST_CTL2;
	SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/
	tmpReg &= ~(7<<16);
	switch (speed)
	{
	case EMMC_SDR12:
		break;
	case EMMC_SDR25:
		tmpReg |= 1<<16;
		break;
	case EMMC_SDR50:
		REG32(EMMC_CLK_WR_DL)     = 0x33;
		REG32(EMMC_CLK_RD_POS_DL) = 0x08;
		REG32(EMMC_CLK_RD_NEG_DL) = 0x08;
		tmpReg |= 2<<16;
		break;
	case EMMC_SDR104:
		tmpReg |= 3<<16;
		break;
	case EMMC_DDR50:
		tmpReg |= 4<<16;
		break;	
	default:
		return;
	}
	sdhost_handler->host_cfg->HOST_CTL2 = tmpReg;
}	
#else
PUBLIC void SDHOST_Cfg_SpeedMode (SDHOST_HANDLE sdhost_handler,SDHOST_SPEED_E speed)
{
    uint32 tmpReg =  sdhost_handler->host_cfg->HOST_CTL0;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpReg &= (~ (0x1 <<2));

    switch (speed)
    {
        case SDIO_HIGHSPEED:
            {
                tmpReg |= (0x1<<2);
            }
            break;

        case SDIO_LOWSPEED:
            {
                //do nothing
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

    sdhost_handler->host_cfg->HOST_CTL0 = tmpReg;
}
#endif

/*
typedef enum
{
	LDO_VOLT_LEVEL0=0,
	LDO_VOLT_LEVEL1,
	LDO_VOLT_LEVEL2,
	LDO_VOLT_LEVEL3,
	LDO_VOLT_LEVEL_MAX
}LDO_VOLT_LEVEL_E;

#define LDO_LDO_SDIO 29 */

/*****************************************************************************/
//  Description:  Set operation voltage of card(mmc \sd\sdio card.etc.)
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      voltage:
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_Cfg_Voltage (SDHOST_HANDLE sdhost_handler,SDHOST_VOL_RANGE_E voltage)
{
#if defined (CONFIG_TIGER) || defined (CONFIG_SC7710G2)
    /*<CR:MS00234475 modify for SPI smartphone 30/03/2011 by shengyanxin bagin*/
#if defined (MODEM_CONTROL_SUPPORT_SPI)
    return;
#else
#ifdef PLATFORM_SC8800G
    LDO_VOLT_LEVEL_E ldo_volt_level;
    u32 tempreg;
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    switch (voltage)
    {
        case VOL_1_8:
            {
                ldo_volt_level = LDO_VOLT_LEVEL3;
            }
            break;

        case VOL_2_65:
            {
                ldo_volt_level = LDO_VOLT_LEVEL2;   ///it is 2.5V in sc8800g spec.
            }
            break;

        case VOL_2_8:
            {
                ldo_volt_level = LDO_VOLT_LEVEL0;
            }
            break;

        case VOL_3_0:
            {
                ldo_volt_level = LDO_VOLT_LEVEL1;
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
    LDO_SetVoltLevel (LDO_LDO_SDIO3, LDO_VOLT_LEVEL3);
    LDO_SetVoltLevel (LDO_LDO_VDD30, LDO_VOLT_LEVEL1); 
#else
    LDO_SetVoltLevel (LDO_LDO_SDIO1, LDO_VOLT_LEVEL3);
    
    LDO_SetVoltLevel (LDO_LDO_SIM2, LDO_VOLT_LEVEL1);
#endif
    
    //__udelay(20*1000);
    //gpio_direction_output(139, 1);
    //gpio_set_value(139, 1);
   //control gpio 139 emmc reset pin 
    /*
    *(volatile u32 *)0x8B000008 = 1<<5;
    *(volatile u32 *)0x8A000384 = 1<<15;
    *(volatile u32 *)0x8A000388 = 1<<15;
    *(volatile u32 *)0x8A000380 = 1<<15;
  */ 
    tempreg = *(volatile u32 *)0x8B000008; 
    *(volatile u32 *)0x8B000008 = tempreg | (1<<5);
 
    //*(volatile u32 *)0x8C0003D0 = 0;

#ifdef CONFIG_SC8810_OPENPHONE
            tempreg = *(volatile unsigned int  *)0x8A000384;
            *(volatile unsigned int  *)0x8A000384 = tempreg | (1<<11);
            tempreg = *(volatile unsigned int  *)0x8A000388;
            *(volatile unsigned int  *)0x8A000388 = tempreg | (1<<11);
            tempreg = *(volatile unsigned int  *)0x8A000380;
            *(volatile unsigned int  *)0x8A000380 = tempreg | (1<<11);
#else
	   tempreg = *(volatile unsigned int  *)0x8A000184;
            *(volatile unsigned int  *)0x8A000184 = tempreg | (1<<7);
            tempreg = *(volatile unsigned int  *)0x8A000188;
            *(volatile unsigned int  *)0x8A000188 = tempreg | (1<<7);
            tempreg = *(volatile unsigned int  *)0x8A000180;
            *(volatile unsigned int  *)0x8A000180 = tempreg | (1<<7);
#endif	

#else

    uint32 tmpReg =  sdhost_handler->host_cfg->HOST_CTL0;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpReg &= (~ (0x7<<9));

    switch (voltage)
    {
        case VOL_1_8:
            {
                tmpReg |= (0x05<<9);
            }
            break;

        case VOL_2_65:
            {
                tmpReg |= (0x01<<9);
            }
            break;

        case VOL_2_8:
            {
                tmpReg |= (0x04<<9);
            }
            break;

        case VOL_3_0:
            {
                tmpReg |= (0x07<<9);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

    sdhost_handler->host_cfg->HOST_CTL0 = tmpReg;
#endif
#endif
#endif
}

/*****************************************************************************/
//  Description:  Open or close power supply of card(mmc \sd\sdio card.etc.)
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      on_off:
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SD_POWER (SDHOST_HANDLE sdhost_handler,SDHOST_PWR_ONOFF_E on_off)
{
    /*<CR:MS00234475 modify for SPI smartphone 30/03/2011 by shengyanxin bagin*/
#if defined (MODEM_CONTROL_SUPPORT_SPI) && defined (AP_SOLUTION_INGENIC)
    return;
#else
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/
#if defined(PLATFORM_SC8800G) || defined(PLATFORM_SC6800H)

    ///added by mingwei, Andy need to check again.
    if (POWR_ON == on_off)
    {
        //LDO_TurnOnLDO (LDO_LDO_SDIO);
#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
		LDO_TurnOnLDO(LDO_LDO_SDIO3);
		LDO_TurnOnLDO(LDO_LDO_VDD30);
#else
        LDO_TurnOnLDO(LDO_LDO_SDIO1);
        LDO_TurnOnLDO(LDO_LDO_SIM2);
#endif
    }
    else
    {
        //LDO_TurnOffLDO (LDO_LDO_SDIO);
#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
		LDO_TurnOffLDO(LDO_LDO_SDIO3);
		LDO_TurnOffLDO(LDO_LDO_VDD30);
#else
        LDO_TurnOffLDO (LDO_LDO_SDIO1);
        LDO_TurnOffLDO (LDO_LDO_SIM2);
#endif
    }

#else

    switch (on_off)
    {
        case POWR_ON:
            {
                sdhost_handler->host_cfg->HOST_CTL0 |= (0x1<<8);
            }
            break;

        case POWR_OFF:
            {
                sdhost_handler->host_cfg->HOST_CTL0 &= (~ (0x1<<8));
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

#endif
#endif
}
//---Block Gap Control Register---
/*****************************************************************************/
//  Description:  Set the position of break point
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_BlkGapIntPosSet (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

}

/*****************************************************************************/
//  Description:  Enable pause function of host. the card must support this function,then this function can be worked
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_EnableReadWaitCtl (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL0 |= BIT_18;
}

/*****************************************************************************/
//  Description:  Set break point during the transmition
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_StopAtBlkGap (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL0 |= (BIT_16);
}
PUBLIC void SDHOST_ClearBlkGap (SDHOST_HANDLE sdhost_handler)
{
    sdhost_handler->host_cfg->HOST_CTL0 &= (~ (0x01<<16));
}
PUBLIC void SDHOST_BlkGapIntEn (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler)); /*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL0 |= (BIT_19);
}
PUBLIC uint32 SDHOST_GetTransStat (SDHOST_HANDLE sdhost_handler)
{
    return (sdhost_handler->host_cfg->PRESENT_STAT & (BIT_8|BIT_9));
}

/*****************************************************************************/
//  Description:  When transmission is paused ,this function can resume the transmission
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_Continue (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL0 &= (~ (0x03<<16));
    sdhost_handler->host_cfg->HOST_CTL0 |= BIT_17;

}



//----Clock Control Register---
/*****************************************************************************/
//  Description:  Open or close internal clk.when this clk is disable ,the host will enter in sleep mode
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      onoff:
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_internalClk_OnOff (SDHOST_HANDLE sdhost_handler,SDHOST_CLK_ONOFF_E onoff)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    switch (onoff)
    {
        case CLK_ON:
            {
                //Enable internal clock
                sdhost_handler->host_cfg->HOST_CTL1 |= BIT_0;
            }
            break;

        case CLK_OFF:
            {
                sdhost_handler->host_cfg->HOST_CTL1 &= (~BIT_0);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }
}

/*****************************************************************************/
//  Description:  Open or close card clk.
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      onoff:
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SD_clk_OnOff (SDHOST_HANDLE sdhost_handler,SDHOST_CLK_ONOFF_E onoff)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    switch (onoff)
    {
        case CLK_ON:
            {
                //Enable internal clock
                sdhost_handler->host_cfg->HOST_CTL1 |= BIT_2;
            }
            break;

        case CLK_OFF:
            {
                sdhost_handler->host_cfg->HOST_CTL1 &= (~BIT_2);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }
}

/*****************************************************************************/
//  Description:  Set the frequence of Card clock
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      sdio_clk: the frequence of card working clock
//  Return:
//      uint32 value : the frequency that be used acctually
//  Note:
/*****************************************************************************/
PUBLIC uint32 SDHOST_SD_Clk_Freq_Set (SDHOST_HANDLE sdhost_handler,uint32 sdio_clk)
{
    volatile uint32 tmpReg;
    uint32 clkDiv;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/
    SCI_ASSERT (0 != sdio_clk);/*assert verified*/

    sdio_clk = (sdio_clk > SDIO_SD_MAX_CLK) ? (SDIO_SD_MAX_CLK) : (sdio_clk);

    //SDCLK Frequency Select ,Configure SDCLK select
    clkDiv = sdhost_handler->baseClock/sdio_clk;
#if defined (CONFIG_SC8825) || defined(CONFIG_SC7710G2)
    clkDiv /= 2;
#endif
    if (0 != sdhost_handler->baseClock%sdio_clk)
    {
        clkDiv++;
    }

    SDHOST_PRINT ( ("   clkDiv: %d, sdio_clk:%d, baseClock:%d\n",clkDiv, sdio_clk, sdhost_handler->baseClock));

    tmpReg = sdhost_handler->host_cfg->HOST_CTL1;
#if defined (CONFIG_SC8825) || defined(CONFIG_SC7710G2)
    clkDiv--;
    tmpReg &= (~ (0x3ff<<6));
    tmpReg |= clkDiv&(0x3<<6);
    tmpReg |= (clkDiv&0xff)<<8;
    sdhost_handler->sdClock = sdhost_handler->baseClock/(2*(clkDiv+1));
#else    
    tmpReg &= (~ (0xff<<8));
    if (256 < clkDiv)
    {
        SDHOST_PRINT ( ("   clkDiv: %d is too big!!!!!",clkDiv));
        SCI_ASSERT (0);/*assert to do*/
    }
    else if (128 < clkDiv)
    {
        clkDiv = 256;
        tmpReg |= (0x80 << 8);
    }
    else if (64 < clkDiv)
    {
        clkDiv = 128;
        tmpReg |= (0x40<<8);
    }
    else if (32 < clkDiv)
    {
        clkDiv = 64;
        tmpReg |= (0x20<<8);
    }
    else if (16 < clkDiv)
    {
        clkDiv = 32;
        tmpReg |= (0x10<<8);
    }
    else if (8 < clkDiv)
    {
        clkDiv = 16;
        tmpReg |= (0x8<<8);
    }
    else if (4 < clkDiv)
    {
        clkDiv = 8;
        tmpReg |= (0x4<<8);
    }
    else if (2 < clkDiv)
    {
        clkDiv = 4;
        tmpReg |= (0x2<<8);
    }
    else if (1 < clkDiv)
    {
        clkDiv = 2;
        tmpReg |= (0x1<<8);
    }
    else if (0 < clkDiv)
    {
        clkDiv = 1;
        //nothing
    }
    else //if (0 == clkDiv)
    {
        SCI_ASSERT (0);/*assert to do*/
    }
    sdhost_handler->sdClock = sdhost_handler->baseClock/clkDiv;
#endif

    sdhost_handler->host_cfg->HOST_CTL1 = tmpReg;
    SDHOST_PRINT ( ("sd clk: %d KHz.",sdhost_handler->sdClock/1000));

    return sdhost_handler->sdClock;
}

//---Timeout Control Register--
/*****************************************************************************/
//  Description:  Set timeout value ,this value is used during the data transmission
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      clk_cnt: the value is (2 ^ (clkcnt+13))*T_BSCLK,T_BSCLK is working frequence of host
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SetDataTimeOutValue (SDHOST_HANDLE sdhost_handler,uint8 clk_cnt) // (2 ^ (clkcnt+13))*T_BSCLK
{
    volatile uint32 tmpReg,tmpIntReg;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpIntReg = sdhost_handler->host_cfg->INT_STA_EN;
    // cfg the data timeout clk----------
    sdhost_handler->host_cfg->INT_STA_EN &= ~BIT_20;

    tmpReg = sdhost_handler->host_cfg->HOST_CTL1;
    tmpReg &= ~ (0xF << 16);
    tmpReg |= (clk_cnt << 16);
    sdhost_handler->host_cfg->HOST_CTL1 = tmpReg;

    sdhost_handler->host_cfg->INT_STA_EN = tmpIntReg;
}

//---Software Reset Register---
/*****************************************************************************/
//  Description: Reset data line of host
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL void _Reset_DAT (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL1 |= BIT_26;

    while (0!= (sdhost_handler->host_cfg->HOST_CTL1 & BIT_26)) {}
}
/*****************************************************************************/
//  Description: Reset command line of host
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL void _Reset_CMD (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL1 |= BIT_25;

    while (0!= (sdhost_handler->host_cfg->HOST_CTL1 & BIT_25)) {}
}

/*****************************************************************************/
//  Description: Reset command line and data line of host
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL void _Reset_DAT_CMD (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL1 |= (BIT_25|BIT_26);

    while (0!= (sdhost_handler->host_cfg->HOST_CTL1 & (BIT_25|BIT_26))) {}


}

/*****************************************************************************/
//  Description: Reset all the module of host
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL void _Reset_ALL (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->HOST_CTL1 |= BIT_24;

    while (0 != (sdhost_handler->host_cfg->HOST_CTL1 & BIT_24)) {}

}

/*****************************************************************************/
//  Description: Reset  module of host
//  Author: Wenjun.Shi
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL  SDHOST_Reset_Controller(SDHOST_SLOT_NO slot_NO)
{
#if defined (CONFIG_TIGER)
	REG32 (AHB_CTL0)	  |= BIT_23;
	REG32 (AHB_SOFT_RST) |= BIT_21;
	REG32 (AHB_SOFT_RST) &= ~BIT_21;
#elif defined(CONFIG_SC7710G2)
	REG32 (AHB_CTL6)	  |= BIT_1;
	REG32 (AHB_SOFT2_RST) |= BIT_1;
	REG32 (AHB_SOFT2_RST) &= ~BIT_1;
#else
#define AHB_CTL0_SDIO0_EN	(BIT_4)
#define AHB_CTL0_SDIO1_EN	(BIT_19)

#define AHB_CTL0_SDIO0_RST	(BIT_12)
#define AHB_CTL0_SDIO1_RST	(BIT_16)

	if(slot_NO == SDHOST_SLOT_0)
	{
		REG32 (AHB_CTL0) |= AHB_CTL0_SDIO0_EN;
		REG32 (AHB_SOFT_RST) |= AHB_CTL0_SDIO0_RST;
		REG32 (AHB_SOFT_RST) &= ~AHB_CTL0_SDIO0_RST;
	}else if(slot_NO == SDHOST_SLOT_1)
	{
		REG32 (AHB_CTL0) |= AHB_CTL0_SDIO1_EN;
		REG32 (AHB_SOFT_RST) |= AHB_CTL0_SDIO1_RST;
		REG32 (AHB_SOFT_RST) &= ~AHB_CTL0_SDIO1_RST;
	}
		// select slot 0
#endif

	
}

LOCAL void _Reset_MODULE (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    if(&sdio_port_ctl[SDHOST_SLOT_0] == sdhost_handler){
		SDHOST_Reset_Controller(SDHOST_SLOT_0);
    } else if(&sdio_port_ctl[SDHOST_SLOT_1] == sdhost_handler){
		SDHOST_Reset_Controller(SDHOST_SLOT_1);
    }
}

/*****************************************************************************/
//  Description: Reset the specify module of host
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      rst_type: indicate which module will be reset(command lin\data line\all the module)
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_RST (SDHOST_HANDLE sdhost_handler,SDHOST_RST_TYPE_E rst_type)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    switch (rst_type)
    {
        case RST_CMD_LINE:
            {
                _Reset_CMD (sdhost_handler);
            }
            break;

        case RST_DAT_LINE:
            {
                _Reset_DAT (sdhost_handler);
            }
            break;

        case RST_CMD_DAT_LINE:
            {
                _Reset_DAT_CMD (sdhost_handler);
            }
            break;

        case RST_ALL:
            {
                _Reset_ALL (sdhost_handler);
            }
            break;
            
        case RST_MODULE:
            {
                _Reset_MODULE (sdhost_handler);
            }
            break;
        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }
}

/*****************************************************************************/
//  Description: Set block size \count of block\and Dma Buffer size
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      block_size: size of block( 0 <= block size <=0x0800)
//      block_cnt: the count of block(0 <= block_cnt <= 0xFFFF)
//      DMAsize:buffer size of DMA(4K,8K,16K,32K,64K,128K,256K,512K)
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SetDataParam (SDHOST_HANDLE sdhost_handler,uint32 block_size,uint32 block_cnt,SDHOST_DMA_BUF_SIZE_E DMAsize)
{
    volatile uint32 tmpReg;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/
    SCI_ASSERT (0x1000 >= block_size);/*assert verified*/
    SCI_ASSERT (0xFFFF >= block_cnt);/*assert verified*/

    tmpReg = sdhost_handler->host_cfg->BLK_SIZE_COUNT;

    // Set Block Size
    tmpReg &= (~BIT_15);
    tmpReg &= (~ (0xFFF));
    if (0x1000 == block_size)
    {
        tmpReg |= BIT_15;
    }
    else
    {
        tmpReg |= block_size;
    }

    // Set Block Cnt
    tmpReg &= (~0xFFFF0000);
    tmpReg |= (block_cnt << 16);

    // Set DMA Buf Size
    tmpReg &= (~ (0x07<<12));

    switch (DMAsize)
    {
        case SDIO_DMA_4K:
            {
                //do nothing
            }
            break;

        case SDIO_DMA_8K:
            {
                tmpReg |= (0x01<<12);
            }
            break;

        case SDIO_DMA_16K:
            {
                tmpReg |= (0x02<<12);
            }
            break;

        case SDIO_DMA_32K:
            {
                tmpReg |= (0x03<<12);
            }
            break;

        case SDIO_DMA_64K:
            {
                tmpReg |= (0x04<<12);
            }
            break;

        case SDIO_DMA_128K:
            {
                tmpReg |= (0x05<<12);
            }
            break;

        case SDIO_DMA_256K:
            {
                tmpReg |= (0x06<<12);
            }
            break;

        case SDIO_DMA_512K:
            {
                tmpReg |= (0x07<<12);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;

    }
    sdhost_handler->host_cfg->BLK_SIZE_COUNT = tmpReg;
}

PUBLIC void SDHOST_GetDataParam (SDHOST_HANDLE sdhost_handler,uint32 *block_size,uint32 *block_cnt, uint32 *dmaAddr)
{
    uint32 sizecnt;

    sizecnt = sdhost_handler->host_cfg->BLK_SIZE_COUNT;
    *block_size = sizecnt&0xFFF;
    *block_cnt  = ( (sizecnt&0xFFFF0000) >>16);
    *dmaAddr    = sdhost_handler->host_cfg->DMA_SYS_ADD;
}

/*****************************************************************************/
//  Description: Set start address of DMA buffer
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      dmaAddr: start address of DMA buffer
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SetDmaAddr (SDHOST_HANDLE sdhost_handler, uint32 dmaAddr)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->DMA_SYS_ADD = dmaAddr;

}

/*****************************************************************************/
//  Description: Get stop address of DMA buffer,when buffer is used ,the dma will stop at last address of buffer.this function will get this address
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      uint32 value: address that DMA stoped at
//  Note:
/*****************************************************************************/
PUBLIC uint32 SDHOST_GetDmaAddr (SDHOST_HANDLE sdhost_handler)
{
    uint32 dmaAddr;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    dmaAddr = sdhost_handler->host_cfg->DMA_SYS_ADD;
    return dmaAddr;
}
//---Argument Register---
/*****************************************************************************/
//  Description: Set the argument of command
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      argument
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SetCmdArg (SDHOST_HANDLE sdhost_handler,uint32 argument)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->host_cfg->CMD_ARGUMENT = argument;
}

//---CMD Register---
/*****************************************************************************/
//  Description: Set the mode of command
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      cmdIndex:   command
//      transmode:  transfer mode
//      cmd_type:   comand type ,it may be normal comman ,resume comman etc.
//      Response:   the inspect response from card. if this comman is performed by card successly ,this response will be return by card
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SetCmd (SDHOST_HANDLE sdhost_handler,uint32 cmdIndex,uint32 transmode,SDHOST_CMD_TYPE_E cmd_type,CMD_RSP_TYPE_E Response)
{
    volatile uint32 tmpReg;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpReg = sdhost_handler->host_cfg->CMD_TRANSMODE;
    tmpReg &= (~ (0x7F|0x30000|0x3FF80000));

    if (0 != (TRANS_MODE_ATA_CMPLETE_SIG_EN&transmode))
    {
        tmpReg |= SDIO_TRANS_COMP_ATA;
    }

    if (0 != (TRANS_MODE_MULTI_BLOCK&transmode))
    {
        tmpReg |= SDIO_TRANS_MULTIBLK;
    }

    if (0 != (TRANS_MODE_READ&transmode))
    {
        tmpReg |= SDIO_TRANS_DIR_READ;
    }

    if (0 != (TRANS_MODE_CMD12_EN&transmode))
    {
        tmpReg |= SDIO_TRANS_AUTO_CMD12_EN;
    }

    if (0 != (TRANS_MODE_BLOCK_COUNT_EN&transmode))
    {
        tmpReg |= SDIO_TRANS_BLK_CNT_EN;
    }

    if (0 != (TRANS_MODE_DMA_EN&transmode))
    {
        tmpReg |= SDIO_TRANS_DMA_EN;
    }

    if (0 != (CMD_HAVE_DATA&transmode))
    {
        tmpReg |= SDIO_CMD_DATA_PRESENT;
    }

    switch (cmd_type)
    {
        case CMD_TYPE_NORMAL:
            {
                tmpReg |= SDIO_CMD_TYPE_NML;
            }
            break;

        case CMD_TYPE_SUSPEND:
            {
                tmpReg |= SDIO_CMD_TYPE_SUSPEND;
            }
            break;

        case CMD_TYPE_RESUME:
            {
                tmpReg |= SDIO_CMD_TYPE_RESUME;
            }
            break;

        case CMD_TYPE_ABORT:
            {
                tmpReg |= SDIO_CMD_TYPE_ABORT;
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

    switch (Response)
    {
        case CMD_NO_RSP:
            {
                tmpReg |= SDIO_NO_RSP;
            }
            break;

        case CMD_RSP_R1:
            {
                tmpReg |= SDIO_R1;
            }
            break;

        case CMD_RSP_R2:
            {
                tmpReg |= SDIO_R2;
            }
            break;

        case CMD_RSP_R3:
            {
                tmpReg |= SDIO_R3;
            }
            break;

        case CMD_RSP_R4:
            {
                tmpReg |= SDIO_R4;
            }
            break;

        case CMD_RSP_R5:
            {
                tmpReg |= SDIO_R5;
            }
            break;

        case CMD_RSP_R6:
            {
                tmpReg |= SDIO_R6;
            }
            break;

        case CMD_RSP_R7:
            {
                tmpReg |= SDIO_R7;
            }
            break;

        case CMD_RSP_R1B:
            {
                tmpReg |= SDIO_R1B;
            }
            break;

        case CMD_RSP_R5B:
            {
                tmpReg |= SDIO_R5B;
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;

    }

    tmpReg |= (cmdIndex<<24);

    sdhost_handler->host_cfg->CMD_TRANSMODE = tmpReg;

}


//==
/*****************************************************************************/
//  Description: Get content from host response register
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      Response:   the type of response
//      rspBuf:     the content will be stored in this place
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_GetRspFromBuf (SDHOST_HANDLE sdhost_handler,CMD_RSP_TYPE_E Response,uint8 *rspBuf)
{
    uint32 tmpRspBuf[4];
    uint32 i;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpRspBuf[0] = sdhost_handler->host_cfg->RSP0;
    tmpRspBuf[1] = sdhost_handler->host_cfg->RSP1;
    tmpRspBuf[2] = sdhost_handler->host_cfg->RSP2;
    tmpRspBuf[3] = sdhost_handler->host_cfg->RSP3;

    //  SDHOST_PRINT(("RSP %x %x %x %x",tmpRspBuf[0],tmpRspBuf[1],tmpRspBuf[2],tmpRspBuf[3]));

    for (i = 0; i < 4; i++)
    {
        rspBuf[0+ (i<<2) ] = (uint8) ( (tmpRspBuf[i]>>24) &0xFF);
        rspBuf[1+ (i<<2) ] = (uint8) ( (tmpRspBuf[i]>>16) &0xFF);
        rspBuf[2+ (i<<2) ] = (uint8) ( (tmpRspBuf[i]>>8) &0xFF);
        rspBuf[3+ (i<<2) ] = (uint8) (tmpRspBuf[i]&0xFF);
    }

    switch (Response)
    {
        case CMD_NO_RSP:
            break;

        case CMD_RSP_R1:
        case CMD_RSP_R1B:
        case CMD_RSP_R3:
        case CMD_RSP_R4:
        case CMD_RSP_R5:
        case CMD_RSP_R6:
        case CMD_RSP_R7:
        case CMD_RSP_R5B:
            {
                rspBuf[0] = (uint8) ( (tmpRspBuf[0]>>24) &0xFF);
                rspBuf[1] = (uint8) ( (tmpRspBuf[0]>>16) &0xFF);
                rspBuf[2] = (uint8) ( (tmpRspBuf[0]>>8) &0xFF);
                rspBuf[3] = (uint8) (tmpRspBuf[0]&0xFF);
            }
            break;

        case CMD_RSP_R2:
            {
                rspBuf[0] = (uint8) ( (tmpRspBuf[3]>>16) &0xFF);
                rspBuf[1] = (uint8) ( (tmpRspBuf[3]>>8) &0xFF);
                rspBuf[2] = (uint8) (tmpRspBuf[3]&0xFF);

                rspBuf[3] = (uint8) ( (tmpRspBuf[2]>>24) &0xFF);
                rspBuf[4] = (uint8) ( (tmpRspBuf[2]>>16) &0xFF);
                rspBuf[5] = (uint8) ( (tmpRspBuf[2]>>8) &0xFF);
                rspBuf[6] = (uint8) (tmpRspBuf[2]&0xFF);

                rspBuf[7] = (uint8) ( (tmpRspBuf[1]>>24) &0xFF);
                rspBuf[8] = (uint8) ( (tmpRspBuf[1]>>16) &0xFF);
                rspBuf[9] = (uint8) ( (tmpRspBuf[1]>>8) &0xFF);
                rspBuf[10] = (uint8) (tmpRspBuf[1]&0xFF);

                rspBuf[11] = (uint8) ( (tmpRspBuf[0]>>24) &0xFF);
                rspBuf[12] = (uint8) ( (tmpRspBuf[0]>>16) &0xFF);
                rspBuf[13] = (uint8) ( (tmpRspBuf[0]>>8) &0xFF);
                rspBuf[14] = (uint8) (tmpRspBuf[0]&0xFF);
            }
            break;

        default:
            {
                SCI_ASSERT (0);/*assert verified*/
            }
            break;
    }

    return;
}


//====

//----
/*****************************************************************************/
//  Description: Get function that host can be support
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      capbility
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL void _GetSDHOSTCapbility (SDHOST_HANDLE sdhost_handler,SDHOST_CAPBILIT_T *capbility)
{
    volatile uint32 tmpReg;
    SCI_MEMSET (capbility,0,sizeof (SDHOST_CAPBILIT_T));

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpReg = sdhost_handler->host_cfg->CAPBILITY;

    if (0 != (tmpReg &  BIT_26))
    {
        capbility->capbility_function |= CAP_VOL_1_8V;
    }

    if (0 != (tmpReg &  BIT_25))
    {
        capbility->capbility_function |= CAP_VOL_3_0V;
    }

    if (0 != (tmpReg &  BIT_24))
    {
        capbility->capbility_function |= CAP_VOL_3_3V;
    }

    if (0 != (tmpReg &  BIT_23))
    {
        capbility->capbility_function |= SPD_RESU;
    }

    if (0 != (tmpReg &  BIT_22))
    {
        capbility->capbility_function |= DMA_SPT;
    }

    if (0 != (tmpReg &  BIT_21))
    {
        capbility->capbility_function |= HIGH_SPEED;
    }

    switch (tmpReg & (0x3<<16))
    {
        case (0x00<<16) :
            capbility->cability_Max_BlkLen = 512;
            break;

        case (0x01<<16) :
            capbility->cability_Max_BlkLen = 1024;
            break;

        case (0x02<<16) :
            capbility->cability_Max_BlkLen = 2048;
            break;

        case (0x03<<16) :
            capbility->cability_Max_BlkLen = 4096;
            break;

        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    capbility->sd_Base_Max_Clk = ( (tmpReg & (0x3F<<8)) >>8) * 1000000;

    if (0!= (tmpReg & BIT_7))
    {
        capbility->timeOut_Clk_unit = 1000000;
    }
    else
    {
        capbility->timeOut_Clk_unit = 1000;
    }

    capbility->timeOut_Base_Clk = (tmpReg & 0x3F) *capbility->timeOut_Clk_unit;

    tmpReg = sdhost_handler->host_cfg->CURR_CAPBILITY;
    capbility->max_current_for_1_8 = ( (tmpReg & (0xFF<<16)) >>16);
    capbility->max_current_for_3_0 = ( (tmpReg & (0xFF<<8)) >>8);
    capbility->max_current_for_3_3 = (tmpReg & 0xFF);

    SDHOST_PRINT ( ("capbility_function = %x",sdhost_handler->capbility.capbility_function));
    SDHOST_PRINT ( ("cability_Max_BlkLen = %x",sdhost_handler->capbility.cability_Max_BlkLen));
    SDHOST_PRINT ( ("sd_Base_Max_Clk = %x",sdhost_handler->capbility.sd_Base_Max_Clk));
    SDHOST_PRINT ( ("timeOut_Clk_unit = %x",sdhost_handler->capbility.timeOut_Clk_unit));
    SDHOST_PRINT ( ("timeOut_Base_Clk = %x",sdhost_handler->capbility.timeOut_Base_Clk));
    SDHOST_PRINT ( ("max_current_for_1_8 = %x",sdhost_handler->capbility.max_current_for_1_8));
    SDHOST_PRINT ( ("max_current_for_3_0 = %x",sdhost_handler->capbility.max_current_for_3_0));
    SDHOST_PRINT ( ("max_current_for_3_3 = %x",sdhost_handler->capbility.max_current_for_3_3));
}


/*****************************************************************************/
//  Description: Clear Normal int Status register ,if event is happed ,host will Set status in register
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      err_msg:        that int you want to clear
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_NML_IntStatus_Clr (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntStatus_Clr (sdhost_handler,ERR_ALL);
        tmpReg |= (BIT_28|BIT_24|BIT_23|BIT_22|BIT_21|
                   BIT_20|BIT_19|BIT_18|BIT_17|BIT_16);

    }

    if (0 != (msg&SIG_CARD_IN))
    {
        //sdhost_handler->host_cfg->INT_STA =BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
		tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
		tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
		tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
		tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
		tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
		tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
		tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
		tmpReg |= BIT_0;
    }
    sdhost_handler->host_cfg->INT_STA = tmpReg;

}

/*****************************************************************************/
//  Description: Enable Normal int Signal register ,
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      err_msg:        that int you want to Enable
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_NML_IntStatus_En (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;
    uint32 err_msg;
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntStatus_En (sdhost_handler,sdhost_handler->err_filter);
        err_msg = sdhost_handler->err_filter;
        if (0 != (err_msg&ERR_RSP))
        {
    		tmpReg |= BIT_28;
        }

        if (0 != (err_msg&ERR_CMD12))
        {
    		tmpReg |= BIT_24;
        }

        if (0 != (err_msg&ERR_CUR_LIMIT))
        {
    		tmpReg |= BIT_23;
        }

        if (0 != (err_msg&ERR_DATA_END))
        {
    		tmpReg |= BIT_22;
        }

        if (0 != (err_msg&ERR_DATA_CRC))
        {
    		tmpReg |= BIT_21;
        }

        if (0 != (err_msg&ERR_DATA_TIMEOUT))
        {
    		tmpReg |= BIT_20;
        }

        if (0 != (err_msg&ERR_CMD_INDEX))
        {
    		tmpReg |= BIT_19;
        }

        if (0 != (err_msg&ERR_CMD_END))
        {
    		tmpReg |= BIT_18;
        }

        if (0 != (err_msg&ERR_CMD_CRC))
        {
    		tmpReg |= BIT_17;
        }

        if (0 != (err_msg&ERR_CMD_TIMEOUT))
        {
    		tmpReg |= BIT_16;
        }

    }

    if (0 != (msg&SIG_CARD_IN))
    {
		tmpReg |= BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
		tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
		tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
		tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
		tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
		tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
		tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
		tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
		tmpReg |= BIT_0;
    }
    sdhost_handler->host_cfg->INT_STA_EN |= tmpReg;

}

/*****************************************************************************/
//  Description: Disable Normal int Signal register
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      err_msg:        that int you want to Disable
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_NML_IntStatus_Dis (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntStatus_Dis (sdhost_handler,ERR_ALL);
        tmpReg |= (BIT_28|BIT_24|BIT_23|BIT_22|BIT_21|
                   BIT_20|BIT_19|BIT_18|BIT_17|BIT_16);

    }

    if (0 != (msg&SIG_CARD_IN))
    {
		tmpReg |= BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
		tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
		tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
		tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
		tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
		tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
		tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
		tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
		tmpReg |= BIT_0;
    }
    sdhost_handler->host_cfg->INT_STA_EN &= ~tmpReg;

}

/*****************************************************************************/
//  Description: Enable Normal int Signal register ,if normal event  is happed ,host will send interrupt to Arm
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      err_msg:        that int you want to Enable
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_NML_IntSig_En (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;
    uint32 err_msg;
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntSig_En (sdhost_handler,sdhost_handler->err_filter);
        err_msg = sdhost_handler->err_filter;
        if (0 != (err_msg&ERR_RSP))
        {
    		tmpReg |= BIT_28;
        }

        if (0 != (err_msg&ERR_CMD12))
        {
    		tmpReg |= BIT_24;
        }

        if (0 != (err_msg&ERR_CUR_LIMIT))
        {
    		tmpReg |= BIT_23;
        }

        if (0 != (err_msg&ERR_DATA_END))
        {
    		tmpReg |= BIT_22;
        }

        if (0 != (err_msg&ERR_DATA_CRC))
        {
    		tmpReg |= BIT_21;
        }

        if (0 != (err_msg&ERR_DATA_TIMEOUT))
        {
    		tmpReg |= BIT_20;
        }

        if (0 != (err_msg&ERR_CMD_INDEX))
        {
    		tmpReg |= BIT_19;
        }

        if (0 != (err_msg&ERR_CMD_END))
        {
    		tmpReg |= BIT_18;
        }

        if (0 != (err_msg&ERR_CMD_CRC))
        {
    		tmpReg |= BIT_17;
        }

        if (0 != (err_msg&ERR_CMD_TIMEOUT))
        {
    		tmpReg |= BIT_16;
        }

    }

    if (0 != (msg&SIG_CARD_IN))
    {
		tmpReg |= BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
		tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
		tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
		tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
		tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
		tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
		tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
		tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
		tmpReg |= BIT_0;
    }
	sdhost_handler->host_cfg->INT_SIG_EN |= tmpReg;

}

/*****************************************************************************/
//  Description: Disable Normal int Signal register ,if normal event  is happed ,host will not send interrupt to Arm
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      err_msg:        that int you want to Disable
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_NML_IntSig_Dis (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    volatile uint32 tmpReg = 0;
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    if (0 != (msg&SIG_ERR))
    {
        //_ERR_IntSig_Dis (sdhost_handler,ERR_ALL);
        tmpReg |= (BIT_28|BIT_24|BIT_23|BIT_22|BIT_21|
                   BIT_20|BIT_19|BIT_18|BIT_17|BIT_16);

    }

    if (0 != (msg&SIG_CARD_IN))
    {
		tmpReg |= BIT_8;
    }

    if (0 != (msg&SIG_CARD_INSERT))
    {
		tmpReg |= BIT_6;
    }

    if (0 != (msg&SIG_CARD_REMOVE))
    {
		tmpReg |= BIT_7;
    }

    if (0 != (msg&SIG_BUF_RD_RDY))
    {
		tmpReg |= BIT_5;
    }

    if (0 != (msg&SIG_BUF_WD_RDY))
    {
		tmpReg |= BIT_4;
    }

    if (0 != (msg&SIG_DMA_INT))
    {
		tmpReg |= BIT_3;
    }

    if (0 != (msg&SIG_BLK_CAP))
    {
		tmpReg |= BIT_2;
    }

    if (0 != (msg&SIG_TRANS_CMP))
    {
		tmpReg |= BIT_1;
    }

    if (0 != (msg&SIG_CMD_CMP))
    {
		tmpReg |= BIT_0;
    }
	sdhost_handler->host_cfg->INT_SIG_EN &= ~tmpReg;

}

/*****************************************************************************/
//  Description: Get normal int status register ,to confirm which normal event has happened
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      uint32 value: indicate which event happened
//  Note:
/*****************************************************************************/
PUBLIC uint32 SDHOST_GetNMLIntStatus (SDHOST_HANDLE sdhost_handler)
{
    volatile uint32 tmpReg;
    uint32 msg;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpReg = sdhost_handler->host_cfg->INT_STA;
    msg = 0;

    if (0 != (tmpReg & BIT_15))
    {
        msg |= SIG_ERR;
    }

    if (0 != (tmpReg & BIT_8))
    {
        msg |= SIG_CARD_IN;
    }

    if (0 != (tmpReg & BIT_6))
    {
        msg |= SIG_CARD_INSERT;
    }

    if (0 != (tmpReg & BIT_7))
    {
        msg |= SIG_CARD_REMOVE;
    }

    if (0 != (tmpReg & BIT_5))
    {
        msg |= SIG_BUF_RD_RDY;
    }

    if (0 != (tmpReg & BIT_4))
    {
        msg |= SIG_BUF_WD_RDY;
    }

    if (0 != (tmpReg & BIT_3))
    {
        msg |= SIG_DMA_INT;
    }

    if (0 != (tmpReg & BIT_2))
    {
        msg |= SIG_BLK_CAP;
    }

    if (0 != (tmpReg & BIT_1))
    {
        msg |= SIG_TRANS_CMP;
    }

    if (0 != (tmpReg & BIT_0))
    {
        msg |= SIG_CMD_CMP;
    }

    return msg;
}


/*****************************************************************************/
//  Description: if error interrupt happened ,this function is used to confirm which error event happened
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      uint32 value: indicate which error event happened
//  Note:
/*****************************************************************************/
PUBLIC uint32 SDHOST_GetErrCode (SDHOST_HANDLE sdhost_handler)
{
    volatile uint32 tmpReg;
    uint32 err_msg;

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    tmpReg = sdhost_handler->host_cfg->INT_STA;
    err_msg = 0;

    if (0 != (tmpReg & BIT_28))
    {
        err_msg |= ERR_RSP;
    }

    if (0 != (tmpReg & BIT_24))
    {
        err_msg |= ERR_CMD12;
    }

    if (0 != (tmpReg & BIT_23))
    {
        err_msg |= ERR_CUR_LIMIT;
    }

    if (0 != (tmpReg & BIT_22))
    {
        err_msg |= ERR_DATA_END;
    }

    if (0 != (tmpReg & BIT_21))
    {
        err_msg |= ERR_DATA_CRC;
    }

    if (0 != (tmpReg & BIT_20))
    {
        err_msg |= ERR_DATA_TIMEOUT;
    }

    if (0 != (tmpReg & BIT_19))
    {
        err_msg |= ERR_CMD_INDEX;
    }

    if (0 != (tmpReg & BIT_18))
    {
        err_msg |= ERR_CMD_END;
    }

    if (0 != (tmpReg & BIT_17))
    {
        err_msg |= ERR_CMD_CRC;
    }

    if (0 != (tmpReg & BIT_16))
    {
        err_msg |= ERR_CMD_TIMEOUT;
    }

    return err_msg;
}

/*****************************************************************************/
//  Description: this function is used to set which error event you want to watched ,other error event will be ignored if happened
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      err_msg:        the event you want to watched
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDHOST_SetErrCodeFilter (SDHOST_HANDLE sdhost_handler,uint32 err_msg)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    sdhost_handler->err_filter = err_msg;
}

/*****************************************************************************/
//  Description: To indicate event that happened from which slot
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//  Return:
//      uint32 value: indicate which slot event happened
//  Note:
/*****************************************************************************/
LOCAL SDHOST_SLOT_NO _GetIntSDHOSTSlotNum (uint32 port)
{
    uint32 tmpReg;
    SDHOST_SLOT_NO ret;
#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
    tmpReg = REG32 (EMMC_SLOT_INT_STS);
#else
    if(port == 0){
        tmpReg = REG32 (SDIO0_SLOT_INT_STS);
    } 
    else 
    {
        tmpReg = REG32 (SDIO1_SLOT_INT_STS);
    }
#endif

    if ( (tmpReg& (0x01<<0)))
    {
#if defined(CONFIG_TIGER) || defined (CONFIG_SC7710G2)
        ret = SDHOST_SLOT_7;
#else
        ret = SDHOST_SLOT_1;  //tbd....
#endif
    }
    else if ( (tmpReg& (0x01<<1)))
    {
        ret = SDHOST_SLOT_1;
    }
    else if ( (tmpReg& (0x01<<2)))
    {
        ret = SDHOST_SLOT_2;
    }
    else if ( (tmpReg& (0x01<<3)))
    {
        ret = SDHOST_SLOT_3;
    }
    else if ( (tmpReg& (0x01<<4)))
    {
        ret = SDHOST_SLOT_4;
    }
    else if ( (tmpReg& (0x01<<5)))
    {
        ret = SDHOST_SLOT_5;
    }
    else if ( (tmpReg& (0x01<<6)))
    {
        ret = SDHOST_SLOT_6;
    }
    else if ( (tmpReg& (0x01<<7)))
    {
        ret = SDHOST_SLOT_7;
    }
    else
    {
        ret = SDHOST_SLOT_MAX_NUM;
        SCI_ASSERT (0);/*assert to do*/
    }

    return ret;
}

/*****************************************************************************/
//  Description: This function is called by interrupt service .is event happened ,this funtion will
//  clear the event and call the callback of application
//  Author: Jason.wu
//  Param
//      isrnum: the number of arm interrupt
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC ISR_EXE_T _SDHOST_IrqHandle (uint32 isrnum)
{
    ISR_Buffer_T buffer;
    SDHOST_HANDLE sdhost_handler;

    buffer.slotNum = _GetIntSDHOSTSlotNum(isrnum);
    sdhost_handler = &sdio_port_ctl[buffer.slotNum];
    buffer.pSdhost_handler = &sdio_port_ctl[buffer.slotNum];

    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    if (FALSE == sdhost_handler->open_flag)
    {
        SDHOST_NML_IntSig_Dis (sdhost_handler, SIG_ALL);
        SDHOST_NML_IntStatus_Dis (sdhost_handler, SIG_ALL);
        SDHOST_NML_IntStatus_Clr (sdhost_handler, SIG_ALL);
        return ISR_DONE;
    }

    if (NULL == sdhost_handler->sigCallBack)
    {
        SDHOST_NML_IntSig_Dis (sdhost_handler, SIG_ALL);
        SDHOST_NML_IntStatus_Dis (sdhost_handler, SIG_ALL);
        SDHOST_NML_IntStatus_Clr (sdhost_handler, SIG_ALL);
        return ISR_DONE;
    }

    buffer.msg = SDHOST_GetNMLIntStatus (sdhost_handler);
    buffer.errCode = SDHOST_GetErrCode (sdhost_handler);

    SDHOST_NML_IntSig_Dis (sdhost_handler, buffer.msg);
    SDHOST_NML_IntStatus_Dis (sdhost_handler, buffer.msg);
    SDHOST_NML_IntStatus_Clr (sdhost_handler, buffer.msg);
#ifndef OS_NONE    	
    IsrWriteBuffer (buffer);
#else
	sdhost_handler->sigCallBack (buffer.msg, buffer.errCode, buffer.slotNum);
#endif

    return CALL_HISR;
}

/*****************************************************************************/
//  Description:    This function is SDIO 's HISR.
//                  1. THE priority is higher than normal task.
//                  2. Is not real ISR.
//  Author:         steve.zhan
//  Note:
/*****************************************************************************/
LOCAL void  SdhostHisrFunc (uint32 cnt, void *pData)
{
    ISR_Buffer_T buffer;

    while (!threadReadBuffer (&buffer))
    {
        buffer.pSdhost_handler->sigCallBack (buffer.msg, buffer.errCode, buffer.slotNum);
    }
}

LOCAL void _SDHOST_Pin_select(SDHOST_SLOT_NO slot_NO)
{
    if(slot_NO == SDHOST_SLOT_1){
        *(volatile uint32*)(0x8c0003e8) = 0x280;    //SD1 CMD pullup drv3, strongest strength
        *(volatile uint32*)(0x8c0003ec) = 0x280;    //SD1 D0 pullup drv3, strongest strength
        *(volatile uint32*)(0x8c0003f0) = 0x280;    //SD1 D1 pullup drv3, strongest strength
        *(volatile uint32*)(0x8c0003f4) = 0x280;    //SD1 D2 pullup drv3, strongest strength
        *(volatile uint32*)(0x8c0003f8) = 0x280;    //SD1 D3 pullup drv3, strongest strength
        *(volatile uint32*)(0x8c0003fc) = 0x280;    //SD1 D3 pullup drv3, strongest strength
    }
}

/*****************************************************************************/
//  Description: Regist host slot
//  Author: Jason.wu
//  Param
//      slot_NO:    which slot you want to used
//      fun:        this function will be called when event happened
//  Return:
//      Not zero: succes
//      zeror: fail
//  Note:
/*****************************************************************************/
PUBLIC SDHOST_HANDLE SDHOST_Register (SDHOST_SLOT_NO slot_NO,SDIO_CALLBACK fun)
{
    uint32 status = 0, i = 0;

    SCI_ASSERT (slot_NO < SDHOST_SLOT_MAX_NUM);/*assert verified*/

    if (TRUE == sdio_port_ctl[slot_NO].open_flag)
    {
        return NULL;
    }

    // select slot 0
#if defined (CONFIG_TIGER)
//    SDHOST_Slot_select(slot_NO); if necessary
	sdio_port_ctl[slot_NO].open_flag = TRUE;
	sdio_port_ctl[slot_NO].baseClock = SDHOST_BaseClk_Set (slot_NO,SDIO_BASE_CLK_384M);
#elif defined(CONFIG_SC7710G2)
//    SDHOST_Slot_select(slot_NO); if necessary
	sdio_port_ctl[slot_NO].open_flag = TRUE;
	sdio_port_ctl[slot_NO].baseClock = SDHOST_BaseClk_Set (slot_NO,SDIO_BASE_CLK_384M);
#else
    _SDHOST_Pin_select(slot_NO);
	sdio_port_ctl[slot_NO].open_flag = TRUE;
	sdio_port_ctl[slot_NO].baseClock = SDHOST_BaseClk_Set (slot_NO,SDIO_BASE_CLK_48M);
#endif

    switch (slot_NO)
    {
        case SDHOST_SLOT_0:
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) SDIO0_BASE_ADDR);
            }
            break;

        case SDHOST_SLOT_1:
            {
#if defined(CONFIG_TIGER)	|| defined(CONFIG_SC7710G2)		
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) (SDIO0_BASE_ADDR+0x100) );
#else
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) SDIO1_BASE_ADDR);
#endif				
            }
            break;

        case SDHOST_SLOT_2:
#if defined(CONFIG_TIGER)	|| defined(CONFIG_SC7710G2)		
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) (SDIO0_BASE_ADDR+0x200) );
            }
            break;
#endif			
        case SDHOST_SLOT_3:
#if defined(CONFIG_TIGER)	|| defined(CONFIG_SC7710G2)		
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) SDIO1_BASE_ADDR );
            }
            break;
#endif			
        case SDHOST_SLOT_4:
#if defined(CONFIG_TIGER)	|| defined(CONFIG_SC7710G2)	
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) (SDIO1_BASE_ADDR+0x100) );
            }
            break;
#endif			
        case SDHOST_SLOT_5:
#if defined(CONFIG_TIGER)	|| defined(CONFIG_SC7710G2)	
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) (SDIO1_BASE_ADDR+0x200) );
            }
            break;
#endif			
        case SDHOST_SLOT_6:
#if defined(CONFIG_TIGER)	|| defined(CONFIG_SC7710G2)			
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) SDIO2_BASE_ADDR );
            }
            break;
#endif			
        case SDHOST_SLOT_7:
#if defined(CONFIG_TIGER)	|| defined(CONFIG_SC7710G2)		
            {
                sdio_port_ctl[slot_NO].host_cfg = (SDIO_REG_CFG *) ( (volatile uint32 *) EMMC_BASE_ADDR);
            }
            break;
#endif
        default:
            {
                SCI_ASSERT (0);/*assert to do*/
            }
            break;
    }

    sdio_port_ctl[slot_NO].sigCallBack = fun;
    sdio_port_ctl[slot_NO].err_filter = 0;
    SDHOST_RST (&sdio_port_ctl[slot_NO],RST_ALL);
    SDHOST_internalClk_OnOff (&sdio_port_ctl[slot_NO],CLK_ON);
    _GetSDHOSTCapbility (&sdio_port_ctl[slot_NO],& (sdio_port_ctl[slot_NO].capbility));
    SDHOST_RST (&sdio_port_ctl[slot_NO],RST_ALL);
#ifndef OS_NONE
    status = ISR_RegHandler_Ex (TB_SDIO_INT, (TB_ISR) _SDHOST_IrqHandle, SdhostHisrFunc, CHIPDRV_HISR_PRIO_1, NULL);
    if (TB_SUCCESS == status)
    {
        CHIPDRV_EnableIRQINT (TB_SDIO_INT);
    }
#endif
    return &sdio_port_ctl[slot_NO];
}

/*****************************************************************************/
//  Description: Free the slot resource
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handler of host driver ,this is returned by SDHOST_Register
//  Return:
//      Not zero: succes
//      zeror: fail
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN SDHOST_UnRegister (SDHOST_HANDLE sdhost_handler)
{
    SCI_ASSERT (TRUE == _RegisterVerifyHOST (sdhost_handler));/*assert verified*/

    SDHOST_SD_POWER (sdhost_handler,POWR_OFF);
    SDHOST_RST (sdhost_handler,RST_ALL);
    SDHOST_SD_clk_OnOff (sdhost_handler,CLK_OFF);
    SDHOST_internalClk_OnOff (sdhost_handler,CLK_OFF);
    sdhost_handler->sigCallBack = NULL;
    sdhost_handler->open_flag = FALSE;
    return TRUE;
}

//====================
//---External function---
//====================
/*****************************************************************************/
//  Description: Set basic clk of host,card clk will divided from this clk
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handler of host driver ,this is returned by SDHOST_Register
//      sdio_base_clk: the frequecy that you want to set
//  Return:
//      uint32 value :the frequency that be used acctually
//  Note: This function must be applied according different platform
/*****************************************************************************/
#ifndef OS_NONE
#if 0
PUBLIC uint32 SDHOST_BaseClk_Set (uint32 sdio_base_clk)
{
#ifdef  PLATFORM_SC8800G
    uint32 clk = 0;

    //Select the clk source of SDIO
    if (sdio_base_clk >= SDIO_BASE_CLK_96M)
    {
        DEVICE_SetClock (s_dev_sdio, SDIO_BASE_CLK_96M);
        clk = SDIO_BASE_CLK_96M;
    }
    else if (sdio_base_clk >= SDIO_BASE_CLK_64M)
    {
        clk = SDIO_BASE_CLK_64M;
        DEVICE_SetClock (s_dev_sdio, SDIO_BASE_CLK_64M);

    }
    else if (sdio_base_clk >= SDIO_BASE_CLK_48M)
    {
        clk = SDIO_BASE_CLK_48M;
        DEVICE_SetClock (s_dev_sdio, SDIO_BASE_CLK_48M);
    }
    else
    {
        clk = SDIO_BASE_CLK_26M;
        DEVICE_SetClock (s_dev_sdio, SDIO_BASE_CLK_26M);
    }

    return clk;
#endif
}
#endif
#else
PUBLIC uint32 SDHOST_BaseClk_Set(SDHOST_SLOT_NO slot_NO,uint32 sdio_base_clk)

{
#if defined(CONFIG_TIGER)
    uint32 clk = 0;
    REG32 (GR_CLK_GEN5) &= ~ (BIT_23|BIT_24);
    //Select the clk source of SDIO
    if (sdio_base_clk >= SDIO_BASE_CLK_384M)
    {
        clk = SDIO_BASE_CLK_384M;
    }
    else if (sdio_base_clk >= SDIO_BASE_CLK_256M)
    {
        clk = SDIO_BASE_CLK_256M;
        REG32 (GR_CLK_GEN5) |= (1<<23);
    }
    else if (sdio_base_clk >= SDIO_BASE_CLK_153M)
    {
        clk = SDIO_BASE_CLK_153M;
        REG32 (GR_CLK_GEN5) |= (2<<23);
    }
    else
    {
        clk = SDIO_BASE_CLK_26M;
        REG32 (GR_CLK_GEN5) |= (3<<23);
    }
    return clk;
#elif defined(CONFIG_SC7710G2)
    uint32 clk = 0;
    REG32 (GR_CLK_GEN7) &= ~ (BIT_23|BIT_24);
    //Select the clk source of SDIO
    if (sdio_base_clk >= SDIO_BASE_CLK_384M)
    {
        clk = SDIO_BASE_CLK_384M;
         REG32 (GR_CLK_GEN7) |= (1<<23);
    }
    else if (sdio_base_clk >= SDIO_BASE_CLK_256M)
    {
        clk = SDIO_BASE_CLK_256M;
        REG32 (GR_CLK_GEN7) |= (1<<24);
    }
    else if (sdio_base_clk >= SDIO_BASE_CLK_153M)
    {
        clk = SDIO_BASE_CLK_153M;
        REG32 (GR_CLK_GEN5) |= (3<<23);
    }
    else
    {
        clk = SDIO_BASE_CLK_26M;
    }
    return clk;
#elif defined(PLATFORM_SC8800G)
    uint32 clk = 0;

    //Select the clk source of SDIO1
    if(SDHOST_SLOT_1 == slot_NO){
    	*(volatile uint32 *)GR_CLK_GEN5 &= ~(BIT_19|BIT_20);
    	if(sdio_base_clk >= SDIO_BASE_CLK_96M)
        	clk = SDIO_BASE_CLK_96M;
    	else if(sdio_base_clk >= SDIO_BASE_CLK_64M){
        	clk = SDIO_BASE_CLK_64M;
        	*(volatile uint32 *)GR_CLK_GEN5 |= (1<<19);
    	} else if(sdio_base_clk >= SDIO_BASE_CLK_48M) {
        	clk = SDIO_BASE_CLK_48M;
        	*(volatile uint32 *)GR_CLK_GEN5 |= (2<<19);
    	} else {
        	clk = SDIO_BASE_CLK_26M;
        	*(volatile uint32 *)GR_CLK_GEN5 |= (3<<19);
    	}
    } else if(SDHOST_SLOT_0 == slot_NO){
    	*(volatile uint32 *)GR_CLK_GEN5 &= ~(BIT_17|BIT_18);
    	if(sdio_base_clk >= SDIO_BASE_CLK_96M)
        	clk = SDIO_BASE_CLK_96M;
    	else if(sdio_base_clk >= SDIO_BASE_CLK_64M){
        	clk = SDIO_BASE_CLK_64M;
        	*(volatile uint32 *)GR_CLK_GEN5 |= (1<<17);
    	} else if(sdio_base_clk >= SDIO_BASE_CLK_48M) {
        	clk = SDIO_BASE_CLK_48M;
        	*(volatile uint32 *)GR_CLK_GEN5 |= (2<<17);
    	} else {
        	clk = SDIO_BASE_CLK_26M;
        	*(volatile uint32 *)GR_CLK_GEN5 |= (3<<17);
    	}
    } 
    return clk;
#endif
}

#endif

/*****************************************************************************/
//  Description: select witch slot to work
//  Author: Jason.wu
//  Param
//      slot_NO: slot number
//  Return:
//      NONE
//  Note: This function must be applied according different platform
/*****************************************************************************/
PUBLIC void SDHOST_Slot_select (SDHOST_SLOT_NO slot_NO)
{
	SCI_ASSERT (slot_NO < SDHOST_SLOT_MAX_NUM);/*assert verified*/

	* (volatile uint32 *) AHB_SDIO_CTL = (AHB_SDIO_CTRL_SLOT0); //select master0
}

//===end===

