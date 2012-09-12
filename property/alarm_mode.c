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

#ifdef CONFIG_EMMC_BOOT
#include "../disk/part_uefi.h"
#include "../drivers/mmc/card_sdio.h"
#include "asm/arch/sci_types.h"
#include <ext_common.h>
#include <ext4fs.h>
#define PRODUCTINFO_SIZE	(3 * 1024)
#define EMMC_SECTOR_SIZE 512
extern int prodinfo_read_partition(block_dev_desc_t *p_block_dev, EFI_PARTITION_INDEX part, int offset, char *buf, int len);
extern unsigned long char2u32(unsigned char *buf, int offset);
#else
extern void cmd_yaffs_mount(char *mp);
extern void cmd_yaffs_umount(char *mp);
extern int cmd_yaffs_ls_chk(const char *dirfilename);
extern unsigned int get_alarm_lead_set(void);
#endif
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
#ifdef CONFIG_EMMC_BOOT
int move2timebuf(unsigned char *src, unsigned char *dst)
{
	int len = 0;
	if(src[len] == 0xff)
		return -1;
	while (src[len] != 0x0a)
		len ++;

	len ++;
	while (src[len] != 0x0a)
		len ++;
	len ++;
	memcpy(dst, src, len);
	return 1;
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

	block_dev_desc_t *p_block_dev = NULL;

	int factoryalarmret1 = 1, factoryalarmret2 = 1,factorypowerret1 = 1, factorypowerret2 = 1;
	unsigned long factoryalarmcnt1, factoryalarmcnt2,factorypowercnt1, factorypowercnt2;
	unsigned char factoryalarmarray1[PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE];
	unsigned char factoryalarmarray2[PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE];
	unsigned char factorypowerarray1[PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE];
	unsigned char factorypowerarray2[PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE];
	
	p_block_dev = get_dev("mmc", 1);
	if(NULL == p_block_dev)
		ret = 0;
	if (ret == -1) {
		memset((unsigned char *)factoryalarmarray1, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
		factoryalarmret1 = prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO1, 8 * 1024, (char *)factoryalarmarray1, PRODUCTINFO_SIZE + 8);
		memset((unsigned char *)factoryalarmarray2, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
		factoryalarmret2 = prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO2, 8 * 1024, (char *)factoryalarmarray2, PRODUCTINFO_SIZE + 8);

		memset((unsigned char *)factorypowerarray1, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
		factorypowerret1 = prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO1, 12 * 1024, (char *)factorypowerarray1, PRODUCTINFO_SIZE + 8);
		memset((unsigned char *)factorypowerarray2, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
		factorypowerret2 = prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO2, 12 * 1024, (char *)factorypowerarray2, PRODUCTINFO_SIZE + 8);
	}

	if ((factoryalarmret1 == 0) && (factoryalarmret2 == 0)) {
		factoryalarmcnt1 = char2u32(factoryalarmarray1, 3 * 1024 + 4);
		factoryalarmcnt2 = char2u32(factoryalarmarray2, 3 * 1024 + 4);
		if (factoryalarmcnt2 >= factoryalarmcnt1)
			ret = move2timebuf(factoryalarmarray2, (unsigned char *)time_buf);
		else
			ret = move2timebuf(factoryalarmarray1, (unsigned char *)time_buf);
	} else if ((factoryalarmret1 == 0) && (factoryalarmret2 == 1)) {
		ret = move2timebuf(factoryalarmarray1, (unsigned char *)time_buf);
	} else if ((factoryalarmret1 == 1) && (factoryalarmret2 == 0)) {
		ret = move2timebuf(factoryalarmarray2, (unsigned char *)time_buf);
	} else if ((factoryalarmret1 == 1) && (factoryalarmret2 == 1)) {
		printf("alarm_flag are all empty or wrong\n");
		ret = -1;
	}
	if(ret > 0){
		printf("file: alarm_flag exist\n");
		printf("time get %s", time_buf);
		time = simple_strtol(time_buf, NULL, 10);
		time_rtc = time;
	}
	if ((factorypowerret1 == 0) && (factorypowerret2 == 0)) {
		factorypowercnt1 = char2u32(factorypowerarray1, 3 * 1024 + 4);
		factorypowercnt2 = char2u32(factorypowerarray2, 3 * 1024 + 4);
		if (factorypowercnt2 >= factorypowercnt1)
			ret1 = move2timebuf(factorypowerarray2, (unsigned char *)time_buf1);
		else
			ret1 = move2timebuf(factorypowerarray1, (unsigned char *)time_buf1);
	} else if ((factorypowerret1 == 0) && (factorypowerret2 == 1)) {
		ret1 = move2timebuf(factorypowerarray1, (unsigned char *)time_buf1);
	} else if ((factorypowerret1 == 1) && (factorypowerret2 == 0)) {
		ret1 = move2timebuf(factorypowerarray2, (unsigned char *)time_buf1);
	} else if ((factorypowerret1 == 1) && (factorypowerret2 == 1)) {
		printf("poweron_timeinmillis are all empty or wrong\n");
		ret1 = -1;
	}

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
		ret = 1;
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
#else
int alarm_flag_check(void)
{
    char *file_partition = "/productinfo";
    char *file_name = "/productinfo/alarm_flag";
    char *file_name1 = "/productinfo/poweron_timeinmillis";
    int ret = 0;
    int ret1 =0;
    long time = 0;
    long time1 = 0;
    unsigned long time_rtc= 0;
    unsigned long time_rtc1= 0;
    unsigned long now_rtc = 0;
    int time_lead = 0;

    cmd_yaffs_mount(file_partition);
    ret = cmd_yaffs_ls_chk(file_name);
    ret1= cmd_yaffs_ls_chk(file_name1);
    if(ret >=0){
        printf("file: %s exist\n", file_name);
        cmd_yaffs_mread_file(file_name, (unsigned char *)time_buf);
        printf("time get %s", time_buf);
        time = simple_strtol(time_buf, NULL, 10);
        time_rtc = time;
    }
   if(ret1 >=0){
        printf("file: %s exist\n", file_name1);
        cmd_yaffs_mread_file(file_name1, (unsigned char *)time_buf1);
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
         ret = 1;
   else if((time1 < time_lead +180) && (time1 > time_lead -10))
         ret =2;
   else{
        if (ret == -1&& ret1== -1) {
          printf("file: %s not found\n", file_name);
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
    cmd_yaffs_umount(file_partition);
    return ret;
}
#endif

