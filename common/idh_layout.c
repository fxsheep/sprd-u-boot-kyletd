#include <common.h>
#include <config.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#include <linux/string.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <nand.h>
#include <malloc.h>

#define MTDPARTS_MAXLEN		(512)
#define PARTITION_NAME_LEN	(20)
#define UNDEFINE_PARTITION_LEN	(0xffffffff)

typedef struct _MTD_PARTITION_STRUCT
{
	unsigned char	name[PARTITION_NAME_LEN];
	unsigned long	size; /* KiB */

} MTD_PARTITION_T;

static unsigned long nandsize = 0;
unsigned char mtdpartbuffer[MTDPARTS_MAXLEN] = "mtdparts=sprd-nand:256k(spl),512k(2ndbl),256k(params),512k(vmjaluna),10m(modem),3840k(fixnv),3840k(backupfixnv),5120k(dsp),3840k(runtimenv),10m(boot),10m(recovery),250m(system),180m(userdata),20m(cache),256k(misc),1m(boot_logo),1m(fastboot_logo),3840k(productinfo),512k(kpanic)";

static MTD_PARTITION_T mtd_partition_cfg[] = {
	{"spl", 		256},
	{"2ndbl", 		512},
	{"params", 		256},
	{"vmjaluna", 		512},
	{"modem", 		10 * 1024},
	{"fixnv", 		3840},
	{"backupfixnv", 	3840},
	{"dsp", 		5120},
	{"runtimenv", 		3840},
	{"boot", 		10 * 1024},
	{"recovery", 		10 * 1024},
	{"system", 		250 * 1024},
	{"userdata", 		UNDEFINE_PARTITION_LEN},
	{"cache", 		UNDEFINE_PARTITION_LEN},
	{"misc", 		256},
	{"boot_logo", 		1 * 1024},
	{"fastboot_logo", 	1 * 1024},
	{"productinfo", 	3840},
	{"kpanic", 		512},
	{"", 			0}
};

void create_mtdpart_buffer(unsigned long nandsize, unsigned long realsize)
{
	unsigned long i;
	unsigned long str_len;
	
	i = 0;
	while (strcmp(mtd_partition_cfg[i].name, "cache") != 0)
		i++;

	if (nandsize <= (512 * 1024))
		mtd_partition_cfg[i].size = 20 * 1024;
	else if (nandsize <= (1024 * 1024))
		mtd_partition_cfg[i].size = 130 * 1024;
	realsize += mtd_partition_cfg[i].size;
	
	i = 0;
	while (strcmp(mtd_partition_cfg[i].name, "userdata") != 0)
		i++;

	if (nandsize <= (512 * 1024))
		mtd_partition_cfg[i].size = 180 * 1024; /* 180 MiB */
	else if (nandsize <= (1024 * 1024))
		mtd_partition_cfg[i].size = (nandsize - realsize) / 1024 * 1024; /* allign to MiB */

	i = 0;
	/*while (mtd_partition_cfg[i].size) {
		printf("i = %d  name = %s  size = %d\n", i, mtd_partition_cfg[i].name, mtd_partition_cfg[i].size);
		i++;
	}*/

	memset(mtdpartbuffer, 0, MTDPARTS_MAXLEN);
	sprintf(mtdpartbuffer, "mtdparts=sprd-nand:");
    	str_len = strlen(mtdpartbuffer);
	i = 0;
	while (mtd_partition_cfg[i].size) {
		sprintf(&mtdpartbuffer[str_len], "%dk(%s),", mtd_partition_cfg[i].size, mtd_partition_cfg[i].name);
		str_len = strlen(mtdpartbuffer);
		i++;
	}
	str_len = strlen(mtdpartbuffer);
	mtdpartbuffer[str_len - 1] = '\0'; /* delete the last , */
}

char *get_mtdparts(void)
{
#if defined(MTDPARTS_DEFAULT)
        return MTDPARTS_DEFAULT;
#else
	unsigned long i, realsize;

	if (nandsize == 0) {
		for (i = 0; i < CONFIG_SYS_MAX_NAND_DEVICE; i++)
			nandsize += nand_info[i].size; /* Bytes */

		nandsize = nandsize / 1024; /* KiB */

		i = 0;
		realsize = 0;
		while (mtd_partition_cfg[i].size) {
			if (mtd_partition_cfg[i].size != UNDEFINE_PARTITION_LEN)
				realsize += mtd_partition_cfg[i].size;
			i++;
		}

		if (realsize >= nandsize)
			printf("\nmtd_partition_cfg error, use default mtd partition table\n");
		else
			create_mtdpart_buffer(nandsize, realsize);
	} /* if (nandsize == 0) */

	return mtdpartbuffer;
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
