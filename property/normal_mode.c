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
#include <malloc.h>

unsigned char raw_header[2048];
#ifdef FLASH_PAGE_SIZE
#undef FALSH_PAGE_SIZE
#endif
#define FLASH_PAGE_SIZE 2048

#define VMJALUNA_PART "vmjaluna"
#define MODEM_PART "modem"
#define KERNEL_PART "kernel"
#define FIXNV_PART "fixnv"
#define RUNTIMEVN_PART "runtimenv"
#define DSP_PART "dsp"

#define FIXNV_SIZE	(64 * 1024)
#define DSP_SIZE	(3968 * 1024)
#define VMJALUNA_SIZE	(256 * 1024)
#define RUNTIMENV_SIZE	(256 * 1024)
#define MODEM_SIZE	(8 * 1024 * 1024)
#define KERNEL_SIZE	(10 * 1024 * 1024)

#define FIXNV_ADR	0x00000000
#define DSP_ADR		0x00020000
#define VMJALUNA_ADR	0x00400000
#define RUNTIMENV_ADR	0x004a0000
#define MODEM_ADR	0x00500000

#ifdef CONFIG_SC8810_OPENPHONE
#define KERNEL_ADR	0x8000
#define RAMDISK_ADR 	0x04c00000

#else
#define KERNEL_ADR	0x04508000
#define RAMDISK_ADR 	0x04c00000

#endif

extern void cmd_yaffs_mount(char *mp);
extern void cmd_yaffs_umount(char *mp);
extern int cmd_yaffs_ls_chk(const char *dirfilename);
extern void cmd_yaffs_mread_file(char *fn, unsigned char *addr);


void nand_block_info(struct mtd_info *nand, int *good, int *bad)
{
	loff_t off;
	int goodblk, badblk;

	goodblk = badblk = 0;

	for (off = 0; off < nand->size; off += nand->erasesize)
		if (nand_block_isbad(nand, off)) {
			//printf("bad block :  %08llx\n", (unsigned long long)off);
			badblk ++;
		} else {
			//printf("good block : %08llx\n", (unsigned long long)off);
			goodblk ++;
		}
	*good = goodblk;
	*bad = badblk;
}

/*
* retval : -1 is wrong  ;  1 is correct
*/
int nv_is_correct(unsigned char *array, unsigned long size)
{
	if ((array[size] == 0x5a) && (array[size + 1] == 0x5a) && (array[size + 2] == 0x5a) && (array[size + 3] == 0x5a)) {
		array[size] = 0xff; array[size + 1] = 0xff;
		array[size + 2] = 0xff; array[size + 3] = 0xff;	
		return 1;
	} else
		return -1;
}

void array_value_range(unsigned char * array, int start, int end)
{
	int aaa;
	
	printf("\n\n");

	for (aaa = start; aaa <= end; aaa ++) {
		printf("arr[%d] = %02x\n", aaa, array[aaa]);
	}

	printf("\n\n");
}


void array_value(unsigned char * array, int len)
{
	int aaa;
	
	printf("\n\n");

	for (aaa = 0; aaa < len; aaa ++) {
		if ((aaa % 16) == 0)
			printf("\n");
		printf(" %02x", array[aaa]);
	}

	printf("\n\n");
}

void array_diff(unsigned char * array1, unsigned char * array2, int len)
{
	int ii;
	
	printf("arrar diff is starting   array1 = 0x%08x  array2 = 0x%08x  len = %d\n", (unsigned int)array1, (unsigned int)array2, len);
	for (ii = 0; ii < len; ii ++) {
		if (array1[ii] != array2[ii]) {
			printf("\narray1[%d] = 0x%02x  array2[%d] = 0x%02x\n", ii, array1[ii], ii, array2[ii]);
		}
	}
	printf("arrar diff is finished\n");
}

void vlx_nand_boot(char * kernel_pname, char * cmdline)
{
    	boot_img_hdr *hdr = (void *)raw_header;
	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;
	size_t size;
	loff_t off = 0;
	char *backupfixnvpoint = "/backupfixnv";
	char *backupfixnvfilename = "/backupfixnv/fixnv.bin";
	char *runtimenvpoint = "/runtimenv";
	char *runtimenvfilename = "/runtimenv/runtimenv.bin";

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
    extern void lcd_display(void);
    extern void set_backlight(uint32_t value);
    lcd_display_bitmap((ulong)bmp_img, 0, 0);
    lcd_display();
    set_backlight(50);
#endif

	/*int good_blknum, bad_blknum;
	nand_block_info(nand, &good_blknum, &bad_blknum);
	printf("good is %d  bad is %d\n", good_blknum, bad_blknum);*/
	///////////////////////////////////////////////////////////////////////
	/* FIXNV_PART */
	printf("Reading fixnv to 0x%08x\n", FIXNV_ADR);
	memset((unsigned char *)DSP_ADR, 0xff, FIXNV_SIZE + 4);
	memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
#if 0
	/* mtd nv */
	ret = find_dev_and_part(FIXNV_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", FIXNV_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", FIXNV_PART);
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
    	cmd_yaffs_mount(backupfixnvpoint);
	ret = cmd_yaffs_ls_chk(backupfixnvfilename);
	if (ret == (FIXNV_SIZE + 4)) {
		/* there is need FIXNV_ADR instead of DSP_ADR, but FIXNV_ADR is 0x00000000, 
	 	* yaffs_read occur error at first page, so i use DSP_ADR, then copy data from DSP_ADR to FIXNV_ADR.
		*/
		cmd_yaffs_mread_file(backupfixnvfilename, (unsigned char *)DSP_ADR);
		if (-1 == nv_is_correct((unsigned char *)DSP_ADR, FIXNV_SIZE)) {
			cmd_yaffs_umount(backupfixnvpoint);
			/* file is wrong */			
			memset((unsigned char *)DSP_ADR, 0xff, FIXNV_SIZE + 4);
			/* read fixnv */
			//////////////////////
			ret = find_dev_and_part(FIXNV_PART, &dev, &pnum, &part);
			if (ret) {
				printf("No partition named %s\n", FIXNV_PART);
				return;
			} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
				printf("Partition %s not a NAND device\n", FIXNV_PART);
				return;
			}

			off = part->offset;
			nand = &nand_info[dev->id->num];

			size = (FIXNV_SIZE + (FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
			if(size <= 0) {
				printf("fixnv image should not be zero\n");
				return;
			}
			ret = nand_read_offset_ret(nand, off, &size, (void*)DSP_ADR, &off);
			if (ret != 0) {
				printf("fixnv nand read error %d\n", ret);
				return;
			}
			//////////////////////
		} else {
			/* file is right */
			cmd_yaffs_umount(backupfixnvpoint);
		}
	} else {
		cmd_yaffs_umount(backupfixnvpoint);
		/* file do not exist */
		memset((unsigned char *)DSP_ADR, 0xff, FIXNV_SIZE + 4);
		/* read fixnv */
		//////////////////////
		ret = find_dev_and_part(FIXNV_PART, &dev, &pnum, &part);
		if (ret) {
			printf("No partition named %s\n", FIXNV_PART);
			return;
		} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
			printf("Partition %s not a NAND device\n", FIXNV_PART);
			return;
		}

		off = part->offset;
		nand = &nand_info[dev->id->num];

		size = (FIXNV_SIZE + (FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
		if(size <= 0) {
			printf("fixnv image should not be zero\n");
			return;
		}
		ret = nand_read_offset_ret(nand, off, &size, (void*)DSP_ADR, &off);
		if (ret != 0) {
			printf("fixnv nand read error %d\n", ret);
			return;
		}
		//////////////////////
	}
	memcpy((unsigned char *)FIXNV_ADR, (unsigned char *)DSP_ADR, FIXNV_SIZE);
#endif
	//array_value((unsigned char *)FIXNV_ADR, FIXNV_SIZE);

	///////////////////////////////////////////////////////////////////////
	/* RUNTIMEVN_PART */
	printf("Reading runtimenv to 0x%08x\n", RUNTIMENV_ADR);
#if 0
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
	ret = cmd_yaffs_ls_chk(runtimenvfilename);
	if (ret == (RUNTIMENV_SIZE + 4)) {
		cmd_yaffs_mread_file(runtimenvfilename, (unsigned char *)RUNTIMENV_ADR);
		if (-1 == nv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE))
			memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + 4);
	} else
		memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + 4);
	cmd_yaffs_umount(runtimenvpoint);
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

	ret = find_dev_and_part(kernel_pname, &dev, &pnum, &part);
	if(ret){
		printf("No partition named %s\n", kernel_pname);
		return;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		printf("Partition %s not a NAND device\n", kernel_pname);
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
	ret = nand_read_offset_ret(nand, off, &size, (void *)KERNEL_ADR, &off);
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
	ret = nand_read_offset_ret(nand, off, &size, (void *)RAMDISK_ADR, &off);
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
    char * buf;
    buf = malloc(150);
    sprintf(buf, "initrd=0x%x,0x%x", RAMDISK_ADR, hdr->ramdisk_size);

    if(cmdline && cmdline[0]){
        int str_len;
        str_len = strlen(buf);
        sprintf(&buf[str_len], " %s", cmdline);
    }

    printf("pass cmdline: %s\n", buf);
    creat_atags(VLX_TAG_ADDR, buf, NULL, 0);
#endif

	void (*entry)(void) = (void*) VMJALUNA_ADR;
	entry();
}
#ifdef CONFIG_SC8810_OPENPHONE
/* 
 * example ARM Linux bootloader code
 * this example is distributed under the BSD licence
 */

/* list of possible tags */
#define ATAG_NONE       0x00000000
#define ATAG_CORE       0x54410001
#define ATAG_MEM        0x54410002
#define ATAG_VIDEOTEXT  0x54410003
#define ATAG_RAMDISK    0x54410004
#define ATAG_INITRD2    0x54420005
#define ATAG_SERIAL     0x54410006
#define ATAG_REVISION   0x54410007
#define ATAG_VIDEOLFB   0x54410008
#define ATAG_CMDLINE    0x54410009

//config begin
//steve.zhan add for normal linux boot.
#define DRAM_BASE		(0X0)
#define ZIMAGE_LOAD_ADDRESS     (DRAM_BASE + 0X8000)
#define INITRD_IN_SDRAM 	(0X1a00000)
#define INITRD_LOAD_ADDRESS	(DRAM_BASE + INITRD_IN_SDRAM)
#define MEM_INDEX_O_SIZE	(256*1024*1024)
#define ROOTFS_SIZE		(2040834)

#define u8 char
#define u16 short
#define u32 unsigned int
#define u32_t unsigned int

static int get_mach_type(void)
{
	return 0x7dd;
}
static void irq_shutdown(void)
{
	
}
static void cpu_op(void)
{

}

void load_image_kernel(char * imageN, unsigned int addr, unsigned copySize)
{
	boot_img_hdr *hdr = (void *)raw_header;
	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part = NULL;
	u8 pnum;
	int ret;
	size_t size;
	loff_t off = 0;
	char *backupfixnvpoint = "/backupfixnv";
	char *backupfixnvfilename = "/backupfixnv/fixnv.bin";
	char *runtimenvpoint = "/runtimenv";
	char *runtimenvfilename = "/runtimenv/runtimenv.bin";

	ret = mtdparts_init();
	if (ret != 0){
		printf("mtdparts init error %d\n", ret);
		return;
	}

	printf("Reading kernel to 0x%08x\n", addr);

	ret = find_dev_and_part(imageN, &dev, &pnum, &part);
	if(ret || part == NULL){
		printf("No partition named %s\n", imageN);
		return;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		printf("Partition %s not a NAND device\n", imageN);
		return;
	} else {
		printf("name = %s, auto_name = 0x%x, size = 0x%x, offset = 0x%x, sector_size = 0x%x", \
			part->name, part->auto_name, part->size, part->offset, part->sector_size);
	}

	off=part->offset;
	nand = &nand_info[dev->id->num];

	//read  image
	if (copySize > part->size)
	{
		printf("the kernel partion size is smaller than copyKernelSize,Only copy part of kernel\n");
		copySize = part->size;
	}
	
	size = (copySize + (FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
	if(size <=0){
		printf("kernel image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void *)addr, &off);
	if(ret != 0){
		printf("kernel nand read error %d\n", ret);
		return;
	}

}

//config end


/* structures for each atag */
struct atag_header {
        u32 size; /* length of tag in words including this header */
        u32 tag;  /* tag type */
};

struct atag_core {
        u32 flags;
        u32 pagesize;
        u32 rootdev;
};

struct atag_mem {
        u32     size;
        u32     start;
};

struct atag_videotext {
        u8              x;
        u8              y;
        u16             video_page;
        u8              video_mode;
        u8              video_cols;
        u16             video_ega_bx;
        u8              video_lines;
        u8              video_isvga;
        u16             video_points;
};

struct atag_ramdisk {
        u32 flags;
        u32 size;
        u32 start;
};

struct atag_initrd2 {
        u32 start;
        u32 size;
};

struct atag_serialnr {
        u32 low;
        u32 high;
};

struct atag_revision {
        u32 rev;
};

struct atag_videolfb {
	u16             lfb_width;
	u16             lfb_height;
	u16             lfb_depth;
	u16             lfb_linelength;
	u32             lfb_base;
	u32             lfb_size;
	u8              red_size;
	u8              red_pos;
	u8              green_size;
	u8              green_pos;
	u8              blue_size;
	u8              blue_pos;
	u8              rsvd_size;
	u8              rsvd_pos;
};

struct atag_cmdline {
        char    cmdline[64];
};

struct atag {
	struct atag_header hdr;
	union {
		struct atag_core         core;
		struct atag_mem          mem;
		struct atag_videotext    videotext;
		struct atag_ramdisk      ramdisk;
		struct atag_initrd2      initrd2;
		struct atag_serialnr     serialnr;
		struct atag_revision     revision;
		struct atag_videolfb     videolfb;
		struct atag_cmdline      cmdline;
	} u;
};


#define tag_next(t)     ((struct atag *)((u32 *)(t) + (t)->hdr.size))
#define tag_size(type)  ((sizeof(struct atag_header) + sizeof(struct type)) >> 2)
static struct atag *params; /* used to point at the current tag */

static void setup_core_tag(void * address,long pagesize)
{
	params = (struct atag *)address;         /* Initialise parameters to start at given address */
	params->hdr.tag = ATAG_CORE;            /* start with the core tag */
	params->hdr.size = tag_size(atag_core); /* size the tag */

	params->u.core.flags = 1;               /* ensure read-only */
	params->u.core.pagesize = pagesize;     /* systems pagesize (4k) */
	params->u.core.rootdev = 0;             /* zero root device (typicaly overidden from commandline )*/

	params = tag_next(params);              /* move pointer to next tag */
}

static void setup_ramdisk_tag(u32_t size)
{
	params->hdr.tag = ATAG_RAMDISK;         /* Ramdisk tag */
	params->hdr.size = tag_size(atag_ramdisk);  /* size tag */
	params->u.ramdisk.flags = 0;            /* Load the ramdisk */
	params->u.ramdisk.size = size;          /* Decompressed ramdisk size */
	params->u.ramdisk.start = 0;            /* Unused */
	params = tag_next(params);              /* move pointer to next tag */
}

static void
setup_initrd2_tag(u32_t start, u32_t size)
{
	params->hdr.tag = ATAG_INITRD2;         /* Initrd2 tag */
	params->hdr.size = tag_size(atag_initrd2);  /* size tag */
	params->u.initrd2.start = start;        /* physical start */
	params->u.initrd2.size = size;          /* compressed ramdisk size */
	params = tag_next(params);              /* move pointer to next tag */
}

static void setup_mem_tag(u32_t start, u32_t len)
{
	params->hdr.tag = ATAG_MEM;             /* Memory tag */
	params->hdr.size = tag_size(atag_mem);  /* size tag */
	params->u.mem.start = start;            /* Start of memory area (physical address) */
	params->u.mem.size = len;               /* Length of area */
	params = tag_next(params);              /* move pointer to next tag */
}

static void setup_cmdline_tag(const char * line)
{
	int linelen = strlen(line);

	if(!linelen)
		return;                             /* do not insert a tag for an empty commandline */

	params->hdr.tag = ATAG_CMDLINE;         /* Commandline tag */
	params->hdr.size = (sizeof(struct atag_header) + linelen + 1 + 4) >> 2;
	strcpy(params->u.cmdline.cmdline,line); /* place commandline into tag */
	params = tag_next(params);              /* move pointer to next tag */
}

static void setup_end_tag(void)
{
	params->hdr.tag = ATAG_NONE;            /* Empty tag ends list */
	params->hdr.size = 0;                   /* zero length */
}


static void setup_tags(void *parameters)
{
	setup_core_tag(parameters, 4096);       /* standard core tag 4k pagesize */
	setup_mem_tag(DRAM_BASE, MEM_INDEX_O_SIZE);    /* 64Mb at 0x10000000 */
	setup_ramdisk_tag(4096*2);                /* create 8Mb ramdisk */ 
	setup_initrd2_tag(INITRD_LOAD_ADDRESS, ROOTFS_SIZE); /* 1Mb of compressed data placed 8Mb into memory */
	setup_cmdline_tag("console=ttyS1,115200n8 root=/dev/ram0 rw init=/init " MTDPARTS_DEFAULT);    /* commandline setting root device */
	setup_end_tag();                    /* end of tags */
}

static int start_linux()
{
	void (*theKernel)(int zero, int arch, u32 params);
	u32 exec_at = (u32)-1;
	u32 parm_at = (u32)-1;
	u32 machine_type;

	exec_at = ZIMAGE_LOAD_ADDRESS;
	parm_at = DRAM_BASE + 0x100;

	*(volatile u32*)0x84001000 = 'c';
	*(volatile u32*)0x84001000 = 'p';
	*(volatile u32*)0x84001000 = 'k';

	load_image_kernel(BOOT_PART, exec_at, COPY_LINUX_KERNEL_SIZE);/* copy image into RAM */

	*(volatile u32*)0x84001000 = 'c';
	*(volatile u32*)0x84001000 = 'p';
	*(volatile u32*)0x84001000 = 'f';

	load_image_kernel(LINUX_INITRD_NAME,INITRD_LOAD_ADDRESS, ROOTFS_SIZE);/* copy initial ramdisk image into RAM */

	setup_tags((void*)parm_at);                    /* sets up parameters */

	machine_type = get_mach_type();         /* get machine type */

	irq_shutdown();                         /* stop irq */

	cpu_op();          /* turn MMU off */

	theKernel = (void (*)(int, int, u32))exec_at; /* set the kernel address */
	*(volatile u32*)0x84001000 = 'j';
	*(volatile u32*)0x84001000 = 'm';
	*(volatile u32*)0x84001000 = 'p';
	theKernel(0, machine_type, parm_at);    /* jump to kernel with register set */

	return 0;
}

#endif

void normal_mode(void)
{
#ifdef CONFIG_SC8810_OPENPHONE
	start_linux();
#else
	vlx_nand_boot(BOOT_PART, NULL);
#endif
}
