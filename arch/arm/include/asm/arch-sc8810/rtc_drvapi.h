/******************************************************************************
 ** File Name:      rtc_drvapi.h                                                 *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           10/10/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 Real Timer Counter and Alarm.                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/10/2002     Xueliang.Wang    Create.                                   *
 ** 08/29/2005     Weihua.wang      Modify.
 ******************************************************************************/
#ifndef _RTC_DRVAPI_H_
#define _RTC_DRVAPI_H_
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

// Real Time Counter constant.
#define TB_RTC_CONST_BASE       0x70
#define TB_RTC_TIME_RANGE_ERROR (TB_RTC_CONST_BASE + 1)
#define TB_RTC_INT_ALARM        (TB_RTC_CONST_BASE + 2)
#define TB_RTC_INT_DAY          (TB_RTC_CONST_BASE + 3)
#define TB_RTC_INT_MIN          (TB_RTC_CONST_BASE + 4)
#define TB_RTC_INT_HOUR         (TB_RTC_CONST_BASE + 5)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum
{
    RTC_INT_START = 0,
    RTC_INT_SEC = RTC_INT_START,
    RTC_INT_MIN ,
    RTC_INT_HOUR,
    RTC_INT_DAY ,
    RTC_INT_ALARM,
    RTC_INT_MAX
}
RTC_INTTYPE_E;

typedef struct TB_RTC_TIME_tag
{
    uint8   sec;
    uint8   min;
    uint8   hour;
    uint16  day;
} TB_RTC_TIME_T;


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Init RTC device. All interrupts of RTC was disabled.
//  Author:         Lin.Liu
//  Note:           If initialize successfully, return TB_SUCCESS;
//                  else return error code.
/*****************************************************************************/
PUBLIC uint32 RTC_Init (void);
/*****************************************************************************/
//  Description:    Set RTC value.
//                  This function check the parameter first.
//                  If it is valid, set the time value to RTC register;
//                  Else return error code.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 RTC_Set ( // If set successfully, return TB_SUCCESS;
    // else return error code.
    TB_RTC_TIME_T time  // Time value to be set to RTC register.
);
/*****************************************************************************/
//  Description:    Set RTC's day value.
//                  This function check the parameter first.
//                  If it is valid, set the time value to RTC register;
//                  Else return error code.
//  Author:         Eric.zhou
//  Note:
/*****************************************************************************/
PUBLIC uint32 RTC_SetDay ( // If set successfully, return TB_SUCCESS;
    // else return error code.
    uint32  day
);
/*****************************************************************************/
//  Description:    Get RTC value.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void RTC_Get (
    TB_RTC_TIME_T *time_ptr // Pointer to a TB_RTC_TIME_T struct,
    // used to save RTC value.
);

/*****************************************************************************/
//  Description:    Set ALARM time.
//                  This function check the parameter first.
//                  If it is valid, set the time value to alarm register;
//                  Else return error code.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 RTC_SetAlarm (// If set successfully, return TB_SUCCESS;
    // else return error code.
    TB_RTC_TIME_T time      // Time value to be set to alarm register.
);

/*****************************************************************************/
//  Description:    Get Alarm time.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void RTC_GetAlarm (
    TB_RTC_TIME_T *time_ptr // Pointer to a TB_RTC_TIME_T struct,
    // used to save Alarm time value.
);


/*****************************************************************************/
//  Description:    Disable Alarm.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void RTC_DisableAlarm (void);

/*****************************************************************************/
//  Description:    Set Rtc Int disable/enable
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 RTC_SetRtcInt (RTC_INTTYPE_E type, BOOLEAN value);


/*******************************************************************************/
// Description: This function used to clear alarm int status value
// Global resource dependence: none
// Author: yuhua.shi
// Note:
/********************************************************************************/
PUBLIC void RTC_ClearAlarmInt (void);


/*below are removed from driver_export.c, wait clean up*/
PUBLIC uint32 CheckRTCState (void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_RTC_DRVAPI_H_

