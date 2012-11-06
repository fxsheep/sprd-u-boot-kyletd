#include "normal_mode.h"

unsigned spl_data_buf[0x1000] __attribute__((align(4)))={0};
unsigned harsh_data_buf[8]__attribute__((align(4)))={0};
void *spl_data = spl_data_buf;
void *harsh_data = harsh_data_buf;
unsigned char raw_header[8192];
const int SP09_MAX_PHASE_BUFF_SIZE = sizeof(SP09_PHASE_CHECK_T);

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

void dump_all_buffer(unsigned char *buf, unsigned long len)
{
	unsigned long row, col;
	unsigned long offset;
	unsigned long total_row, remain_col;
	unsigned long flag = 1;

	total_row = len / 16;
	remain_col = len - total_row * 16;
    offset = 0;
	for (row = 0; row < total_row; row ++) {
		printf("%08xh: ", offset );
		for (col = 0; col < 16; col ++)
			printf("%02x ", buf[offset + col]);
		printf("\n");
        offset += 16;
	}

	if (remain_col > 0) {
		printf("%08xh: ", offset);
		for (col = 0; col < remain_col; col ++)
			printf("%02x ", buf[offset + col]);
		printf("\n");
	}

	printf("\n");
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
void lcd_display_logo(int backlight_set,ulong bmp_img,size_t size)
{
#ifdef CONFIG_SPLASH_SCREEN
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
}

void creat_cmdline(char * cmdline,boot_img_hdr *hdr)
{
	int str_len;
	char * buf;
	buf = malloc(1024);
	memset(buf, 0, 1024);

	sprintf(buf, "initrd=0x%x,0x%x", RAMDISK_ADR, hdr->ramdisk_size);
	addbuf(buf);

	/* preset loop_per_jiffy */
#ifdef CONFIG_LOOP_PER_JIFFY
	str_len = strlen(buf);
	sprintf(&buf[str_len], " lpj=%d", CONFIG_LOOP_PER_JIFFY);
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

	int ret=is_factorymode();

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

	addcmdline(buf);
	ret =read_spldata();
	if(ret != 0)
		return;
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
}
void vlx_entry()
{
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
