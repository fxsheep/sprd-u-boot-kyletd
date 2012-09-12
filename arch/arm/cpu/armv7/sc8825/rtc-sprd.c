/*
 * An RTC device/driver
 * Copyright (C) 2011 Spreadtrum Communication Inc 
 * Author: Mark Yang<markyang@spreadtrum.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include "asm/arch/bits.h"
#include "asm/arch/rtc_reg_v3.h"
#include "asm/arch/regs_ana.h"
#include "asm/arch/adi_hal_internal.h"

#define CLEAR_RTC_INT(mask) \
	do{ ANA_REG_SET(ANA_RTC_INT_CLR, mask); \
		while(ANA_REG_GET(ANA_RTC_INT_RSTS) & mask); \
	}while(0)

#define mdelay(_ms) udelay(_ms*1000)

static inline unsigned get_sec(void)
{
    unsigned sec, sec_bak;
    sec = ANA_REG_GET(ANA_RTC_SEC_CNT) & RTC_SEC_MASK;
    do{
        sec_bak = ANA_REG_GET(ANA_RTC_SEC_CNT) & RTC_SEC_MASK;
        if(sec_bak == sec)
          break;
        else
          sec = sec_bak;
    }while(1);

    return sec;
}
static inline unsigned get_min(void)
{
    unsigned min, min_bak;
    min = ANA_REG_GET(ANA_RTC_MIN_CNT) & RTC_MIN_MASK;
    do{
        min_bak = ANA_REG_GET(ANA_RTC_MIN_CNT) & RTC_MIN_MASK;
        if(min_bak == min)
          break;
        else
          min = min_bak;
    }while(1);

    return min;
}
static inline unsigned get_hour(void)
{
    unsigned hour, hour_bak;
    hour = ANA_REG_GET(ANA_RTC_HOUR_CNT) & RTC_HOUR_MASK;
    do{
        hour_bak = ANA_REG_GET(ANA_RTC_HOUR_CNT) & RTC_HOUR_MASK;
        if(hour_bak == hour)
          break;
        else
          hour = hour_bak;
    }while(1);

    return hour;
}
static inline unsigned get_day(void)
{
    unsigned day, day_bak;
    day = ANA_REG_GET(ANA_RTC_DAY_CNT) & RTC_DAY_MASK;
    do{
        day_bak = ANA_REG_GET(ANA_RTC_DAY_CNT) & RTC_DAY_MASK;
        if(day == day_bak)
          break;
        else
          day = day_bak;
    }while(1);
    
    return day;
}

unsigned long sprd_rtc_get_sec(void)
{
	unsigned sec, min, hour, day;
	unsigned first = 0, second = 0;

	do {
		sec = get_sec();
		min = get_min();
		hour = get_hour();
		day = get_day();

		second = ((((day*24) + hour)*60 + min)*60 + sec);
		if((second - first) == 0)
			break;
		first = second;
	}while(1);
	return first;
}
void sprd_rtc_set_alarm_sec(unsigned long secs);
void sprd_rtc_set_sec(unsigned long secs)
{
	unsigned sec, min, hour, day;
    unsigned set_mask = 0, int_rsts;
	unsigned long temp;

	sec = secs % 60;
	temp = (secs - sec)/60;
	min = temp%60;
	temp = (temp - min)/60;
	hour = temp%24;
	temp = (temp - hour)/24;
	day = temp;


    ANA_REG_OR(ANA_RTC_INT_CLR, RTC_UPD_TIME_MASK);

    if(sec != get_sec()){
        ANA_REG_SET(ANA_RTC_SEC_UPDATE, sec);
        set_mask |= RTC_SEC_ACK_BIT;
    }
    if(min != get_min()){
        ANA_REG_SET(ANA_RTC_MIN_UPDATE, min);
        set_mask |= RTC_MIN_ACK_BIT;
    }
    if(hour != get_hour()){
        ANA_REG_SET(ANA_RTC_HOUR_UPDATE, hour);
        set_mask |= RTC_HOUR_ACK_BIT;
    }
    if(day != get_day()){
        ANA_REG_SET(ANA_RTC_DAY_UPDATE, day);
        set_mask |= RTC_DAY_ACK_BIT;
    }

    //wait till all update done

    do{
        int_rsts = ANA_REG_GET(ANA_RTC_INT_RSTS) & RTC_UPD_TIME_MASK;

        if(set_mask == int_rsts)
          break;
    }while(1);
    ANA_REG_OR(ANA_RTC_INT_CLR, RTC_UPD_TIME_MASK);

	return;
}

unsigned long sprd_rtc_get_alarm_sec(void)
{
	unsigned sec, min, hour, day;
	day = ANA_REG_GET(ANA_RTC_DAY_ALM) & RTC_DAY_MASK;
	hour = ANA_REG_GET(ANA_RTC_HOUR_ALM) & RTC_HOUR_MASK;
	min = ANA_REG_GET(ANA_RTC_MIN_ALM) & RTC_MIN_MASK;
	sec = ANA_REG_GET(ANA_RTC_SEC_ALM) & RTC_SEC_MASK;

	return ((((day*24) + hour)*60 + min)*60 + sec);
}
void sprd_rtc_set_alarm_sec(unsigned long secs)
{
	unsigned sec, min, hour, day;
	unsigned long temp;
	sec = secs % 60;
	temp = (secs - sec)/60;
	min = temp%60;
	temp = (temp - min)/60;
	hour = temp%24;
	temp = (temp - hour)/24;
	day = temp;
	ANA_REG_SET(ANA_RTC_SEC_ALM, sec);
	ANA_REG_SET(ANA_RTC_MIN_ALM, min);
	ANA_REG_SET(ANA_RTC_HOUR_ALM, hour);
	ANA_REG_SET(ANA_RTC_DAY_ALM, day);

	return;
}


int sprd_clean_rtc(void)
{
	int err;
	ANA_REG_AND(ANA_RTC_INT_EN, ~(RTC_INT_ALL_MSK)); // disable all interrupt
	ANA_REG_OR(ANA_APB_CLK_EN, AGEN_RTC_EN | AGEN_RTC_RTC_EN); //enable rtc device
	CLEAR_RTC_INT(RTC_INT_ALL_MSK);
	sprd_rtc_set_sec(0);
    sprd_rtc_set_alarm_sec(0);
    printf("now time sec %lu\n", sprd_rtc_get_sec());
    printf("now alarm sec %lu\n", sprd_rtc_get_alarm_sec());

	return 0;
}
void sprd_rtc_init(void)
{
	ANA_REG_OR(ANA_APB_CLK_EN, AGEN_RTC_EN | AGEN_RTC_RTC_EN); //enable rtc device
}
