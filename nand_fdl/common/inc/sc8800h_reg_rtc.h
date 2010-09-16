/******************************************************************************
 ** File Name:    sc8800h_reg_rtc.h                                           *
 ** Author:       Aiguo.Miao	                                              *
 ** DATE:         08/28/2007                                                  *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 08/28/2007    Aiguo.Miao      Create.                                     *
 ******************************************************************************/
#ifndef _SC8800H_REG_RTC_H_
    #define _SC8800H_REG_RTC_H_
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
#define RTC_BASE                		0x89000000

#define RTC_SECOND_CNT          		(RTC_BASE + 0x0000)
#define RTC_MINUTE_CNT          		(RTC_BASE + 0x0004)
#define RTC_HOUR_CNT            		(RTC_BASE + 0x0008)
#define RTC_DAY_CNT             		(RTC_BASE + 0x000C)
#define RTC_SECOND_UPDATE       		(RTC_BASE + 0x0010)
#define RTC_MINUTE_UPDATE       		(RTC_BASE + 0x0014)
#define RTC_HOUR_UPDATE         		(RTC_BASE + 0x0018)
#define RTC_DAY_UPDATE          		(RTC_BASE + 0x001C)
#define RTC_SECOND_ALM          		(RTC_BASE + 0x0020)
#define RTC_MINUTE_ALM          		(RTC_BASE + 0x0024)
#define RTC_HOUR_ALM            		(RTC_BASE + 0x0028)
#define RTC_DAY_ALM             		(RTC_BASE + 0x002C)
#define RTC_CTL                 		(RTC_BASE + 0x0030)
#define RTC_STS                 		(RTC_BASE + 0x0034)
#define RTC_ICLR                		(RTC_BASE + 0x0038)
#define RTC_IMSK                		(RTC_BASE + 0x003C)


//The corresponding bit of RTC_CTL register.
#define RTCCTL_SEC_IE           		(1 << 0)        //Sec int enable
#define RTCCTL_MIN_IE           		(1 << 1)        //Min int enable
#define RTCCTL_HOUR_IE          		(1 << 2)        //Hour int enable
#define RTCCTL_DAY_IE           		(1 << 3)        //Day int enable
#define RTCCTL_ALARM_IE         		(1 << 4)        //Alarm int enable
#define RTCCTL_HOUR_FMT_SEL_IE  		(1 << 5)        //Hour format select
#define RTCCTL_EN               		(1 << 6)        //Rtc module enable

//The corresponding bit of RTC_STS register.
#define RTCSTS_SEC             	 		(1 << 0)        //Second int
#define RTCSTS_MIN             	 		(1 << 1)        //Minute int
#define RTCSTS_HOUR            	 		(1 << 2)        //Hour int
#define RTCSTS_DAY             	 		(1 << 3)        //Day int
#define RTCSTS_ALARM           	 		(1 << 4)        //Alarm int

//The corresponding bit of RTC_ICLR register.
#define RTCICLR_SEC             		(1 << 0)        //Second int is cleared by write 1 to this bit
#define RTCICLR_MIN             		(1 << 1)        //Minute int is cleared by write 1 to this bit
#define RTCICLR_HOUR            		(1 << 2)        //Hour int is cleared by write 1 to this bit
#define RTCICLR_DAY             		(1 << 3)        //Day int is cleared by write 1 to this bit
#define RTCICLR_ALARM           		(1 << 4)        //Alarm int is cleared by write 1 to this bit

//The corresponding bit of RTC_IMSK register.
#define RTCIMSK_SEC             		(1 << 0)        //
#define RTCIMSK_MIN             		(1 << 1)        //
#define RTCIMSK_HOUR            		(1 << 2)        //
#define RTCIMSK_DAY             		(1 << 3)        //
#define RTCIMSK_ALARM           		(1 << 4)        //


//Real Time Counter.
typedef struct rtc_tag
{
    VOLATILE uint32 sec_cnt;
    VOLATILE uint32 min_cnt;
    VOLATILE uint32 hour_cnt;
    VOLATILE uint32 day_cnt;
    VOLATILE uint32 sec_update;
    VOLATILE uint32 min_update;
    VOLATILE uint32 hour_update;
    VOLATILE uint32 day_update;
    VOLATILE uint32 sec_alm;
    VOLATILE uint32 min_alm;
    VOLATILE uint32 hour_alm;
    VOLATILE uint32 day_alm;
    VOLATILE uint32 ctl;
    VOLATILE uint32 sts;
    VOLATILE uint32 iclr;
    VOLATILE uint32 imsk;   
}rtc_s;

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

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
#endif
// End 

