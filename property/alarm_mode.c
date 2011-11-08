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

extern void cmd_yaffs_mount(char *mp);
extern void cmd_yaffs_umount(char *mp);
extern int cmd_yaffs_ls_chk(const char *dirfilename);
extern unsigned int get_alarm_lead_set(void);
void alarm_mode(void)
{
    printf("%s\n", __func__);
    vlx_nand_boot(BOOT_PART, "androidboot.mode=alarm", BACKLIGHT_ON);
}

unsigned long sprd_rtc_get_alarm_sec(void);
unsigned long sprd_rtc_get_sec(void);
void sprd_rtc_init(void);
int alarm_flag_check(void)
{
    char *file_partition = "/cache";
    char *file_name = "/cache/alarm_flag";
    int ret = 0;
    char time_buf[20]={0};
    long time = 0;
    unsigned long now_rtc = 0;
    int time_lead = 0;

    cmd_yaffs_mount(file_partition);
    ret = cmd_yaffs_ls_chk(file_name);
    if (ret == -1) {
        printf("file: %s not found\n", file_name);
        ret = 0;
    } else {
        printf("file: %s exist\n", file_name);
        cmd_yaffs_mread_file(file_name, (unsigned char *)time_buf);
        printf("time get %s", time_buf);
        time = simple_strtol(time_buf, NULL, 10);
        sprd_rtc_init();
        now_rtc = sprd_rtc_get_sec();
        printf("now rtc %lu\n", now_rtc);
        time = time - now_rtc;
        time_lead = get_alarm_lead_set();
        if((time < time_lead) && (time > time_lead -10))
          ret = 1;
        else
          ret = 0;
    }
    cmd_yaffs_umount(file_partition);
    return ret;
}
