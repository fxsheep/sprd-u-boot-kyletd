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
void alarm_mode(void)
{
    printf("%s\n", __func__);
    vlx_nand_boot(BOOT_PART, "androidboot.mode=alarm", BACKLIGHT_ON);
}

int alarm_flag_check(void)
{
    char *file_partition = "/cache";
    char *file_name = "/cache/alarm_flag";
    int ret;

    cmd_yaffs_mount(file_partition);
    ret = cmd_yaffs_ls_chk(file_name);
    cmd_yaffs_umount(file_partition);
    if (ret == -1) {
        printf("file: %s not found\n", file_name);
        return 0;
    } else {
        printf("file: %s exist\n", file_name);
        return 1;
    }
}
