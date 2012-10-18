#include <config.h>
#include <common.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#include <linux/string.h>
#include <android_bootimg.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <nand.h>
#include <android_boot.h>
#include <environment.h>
#include <jffs2/jffs2.h>
#include <boot_mode.h>


#define msleep(a) udelay(a * 1000)
void alarm_mode(void)
{
    printf("%s\n", __func__);

#if BOOT_NATIVE_LINUX
    vlx_nand_boot(BOOT_PART, CONFIG_BOOTARGS, BACKLIGHT_OFF);
#else
    vlx_nand_boot(BOOT_PART, "androidboot.mode=alarm", BACKLIGHT_OFF);
#endif
}
int alarm_file_check(char *time_buf);
int poweron_file_check(char *time_buf);

unsigned long sprd_rtc_get_alarm_sec(void);
unsigned long sprd_rtc_get_sec(void);
void sprd_rtc_set_alarm_sec(unsigned long secs);
void sprd_rtc_init(void);
char time_buf[200]={0};
char time_buf1[200]={0};
static void uboot_rtc_set_alarm_sec(unsigned long secs)
{
   int i=0;
   unsigned long read_secs;
   sprd_rtc_set_alarm_sec(secs);
   msleep(150);
   do {
      if(i!=0){
           sprd_rtc_set_alarm_sec(secs);
	    msleep(150);
       }
      read_secs = sprd_rtc_get_alarm_sec();
      msleep(1);
      i++;
   }while(read_secs != secs && i < 100);
}


int alarm_flag_check(void)
{

	int ret = -1,ret1 = -1;
	long time = 0;
	long time1 = 0;
	unsigned long time_rtc= 0;
	unsigned long time_rtc1= 0;
	unsigned long now_rtc = 0;
	int time_lead = 0;

	ret = alarm_file_check(time_buf);
	if(ret > 0){
		printf("file: alarm_flag exist\n");
		printf("time get %s", time_buf);
		time = simple_strtol(time_buf, NULL, 10);
		time_rtc = time;
	}
	ret1 = poweron_file_check(time_buf1);
	if(ret1 > 0){
		printf("file: poweron_timeinmillis exist\n");
		printf("time get %s", time_buf1);
		time1 = simple_strtol(time_buf1, NULL, 10);
		time_rtc1 = time1;
	}
	sprd_rtc_init();
	now_rtc = sprd_rtc_get_sec();
	printf("now rtc %lu\n", now_rtc);
	time_lead = get_alarm_lead_set();
	time = time - now_rtc;
	time1 = time1 - now_rtc;
	if((time < time_lead +180) && (time > time_lead -10))
	{
		if((time1 < time_lead +180) && (time1 > time_lead -10)&& (time -time1>50))
			ret = 2;
		else
			ret = 1;
	}
	else if((time1 < time_lead +180) && (time1 > time_lead -10))
		ret =2;
	else{
		if (ret == -1&& ret1== -1) {
			printf("file: all not found\n");
		}else if(ret1== -1){
			uboot_rtc_set_alarm_sec(time_rtc);
		}else if(ret== -1){
			uboot_rtc_set_alarm_sec(time_rtc1);
		}else{
			if(time_rtc1 > time_rtc){
				uboot_rtc_set_alarm_sec(time_rtc);
			}else{
				uboot_rtc_set_alarm_sec(time_rtc1);
			}
		}
		ret = 0;
	}

	return ret;
}


