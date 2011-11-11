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

#define DSP_SIZE		(3968 * 1024)
#define VMJALUNA_SIZE		(300 * 1024)
#define FIXNV_SIZE		(64 * 1024)
#define PRODUCTINFO_SIZE	(3 * 1024)
#define RUNTIMENV_SIZE		(256 * 1024)
#ifdef MCP_F2R1
#define MODEM_SIZE		(3500 * 1024)  	/* 3.5MB */
#define KERNEL_SIZE		(6500 * 1024)	/* 6.5MB */
#else
#define MODEM_SIZE		(8 * 1024 * 1024)
#define KERNEL_SIZE		(10 * 1024 * 1024)
#endif

//
//




#define DSP_ADR			0x00020000
#define VMJALUNA_ADR		0x00400000
#define FIXNV_ADR		0x00480000
#define PRODUCTINFO_ADR		0x00490000
#define RUNTIMENV_ADR		0x004a0000
#define MODEM_ADR		0x00500000
#define RAMDISK_ADR 		0x04c00000


#if BOOT_NATIVE_LINUX
//pls make sure uboot running area
#define VLX_TAG_ADDR            (0x100)
#define KERNEL_ADR		(0x8000)

#else

#define KERNEL_ADR		0x04508000
#define VLX_TAG_ADDR            0x5100000 //after initrd

#endif

#define MAX_SN_LEN 			(24)
#define SP09_MAX_SN_LEN			MAX_SN_LEN
#define SP09_MAX_STATION_NUM		(15)
#define SP09_MAX_STATION_NAME_LEN	(10)
#define SP09_SPPH_MAGIC_NUMBER          (0X53503039)    // "SP09"
#define SP09_MAX_LAST_DESCRIPTION_LEN   (32)
typedef struct _tagSP09_PHASE_CHECK
{
	unsigned long 	Magic;                	// "SP09"
	char    	SN1[SP09_MAX_SN_LEN]; 	// SN , SN_LEN=24
	char    	SN2[SP09_MAX_SN_LEN];    // add for Mobile
	int     	StationNum;                 	// the test station number of the testing
	char    	StationName[SP09_MAX_STATION_NUM][SP09_MAX_STATION_NAME_LEN];
	unsigned char 	Reserved[13];               	// 
	unsigned char 	SignFlag;
	char    	szLastFailDescription[SP09_MAX_LAST_DESCRIPTION_LEN];
	unsigned short  iTestSign;				// Bit0~Bit14 ---> station0~station 14 
	unsigned short  iItem;    // part1: Bit0~ Bit_14 indicate test Station,1¡À¨ª¨º\u0178Pass,    
}SP09_PHASE_CHECK_T, *LPSP09_PHASE_CHECK_T;
const static int SP09_MAX_PHASE_BUFF_SIZE = sizeof(SP09_PHASE_CHECK_T);
int eng_getphasecheck(SP09_PHASE_CHECK_T* phase_check)
{
	unsigned char *point;
	int aaa;
	point = (unsigned char *)phase_check;
	if (phase_check->Magic == SP09_SPPH_MAGIC_NUMBER) {
		printf("SN1 = %s   SN2 = %s\n",phase_check->SN1, phase_check->SN2);
		for (aaa = 0; aaa < phase_check->StationNum/*SP09_MAX_STATION_NUM*/; aaa ++) {
			printf("%s  ", phase_check->StationName[aaa]);
		}
		printf("\n");
	} else
		printf("no production information / phase check!\n");
	return 0;
}
int eng_phasechecktest(unsigned char *array, int len)
{
	SP09_PHASE_CHECK_T phase;
	memset(&phase, 0, sizeof(SP09_PHASE_CHECK_T));
	memcpy(&phase, array, len);
	return eng_getphasecheck(&phase);
}
extern void cmd_yaffs_mount(char *mp);
extern void cmd_yaffs_umount(char *mp);
extern int cmd_yaffs_ls_chk(const char *dirfilename);
extern void cmd_yaffs_mread_file(char *fn, unsigned char *addr);
void set_vibrator(int on);
void MMU_InvalideICACHEALL(void);

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
static int start_linux()
{
	void (*theKernel)(int zero, int arch, u32 params);
	u32 exec_at = (u32)-1;
	u32 parm_at = (u32)-1;
	u32 machine_type;

	machine_type = 0x7dd;         /* get machine type */

	theKernel = (void (*)(int, int, u32))KERNEL_ADR; /* set the kernel address */
	*(volatile u32*)0x84001000 = 'j';
	*(volatile u32*)0x84001000 = 'm';
	*(volatile u32*)0x84001000 = 'p';

	*(volatile u32*)(0x20900000 + 0x218) |= (0x1);//internal ram using 0xffff0000
	theKernel(0, machine_type, VLX_TAG_ADDR);    /* jump to kernel with register set */
	while(1);
	return 0;
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
	char *fixnvpoint = "/fixnv";
	char *fixnvfilename = "/fixnv/fixnv.bin";
	char *fixnvfilename2 = "/fixnv/fixnvchange.bin";
	char *backupfixnvpoint = "/backupfixnv";
	char *backupfixnvfilename = "/backupfixnv/fixnv.bin";
	char *backupfixnvfilename2 = "/backupfixnv/fixnvchange.bin";
	char *runtimenvpoint = "/runtimenv";
	char *runtimenvfilename = "/runtimenv/runtimenv.bin";
	char *runtimenvfilename2 = "/runtimenv/runtimenvchange.bin";
	char *productinfopoint = "/productinfo";
	char *productinfofilename = "/productinfo/productinfo.bin";
	char *productinfofilename2 = "/productinfo/productinfochange.bin";
        char * mtdpart_def = NULL;

	ret = mtdparts_init();
	if (ret != 0){
		printf("mtdparts init error %d\n", ret);
		//return;
	}

#ifdef CONFIG_SPLASH_SCREEN 
#define SPLASH_PART "boot_logo"

	ret = find_dev_and_part(SPLASH_PART, &dev, &pnum, &part);
	if(ret){
		printf("No partition named %s\n", SPLASH_PART);
		//return;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		printf("Partition %s not a NAND device\n", SPLASH_PART);
		//return;
	}

	off=part->offset;
	nand = &nand_info[dev->id->num];
	//read boot image header
	size = 1<<19;
    char * bmp_img = malloc(size);
    if(!bmp_img){
        printf("not enough memory for splash image\n");
        //return;
    }
	ret = nand_read_offset_ret(nand, off, &size, (void *)bmp_img, &off);
	if(ret != 0){
		printf("function: %s nand read error %d\n", __FUNCTION__, ret);
		//return;
	}
    extern int lcd_display_bitmap(ulong bmp_image, int x, int y);
    extern void lcd_display(void);
    extern void set_backlight(uint32_t value);
    lcd_display_bitmap((ulong)bmp_img, 0, 0);
    lcd_display();
    set_backlight(50);
#endif//CONFIG_SPLASH_SCREEN

    set_vibrator(0);

#if !(BOOT_NATIVE_LINUX)
	/*int good_blknum, bad_blknum;
	nand_block_info(nand, &good_blknum, &bad_blknum);
	printf("good is %d  bad is %d\n", good_blknum, bad_blknum);*/
	///////////////////////////////////////////////////////////////////////
	/* FIXNV_PART */
	printf("Reading fixnv to 0x%08x\n", FIXNV_ADR);
	memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
    	cmd_yaffs_mount(backupfixnvpoint);
	ret = cmd_yaffs_ls_chk(backupfixnvfilename);
	if (ret == (FIXNV_SIZE + 4)) {
		cmd_yaffs_mread_file(backupfixnvfilename, (unsigned char *)FIXNV_ADR);
		if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
			memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
			ret = cmd_yaffs_ls_chk(backupfixnvfilename2);
			if (ret == (FIXNV_SIZE + 4)) {
				cmd_yaffs_mread_file(backupfixnvfilename2, (unsigned char *)FIXNV_ADR);
				if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
					cmd_yaffs_umount(backupfixnvpoint);
					memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
	/* mtd nv */
   			cmd_yaffs_mount(fixnvpoint);
			ret = cmd_yaffs_ls_chk(fixnvfilename);
			if (ret == (FIXNV_SIZE + 4)) {
				cmd_yaffs_mread_file(fixnvfilename, (unsigned char *)FIXNV_ADR);
				if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
					memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
					/* read fixnv backup */
					ret = cmd_yaffs_ls_chk(fixnvfilename2);
					if (ret == (FIXNV_SIZE + 4)) {
						cmd_yaffs_mread_file(fixnvfilename2, (unsigned char *)FIXNV_ADR);
						if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
							memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
					}
				}
			} else {
				/* read fixnv backup */
				ret = cmd_yaffs_ls_chk(fixnvfilename2);
				if (ret == (FIXNV_SIZE + 4)) {
					cmd_yaffs_mread_file(fixnvfilename2, (unsigned char *)FIXNV_ADR);
					if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
						memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
				}
				/* read fixnv backup */
			}
			cmd_yaffs_umount(fixnvpoint);
					/*#########################*/
			}
		} else {
				cmd_yaffs_umount(backupfixnvpoint);
				memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
	/* fixnv */
    			cmd_yaffs_mount(fixnvpoint);
			ret = cmd_yaffs_ls_chk(fixnvfilename);
	if (ret == (FIXNV_SIZE + 4)) {
		/* there is need FIXNV_ADR instead of DSP_ADR, but FIXNV_ADR is 0x00000000, 
	 	* yaffs_read occur error at first page, so i use DSP_ADR, then copy data from DSP_ADR to FIXNV_ADR.
		*/
				cmd_yaffs_mread_file(fixnvfilename, (unsigned char *)FIXNV_ADR);
				if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
					memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
					ret = cmd_yaffs_ls_chk(fixnvfilename2);
					if (ret == (FIXNV_SIZE + 4)) {
						cmd_yaffs_mread_file(fixnvfilename2, (unsigned char *)FIXNV_ADR);
						if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
							memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
					}
			/* read fixnv */
			//////////////////////
			}
			} else {
				/* read fixnv backup */
				ret = cmd_yaffs_ls_chk(fixnvfilename2);
				if (ret == (FIXNV_SIZE + 4)) {
					cmd_yaffs_mread_file(fixnvfilename2, (unsigned char *)FIXNV_ADR);
					if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
						memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
				}
				/* read fixnv backup */
			}
			cmd_yaffs_umount(fixnvpoint);
				/*#########################*/
			}
			//////////////////////
		} else {
			/* file is right */
			cmd_yaffs_umount(backupfixnvpoint);
		}
	} else {
		memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
		ret = cmd_yaffs_ls_chk(backupfixnvfilename2);
		if (ret == (FIXNV_SIZE + 4)) {
			cmd_yaffs_mread_file(backupfixnvfilename2, (unsigned char *)FIXNV_ADR);
			if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
		cmd_yaffs_umount(backupfixnvpoint);
		/* file do not exist */
				memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
		/* read fixnv */
    				cmd_yaffs_mount(fixnvpoint);
				ret = cmd_yaffs_ls_chk(fixnvfilename);
				if (ret == (FIXNV_SIZE + 4)) {
					cmd_yaffs_mread_file(fixnvfilename, (unsigned char *)FIXNV_ADR);
					if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
						memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
						ret = cmd_yaffs_ls_chk(fixnvfilename2);
						if (ret == (FIXNV_SIZE + 4)) {
							cmd_yaffs_mread_file(fixnvfilename2, (unsigned char *)FIXNV_ADR);
							if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
								memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
						}
						/* read fixnv backup */
					}
				} else {
					ret = cmd_yaffs_ls_chk(fixnvfilename2);
					if (ret == (FIXNV_SIZE + 4)) {
						cmd_yaffs_mread_file(fixnvfilename2, (unsigned char *)FIXNV_ADR);
						if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
							memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
		}
				}
				cmd_yaffs_umount(fixnvpoint);
				/*#########################*/
			}
		} else {
			cmd_yaffs_umount(backupfixnvpoint);
			memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
    			cmd_yaffs_mount(fixnvpoint);
			ret = cmd_yaffs_ls_chk(fixnvfilename);
			if (ret == (FIXNV_SIZE + 4)) {
				cmd_yaffs_mread_file(fixnvfilename, (unsigned char *)FIXNV_ADR);
				if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
					memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
					ret = cmd_yaffs_ls_chk(fixnvfilename2);
					if (ret == (FIXNV_SIZE + 4)) {
						cmd_yaffs_mread_file(fixnvfilename2, (unsigned char *)FIXNV_ADR);
						if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
							memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
					}
		}
			} else {
				ret = cmd_yaffs_ls_chk(fixnvfilename2);
				if (ret == (FIXNV_SIZE + 4)) {
					cmd_yaffs_mread_file(fixnvfilename2, (unsigned char *)FIXNV_ADR);
					if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
						memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
				}
			}
			cmd_yaffs_umount(fixnvpoint);
		}
		//////////////////////
	}
	//array_value((unsigned char *)FIXNV_ADR, FIXNV_SIZE);

	///////////////////////////////////////////////////////////////////////
	/* PRODUCTINFO_PART */
	printf("Reading productinfo to 0x%08x\n", PRODUCTINFO_ADR);
	/* runtimenv */
    	cmd_yaffs_mount(productinfopoint);
	ret = cmd_yaffs_ls_chk(productinfofilename);
	if (ret == (PRODUCTINFO_SIZE + 4)) {
		cmd_yaffs_mread_file(productinfofilename, (unsigned char *)PRODUCTINFO_ADR);
		if (-1 == nv_is_correct((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE)) {
			memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + 4);
			ret = cmd_yaffs_ls_chk(productinfofilename2);
			if (ret == (PRODUCTINFO_SIZE + 4)) {
				cmd_yaffs_mread_file(productinfofilename2, (unsigned char *)PRODUCTINFO_ADR);
				if (-1 == nv_is_correct((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE)) {
					memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + 4);
				}
			}
		}
	} else {
		memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + 4);
		ret = cmd_yaffs_ls_chk(productinfofilename2);
		if (ret == (PRODUCTINFO_SIZE + 4)) {
			cmd_yaffs_mread_file(productinfofilename2, (unsigned char *)PRODUCTINFO_ADR);
			if (-1 == nv_is_correct((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE)) {
				memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + 4);
			}
		}
	}
	cmd_yaffs_umount(productinfopoint);
	//array_value((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE);
	eng_phasechecktest((unsigned char *)PRODUCTINFO_ADR, SP09_MAX_PHASE_BUFF_SIZE);
	///////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////
	/* RUNTIMEVN_PART */
	printf("Reading runtimenv to 0x%08x\n", RUNTIMENV_ADR);
	/* mtd nv */

	/* runtimenv */
    	cmd_yaffs_mount(runtimenvpoint);
	ret = cmd_yaffs_ls_chk(runtimenvfilename);
	if (ret == (RUNTIMENV_SIZE + 4)) {
		cmd_yaffs_mread_file(runtimenvfilename, (unsigned char *)RUNTIMENV_ADR);
		if (-1 == nv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
			memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + 4);
			ret = cmd_yaffs_ls_chk(runtimenvfilename2);
			if (ret == (RUNTIMENV_SIZE + 4)) {
				cmd_yaffs_mread_file(runtimenvfilename2, (unsigned char *)RUNTIMENV_ADR);
				if (-1 == nv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
					memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + 4);
				}
			}	
		}
	} else {
		memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + 4);
		ret = cmd_yaffs_ls_chk(runtimenvfilename2);
		if (ret == (RUNTIMENV_SIZE + 4)) {
			cmd_yaffs_mread_file(runtimenvfilename2, (unsigned char *)RUNTIMENV_ADR);
			if (-1 == nv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
				memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + 4);
			}
		}
	}
	cmd_yaffs_umount(runtimenvpoint);
	//array_value((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE);
	////////////////////////////////////////////////////////////////
	/* DSP_PART */
	printf("Reading dsp to 0x%08x\n", DSP_ADR);
	ret = find_dev_and_part(DSP_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", DSP_PART);
		//return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", DSP_PART);
		//return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];

	size = (DSP_SIZE + (FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
	if(size <= 0) {
		printf("dsp image should not be zero\n");
		//return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)DSP_ADR, &off);
	if(ret != 0) {
		printf("dsp nand read error %d\n", ret);
		//return;
	}
#endif
	////////////////////////////////////////////////////////////////
	/* KERNEL_PART */
	printf("Reading kernel to 0x%08x\n", KERNEL_ADR);

	ret = find_dev_and_part(kernel_pname, &dev, &pnum, &part);
	if(ret){
		printf("No partition named %s\n", kernel_pname);
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		printf("Partition %s not a NAND device\n", kernel_pname);
	}

	off=part->offset;
	nand = &nand_info[dev->id->num];
	//read boot image header
	size = 2048;
	ret = nand_read_offset_ret(nand, off, &size, (void *)hdr, &off);
	if(ret != 0){
		printf("function: %s nand read error %d\n", __FUNCTION__, ret);
	}
	if(memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)){
		printf("bad boot image header, give up read!!!!\n");
	}
	else
	{
		//read kernel image
		size = (hdr->kernel_size+(FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
		if(size <=0){
			printf("kernel image should not be zero\n");
			//return;
		}
		ret = nand_read_offset_ret(nand, off, &size, (void *)KERNEL_ADR, &off);
		if(ret != 0){
			printf("kernel nand read error %d\n", ret);
			//return;
		}
		//read ramdisk image
		size = (hdr->ramdisk_size+(FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1));
		if(size<0){
			printf("ramdisk size error\n");
			//return;
		}
		ret = nand_read_offset_ret(nand, off, &size, (void *)RAMDISK_ADR, &off);
		if(ret != 0){
			printf("ramdisk nand read error %d\n", ret);
			//return;
		}
	}

#if !(BOOT_NATIVE_LINUX)
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
#endif

	//array_value((unsigned char *)VMJALUNA_ADR, 16 * 10);
    //check caliberation mode
    int str_len;
    char * buf;
    buf = malloc(384);

    sprintf(buf, "initrd=0x%x,0x%x", RAMDISK_ADR, hdr->ramdisk_size);
    str_len = strlen(buf);
    mtdpart_def = get_mtdparts();
    sprintf(&buf[str_len], " %s", mtdpart_def);
    if(cmdline && cmdline[0]){
            str_len = strlen(buf);
            sprintf(&buf[str_len], " %s", cmdline);
    }
#if 0
	{
		extern uint32_t load_lcd_id_to_kernel();
		uint32_t lcd_id;

		lcd_id = load_lcd_id_to_kernel();
	    //add lcd id
		if(lcd_id)
		{
			str_len = strlen(buf);
			sprintf(&buf[str_len], " lcd_id=ID");
			str_len = strlen(buf);
			buf[str_len] = (char)((lcd_id>>8)&0xff);
			buf[str_len+1] = (char)(lcd_id&0xff);
			buf[str_len+2] = 0;
		}
	}
	{
		char *factorymodepoint = "/productinfo";
		char *factorymodefilename = "/productinfo/factorymode.file";
		cmd_yaffs_mount(factorymodepoint);
		ret = cmd_yaffs_ls_chk(factorymodefilename );
		if (ret == -1) {
			/* no factorymode.file found, nothing to do */
		} else {
			str_len = strlen(buf);
			sprintf(&buf[str_len], " factory");
		}
		cmd_yaffs_umount(factorymodepoint);
	}
#endif
    printf("pass cmdline: %s\n", buf);
    creat_atags(VLX_TAG_ADDR, buf, NULL, 0);

	void (*entry)(void) = (void*) VMJALUNA_ADR;
#if BOOT_NATIVE_LINUX
	start_linux();
#else
	entry();
#endif
}
void normal_mode(void)
{
    set_vibrator(1);
#if BOOT_NATIVE_LINUX
    vlx_nand_boot(BOOT_PART, CONFIG_BOOTARGS);
#else
    vlx_nand_boot(BOOT_PART, 0);
#endif

}
