#include <config.h>
#include <common.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#include <image.h>
#include <linux/string.h>
#include <android_bootimg.h>
#include <android_recovery.h>

#include "../disk/part_uefi.h"
#include "../drivers/mmc/card_sdio.h"
#include "asm/arch/sci_types.h"
#include <ext_common.h>
#include <ext4fs.h>
#define PRODUCTINFO_SIZE	(3 * 1024)
#define EMMC_SECTOR_SIZE 512
extern int prodinfo_read_partition(block_dev_desc_t *p_block_dev, EFI_PARTITION_INDEX part, int offset, char *buf, int len);


#ifdef dprintf
#undef dprintf
#endif
#define dprintf(fmt, args...) printf(fmt, ##args)

static char buf[8192];



int get_recovery_message(struct recovery_message *out)
{
	block_dev_desc_t *p_block_dev = NULL;
	disk_partition_t info;
	int size=8192;
	p_block_dev = get_dev("mmc", 1);
	if(NULL == p_block_dev){
		return -1;
	}
	if (!get_partition_info(p_block_dev, PARTITION_MISC, &info)) {
		if(TRUE !=  Emmc_Read(PARTITION_USER, info.start, size/EMMC_SECTOR_SIZE, (void *)buf)){
			printf("function: %s emcc read error\n", __FUNCTION__);
			return -1;
		}
	}

	memcpy(out, buf, sizeof(*out));
	return 0;
}

int set_recovery_message(const struct recovery_message *in)
{
	block_dev_desc_t *p_block_dev = NULL;
	disk_partition_t info;
	int size=8192;
	p_block_dev = get_dev("mmc", 1);
	if(NULL == p_block_dev){
		return -1;
	}
	if (!get_partition_info(p_block_dev, PARTITION_MISC, &info)) {
		memset(buf, 0, sizeof(buf));
		if(TRUE !=  Emmc_Read(PARTITION_USER, info.start, size/EMMC_SECTOR_SIZE, (void *)buf)){
			printf("function: %s emcc read error\n", __FUNCTION__);
			return -1;
		}
		memcpy((void *)buf, in, sizeof(*in));
		if(TRUE !=  Emmc_Write(PARTITION_USER, info.start, size/EMMC_SECTOR_SIZE, (void *)buf)){
			printf("function: %s emcc write error\n", __FUNCTION__);
			return -1;
		}
	}
	return 0;
}

