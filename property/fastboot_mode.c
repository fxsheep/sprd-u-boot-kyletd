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
#include "normal_mode.h"

extern int dwc_otg_driver_init(void);
extern int usb_fastboot_initialize(void);


void fastboot_mode(void)
{
	printf("%s\n", __FUNCTION__);
#ifdef CONFIG_SPLASH_SCREEN

	vibrator_hw_init();
	set_vibrator(1);
	//read boot image header
	size_t size = 1<<19;
	char * bmp_img = malloc(size);
	if(!bmp_img){
		printf("not enough memory for splash image\n");
		return;
	}
	int ret = read_logoimg(bmp_img,size);
	if(ret == -1)
		return;
	extern int lcd_display_bitmap(ulong bmp_image, int x, int y);
	extern lcd_display(void);
	extern void set_backlight(uint32_t value);
	lcd_display_bitmap((ulong)bmp_img, 0, 0);
	lcd_printf("   fastboot mode");
	lcd_display();
	set_backlight(255);
	set_vibrator(0);

#endif


#ifdef CONFIG_CMD_FASTBOOT
	dwc_otg_driver_init();
	usb_fastboot_initialize();
#endif
#if defined CONFIG_SPLASH_SCREEN && defined CONFIG_EXT4_SPARSE_DOWNLOAD
free(bmp_img);
#endif
}
