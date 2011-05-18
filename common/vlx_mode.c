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

unsigned char vlx_raw_header[2048];
#ifdef FLASH_PAGE_SIZE
#undef FALSH_PAGE_SIZE
#endif
#define FLASH_PAGE_SIZE 2048

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
#define FIXNV_ADR	0x00000000
#define RUNTIMENV_ADR	0x004a0000
#define DSP_ADR		0x00020000

extern void cmd_yaffs_mount(char *mp);
extern void cmd_yaffs_umount(char *mp);
extern void cmd_yaffs_read_file(char *fn);
extern void cmd_yaffs_ls(const char *mountpt, int longlist);
extern void cmd_yaffs_mread_file(char *fn, unsigned char *addr);

extern unsigned int caliberate_mode;

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
	
	printf("arrar diff is starting   array1 = 0x%08x  array2 = 0x%08x  len = %d\n", array1, array2, len);
	for (ii = 0; ii < len; ii ++) {
		if (array1[ii] != array2[ii]) {
			printf("\narray1[%d] = 0x%02x  array2[%d] = 0x%02x\n", ii, array1[ii], ii, array2[ii]);
		}
	}
	printf("arrar diff is finished\n");
}

void vlx_mode(void)
{
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
    unsigned char *array;

	ret = mtdparts_init();
	if (ret != 0){
		printf("mtdparts init error %d\n", ret);
		return;
	}

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
    array = (unsigned char *)FIXNV_ADR;
    memset(array, 0xff, FIXNV_SIZE);
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
	ret = find_dev_and_part(KERNEL_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", KERNEL_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", KERNEL_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];

	size = (KERNEL_SIZE +(FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
	if (size <= 0) {
		printf("kernel image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)KERNEL_ADR, &off);
	if (ret != 0) {
		printf("kernel nand read error %d\n", ret);
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
        creat_atags(VLX_TAG_ADDR, buf, NULL, 0);
    }
#endif

	void (*entry)(void) = (void*) VMJALUNA_ADR;
	entry();
}
