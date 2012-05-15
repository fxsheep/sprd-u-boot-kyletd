#include <config.h>
#include <common.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#include <linux/string.h>
#include <android_bootimg.h>
#include <linux/mtd/mtd.h>
#include <linux/crc32b.h>
#include <linux/mtd/nand.h>
#include <nand.h>
#include <android_boot.h>
#include <environment.h>
#include <jffs2/jffs2.h>
#include <boot_mode.h>
#include <malloc.h>

#ifdef CONFIG_EMMC_BOOT
#include "../disk/part_uefi.h"
#include "../drivers/mmc/card_sdio.h"
#include "asm/arch/sci_types.h"
#include <ext_common.h>
#include <ext4fs.h>
#endif

unsigned char raw_header[8192];
static int flash_page_size = 0;

#define VMJALUNA_PART "vmjaluna"
#define MODEM_PART "modem"
#define KERNEL_PART "kernel"
#define FIXNV_PART "fixnv"
#define BACKUPFIXNV_PART "backupfixnv"
#define RUNTIMEVN_PART "runtimenv"
#define DSP_PART "dsp"

#define DSP_SIZE		(3968 * 1024)
#define VMJALUNA_SIZE		(300 * 1024)
#define FIXNV_SIZE		(64 * 1024)
#define PRODUCTINFO_SIZE	(3 * 1024)
#define RUNTIMENV_SIZE		(256 * 1024)
#ifdef MCP_F2R1
#define MODEM_SIZE		(3500 * 1024)  	/* 3.5MB */
#else
#define MODEM_SIZE		(8 * 1024 * 1024)
#endif

#define DSP_ADR			0x00020000
#define VMJALUNA_ADR		0x00400000
#define FIXNV_ADR		0x00480000
#define PRODUCTINFO_ADR		0x00490000
#define RUNTIMENV_ADR		0x004a0000
#define MODEM_ADR		0x00500000
#define RAMDISK_ADR 		0x05500000

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
	                 		  			  //if tested. 0: tested, 1: not tested
	unsigned short  iItem;    // part1: Bit0~ Bit_14 indicate test Station,1 : Pass,

}SP09_PHASE_CHECK_T, *LPSP09_PHASE_CHECK_T;
const static int SP09_MAX_PHASE_BUFF_SIZE = sizeof(SP09_PHASE_CHECK_T);

int eng_getphasecheck(SP09_PHASE_CHECK_T* phase_check)
{
	int aaa;
	unsigned long tested;

	if (phase_check->Magic == SP09_SPPH_MAGIC_NUMBER) {
		//printf("Magic = 0x%08x\n",phase_check->Magic);
		printf("SN1 = %s   SN2 = %s\n",phase_check->SN1, phase_check->SN2);
		/*printf("StationNum = %d\n",phase_check->StationNum);
		printf("Reserved = %s\n",phase_check->Reserved);
		printf("SignFlag = 0x%02x\n",phase_check->SignFlag);
		printf("iTestSign = 0x%04x\n",phase_check->iTestSign);
		printf("iItem = 0x%04x\n",phase_check->iItem);*/
		if (phase_check->SignFlag == 1) {
			for (aaa = 0; aaa < phase_check->StationNum/*SP09_MAX_STATION_NUM*/; aaa ++) {
				printf("%s : ", phase_check->StationName[aaa]);
				tested = 1 << aaa;
				if ((tested & phase_check->iTestSign) == 0) {
					if ((tested & phase_check->iItem) == 0)
						printf("Pass; ");
					else
						printf("Fail; ");
				} else
					printf("UnTested; ");
			}
		} else {
			printf("station status are all invalid!\n");
			for (aaa = 0; aaa < phase_check->StationNum/*SP09_MAX_STATION_NUM*/; aaa ++)
				printf("%s  ", phase_check->StationName[aaa]);
		}
		printf("\nLast error: %s\n",phase_check->szLastFailDescription);
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
void vibrator_hw_init(void);
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

int nv_is_correct_endflag(unsigned char *array, unsigned long size)
{
	if ((array[size] == 0x5a) && (array[size + 1] == 0x5a) && (array[size + 2] == 0x5a) && (array[size + 3] == 0x5a))
		return 1;
	else
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

#ifdef CONFIG_EMMC_BOOT
#define EMMC_SECTOR_SIZE 512
#define KERNL_PAGE_SIZE 2048

int nv_erase_partition(block_dev_desc_t *p_block_dev, EFI_PARTITION_INDEX part)
{
	disk_partition_t info;
	int ret = 0; /* success */
	unsigned char *tmpbuf = (unsigned char *)MODEM_ADR;

	if (!get_partition_info(p_block_dev, part, &info)) {
		memset(tmpbuf, 0xff, info.size * EMMC_SECTOR_SIZE);
		printf("%s %d part = %d  info.start = 0x%08x  info.size = 0x%08x\n", __FUNCTION__, __LINE__, part, info.start, info.size);
		if (TRUE !=  Emmc_Write(PARTITION_USER, info.start, info.size, (unsigned char *)tmpbuf)) {
			printf("emmc image erase error \n");
			ret = 1; /* fail */
		}
	}

	printf("%s %d ret = %d\n", __FUNCTION__, __LINE__, ret);
	return ret;
}

int nv_read_partition(block_dev_desc_t *p_block_dev, EFI_PARTITION_INDEX part, char *buf, int len)
{
	disk_partition_t info;
	unsigned long size = (len +(EMMC_SECTOR_SIZE - 1)) & (~(EMMC_SECTOR_SIZE - 1));
	int ret = 0; /* success */

	if (!get_partition_info(p_block_dev, part, &info)) {
		if (TRUE !=  Emmc_Read(PARTITION_USER, info.start, size / EMMC_SECTOR_SIZE, (uint8*)buf)) {
			printf("emmc image read error \n");
			ret = 1; /* fail */
		}
	}
	 
	return ret;
}

unsigned long char2u32(unsigned char *buf, int offset)
{
	unsigned long ret = 0;

	ret = (buf[offset + 3] & 0xff) \
		| ((buf[offset + 2] & 0xff) << 8) \
		| ((buf[offset + 1] & 0xff) << 16) \
		| ((buf[offset] & 0xff) << 24);
	
	return ret;
}

int prodinfo_read_partition(block_dev_desc_t *p_block_dev, EFI_PARTITION_INDEX part, int offset, char *buf, int len)
{
	disk_partition_t info;
	unsigned long size = (len +(EMMC_SECTOR_SIZE - 1)) & (~(EMMC_SECTOR_SIZE - 1));
	int ret = 0; /* success */
	unsigned long crc;
	unsigned long offset_block = offset / EMMC_SECTOR_SIZE;

	if (!get_partition_info(p_block_dev, part, &info)) {
		if (TRUE !=  Emmc_Read(PARTITION_USER, info.start + offset_block, size / EMMC_SECTOR_SIZE, (uint8*)buf)) {
			printf("emmc image read error : %d\n", offset);
			ret = 1; /* fail */
		}
	} else
		ret = 1;

	if (ret == 1)
		return ret;

	crc = crc32b(0xffffffff, buf, len);
	/* dump_all_buffer(buf, size); */
	if (offset == 0) {
		/* phasecheck */
		if ((buf[PRODUCTINFO_SIZE + 7] == (crc & 0xff)) \
			&& (buf[PRODUCTINFO_SIZE + 6] == ((crc & (0xff << 8)) >> 8)) \
			&& (buf[PRODUCTINFO_SIZE + 5] == ((crc & (0xff << 16)) >> 16)) \
			&& (buf[PRODUCTINFO_SIZE + 4] == ((crc & (0xff << 24)) >> 24))) {
				buf[PRODUCTINFO_SIZE + 7] = 0xff;
				buf[PRODUCTINFO_SIZE + 6] = 0xff;
				buf[PRODUCTINFO_SIZE + 5] = 0xff;
				buf[PRODUCTINFO_SIZE + 4] = 0xff;
		} else
			ret = 1;
	} else if ((offset == 4096) || (offset == 8192)) {
		/* factorymode or alarm mode */
		if ((buf[PRODUCTINFO_SIZE + 11] == (crc & 0xff)) \
			&& (buf[PRODUCTINFO_SIZE + 10] == ((crc & (0xff << 8)) >> 8)) \
			&& (buf[PRODUCTINFO_SIZE + 9] == ((crc & (0xff << 16)) >> 16)) \
			&& (buf[PRODUCTINFO_SIZE + 8] == ((crc & (0xff << 24)) >> 24))) {
				buf[PRODUCTINFO_SIZE + 11] = 0xff;
				buf[PRODUCTINFO_SIZE + 10] = 0xff;
				buf[PRODUCTINFO_SIZE + 9] = 0xff;
				buf[PRODUCTINFO_SIZE + 8] = 0xff;
		} else
			ret = 1;
	} else
		ret = 1;

	return ret;
}

int nv_write_partition(block_dev_desc_t *p_block_dev, EFI_PARTITION_INDEX part, char *buf, int len)
{
	disk_partition_t info;
	unsigned long size = (len +(EMMC_SECTOR_SIZE - 1)) & (~(EMMC_SECTOR_SIZE - 1));
	int ret = 0; /* success */

	if (!get_partition_info(p_block_dev, part, &info)) {
		if (TRUE !=  Emmc_Write(PARTITION_USER, info.start, size / EMMC_SECTOR_SIZE, (uint8*)buf)) {
			printf("emmc image read error \n");
			ret = 1; /* fail */
		}
	}

	return ret;
}

void dump_all_buffer(unsigned char *buf, unsigned long len)
{
	unsigned long row, col;
	unsigned int offset;
	unsigned long total_row, remain_col;
	unsigned long flag = 1;

	total_row = len / 16;
	remain_col = len - total_row * 16;
	//printf("total_row = %d  remain_col = %d\n", total_row, remain_col);
	offset = 0;
	for (row = 0; row < total_row; row ++) {
		/*flag = 0;
		for (col = 0; col < 16; col ++)
			if (buf[offset + col] != 0) {
				flag = 1;
				break;
			}*/
		if (flag == 1) {
			printf("%08xh: ", offset);
			for (col = 0; col < 16; col ++)
				printf("%02x ", buf[offset + col]);
			printf("\n");
		}
		offset += 16;
	}

	if (remain_col > 0) {
		/*flag = 0;
		for (col = 0; col < remain_col; col ++)
			if (buf[offset + col] != 0) {
				flag = 1;
				break;
			}*/
		if (flag == 1) {
			printf("%08xh: ", offset);
			for (col = 0; col < remain_col; col ++)
				printf("%02x ", buf[offset + col]);
			printf("\n");
		}
	}

	printf("\n");
}

void vlx_nand_boot(char * kernel_pname, char * cmdline, int backlight_set)
{
    boot_img_hdr *hdr = (void *)raw_header;
	block_dev_desc_t *p_block_dev = NULL;    
	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;
	size_t size;
	loff_t off = 0;
	int fixnv_right, backupfixnv_right;
	nand_erase_options_t opts;
    	char * mtdpart_def = NULL;
	disk_partition_t info;
	int filelen;
	ulong part_length;
        #ifdef CONFIG_SC8810
     	MMU_Init(CONFIG_MMU_TABLE_ADDR);
	#endif
	p_block_dev = get_dev("mmc", 1);
	if(NULL == p_block_dev){
		return;
	}
	
	
#ifdef CONFIG_SPLASH_SCREEN
#define SPLASH_PART "boot_logo"
	//read boot image header
	size = 1<<19;
	uint8 * bmp_img = malloc(size);
	if(!bmp_img){
	    printf("not enough memory for splash image\n");
	    return;
	}
	if (!get_partition_info(p_block_dev, PARTITION_LOGO, &info)) {
		if(TRUE !=  Emmc_Read(PARTITION_USER, info.start, size/EMMC_SECTOR_SIZE, bmp_img)){
			printf("function: %s nand read error\n", __FUNCTION__);
			return;
		}
	} 
	else{
		return;
	}
	
    extern int lcd_display_bitmap(ulong bmp_image, int x, int y);
    extern void lcd_display(void);
    extern void set_backlight(uint32_t value);
    if(backlight_set == BACKLIGHT_ON){
	    extern void *lcd_base;
	    extern void Dcache_CleanRegion(unsigned int addr, unsigned int length);

	    lcd_display_bitmap((ulong)bmp_img, 0, 0);
#ifdef CONFIG_SC8810
	    Dcache_CleanRegion((unsigned int)(lcd_base), size);//Size is to large.
#endif
	    lcd_display();
	    set_backlight(255);
    }
#endif
    set_vibrator(0);

#if !(BOOT_NATIVE_LINUX)
#if 1
	/* recovery damaged fixnv or backupfixnv */
	fixnv_right = 0;
	memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
	if(0 == nv_read_partition(p_block_dev, PARTITION_FIX_NV1, (char *)FIXNV_ADR, FIXNV_SIZE + 4)){
		if (1 == nv_is_correct_endflag((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
			fixnv_right = 1;//right
	}
	
    backupfixnv_right = 0;
	memset((unsigned char *)RUNTIMENV_ADR, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
	if(0 == nv_read_partition(p_block_dev, PARTITION_FIX_NV2, (char *)RUNTIMENV_ADR, FIXNV_SIZE + 4)){
		if (1 == nv_is_correct_endflag((unsigned char *)RUNTIMENV_ADR, FIXNV_SIZE))
			backupfixnv_right = 1;//right
	}
	
    if ((fixnv_right == 1) && (backupfixnv_right == 0)) {
		printf("fixnv is right, but backupfixnv is wrong, so erase and recovery backupfixnv\n");
		nv_erase_partition(p_block_dev, PARTITION_FIX_NV2);
		nv_write_partition(p_block_dev, PARTITION_FIX_NV2, (char *)FIXNV_ADR, (FIXNV_SIZE + 4));
	} else if ((fixnv_right == 0) && (backupfixnv_right == 1)) {
		printf("backupfixnv is right, but fixnv is wrong, so erase and recovery fixnv\n");
		nv_erase_partition(p_block_dev, PARTITION_FIX_NV1);
		nv_write_partition(p_block_dev, PARTITION_FIX_NV1, (char *)RUNTIMENV_ADR, (FIXNV_SIZE + 4));
	} else if ((fixnv_right == 0) && (backupfixnv_right == 0)) {
		printf("\n\nfixnv and backupfixnv are all wrong.\n\n");
	}
	///////////////////////////////////////////////////////////////////////
	/* FIXNV_PART */
	printf("Reading fixnv to 0x%08x\n", FIXNV_ADR);
	memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
	/* fixnv */
	if(nv_read_partition(p_block_dev, PARTITION_FIX_NV2, (char *)FIXNV_ADR, FIXNV_SIZE + 4) == 0) {
		if (-1 == nv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
			printf("nv is wrong, can not run here!\n");
	} else
		printf("nv is wrong, can not run here!\n");
	//array_value((unsigned char *)FIXNV_ADR, FIXNV_SIZE);

	///////////////////////////////////////////////////////////////////////
	/* PRODUCTINFO_PART */
	printf("Reading productinfo to 0x%08x\n", PRODUCTINFO_ADR);
	memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
	if(prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO1, 0, (char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE + 4) == 0){
		if (-1 == nv_is_correct((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE)) {
			memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + EMMC_SECTOR_SIZE);
			if(prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO2, 0, (char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE + 4) == 0){
				if (-1 == nv_is_correct((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE))
					memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + EMMC_SECTOR_SIZE);
			} else
				memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + EMMC_SECTOR_SIZE);
		}
	} else {
		memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + EMMC_SECTOR_SIZE);
		if(prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO1, 0, (char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE + 4) == 0) {
			if (-1 == nv_is_correct((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE))
				memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + EMMC_SECTOR_SIZE);
		} else
			memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + EMMC_SECTOR_SIZE);
	}
	//array_value((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE);
	eng_phasechecktest((unsigned char *)PRODUCTINFO_ADR, SP09_MAX_PHASE_BUFF_SIZE);

	/* RUNTIMEVN_PART */
	printf("Reading runtimenv to 0x%08x\n", RUNTIMENV_ADR);
	/* runtimenv */
	memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + EMMC_SECTOR_SIZE);
	if(nv_read_partition(p_block_dev, PARTITION_RUNTIME_NV1, (char *)RUNTIMENV_ADR, RUNTIMENV_SIZE + 4) == 0) {
		if (-1 == nv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
			/* file isn't right and read backup file */
			memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + EMMC_SECTOR_SIZE);
			if(nv_read_partition(p_block_dev, PARTITION_RUNTIME_NV1, (char *)RUNTIMENV_ADR, RUNTIMENV_SIZE + 4) == 0) {
				if (-1 == nv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
					/* file isn't right */
					memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + EMMC_SECTOR_SIZE);
				}
			}
		}
	} else {
		/* file don't exist and read backup file */
		memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + EMMC_SECTOR_SIZE);
		if(nv_read_partition(p_block_dev, PARTITION_RUNTIME_NV1, (char *)RUNTIMENV_ADR, RUNTIMENV_SIZE + 4) == 0){
			if (-1 == nv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
				/* file isn't right */
				memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + 4);
			}
		}
	}
	//array_value((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE);
#endif	
	/* DSP_PART */
	printf("Reading dsp to 0x%08x\n", DSP_ADR);

	if (!get_partition_info(p_block_dev, PARTITION_DSP, &info)) {
		 if(TRUE !=  Emmc_Read(PARTITION_USER, info.start, DSP_SIZE/512+1, (uint8*)DSP_ADR)){
			printf("dsp nand read error \n");
			return;
		}
	} 
	else{
		return;
	}
                 
#endif
	////////////////////////////////////////////////////////////////
	/* KERNEL_PART */
	printf("Reading kernel to 0x%08x\n", KERNEL_ADR);
	if(memcmp(kernel_pname, RECOVERY_PART, strlen(RECOVERY_PART))){
		printf("Reading bootimg to 0x%08x\n", KERNEL_ADR);
		if(get_partition_info(p_block_dev, PARTITION_KERNEL, &info) != 0)
			return ;
	}else{
		printf("Reading recoverimg to 0x%08x\n", KERNEL_ADR);
		if(get_partition_info(p_block_dev, PARTITION_RECOVERY, &info) != 0)
			return ;
	}
	 if(TRUE !=  Emmc_Read(PARTITION_USER, info.start, 4, (uint8*)(uint8*)hdr)){
		printf("kernel nand read error \n");
		return;
	}

	if(memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)){
		printf("bad boot image header, give up read!!!!\n");
                                      return;
	}
	else
	{
	                  uint32 noffsetsector = 4;
		//read kernel image
		size = (hdr->kernel_size+(KERNL_PAGE_SIZE - 1)) & (~(KERNL_PAGE_SIZE - 1));
		if(size <=0){
			printf("kernel image should not be zero\n");
			return;
		}
                                     if(TRUE !=  Emmc_Read(PARTITION_USER, info.start+noffsetsector, size/EMMC_SECTOR_SIZE, (uint8*)KERNEL_ADR)){
			printf("kernel nand read error\n");
			return;
		}
		noffsetsector += size/512;
		noffsetsector = (noffsetsector+3)/4;
		noffsetsector = noffsetsector*4;
		//read ramdisk image
		size = (hdr->ramdisk_size+(KERNL_PAGE_SIZE - 1)) & (~(KERNL_PAGE_SIZE - 1));
		if(size<0){
			printf("ramdisk size error\n");
			return;
		}
                                     if(TRUE !=  Emmc_Read(PARTITION_USER, info.start+noffsetsector, size/EMMC_SECTOR_SIZE, (uint8*)RAMDISK_ADR))
		if(ret != 0){
			printf("ramdisk nand read error %d\n", ret);
			return;
		}
	}

#if !(BOOT_NATIVE_LINUX)
	////////////////////////////////////////////////////////////////
	/* MODEM_PART */
	printf("Reading modem to 0x%08x\n", MODEM_ADR);

	size = (MODEM_SIZE +(EMMC_SECTOR_SIZE - 1)) & (~(EMMC_SECTOR_SIZE - 1));
	if(size <= 0) {
		printf("modem image should not be zero\n");
		return;
	}
	if (!get_partition_info(p_block_dev, PARTITION_MODEM, &info)) {
		 if(TRUE !=  Emmc_Read(PARTITION_USER, info.start, size/EMMC_SECTOR_SIZE, (uint8*)MODEM_ADR)){
			printf("modem nand read error \n");
			return;
		}
	} 
	else{
		return;
	}
	 

	//array_value((unsigned char *)MODEM_ADR, MODEM_SIZE);

	////////////////////////////////////////////////////////////////
	/* VMJALUNA_PART */
	printf("Reading vmjaluna to 0x%08x\n", VMJALUNA_ADR);

	size = (VMJALUNA_SIZE +(EMMC_SECTOR_SIZE - 1)) & (~(EMMC_SECTOR_SIZE - 1));
	if(size <= 0) {
		printf("vmjuluna image should not be zero\n");
		return;
	}
	if (!get_partition_info(p_block_dev, PARTITION_VM, &info)) {
		 if(TRUE !=  Emmc_Read(PARTITION_USER, info.start, size/EMMC_SECTOR_SIZE, (uint8*)VMJALUNA_ADR)){
			printf("vmjaluna nand read error \n");
			return;
		}
	} 
	else{
		return;
	}
#endif

	//array_value((unsigned char *)VMJALUNA_ADR, 16 * 10);
    //check caliberation mode
    int str_len;
    char * buf;
    buf = malloc(1024);

    sprintf(buf, "initrd=0x%x,0x%x", RAMDISK_ADR, hdr->ramdisk_size);
    str_len = strlen(buf);
    //mtdpart_def = get_mtdparts();
    //sprintf(&buf[str_len], " %s", mtdpart_def);
    if(cmdline && cmdline[0]){
            str_len = strlen(buf);
            sprintf(&buf[str_len], " %s", cmdline);
    }
	{
		extern uint32_t load_lcd_id_to_kernel();
		uint32_t lcd_id;

		lcd_id = load_lcd_id_to_kernel();
		//add lcd id
		if(lcd_id) {
			str_len = strlen(buf);
			sprintf(&buf[str_len], " lcd_id=ID");
			str_len = strlen(buf);
			sprintf(&buf[str_len], "%x",lcd_id);
			str_len = strlen(buf);
			buf[str_len] = '\0';
		}
	}

	ret = 0;
	printf("Checking factorymode : ");

	int factoryalarmret1, factoryalarmret2;
	unsigned long factoryalarmcnt1, factoryalarmcnt2;
	unsigned char factoryalarmarray1[PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE];
	unsigned char factoryalarmarray2[PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE];
	
	memset((unsigned char *)factoryalarmarray1, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
	factoryalarmret1 = prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO1, 4 * 1024, (char *)factoryalarmarray1, PRODUCTINFO_SIZE + 8);
	memset((unsigned char *)factoryalarmarray2, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
	factoryalarmret2 = prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO2, 4 * 1024, (char *)factoryalarmarray2, PRODUCTINFO_SIZE + 8);
	
	if ((factoryalarmret1 == 0) && (factoryalarmret2 == 0)) {
		factoryalarmcnt1 = char2u32(factoryalarmarray1, 3 * 1024 + 4);
		factoryalarmcnt2 = char2u32(factoryalarmarray2, 3 * 1024 + 4);
		if (factoryalarmcnt2 >= factoryalarmcnt1) {
			if (factoryalarmarray2[0] == 0x31)
				ret = 1;
		} else {
			if (factoryalarmarray1[0] == 0x31)
				ret = 1;
		}
	} else if ((factoryalarmret1 == 0) && (factoryalarmret2 == 1)) {
		if (factoryalarmarray1[0] == 0x31)
			ret = 1;
	} else if ((factoryalarmret1 == 1) && (factoryalarmret2 == 0)) {
		if (factoryalarmarray2[0] == 0x31)
			ret = 1;
	} else if ((factoryalarmret1 == 1) && (factoryalarmret2 == 1))
		printf("0\n");

	if (ret == 1) {
		printf("1\n");
		str_len = strlen(buf);
		sprintf(&buf[str_len], " factory=1");
	}

	str_len = strlen(buf);
#ifdef CONFIG_RAM512M
    sprintf(&buf[str_len], " ram=512M");
#else
    sprintf(&buf[str_len], " ram=256M");
#endif
    printf("pass cmdline: %s\n", buf);
    //lcd_printf(" pass cmdline : %s\n",buf);
    //lcd_display();
    creat_atags(VLX_TAG_ADDR, buf, NULL, 0);
    void (*entry)(void) = (void*) VMJALUNA_ADR;
#ifndef CONFIG_SC8810
    MMU_InvalideICACHEALL();
#endif
#ifdef CONFIG_SC8810
    MMU_DisableIDCM();
#endif

#if BOOT_NATIVE_LINUX
	start_linux();
#else
	entry();
#endif
}

#else
void vlx_nand_boot(char * kernel_pname, char * cmdline, int backlight_set)
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
	int fixnv_right, backupfixnv_right;
	nand_erase_options_t opts;
    	char * mtdpart_def = NULL;
        #ifdef CONFIG_SC8810
     	MMU_Init(CONFIG_MMU_TABLE_ADDR);
	#endif
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
	size = 1<<19;//where the size come from????
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
    if(backlight_set == BACKLIGHT_ON){
	    extern void *lcd_base;
	    extern void Dcache_CleanRegion(unsigned int addr, unsigned int length);

	    lcd_display_bitmap((ulong)bmp_img, 0, 0);
#ifdef CONFIG_SC8810
	    Dcache_CleanRegion((unsigned int)(lcd_base), size);//Size is to large.
#endif
	    lcd_display();
	    set_backlight(255);
    }
#endif
    set_vibrator(0);

#if !(BOOT_NATIVE_LINUX)
	/*int good_blknum, bad_blknum;
	nand_block_info(nand, &good_blknum, &bad_blknum);
	printf("good is %d  bad is %d\n", good_blknum, bad_blknum);*/
	///////////////////////////////////////////////////////////////////////
	/* recovery damaged fixnv or backupfixnv */
	fixnv_right = 0;
	memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
	cmd_yaffs_mount(fixnvpoint);
	ret = cmd_yaffs_ls_chk(fixnvfilename);
	if (ret == (FIXNV_SIZE + 4)) {
		cmd_yaffs_mread_file(fixnvfilename, (unsigned char *)FIXNV_ADR);
		if (1 == nv_is_correct_endflag((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
			fixnv_right = 1;//right
	}
	cmd_yaffs_umount(fixnvpoint);

	backupfixnv_right = 0;
	memset((unsigned char *)RUNTIMENV_ADR, 0xff, FIXNV_SIZE + 4);
	cmd_yaffs_mount(backupfixnvpoint);
	ret = cmd_yaffs_ls_chk(backupfixnvfilename);
	if (ret == (FIXNV_SIZE + 4)) {
		cmd_yaffs_mread_file(backupfixnvfilename, (unsigned char *)RUNTIMENV_ADR);
		if (1 == nv_is_correct_endflag((unsigned char *)RUNTIMENV_ADR, FIXNV_SIZE))
			backupfixnv_right = 1;//right
	}
	cmd_yaffs_umount(backupfixnvpoint);
	//printf("fixnv_right = %d  backupfixnv_right = %d\n", fixnv_right, backupfixnv_right);
	if ((fixnv_right == 1) && (backupfixnv_right == 0)) {
		printf("fixnv is right, but backupfixnv is wrong, so erase and recovery backupfixnv\n");
		////////////////////////////////
		find_dev_and_part(BACKUPFIXNV_PART, &dev, &pnum, &part);
		//printf("offset = 0x%08x  size = 0x%08x\n", part->offset, part->size);
		nand = &nand_info[dev->id->num];
		memset(&opts, 0, sizeof(opts));
		opts.offset = part->offset;
		opts.length = part->size;
		opts.quiet = 1;
		nand_erase_opts(nand, &opts);
		////////////////////////////////
		cmd_yaffs_mount(backupfixnvpoint);
    		cmd_yaffs_mwrite_file(backupfixnvfilename, (char *)FIXNV_ADR, (FIXNV_SIZE + 4));
		cmd_yaffs_ls_chk(backupfixnvfilename);
		cmd_yaffs_umount(backupfixnvpoint);
	} else if ((fixnv_right == 0) && (backupfixnv_right == 1)) {
		printf("backupfixnv is right, but fixnv is wrong, so erase and recovery fixnv\n");
		////////////////////////////////
		find_dev_and_part(FIXNV_PART, &dev, &pnum, &part);
		//printf("offset = 0x%08x  size = 0x%08x\n", part->offset, part->size);
		nand = &nand_info[dev->id->num];
		memset(&opts, 0, sizeof(opts));
		opts.offset = part->offset;
		opts.length = part->size;
		opts.quiet = 1;
		nand_erase_opts(nand, &opts);
		////////////////////////////////
		cmd_yaffs_mount(fixnvpoint);
    		cmd_yaffs_mwrite_file(fixnvfilename, (char *)RUNTIMENV_ADR, (FIXNV_SIZE + 4));
		cmd_yaffs_ls_chk(fixnvfilename);
		cmd_yaffs_umount(fixnvpoint);
	} else if ((fixnv_right == 0) && (backupfixnv_right == 0)) {
		printf("\n\nfixnv and backupfixnv are all wrong.\n\n");
	}
	///////////////////////////////////////////////////////////////////////
	/* FIXNV_PART */
	printf("Reading fixnv to 0x%08x\n", FIXNV_ADR);
	memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
	/* fixnv */
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
					/*#########################*/
					cmd_yaffs_umount(backupfixnvpoint);
					/* file is wrong */
					memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
					/* read fixnv */
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
					/* read fixnv backup */
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
				/*#########################*/
				cmd_yaffs_umount(backupfixnvpoint);
				/* file is wrong */
				memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
				/* read fixnv */
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
					/* read fixnv backup */
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
				/*#########################*/
				cmd_yaffs_umount(backupfixnvpoint);
				/* file is wrong */
				memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
				/* read fixnv */
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
						/* read fixnv backup */
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
			/*#########################*/
			cmd_yaffs_umount(backupfixnvpoint);
			/* file is wrong */
			memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
			/* read fixnv */
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
					/* read fixnv backup */
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
		///////////////////////////////
	}
	//array_value((unsigned char *)FIXNV_ADR, FIXNV_SIZE);

	///////////////////////////////////////////////////////////////////////
	/* PRODUCTINFO_PART */
	printf("Reading productinfo to 0x%08x\n", PRODUCTINFO_ADR);
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
	/* runtimenv */
    cmd_yaffs_mount(runtimenvpoint);
	ret = cmd_yaffs_ls_chk(runtimenvfilename);
	if (ret == (RUNTIMENV_SIZE + 4)) {
		/* file exist */
		cmd_yaffs_mread_file(runtimenvfilename, (unsigned char *)RUNTIMENV_ADR);
		if (-1 == nv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
			////////////////
			/* file isn't right and read backup file */
			memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + 4);
			ret = cmd_yaffs_ls_chk(runtimenvfilename2);
			if (ret == (RUNTIMENV_SIZE + 4)) {
				cmd_yaffs_mread_file(runtimenvfilename2, (unsigned char *)RUNTIMENV_ADR);
				if (-1 == nv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
					/* file isn't right */
					memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + 4);
				}
			}
			////////////////
		}
	} else {
		/* file don't exist and read backup file */
		memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + 4);
		ret = cmd_yaffs_ls_chk(runtimenvfilename2);
		if (ret == (RUNTIMENV_SIZE + 4)) {
			cmd_yaffs_mread_file(runtimenvfilename2, (unsigned char *)RUNTIMENV_ADR);
			if (-1 == nv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
				/* file isn't right */
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
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", DSP_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];
	flash_page_size = nand->writesize;
	size = (DSP_SIZE + (flash_page_size - 1)) & (~(flash_page_size - 1));
	if(size <= 0) {
		printf("dsp image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)DSP_ADR, &off);
	if(ret != 0) {
		printf("dsp nand read error %d\n", ret);
		return;
	}
#endif
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
	size = nand->writesize;
	flash_page_size = nand->writesize;
	ret = nand_read_offset_ret(nand, off, &size, (void *)hdr, &off);
	if(ret != 0){
		printf("function: %s nand read error %d\n", __FUNCTION__, ret);
        return;
	}
	if(memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)){
		printf("bad boot image header, give up read!!!!\n");
        return;
	}
	else
	{
		//read kernel image
		size = (hdr->kernel_size+(flash_page_size - 1)) & (~(flash_page_size - 1));
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
		size = (hdr->ramdisk_size+(flash_page_size - 1)) & (~(flash_page_size - 1));
		if(size<0){
			printf("ramdisk size error\n");
			return;
		}
		ret = nand_read_offset_ret(nand, off, &size, (void *)RAMDISK_ADR, &off);
		if(ret != 0){
			printf("ramdisk nand read error %d\n", ret);
			return;
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
	size = (MODEM_SIZE +(flash_page_size - 1)) & (~(flash_page_size - 1));
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
	size = (VMJALUNA_SIZE +(flash_page_size - 1)) & (~(flash_page_size - 1));
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
    buf = malloc(1024);

    sprintf(buf, "initrd=0x%x,0x%x", RAMDISK_ADR, hdr->ramdisk_size);
#if !(BOOT_NATIVE_LINUX)
    str_len = strlen(buf);
    mtdpart_def = get_mtdparts();
    sprintf(&buf[str_len], " %s", mtdpart_def);
#endif
    if(cmdline && cmdline[0]){
            str_len = strlen(buf);
            sprintf(&buf[str_len], " %s", cmdline);
    }
	{
		extern uint32_t load_lcd_id_to_kernel();
		uint32_t lcd_id;

		lcd_id = load_lcd_id_to_kernel();
		//add lcd id
		if(lcd_id) {
			str_len = strlen(buf);
			sprintf(&buf[str_len], " lcd_id=ID");
			str_len = strlen(buf);
			sprintf(&buf[str_len], "%x",lcd_id);
			str_len = strlen(buf);
			buf[str_len] = '\0';
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
			sprintf(&buf[str_len], " factory=1");
		}
		cmd_yaffs_umount(factorymodepoint);
	}
	str_len = strlen(buf);
#ifdef CONFIG_RAM512M
    sprintf(&buf[str_len], " ram=512M");
#else
    sprintf(&buf[str_len], " ram=256M");
#endif
    printf("pass cmdline: %s\n", buf);
    //lcd_printf(" pass cmdline : %s\n",buf);
    //lcd_display();
    creat_atags(VLX_TAG_ADDR, buf, NULL, 0);
    void (*entry)(void) = (void*) VMJALUNA_ADR;
#ifndef CONFIG_SC8810
    MMU_InvalideICACHEALL();
#endif
#ifdef CONFIG_SC8810
    MMU_DisableIDCM();
#endif

#if BOOT_NATIVE_LINUX
	start_linux();
#else
	entry();
#endif
}
#endif
void normal_mode(void)
{
#ifdef CONFIG_SC8810
    //MMU_Init(CONFIG_MMU_TABLE_ADDR);
	vibrator_hw_init();
#endif
    set_vibrator(1);
#if BOOT_NATIVE_LINUX
    vlx_nand_boot(BOOT_PART, CONFIG_BOOTARGS, BACKLIGHT_ON);
#else
    vlx_nand_boot(BOOT_PART, NULL, BACKLIGHT_ON);
#endif

}
