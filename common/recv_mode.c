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

#define VMJALUNA_PART "vmjaluna"
#define MODEM_PART "modem"
#define KERNEL_PART "kernel"
#define FIXVN_PART "fixnv"
#define RUNTIMEVN_PART "runtimenv"
#define DSP_PART "dsp"

#define VMJALUNA_SIZE	(0x40000)
#define MODEM_SIZE	(5973388)
#define KERNEL_SIZE	(7474533)
#define FIXNV_SIZE	(64 * 1024)
#define RUNTIMENV_SIZE	(512 * 1024)
#define DSP_SIZE	(4063232)

#define VMJALUNA_ADR	0x00400000
#define MODEM_ADR	0x00500000
#define KERNEL_ADR	0x04508000
#define RAMDISK_ADR 0x04c00000
#define FIXNV_ADR	0x00000000
#define RUNTIMENV_ADR	0x004a0000
#define DSP_ADR		0x00020000

extern void cmd_yaffs_mount(char *mp);
extern void cmd_yaffs_umount(char *mp);
extern void cmd_yaffs_read_file(char *fn);
extern void cmd_yaffs_ls(const char *mountpt, int longlist);
extern void cmd_yaffs_mread_file(char *fn, unsigned char *addr);

extern unsigned int caliberate_mode;

void recovery_mode(void)
{
    boot_img_hdr *hdr = (void *)raw_header;
	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;
	size_t size;
	loff_t off = 0;
	char *fixnvpoint = "/fixnv";
	char *fixnvfilename = "/fixnv/nvitem.bin";
	char *runtimvnvpoint = "/runtimenv";
	char *runtimenvfilename = "/runtime/runtimenvitem.bin";

	ret = mtdparts_init();
	if (ret != 0){
		printf("mtdparts init error %d\n", ret);
		return;
	}
#ifdef CONFIG_SPLASH_SCREEN 
#define SPLASH_PART "boot_logo"

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
	size = 1<<19;
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
    lcd_display();
    set_backlight(50);
#endif
	///////////////////////////////////////////////////////////////////////
	/* FIXVN_PART */
	printf("Reading fixnv to 0x%08x\n", FIXNV_ADR);
#if 1
	/* mtd nv */
	ret = find_dev_and_part(FIXVN_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", FIXVN_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", FIXVN_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];

	size = (FIXNV_SIZE + (FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
	if(size <= 0) {
		printf("fixnv image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)FIXNV_ADR, &off);
	if (ret != 0) {
		printf("fixnv nand read error %d\n", ret);
		return;
	}
#else
	/* fixnv */
    	cmd_yaffs_mount(fixnvpoint);
	/* there is need FIXNV_ADR instead of DSP_ADR, but FIXNV_ADR is 0x00000000, 
	 * yaffs_read occur error at first page, so i use DSP_ADR, then copy data from DSP_ADR to FIXNV_ADR.
	*/
    	cmd_yaffs_mread_file(fixnvfilename, (unsigned char *)DSP_ADR);
	cmd_yaffs_umount(fixnvpoint);
	memcpy((unsigned char *)FIXNV_ADR, (unsigned char *)DSP_ADR, FIXNV_SIZE);
#endif
	//array_value((unsigned char *)FIXNV_ADR, FIXNV_SIZE);

	///////////////////////////////////////////////////////////////////////
	/* RUNTIMEVN_PART */
	printf("Reading runtimenv to 0x%08x\n", RUNTIMENV_ADR);
#if 1
	/* mtd nv */
	ret = find_dev_and_part(RUNTIMEVN_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", RUNTIMEVN_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", RUNTIMEVN_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];

	size = (RUNTIMENV_SIZE + (FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
	if(size <= 0) {
		printf("runtimenv image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)RUNTIMENV_ADR, &off);
	if (ret != 0) {
		printf("runtimenv nand read error %d\n", ret);
		return;
	}
#else
	/* runtimenv */
    	cmd_yaffs_mount(runtimenvpoint);
	/* there is need RUNTIMENV_ADR instead of DSP_ADR, but RUNTIMENV_ADR is 0x00000000, 
	 * yaffs_read occur error at first page, so i use DSP_ADR, then copy data from DSP_ADR to RUNTIMENV_ADR.
	*/
    	cmd_yaffs_mread_file(runtimvenvfilename, (unsigned char *)DSP_ADR);
	cmd_yaffs_umount(runtimenvpoint);
	memcpy((unsigned char *)RUNTIMENV_ADR, (unsigned char *)DSP_ADR, RUNTIMENV_SIZE);
#endif
	//array_value((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE);

	////////////////////////////////////////////////////////////////
	/* DSP_PART */
	printf("Reading dsp to 0x%08x\n", DSP_ADR);
	ret = find_dev_and_part(DSP_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", DSP_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", DSP_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];

	size = (DSP_SIZE + (FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
	if(size <= 0) {
		printf("dsp image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)DSP_ADR, &off);
	if(ret != 0) {
		printf("dsp nand read error %d\n", ret);
		return;
	}

	//array_value((unsigned char *)DSP_ADR, DSP_SIZE);

	////////////////////////////////////////////////////////////////
	/* KERNEL_PART */
	printf("Reading kernel to 0x%08x\n", KERNEL_ADR);

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

	//read kernel image
	size = (hdr->kernel_size+(FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
	if(size <=0){
		printf("kernel image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, KERNEL_ADR, &off);
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
	ret = nand_read_offset_ret(nand, off, &size, RAMDISK_ADR, &off);
	if(ret != 0){
		printf("ramdisk nand read error %d\n", ret);
		return;
	}
	//array_value((unsigned char *)KERNEL_ADR, KERNEL_SIZE);

	////////////////////////////////////////////////////////////////
	/* MODEM_PART */
	printf("Reading modem to 0x%08x\n", MODEM_ADR);
	ret = find_dev_and_part(MODEM_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", MODEM_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", MODEM_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];

	size = (MODEM_SIZE +(FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
	if(size <= 0) {
		printf("modem image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)MODEM_ADR, &off);
	if(ret != 0) {
		printf("modem nand read error %d\n", ret);
		return;
	}

	//array_value((unsigned char *)MODEM_ADR, MODEM_SIZE);

	////////////////////////////////////////////////////////////////
	/* VMJALUNA_PART */
	printf("Reading vmjaluna to 0x%08x\n", VMJALUNA_ADR);
	ret = find_dev_and_part(VMJALUNA_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", VMJALUNA_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", VMJALUNA_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];

	size = (VMJALUNA_SIZE +(FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
	if(size <= 0) {
		printf("modem image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)VMJALUNA_ADR, &off);
	if(ret != 0) {
		printf("modem nand read error %d\n", ret);
		return;
	}

	//array_value((unsigned char *)VMJALUNA_ADR, 16 * 10);
    //check caliberation mode
#ifdef CONFIG_MODEM_CALIBERATE
#define VLX_TAG_ADDR 0x5100000 //after initrd
    printf("caliberate mode is %d\n", caliberate_mode);
    if(caliberate_mode){
        char buf[20];
        sprintf(buf, "calibration=%d", caliberate_mode);
        creat_atags(VLX_TAG_ADDR, buf, RAMDISK_ADR, hdr->ramdisk_size);
    }
#else
    creat_atags(VLX_TAG_ADDR, NULL, RAMDISK_ADR, hdr->ramdisk_size);
#endif

	void (*entry)(void) = (void*) VMJALUNA_ADR;
	entry();
}
