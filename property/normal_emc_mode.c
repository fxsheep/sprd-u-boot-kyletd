#include "normal_mode.h"

#include "../disk/part_uefi.h"
#include "../drivers/mmc/card_sdio.h"
#include "asm/arch/sci_types.h"
#include <ext_common.h>
#include <ext4fs.h>
#ifdef BOOTING_BACKUP_NVCALIBRATION
#include "backupnvitem.h"
#endif

#define KERNL_PAGE_SIZE 2048
int get_partition_info (block_dev_desc_t *dev_desc, int part, disk_partition_t *info);

int nv_erase_partition(block_dev_desc_t *p_block_dev, EFI_PARTITION_INDEX part)
{
	disk_partition_t info;
	int ret = 0; /* success */
	unsigned char *tmpbuf = (unsigned char *)MODEM_ADR;

	if (!get_partition_info(p_block_dev, part, &info)) {
		memset(tmpbuf, 0xff, info.size * EMMC_SECTOR_SIZE);
		//printf("part = %d  info.start = 0x%08x  info.size = 0x%08x\n", part, info.start, info.size);
		if (TRUE !=  Emmc_Write(PARTITION_USER, info.start, info.size, (unsigned char *)tmpbuf)) {
			printf("emmc image erase error \n");
			ret = 1; /* fail */
		}
	}

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

int prodinfo_read_partition_flag(block_dev_desc_t *p_block_dev, EFI_PARTITION_INDEX part, int offset, char *buf, int len)
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

	if (offset == 0) {
		/* phasecheck */
		if ((buf[PRODUCTINFO_SIZE + 7] == (crc & 0xff)) \
			&& (buf[PRODUCTINFO_SIZE + 6] == ((crc & (0xff << 8)) >> 8)) \
			&& (buf[PRODUCTINFO_SIZE + 5] == ((crc & (0xff << 16)) >> 16)) \
			&& (buf[PRODUCTINFO_SIZE + 4] == ((crc & (0xff << 24)) >> 24))) {
			ret = 0;
		} else
			ret = 1;
	} else if ((offset == 4096) || (offset == 8192) || (offset == 12288)) {
		/* factorymode or alarm mode */
		if ((buf[PRODUCTINFO_SIZE + 11] == (crc & 0xff)) \
			&& (buf[PRODUCTINFO_SIZE + 10] == ((crc & (0xff << 8)) >> 8)) \
			&& (buf[PRODUCTINFO_SIZE + 9] == ((crc & (0xff << 16)) >> 16)) \
			&& (buf[PRODUCTINFO_SIZE + 8] == ((crc & (0xff << 24)) >> 24))) {
			ret = 0;
		} else
			ret = 1;
	} else
		ret = 1;

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
				/* clear 5a flag */
				buf[PRODUCTINFO_SIZE] = 0xff;
				buf[PRODUCTINFO_SIZE + 1] = 0xff;
				buf[PRODUCTINFO_SIZE + 2] = 0xff;
				buf[PRODUCTINFO_SIZE + 3] = 0xff;
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

int read_logoimg(char *bmp_img,size_t size)
{
	block_dev_desc_t *p_block_dev = NULL;
	disk_partition_t info;

	p_block_dev = get_dev("mmc", 1);
	if(NULL == p_block_dev){
		return -1;
	}
	if (!get_partition_info(p_block_dev, PARTITION_LOGO, &info)) {
		if(TRUE !=  Emmc_Read(PARTITION_USER, info.start, size/EMMC_SECTOR_SIZE, bmp_img)){
			printf("function: %s nand read error\n", __FUNCTION__);
			return -1;
		}
	}
	return 0;
}

int is_factorymode()
{
	int ret = 0;
	block_dev_desc_t *p_block_dev = NULL;

	printf("Checking factorymode : ");
	p_block_dev = get_dev("mmc", 1);
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
	return ret;
}
void addbuf(char *buf)
{
}
void addcmdline(char *buf)
{
#if !BOOT_NATIVE_LINUX
	/* fixnv=0x????????,0x????????*/
	int str_len = strlen(buf);
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
}

int read_spldata()
{
	int size = CONFIG_SPL_LOAD_LEN;
	if(TRUE !=  Emmc_Read(PARTITION_BOOT1, 0, size/EMMC_SECTOR_SIZE, (uint8*)spl_data)){
		printf("vmjaluna nand read error \n");
		return -1;
	}
	return 0;
}

/*The function is temporary, will move to the chip directory*/
#if BOOT_NATIVE_LINUX_MODEM
void modem_entry()
{
#ifdef CONFIG_SC8825
	//  *(volatile u32 *)0x4b00100c=0x100;
	u32 cpdata[3] = {0xe59f0000, 0xe12fff10, MODEM_ADR};
	*(volatile u32*)0x20900250 = 0;//disbale cp clock, cp iram select to ap
	//*(volatile u32*)0x20900254 = 0;// hold cp
	memcpy((volatile u32*)0x30000, cpdata, sizeof(cpdata));
	*(volatile u32*)0x20900250 =0xf;// 0x3;//enale cp clock, cp iram select to cp
	*(volatile u32*)0x20900254 = 1;// reset cp
#endif
}

void sipc_addr_reset()
{
	memset((void *)SIPC_APCP_START_ADDR, 0x0, SIPC_APCP_RESET_ADDR_SIZE);
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
	int orginal_right, backupfile_right;
	unsigned long orginal_index, backupfile_index;
	nand_erase_options_t opts;
	char * mtdpart_def = NULL;
	disk_partition_t info;
	int filelen;
	ulong part_length;
	#if (defined CONFIG_SC8810) || (defined CONFIG_SC8825)
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

   lcd_display_logo(backlight_set,(ulong)bmp_img,size);
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

#if((!BOOT_NATIVE_LINUX)||(BOOT_NATIVE_LINUX_MODEM))

	/* recovery damaged fixnv or backupfixnv */
	orginal_right = 0;
	memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
	if(0 == nv_read_partition(p_block_dev, PARTITION_FIX_NV1, (char *)FIXNV_ADR, FIXNV_SIZE + 4)){
		if (1 == fixnv_is_correct_endflag((unsigned char *)FIXNV_ADR, FIXNV_SIZE))
			orginal_right = 1;//right
	}

    backupfile_right = 0;
	memset((unsigned char *)RUNTIMENV_ADR, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
	if(0 == nv_read_partition(p_block_dev, PARTITION_FIX_NV2, (char *)RUNTIMENV_ADR, FIXNV_SIZE + 4)){
		if (1 == fixnv_is_correct_endflag((unsigned char *)RUNTIMENV_ADR, FIXNV_SIZE))
			backupfile_right = 1;//right
	}

	if ((orginal_right == 1) && (backupfile_right == 1)) {
		/* check index */
		orginal_index = get_nv_index((unsigned char *)FIXNV_ADR, FIXNV_SIZE);
		backupfile_index = get_nv_index((unsigned char *)RUNTIMENV_ADR, FIXNV_SIZE);
		if (orginal_index != backupfile_index) {
			orginal_right = 1;
			backupfile_right = 0;
		}
	}

    if ((orginal_right == 1) && (backupfile_right == 0)) {
		printf("fixnv is right, but backupfixnv is wrong, so erase and recovery backupfixnv\n");
		nv_erase_partition(p_block_dev, PARTITION_FIX_NV2);
		nv_write_partition(p_block_dev, PARTITION_FIX_NV2, (char *)FIXNV_ADR, (FIXNV_SIZE + 4));
	} else if ((orginal_right == 0) && (backupfile_right == 1)) {
		printf("backupfixnv is right, but fixnv is wrong, so erase and recovery fixnv\n");
		nv_erase_partition(p_block_dev, PARTITION_FIX_NV1);
		nv_write_partition(p_block_dev, PARTITION_FIX_NV1, (char *)RUNTIMENV_ADR, (FIXNV_SIZE + 4));
	} else if ((orginal_right == 0) && (backupfile_right == 0)) {
		printf("\n\nfixnv and backupfixnv are all wrong.\n\n");
	}
	///////////////////////////////////////////////////////////////////////
	/* FIXNV_PART */
	printf("Reading fixnv to 0x%08x\n", FIXNV_ADR);
	memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
	/* fixnv */
	if (nv_read_partition(p_block_dev, PARTITION_FIX_NV1, (char *)FIXNV_ADR, FIXNV_SIZE + 4) == 0) {
		if (-1 == fixnv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
			printf("nv is wrong, read backup nv\n");
			memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
			if (nv_read_partition(p_block_dev, PARTITION_FIX_NV2, (char *)FIXNV_ADR, FIXNV_SIZE + 4) == 0) {
				if (-1 == fixnv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
					memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
					printf("nv and backup nv are all wrong!\n");
				}
			} else {
				printf("read backup nv fail\n");
				memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
				printf("nv and backup nv are all wrong!\n");
			}
		}
	} else {
		printf("read nv fail, read backup nv\n");
		if (nv_read_partition(p_block_dev, PARTITION_FIX_NV2, (char *)FIXNV_ADR, FIXNV_SIZE + 4) == 0) {
			if (-1 == fixnv_is_correct((unsigned char *)FIXNV_ADR, FIXNV_SIZE)) {
				memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
				printf("nv and backup nv are all wrong!\n");
			}
		} else {
			printf("read backup nv fail\n");
			memset((unsigned char *)FIXNV_ADR, 0xff, FIXNV_SIZE + EMMC_SECTOR_SIZE);
			printf("nv and backup nv are all wrong!\n");
		}
	}
	//array_value((unsigned char *)FIXNV_ADR, FIXNV_SIZE);

	///////////////////////////////////////////////////////////////////////
	/* PRODUCTINFO_PART */
	orginal_right = 0;
	memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
	if(prodinfo_read_partition_flag(p_block_dev, PARTITION_PROD_INFO1, 0, (char *)PRODUCTINFO_ADR,
		PRODUCTINFO_SIZE + 4) == 0){
		orginal_right = 1;
	}

	backupfile_right = 0;
	memset((unsigned char *)RUNTIMENV_ADR, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
	if(prodinfo_read_partition_flag(p_block_dev, PARTITION_PROD_INFO2, 0, (char *)RUNTIMENV_ADR,
		PRODUCTINFO_SIZE + 4) == 0){
			backupfile_right = 1;
	}

	if ((orginal_right == 1) && (backupfile_right == 0)) {
		printf("productinfo is right, but productinfobkup is wrong, so recovery productinfobkup\n");
		nv_erase_partition(p_block_dev, PARTITION_PROD_INFO2);
		nv_write_partition(p_block_dev, PARTITION_PROD_INFO2, (char *)PRODUCTINFO_ADR, (PRODUCTINFO_SIZE + 8));
	} else if ((orginal_right == 0) && (backupfile_right == 1)) {
		printf("productinfobkup is right, but productinfo is wrong, so recovery productinfo\n");
		nv_erase_partition(p_block_dev, PARTITION_PROD_INFO1);
		nv_write_partition(p_block_dev, PARTITION_PROD_INFO1, (char *)RUNTIMENV_ADR, (PRODUCTINFO_SIZE + 8));
	} else if ((orginal_right == 0) && (backupfile_right == 0)) {
		printf("\n\nproductinfo and productinfobkup are all wrong or no phasecheck.\n\n");
	}

	printf("Reading productinfo to 0x%08x\n", PRODUCTINFO_ADR);
	memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE +  EMMC_SECTOR_SIZE);
	if(prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO1, 0, (char *)PRODUCTINFO_ADR, 
		PRODUCTINFO_SIZE + 4) == 0){
		printf("productinfo is right\n");
	} else {
		memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + EMMC_SECTOR_SIZE);
		if(prodinfo_read_partition(p_block_dev, PARTITION_PROD_INFO2, 0, (char *)PRODUCTINFO_ADR, 
			PRODUCTINFO_SIZE + 4) == 0) {
			printf("productbackinfo is right\n");
		} else
			memset((unsigned char *)PRODUCTINFO_ADR, 0xff, PRODUCTINFO_SIZE + EMMC_SECTOR_SIZE);
	}
	//array_value((unsigned char *)PRODUCTINFO_ADR, PRODUCTINFO_SIZE);
	eng_phasechecktest((unsigned char *)PRODUCTINFO_ADR, SP09_MAX_PHASE_BUFF_SIZE);

	/* RUNTIMEVN_PART */
	orginal_right = 0;
	memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + EMMC_SECTOR_SIZE);
	if(nv_read_partition(p_block_dev, PARTITION_RUNTIME_NV1, (char *)RUNTIMENV_ADR, RUNTIMENV_SIZE) == 0) {
		if (1 == runtimenv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
			orginal_right = 1;//right
		}
	}

	backupfile_right = 0;
	memset((unsigned char *)DSP_ADR, 0xff, RUNTIMENV_SIZE + EMMC_SECTOR_SIZE);
	if(nv_read_partition(p_block_dev, PARTITION_RUNTIME_NV2, (char *)DSP_ADR, RUNTIMENV_SIZE) == 0) {
		if (1 == runtimenv_is_correct((unsigned char *)DSP_ADR, RUNTIMENV_SIZE)) {
			backupfile_right = 1;//right
		}
	}

	if ((orginal_right == 1) && (backupfile_right == 0)) {
		printf("runtimenv is right, but runtimenvbkup is wrong, so recovery runtimenvbkup\n");
		nv_erase_partition(p_block_dev, PARTITION_RUNTIME_NV2);
		nv_write_partition(p_block_dev, PARTITION_RUNTIME_NV2, (char *)RUNTIMENV_ADR, RUNTIMENV_SIZE);
	} else if ((orginal_right == 0) && (backupfile_right == 1)) {
		printf("productinfobkup is right, but productinfo is wrong, so recovery productinfo\n");
		nv_erase_partition(p_block_dev, PARTITION_RUNTIME_NV1);
		nv_write_partition(p_block_dev, PARTITION_RUNTIME_NV1, (char *)DSP_ADR, RUNTIMENV_SIZE);
	} else if ((orginal_right == 0) && (backupfile_right == 0)) {
		printf("\n\nruntimenv and runtimenvbkup are all wrong or no runtimenv.\n\n");
	}

	printf("Reading runtimenv to 0x%08x\n", RUNTIMENV_ADR);
	/* runtimenv */
	memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + EMMC_SECTOR_SIZE);
	if(nv_read_partition(p_block_dev, PARTITION_RUNTIME_NV1, (char *)RUNTIMENV_ADR, RUNTIMENV_SIZE) == 0) {
		if (-1 == runtimenv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
			/* file isn't right and read backup file */
			memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + EMMC_SECTOR_SIZE);
			if(nv_read_partition(p_block_dev, PARTITION_RUNTIME_NV2, (char *)RUNTIMENV_ADR, RUNTIMENV_SIZE) == 0) {
				if (-1 == runtimenv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
					/* file isn't right */
					memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + EMMC_SECTOR_SIZE);
				}
			}
		}
	} else {
		/* file don't exist and read backup file */
		memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE + EMMC_SECTOR_SIZE);
		if(nv_read_partition(p_block_dev, PARTITION_RUNTIME_NV2, (char *)RUNTIMENV_ADR, RUNTIMENV_SIZE) == 0){
			if (-1 == runtimenv_is_correct((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE)) {
				/* file isn't right */
				memset((unsigned char *)RUNTIMENV_ADR, 0xff, RUNTIMENV_SIZE);
			}
		}
	}
	//array_value((unsigned char *)RUNTIMENV_ADR, RUNTIMENV_SIZE);

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

#if((!BOOT_NATIVE_LINUX)||(BOOT_NATIVE_LINUX_MODEM))
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

	creat_cmdline(cmdline,hdr);

#if BOOT_NATIVE_LINUX_MODEM
	//sipc addr clear
	sipc_addr_reset();
	// start modem CP
	modem_entry();
#endif

	vlx_entry();
}

