#include <config.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#include <boot_mode.h>

extern int dwc_otg_driver_init(void);
extern int usb_fastboot_initialize(void);


void fastboot_mode(void)
{
    printf("%s\n", __FUNCTION__);
	
#ifdef CONFIG_CMD_FASTBOOT
	dwc_otg_driver_init();
	usb_fastboot_initialize();
#endif

}
