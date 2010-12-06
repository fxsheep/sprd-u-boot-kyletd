/******************************************************************************
 ** File Name:      chng_freq_drvapi.h                                                    *
 ** Author:         Zhengjiang.Lei                                              *
 ** DATE:           12/08/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This driver is for spreadtrum chip set.                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/08/2006     Zhengjiang.Lei     Create.                                   *
   ** 08/17/2010     Yuhua.Shii     Modify.                                   *
 ******************************************************************************/
#ifndef _CHNG_FREQ_DRVAPI_H_
#define _CHNG_FREQ_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                         Defines                                           *
 **---------------------------------------------------------------------------*/
//define the freq index for application(the freq index should be defined continuously)
#define FREQ_INDEX_BASE 0
typedef enum
{
    FREQ_INDEX_GENERAL = FREQ_INDEX_BASE,//general freq index can be used for general application
    FREQ_INDEX_MP3,                 //mp3 freq index
    FREQ_INDEX_MIDI,                //midi freq index
    FREQ_INDEX_AAC,                 //aac freq index
    FREQ_INDEX_DC_LOW,          //dc low freq index
    FREQ_INDEX_DC_HIGH,            //dc high freq index
    FREQ_INDEX_ISP_LOW,         //isp low freq index
    FREQ_INDEX_ISP_HIGH,               //isp high freq index
    FREQ_INDEX_MP4_PLAY_LVL1,   //mp4 play level1 freq index
    FREQ_INDEX_MP4_PLAY_LVL2,   //mp4 play level2 freq index
    FREQ_INDEX_MP4_PLAY_LVL3,   //mp4 play level3 freq index
    FREQ_INDEX_MP4_PLAY_LVL4,   //mp4 play level4 freq index
    FREQ_INDEX_MP4_REC,         //mp4 record freq index
    FREQ_INDEX_USB_LOW,         //usb low freq index
    FREQ_INDEX_USB_MID,         //usb middle freq index
    FREQ_INDEX_USB_HIGH,            //usb high freq index
    FREQ_INDEX_PCM_BLUE_PLAY,   //pcm blue play freq index
    FREQ_INDEX_BT_IDLE,         //bt idle freq index
    FREQ_INDEX_BT_FTP,          //bt ftp freq index
    FREQ_INDEX_BT_HFG,          //bt hfg freq index
    FREQ_INDEX_BT_A2DP,         //bt a2dp freq index
    FREQ_INDEX_ATC,               //atc freq index

    FREQ_INDEX_CUSTOMIZED_BASE=0x1000, //base index for customized clk

    //from FREQ_INDEX_CUSTOMIZED_BASE to FREQ_INDEX_MAX, reserved for application use

    FREQ_INDEX_MAX = 0xFFFF           //max freq index
                 }
                 CHNG_FREQ_INDEX_E;

//define the min level that arm/ahb can run in our system( >= 0x1).
#define ARM_CLK_LVL5    0x5
#define ARM_CLK_LVL4    0x4
#define ARM_CLK_LVL3    0x3
#define ARM_CLK_LVL2    0x2


#define     WAIT_EMC_IDLE_TIMEOUT_RET        0xFF
#define CHNG_FREQ_REQ_HANDLER_NULL      0xFFFFFFFF

/*
    define the event id of frequency-changing sync. usually all ahb masters which
    will trig a hardware burst to access sdram should be defined .
*/
typedef enum
{
    FREQ_CHNG_EVENT_LCDC = 0,
    FREQ_CHNG_EVENT_MPEG_ENC,
    FREQ_CHNG_EVENT_MPEG_DEC,
    FREQ_CHNG_EVENT_ISP,
    FREQ_CHNG_EVENT_SDIO,
    FREQ_CHNG__MAX_EVENT_ID
} FREQ_CHNG_EVENT_E;

#define FREQ_CHNG_EVENT_ALL             ((1<<FREQ_CHNG__MAX_EVENT_ID) - 1)

typedef enum
{
    SYS_MODE_ASSERT = 0,            // Assert state
    SYS_MODE_NORMAL    ,            // Normal Run
    SYS_MODE_INT                // Interrupt state
} SYS_MODE_E;

typedef enum
{
    CHNG_FREQ_REQ_CMD = 0xF000,
    CHNG_FREQ_REQ_MAX
} CHNG_FREQ_COMMAND_E;


typedef struct _chng_freq_msg
{
    uint32 freq_param1;//arm_ahb_clk_lvl;
    uint32 freq_param2;//sdram_timing1;
    uint32 freq_param3;//sdram_timing2;
    uint32 freq_param4;//reserved
} CHNG_FREQ_MESSAGE_T;

/*****************************************************************************/
// Description :    This function is used to get a handler which specify the application
//                  that request the frequency-changing.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           before an application request to change frequency, get a handler first.
/*****************************************************************************/
PUBLIC uint32 CHNG_FREQ_GetReqHandler (const char *handler_name);
/*****************************************************************************/
// Description :    This function is used to delete the handler which request to change frequency
//                  before .
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           .
/*****************************************************************************/
PUBLIC uint32 CHNG_FREQ_DeleteReqHandler (uint32 handler);

/*****************************************************************************/
// Description :    This function is used to set an event which specify this event
//                  permits to change frequency. after this event finished accessing
//                  sdram, set the event.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           sys_mode: when you call this function in an isr handler,
//                          use SYS_MODE_INT.
/*****************************************************************************/
PUBLIC uint32 CHNG_FREQ_Event_Set (FREQ_CHNG_EVENT_E id, SYS_MODE_E sys_mode);
/*****************************************************************************/
// Description :    This function is used to clear an event which specify this event
//                  forbids to change frequency. before this event will begin to access
//                  sdram, clear the event.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           sys_mode: when you call this function in an isr handler,
//                          use SYS_MODE_INT.
/*****************************************************************************/
PUBLIC uint32 CHNG_FREQ_Event_Clr (FREQ_CHNG_EVENT_E id, SYS_MODE_E sys_mode);
/*****************************************************************************/
// Description :    This function is used to init the chng_freq module.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           you must call this function after RTOS init.
/*****************************************************************************/
PUBLIC void CHNG_FREQ_Init (void);

/*****************************************************************************/
// Description :    This function is used to set the customized freq config(base address)
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void CHNG_FREQ_SetCustomFreqCfg (uint32 *p_customized_freq_ptr);

/*****************************************************************************/
// Description :    This function is used to set ARM/AHB clock.
//                  Now, we only support change clock in a thread. If it is called
//                  at IRQ or FIQ, an assert happened.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void CHNG_FREQ_SetArmClk (uint32 handler, CHNG_FREQ_INDEX_E clk_type_index);

/*****************************************************************************/
// Description :    This function is used to restore the original ARM_CLK/AHB_CLK
// Global resource dependence :
// Author :         Nick.Zhao
// Note :
/*****************************************************************************/
PUBLIC void CHNG_FREQ_RestoreARMClk (uint32 handler);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_CHNG_FREQ_DRVAPI_H_
