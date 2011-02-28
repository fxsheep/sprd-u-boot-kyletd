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

#ifdef dprintf
#undef dprintf
#endif
#define dprintf(fmt, args...) printf(fmt, ##args)


extern unsigned char raw_header[2048];
#ifdef FLASH_PAGE_SIZE
#undef FALSH_PAGE_SIZE
#endif
#define FLASH_PAGE_SIZE 2048

#include <android_recovery.h>

static const int MISC_PAGES = 3;			// number of pages to save
static const int MISC_COMMAND_PAGE = 1;		// bootloader command is this page
static char buf[4096];
unsigned boot_into_recovery = 0;


int get_recovery_message(struct recovery_message *out)
{
	loff_t offset = 0;
	unsigned pagesize = 2048;
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

	offset = pagesize * MISC_COMMAND_PAGE + part->offset; 
	nand = &nand_info[dev->id->num];
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
	unsigned pagesize = 2048;
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
	
	offset = part->offset; 
	nand = &nand_info[dev->id->num];
	size = pagesize*(MISC_COMMAND_PAGE + 1);

	ret = nand_read_skip_bad(nand, offset, SCRATCH_ADDR, size);
	if(ret != 0){
		dprintf("%s: nand read error %d\n", __FUNCTION__, ret);
		return -1;
	}

	

	offset += (pagesize * MISC_COMMAND_PAGE);
	offset += SCRATCH_ADDR;
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

int read_update_header_for_bootloader(struct update_header *header)
{
	return 0;
}

int update_firmware_image (struct update_header *header, char *name)
{

	return 0;
}

/* Bootloader / Recovery Flow
 *
 * On every boot, the bootloader will read the recovery_message
 * from flash and check the command field.  The bootloader should
 * deal with the command field not having a 0 terminator correctly
 * (so as to not crash if the block is invalid or corrupt).
 *
 * The bootloader will have to publish the partition that contains
 * the recovery_message to the linux kernel so it can update it.
 *
 * if command == "boot-recovery" -> boot recovery.img
 * else if command == "update-radio" -> update radio image (below)
 * else -> boot boot.img (normal boot)
 *
 * Radio Update Flow
 * 1. the bootloader will attempt to load and validate the header
 * 2. if the header is invalid, status="invalid-update", goto #8
 * 3. display the busy image on-screen
 * 4. if the update image is invalid, status="invalid-radio-image", goto #8
 * 5. attempt to update the firmware (depending on the command)
 * 6. if successful, status="okay", goto #8
 * 7. if failed, and the old image can still boot, status="failed-update"
 * 8. write the recovery_message, leaving the recovery field
 *    unchanged, updating status, and setting command to
 *    "boot-recovery"
 * 9. reboot
 *
 * The bootloader will not modify or erase the cache partition.
 * It is recovery's responsibility to clean up the mess afterwards.
 */

int recovery_init (void)
{
	struct recovery_message msg;
	struct update_header header;
	char partition_name[32];
	unsigned valid_command = 0;

	// get recovery message
	if(get_recovery_message(&msg))
		return -1;
	if (msg.command[0] != 0 && msg.command[0] != 255) {
		dprintf("Recovery command: %.*s\n", sizeof(msg.command), msg.command);
	}
	msg.command[sizeof(msg.command)-1] = '\0'; //Ensure termination

	if (!strcmp("boot-recovery",msg.command)) {
		valid_command = 1;
		strcpy(msg.command, "");	// to safe against multiple reboot into recovery
		strcpy(msg.status, "OKAY");
		set_recovery_message(&msg);	// send recovery message
		// Boot in recovery mode
		return 1;
	}

	if (!strcmp("update-radio",msg.command)) {
		valid_command = 1;
		strcpy(partition_name, "FOTA");
	}

	//Todo: Add support for bootloader update too.

	if(!valid_command) {
		//We need not to do anything
		return 0; // Boot in normal mode
	}

	if (read_update_header_for_bootloader(&header)) {
		strcpy(msg.status, "invalid-update");
		goto SEND_RECOVERY_MSG;
	}

	if (update_firmware_image (&header, partition_name)) {
		strcpy(msg.status, "failed-update");
		goto SEND_RECOVERY_MSG;
	}
	strcpy(msg.status, "OKAY");

SEND_RECOVERY_MSG:
	strcpy(msg.command, "boot-recovery");
	set_recovery_message(&msg);	// send recovery message
	reboot_devices(0);
	return 0;
}


void recovery_mode(void)
{
	boot_img_hdr *hdr = (void *)raw_header;
	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;
	unsigned int size;
	const char *cmdline;
	loff_t off = 0;

	ret = mtdparts_init();
	if (ret != 0){
		printf("mtdparts init error %d\n", ret);
		return;
	}
	ret = find_dev_and_part(RECOVERY_PART, &dev, &pnum, &part);
	if(ret){
		printf("No partition named %s\n", RECOVERY_PART);
		return;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		printf("Partition %s not a NAND device\n", RECOVERY_PART);
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

#ifdef BOOT_DEBUG
	printf("function: %s kernel addr 0x%08x size 0x%08x, ramdisk: addr 0x%08x size 0x%08x\n", __FUNCTION__, hdr->kernel_addr, hdr->kernel_size, hdr->ramdisk_addr, hdr->ramdisk_size);
#endif
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
	printf("kernel @0x08x (0x08x bytes)\n", hdr->kernel_addr, hdr->kernel_size);
	printf("ramdisk @0x08x (0x08x bytes)\n", hdr->ramdisk_addr, hdr->ramdisk_size);
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

