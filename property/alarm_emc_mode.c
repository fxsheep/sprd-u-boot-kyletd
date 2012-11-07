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


#include "../disk/part_uefi.h"
#include "../drivers/mmc/card_sdio.h"
#include "asm/arch/sci_types.h"
#include <ext_common.h>
#include <ext4fs.h>
#define PRODUCTINFO_SIZE	(3 * 1024)
extern int prodinfo_read_partition(block_dev_desc_t *p_block_dev, EFI_PARTITION_INDEX part, int offset, char *buf, int len);
extern unsigned long char2u32(unsigned char *buf, int offset);

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
int alarm_file_check(char *time_buf)
{
	int ret = -1;
	block_dev_desc_t *p_block_dev = NULL;

	int factoryalarmret1 = 1, factoryalarmret2 = 1;
	unsigned long factoryalarmcnt1, factoryalarmcnt2;
	unsigned char factoryalarmarray1[PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE];
	unsigned char factoryalarmarray2[PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE];

	p_block_dev = get_dev("mmc", 1);
	if(NULL == p_block_dev)
		ret = 0;
	if (ret == -1) {
		memset((unsigned char *)factoryalarmarray1, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
		factoryalarmret1 = prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO1, 8 * 1024, (char *)factoryalarmarray1, PRODUCTINFO_SIZE + 8);
		memset((unsigned char *)factoryalarmarray2, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
		factoryalarmret2 = prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO2, 8 * 1024, (char *)factoryalarmarray2, PRODUCTINFO_SIZE + 8);
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
	return ret;

}
int poweron_file_check(char *time_buf)
{
	int ret = -1;
	block_dev_desc_t *p_block_dev = NULL;

	int factorypowerret1 = 1, factorypowerret2 = 1;
	unsigned long factorypowercnt1, factorypowercnt2;
	unsigned char factorypowerarray1[PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE];
	unsigned char factorypowerarray2[PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE];

	p_block_dev = get_dev("mmc", 1);
	if(NULL == p_block_dev)
		ret = 0;
	if (ret == -1) {
		memset((unsigned char *)factorypowerarray1, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
		factorypowerret1 = prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO1, 12 * 1024, (char *)factorypowerarray1, PRODUCTINFO_SIZE + 8);
		memset((unsigned char *)factorypowerarray2, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
		factorypowerret2 = prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO2, 12 * 1024, (char *)factorypowerarray2, PRODUCTINFO_SIZE + 8);
	}

	if ((factorypowerret1 == 0) && (factorypowerret2 == 0)) {
		factorypowercnt1 = char2u32(factorypowerarray1, 3 * 1024 + 4);
		factorypowercnt2 = char2u32(factorypowerarray2, 3 * 1024 + 4);
		if (factorypowercnt2 >= factorypowercnt1)
			ret = move2timebuf(factorypowerarray2, (unsigned char *)time_buf);
		else
			ret = move2timebuf(factorypowerarray1, (unsigned char *)time_buf);
	} else if ((factorypowerret1 == 0) && (factorypowerret2 == 1)) {
		ret = move2timebuf(factorypowerarray1, (unsigned char *)time_buf);
	} else if ((factorypowerret1 == 1) && (factorypowerret2 == 0)) {
		ret = move2timebuf(factorypowerarray2, (unsigned char *)time_buf);
	} else if ((factorypowerret1 == 1) && (factorypowerret2 == 1)) {
		printf("poweron_timeinmillis are all empty or wrong\n");
		ret = -1;
	}
	return ret;
}



