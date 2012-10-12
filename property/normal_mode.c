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
#include <asm/arch/secure_boot.h>

#ifdef CONFIG_EMMC_BOOT
#include "../disk/part_uefi.h"
#include "../drivers/mmc/card_sdio.h"
#include "asm/arch/sci_types.h"
#include <ext_common.h>
#include <ext4fs.h>
#ifdef BOOTING_BACKUP_NVCALIBRATION
#include "backupnvitem.h"
#endif
#endif

unsigned spl_data_buf[0x1000] __attribute__((align(4)))={0};
unsigned harsh_data_buf[8]__attribute__((align(4)))={0};
void *spl_data = spl_data_buf;
void *harsh_data = harsh_data_buf;
unsigned char raw_header[8192];
static int flash_page_size = 0;

#define VMJALUNA_PART "vmjaluna"
#define MODEM_PART "modem"
#define KERNEL_PART "kernel"
#define FIXNV_PART "fixnv"
#define BACKUPFIXNV_PART "backupfixnv"
#define RUNTIMEVN_PART "runtimenv"
#define DSP_PART "dsp"
#define SPL_PART "spl"

#ifdef CONFIG_TIGER
#define DSP_ADR			0x80020000
#define VMJALUNA_ADR		0x80400000
#define FIXNV_ADR		0x80480000
#define RUNTIMENV_ADR		0x804a0000
#define MODEM_ADR		0x80500000
#define RAMDISK_ADR 		0x85500000
#if BOOT_NATIVE_LINUX
//pls make sure uboot running area
#define VLX_TAG_ADDR            (0x80000100)
#define KERNEL_ADR		(0x80008000)

#else

#define KERNEL_ADR		0x84508000
#define VLX_TAG_ADDR            0x85100000 //after initrd

#endif

#else
#define DSP_ADR			0x00020000
#define VMJALUNA_ADR		0x00400000
#define FIXNV_ADR		0x00480000
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
#endif

/////////////////////////////////////////////////////////
/* log_switch function */
#if 0
GSM_Download_Param include the following :

system_param_version  short
fcch_det_struct		12 * short
freq_est_struct		6 * short
freq_track_algo_struct	4 * short
AGC_algo_cal_struct		12 * short
time_track_algo_struct	4 * short
rx_equalizer_struct		3 * short
rx_deinterlever_struct	1 * short
power_scan_struct		----------------->  power_scan_saturation_thresh		1 * short
                                               power_exp_lev					3 * short
                                               power_underflow_thresh			3 * short
tone_para_struct		1 * short
action_tables_struct		0x120 * short
audio_filter_coef		----------------->  HPF_coef					36 * short
                                               LPF_coef					15 * short
dsp_uart_struct		1 * short
deep_sleep_struct		5 * short
log_switch_struct		-----------------> DSP_log_switch                             1 * short
#endif

struct power_scan_saturation_thresh_tag {
	unsigned short	power_scan_saturation_thresh;
};

struct power_exp_lev_tag {
	unsigned short	power_exp_lev[3];
};

struct power_underflow_thresh_tag {
	unsigned short	power_underflow_thresh[3];
};

struct HPF_coef_tag {
	unsigned short	HPF_coef[36];
};

struct LPF_coef_tag {
	unsigned short	LPF_coef[15];
};

struct power_scan_struct_tag {
	struct power_scan_saturation_thresh_tag power_scan_saturation_thresh;
	struct power_exp_lev_tag power_exp_lev;
	struct power_underflow_thresh_tag power_underflow_thresh;
};

struct audio_filter_coef_tag {
	struct HPF_coef_tag HPF_coef;
	struct LPF_coef_tag LPF_coef;
};

struct DSP_log_switch_tag {
	unsigned short DSP_log_switch_value;
};

struct log_switch_struct_tag {
	struct DSP_log_switch_tag DSP_log_switch;
};

struct GSM_Download_Param_Tag {
	unsigned short system_param_version;
	unsigned short fcch_det_struct[12];
	unsigned short freq_est_struct[6];
	unsigned short freq_track_algo_struct[4];
	unsigned short AGC_algo_cal_struct[12];
	unsigned short time_track_algo_struct[4];
	unsigned short rx_equalizer_struct[3];
	unsigned short rx_deinterlever_struct;
	struct power_scan_struct_tag power_scan_struct;
	unsigned short tone_para_struct;
	unsigned short action_tables_struct[0x120];
	struct audio_filter_coef_tag audio_filter_coef;
	unsigned short dsp_uart_struct;
	unsigned short deep_sleep_struct[5];
	struct log_switch_struct_tag log_switch_struct;
};
/////////////////////////////////////////////////////////

#define NV_MAGIC		(0x53544e56)
#define INVALID_NV_MAGIC	(0xffffffff)

#define STATUS_VALID		(0x00000001)
#define STATUS_DELETED		(0x00000002)
#define STATUS_MASK		(0x00000003)

struct npb_tag {
	unsigned long magic;
	unsigned long timestamp;
	unsigned short min_id;
	unsigned short max_id;
	unsigned short tot_scts;
	unsigned short sct_size;
	unsigned short dir_entry_count;
	unsigned short dir_entry_size;
	unsigned long next_offset;
	unsigned char backup_npb;
	unsigned char backup_dir;
};

struct nv_dev {
	struct npb_tag *npb;
	unsigned long tot_size;
	unsigned short first_dir_sct;
	unsigned short first_backup_dir_sct;
	unsigned long data_offset;
	unsigned char *runtime;
};

struct direntry_tag {
	unsigned short size;
	unsigned short checksum;
	unsigned long offset;
	unsigned long status;
	unsigned long reserved;
};

struct item_tag {
	unsigned short id;
	unsigned short size;
};

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
int get_partition_info (block_dev_desc_t *dev_desc, int part, disk_partition_t *info);
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

unsigned short calc_checksum(unsigned char *dat, unsigned long len)
{
	unsigned long checksum = 0;
	unsigned short *pstart, *pend;
	if (0 == (unsigned long)dat % 2)  {
		pstart = (unsigned short *)dat;
		pend = pstart + len / 2;
		while (pstart < pend) {
			checksum += *pstart;
			pstart ++;
		}
		if (len % 2)
			checksum += *(unsigned char *)pstart;
		} else {
		pstart = (unsigned char *)dat;
		while (len > 1) {
			checksum += ((*pstart) | ((*(pstart + 1)) << 8));
			len -= 2;
			pstart += 2;
		}
		if (len)
			checksum += *pstart;
	}
	checksum = (checksum >> 16) + (checksum & 0xffff);
	checksum += (checksum >> 16);
	return (~checksum);
}

#define NV_MULTI_LANG_ID   (405)
#define GSM_CALI_ITEM_ID   (0x2)
#define GSM_IMEI_ITEM_ID   (0x5)
#define XTD_CALI_ITEM_ID   (0x516)
#define LTE_CALI_ITEM_ID   (0x9C4)
#define BT_ITEM_ID         (0x191)

#define BT_ADDR_LEN  6

#define IMEI_LEN			(8)
#define GSM_CALI_VER_A      0xFF0A
#define GSM_CALI_VER_MIN    GSM_CALI_VER_A
#define GSM_CALI_VER_MAX    GSM_CALI_VER_A

#define NUM_TEMP_BANDS		(5)
#define NUM_RAMP_RANGES		(16)		/* constant parameter numbers, 16 level */
#define NUM_TX_LEVEL		(16)		/* 2db per step */
#define NUM_RAMP_POINTS		(20)
#define NUM_GSM_ARFCN_BANDS	(6)
#define NUM_DCS_ARFCN_BANDS	(8)
#define NUM_PCS_ARFCN_BANDS	(7)
#define NUM_GSM850_ARFCN_BANDS	(6)
#define MAX_COMPENSATE_POINT	(75)

static unsigned long XCheckNVStruct(unsigned char *lpPhoBuf, unsigned long dwPhoSize)
{
	unsigned long dwOffset = 0, dwLength = 0, bRet;
	unsigned char *lpCode = lpPhoBuf;
	unsigned long dwCodeSize = dwPhoSize;
	unsigned short wCurID;

	dwOffset = 4;     /* Skip first four bytes,that is time stamp */
    dwLength = 0;
    unsigned char *pTemp = lpCode + dwOffset;

	unsigned long bIMEI = 0;
	unsigned long bGSMCali = 0;
	unsigned short wGSMCaliVer = 0;
    while (dwOffset < dwCodeSize) {
	    wCurID = *(unsigned short *)pTemp;
        pTemp += 2;

        dwLength = *(unsigned short *)pTemp;
		/* printf("wCurID = 0x%08x  dwLength = 0x%08x\n", wCurID, dwLength); */
		if (wCurID == GSM_IMEI_ITEM_ID) {
			if (dwLength != IMEI_LEN) {
				return 0;
			} else {
				bIMEI = 1;
			}
		} else if (wCurID == GSM_CALI_ITEM_ID) {
			wGSMCaliVer =  *(unsigned short *)(pTemp + 2); /* pTemp + 2: skip length */
            /* printf("wGSMCaliVer = 0x%08x\n", wGSMCaliVer); */
			if ((wGSMCaliVer > GSM_CALI_VER_MAX) || (wGSMCaliVer < GSM_CALI_VER_MIN)) {
				return 0;
			} else {
				bGSMCali = 1;
			}
		}

		/* 0xFFFF is end-flag in module (NV in phone device) */
		if (wCurID == 0xFFFF) {
			if (!bIMEI || !bGSMCali) {
				return 0;
			}
			return 1;
		}

		if (wCurID == 0 || dwLength == 0) {
			break;
		}

        pTemp += 2;
        dwOffset += 4;
        /* Must be four byte aligned */
        bRet = dwLength % 4;
        if (bRet != 0)
                dwLength += 4 - bRet;
        dwOffset += dwLength;
        pTemp += dwLength;
        /* (dwOffset == dwCodeSize) is end condition in File */
		if (dwOffset == dwCodeSize) {
			if(!bIMEI || !bGSMCali) {
				return 0;
			}
			return 1;
		}
	}

	return 0;
}

unsigned long LogSwitch_Function(unsigned char *lpPhoBuf, unsigned long dwPhoSize)
{
	unsigned long dwOffset = 0, dwLength = 0, bRet;
	unsigned char *lpCode = lpPhoBuf;
	unsigned long dwCodeSize = dwPhoSize;
	unsigned short wCurID;
	unsigned long *timestamp;
	struct GSM_Download_Param_Tag *GSM_Download_Param;
	timestamp = lpPhoBuf;
	dwOffset = 4;     /* Skip first four bytes,that is time stamp */
	dwLength = 0;
	unsigned char *pTemp = lpCode + dwOffset;

	unsigned long bIMEI = 0;
	unsigned long bGSMCali = 0;
	unsigned short wGSMCaliVer = 0;
	while (dwOffset < dwCodeSize) {
		wCurID = *(unsigned short *)pTemp;
		pTemp += 2;

		dwLength = *(unsigned short *)pTemp;
		//printf("wCurID = %d  dwLength = 0x%08x\n", wCurID, dwLength);

		/* 0xFFFF is end-flag in module (NV in phone device) */
		if (wCurID == 0xFFFF) {
			if (!bIMEI || !bGSMCali) {
				return 0;
			}
			return 1;
		}

		if (wCurID == 0 || dwLength == 0) {
			break;
		}
		pTemp += 2;

		if (wCurID == 1) {
			printf("pTemp = 0x%08x  dwLength = %d\n", pTemp, dwLength);
			GSM_Download_Param = (struct GSM_Download_Param_Tag *)pTemp;
			printf("flag = %d sizeof = 0x%08x\n", GSM_Download_Param->log_switch_struct.DSP_log_switch.DSP_log_switch_value, sizeof(struct GSM_Download_Param_Tag));

			GSM_Download_Param->log_switch_struct.DSP_log_switch.DSP_log_switch_value = 0;
			printf("flag = %d\n", GSM_Download_Param->log_switch_struct.DSP_log_switch);
			break;
		}
		dwOffset += 4;
		/* Must be four byte aligned */
		bRet = dwLength % 4;
		if (bRet != 0)
			dwLength += 4 - bRet;
		dwOffset += dwLength;
		pTemp += dwLength;
		/* (dwOffset == dwCodeSize) is end condition in File */
		if (dwOffset == dwCodeSize) {
			if(!bIMEI || !bGSMCali) {
				return 0;
			}
			return 1;
		}
	}

	return 0;
}

/*
* retval : -1 is wrong  ;  1 is correct
*/
int fixnv_is_correct(unsigned char *array, unsigned long size)
{
	unsigned short sum = 0, *dataaddr;

	if ((array[size - 4] == 0xff) && (array[size - 3] == 0xff) && (array[size - 2] == 0xff) \
		&& (array[size - 1] == 0xff)) {
		/* old version */
		if ((array[size] == 0x5a) && (array[size + 1] == 0x5a) && (array[size + 2] == 0x5a) \
			&& (array[size + 3] == 0x5a)) {
			/* check nv right or wrong */
			if (XCheckNVStruct(array, size) == 0) {
				printf("NV data is crashed!!!.\n");
				return -1;
			} else {
				printf("NV data is right!!!.\n");
				array[size] = 0xff; array[size + 1] = 0xff;
				array[size + 2] = 0xff; array[size + 3] = 0xff;
				return 1;
			}
		} else
			return -1;
	} else {
		/* new version */
		sum = calc_checksum(array, size - 4);
		dataaddr = (unsigned short *)(array + size - 4);

		if (*dataaddr == sum) {
			/* check nv right or wrong */
			if (XCheckNVStruct(array, size) == 0) {
				printf("NV data is crashed!!!.\n");
				return -1;
			} else {
				printf("NV data is right!!!.\n");
				array[size] = 0xff; array[size + 1] = 0xff;
				array[size + 2] = 0xff; array[size + 3] = 0xff;
				array[size - 4] = 0xff; array[size - 3] = 0xff;
				array[size - 2] = 0xff; array[size - 1] = 0xff;
				return 1;
			}
		} else {
			printf("NV data crc error\n");
			return -1;
		}
	}
}

int fixnv_is_correct_endflag(unsigned char *array, unsigned long size)
{
	unsigned short sum = 0, *dataaddr;

	if ((array[size - 4] == 0xff) && (array[size - 3] == 0xff) && (array[size - 2] == 0xff) \
		&& (array[size - 1] == 0xff)) {
		/* old version */
		if ((array[size] == 0x5a) && (array[size + 1] == 0x5a) && (array[size + 2] == 0x5a) \
			&& (array[size + 3] == 0x5a)) {
			/* check nv right or wrong */
			if (XCheckNVStruct(array, size) == 0) {
				printf("NV data is crashed!!!.\n");
				return -1;
			} else {
				printf("NV data is right!!!.\n");
				return 1;
			}
		} else
			return -1;
	} else {
		/* new version */
		sum = calc_checksum(array, size - 4);
		dataaddr = (unsigned short *)(array + size - 4);

		if (*dataaddr == sum) {
			/* check nv right or wrong */
			if (XCheckNVStruct(array, size) == 0) {
				printf("NV data is crashed!!!.\n");
				return -1;
			} else {
				printf("NV data is right!!!.\n");
				return 1;
			}
		} else {
			printf("NV data crc error\n");
			return -1;
		}
	}
}

unsigned long get_nv_index(unsigned char *array, unsigned long size)
{
	unsigned long index = 0;
	unsigned short sum = 0, *dataaddr;

	if ((array[FIXNV_SIZE - 4] == 0xff) && (array[FIXNV_SIZE - 3] == 0xff) && (array[FIXNV_SIZE - 2] == 0xff) \
		&& (array[FIXNV_SIZE - 1] == 0xff)) {
		/* old version */
		index = 1;
	} else {
		/* new version */
		dataaddr = (unsigned short *)(array + FIXNV_SIZE - 2);
		index = (unsigned long)(*dataaddr);
	}
	return index;
}

/*
* retval : -1 is wrong  ;  1 is correct
*/
int nv_is_correct(unsigned char *array, unsigned long size)
{
	if ((array[size] == 0x5a) && (array[size + 1] == 0x5a) && (array[size + 2] == 0x5a) && (array[size + 3] == 0x5a)) {
		/* check nv right or wrong */
		if (XCheckNVStruct(array, size) == 0) {
			printf("NV data is crashed!!!.\n");
			return -1;
		} else {
			printf("NV data is right!!!.\n");
			array[size] = 0xff; array[size + 1] = 0xff;
			array[size + 2] = 0xff; array[size + 3] = 0xff;
			return 1;
		}
	} else
		return -1;
}

/* check runtimenv */
unsigned long check_npb(struct nv_dev dev, unsigned long size)
{
	struct npb_tag *backup_npb;
	unsigned long ret;

	if ((dev.npb->magic == NV_MAGIC) && (dev.npb->backup_npb)) {
		backup_npb = (struct npb_tag *)(dev.runtime + 1 * 512);
		ret = memcmp((unsigned char *)(dev.npb), (unsigned char *)backup_npb, size);
		if (ret == 0)
			return 1;
	}

	return 0;
}

unsigned long setup_devparam(struct nv_dev *dev)
{
	unsigned long dir_sects;

	dev->tot_size = dev->npb->tot_scts * dev->npb->sct_size;
	if ((dev->npb->max_id <= dev->npb->min_id) || (dev->npb->dir_entry_size != sizeof(struct direntry_tag)) \
		|| (dev->npb->dir_entry_count < (dev->npb->max_id - dev->npb->min_id + 1)))
		return 0;

	dev->first_dir_sct = dev->npb->backup_npb ? 2 : 1;
	dev->first_backup_dir_sct = dev->first_dir_sct;
	dir_sects = (dev->npb->dir_entry_count * dev->npb->dir_entry_size + dev->npb->sct_size - 1) / dev->npb->sct_size;

	if (dev->npb->backup_dir) {
		dev->first_backup_dir_sct += dir_sects;
		dir_sects *= 2;
	}

	dev->data_offset = (dev->first_dir_sct + dir_sects) * dev->npb->sct_size;

	if ((dev->npb->next_offset > dev->tot_size) || (dev->npb->next_offset < dev->data_offset) \
		|| (dev->data_offset >= dev->tot_size))
		return 0;

	return 1;
}

unsigned long check_dir_table(struct nv_dev *dev)
{
	unsigned long dir_sects, i, sct, backup_sct, ret;
	unsigned char *dir, *backup_dir;

	if (dev->npb->backup_npb == 0)
		return 1;
	dir_sects = (dev->npb->dir_entry_count * dev->npb->dir_entry_size + dev->npb->sct_size - 1) / dev->npb->sct_size;
	for (i = 0; i < dir_sects; i ++) {
		sct = dev->first_dir_sct + i;
		backup_sct = dev->first_backup_dir_sct + i;
		dir = dev->runtime + sct * dev->npb->sct_size;
		backup_dir = dev->runtime + backup_sct * dev->npb->sct_size;
		ret = memcmp(dir, backup_dir, dev->npb->sct_size);
		if (ret != 0) {
			printf("sct = %d  backupsct = %d is diffrent\n", sct, backup_sct);
			return 0;
		}
	}

	return 1;
}

unsigned long check_dir_entry(struct nv_dev *dev, unsigned short id, struct direntry_tag *dir)
{
	if (0 == dir->status & STATUS_MASK) {
		if ((dir->offset == 0) && (dir->size == 0))
			return 1;
	} else if ((dir->offset >= dev->data_offset) && (dir->offset < dev->tot_size) && (dir->size <= dev->tot_size) \
			&& ((dir->offset + dir->size) <= dev->tot_size))
		return 1;

	return 0;
}

unsigned long read_dir(struct nv_dev *dev, unsigned short id, struct direntry_tag *dir)
{
	unsigned long addr, main_sct, backup_sct, off, ret;
	unsigned char *direntry;

	addr = (id - dev->npb->min_id) * sizeof(struct direntry_tag);
	main_sct = dev->first_dir_sct + addr / dev->npb->sct_size;
	backup_sct = dev->first_backup_dir_sct + addr / dev->npb->sct_size;
	off = addr % dev->npb->sct_size;
	if ((off + sizeof(struct direntry_tag)) > dev->npb->sct_size)
		return 0;

	direntry = dev->runtime + main_sct * dev->npb->sct_size + off;
	memcpy(dir, direntry, sizeof(struct direntry_tag));
	ret = check_dir_entry(dev, id, dir);
	/*printf("1id=%d offset=0x%08x ", id, main_sct * dev->npb->sct_size + off);
	printf("size=%d checksum=0x%04x offset=0x%08x status=0x%08x ret = %d\n", dir->size, dir->checksum, dir->offset, dir->status, ret);*/
	if (ret)
		return 1;

	direntry = dev->runtime + backup_sct * dev->npb->sct_size + off;
	memcpy(dir, direntry, sizeof(struct direntry_tag));
	ret = check_dir_entry(dev, id, dir);
	/*printf("2id=%d offset=0x%08x ", id, backup_sct * dev->npb->sct_size + off);
	printf("size = %d checksum=0x%04x offset=0x%08x status=0x%08x ret = %d\n", dir->size, dir->checksum, dir->offset, dir->status, ret);*/

	return ret;
}

unsigned long read_itemhdr(struct nv_dev *dev, unsigned long offset, struct item_tag *header)
{
	unsigned char *addr;

	addr = (unsigned char *)(dev->runtime + offset);
	memcpy(header, addr, sizeof(struct item_tag));
}

unsigned long read_itemdata(struct nv_dev *dev, unsigned long offset, unsigned long size, unsigned char *buffer)
{
	unsigned char *addr;

	addr = (unsigned char *)(dev->runtime + offset + sizeof(struct item_tag));
	memcpy(buffer, addr, size);
}


unsigned long check_items(struct nv_dev *dev)
{
	unsigned short id, checksum;
	struct direntry_tag dir;
	struct item_tag header;
	unsigned long bufsize = 64 * 1024;
	unsigned char buf[bufsize];

	for (id = dev->npb->min_id; id < dev->npb->max_id; id ++) {
		if (read_dir(dev, id, &dir) == 0)
			continue;
		if ((dir.status & STATUS_MASK) == 0)
			continue;

		read_itemhdr(dev, dir.offset, &header);
		if ((dir.size != header.size) || (header.id != id)) {
			printf("item header is corrupted id = %d headerid = %d direntry.size = %d header.size = %d\n", id, header.id, dir.size, header.size);
		}

		if (dir.size > bufsize) {
			printf("item size is too large : %d\n", dir.size);
			continue;
		}

		read_itemdata(dev, dir.offset, dir.size, buf);
		checksum = calc_checksum(buf, dir.size);
		if (checksum != dir.checksum) {
			printf("item data is corrupted id = %d orgsum = 0x%04x checksum = 0x%04x\n", id, dir.checksum, checksum);
			return 0;
		}
	}

	return 1;
}

unsigned long XCheckRunningNVStruct(unsigned char *lpPhoBuf, unsigned long dwPhoSize)
{
	struct npb_tag *npb;
	struct nv_dev dev;
	unsigned long ret;

	memset(&dev, 0, sizeof(struct nv_dev));
	npb = (struct npb_tag *)(lpPhoBuf + 0 * 512);
	dev.runtime = lpPhoBuf;
	dev.npb = npb;
	ret = check_npb(dev, 512);
	if (ret == 0) {
		printf("runtimenv is wrong\n");
		return 0;
	}

	/*printf("magic = 0x%08x timestamp = 0x%08x min_id = %d max_id = %d tot_scts = %d sct_size = %d dir_entry_count = %d dir_entry_size = %d next_offset = 0x%08x backup_npb = %d backup_dir = %d\n", npb->magic, npb->timestamp, npb->min_id, npb->max_id, npb->tot_scts, npb->sct_size, npb->dir_entry_count, npb->dir_entry_size, npb->next_offset, npb->backup_npb, npb->backup_dir);*/

	ret = setup_devparam(&dev);
	ret = check_dir_table(&dev);
	ret = check_items(&dev);

	return ret;
}


/*
* retval : -1 is wrong  ;  1 is correct
*/
int runtimenv_is_correct(unsigned char *array, unsigned long size)
{
	unsigned long ret;

	ret = XCheckRunningNVStruct(array, size);
	if (ret == 1) {
		printf("runtimenv is right\n");
		return 1;
	} else
		return -1;
}

/* /* phasecheck : 0 --- 3071; crc : 3072 3073; index : 3074 3075 */
/*
* retval : -1 is wrong  ;  1 is correct
*/
int sn_is_correct(unsigned char *array, unsigned long size)
{
	unsigned long crc;
	unsigned short sum = 0, *dataaddr;

	if (size == PRODUCTINFO_SIZE) {
		sum = calc_checksum(array, size);
		dataaddr = (unsigned short *)(array + size);
		if (*dataaddr == sum) {
			array[size] = 0xff; array[size + 1] = 0xff;
			array[size + 2] = 0xff; array[size + 3] = 0xff;
			return 1;
		}
	}

	printf("phasecheck crc error\n");
	return -1;
}

unsigned long get_productinfo_index(unsigned char *array)
{
	unsigned long index = 0;
	unsigned short sum = 0, *dataaddr;

	dataaddr = (unsigned short *)(array + PRODUCTINFO_SIZE + 2);
	index = (unsigned long)(*dataaddr);
	return index;
}

int sn_is_correct_endflag(unsigned char *array, unsigned long size)
{
	unsigned long crc;
	unsigned short sum = 0, *dataaddr;

	if (size == PRODUCTINFO_SIZE) {
		sum = calc_checksum(array, size);
		dataaddr = (unsigned short *)(array + size);
		if (*dataaddr == sum)
			return 1;
	}

	printf("phasecheck crc error\n");
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

	machine_type = machine_arch_type;         /* get machine type */

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
	} else if ((offset == 4096) || (offset == 8192) || (offset == 12288)) {
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

#if 0
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
#endif

#ifdef BOOTING_BACKUP_NVCALIBRATION
void get_nvitem_from_hostpc(unsigned char *Buffer)
{
	/* samsung implement : get nvitem from HOST PC */
	
}

int eMMC_nv_is_correct(unsigned char *array, unsigned long size)
{
	if ((array[size] == 0x5a) && (array[size + 1] == 0x5a) && (array[size + 2] == 0x5a) && (array[size + 3] == 0x5a)) {
		array[size] = 0xff; array[size + 1] = 0xff;
		array[size + 2] = 0xff; array[size + 3] = 0xff;	
		return 1;
	} else
		return -1;
}

unsigned long Boot_GetPartBaseSec(block_dev_desc_t *p_block_dev, EFI_PARTITION_INDEX part)
{
	disk_partition_t info;

	if (!get_partition_info(p_block_dev, part, &info))
		return info.start;

	return 0;
}

void dump_nvitem(NV_BACKUP_ITEM_T *nvitem)
{
	int cnt2;

	printf("\nszItemName = %s  wIsBackup = %d  wIsUseFlag = %d  dwID = 0x%08x  dwFlagCount = %d\n", nvitem->szItemName, nvitem->wIsBackup, nvitem->wIsUseFlag, nvitem->dwID, nvitem->dwFlagCount);
	
	for (cnt2 = 0; cnt2 < nvitem->dwFlagCount; cnt2 ++) {
		printf("szFlagName = %s  dwCheck = %d\n", nvitem->nbftArray[cnt2].szFlagName, nvitem->nbftArray[cnt2].dwCheck);
	}
}

#endif

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
              free(bmp_img);
		return;
	}
	
    extern int lcd_display_bitmap(ulong bmp_image, int x, int y);
    extern void lcd_display(void);
    extern void *lcd_base;
    extern void Dcache_CleanRegion(unsigned int addr, unsigned int length);
    extern void set_backlight(uint32_t value);
    if(backlight_set == BACKLIGHT_ON){
	    lcd_display_bitmap((ulong)bmp_img, 0, 0);
#ifdef CONFIG_SC8810
	    Dcache_CleanRegion((unsigned int)(lcd_base), size);//Size is to large.
#endif
	    lcd_display();
	    set_backlight(255);
    }else{
        memset((unsigned int)lcd_base, 0, size);
#ifdef CONFIG_SC8810
	    Dcache_CleanRegion((unsigned int)(lcd_base), size);//Size is to large.
#endif
	    lcd_display();
     }
#endif
    set_vibrator(0);

#ifdef BOOTING_BACKUP_NVCALIBRATION
	/* nv backup example : nvitem.bin is from HOST PC, and saved into g_eMMCBuf */
	unsigned char g_eMMCBuf[FIXNV_SIZE + EMMC_SECTOR_SIZE];
	memset(g_eMMCBuf, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
	/* get nvitem.bin from HOST PC and save it into g_eMMCBuf */
	get_nvitem_from_hostpc(g_eMMCBuf);

#define FIX_NV_IS_OK		(1)
#define FIX_BACKUP_NV_IS_OK	(2)

	unsigned char *pDestCode = g_eMMCBuf;
	unsigned long dwCodeSize = FIXNV_SIZE;
	unsigned char g_fix_nv_buf[FIXNV_SIZE + EMMC_SECTOR_SIZE];
	unsigned char g_fixbucknv_buf[FIXNV_SIZE + EMMC_SECTOR_SIZE];

	/* check nv struction from HOST PC */
	/*dump_all_buffer(pDestCode, 1024);*/
	if (!XCheckNVStructEx(pDestCode, dwCodeSize, 0, 1)) {
		printf("NV data from HOST PC is wrong, Failed : Verify error.\n");
		return;
	}
	
	unsigned long backupnvitem_count = sizeof(backupnvitem) / sizeof(NV_BACKUP_ITEM_T);
	unsigned long cnt;
	unsigned long nSectorCount, base_sector, nSectorBase;
	unsigned char *lpReadBuffer = NULL;
	unsigned long dwReadSize = 0;
	int read_nv_check = 0;
	/* printf("backupnvitem_count = %d\n", backupnvitem_count); */
	NV_BACKUP_ITEM_T *pNvBkpItem;

	if (backupnvitem_count) {
		/* read nv from PARTITION_FIX_NV1 */
		memset(g_fix_nv_buf, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
		if (0 == ((FIXNV_SIZE + 4) % EMMC_SECTOR_SIZE))
		 	nSectorCount = (FIXNV_SIZE + 4) / EMMC_SECTOR_SIZE;
		else
		 	nSectorCount = (FIXNV_SIZE + 4) / EMMC_SECTOR_SIZE + 1;

		base_sector = Boot_GetPartBaseSec(p_block_dev, PARTITION_FIX_NV1);
		if (!Emmc_Read(PARTITION_USER, base_sector, nSectorCount, (unsigned char *)g_fix_nv_buf))
			memset(g_fix_nv_buf, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);

		read_nv_check = 0;
		if (eMMC_nv_is_correct(g_fix_nv_buf, FIXNV_SIZE)) {
			/* check nv in PARTITION_FIX_NV1 */
			if (!XCheckNVStructEx(g_fix_nv_buf, FIXNV_SIZE, 0, 1))
				printf("NV data in PARTITION_FIX_NV1 is crashed.\n");
			else
				read_nv_check += FIX_NV_IS_OK;
		}

		/* read nv from PARTITION_FIX_NV2 */
		memset(g_fixbucknv_buf, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
		if (0 == ((FIXNV_SIZE + 4) % EMMC_SECTOR_SIZE))
		 	nSectorCount = (FIXNV_SIZE + 4) / EMMC_SECTOR_SIZE;
		else
		 	nSectorCount = (FIXNV_SIZE + 4) / EMMC_SECTOR_SIZE + 1;

		base_sector = Boot_GetPartBaseSec(p_block_dev, PARTITION_FIX_NV2);
		if (!Emmc_Read(PARTITION_USER, base_sector, nSectorCount, (unsigned char *)g_fixbucknv_buf))
			memset(g_fixbucknv_buf, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);

		if (eMMC_nv_is_correct(g_fixbucknv_buf, FIXNV_SIZE)) {
			/* check nv in PARTITION_FIX_NV2 */
			if (!XCheckNVStructEx(g_fixbucknv_buf, FIXNV_SIZE, 0, 1))
				printf("NV data in PARTITION_FIX_NV2 is crashed.\n");
			else
				read_nv_check += FIX_BACKUP_NV_IS_OK;
		}

		if (read_nv_check > 0) {
			switch (read_nv_check) {
			case (FIX_NV_IS_OK):
				printf("nv is right!\n");
				lpReadBuffer = g_fix_nv_buf;
				dwReadSize = FIXNV_SIZE;
			break;
			case (FIX_BACKUP_NV_IS_OK):
				printf("backupnv is right!\n");
				lpReadBuffer = g_fixbucknv_buf;
				dwReadSize = FIXNV_SIZE;
			break;
			case (FIX_NV_IS_OK + FIX_BACKUP_NV_IS_OK):
				printf("nv and backupnv are all right!\n");
				lpReadBuffer = g_fix_nv_buf;
				dwReadSize = FIXNV_SIZE;
			break;
			}

			/* dump_all_buffer(lpReadBuffer, 1024); */
			if (!XCheckNVStructEx(lpReadBuffer, dwReadSize, 0, 1))
				printf("NV data in phone is crashed.\n");
			else
				printf("NV data in phone is right.\n");

			/* check calibration reserved 7 and struct itself only for GSM */
			/* if (!XCheckCalibration(lpReadBuffer, dwReadSize, 1)) {
				printf("the phone is not to be GSM calibrated.\n");
			} */

			/* calibrate every item in backupnvitem array */
			for (cnt = 0; cnt < backupnvitem_count; cnt ++) {
				pNvBkpItem = (backupnvitem + cnt);
				dump_nvitem(pNvBkpItem);

				unsigned long bReplace = 0;
				unsigned long bContinue = 0;
				unsigned long dwErrorRCID;
				int mm, nNvBkpFlagCount = pNvBkpItem->dwFlagCount;
				
				if (nNvBkpFlagCount > MAX_NV_BACKUP_FALG_NUM)
					nNvBkpFlagCount = MAX_NV_BACKUP_FALG_NUM;

				for (mm = 0; mm < nNvBkpFlagCount; mm++) {
					if (strcmp(pNvBkpItem->nbftArray[mm].szFlagName, "Replace") == 0)
						bReplace = pNvBkpItem->nbftArray[mm].dwCheck;
					else if (strcmp(pNvBkpItem->nbftArray[mm].szFlagName, "Continue") == 0)
						bContinue = pNvBkpItem->nbftArray[mm].dwCheck;
				}
				
				printf("bReplace = %d  bContinue = %d\n", bReplace, bContinue);
				/* ------------------------------ */
				if ((strcmp(pNvBkpItem->szItemName, "Calibration")) == 0) {
					if (pNvBkpItem->wIsBackup == 1) {
						dwErrorRCID = GSMCaliPreserve((unsigned char *)pDestCode, 									dwCodeSize, lpReadBuffer, dwReadSize,
									  bReplace, bContinue, GSMCaliVaPolicy);
						if (dwErrorRCID != 0) {
							printf("Preserve calibration Failed : not Verify.\n");
							return;
						}						
					}					
				} else if ((strcmp(pNvBkpItem->szItemName, "TD_Calibration")) == 0) {
					if (pNvBkpItem->wIsBackup == 1) {
						dwErrorRCID = XTDCaliPreserve((unsigned char *)pDestCode, 									dwCodeSize, lpReadBuffer, dwReadSize,	 										bReplace, bContinue);
						if (dwErrorRCID != 0) {
							printf("Preserve TD calibration Failed : not Verify.\n");
							return;
						}
					}
				} else if ((strcmp(pNvBkpItem->szItemName, "LTE_Calibration")) == 0) {
					if (pNvBkpItem->wIsBackup == 1) {
						dwErrorRCID = LTECaliPreserve((unsigned char *)pDestCode, 									dwCodeSize, lpReadBuffer, dwReadSize, 										bReplace, bContinue);
						if (dwErrorRCID != 0) {
							printf("Preserve LTE calibration Failed : not Verify.\n");
							return;
						}
					}
				} else if ((strcmp(pNvBkpItem->szItemName, "IMEI")) == 0) {
					if (pNvBkpItem->wIsBackup == 1) {
						dwErrorRCID = XPreserveIMEIs((unsigned short)pNvBkpItem->dwID, 
								(unsigned char *)pDestCode, dwCodeSize, 								lpReadBuffer, dwReadSize, bReplace, bContinue);
						if (dwErrorRCID != 0) {
							printf("Preserve %s Failed : not Verify.\n", 
								pNvBkpItem->szItemName);
							return;
						}	
					}					
				} else {
					if ((strcmp(pNvBkpItem->szItemName, "MMITest") == 0) || 
						(strcmp(pNvBkpItem->szItemName, "MMITest Result") == 0))
							bContinue = 1;

					if (pNvBkpItem->wIsBackup == 1 && pNvBkpItem->dwID != 0xFFFFFFFF) {
						dwErrorRCID = XPreserveNVItem((unsigned short)(pNvBkpItem->dwID), 									(unsigned char *)pDestCode, dwCodeSize, 								lpReadBuffer, dwReadSize, bReplace, bContinue);
						if (dwErrorRCID != 0) {
							printf("Preserve %s Failed : not Verify.\n", 
								pNvBkpItem->szItemName);
							return;
						}	
					}	
				}
				/* ------------------------------ */
			} /* for (cnt = 0; cnt < backupnvitem_count; cnt ++) */
		} else /* if (read_nv_check > 0) */
			printf("nv and backupnv are all empty or wrong, so download nv from DLoad Tools only.\n");
	} else /* if (backupnvitem_count) */
		printf("backupnvitem is empty, so don't backup nv and download nv from DLoad Tools only.\n");

	g_eMMCBuf[FIXNV_SIZE + 0] = g_eMMCBuf[FIXNV_SIZE + 1] = 0x5a;
	g_eMMCBuf[FIXNV_SIZE + 2] = g_eMMCBuf[FIXNV_SIZE + 3] = 0x5a;

	if (0 == ((FIXNV_SIZE + 4) % EMMC_SECTOR_SIZE))
		nSectorCount = (FIXNV_SIZE + 4) / EMMC_SECTOR_SIZE;
	else
		nSectorCount = (FIXNV_SIZE + 4) / EMMC_SECTOR_SIZE + 1;

	memset(g_fix_nv_buf, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
	memcpy(g_fix_nv_buf, g_eMMCBuf, FIXNV_SIZE + EMMC_SECTOR_SIZE);
	nSectorBase = Boot_GetPartBaseSec(p_block_dev, PARTITION_FIX_NV1);
	nv_erase_partition(p_block_dev, PARTITION_FIX_NV1);
	if (!Emmc_Write(PARTITION_USER, nSectorBase, nSectorCount, (unsigned char *)g_fix_nv_buf))
		return;

	memset(g_fixbucknv_buf, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
	memcpy(g_fixbucknv_buf, g_fix_nv_buf, FIXNV_SIZE + EMMC_SECTOR_SIZE);
	nSectorBase = Boot_GetPartBaseSec(p_block_dev, PARTITION_FIX_NV2);
	nv_erase_partition(p_block_dev, PARTITION_FIX_NV2);	
	if (!Emmc_Write(PARTITION_USER, nSectorBase, nSectorCount, (unsigned char *)g_fixbucknv_buf))
		return;

#endif /* BOOTING_BACKUP_NVCALIBRATION */

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
			if(nv_read_partition(p_block_dev, PARTITION_RUNTIME_NV2, (char *)RUNTIMENV_ADR, RUNTIMENV_SIZE + 4) == 0) {
				if (-1 == nv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
					/* file isn't right */
					memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + EMMC_SECTOR_SIZE);
				}
			}
		}
	} else {
		/* file don't exist and read backup file */
		memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + EMMC_SECTOR_SIZE);
		if(nv_read_partition(p_block_dev, PARTITION_RUNTIME_NV2, (char *)RUNTIMENV_ADR, RUNTIMENV_SIZE + 4) == 0){
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
	secure_check(DSP_ADR, 0, DSP_ADR + DSP_SIZE - VLR_INFO_OFF, CONFIG_SYS_NAND_U_BOOT_DST + CONFIG_SYS_NAND_U_BOOT_SIZE - KEY_INFO_SIZ - VLR_INFO_OFF);
                 
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
	secure_check(MODEM_ADR, 0, MODEM_ADR + MODEM_SIZE - VLR_INFO_OFF, CONFIG_SYS_NAND_U_BOOT_DST + CONFIG_SYS_NAND_U_BOOT_SIZE - KEY_INFO_SIZ - VLR_INFO_OFF);
	 

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
	secure_check(VMJALUNA_ADR, 0, VMJALUNA_ADR + VMJALUNA_SIZE - VLR_INFO_OFF, CONFIG_SYS_NAND_U_BOOT_DST + CONFIG_SYS_NAND_U_BOOT_SIZE - KEY_INFO_SIZ - VLR_INFO_OFF);
#endif

	//array_value((unsigned char *)VMJALUNA_ADR, 16 * 10);
    //check caliberation mode
    int str_len;
    char * buf;
    buf = malloc(1024);
	memset(buf, 0, 1024);

    sprintf(buf, "initrd=0x%x,0x%x", RAMDISK_ADR, hdr->ramdisk_size);

    /* preset loop_per_jiffy */
    str_len = strlen(buf);
#ifdef CONFIG_LOOP_PER_JIFFY
    sprintf(&buf[str_len], " lpj=%d", CONFIG_LOOP_PER_JIFFY);
#else
    sprintf(&buf[str_len], " lpj=%d", 3350528); /* SC8810 1GHz */
#endif

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

#if !BOOT_NATIVE_LINUX
	/* fixnv=0x????????,0x????????*/
	str_len = strlen(buf);
	sprintf(&buf[str_len], " fixnv=0x");
	str_len = strlen(buf);
	sprintf(&buf[str_len], "%08x", FIXNV_ADR);
	str_len = strlen(buf);
	sprintf(&buf[str_len], ",0x");
	str_len = strlen(buf);
	sprintf(&buf[str_len], "%x", FIXNV_SIZE);

	/* productinfo=0x????????,0x????????*/
	str_len = strlen(buf);
	sprintf(&buf[str_len], " productinfo=0x");
	str_len = strlen(buf);
	sprintf(&buf[str_len], "%08x", PRODUCTINFO_ADR);
	str_len = strlen(buf);
	sprintf(&buf[str_len], ",0x");
	str_len = strlen(buf);
	sprintf(&buf[str_len], "%x", PRODUCTINFO_SIZE);
	
	/* productinfo=0x????????,0x????????*/
	str_len = strlen(buf);
	sprintf(&buf[str_len], " runtimenv=0x");
	str_len = strlen(buf);
	sprintf(&buf[str_len], "%08x", RUNTIMENV_ADR);
	str_len = strlen(buf);
	sprintf(&buf[str_len], ",0x");
	str_len = strlen(buf);
	sprintf(&buf[str_len], "%x", RUNTIMENV_SIZE);
#endif

	size = CONFIG_SPL_LOAD_LEN; 
	if(TRUE !=  Emmc_Read(PARTITION_BOOT1, 0, size/EMMC_SECTOR_SIZE, (uint8*)spl_data)){
		printf("vmjaluna nand read error \n");
		return;
	}

	if(harsh_data == NULL){
		printf("harsh_data malloc failed\n");
		return;
	}
	printf("spl_data adr 0x%x harsh_data adr 0x%x\n", spl_data, harsh_data);
	ret = cal_md5(spl_data, CONFIG_SPL_LOAD_LEN, harsh_data);
	if(ret){
		str_len = strlen(buf);
		sprintf(&buf[str_len], " securemd5=%08x%08x%08x%08x", *(uint32_t*)harsh_data, *(uint32_t*)(harsh_data+4),\
			*(uint32_t*)(harsh_data+8), *(uint32_t*)(harsh_data+12));
	}

    printf("pass cmdline: %s\n", buf);
    //lcd_printf(" pass cmdline : %s\n",buf);
    //lcd_display();
    creat_atags(VLX_TAG_ADDR, buf, NULL, 0);
    void (*entry)(void) = (void*) VMJALUNA_ADR;
#ifndef CONFIG_SC8810
#ifndef CONFIG_TIGER
    MMU_InvalideICACHEALL();
#endif
#endif
#ifdef CONFIG_SC8810
    MMU_DisableIDCM();
#endif

#ifdef REBOOT_FUNCTION_INUBOOT
	reboot_func();
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
	char *backupfixnvpoint = "/backupfixnv";
	char *backupfixnvfilename = "/backupfixnv/fixnv.bin";
	char *runtimenvpoint = "/runtimenv";
	char *runtimenvfilename = "/runtimenv/runtimenv.bin";
	char *runtimenvfilename2 = "/runtimenv/runtimenvbkup.bin";
	char *productinfopoint = "/productinfo";
	char *productinfofilename = "/productinfo/productinfo.bin";
	char *productinfofilename2 = "/productinfo/productinfobkup.bin";
	int orginal_right, backupfile_right;
	unsigned long orginal_index, backupfile_index;
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
	orginal_right = 0;
	memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
	cmd_yaffs_mount(fixnvpoint);
	ret = cmd_yaffs_ls_chk(fixnvfilename);
	if (ret == (FIXNV_SIZE + 4)) {
		cmd_yaffs_mread_file(fixnvfilename, (unsigned char *)FIXNV_ADR);
		if (1 == fixnv_is_correct_endflag((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
			orginal_right = 1;//right
	}

	cmd_yaffs_umount(fixnvpoint);

	backupfile_right = 0;
	memset((unsigned char *)RUNTIMENV_ADR, 0xff, FIXNV_SIZE + 4);
	cmd_yaffs_mount(backupfixnvpoint);
	ret = cmd_yaffs_ls_chk(backupfixnvfilename);
	if (ret == (FIXNV_SIZE + 4)) {
		cmd_yaffs_mread_file(backupfixnvfilename, (unsigned char *)RUNTIMENV_ADR);
		if (1 == fixnv_is_correct_endflag((unsigned char *)RUNTIMENV_ADR, FIXNV_SIZE))
			backupfile_right = 1;//right
	}
	cmd_yaffs_umount(backupfixnvpoint);

	printf("orginal_right = %d  backupfile_right = %d\n", orginal_right, backupfile_right);
	if ((orginal_right == 1) && (backupfile_right == 1)) {
		/* check index */
		orginal_index = get_nv_index((unsigned char *)FIXNV_ADR, FIXNV_SIZE);
		backupfile_index = get_nv_index((unsigned char *)RUNTIMENV_ADR, FIXNV_SIZE);
		printf("1orginal_index = %d  backupfile_index = %d\n", orginal_index, backupfile_index);
		if (orginal_index != backupfile_index) {
			orginal_right = 1;
			backupfile_right = 0;
		}
	}

	if ((orginal_right == 1) && (backupfile_right == 0)) {
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
	} else if ((orginal_right == 0) && (backupfile_right == 1)) {
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
	} else if ((orginal_right == 0) && (backupfile_right == 0)) {
		printf("\n\nfixnv and backupfixnv are all wrong.\n\n");
	}
	///////////////////////////////////////////////////////////////////////
	/* FIXNV_PART */
	printf("Reading fixnv to 0x%08x\n", FIXNV_ADR);
	memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
    	cmd_yaffs_mount(fixnvpoint);
	ret = cmd_yaffs_ls_chk(fixnvfilename);
	if (ret == (FIXNV_SIZE + 4)) {
		cmd_yaffs_mread_file(fixnvfilename, (unsigned char *)FIXNV_ADR);
		cmd_yaffs_umount(fixnvpoint);
		if (-1 == fixnv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
			memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
			/* backup nv start */
			cmd_yaffs_mount(backupfixnvpoint);
			ret = cmd_yaffs_ls_chk(backupfixnvfilename);
			if (ret == (FIXNV_SIZE + 4)) {
				cmd_yaffs_mread_file(backupfixnvfilename, (unsigned char *)FIXNV_ADR);
				cmd_yaffs_umount(backupfixnvpoint);
				if (-1 == fixnv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
					memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
				}
			} else
				cmd_yaffs_umount(backupfixnvpoint);
			/* backup nv end */
		}
	} else {
		cmd_yaffs_umount(fixnvpoint);
		/* backup nv start */
		cmd_yaffs_mount(backupfixnvpoint);
		ret = cmd_yaffs_ls_chk(backupfixnvfilename);
		if (ret == (FIXNV_SIZE + 4)) {
			cmd_yaffs_mread_file(backupfixnvfilename, (unsigned char *)FIXNV_ADR);
			cmd_yaffs_umount(backupfixnvpoint);
			if (-1 == fixnv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
				memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + 4);
			}
		} else
			cmd_yaffs_umount(backupfixnvpoint);
		/* backup nv end */
	}
	//array_value((unsigned char *)FIXNV_ADR, FIXNV_SIZE);


	/* recovery damaged productinfo or productinfobkup */
	orginal_right = 0;
	memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + 4);
	cmd_yaffs_mount(productinfopoint);
	ret = cmd_yaffs_ls_chk(productinfofilename);
	if (ret == (PRODUCTINFO_SIZE + 4)) {
		cmd_yaffs_mread_file(productinfofilename, (unsigned char *)PRODUCTINFO_ADR);
		if (1 == sn_is_correct_endflag((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE))
			orginal_right = 1;//right
	}
	cmd_yaffs_umount(productinfopoint);

	backupfile_right = 0;
	memset((unsigned char *)RUNTIMENV_ADR, 0xff, PRODUCTINFO_SIZE + 4);
	cmd_yaffs_mount(productinfopoint);
	ret = cmd_yaffs_ls_chk(productinfofilename2);
	if (ret == (PRODUCTINFO_SIZE + 4)) {
		cmd_yaffs_mread_file(productinfofilename2, (unsigned char *)RUNTIMENV_ADR);
		if (1 == sn_is_correct_endflag((unsigned char *)RUNTIMENV_ADR, PRODUCTINFO_SIZE))
			backupfile_right = 1;//right
	}
	cmd_yaffs_umount(productinfopoint);
	if ((orginal_right == 1) && (backupfile_right == 1)) {
		/* check index */
		orginal_index = get_productinfo_index((unsigned char *)PRODUCTINFO_ADR);
		backupfile_index = get_productinfo_index((unsigned char *)RUNTIMENV_ADR);
		if (orginal_index != backupfile_index) {
			orginal_right = 1;
			backupfile_right = 0;
		}
	}
	if ((orginal_right == 1) && (backupfile_right == 0)) {
		printf("productinfo is right, but productinfobkup is wrong, so recovery productinfobkup\n");
		cmd_yaffs_mount(productinfopoint);
		cmd_yaffs_mwrite_file(productinfofilename2, (char *)PRODUCTINFO_ADR, (PRODUCTINFO_SIZE + 4));
		cmd_yaffs_ls_chk(productinfofilename2);
		cmd_yaffs_umount(productinfopoint);
	} else if ((orginal_right == 0) && (backupfile_right == 1)) {
		printf("productinfobkup is right, but productinfo is wrong, so recovery productinfo\n");
		cmd_yaffs_mount(productinfopoint);
		cmd_yaffs_mwrite_file(productinfofilename, (char *)RUNTIMENV_ADR, (PRODUCTINFO_SIZE + 4));
		cmd_yaffs_ls_chk(productinfofilename);
		cmd_yaffs_umount(productinfopoint);
	} else if ((orginal_right == 0) && (backupfile_right == 0)) {
		printf("\n\nproductinfo and productinfobkup are all wrong or no phasecheck.\n\n");
	}
	///////////////////////////////////////////////////////////////////////
	/* PRODUCTINFO_PART */
	printf("Reading productinfo to 0x%08x\n", PRODUCTINFO_ADR);
    	cmd_yaffs_mount(productinfopoint);
	ret = cmd_yaffs_ls_chk(productinfofilename);
	if (ret == (PRODUCTINFO_SIZE + 4)) {
		cmd_yaffs_mread_file(productinfofilename, (unsigned char *)PRODUCTINFO_ADR);
		if (-1 == sn_is_correct((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE)) {
			memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + 4);
			ret = cmd_yaffs_ls_chk(productinfofilename2);
			if (ret == (PRODUCTINFO_SIZE + 4)) {
				cmd_yaffs_mread_file(productinfofilename2, (unsigned char *)PRODUCTINFO_ADR);
				if (-1 == sn_is_correct((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE)) {
					memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + 4);
				}
			}
		}
	} else {
		memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + 4);
		ret = cmd_yaffs_ls_chk(productinfofilename2);
		if (ret == (PRODUCTINFO_SIZE + 4)) {
			cmd_yaffs_mread_file(productinfofilename2, (unsigned char *)PRODUCTINFO_ADR);
			if (-1 == sn_is_correct((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE)) {
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
	/* recovery damaged runtimenv or runtimenvbkup */
	orginal_right = 0;
	memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE);
	cmd_yaffs_mount(runtimenvpoint);
	ret = cmd_yaffs_ls_chk(runtimenvfilename);
	if (ret == RUNTIMENV_SIZE) {
		cmd_yaffs_mread_file(runtimenvfilename, (unsigned char *)RUNTIMENV_ADR);
		if (1 == runtimenv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE))
			orginal_right = 1;//right
	}
	cmd_yaffs_umount(runtimenvpoint);

	backupfile_right = 0;
	memset((unsigned char *)DSP_ADR, 0xff, RUNTIMENV_SIZE);
	cmd_yaffs_mount(runtimenvpoint);
	ret = cmd_yaffs_ls_chk(runtimenvfilename2);
	if (ret == RUNTIMENV_SIZE) {
		cmd_yaffs_mread_file(runtimenvfilename2, (unsigned char *)DSP_ADR);
		if (1 == runtimenv_is_correct((unsigned char *)DSP_ADR, RUNTIMENV_SIZE))
			backupfile_right = 1;//right
	}
	cmd_yaffs_umount(runtimenvpoint);
	printf("orginal_right = %d  backupfile_right = %d\n", orginal_right, backupfile_right);
	if ((orginal_right == 1) && (backupfile_right == 0)) {
		printf("runtimenv is right, but runtimenvbkup is wrong, so recovery runtimenvbkup\n");
		cmd_yaffs_mount(runtimenvpoint);
		cmd_yaffs_mwrite_file(runtimenvfilename2, (char *)RUNTIMENV_ADR, RUNTIMENV_SIZE);
		cmd_yaffs_ls_chk(runtimenvfilename2);
		cmd_yaffs_umount(runtimenvpoint);
	} else if ((orginal_right == 0) && (backupfile_right == 1)) {
		printf("productinfobkup is right, but productinfo is wrong, so recovery productinfo\n");
		cmd_yaffs_mount(runtimenvpoint);
		cmd_yaffs_mwrite_file(runtimenvfilename, (char *)DSP_ADR, RUNTIMENV_SIZE);
		cmd_yaffs_ls_chk(runtimenvfilename);
		cmd_yaffs_umount(runtimenvpoint);
	} else if ((orginal_right == 0) && (backupfile_right == 0)) {
		printf("\n\nruntimenv and runtimenvbkup are all wrong or no runtimenv.\n\n");
	}

	printf("Reading runtimenv to 0x%08x\n", RUNTIMENV_ADR);
	/* runtimenv */
    cmd_yaffs_mount(runtimenvpoint);
	ret = cmd_yaffs_ls_chk(runtimenvfilename);
	if (ret == RUNTIMENV_SIZE) {
		/* file exist */
		cmd_yaffs_mread_file(runtimenvfilename, (unsigned char *)RUNTIMENV_ADR);
		if (-1 == runtimenv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
			/* file isn't right and read backup file */
			memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE);
			ret = cmd_yaffs_ls_chk(runtimenvfilename2);
			if (ret == RUNTIMENV_SIZE) {
				cmd_yaffs_mread_file(runtimenvfilename2, (unsigned char *)RUNTIMENV_ADR);
				if (-1 == nv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
					/* file isn't right */
					memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE);
				}
			}
		}
	} else {
		/* file don't exist and read backup file */
		memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE);
		ret = cmd_yaffs_ls_chk(runtimenvfilename2);
		if (ret == RUNTIMENV_SIZE) {
			cmd_yaffs_mread_file(runtimenvfilename2, (unsigned char *)RUNTIMENV_ADR);
			if (-1 == nv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
				/* file isn't right */
				memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE);
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
	secure_check(DSP_ADR, 0, DSP_ADR + DSP_SIZE - VLR_INFO_OFF, CONFIG_SYS_NAND_U_BOOT_DST + CONFIG_SYS_NAND_U_BOOT_SIZE - KEY_INFO_SIZ - VLR_INFO_OFF);
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
	flash_page_size = nand->writesize;
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

	secure_check(MODEM_ADR, 0, MODEM_ADR + MODEM_SIZE - VLR_INFO_OFF, CONFIG_SYS_NAND_U_BOOT_DST + CONFIG_SYS_NAND_U_BOOT_SIZE - KEY_INFO_SIZ - VLR_INFO_OFF);
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
	secure_check(VMJALUNA_ADR, 0, VMJALUNA_ADR + VMJALUNA_SIZE - VLR_INFO_OFF, CONFIG_SYS_NAND_U_BOOT_DST + CONFIG_SYS_NAND_U_BOOT_SIZE - KEY_INFO_SIZ - VLR_INFO_OFF);
#endif

	//array_value((unsigned char *)VMJALUNA_ADR, 16 * 10);
    //check caliberation mode
    int str_len;
    char * buf;
    buf = malloc(1024);
	memset(buf, 0, 1024);

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
	if(spl_data == NULL){
		printf("spl_data malloc failed\n");
		return;
	}
	ret = find_dev_and_part(SPL_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", SPL_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", SPL_PART);
		return;
	}
	off = part->offset;
	nand = &nand_info[dev->id->num];
	flash_page_size = nand->writesize;
	size = CONFIG_SPL_LOAD_LEN; 
	ret = nand_read_offset_ret(nand, off, &size, (void*)spl_data, &off);
	if(ret != 0) {
		printf("spl nand read error %d\n", ret);
		return;
	}
	if(size != CONFIG_SPL_LOAD_LEN){
		printf("spl nand read length 0x%x != 0x%x\n", size, CONFIG_SPL_LOAD_LEN);
		return;
	}
	if(harsh_data == NULL){
		printf("harsh_data malloc failed\n");
		return;
	}
	printf("spl_data adr 0x%x harsh_data adr 0x%x\n", spl_data, harsh_data);
	ret = cal_md5(spl_data, CONFIG_SPL_LOAD_LEN, harsh_data);
	if(ret){
		str_len = strlen(buf);
		sprintf(&buf[str_len], " securemd5=%08x%08x%08x%08x", *(uint32_t*)harsh_data, *(uint32_t*)(harsh_data+4),\
			*(uint32_t*)(harsh_data+8), *(uint32_t*)(harsh_data+12));
	}
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
