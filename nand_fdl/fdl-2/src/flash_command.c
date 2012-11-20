#include <asm/arch/sci_types.h>
#include "dload_op.h"
#include "flash_command.h"
#include "fdl_nand.h"
#include <asm/arch/packet.h>
#include "fdl_conf.h"
#include <asm/arch/fdl_crc.h>
#include <asm/arch/fdl_stdio.h>
#include "parsemtdparts.h"

#include "fdl_yaffs2.h"
#include "asm/arch/sci_types.h"
#include "asm/arch/nand_controller.h"
#include <linux/mtd/mtd.h>
#include <nand.h>
#include <linux/mtd/nand.h>
#include <jffs2/jffs2.h>
#include <malloc.h>
#include "fdl_yaffs2.h"

extern void cmd_yaffs_mount(char *mp);
extern void cmd_yaffs_umount(char *mp);
extern int cmd_yaffs_ls_chk(const char *dirfilename);
extern void cmd_yaffs_mread_file(char *fn, unsigned char *addr);
extern void cmd_yaffs_mwrite_file(char *fn, char *addr, int size);
extern int cmd_yaffs_rm_chk(const char *path);


#define TRANS_CODE_SIZE		(12 * 1024) /* dloadtools optimization value */
#define NAND_NOTUSED_ADDRESS	(0x9000000f)

#define PAGE_SIZE_2K 2048
#define NAND_OOB_64 64

typedef struct _DL_FILE_STATUS
{
    unsigned long   total_size;
    unsigned long   recv_size;
} DL_FILE_STATUS, *PDL_FILE_STATUS;

typedef struct
{
	unsigned long chunkId;
	unsigned long dataSize;
	unsigned long objectId;
	__u8 p_data[PAGE_SIZE_2K];
	__u8 p_spare[NAND_OOB_64];
} chunk_data;

char *Dl_Op_Type_Name[] = {
"StartData  ",
"Download   ",
"EndData    ",
"EraseFlash ",
"ReadFlash  "
};

char *Dl_Op_Status_Name[] = {
"Success ",
"Fail    "
};


static unsigned long g_checksum;
static unsigned long g_sram_addr;
static int read_nv_flag = 0;
static int read_bkupnv_flag = 0;
static int read_dlstatus_flag = 0;

#ifdef CONFIG_BOARD_788
static unsigned char buildprop_BUF[2048];
static unsigned long find_zte_version = 0;
static int zte_version_size = 0;
static unsigned long zte_version_point = 0;
static char  versionBUF[512];
static unsigned int read_ZTEversion_flag = 0;
static int download_system_flag = 0;
#endif

__align(4) unsigned char g_fixnv_buf[FIXNV_SIZE + 4];
__align(4) unsigned char g_fixnv_buf_yaffs[FIXNV_SIZE + 4];

#define CHECKSUM_OTHER_DATA       0x5555aaaa
static DL_FILE_STATUS g_status;
static int g_prevstatus;
static __inline void FDL2_SendRep (unsigned long err)
{
    FDL_SendAckPacket (convert_err (err));
}
unsigned long FDL2_GetRecvDataSize (void)
{
    return g_status.recv_size;
}

struct real_mtd_partition phy_partition;
struct real_mtd_partition phy_nv_partition;
static unsigned int is_nbl_write;
static unsigned int is_phasecheck_write;
static unsigned int file_in_productinfo_partition = 0; /* 0 is null; 1 is productinfo.bin; 2 is nv_ram */
static unsigned int read_productinfo_flag = 0, read_nvram_flag = 0,read_amt0_flag=0,read_amt2_flag = 0,read_fact_flag = 0;
static unsigned int g_NBLFixBufDataSize = 0;
static unsigned char g_FixNBLBuf[0x8000];
static unsigned int g_PhasecheckBUFDataSize = 0;
static unsigned char g_PhasecheckBUF[0x2000];
static unsigned char temBuf[8192 + 1024];
static DL_OP_RECORD_S g_dl_op_table[DL_OP_MTD_COUNT];
static NAND_PAGE_OOB_STATUS nand_page_oob_info = {0};
static unsigned long g_dl_op_index = 0;
static unsigned long is_factorydownload_tools = 0;
static unsigned long is_check_dlstatus = 0; /* 1 : check  0 : don't check */
static DL_Address_CNT_S Dl_Data_Address = {NULL, 0};
static DL_Address_CNT_S Dl_Erase_Address = {NULL, 0};

#ifdef  TRANS_CODE_SIZE
#define min(A,B)		(((A) < (B)) ? (A) : (B))
#define	DATA_BUFFER_SIZE	(TRANS_CODE_SIZE * 2)
static unsigned long  yaffs_buffer_size = 0;
static unsigned long g_BigSize = 0;
static unsigned long g_ReadBufLen = 0;
static unsigned long code_yaffs_buflen	= 0;
static unsigned long code_yaffs_onewrite = 0;
static unsigned char *g_BigBUF = NULL;
#endif

static int orginal_right, backupfile_right;
static unsigned long orginal_index, backupfile_index;
static unsigned char *orginal, *backupfile;

typedef struct _CUSTOM2LOG
{
    unsigned long   custom;
    unsigned long   log;
} CUSTOM2LOG;

bootimg_hdr g_BootimgHDR;
unsigned g_BootimgCurAddr = 0;
unsigned g_BootimgWritesize = 0;
unsigned long totalchunk = 0;
unsigned long chunknumber = 0;
chunk_data chunk, chunk_odd, chunk_even;
static unsigned long outpos = 0;
static unsigned long check_image_type = 0;

static CUSTOM2LOG custom2log_table[] = {
	{0x90000001, 0x80000005}, 
	{0x90000003, 0x80000008}, 
	{0x90000002, 0x80000011},
	{0x90000022, 0x80000011},
	{0x90000023, 0x80000011},
	{0x90000024, 0x80000011},
	{0x90000025, 0x80000011},
	{0x90000026, 0x80000011},
	{0xffffffff, 0xffffffff}
};
#define ECC_NBL_SIZE 0x4000
//bootloader header flag offset from the beginning
#define BOOTLOADER_HEADER_OFFSET   32
#define NAND_PAGE_LEN              512
#define NAND_MAGIC_DATA            0xaa55a5a5
//define nand data bus len
#define NAND_BUS_SIZE_8              8
#define NAND_BUS_SIZE_16              16
#define NAND_BUS_SIZE_32              32

void set_dl_op_val(unsigned long addr, unsigned long size, DL_OP_TYPE_E type, DL_OP_STATUS_E status, unsigned long cnt)
{
	if (g_dl_op_index >= DL_OP_MTD_COUNT) {
		printf("\nmtd count is beyoned %d\n", DL_OP_MTD_COUNT);
		return;
	}
    
	switch (type) {
		case STARTDATA:
			if (status == FAIL) {
				memset(&(g_dl_op_table[g_dl_op_index]), 0, sizeof(DL_OP_RECORD_S));
				g_dl_op_table[g_dl_op_index].base = addr;
    				g_dl_op_table[g_dl_op_index].size = size;
    				g_dl_op_table[g_dl_op_index].type = type;
    				g_dl_op_table[g_dl_op_index].status = status;
    				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
			} else {
    				g_dl_op_table[g_dl_op_index].status = status;
    				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
				g_dl_op_index++;
				memset(&(g_dl_op_table[g_dl_op_index]), 0, sizeof(DL_OP_RECORD_S));
				g_dl_op_table[g_dl_op_index].base = addr;
    				g_dl_op_table[g_dl_op_index].size = size;
			}
		break;
		case MIDSTDATA:
			g_dl_op_table[g_dl_op_index].type = type;
			g_dl_op_table[g_dl_op_index].status = status;
			if (status == FAIL)
    				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
			else
				g_dl_op_table[g_dl_op_index].status_cnt++;
		break;
		case ENDDATA:
			if ((status == FAIL) && (cnt == 1)) {
				g_dl_op_index++;
				memset(&(g_dl_op_table[g_dl_op_index]), 0, sizeof(DL_OP_RECORD_S));
				g_dl_op_table[g_dl_op_index].base = g_dl_op_table[g_dl_op_index - 1].base;
				g_dl_op_table[g_dl_op_index].size = g_dl_op_table[g_dl_op_index - 1].size;
				g_dl_op_table[g_dl_op_index].type = type;
				g_dl_op_table[g_dl_op_index].status = status;
				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
			} else {
				g_dl_op_table[g_dl_op_index].status = status;
				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
				if (status == SUCCESS) {
					g_dl_op_index++;
					memset(&(g_dl_op_table[g_dl_op_index]), 0, sizeof(DL_OP_RECORD_S));
				}
			}
		break;
		case ERASEFLASH:
			if (status == FAIL) {
				memset(&(g_dl_op_table[g_dl_op_index]), 0, sizeof(DL_OP_RECORD_S));
				g_dl_op_table[g_dl_op_index].base = addr;
				g_dl_op_table[g_dl_op_index].size = size;
				g_dl_op_table[g_dl_op_index].type = type;
				g_dl_op_table[g_dl_op_index].status = status;
				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
			} else {
				g_dl_op_table[g_dl_op_index].status = status;
				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
				g_dl_op_index++;
				memset(&(g_dl_op_table[g_dl_op_index]), 0, sizeof(DL_OP_RECORD_S));
			}
		break;
		case READFLASH:
		break;
	}	
}

DL_OP_STATUS_E check_dl_data_status(unsigned long addr)
{
	int cnt;
	DL_OP_STATUS_E status;
	
	status = FAIL;
	for (cnt = 0; cnt < g_dl_op_index; cnt ++)
		if ((g_dl_op_table[cnt].base == addr) && (g_dl_op_table[cnt].type == STARTDATA) && (g_dl_op_table[cnt].status == SUCCESS) && (g_dl_op_table[cnt].status_cnt == 1)) {
			if ((cnt + 2) < g_dl_op_index) {
				if ((g_dl_op_table[cnt + 1].base == addr) && (g_dl_op_table[cnt + 1].type == MIDSTDATA) && (g_dl_op_table[cnt + 1].status == SUCCESS) && (g_dl_op_table[cnt + 1].status_cnt >= 1)) {
					if ((g_dl_op_table[cnt + 2].base == addr) && (g_dl_op_table[cnt + 2].type == ENDDATA) && (g_dl_op_table[cnt + 2].status == SUCCESS) && (g_dl_op_table[cnt + 2].status_cnt == 1)) {
						status = SUCCESS;
						break;
					}
				}
			}
		}

	return status;
}

DL_OP_STATUS_E check_dl_erase_status(unsigned long addr)
{
	int cnt;
	DL_OP_STATUS_E status;
	
	status = FAIL;
	for (cnt = 0; cnt < g_dl_op_index; cnt ++)
		if ((g_dl_op_table[cnt].base == addr) && (g_dl_op_table[cnt].type == ERASEFLASH) && (g_dl_op_table[cnt].status == SUCCESS) && (g_dl_op_table[cnt].status_cnt == 1)) {
			status = SUCCESS;
			break;
		}

	return status;
}

unsigned long custom2log(unsigned long custom)
{
	unsigned long idx, log = 0xffffffff;

	file_in_productinfo_partition = 0;

	if (custom == NAND_NOTUSED_ADDRESS)
		return custom;

	if ((custom & 0xf0000000) == 0x80000000)
		return custom;

	for (idx = 0; custom2log_table[idx].custom != 0xffffffff; idx ++) {
		if (custom2log_table[idx].custom == custom) {
			if ((custom == 0x90000002) || ((custom >= 0x90000022) && (custom <= 0x90000026 )))
				file_in_productinfo_partition = custom;

			log = custom2log_table[idx].log;
			break;
		}
	}
	
	return log;
}

void phy_partition_info(struct real_mtd_partition phy, int line)
{
	int i;

	if (phy.offset == 0xffffffff) {
		printf("\n\nInvaild partition address\n\n");
		return;
	}

	printf("line : %d, name : %20s, offset : 0x%08x, size : 0x%08x, yaffs : %d\n", line, phy.name, phy.offset, phy.size, phy.yaffs);

	return;
}

static unsigned short calc_checksum(unsigned char *dat, unsigned long len)
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

int nv_is_correct_endflag(unsigned char *array, unsigned long size)
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

static unsigned long get_nv_index(unsigned char *array, unsigned long size)
{
	unsigned long index = 0;
	unsigned short sum = 0, *dataaddr;

	if ((array[size - 4] == 0xff) && (array[size - 3] == 0xff) && (array[size - 2] == 0xff) \
		&& (array[size - 1] == 0xff)) {
		/* old version */
		index = 1;
	} else {
		/* new version */
		dataaddr = (unsigned short *)(array + size - 2);
		index = (unsigned long)(*dataaddr);
	}
	return index;
}

/*
* retval : -1 is wrong  ;  1 is correct
*/
static int nv_is_correct(unsigned char *array, unsigned long size)
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
				array[size + 0] = 0xff; array[size + 1] = 0xff;
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

/* phasecheck : 0 --- 3071; crc : 3072 3073; index : 3074 3075
/*
* retval : -1 is wrong  ;  1 is correct
*/
int sn_is_correct(unsigned char *array, unsigned long size)
{
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
	unsigned short sum = 0, *dataaddr;

	if (size == PRODUCTINFO_SIZE) {
		sum = calc_checksum(array, size);
		dataaddr = (unsigned short *)(array + size);
		if (*dataaddr == sum)
			return 1;
	}

	printf("phasecheck flage error\n");
	return -1;
}

/*
   1 ; success
   2 : error
*/
int nand_read_fdl_yaffs(struct real_mtd_partition *phypart, unsigned int off, unsigned int size, unsigned char *buf)
{
	int ret = 0;
	int pos;
	unsigned long addr = phypart->offset;

	if (strcmp(phypart->name, "fixnv") == 0) {
		/* for fixnv, read total 64KB */
		char *fixnvpoint = "/fixnv";
		char *fixnvfilename = "/fixnv/fixnv.bin";
		char *backupfixnvpoint = "/backupfixnv";
		char *backupfixnvfilename = "/backupfixnv/fixnv.bin";

		if ((read_nv_flag == 0) && (read_bkupnv_flag == 0)) {
			read_nv_flag = 1;//wrong
			memset(g_fixnv_buf_yaffs, 0xff, FIXNV_SIZE + 4);
			/* read fixnv */
    			cmd_yaffs_mount(fixnvpoint);
			ret = cmd_yaffs_ls_chk(fixnvfilename);
			if (ret == (FIXNV_SIZE + 4)) {
				cmd_yaffs_mread_file(fixnvfilename, g_fixnv_buf_yaffs);
				if (1 == nv_is_correct_endflag(g_fixnv_buf_yaffs, FIXNV_SIZE))
					read_nv_flag = 2;//right
			}
			cmd_yaffs_umount(fixnvpoint);

			read_bkupnv_flag = 1;//wrong
			memset(g_fixnv_buf, 0xff, FIXNV_SIZE + 4);
			cmd_yaffs_mount(backupfixnvpoint);
			ret = cmd_yaffs_ls_chk(backupfixnvfilename);
			if (ret == (FIXNV_SIZE + 4)) {
				cmd_yaffs_mread_file(backupfixnvfilename, g_fixnv_buf);
				if (1 == nv_is_correct_endflag(g_fixnv_buf, FIXNV_SIZE))
					read_bkupnv_flag = 2;//right
			}
			cmd_yaffs_umount(backupfixnvpoint);

			if ((read_nv_flag == 2) && (read_bkupnv_flag == 2)) {
				/* check index */
				orginal_index = get_nv_index((unsigned char *)g_fixnv_buf_yaffs, FIXNV_SIZE);
				backupfile_index = get_nv_index((unsigned char *)g_fixnv_buf, FIXNV_SIZE);
				if (orginal_index != backupfile_index) {
					read_nv_flag = 2;
					read_bkupnv_flag = 1;
				}
			}

			if ((read_nv_flag == 2) && (read_bkupnv_flag == 1)) {
				printf("fixnv is right, but backupfixnv is wrong, so erase and recovery backupfixnv\n");
				memset(&phy_nv_partition, 0, sizeof(struct real_mtd_partition));
				strcpy(phy_nv_partition.name, "backupfixnv");
				log2phy_table(&phy_nv_partition);
				phy_partition_info(phy_nv_partition, __LINE__);
				printf("erase backupfixnv start\n");
				nand_start_write(&phy_nv_partition, 0, &nand_page_oob_info, 0);
				printf("\nerase backupfixnv end\n");
				printf("write backupfixnv start\n");
				cmd_yaffs_mount(backupfixnvpoint);
    				cmd_yaffs_mwrite_file(backupfixnvfilename, (char *)g_fixnv_buf_yaffs, (FIXNV_SIZE + 4));
				cmd_yaffs_ls_chk(backupfixnvfilename);
				cmd_yaffs_umount(backupfixnvpoint);
				printf("write backupfixnv end\n");
			} else if ((read_nv_flag == 1) && (read_bkupnv_flag == 2)) {
				printf("backupfixnv is right, but fixnv is wrong, so erase and recovery fixnv\n");
				memset(&phy_nv_partition, 0, sizeof(struct real_mtd_partition));
				strcpy(phy_nv_partition.name, "fixnv");
				log2phy_table(&phy_nv_partition);
				phy_partition_info(phy_nv_partition, __LINE__);
				/* erase fixnv partition */
				printf("erase fixnv start\n");
				nand_start_write(&phy_nv_partition, 0, &nand_page_oob_info, 0);
				printf("\nerase fixnv end\n");
				printf("write fixnv start\n");
				cmd_yaffs_mount(fixnvpoint);
    				cmd_yaffs_mwrite_file(fixnvfilename, (char *)g_fixnv_buf, (FIXNV_SIZE + 4));
				cmd_yaffs_ls_chk(fixnvfilename);
				cmd_yaffs_umount(fixnvpoint);
				printf("write fixnv end\n");
				memcpy((unsigned char *)g_fixnv_buf_yaffs, (unsigned char *)g_fixnv_buf, (FIXNV_SIZE + 4));
			} else if ((read_nv_flag == 1) && (read_bkupnv_flag == 1)) {
				printf("\n\nfixnv and backupfixnv are all wrong.\n\n");
				memset(g_fixnv_buf_yaffs, 0xff, FIXNV_SIZE + 4);			
			} else if ((read_nv_flag == 2) && (read_bkupnv_flag == 2))
				printf("fixnv and backupfixnv are all right.\n");

			nv_is_correct(g_fixnv_buf_yaffs, FIXNV_SIZE);
			memset(g_fixnv_buf, 0xff, FIXNV_SIZE + 4);
		} //if ((read_nv_flag == 0) && (read_bkupnv_flag == 0))

		memcpy(buf, (unsigned char *)(g_fixnv_buf_yaffs + off), size);

		if ((read_nv_flag == 2) || (read_bkupnv_flag == 2))
			return NAND_SUCCESS;
		return NAND_SYSTEM_ERROR;
	}//if (strcmp(phypart->name, "fixnv") == 0)

	if (strcmp(phypart->name, "productinfo") == 0) {
		/* for dlstatus, read real length */
		char *productinfopoint = "/productinfo";
		/* char *productinfofilename = "/productinfo/dlstatus.txt"; */
		char *productinfofilename = "/productinfo/productinfo.bin";
		char *bkproductinfofilename = "/productinfo/productinfobkup.bin";
		char *nvramfilename = "/productinfo/nvram";
		char *amt0filename = "/productinfo/amt0";
		char *amt2filename = "/productinfo/amt2";
		char *factfilename = "/productinfo/fact";
#ifdef CONFIG_BOARD_788
		char *zteversionfilename = "/productinfo/ZTEversion";
#endif
		char convbuf[2] = {0x5a, 0x5a};

		if (file_in_productinfo_partition == 0x90000002) {
			if (read_productinfo_flag == 0) {
				memset(g_PhasecheckBUF, 0, 0x2000);
				orginal = g_PhasecheckBUF;
				backupfile = g_PhasecheckBUF + 0x1000;
				/* read dlstatus */
				cmd_yaffs_mount(productinfopoint);
				ret = cmd_yaffs_ls_chk(productinfofilename);
				if (ret == (PRODUCTINFO_SIZE + 4)) {
					cmd_yaffs_mread_file(productinfofilename, (unsigned char *)orginal);
					if (1 == sn_is_correct_endflag((unsigned char *)orginal, PRODUCTINFO_SIZE))
						orginal_right = 1;//right
				}

				ret = cmd_yaffs_ls_chk(bkproductinfofilename);
				if (ret == (PRODUCTINFO_SIZE + 4)) {
					cmd_yaffs_mread_file(bkproductinfofilename, (unsigned char *)backupfile);
					if (1 == sn_is_correct_endflag((unsigned char *)backupfile, PRODUCTINFO_SIZE))
						backupfile_right = 1;//right
				}

				cmd_yaffs_umount(productinfopoint);
				if ((orginal_right == 1) && (backupfile_right == 1)) {
					/* check index */
					orginal_index = get_productinfo_index((unsigned char *)orginal);
					backupfile_index = get_productinfo_index((unsigned char *)backupfile);
					if (orginal_index != backupfile_index) {
						orginal_right = 1;
						backupfile_right = 0;
					}
				}
				read_productinfo_flag = 1;//success
				if ((orginal_right == 1) && (backupfile_right == 1)) {
					sn_is_correct((unsigned char *)orginal, PRODUCTINFO_SIZE);
				} else if ((orginal_right == 1) && (backupfile_right == 0)) {
					printf("productinfo is right, but productinfobkup is wrong, so use productinfobkup\n");
					sn_is_correct((unsigned char *)orginal, PRODUCTINFO_SIZE);
				} else if ((orginal_right == 0) && (backupfile_right == 1)) {
					printf("productinfobkup is right, but productinfo is wrong, so use productinfo\n");
					sn_is_correct((unsigned char *)backupfile, PRODUCTINFO_SIZE);
					memcpy(orginal, backupfile, PRODUCTINFO_SIZE + 4);
				} else if ((orginal_right == 0) && (backupfile_right == 0)) {
					printf("\n\nproductinfo and productinfobkup are all wrong or no phasecheck.\n\n");
					memset(g_PhasecheckBUF, 0, 0x2000);
					read_productinfo_flag = 0;
				}
			}
			memcpy(buf, (unsigned char *)(g_PhasecheckBUF + off), size);

			if (read_productinfo_flag == 1)
				return NAND_SUCCESS;
		} else if (file_in_productinfo_partition == 0x90000022) {
			if (read_nvram_flag == 0) {
				memset(g_PhasecheckBUF, 0, 0x2000);
				/* read dlstatus */
    				cmd_yaffs_mount(productinfopoint);
				ret = cmd_yaffs_ls_chk(nvramfilename);
				if (ret > 0) {
					cmd_yaffs_mread_file(nvramfilename, g_PhasecheckBUF);
					read_nvram_flag = 1;
				}
				cmd_yaffs_umount(productinfopoint);
			}

			memcpy(buf, (unsigned char *)(g_PhasecheckBUF + off), size);

			if (read_nvram_flag == 1)
				return NAND_SUCCESS;
		}else if (file_in_productinfo_partition == 0x90000023) {
			if (read_amt0_flag == 0) {
				memset(g_PhasecheckBUF, 0, 0x2000);
				/* read dlstatus */
    				cmd_yaffs_mount(productinfopoint);
				ret = cmd_yaffs_ls_chk(amt0filename);
				if (ret == 0) {
					cmd_yaffs_mwrite_file(amt0filename, convbuf, 1);
					ret = 1;
				}
				if (ret > 0) {
					cmd_yaffs_mread_file(amt0filename, g_PhasecheckBUF);
					read_amt0_flag = 1;
				}
				cmd_yaffs_umount(productinfopoint);
			}

			memcpy(buf, (unsigned char *)(g_PhasecheckBUF + off), size);

			if (read_amt0_flag == 1)
				return NAND_SUCCESS;
		}else if (file_in_productinfo_partition == 0x90000024) {
			if (read_amt2_flag == 0) {
				memset(g_PhasecheckBUF, 0, 0x2000);
				/* read dlstatus */
    				cmd_yaffs_mount(productinfopoint);
				ret = cmd_yaffs_ls_chk(amt2filename);
				if (ret == 0) {
					cmd_yaffs_mwrite_file(amt2filename, convbuf, 1);
					ret = 1;
				}
				if (ret > 0) {
					cmd_yaffs_mread_file(amt2filename, g_PhasecheckBUF);
					read_amt2_flag = 1;
				}
				cmd_yaffs_umount(productinfopoint);
			}

			memcpy(buf, (unsigned char *)(g_PhasecheckBUF + off), size);

			if (read_amt2_flag == 1)
				return NAND_SUCCESS;
		}else if (file_in_productinfo_partition == 0x90000025) {
			if (read_fact_flag == 0) {
				memset(g_PhasecheckBUF, 0, 0x2000);
				/* read dlstatus */
    				cmd_yaffs_mount(productinfopoint);
				ret = cmd_yaffs_ls_chk(factfilename);
				if (ret == 0) {
					cmd_yaffs_mwrite_file(factfilename, convbuf, 1);
					ret = 1;
				}
				if (ret > 0) {
					cmd_yaffs_mread_file(factfilename, g_PhasecheckBUF);
					read_fact_flag = 1;
				}
				cmd_yaffs_umount(productinfopoint);
			}

			memcpy(buf, (unsigned char *)(g_PhasecheckBUF + off), size);

			if (read_fact_flag == 1)
				return NAND_SUCCESS;
		}
#ifdef CONFIG_BOARD_788
		else if (file_in_productinfo_partition == 0x90000026) {
			if (read_ZTEversion_flag == 0) {
				memset(g_PhasecheckBUF, 0, 0x2000);
				/* read dlstatus */
				cmd_yaffs_mount(productinfopoint);
				ret = cmd_yaffs_ls_chk(zteversionfilename);

				if (ret > 0) {
					cmd_yaffs_mread_file(zteversionfilename, g_PhasecheckBUF);
					read_ZTEversion_flag = 1;
				}
				cmd_yaffs_umount(productinfopoint);
			}

			memcpy(buf, (unsigned char *)(g_PhasecheckBUF + off), size);

			if (read_ZTEversion_flag == 1)
				return NAND_SUCCESS;
		}
#endif
	
		return NAND_SYSTEM_ERROR;
	}//if (strcmp(phypart->name, "dlstatus") == 0)
       return NAND_SYSTEM_ERROR;
}

#ifdef CONFIG_BOARD_788
static char * g_zte_version(char *buf)
{
		char *product_release = "apps.setting.product.release";
		char *internal_version = "ro.build.sw_internal_version";
		char *product_release_pointer = NULL;
		char *internal_version_pointer = NULL;
		int   product_release_flag = 0;
		int   internal_version_flag = 0;

		memset(versionBUF,0x0,512);
		product_release_pointer = strstr(buf,product_release);
		internal_version_pointer = strstr(buf,internal_version);
		if ((product_release_pointer == NULL) || (internal_version_pointer == NULL)) {
			printf("zte version not found\n");
			return NULL;
		}

		while ((*(product_release_pointer + product_release_flag)) != 0x0a) {
			versionBUF[product_release_flag] = *(product_release_pointer + product_release_flag);
			product_release_flag++;
		}
		versionBUF[product_release_flag++] = ';';

		while((*(internal_version_pointer + internal_version_flag)) != 0x0a){
			versionBUF[product_release_flag+internal_version_flag] = *(internal_version_pointer + internal_version_flag);
			internal_version_flag++;
		}

		return versionBUF;
}
#endif

int bootimgWriteBuf(unsigned char *buf)
{
	int ret = nand_write_fdl(nand_page_oob_info.writesize, buf);

	return ret;
 }

void dump_oh(yaffs_ObjectHeader *ob)
{
	printf("object head : type = %d\n", ob->type);
	printf("fileSize = %d\n", ob->fileSize);
}

void dump_all_buffer(unsigned long pos, unsigned char *buf, unsigned long len)
{
	unsigned long row, col;
	unsigned long offset;
	unsigned long total_row, remain_col;
	unsigned long flag = 1;

	total_row = len / 16;
	remain_col = len - total_row * 16;
    offset = 0;
	for (row = 0; row < total_row; row ++) {
		printf("%08xh: ", pos + offset );
		for (col = 0; col < 16; col ++)
			printf("%02x ", buf[offset + col]);
		printf("\n");
        offset += 16;
	}

	if (remain_col > 0) {
		printf("%08xh: ", pos + offset);
		for (col = 0; col < remain_col; col ++)
			printf("%02x ", buf[offset + col]);
		printf("\n");
	}

	printf("\n");
}

int bootimg_convertAndWrite(void)
{
	int ret = NAND_SUCCESS;
	int remainBufSize = g_BigSize - g_ReadBufLen;
	int remainFileSize = 0;
	int writesize = 0;
	int pagesize = nand_page_oob_info.writesize;
	unsigned nextFileAddr = 0;
	unsigned filesize = 0;
	unsigned long remaindata = 0;

	while ((g_BigSize - g_ReadBufLen) >= 2048) {
		remainBufSize = g_BigSize - g_ReadBufLen;

		if (g_BootimgCurAddr == 0) {
			memset(temBuf, 0, pagesize);
			memset(&g_BootimgHDR, 0, sizeof(bootimg_hdr));
			memcpy(&g_BootimgHDR, g_BigBUF, sizeof(bootimg_hdr));
			if ((g_BootimgHDR.magic[0] != 'A') || (g_BootimgHDR.magic[1] != 'N') || (g_BootimgHDR.magic[2] != 'D') || (g_BootimgHDR.page_size != 2048)) {
				printf("mkbootimage is not default parameter, need pagesize 2KB!\n");
				g_prevstatus = NAND_INVALID_SIZE;
				FDL2_SendRep(g_prevstatus);
				return 0;
			}

			g_BootimgHDR.page_size = pagesize;
			memcpy(temBuf, &g_BootimgHDR, sizeof(bootimg_hdr));
			ret = bootimgWriteBuf(temBuf);
			if (ret != NAND_SUCCESS) {
				printf("hdr error\n");
				goto fail;
			}

			g_ReadBufLen += 2048;
			g_BootimgCurAddr = g_BootimgHDR.kernel_addr;
			chunknumber = 0;
			continue;
		} else if (g_BootimgCurAddr == g_BootimgHDR.kernel_addr) {
			filesize = g_BootimgHDR.kernel_size;
			remainFileSize = g_BootimgHDR.kernel_size - g_BootimgWritesize;
			nextFileAddr = g_BootimgHDR.ramdisk_addr;
			totalchunk = (filesize + 2048 - 1) / 2048;
			chunknumber ++;
		}else if (g_BootimgCurAddr == g_BootimgHDR.ramdisk_addr) {
			filesize = g_BootimgHDR.ramdisk_size;
			remainFileSize = g_BootimgHDR.ramdisk_size- g_BootimgWritesize;
			nextFileAddr = g_BootimgHDR.second_addr;
			totalchunk = (filesize + 2048 - 1) / 2048;
			chunknumber ++;
		} else if (g_BootimgCurAddr == g_BootimgHDR.second_addr) {
			filesize = g_BootimgHDR.second_size;
			remainFileSize = g_BootimgHDR.second_size- g_BootimgWritesize;
			nextFileAddr = -1;
		}

		writesize = pagesize;
		if (chunknumber % 2) {
			memset(temBuf, 0, pagesize);
			memcpy(temBuf, g_BigBUF + g_ReadBufLen, 2048);
			g_BootimgWritesize += 2048;
			g_ReadBufLen += 2048;
			if (chunknumber == totalchunk) {
				ret = bootimgWriteBuf(temBuf);
				if (ret != NAND_SUCCESS)
					goto fail;
			}
		} else {
			memcpy(temBuf + 2048, g_BigBUF + g_ReadBufLen, 2048);
			ret = bootimgWriteBuf(temBuf);
			if (ret != NAND_SUCCESS)
				goto fail;

			g_BootimgWritesize += 2048;
			g_ReadBufLen += 2048;
		}

		if (chunknumber == totalchunk) {
			g_BootimgWritesize = 0;
			g_BootimgCurAddr = nextFileAddr;
			chunknumber = 0;
			totalchunk = 0;
		}
	}

	remaindata = g_BigSize - g_ReadBufLen;
	memcpy(g_BigBUF, g_BigBUF + g_ReadBufLen, remaindata);
	memset(g_BigBUF + remaindata, 0xff, yaffs_buffer_size - remaindata);
	g_BigSize = remaindata;
	g_ReadBufLen = 0;

	return NAND_SUCCESS;

fail:
	return ret;
 }

int yaffs2_is4kImg(void)
{
	return (((!strcmp(phy_partition.name, "system")) || (!strcmp(phy_partition.name, "userdata"))) && (nand_page_oob_info.writesize == 4096));
}

int mkboot_is4kImg(void)
{
	return (((!strcmp(phy_partition.name, "boot")) || (!strcmp(phy_partition.name, "recovery"))) && (nand_page_oob_info.writesize == 4096));
}

static int yaffs2_nand_write_chunk(__u8 *data, __u32 objId, __u32 chunkId, __u32 nBytes)
{
	unsigned char spare[nand_page_oob_info.oobsize];
	yaffs_ExtendedTags t;
    yaffs_PackedTags2 *pt;
    memset(spare, 0xff, sizeof(spare));
    pt = (yaffs_PackedTags2 *)spare;
    yaffs2_InitialiseTags(&t);
    t.chunkId = chunkId;
    t.serialNumber = 1;
    t.byteCount = nBytes;
	t.objectId = objId;
	t.sequenceNumber = YAFFS_LOWEST_SEQUENCE_NUMBER;
	t.chunkUsed = 1;

	yaffs2_PackTags2(pt, &t);

	memcpy(temBuf + nand_page_oob_info.writesize, spare, nand_page_oob_info.oobsize);
	//dump_all_buffer(outpos, temBuf, nand_page_oob_info.writesize + nand_page_oob_info.oobsize);
	outpos += (nand_page_oob_info.writesize + nand_page_oob_info.oobsize);
    return nand_write_fdl(nand_page_oob_info.writesize + nand_page_oob_info.oobsize, temBuf);
}

/* convert 2k yaffs2 image object header to 4k header*/
static int yaffs2_convert_header(chunk_data *chunk)
{
	memset(temBuf, 0xff, nand_page_oob_info.writesize + nand_page_oob_info.oobsize);
	memcpy(temBuf, chunk->p_data, PAGE_SIZE_2K);

	return yaffs2_nand_write_chunk(temBuf, chunk->objectId, 0, 0xffff);
}

/* convert 2k yaffs2 image chunk to 4k chunk*/
static int yaffs2_convert_chunk(chunk_data *chunk1, chunk_data *chunk2)
{
	unsigned long nbBytes, chunkId;

	memset(temBuf, 0xff, nand_page_oob_info.writesize + nand_page_oob_info.oobsize);
	memcpy(temBuf, chunk1->p_data, chunk1->dataSize);

	nbBytes = chunk1->dataSize;
	chunkId = chunk1->chunkId / 2 + 1;

	if (chunk2) {
		memcpy(temBuf + nbBytes, chunk2->p_data, chunk2->dataSize);
		nbBytes += chunk2->dataSize;
	}
    //printf("4kchunk, objId=%d, chunkId=%d, dataSize=%d\n", chunk1->objectId, chunkId, nbBytes);
	return yaffs2_nand_write_chunk(temBuf, chunk1->objectId, chunkId, nbBytes);
}

int yaffs2_convertAndWrite(void)
{
	unsigned long ret = NAND_SUCCESS;
	yaffs_PackedTags2 *pt;
	yaffs_ObjectHeader *oh;
	yaffs_PackedTags2 checkpt;
	yaffs_ObjectHeader checkoh;

	while ((g_BigSize - g_ReadBufLen) >= (2048 + 64)) {
		if (check_image_type == 0) {
			check_image_type = 1;
			memset(&checkoh, 0xff, sizeof(yaffs_ObjectHeader));
			memset(&checkpt, 0xff, sizeof(yaffs_PackedTags2));

			memcpy(&checkoh, g_BigBUF, sizeof(yaffs_ObjectHeader));
			memcpy(&checkpt, g_BigBUF + 2048, sizeof(yaffs_PackedTags2));
			if ((checkoh.type != YAFFS_OBJECT_TYPE_DIRECTORY) || (checkpt.t.objectId != 1) || (checkpt.t.chunkId != 0)) {
				printf("yaffs image is not default parameter, need pagesize 2KB!\n");
				g_prevstatus = NAND_INVALID_SIZE;
				FDL2_SendRep(g_prevstatus);
				return 0;
			}
		}

		memset(&chunk, 0xff, sizeof(chunk_data));

		memcpy(&(chunk.p_data), g_BigBUF + g_ReadBufLen, PAGE_SIZE_2K);
		g_ReadBufLen += PAGE_SIZE_2K;

		memcpy(&(chunk.p_spare), g_BigBUF + g_ReadBufLen, NAND_OOB_64);
		g_ReadBufLen += NAND_OOB_64;

		pt = (yaffs_PackedTags2 *)&(chunk.p_spare);
		chunk.dataSize = pt->t.byteCount;
		chunk.chunkId = pt->t.chunkId;
		chunk.objectId = pt->t.objectId;

		if (chunk.chunkId == 0) {
			/* header */
			ret = yaffs2_convert_header(&chunk);
			if (ret != NAND_SUCCESS)
				goto fail;

			oh = (yaffs_ObjectHeader *)&(chunk.p_data);
			if (oh->type == YAFFS_OBJECT_TYPE_FILE) {
				totalchunk = (oh->fileSize + 2048 - 1) / 2048;
				chunknumber = 0;
				memset(&chunk_odd, 0xff, sizeof(chunk_data));
				memset(&chunk_even, 0xff, sizeof(chunk_data));
			}

			memset(&chunk, 0xff, sizeof(chunk_data));
		} else {
			/* data */
			chunknumber ++;
			if (chunk.chunkId % 2) {
				/* odd */
				memcpy(&chunk_odd, &chunk, sizeof(chunk_data));
				if (chunk.chunkId == totalchunk) {
					/* last odd chunk */
					ret = yaffs2_convert_chunk(&chunk_odd, NULL);
					if (ret != NAND_SUCCESS)
						goto fail;

					memset(&chunk_odd, 0xff, sizeof(chunk_data));
				}
			} else {
				/* even */
				memcpy(&chunk_even, &chunk, sizeof(chunk_data));
				ret = yaffs2_convert_chunk(&chunk_odd, &chunk_even);
				if (ret != NAND_SUCCESS)
					goto fail;

				memset(&chunk_odd, 0xff, sizeof(chunk_data));
				memset(&chunk_even, 0xff, sizeof(chunk_data));
			}

			if (chunk.chunkId == totalchunk) {
				chunknumber = 0;
				totalchunk = 0;
			}
		}
	}

	g_BigSize -= g_ReadBufLen;
	memcpy(g_BigBUF, g_BigBUF + g_ReadBufLen, g_BigSize);
	memset(g_BigBUF + g_BigSize, 0xff, yaffs_buffer_size - g_BigSize);
	g_ReadBufLen = 0;

	return NAND_SUCCESS;

fail:
	return ret;
}

int FDL2_DataStart (PACKET_T *packet, void *arg)
{
    unsigned long *data = (unsigned long *) (packet->packet_body.content);
    unsigned long start_addr = *data;
    unsigned long size = * (data + 1);
    int           ret;
#if defined(CHIP_ENDIAN_LITTLE)
    start_addr = EndianConv_32 (start_addr);
    size = EndianConv_32 (size);
#endif

    set_dl_op_val(start_addr, size, STARTDATA, FAIL, 1);
    if (packet->packet_body.size == 12)
    {
	memset(g_fixnv_buf, 0xff, FIXNV_SIZE + 4);
        g_checksum = * (data+2);
        g_sram_addr = (unsigned long) g_fixnv_buf;
    } else {
	        g_checksum = CHECKSUM_OTHER_DATA;
    }
    if (0 == (g_checksum & 0xffffff))
    {
        //The fixnv checksum is error.
        SEND_ERROR_RSP (BSL_EEROR_CHECKSUM); /*lint !e527*/
    }

    do
    {
	memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
	phy_partition.offset = custom2log(start_addr);
	ret = log2phy_table(&phy_partition);
	phy_partition_info(phy_partition, __LINE__);

	if (NAND_SUCCESS != ret)
        	break;

	if (size >= phy_partition.size) {
		printf("\n\nimage file size : 0x%08x is bigger than partition size : 0x%08x\n", size, phy_partition.size);
		ret = NAND_INVALID_SIZE;
	}
	if (NAND_SUCCESS != ret)
        	break;

#ifdef CONFIG_BOARD_788
	if (strcmp(phy_partition.name, "system") == 0)
		download_system_flag = 1;
	else
		download_system_flag = 0;
#endif

	if (strcmp(phy_partition.name, "fixnv") == 0)
		ret = get_nand_pageoob(&nand_page_oob_info);
	else
        	ret = nand_start_write(&phy_partition, size, &nand_page_oob_info, file_in_productinfo_partition);


        if (NAND_SUCCESS != ret)
            break;

	is_nbl_write = 0;
	is_phasecheck_write = 0;

	if (strcmp(phy_partition.name, "spl") == 0) {
		is_nbl_write = 1;
		g_NBLFixBufDataSize = 0;
	} else if (strcmp(phy_partition.name, "productinfo") == 0) {
		is_phasecheck_write = 1;
		g_PhasecheckBUFDataSize = 0;
		memset(g_PhasecheckBUF, 0xff, 0x2000);
	}
#ifdef TRANS_CODE_SIZE
	yaffs_buffer_size = (DATA_BUFFER_SIZE + (DATA_BUFFER_SIZE / nand_page_oob_info.writesize) * nand_page_oob_info.oobsize);

	if (phy_partition.yaffs == 0) {
		code_yaffs_buflen = DATA_BUFFER_SIZE;
		code_yaffs_onewrite = nand_page_oob_info.writesize;
	} else if (phy_partition.yaffs == 1) {
		code_yaffs_buflen = yaffs_buffer_size;
		code_yaffs_onewrite = nand_page_oob_info.writesize + nand_page_oob_info.oobsize;
	}
	
	g_BigSize = 0;
	if (g_BigBUF == NULL)
		g_BigBUF = (unsigned char *)malloc(yaffs_buffer_size);

	if (g_BigBUF == NULL) {
		printf("malloc is wrong : %d\n", yaffs_buffer_size);
		ret = NAND_SYSTEM_ERROR;		
		break;
	}
	memset(g_BigBUF, 0xff, yaffs_buffer_size);
	//printf("code_yaffs_onewrite = %d  code_yaffs_buflen = %d  yaffs_buffer_size = %d\n", code_yaffs_onewrite, code_yaffs_buflen, yaffs_buffer_size);
#endif

        g_status.total_size  = size;
        g_status.recv_size   = 0;
        g_prevstatus = NAND_SUCCESS;
	g_ReadBufLen = 0;
	g_BootimgCurAddr = 0;
	g_BootimgWritesize = 0;
	check_image_type = 0;

	set_dl_op_val(start_addr, size, STARTDATA, SUCCESS, 1);
        FDL_SendAckPacket (BSL_REP_ACK);	
        return 1;
    }
    while (0);

    FDL2_SendRep (ret);
    return 0;
}

/******************************************************************************
 * make the checksum of one packet
 ******************************************************************************/
unsigned short CheckSum(const unsigned int *src, int len)
{
    unsigned int   sum = 0;
    unsigned short *src_short_ptr = PNULL;

    while (len > 3)
    {
        sum += *src++;
        len -= 4;
    }

    src_short_ptr = (unsigned short *) src;

    if (0 != (len&0x2))
    {
        sum += * (src_short_ptr);
        src_short_ptr++;
    }

    if (0 != (len&0x1))
    {
        sum += * ( (unsigned char *) (src_short_ptr));
    }

    sum  = (sum >> 16) + (sum & 0x0FFFF);
    sum += (sum >> 16);

    return (unsigned short) (~sum);
}

/******************************************************************************
 * change the header of first bootloader page
 ******************************************************************************/
int NandChangeBootloaderHeader(unsigned int *bl_start_addr)
{
    unsigned int       *start_addr = bl_start_addr;
    unsigned short     check_sum = 0;

    unsigned short 	g_PageAttr = 0x1;
    unsigned short      nCycleDev = 0x5;
    unsigned int        nAdvance = 1;


    //set pointer to nand parameter config start address
    start_addr += BOOTLOADER_HEADER_OFFSET / 4;

    //set nand page attribute
    * (start_addr + 1) = g_PageAttr;
    //set nand address cycle
    * (start_addr+2) = nCycleDev;

    //set nand data bus len
    //* (start_addr + 3) = NAND_BUS_SIZE_8;
    * (start_addr + 3) = NAND_BUS_SIZE_16;
    //* (start_addr + 3) = NAND_BUS_SIZE_32;

    if (0) // for 6800h
    {
        //set magic data
        * (start_addr+4) = NAND_MAGIC_DATA;
        //make checksum of first 504 bytes
        check_sum = CheckSum ((unsigned int *) (start_addr + 1), (NAND_PAGE_LEN - BOOTLOADER_HEADER_OFFSET - 4));
    }
    else
    {
        if (nAdvance)
            * (start_addr + 4) = 1;
        else
            * (start_addr + 4) = 0;

        //set magic data
        * (start_addr + 5) = NAND_MAGIC_DATA;

        //make checksum of first 504 bytes
        check_sum = CheckSum((unsigned int *)(start_addr + 1), (NAND_PAGE_LEN - BOOTLOADER_HEADER_OFFSET - 4));
    }

    //set checksum
    * (start_addr) = (unsigned int) check_sum;

    return 1;

}

int FDL2_DataEnd (PACKET_T *packet, void *arg)
{
	unsigned long pos, size, ret, crc;
    	unsigned long i, fix_nv_size, fix_nv_checksum, ii, realii;
	unsigned short sum = 0, *dataaddr;

	set_dl_op_val(0, 0, ENDDATA, FAIL, 1);
    	if (CHECKSUM_OTHER_DATA != g_checksum) {
		/* It's fixnv data */
        	fix_nv_size = g_sram_addr - (unsigned long) g_fixnv_buf;
        	fix_nv_checksum = Get_CheckSum ( (unsigned char *) g_fixnv_buf, fix_nv_size);
        	fix_nv_checksum = EndianConv_32 (fix_nv_checksum);
        	if (fix_nv_checksum != g_checksum)
            		SEND_ERROR_RSP(BSL_CHECKSUM_DIFF);
		
		/* write fixnv to yaffs2 format : orginal */
		char *fixnvpoint = "/fixnv";
		char *fixnvfilename = "/fixnv/fixnv.bin";
		char *backupfixnvpoint = "/backupfixnv";
		char *backupfixnvfilename = "/backupfixnv/fixnv.bin";
		
		/* g_fixnv_buf : (FIXNV_SIZE + 4) instead of fix_nv_size */
		g_fixnv_buf[FIXNV_SIZE + 0] = g_fixnv_buf[FIXNV_SIZE + 1] = 0x5a;
		g_fixnv_buf[FIXNV_SIZE + 2] = g_fixnv_buf[FIXNV_SIZE + 3] = 0x5a;

		sum = calc_checksum(g_fixnv_buf, FIXNV_SIZE - 4);
		dataaddr = (unsigned short *)(g_fixnv_buf + FIXNV_SIZE - 4);
		*dataaddr = sum;
		dataaddr = (unsigned short *)(g_fixnv_buf + FIXNV_SIZE - 2);
		*dataaddr = 0;

		/* erase fixnv partition */
		memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
		strcpy(phy_partition.name, "fixnv");
		ret = log2phy_table(&phy_partition);
		phy_partition_info(phy_partition, __LINE__);
		g_prevstatus = ret;
		if (ret == NAND_SUCCESS) {
			printf("erase fixnv start\n");
			ret = nand_start_write (&phy_partition, fix_nv_size, &nand_page_oob_info, 0);
			printf("\nerase fixnv end\n");
			g_prevstatus = ret;
			if (ret == NAND_SUCCESS) {
				printf("write fixnv start\n");
				cmd_yaffs_mount(fixnvpoint);
    				cmd_yaffs_mwrite_file(fixnvfilename, (char *)g_fixnv_buf, (FIXNV_SIZE + 4));
				ret = cmd_yaffs_ls_chk(fixnvfilename);
				cmd_yaffs_umount(fixnvpoint);
				printf("write fixnv end\n");
				g_prevstatus = NAND_SUCCESS;			
			}
		}

		/* write fixnv to yaffs2 format : backup */
		/* erase backup fixnv partition */
		memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
		strcpy(phy_partition.name, "backupfixnv");
		ret = log2phy_table(&phy_partition);
		phy_partition_info(phy_partition, __LINE__);
		g_prevstatus = ret;
		if (ret == NAND_SUCCESS) {
			printf("erase backupfixnv start\n");
			ret = nand_start_write (&phy_partition, fix_nv_size, &nand_page_oob_info, 0);
			printf("\nerase backupfixnv end\n");
			g_prevstatus = ret;
			if (ret == NAND_SUCCESS) {
				cmd_yaffs_mount(backupfixnvpoint);
    				cmd_yaffs_mwrite_file(backupfixnvfilename, (char *)g_fixnv_buf, (FIXNV_SIZE + 4));
				ret = cmd_yaffs_ls_chk(backupfixnvfilename);
				cmd_yaffs_umount(backupfixnvpoint);
				g_prevstatus = NAND_SUCCESS;
			}
		}
    	} else if (is_nbl_write == 1) {
#if (defined(CONFIG_NAND_SC8810) || defined(CONFIG_NAND_TIGER))//only for sc8810 to write spl
		g_prevstatus = nand_write_fdl(0x0, g_FixNBLBuf);
#else

		/* write the spl loader image to the nnd*/
		for (i = 0; i < 3; i++) {
			pos = 0;
			while (pos < g_NBLFixBufDataSize) {
				if ((g_NBLFixBufDataSize  pos) >= 2048)
					size = 2048;
				else
					size = g_NBLFixBufDataSize - pos;
				if (size == 0)
					break;
				g_prevstatus = nand_write_fdl (size, g_FixNBLBuf + pos);
				pos += size;
			}
		}
#endif
		is_nbl_write = 0;
   	} else if (is_phasecheck_write == 1) {
		/* write phasecheck to yaffs2 format */
		char *productinfopoint = "/productinfo";
		char *productinfofilename = "/productinfo/productinfo.bin";
		char *bkproductinfofilename = "/productinfo/productinfobkup.bin";
		char *nvramfilename = "/productinfo/nvram";
		char *amt0filename = "/productinfo/amt0";
		char *amt2filename = "/productinfo/amt2";
		char *factfilename = "/productinfo/fact";

#ifdef CONFIG_BOARD_788
		char *zteversionfilename = "/productinfo/ZTEversion";
#endif
		if (file_in_productinfo_partition == 0x90000002) {
			/* g_PhasecheckBUF : (PRODUCTINFO_SIZE + 4) instead of g_PhasecheckBUFDataSize */
			sum = calc_checksum(g_PhasecheckBUF, PRODUCTINFO_SIZE);
			dataaddr = (unsigned short *)(g_PhasecheckBUF + PRODUCTINFO_SIZE);
			*dataaddr = sum;
			dataaddr = (unsigned short *)(g_PhasecheckBUF + PRODUCTINFO_SIZE + 2);
			*dataaddr = 0;

			cmd_yaffs_mount(productinfopoint);
    			cmd_yaffs_mwrite_file(productinfofilename, g_PhasecheckBUF, (PRODUCTINFO_SIZE + 4));
			ret = cmd_yaffs_ls_chk(productinfofilename);
			cmd_yaffs_mwrite_file(bkproductinfofilename, g_PhasecheckBUF, (PRODUCTINFO_SIZE + 4));
			ret = cmd_yaffs_ls_chk(bkproductinfofilename);
        		cmd_yaffs_umount(productinfopoint);

			g_prevstatus = NAND_SUCCESS;
			/* factorydownload tools */
			is_factorydownload_tools = 1;
			is_check_dlstatus = get_DL_Status();
			if (is_check_dlstatus == 1) {
				get_Dl_Erase_Address_Table(&Dl_Erase_Address);
				get_Dl_Data_Address_Table(&Dl_Data_Address);
			}
		} else if (file_in_productinfo_partition == 0x90000022) {
			cmd_yaffs_mount(productinfopoint);
    			cmd_yaffs_mwrite_file(nvramfilename, g_PhasecheckBUF, PRODUCTINFO_SIZE);
			ret = cmd_yaffs_ls_chk(nvramfilename);
			cmd_yaffs_umount(productinfopoint);
			g_prevstatus = NAND_SUCCESS;
		}else if (file_in_productinfo_partition == 0x90000023) {
			cmd_yaffs_mount(productinfopoint);
    			cmd_yaffs_mwrite_file(amt0filename, g_PhasecheckBUF, 1);
			ret = cmd_yaffs_ls_chk(amt0filename);
			cmd_yaffs_umount(productinfopoint);
			g_prevstatus = NAND_SUCCESS;
		}else if (file_in_productinfo_partition == 0x90000024) {
			cmd_yaffs_mount(productinfopoint);
    			cmd_yaffs_mwrite_file(amt2filename, g_PhasecheckBUF, 1);
			ret = cmd_yaffs_ls_chk(amt2filename);
			cmd_yaffs_umount(productinfopoint);
			g_prevstatus = NAND_SUCCESS;
		}else if (file_in_productinfo_partition == 0x90000025) {
			cmd_yaffs_mount(productinfopoint);
    			cmd_yaffs_mwrite_file(factfilename, g_PhasecheckBUF, 1);
			ret = cmd_yaffs_ls_chk(factfilename);
			cmd_yaffs_umount(productinfopoint);
			g_prevstatus = NAND_SUCCESS;
		}
#ifdef CONFIG_BOARD_788
		else if (file_in_productinfo_partition == 0x90000026) {
			cmd_yaffs_mount(productinfopoint);
			cmd_yaffs_mwrite_file(zteversionfilename, g_PhasecheckBUF, 512);
			ret = cmd_yaffs_ls_chk(zteversionfilename);
			cmd_yaffs_umount(productinfopoint);
			g_prevstatus = NAND_SUCCESS;
		}
#endif
    	}

#ifdef CONFIG_BOARD_788
	if (find_zte_version == 1) {
		char *productinfopoint = "/productinfo";
		char *productinfofilename = "/productinfo/productinfo.bin";

		cmd_yaffs_mount(productinfopoint);
		ret = cmd_yaffs_ls_chk(productinfofilename);
	if (ret > 0) {
		char *zte_buf = NULL;
		zte_buf = g_zte_version(buildprop_BUF);

		if (zte_buf == NULL) {
			printf("zte version is NULL");
		} else {
			char *zteversionfilename = "/productinfo/ZTEversion";
			cmd_yaffs_mwrite_file(zteversionfilename, zte_buf,512);
			ret = cmd_yaffs_ls_chk(zteversionfilename);
			cmd_yaffs_umount(productinfopoint);
			g_prevstatus = NAND_SUCCESS;
			find_zte_version = 0;
		}
	}
}
#endif

#ifdef	TRANS_CODE_SIZE
	else {
		//printf("data end, g_BigSize = %d\n", g_BigSize);
		if (yaffs2_is4kImg())
			g_prevstatus = yaffs2_convertAndWrite();
		else if (mkboot_is4kImg())
			g_prevstatus = bootimg_convertAndWrite();
		else {
		ii = 0;
		while (ii < g_BigSize) {
			realii = min(g_BigSize - ii, code_yaffs_onewrite);
			if (strcmp(phy_partition.name, "cache") == 0)
				g_prevstatus = NAND_SUCCESS;
			else
				g_prevstatus = nand_write_fdl((unsigned int)realii, 
					(unsigned char *)(g_BigBUF + ii));
			if (NAND_SUCCESS != g_prevstatus) {
				break;
			}
			ii += realii;
			}
		}
		g_BigSize = 0;
	}
#endif

    	if (NAND_SUCCESS != g_prevstatus) {
		set_dl_op_val(0, 0, ENDDATA, FAIL, 2);
        	FDL2_SendRep (g_prevstatus);
        	return 0;
    	}

    	g_prevstatus = nand_end_write();
	set_dl_op_val(0, 0, ENDDATA, SUCCESS, 1);
    	FDL2_SendRep (g_prevstatus);
    	return (NAND_SUCCESS == g_prevstatus);
}


int NandWriteAndCheck(unsigned int size, unsigned char *buf)
{

    memcpy (g_FixNBLBuf + g_NBLFixBufDataSize, buf, size); /* copy the data to the temp buffer */
    g_NBLFixBufDataSize += size;

    if ((g_NBLFixBufDataSize) <= ECC_NBL_SIZE)
    {
        return NAND_SUCCESS;
    }
#if !(defined(CONFIG_NAND_SC8810) || defined(CONFIG_NAND_TIGER))
    NandChangeBootloaderHeader((unsigned int *) g_FixNBLBuf);
#endif	
    return NAND_SUCCESS;
}

int FDL2_DataMidst (PACKET_T *packet, void *arg)
{
    	unsigned long size;
	unsigned long ii;
    /* The previous download step failed. */
    if (NAND_SUCCESS != g_prevstatus)
    {
	set_dl_op_val(0, 0, MIDSTDATA, FAIL, 1);
        FDL2_SendRep (g_prevstatus);
        return 0;
    }

    size = packet->packet_body.size;
	//printf("size = %d  recv_size = %d   total_size = %d\n", size, g_status.recv_size, g_status.total_size);
    if ( (g_status.recv_size + size) > g_status.total_size)
    {
        g_prevstatus = NAND_INVALID_SIZE;
	set_dl_op_val(0, 0, MIDSTDATA, FAIL, 2);
        FDL2_SendRep (g_prevstatus);
        return 0;
    }

    if (CHECKSUM_OTHER_DATA == g_checksum)
    {
        if (is_nbl_write == 1) {
		g_prevstatus = NandWriteAndCheck( (unsigned int) size, (unsigned char *) (packet->packet_body.content));
	} else if (is_phasecheck_write == 1) {
        	memcpy((g_PhasecheckBUF + g_PhasecheckBUFDataSize), (char *)(packet->packet_body.content), size);
        	g_PhasecheckBUFDataSize += size;
		g_prevstatus = NAND_SUCCESS;
	} else {
#ifdef TRANS_CODE_SIZE
		//printf("g_BigSize = %d  buflen = %d, onewrite = %d  size = %d\n", g_BigSize, code_yaffs_buflen, code_yaffs_onewrite, size);
		memcpy((g_BigBUF + g_BigSize), (char *)(packet->packet_body.content), size);
		g_BigSize += size;

		if (g_BigSize < (code_yaffs_buflen / 2)) {
			g_prevstatus = NAND_SUCCESS;
		} else {
				if (yaffs2_is4kImg())
					g_prevstatus = yaffs2_convertAndWrite();
				else if (mkboot_is4kImg())
					g_prevstatus = bootimg_convertAndWrite();
				else {
			for (ii = 0; ii < g_BigSize; ii += code_yaffs_onewrite) {
				if (strcmp(phy_partition.name, "cache") == 0)
					g_prevstatus = NAND_SUCCESS;
				else {
#ifdef CONFIG_BOARD_788
					if ((find_zte_version == 1) && (zte_version_size > 0)) {
							memcpy(buildprop_BUF + zte_version_point, g_BigBUF + ii, 2048);
							zte_version_point += 2048;
							zte_version_size -= 2048;
					}
#endif
					g_prevstatus = nand_write_fdl((unsigned int)code_yaffs_onewrite, 
						(unsigned char *)(g_BigBUF + ii));

#ifdef CONFIG_BOARD_788
				if ((download_system_flag == 1) && (find_zte_version == 0)) {
						yaffs_PackedTags2 checkpt;

						memset(&checkpt, 0xff, sizeof(yaffs_PackedTags2));
						memcpy(&checkpt, g_BigBUF + ii + 2048, sizeof(yaffs_PackedTags2));

						if (checkpt.t.chunkId == 0) {
							yaffs_ObjectHeader checkoh;
							memset(&checkoh, 0xff, sizeof(yaffs_ObjectHeader));
							memcpy(&checkoh, g_BigBUF + ii, sizeof(yaffs_ObjectHeader));

							if ((strcmp(checkoh.name,"build.prop") ) == 0) {
								find_zte_version = 1;
								zte_version_size = checkoh.fileSize;
								zte_version_point = 0;
								memset(buildprop_BUF, 0xff, 2048);
							}
					}
				}
#endif

				if (NAND_SUCCESS != g_prevstatus) {
					printf("big buffer write error.\n");				
					break;
				}
			}
					}
			g_BigSize = 0;
			memset(g_BigBUF, 0xff, yaffs_buffer_size);
				}
		}
#else
		if (strcmp(phy_partition.name, "cache") == 0)
			g_prevstatus = NAND_SUCCESS;
		else
        		g_prevstatus = nand_write_fdl((unsigned int) size, 
				(unsigned char *)(packet->packet_body.content));
#endif
	}

        if (NAND_SUCCESS == g_prevstatus)
        {
            g_status.recv_size += size;

            if (!packet->ack_flag)
            {
                packet->ack_flag = 1;
		set_dl_op_val(0, 0, MIDSTDATA, SUCCESS, 8);
		FDL_SendAckPacket (BSL_REP_ACK);
                return NAND_SUCCESS == g_prevstatus;
            }
        }

	set_dl_op_val(0, 0, MIDSTDATA, FAIL, 4);
        FDL2_SendRep (g_prevstatus);
        return NAND_SUCCESS == g_prevstatus;
    }
    else //It's fixnv data. We should backup it.
    {
        memcpy ( (unsigned char *) g_sram_addr, (char *) (packet->packet_body.content), size);
        g_sram_addr += size;
	g_status.recv_size += size;
	set_dl_op_val(0, 0, MIDSTDATA, SUCCESS, 8);
        FDL_SendAckPacket (BSL_REP_ACK);
        return 1;
    }
}
#ifdef FPGA_TRACE_DOWNLOAD
#ifndef CONFIG_EMMC_BOOT
int FDL2_DramStart (unsigned long base_addr, unsigned long size)
{
    unsigned long start_addr = base_addr;
    int           ret;
    //FDL_SendAckPacket (BSL_REP_ACK);
    
    set_dl_op_val(start_addr, size, STARTDATA, FAIL, 1);

    //if (packet->packet_body.size == 12)
    if (base_addr == 0x90000001) //nv
    {
	memset(g_fixnv_buf, 0xff, FIXNV_SIZE + 4);
        //g_checksum = * (data+2);
        g_checksum = 0;
        g_sram_addr = (unsigned long) g_fixnv_buf;
    } else {
	        g_checksum = CHECKSUM_OTHER_DATA;
    }

    do
    {
	memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
	phy_partition.offset = custom2log(start_addr);
	ret = log2phy_table(&phy_partition);
	phy_partition_info(phy_partition, __LINE__);

	if (NAND_SUCCESS != ret)
        	break;

	if (size >= phy_partition.size) {
		printf("\n\nimage file size : 0x%08x is bigger than partition size : 0x%08x\n", size, phy_partition.size);
		ret = NAND_INVALID_SIZE;
	}
	if (NAND_SUCCESS != ret)
        	break;

	if (strcmp(phy_partition.name, "fixnv") == 0)
		ret = get_nand_pageoob(&nand_page_oob_info);
	else
        	ret = nand_start_write(&phy_partition, size, &nand_page_oob_info);
        if (NAND_SUCCESS != ret)
            break;

	is_nbl_write = 0;
	is_phasecheck_write = 0;

	if (strcmp(phy_partition.name, "spl") == 0) {
		is_nbl_write = 1;
		g_NBLFixBufDataSize = 0;
	} else if (strcmp(phy_partition.name, "productinfo") == 0) {
		is_phasecheck_write = 1;
		g_PhasecheckBUFDataSize = 0;
		memset(g_PhasecheckBUF, 0xff, 0x2000);
	}

#ifdef TRANS_CODE_SIZE
	yaffs_buffer_size = (DATA_BUFFER_SIZE + (DATA_BUFFER_SIZE / nand_page_oob_info.writesize) * nand_page_oob_info.oobsize);

	if (phy_partition.yaffs == 0) {
		code_yaffs_buflen = DATA_BUFFER_SIZE;
		code_yaffs_onewrite = nand_page_oob_info.writesize;
	} else if (phy_partition.yaffs == 1) {
		code_yaffs_buflen = yaffs_buffer_size;
		code_yaffs_onewrite = nand_page_oob_info.writesize + nand_page_oob_info.oobsize;
	}
	
	g_BigSize = 0;
	if (g_BigBUF == NULL)
		g_BigBUF = (unsigned char *)malloc(yaffs_buffer_size);

	if (g_BigBUF == NULL) {
		printf("malloc is wrong : %d\n", yaffs_buffer_size);
		ret = NAND_SYSTEM_ERROR;		
		break;
	}
	memset(g_BigBUF, 0xff, yaffs_buffer_size);
	//printf("code_yaffs_onewrite = %d  code_yaffs_buflen = %d  yaffs_buffer_size = %d\n", code_yaffs_onewrite, code_yaffs_buflen, yaffs_buffer_size);
#endif

        g_status.total_size  = size;
        g_status.recv_size   = 0;
        g_prevstatus = NAND_SUCCESS;

	set_dl_op_val(start_addr, size, STARTDATA, SUCCESS, 1);
        //FDL_SendAckPacket (BSL_REP_ACK);	
        return 1;
    }
    while (0);

    //FDL2_SendRep (ret);
    return 0;
}
int FDL2_DramMidst (unsigned char *pbuf, unsigned long size)
{
	unsigned long ii;

    /* The previous download step failed. */
    if (NAND_SUCCESS != g_prevstatus)
    {
	set_dl_op_val(0, 0, MIDSTDATA, FAIL, 1);
        //FDL2_SendRep (g_prevstatus);
        return 0;
    }

   // size = packet->packet_body.size;
	//printf("size = %d  recv_size = %d   total_size = %d\n", size, g_status.recv_size, g_status.total_size);
    if ( (g_status.recv_size + size) > g_status.total_size)
    {
        g_prevstatus = NAND_INVALID_SIZE;
	set_dl_op_val(0, 0, MIDSTDATA, FAIL, 2);
        //FDL2_SendRep (g_prevstatus);
        return 0;
    }

    if (CHECKSUM_OTHER_DATA == g_checksum)
    {
        if (is_nbl_write == 1) {
		g_prevstatus = NandWriteAndCheck( (unsigned int) size, (unsigned char *) (pbuf));
	} else if (is_phasecheck_write == 1) {
		printf("g_PhasecheckBUFDataSize = %d\n", g_PhasecheckBUFDataSize);
        	memcpy((g_PhasecheckBUF + g_PhasecheckBUFDataSize), (char *)(pbuf), size);
        	g_PhasecheckBUFDataSize += size;
		g_prevstatus = NAND_SUCCESS;
	} else {
#ifdef TRANS_CODE_SIZE
		//printf("g_BigSize = %d  buflen = %d, onewrite = %d  size = %d\n", g_BigSize, code_yaffs_buflen, code_yaffs_onewrite, size);
		memcpy((g_BigBUF + g_BigSize), (char *)(pbuf), size);
		g_BigSize += size;

		if (g_BigSize < code_yaffs_buflen) {
			//printf("continue to big buffer\n");
			g_prevstatus = NAND_SUCCESS;
		} else {
			//printf("big buffer is full. g_BigSize = %d\n", g_BigSize);
			for (ii = 0; ii < g_BigSize; ii += code_yaffs_onewrite) {
				//printf(".");
				g_prevstatus = nand_write_fdl( (unsigned int) code_yaffs_onewrite, (unsigned char *) (g_BigBUF + ii));
				if (NAND_SUCCESS != g_prevstatus) {
					//printf("\n");
					printf("big buffer write error.\n");				
					break;
				}
			}
			//printf("\n");
			g_BigSize = 0;
			memset(g_BigBUF, 0xff, yaffs_buffer_size);
		}
#else
        	g_prevstatus = nand_write_fdl( (unsigned int) size, (unsigned char *) (pbuf));
#endif
	}

        if (NAND_SUCCESS == g_prevstatus)
        {
            g_status.recv_size += size;

            //if (!packet->ack_flag)
            //  {
           //      packet->ack_flag = 1;
	   //	set_dl_op_val(0, 0, MIDSTDATA, SUCCESS, 8);
		//FDL_SendAckPacket (BSL_REP_ACK);
          //       return NAND_SUCCESS == g_prevstatus;
          //  }
        }

	set_dl_op_val(0, 0, MIDSTDATA, FAIL, 4);
        //FDL2_SendRep (g_prevstatus);
        return NAND_SUCCESS == g_prevstatus;
    }
    else //It's fixnv data. We should backup it.
    {
        memcpy ( (unsigned char *) g_sram_addr, (char *) (pbuf), size);
        g_sram_addr += size;
	g_status.recv_size += size;
	set_dl_op_val(0, 0, MIDSTDATA, SUCCESS, 8);
        //FDL_SendAckPacket (BSL_REP_ACK);
        return 1;
    }
}
int FDL2_DramEnd ()
{
	unsigned long pos, size, ret;
    	unsigned long i, fix_nv_size, fix_nv_checksum, ii, realii;

	set_dl_op_val(0, 0, ENDDATA, FAIL, 1);
    	if (CHECKSUM_OTHER_DATA != g_checksum) {
		/* It's fixnv data */
        	fix_nv_size = g_sram_addr - (unsigned long) g_fixnv_buf;
        	fix_nv_checksum = Get_CheckSum ( (unsigned char *) g_fixnv_buf, fix_nv_size);
        	fix_nv_checksum = EndianConv_32 (fix_nv_checksum);
        	//don't check the data when use trace32 load the nv
        	//if (fix_nv_checksum != g_checksum)
        	//	printf("FDL2_DramEnd fixnv check sum error\n");
		
		/* write fixnv to yaffs2 format : orginal */
		char *fixnvpoint = "/fixnv";
		char *fixnvfilename = "/fixnv/fixnv.bin";
		
		/* g_fixnv_buf : (FIXNV_SIZE + 4) instead of fix_nv_size */
		g_fixnv_buf[FIXNV_SIZE + 0] = g_fixnv_buf[FIXNV_SIZE + 1] = 0x5a;
		g_fixnv_buf[FIXNV_SIZE + 2] = g_fixnv_buf[FIXNV_SIZE + 3] = 0x5a;
		/* erase fixnv partition */
		memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
		strcpy(phy_partition.name, "fixnv");
		ret = log2phy_table(&phy_partition);
		phy_partition_info(phy_partition, __LINE__);
		g_prevstatus = ret;
		if (ret == NAND_SUCCESS) {
			printf("erase fixnv start\n");
			ret = nand_start_write (&phy_partition, fix_nv_size, &nand_page_oob_info);
			printf("\nerase fixnv end\n");
			g_prevstatus = ret;
			if (ret == NAND_SUCCESS) {
				printf("write fixnv start\n");
				cmd_yaffs_mount(fixnvpoint);
    				cmd_yaffs_mwrite_file(fixnvfilename, (char *)g_fixnv_buf, (FIXNV_SIZE + 4));
				ret = cmd_yaffs_ls_chk(fixnvfilename);
				cmd_yaffs_umount(fixnvpoint);
				printf("write fixnv end\n");
				g_prevstatus = NAND_SUCCESS;			
			}
		}

		/* write fixnv to yaffs2 format : backup */
		char *backupfixnvpoint = "/backupfixnv";
		char *backupfixnvfilename = "/backupfixnv/fixnv.bin";

		/* g_fixnv_buf : (FIXNV_SIZE + 4) instead of fix_nv_size */
		g_fixnv_buf[FIXNV_SIZE + 0] = g_fixnv_buf[FIXNV_SIZE + 1] = 0x5a;
		g_fixnv_buf[FIXNV_SIZE + 2] = g_fixnv_buf[FIXNV_SIZE + 3] = 0x5a;
		/* erase backup fixnv partition */
		memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
		strcpy(phy_partition.name, "backupfixnv");
		ret = log2phy_table(&phy_partition);
		phy_partition_info(phy_partition, __LINE__);
		g_prevstatus = ret;
		if (ret == NAND_SUCCESS) {
			printf("erase backupfixnv start\n");
			ret = nand_start_write (&phy_partition, fix_nv_size, &nand_page_oob_info);
			printf("\nerase backupfixnv end\n");
			g_prevstatus = ret;
			if (ret == NAND_SUCCESS) {
				printf("write backupfixnv start\n");
				cmd_yaffs_mount(backupfixnvpoint);
    				cmd_yaffs_mwrite_file(backupfixnvfilename, (char *)g_fixnv_buf, (FIXNV_SIZE + 4));
				ret = cmd_yaffs_ls_chk(backupfixnvfilename);
				cmd_yaffs_umount(backupfixnvpoint);
				printf("write backupfixnv end\n");
				g_prevstatus = NAND_SUCCESS;
			}
		}
		//////////////////////////////
    	} else if (is_nbl_write == 1) {
#if (defined(CONFIG_NAND_SC8810) || defined(CONFIG_NAND_TIGER))	//only for write spl
		g_prevstatus = nand_write_fdl(0x0, g_FixNBLBuf);
#else
	   	/* write the spl loader image to the nand*/
		for (i = 0; i < 3; i++) {
			pos = 0;
			while (pos < g_NBLFixBufDataSize) {
				if ((g_NBLFixBufDataSize - pos) >= 2048)
					size = 2048;
				else
					size = g_NBLFixBufDataSize - pos;
				//printf("pos = %d  size = %d\n", pos, size);
				if (size == 0)
					break;
				g_prevstatus = nand_write_fdl (size, g_FixNBLBuf + pos);
				pos += size;
			}
        	}//for (i = 0; i < 3; i++)
#endif
		is_nbl_write = 0;
   	} else if (is_phasecheck_write == 1) {
		/* write phasecheck to yaffs2 format */
		char *productinfopoint = "/productinfo";
		char *productinfofilename = "/productinfo/productinfo.bin";

		/* g_PhasecheckBUF : (PHASECHECK_SIZE + 4) instead of g_PhasecheckBUFDataSize */
		g_PhasecheckBUF[PHASECHECK_SIZE + 0] = g_PhasecheckBUF[PHASECHECK_SIZE + 1] = 0x5a;
		g_PhasecheckBUF[PHASECHECK_SIZE + 2] = g_PhasecheckBUF[PHASECHECK_SIZE + 3] = 0x5a;
		cmd_yaffs_mount(productinfopoint);
    		cmd_yaffs_mwrite_file(productinfofilename, g_PhasecheckBUF, (PHASECHECK_SIZE + 4));
		ret = cmd_yaffs_ls_chk(productinfofilename);
		cmd_yaffs_umount(productinfopoint);
		g_prevstatus = NAND_SUCCESS;
		/* factorydownload tools */
		is_factorydownload_tools = 1;
		is_check_dlstatus = get_DL_Status();
		if (is_check_dlstatus == 1) {
			get_Dl_Erase_Address_Table(&Dl_Erase_Address);
			get_Dl_Data_Address_Table(&Dl_Data_Address);
		}
    	}
#ifdef	TRANS_CODE_SIZE
	else {
		//printf("data end, g_BigSize = %d\n", g_BigSize);
		ii = 0;
		while (ii < g_BigSize) {
			realii = min(g_BigSize - ii, code_yaffs_onewrite);
			//printf(".");
			g_prevstatus = nand_write_fdl( (unsigned int) realii, (unsigned char *) (g_BigBUF + ii));
			if (NAND_SUCCESS != g_prevstatus) {
				//printf("\n");
				printf("big buffer write error.\n");				
				break;
			}
			ii += realii;
		}
		//printf("\n");
		g_BigSize = 0;
	}
#endif

    	if (NAND_SUCCESS != g_prevstatus) {
		set_dl_op_val(0, 0, ENDDATA, FAIL, 2);
        	//FDL2_SendRep (g_prevstatus);
        	return 0;
    	}

    	g_prevstatus = nand_end_write();
	set_dl_op_val(0, 0, ENDDATA, SUCCESS, 1);
    	//FDL2_SendRep (g_prevstatus);
    	return (NAND_SUCCESS == g_prevstatus);
}
#endif
#endif
int FDL2_ReadFlash (PACKET_T *packet, void *arg)
{
    	unsigned long *data = (unsigned long *) (packet->packet_body.content);
    	unsigned long addr = *data;
    	unsigned long size = * (data + 1);
    	unsigned long off = 0;
    	int           ret;

#if defined(CHIP_ENDIAN_LITTLE)
    	addr = EndianConv_32 (addr);
    	size = EndianConv_32 (size);
#endif
	memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
	phy_partition.offset = custom2log(addr);
	ret = log2phy_table(&phy_partition);
    
	if (size > MAX_PKT_SIZE) {
        	FDL_SendAckPacket (BSL_REP_DOWN_SIZE_ERROR);
        	return 0;
    	}

    	if (packet->packet_body.size > 8)
        	off = EndianConv_32 (* (data + 2));
	//printf("addr = 0x%08x  size = 0x%08x  off = 0x%08x  name = %s\n", addr, size, off, phy_partition.name);

	if ((strcmp(phy_partition.name, "fixnv") == 0) || (strcmp(phy_partition.name, "productinfo") == 0))
		ret = nand_read_fdl_yaffs(&phy_partition, off, size, (unsigned char *)(packet->packet_body.content));
    	else if ((strcmp(phy_partition.name, "spl") == 0) || (strcmp(phy_partition.name, "2ndbl") == 0))
    		ret = nand_read_fdl(&phy_partition, off, size, (unsigned char *)(packet->packet_body.content));
    	else
		ret = NAND_INVALID_ADDR;

    	if (NAND_SUCCESS == ret) {
        	packet->packet_body.type = BSL_REP_READ_FLASH;
        	packet->packet_body.size = size;
        	FDL_SendPacket (packet);
        	return 1;
    	} else {
        	FDL2_SendRep (ret);
        	return 0;
    	}
}

int FDL2_EraseFlash (PACKET_T *packet, void *arg)
{
    unsigned long *data = (unsigned long *) (packet->packet_body.content);
    unsigned long addr = *data;
    unsigned long size = * (data + 1);
    int           ret;
    int           cnt;
    int           dl_op_buf_len = 0;
    int 	  dl_item_cnt;
    DL_OP_STATUS_E	dl_data_status, dl_erase_status;
	
    addr = EndianConv_32 (addr);
    size = EndianConv_32 (size);
	
	set_dl_op_val(addr, size, ERASEFLASH, FAIL, 1);
	if ((addr == 0) && (size = 0xffffffff)) {
		printf("Scrub to erase all of flash\n");
		nand_erase_allflash();
		ret = NAND_SUCCESS;
		set_dl_op_val(addr, size, ERASEFLASH, SUCCESS, 1);
	} else {
		memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
		phy_partition.offset = custom2log(addr);

		if ((file_in_productinfo_partition >= 0x90000022) && (file_in_productinfo_partition <= 0x90000026 )) {
			char *productinfopoint = "/productinfo";
			char *nvramfilename = "/productinfo/nvram";
			char *amt0filename = "/productinfo/amt0";
			char *amt2filename = "/productinfo/amt2";
			char *factfilename = "/productinfo/fact";
			char *zteversionfilename = "/productinfo/ZTEversion";
			char convbuf[2] = {0x5a, 0x5a};

			if (file_in_productinfo_partition == 0x90000022) {
				cmd_yaffs_mount(productinfopoint);
				ret = cmd_yaffs_ls_chk(nvramfilename);
				if (ret == 0) {
					cmd_yaffs_mwrite_file(nvramfilename, convbuf, 1);
					ret = 1;
				}
				if(ret > 0)
					ret = cmd_yaffs_rm_chk(nvramfilename);
				cmd_yaffs_umount(productinfopoint);
				if(ret < 0)
					g_prevstatus = NAND_SYSTEM_ERROR;
				else
					g_prevstatus = NAND_SUCCESS;
			} else if (file_in_productinfo_partition == 0x90000023) {
				cmd_yaffs_mount(productinfopoint);
				ret = cmd_yaffs_ls_chk(amt0filename);
				if (ret == 0) {
					cmd_yaffs_mwrite_file(amt0filename, convbuf, 1);
					ret = 1;
				}
				if(ret > 0)
					ret = cmd_yaffs_rm_chk(amt0filename);
				cmd_yaffs_umount(productinfopoint);
				if(ret < 0)
					g_prevstatus = NAND_SYSTEM_ERROR;
				else
					g_prevstatus = NAND_SUCCESS;
			} else if (file_in_productinfo_partition == 0x90000024) {
				cmd_yaffs_mount(productinfopoint);
				ret = cmd_yaffs_ls_chk(amt2filename);
				if (ret == 0) {
					cmd_yaffs_mwrite_file(amt2filename, convbuf, 1);
					ret = 1;
				}
				if(ret > 0)
					ret = cmd_yaffs_rm_chk(amt2filename);
				cmd_yaffs_umount(productinfopoint);
				if(ret < 0)
					g_prevstatus = NAND_SYSTEM_ERROR;
				else
					g_prevstatus = NAND_SUCCESS;
			} else if (file_in_productinfo_partition == 0x90000025) {
				cmd_yaffs_mount(productinfopoint);
    				cmd_yaffs_mwrite_file(factfilename, g_PhasecheckBUF, 1);
				ret = cmd_yaffs_ls_chk(factfilename);
				if (ret == 0) {
					cmd_yaffs_mwrite_file(factfilename, convbuf, 1);
					ret = 1;
				}
				if(ret > 0)
					ret=cmd_yaffs_rm_chk(factfilename);
				cmd_yaffs_umount(productinfopoint);
				if(ret < 0)
					g_prevstatus = NAND_SYSTEM_ERROR;
				else
					g_prevstatus = NAND_SUCCESS;
			}
#ifdef CONFIG_BOARD_788
			else if (file_in_productinfo_partition == 0x90000026) {
				cmd_yaffs_mount(productinfopoint);
				cmd_yaffs_mwrite_file(zteversionfilename, g_PhasecheckBUF, 1);
				ret = cmd_yaffs_ls_chk(zteversionfilename);

				if (ret == 0) {
					cmd_yaffs_mwrite_file(zteversionfilename, convbuf, 1);
					ret = 1;
				}

				if (ret > 0)
					ret = cmd_yaffs_rm_chk(zteversionfilename);
				cmd_yaffs_umount(productinfopoint);

				if (ret < 0)
					g_prevstatus = NAND_SYSTEM_ERROR;
				else
					g_prevstatus = NAND_SUCCESS;
			}
#endif

		} else if (phy_partition.offset == NAND_NOTUSED_ADDRESS) {
			ret = NAND_SUCCESS;
		} else {
			ret = log2phy_table(&phy_partition);
			phy_partition_info(phy_partition, __LINE__);
			if (NAND_SUCCESS == ret)
				ret = nand_erase_partition(phy_partition.offset, phy_partition.size);

			if (NAND_SUCCESS == ret)
				set_dl_op_val(addr, size, ERASEFLASH, SUCCESS, 1);
		}
	}
	
	/*printf("Dl_Erase_Address.cnt = 0x%08x  Dl_Data_Address.cnt = 0x%08x\n", Dl_Erase_Address.cnt, Dl_Data_Address.cnt);
	for (cnt = 0; cnt < Dl_Erase_Address.cnt; cnt ++)
		printf("Dl_Erase_Address_Table[%d] = 0x%08x\n", cnt, Dl_Erase_Address.base[cnt]);
	for (cnt = 0; cnt < Dl_Data_Address.cnt; cnt ++)
		printf("Dl_Data_Address_Table[%d] = 0x%08x\n", cnt, Dl_Data_Address.base[cnt]);*/

	if ((is_factorydownload_tools == 1) && (is_check_dlstatus == 1) && (Dl_Erase_Address.cnt > 0)) {
		if (addr == Dl_Erase_Address.base[Dl_Erase_Address.cnt - 1]) {
			printf("\nSave dload status into dlstatus.txt\n");
			dl_op_buf_len = DL_OP_RECORD_LEN * (g_dl_op_index + 1);
			if (dl_op_buf_len > 0x2000) {
				printf("dload status is too long and does not save it.\n");
			} else {
				memset(g_PhasecheckBUF, 0, 0x2000);
				for (cnt = 0; cnt <= g_dl_op_index; cnt++)
					sprintf((g_PhasecheckBUF + cnt * DL_OP_RECORD_LEN), 
					"{%02d Base:0x%08x Size:0x%08x Op:%s Status:%s Scnt:0x%08x}", 
					cnt, 
					g_dl_op_table[cnt].base, 
					g_dl_op_table[cnt].size, 
					Dl_Op_Type_Name[g_dl_op_table[cnt].type], 
					Dl_Op_Status_Name[g_dl_op_table[cnt].status], 
					g_dl_op_table[cnt].status_cnt);
				/* printf("%s\n", g_PhasecheckBUF); the line will result in dead here, so mask it */
				/* write dload_status to yaffs2 format */
				char *productinfopoint = "/productinfo";
				char *productinfofilename = "/productinfo/dlstatus.txt";
				cmd_yaffs_mount(productinfopoint);
 				cmd_yaffs_mwrite_file(productinfofilename, g_PhasecheckBUF, dl_op_buf_len);
				cmd_yaffs_ls_chk(productinfofilename);
				cmd_yaffs_umount(productinfopoint);
			}

			/* check factorydownload status */
			printf("\nCheck dload status\n");
			for (cnt = 0; cnt <= g_dl_op_index; cnt++)
				printf("%02d Base:0x%08x Size:0x%08x Op:%d Status:%d Scnt:0x%08x\n", 
				cnt, 
				g_dl_op_table[cnt].base, 
				g_dl_op_table[cnt].size, 
				g_dl_op_table[cnt].type, 
				g_dl_op_table[cnt].status, 
				g_dl_op_table[cnt].status_cnt);
		
			dl_data_status = FAIL; 
			dl_erase_status = FAIL;
			for (cnt = 0; cnt < Dl_Data_Address.cnt; cnt++) {
				dl_data_status = check_dl_data_status(Dl_Data_Address.base[cnt]);
				if (dl_data_status == FAIL) {
					printf("check address:0x%08x download status error\n", Dl_Data_Address.base[cnt]);
					break;
				}
			}

			for (cnt = 0; cnt < Dl_Erase_Address.cnt; cnt++) {
				dl_erase_status = check_dl_erase_status(Dl_Erase_Address.base[cnt]);
				if (dl_erase_status == FAIL) {
			   		printf("check address:0x%08x erase status error\n", Dl_Erase_Address.base[cnt]);
					break;
				}
			}
		
			if ((dl_data_status == SUCCESS) && (dl_erase_status == SUCCESS))
				ret = NAND_SUCCESS;
			else
				ret = NAND_SYSTEM_ERROR;
		} //if (addr == Dl_Erase_Address.base[Dl_Erase_Address.cnt - 1])
	} //if ((is_factorydownload_tools == 1) && (Dl_Erase_Address.cnt > 0))

    FDL2_SendRep (ret);
    return (NAND_SUCCESS == ret);
}

int FDL2_FormatFlash (PACKET_T *pakcet, void *arg)
{
    int ret = nand_format();
    FDL2_SendRep (ret);
    return (NAND_SUCCESS == ret);
}
