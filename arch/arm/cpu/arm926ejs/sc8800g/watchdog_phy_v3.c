/******************************************************************************
 ** File Name:      watchdog_phy_v3.c                                                 *
 ** Author:         Jie Dai                                                   *
 ** DATE:           08/02/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the physical layer of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/02/2010     Jie Dai            Create.                                 *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <asm/arch/os_api.h>
#include <asm/arch/sc_reg.h>
#include <asm/arch/adi_hal_internal.h>
#include <asm/arch/watchdog_phy.h>

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define ANA_WDG_LOAD_TIMEOUT_NUM    (10000)
#define ANA_WDG_CLR_INT_TIMEOUT_NUM (10000)

#define WDG_LOAD_TIMER_VALUE(value) \
    do{\
        uint32   cnt          =  0;\
        ANA_REG_SET( WDG_LOAD_HIGH, (uint16)(((value) >> 16 ) & 0xffff));\
        ANA_REG_SET( WDG_LOAD_LOW , (uint16)((value)  & 0xffff) );\
        while((ANA_REG_GET(WDG_INT_RAW) & WDG_LD_BUSY_BIT) && ( cnt < ANA_WDG_LOAD_TIMEOUT_NUM )) cnt++;\
    }while(0)

#define CLEAR_WDG_INT(msk) \
    do{ \
        uint32   cnt          =  0;\
        ANA_REG_SET(WDG_INT_CLR, (msk));\
        while((ANA_REG_GET(WDG_INT_RAW) & (msk))&&(cnt < ANA_WDG_CLR_INT_TIMEOUT_NUM)) cnt++; \
    }while(0)


/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#define  WDG_TRACE 

/*****************************************************************************/
//  Description:    This function config the watch dog module.
//  Dependency:     No
//  Author:         Jie.Dai
//  Note:
/*****************************************************************************/
PUBLIC int32 WDG_PHY_CONFIG (WDG_CONFIG_T *cfg)
{
    uint32 ctrl = 0;
    uint32 val  = 0;

    ANA_REG_SET (WDG_LOCK, WDG_UNLOCK_KEY);

    switch (cfg->mode)
    {
        case WDG_TIMEOUT_MODE_RESET:
            ANA_REG_AND (WDG_CTRL, (~WDG_INT_EN_BIT));
            break;

        case WDG_TIMEOUT_MODE_INT:
            ANA_REG_OR (WDG_CTRL, WDG_INT_EN_BIT);
            break;

        default:
            break;  //No need to change
    }

    if (WDG_TIMER_STATE_STOP != cfg->state)
    {
        WDG_LOAD_TIMER_VALUE (cfg->val);
    }

    switch (cfg->state)
    {
        case WDG_TIMER_STATE_STOP:
            ANA_REG_AND (WDG_CTRL, (~WDG_CNT_EN_BIT));
            break;

        case WDG_TIMER_STATE_START:
            ANA_REG_OR (WDG_CTRL, WDG_CNT_EN_BIT);
            break;

        default:
            break;  //No need to change
    }

    // WDG_TRACE ("Watch Dog Trace: Watch Dog LOAD    0x%8.8x", CHIP_REG_GET (WDG_LOAD));

    ANA_REG_SET (WDG_LOCK, (~WDG_UNLOCK_KEY));

    return 0;
}

/*****************************************************************************/
//  Description:    This function clear the watch dog interrupt
//  Dependency:     No
//  Author:         Jie.Dai
//  Note:
/*****************************************************************************/
PUBLIC int32 WDG_PHY_INT_CLR (void)
{
    ANA_REG_SET (WDG_LOCK, WDG_UNLOCK_KEY);
    CLEAR_WDG_INT (WDG_INT_CLEAR_BIT);
    ANA_REG_SET (WDG_LOCK, (~WDG_UNLOCK_KEY));
    return 0;
}
PUBLIC void WDG_ClockOn(void)
{
    ANA_REG_OR(ANA_AGEN, AGEN_WDG_EN | AGEN_RTC_ARCH_EN | AGEN_RTC_WDG_EN);
}




/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
}
#endif

/*  End Of File */


