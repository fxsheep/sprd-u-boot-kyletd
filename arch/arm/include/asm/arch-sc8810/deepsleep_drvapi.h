/******************************************************************************
 ** File Name:      deepsleep_drvapi.h                                               *
 ** Author:         Richard Yang                                              *
 ** DATE:           08/25/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DSP and MCU for deep sleep.                               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/25/2003     Richard.Yang     Create.                                   *
 ******************************************************************************/
#ifndef _DEEPSLEEP_DRVAPI_H_
#define _DEEPSLEEP_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define ENABLE_DEEP_SLEEP       1
#define DISABLE_DEEP_SLEEP      0

#define ENABLE_AHB_SLEEP        1
#define DISABLE_AHB_SLEEP       0

typedef enum
{
    UART0_E = 0,
    UART1_E,
    UART2_E,
    UART3_E,
    UART_MAX
}
UART_PORT_ID_E;

typedef enum
{
    DISABLE_APB_SLEEP = 0,
    ENABLE_APB_SLEEP
} UART_DSLP_CTRL_E;
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
PUBLIC void SCI_UPM_EnableDeepSleep (uint32 mode);
PUBLIC void SLEEP_Init (void);
//@CR97469 jiexia.yu add to protect DMA transfer when in deepsleep
PUBLIC void SCI_DMA_EnableDeepSleep (uint32 mode);
//@CR97469 end
PUBLIC void DPSLP_PWM_EnableApbSleep (PWM_PORT_ID_E port_id, uint32 mode);
/*****************************************************************************/
//  Description:    This function is used to config the apb clk request of uart.
//  Author:         Zhengjiang.Lei
//  Note:
//****************************************************************************/
PUBLIC void DPSLP_UART_EnableApbSleep(
        UART_PORT_ID_E port_id, 
        uint32 mode
        );
// Enable Audio deep sleep
PUBLIC void SCI_AUD_EnableDeepSleep (uint32 mode);
/*****************************************************************************/
//  Description:    This function is dsp fiq handler
//  Author:
//  Note:
//****************************************************************************/
PUBLIC void DSP_FiqHandler (
    uint32 fiq_num      // fiq number
);

uint32 SLEEP_GetAHBModuleStatus (void);
uint32 SLEEP_GetAPBModuleStatus (void);


PUBLIC void SCI_TPC_EnableDeepSleep (uint32 mode);

// Enable MMI deep sleep

#if defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H)
// Enable LCD backlight brightless deep sleep.
// For the brightless is controlled by PWM, if enter deepsleep, the mcu clk stop,
// and the PWM also stop.
PUBLIC void SCI_LCDBacklightBrightless_EnableDeepSleep (uint32 mode);

//
// Set charge deepsleep mode
//
PUBLIC void SCI_CHR_EnableDeepSleep (uint32 mode);
//enable tv deepsleep
PUBLIC void SCI_TV_EnableDeepSleep (uint32 mode);
PUBLIC void SCI_MMI_EnableDeepSleep (uint32 mode);
// Enable SIM deep sleep
PUBLIC void SCI_SIM_EnableDeepSleep (uint32 mode);
// Enable Layer1 deeep sleep
PUBLIC void SCI_L1_EnableDeepSleep (uint32 mode);
// Enable Bluetooth deep sleep
PUBLIC void SCI_BT_EnableDeepSleep (uint32 mode);
// Get DSP absolute time
PUBLIC uint32 SCI_GetDSPAbsoluteTime (void);

// Init delta time between DSP and MCU
PUBLIC void SCI_InitDSPDeltaTime (void);

// Check if enter deep sleep
PUBLIC void SCI_EnterDeepSleep (void);


PUBLIC void DS_CreateAbsoluteTimer (uint32 abs_time);

PUBLIC void deep_sleep_delay (uint32 count);

PUBLIC void SCI_PCM_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_BTCLK_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_DCAM_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_Calibration_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_SDIO_EnableDeepSleep (uint32 mode);
PUBLIC void SCI_UPM_EnableDeepSleep (uint32 mode);
/*****************************************************************************/
//  Description:    This function is used to config the apb clk request of pwm.
//  Author:         Younger.Yang
//  Note:
//****************************************************************************/
PUBLIC void DPSLP_Lcdc_EnableAhbSleep (uint32 mode);
PUBLIC void DPSLP_MpegEnc_EnableAhbSleep (uint32 mode);
PUBLIC void DPSLP_MpegDec_EnableAhbSleep (uint32 mode);
PUBLIC void DPSLP_ISP_EnableAhbSleep (uint32 mode);//just used for PLATFORM_SC6800H and PLATFORM_SC8800H
PUBLIC void DPSLP_DMA_EnableAhbSleep (uint32 mode);
PUBLIC void DPSLP_SetBklight_Sts (BOOLEAN bklight_on);


/*****************************************************************************/
// Description :    This function is used to get the val of sleepmode.
// Global resource dependence :sleep_mode
// Author :         kevin.wang
// Note :
/*****************************************************************************/
PUBLIC uint32 SCI_Getsleepmode (void);

/*****************************************************************************/
// Description :    This function is used to set the flag which indicate if xtl need open in deepsleep
// Global resource dependence :s_sleep_xtl_open_cnt
// Author :         yuhua.shi
// Note :
/*****************************************************************************/
PUBLIC void DPSLP_XTLOpenInSleep (
    BOOLEAN is_open                         //indicate if xtl need open in deepsleep,
    //SCI_TRUE: need open xtl in deepsleep; SCI_FALSE: can close xtl in deepsleep
);

/*****************************************************************************/
// Description :    This function is used to enable or disable rf
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void DSLP_RFLDOControl (
    BOOLEAN enable          // TRUE:enable, FALSE:disable
);

/*****************************************************************************/
// Description :    This function is used to get rf enable flag
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC uint32 DSLP_GetRFEnableFlag (void);

#elif defined(PLATFORM_SC8800H)
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
// Enable LCD backlight brightless deep sleep.
// For the brightless is controlled by PWM, if enter deepsleep, the mcu clk stop,
// and the PWM also stop.
PUBLIC void SCI_LCDBacklightBrightless_EnableDeepSleep (uint32 mode);

//
// Set charge deepsleep mode
//
PUBLIC void SCI_CHR_EnableDeepSleep (uint32 mode);

// Enable Audio deep sleep
PUBLIC void SCI_AUD_EnableDeepSleep (uint32 mode);
// Enable MMI deep sleep
PUBLIC void SCI_MMI_EnableDeepSleep (uint32 mode);
// Enable SIM deep sleep
PUBLIC void SCI_SIM_EnableDeepSleep (uint32 mode);
// Enable Layer1 deeep sleep
PUBLIC void SCI_L1_EnableDeepSleep (uint32 mode);
// Enable Bluetooth deep sleep
PUBLIC void SCI_BT_EnableDeepSleep (uint32 mode);
// Get DSP absolute time
PUBLIC uint32 SCI_GetDSPAbsoluteTime (void);

// Init delta time between DSP and MCU
PUBLIC void SCI_InitDSPDeltaTime (void);

// Check if enter deep sleep
PUBLIC void SCI_EnterDeepSleep (void);



// RTC handler
PUBLIC void SCI_RTC_TimerHandler (void);

PUBLIC void DS_CreateAbsoluteTimer (uint32 abs_time);

PUBLIC void deep_sleep_delay (uint32 count);

PUBLIC void SCI_PCM_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_BTCLK_EnableDeepSleep (uint32 mode);

PUBLIC void SCI_AP_EnableDeepSleep (uint32 mode);


PUBLIC void SCI_Calibration_EnableDeepSleep (uint32 mode);

/*****************************************************************************/
//  Description:    This function is used to config the apb clk request of pwm.
//  Author:         Younger.Yang
//  Note:
//****************************************************************************/
PUBLIC void DPSLP_PWM_EnableApbSleep (PWM_PORT_ID_E port_id, uint32 mode);
PUBLIC void DPSLP_Lcdc_EnableAhbSleep (uint32 mode);
PUBLIC void DPSLP_MpegEnc_EnableAhbSleep (uint32 mode);
PUBLIC void DPSLP_MpegDec_EnableAhbSleep (uint32 mode);
PUBLIC void DPSLP_ISP_EnableAhbSleep (uint32 mode);//just used for PLATFORM_SC6800H and PLATFORM_SC8800H
PUBLIC void DPSLP_DCAM_EnableAhbSleep (uint32 mode);
PUBLIC void DPSLP_DMA_EnableAhbSleep (uint32 mode);

/*****************************************************************************/
// Description :    This function is used to get the val of sleepmode.
// Global resource dependence : sleep_mode
// Author :         kevin.wang
// Note :
/*****************************************************************************/
PUBLIC uint32 SCI_Getsleepmode (void);
/*****************************************************************************/
// Description :    This function is used to Enable
// LDO_RF power down when enter deepsleep
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void DSSLP_EnableLDORFAutoPD (void);
/*****************************************************************************/
//  Description:    This function is used to config the ahb clk request of dma.
//  Author:         Zhengjiang.Lei
//  Note:
//****************************************************************************/
PUBLIC void SCI_SDIO_EnableDeepSleep (uint32 mode);

/*****************************************************************************/
// Description :    This function is used to enable arm CEVAX matrix to sleep.
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void DSLP_ARM_DAHB_SleepEn (void);

/*****************************************************************************/
// Description :    This function is used to enable or disable rf
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void DSLP_RFLDOControl (
    BOOLEAN enable          // TRUE:enable, FALSE:disable
);
/*****************************************************************************/
// Description :    This function is used to set sleep flag.
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void DSLP_SetEnableSleepFlag (void);


/*****************************************************************************/
//  Description:    This function is dsp semaphore fiq handler
//  Author:
//  Note:
//****************************************************************************/
PUBLIC void DSP_SEMA_Handler (
    uint32 fiq_num      // fiq number
);

/*****************************************************************************/
//  Description:    This function is used to init dsp amr share memeory
//  Author:
//  Note:
//****************************************************************************/
PUBLIC void SCI_InitDSP_MCU_SHR (void);

/*****************************************************************************/
// Description :    This function is used to clear DSP semaphore0.
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void SCI_DSPSEMA0_Clear (void);

/*****************************************************************************/
// Description :    This function is used to set the flag which indicate if xtl need open in deepsleep
// Global resource dependence :s_sleep_xtl_open_cnt
// Author :         yuhua.shi
// Note :
/*****************************************************************************/
PUBLIC void DPSLP_XTLOpenInSleep (
    BOOLEAN is_open                         //indicated if xtl need open in deepsleep,
    //SCI_TRUE: need open xtl in deepsleep; SCI_FALSE: can close xtl in deepsleep
);
PUBLIC void SCI_Deep_Sleep_Test (void);
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif //




#endif // _DEEPSLEEP_DRVAPI_H_
