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

unsigned char raw_header[2048];
#ifdef FLASH_PAGE_SIZE
#undef FALSH_PAGE_SIZE
#endif
#define FLASH_PAGE_SIZE 2048

void normal_mode(void)
{
	boot_img_hdr *hdr = (void *)raw_header;
	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;
	size_t size;
	const char *cmdline;
	loff_t off = 0;

	ret = mtdparts_init();
	if (ret != 0){
		printf("mtdparts init error %d\n", ret);
		return;
	}

	ret = find_dev_and_part(BOOT_PART, &dev, &pnum, &part);
	if(ret){
		printf("No partition named %s\n", BOOT_PART);
		return;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		printf("Partition %s not a NAND device\n", BOOT_PART);
		return;
	}

	off=part->offset;
	nand = &nand_info[dev->id->num];
	//read boot image header
	size = 2048;
	ret = nand_read_offset_ret(nand, off, &size, (void *)hdr, &off);
	if(ret != 0){
		printf("function: %s nand read error %d\n", __FUNCTION__, ret);
		return;
	}

	if(memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)){
		printf("bad boot image header\n");
		return;
	}

	//read kernel image
	size = (hdr->kernel_size+(FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
	if(size <=0){
		printf("kernel image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)hdr->kernel_addr, &off);
	if(ret != 0){
		printf("kernel nand read error %d\n", ret);
		return;
	}
	//read ramdisk image
	size = (hdr->ramdisk_size+(FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
	if(size<0){
		printf("ramdisk size error\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)hdr->ramdisk_addr, &off);
	if(ret != 0){
		printf("ramdisk nand read error %d\n", ret);
		return;
	}
#ifdef BOOT_DEBUG
	printf("kernel @0x%08x (0x%08x bytes)\n", hdr->kernel_addr, hdr->kernel_size);
	printf("ramdisk @0x%08x (0x%08x bytes)\n", hdr->ramdisk_addr, hdr->ramdisk_size);
#endif
	//set boot environment
	if(hdr->cmdline[0]){
		cmdline = (char *)hdr->cmdline;
	}else{
		cmdline = getenv("bootargs");
	}
#ifdef BOOT_DEBUG
	printf("cmdline %s\n", cmdline);
#endif
	printf("Booting Linux\n");
	creat_atags(hdr->tags_addr, cmdline, hdr->ramdisk_addr, hdr->ramdisk_size);
	boot_linux(hdr->kernel_addr,hdr->tags_addr);
}
