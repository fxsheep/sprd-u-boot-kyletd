#include <common.h>
#include <config.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#include <linux/string.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <nand.h>
#include <malloc.h>

char * get_mtdparts(void)
{
#if defined(MTDPARTS_DEFAULT)
        return MTDPARTS_DEFAULT;
#else
	unsigned long size = 0, i;

	for (i = 0; i < CONFIG_SYS_MAX_NAND_DEVICE; i++)
		size += nand_info[i].size / 1024; /* KiB */

	size = size / 1024; /* MiB */

	if (size <= 512)
		return "mtdparts=sprd-nand:256k(spl),512k(2ndbl),256k(params),512k(vmjaluna),10m(modem),3840k(fixnv),3840k(backupfixnv),5120k(dsp),3840k(runtimenv),10m(boot),10m(recovery),250m(system),180m(userdata),20m(cache),256k(misc),1m(boot_logo),1m(fastboot_logo),3840k(productinfo),512k(kpanic)";
	else if (size <= 1024)
		return "mtdparts=sprd-nand:256k(spl),512k(2ndbl),256k(params),512k(vmjaluna),10m(modem),3840k(fixnv),3840k(backupfixnv),5120k(dsp),3840k(runtimenv),10m(boot),10m(recovery),250m(system),600m(userdata),130m(cache),256k(misc),1m(boot_logo),1m(fastboot_logo),3840k(productinfo),512k(kpanic)";
#endif
}

int get_cal_enum_ms(void)
{
    return CALIBRATE_ENUM_MS;
}
int get_cal_io_ms(void)
{
    return CALIBRATE_IO_MS;
}
unsigned int get_bat_low_level(void)
{
    return LOW_BAT_VOL;
}
unsigned int get_bat_low_level_chg(void)
{
	return LOW_BAT_VOL_CHG;
}


unsigned int get_pwr_key_cnt(void)
{
    return PWR_KEY_DETECT_CNT;
}

unsigned int get_alarm_lead_set(void)
{
    return ALARM_LEAD_SET_MS;
}
