#include <config.h>
#include <common.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#include <image.h>
#include <linux/string.h>
#include <android_bootimg.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <nand.h>
#include <android_boot.h>
#include <environment.h>
#include <jffs2/jffs2.h>
#include <boot_mode.h>
#include <android_recovery.h>

#ifdef dprintf
#undef dprintf
#endif
#define dprintf(fmt, args...) printf(fmt, ##args)



static const int MISC_COMMAND_PAGE = 1;		// bootloader command is this page
static char buf[8192];



int get_recovery_message(struct recovery_message *out)
{
	loff_t offset = 0;
	unsigned pagesize;
	size_t size;

	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;

	ret = mtdparts_init();
	if(ret != 0){
		dprintf("mtdparts init error %d\n", ret);
		return -1;
	}

	ret = find_dev_and_part("misc", &dev, &pnum, &part);
	if(ret){
		dprintf("No partiton named %s found\n", "misc");
		return -1;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
        printf("Partition %s not a NAND device\n", "misc");
        return -1;
    }

	nand = &nand_info[dev->id->num];
	pagesize = nand->writesize;

	offset = pagesize * MISC_COMMAND_PAGE + part->offset;
	size = pagesize;
	ret = nand_read_skip_bad(nand, offset, &size, (void *)buf);
	if(ret != 0){
		printf("function: %s nand read error %d\n", __FUNCTION__, ret);
		return -1;
	}

	memcpy(out, buf, sizeof(*out));
	return 0;
}

int set_recovery_message(const struct recovery_message *in)
{
	loff_t offset = 0;
	unsigned pagesize;
	size_t size;

	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;

	ret = mtdparts_init();
	if(ret != 0){
		dprintf("mtdparts init error %d\n", ret);
		return -1;
	}

	ret = find_dev_and_part("misc", &dev, &pnum, &part);
	if(ret){
		dprintf("No partiton named %s found\n", "misc");
		return -1;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		dprintf("Partition %s not a NAND device\n", "misc");
		return -1;
	}

	nand = &nand_info[dev->id->num];
	pagesize = nand->writesize;

	size = pagesize*(MISC_COMMAND_PAGE + 1);

	ret = nand_read_skip_bad(nand, part->offset, &size, (void *)SCRATCH_ADDR);
	if(ret != 0){
		dprintf("%s: nand read error %d\n", __FUNCTION__, ret);
		return -1;
	}


	offset = SCRATCH_ADDR;
	offset += (pagesize * MISC_COMMAND_PAGE);
	memcpy(offset, in, sizeof(*in));

	nand_erase_options_t opts;
	memset(&opts, 0, sizeof(opts));
	opts.offset = part->offset;
	opts.length = pagesize *(MISC_COMMAND_PAGE + 1);
	opts.jffs2 = 0;
	opts.scrub = 0;
	opts.quiet = 1;
	ret = nand_erase_opts(nand, &opts);
	if(ret != 0){
		dprintf("%s, nand erase error %d\n", __FUNCTION__, ret);
		return -1;
	}
	ret = nand_write_skip_bad(nand, part->offset, &size, (void *)SCRATCH_ADDR);
	if(ret != 0){
		dprintf("%s, nand erase error %d\n", __FUNCTION__, ret);
		return -1;
	}

}

