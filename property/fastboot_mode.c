#include <config.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#include <boot_mode.h>
#include <common.h>
#include <linux/string.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <nand.h>
#include <android_boot.h>
#include <environment.h>
#include <jffs2/jffs2.h>

#ifdef CONFIG_EMMC_BOOT
#include "../disk/part_uefi.h"
#include "../drivers/mmc/card_sdio.h"
#include "asm/arch/sci_types.h"
#include <ext_common.h>
#include <ext4fs.h>
#define EMMC_SECTOR_SIZE 512
#endif

extern int dwc_otg_driver_init(void);
extern int usb_fastboot_initialize(void);


void fastboot_mode(void)
{
    printf("%s\n", __FUNCTION__);
#ifdef CONFIG_SPLASH_SCREEN
	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;
	size_t size;
	const char *cmdline;
	loff_t off = 0;

#ifndef CONFIG_EXT4_SPARSE_DOWNLOAD
	ret = mtdparts_init();
	if (ret != 0){
		printf("mtdparts init error %d\n", ret);
		return;
	}
#define SPLASH_PART "fastboot_logo"

	ret = find_dev_and_part(SPLASH_PART, &dev, &pnum, &part);
	if(ret){
		printf("No partition named %s\n", SPLASH_PART);
		return;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		printf("Partition %s not a NAND device\n", SPLASH_PART);
		return;
	}

	off=part->offset;
	nand = &nand_info[dev->id->num];
	//read boot image header
	size = 1<<20;
    char * bmp_img = malloc(size);
    if(!bmp_img){
        printf("not enough memory for splash image\n");
        return;
    }
	ret = nand_read_offset_ret(nand, off, &size, (void *)bmp_img, &off);
	if(ret != 0){
		printf("function: %s nand read error %d\n", __FUNCTION__, ret);
		return;
	}
    extern int lcd_display_bitmap(ulong bmp_image, int x, int y);
    extern lcd_display(void);
    extern void set_backlight(uint32_t value);
    lcd_display_bitmap((ulong)bmp_img, 0, 0);
    lcd_printf("   fastboot mode");
    lcd_display();
    set_backlight(255);
    //char img_addr[9];
    //sprintf(img_addr, "%x\n", bmp_img);
    //setenv("splashimage", img_addr);
#else
    //read boot image header
    block_dev_desc_t *p_block_dev = NULL;
    disk_partition_t info;
    vibrator_hw_init();
    set_vibrator(1);

    p_block_dev = get_dev("mmc", 1);
    if(NULL == p_block_dev){
        return;
    }

    size = 1<<19;
    char * bmp_img = malloc(size);
    if(!bmp_img){
        printf("not enough memory for splash image\n");
        return;
    }
    if (!get_partition_info(p_block_dev, PARTITION_LOGO, &info)) {
        if(TRUE !=  Emmc_Read(PARTITION_USER, info.start, size/EMMC_SECTOR_SIZE, bmp_img)){
            printf("function: %s nand read error\n", __FUNCTION__);
            return;
        }
    }

    extern int lcd_display_bitmap(ulong bmp_image, int x, int y);
    extern void lcd_display(void);
    extern void set_backlight(uint32_t value);
    extern void *lcd_base;
    extern void Dcache_CleanRegion(unsigned int addr, unsigned int length);

    lcd_display_bitmap((ulong)bmp_img, 0, 0);
    lcd_printf("   fastboot mode");
    lcd_display();
    set_backlight(255);
    set_vibrator(0);
#endif
#endif


#ifdef CONFIG_CMD_FASTBOOT
	dwc_otg_driver_init();
	usb_fastboot_initialize();
#endif
#if defined CONFIG_SPLASH_SCREEN && defined CONFIG_EXT4_SPARSE_DOWNLOAD
free(bmp_img);
#endif
}
