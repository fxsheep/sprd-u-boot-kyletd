#include "sci_types.h"
#include "fdl_conf.h"
#ifdef CONFIG_EMMC_BOOT
#include "card_sdio.h"
#include "dload_op.h"
#include "flash_command.h"
#include "fdl_emmc.h"
#include "packet.h"
#include "fdl_crc.h"
#include "fdl_stdio.h"

#include "parsemtdparts.h"
#include "asm/arch/sci_types.h"
#include "asm/arch/nand_controller.h"
#include <linux/mtd/mtd.h>
#include <nand.h>
#include <linux/mtd/nand.h>
#include <jffs2/jffs2.h>
#include <malloc.h>
#include <ext_common.h>
#include <ext4fs.h>


//#define NULL_EFI_AND_EXT4
#ifndef NULL_EFI_AND_EXT4
#include "../../../include/ext4fs.h"
#endif

#define EFI_SECTOR_SIZE 	512
#define ERASE_SECTOR_SIZE		(64*1024/EFI_SECTOR_SIZE)
typedef struct DL_EMMC_STATUS_TAG
{
	uint32 part_total_size ;
	uint32 base_sector;
	uint32 curUserPartition;
	uint8 isLastPakFlag ;
	uint8 curEMMCArea ;
} DL_EMMC_STATUS;

static DL_EMMC_STATUS g_dl_eMMCStatus = {0, 0, 0xffffffff, 0, 0};

//#define CHECKSUM_OTHER_DATA       0x5555aaaa
static unsigned long g_checksum;
static unsigned long g_sram_addr;
static int is_nv_flag = 0;
static int read_nv_flag = 0;
static int read_bkupnv_flag = 0;
//static int read_dlstatus_flag = 0;
static int is_ProdInfo_flag = 0;
static unsigned long is_factorydownload_flag = 0;
static int read_prod_info_flag = 0;

#define EMMC_UNDATA_WRITE
#ifdef EMMC_UNDATA_WRITE
#define EMMC_BUF_SIZE	(2*1024*1024)
//#define EMMC_BUF_SIZE		(64*1024)
#else
#define EMMC_BUF_SIZE		(64*1024)
#endif
#define EMMC_FIXNV_SIZE		(64 * 1024)
#define EMMC_PROD_INFO_SIZE	(3 * 1024)
//#define TRANS_CODE_SIZE		(12 * 1024) /* dloadtools optimization value */

__align(32) unsigned char g_eMMCBuf[EMMC_BUF_SIZE + 4];
unsigned char g_fix_nv_buf[EMMC_FIXNV_SIZE + 4];
unsigned char g_fixbucknv_buf[EMMC_FIXNV_SIZE + 4];
unsigned char g_prod_info_buf[EMMC_PROD_INFO_SIZE + 4];

typedef struct DL_FILE_STATUS_TAG
{
	unsigned long   total_size;
	unsigned long   total_recv_size;
	unsigned long   unsave_recv_size;
} DL_EMMC_FILE_STATUS;

static DL_EMMC_FILE_STATUS g_status;
static int g_prevstatus;

static __inline void FDL2_eMMC_SendRep (unsigned long err)
{
	FDL_SendAckPacket (convert_err (err));
}

typedef struct ADDR_TO_PART_TAG
{
	unsigned long   custom;
	unsigned long   partition;
} ADDR_TO_PART;

int FDL_BootIsEMMC(void)
{
//	return gpio_get_value(EMMC_SELECT_GPIO);
	return 1;
}

#if 0
unsigned long Dl_Data_Address_Table[] = {
0x90000001,
0x80000000,
0x80000001,
0x80000003,
0x80000004,
0x80000007,
0x80000009,
0x8000000a,
0x8000000b,
0x8000000c,
0x8000000f,
0x80000010,
0x90000002
};

unsigned long Dl_Erase_Address_Table[] = {
0x90000003,
0x8000000d,
0x8000000e	/* the last ddress must be the last erase partition logical address in factorydownload tool */
};
#endif

#define MAX_PARTITION_REQ_FORMAT	5
unsigned long g_partRequestFormat[MAX_PARTITION_REQ_FORMAT + 1] = {
	PARTITION_FIX_NV,
	PARTITION_BACK_NV,
	PARTITION_RUNTIME_NV,
	PARTITION_PROD_INFO,
	PARTITION_CACHE	,
	PARTITON_NULL
};


#define PARTITION_SPL_LOADER	MAX_PARTITION_INFO+0
#define PARTITION_UBOOT		MAX_PARTITION_INFO+1
static ADDR_TO_PART g_eMMC_Addr2Part_Table[] = {
	{0x80000000, PARTITION_SPL_LOADER}, 
	{0x80000001, PARTITION_UBOOT}, 
	{0x80000003, PARTITION_VM}, 
	{0x80000004, PARTITION_MODEM}, 
	{0x80000007, PARTITION_DSP}, 
	{0x80000009, PARTITION_KERNEL}, 
	{0x8000000b, PARTITION_SYSTEM}, 
	{0x8000000c, PARTITION_USER_DAT}, 
	{0x8000000d, PARTITION_CACHE}, 
	{0x8000000f,  PARTITION_LOGO},
	{0x90000001, PARTITION_FIX_NV}, 
	{0x90000003, PARTITION_RUNTIME_NV}, 
//	{0x90000002, PARTITION_PHASE_CHECK},
	{0xffffffff, 0xffffffff}
};

unsigned long addr2part(unsigned long custom)
{
	unsigned long idx, log = 0xffffffff;

//	if ((custom & 0xf0000000) == 0x80000000)
//		return custom;
	for (idx = 0; g_eMMC_Addr2Part_Table[idx].custom != 0xffffffff; idx ++) {
		if (g_eMMC_Addr2Part_Table[idx].custom == custom) {
			log = g_eMMC_Addr2Part_Table[idx].partition;
			break;
		}
	}
	
	return log;
}

extern PARTITION_CFG g_sprd_emmc_partition_cfg[];
PARTITION_CFG uefi_part_info[MAX_PARTITION_INFO];
static int uefi_part_info_ok_flag = 0;
int uefi_get_part_info(void)
{
	block_dev_desc_t *dev_desc = NULL;
	disk_partition_t info;
	int i;

	if(uefi_part_info_ok_flag)
		return 1;

	dev_desc = get_dev("mmc", 1);	//From cmd_ext2.c line175
	if (dev_desc==NULL) {
		return 0;
	}
	for(i=0; i<MAX_PARTITION_INFO; i++){
		if(g_sprd_emmc_partition_cfg[i].partition_index == 0)
			break;
		if (get_partition_info (dev_desc, g_sprd_emmc_partition_cfg[i].partition_index, &info)) {
			return 0;
		}
		if(info.size <= 0 )
			return 0;
		uefi_part_info[i].partition_index = g_sprd_emmc_partition_cfg[i].partition_index;
		uefi_part_info[i].partition_size= info.size;
		uefi_part_info[i].partition_index= info.start;			
	}

	uefi_part_info_ok_flag = 1;
	return 1;
}

unsigned long efi_covert_index(unsigned long npart)
{
	uint i;

	for(i=0; i<MAX_PARTITION_INFO; i++){
		if(g_sprd_emmc_partition_cfg[i].partition_index == npart)
			return i;
	}
	return MAX_PARTITION_INFO;
}

unsigned long efi_GetPartBaseSec(unsigned long Partition)
{
#ifndef NULL_EFI_AND_EXT4
	#if 0
	block_dev_desc_t *dev_desc = NULL;
	disk_partition_t info;

	dev_desc = get_dev("mmc", 1);	//From cmd_ext2.c line175
	if (dev_desc==NULL) {
		return 0;
	}
	if (get_partition_info (dev_desc, Partition, &info)) {
		return 0;
	}
	return info.start;
	#else
	uefi_get_part_info();
	return uefi_part_info[efi_covert_index(Partition)].partition_index;
	#endif
#else
	return 0;	
#endif
}

unsigned long efi_GetPartSize(unsigned long Partition)
{
#ifndef NULL_EFI_AND_EXT4
	#if 0
	block_dev_desc_t *dev_desc = NULL;
	disk_partition_t info;

	dev_desc = get_dev("mmc", 1);	//From cmd_ext2.c line175
	if (dev_desc==NULL) {
		return 0;
	}
	if (get_partition_info (dev_desc, Partition, &info)) {
		return 0;
	}

	return (EFI_SECTOR_SIZE * info.size);	
	#else
	uefi_get_part_info();
	return (EFI_SECTOR_SIZE * uefi_part_info[efi_covert_index(Partition)].partition_size);
	#endif
#else
	if(Partition == PARTITION_SYSTEM)
		return 256*1024*1024;
	else
		return 16*1024*1024;
#endif
}

int FDL_Check_Partition_Table(void)
{
#ifndef NULL_EFI_AND_EXT4
	#if 0
	block_dev_desc_t *dev_desc = NULL;
	disk_partition_t info;
	int i;

	dev_desc = get_dev("mmc", 1);	//From cmd_ext2.c line175
	if (dev_desc==NULL) {
		return 0;
	}
	for(i=0; i<MAX_PARTITION_INFO; i++){
		if(g_sprd_emmc_partition_cfg[i].partition_index == 0)
			break;
		if(MAX_SIZE_FLAG == g_sprd_emmc_partition_cfg[i].partition_size)
			continue;
		if (get_partition_info (dev_desc, g_sprd_emmc_partition_cfg[i].partition_index, &info)) {
			return 0;
		}
		if(2 * g_sprd_emmc_partition_cfg[i].partition_size !=  info.size)
			return 0;
	}
	return 1;
	#else
	int i;
	uefi_part_info_ok_flag = 0;
	if(!uefi_get_part_info())
		return 0;
	for(i=0; i<MAX_PARTITION_INFO; i++){
		if(g_sprd_emmc_partition_cfg[i].partition_index == 0)
			break;
		if(MAX_SIZE_FLAG == g_sprd_emmc_partition_cfg[i].partition_size)
			continue;
		if(2 * g_sprd_emmc_partition_cfg[i].partition_size !=  uefi_part_info[i].partition_size)
			return 0;
	}
	return 1;
	#endif
#else
	return 1;
#endif
}

int eMMCFormatParttion(EFI_PARTITION_INDEX part)
{	
#ifndef NULL_EFI_AND_EXT4
	if(ext4fs_format("mmc", 1, part) == -1)
		return 0;
	else
		return 1;
#else
	return 1;
#endif
}

int eMMCPreFormatRequestExt4FS(void)
{
	int i;
	for(i=0; i<MAX_PARTITION_REQ_FORMAT; i++){
		if(0 == g_partRequestFormat[i])
			break;
		if(!eMMCFormatParttion(g_partRequestFormat[i]))
			return 0;
	}
	return 1;
}

int emmc_erase_partition(EFI_PARTITION_INDEX part, int fastEraseFlag)
{
	unsigned long i, count, len,  base_sector;
	uint8 curArea;
	if(PARTITION_SPL_LOADER == part)
		curArea = PARTITION_BOOT1;
	else if(PARTITION_UBOOT == part)
		curArea = PARTITION_BOOT2;
	else if (part >= MAX_PARTITION_INFO)
		return 0;
	else curArea = PARTITION_USER;
	
	len = efi_GetPartSize(part);
	len = len/EFI_SECTOR_SIZE;
	base_sector = efi_GetPartBaseSec(part);

	if(fastEraseFlag){
		memset(g_eMMCBuf, 0xff, ERASE_SECTOR_SIZE*EFI_SECTOR_SIZE);		
		if(!Emmc_Write(curArea, base_sector,  ERASE_SECTOR_SIZE, (unsigned char *) g_eMMCBuf))
			 return 0;
	}else{
		count = len/(EMMC_BUF_SIZE/EFI_SECTOR_SIZE);
		memset(g_eMMCBuf, 0xff, EMMC_BUF_SIZE);
		for(i=0; i<count; i++){
			if(!Emmc_Write(curArea, base_sector + i * EMMC_BUF_SIZE/EFI_SECTOR_SIZE,  EMMC_BUF_SIZE/EFI_SECTOR_SIZE, (unsigned char *) g_eMMCBuf))
				 return 0;
		}
		count = len%(EMMC_BUF_SIZE/EFI_SECTOR_SIZE);		
		if(count){
			if(!Emmc_Write(curArea, base_sector + i * EMMC_BUF_SIZE/EFI_SECTOR_SIZE,  count, (unsigned char *) g_eMMCBuf))
				 return 0;			
		}	
	}
	return 1;
}

int emmc_erase_allflash(void)
{
	int i, count;
	
	memset(g_eMMCBuf, 0xff, ERASE_SECTOR_SIZE*EFI_SECTOR_SIZE);		

	for(i=0; i<MAX_PARTITION_INFO; i++){
		if(g_sprd_emmc_partition_cfg[i].partition_index == 0)
			break;
		if(!emmc_erase_partition(g_sprd_emmc_partition_cfg[i].partition_index, 1))
			return 0;
	}
	if(!Emmc_Write(PARTITION_USER, 0,  ERASE_SECTOR_SIZE, (unsigned char *) g_eMMCBuf))
		 return 0;
	
	count = Emmc_GetCapacity(PARTITION_BOOT1);
	count = count/ERASE_SECTOR_SIZE;
	for(i=0; i<count; i++){	
		if(!Emmc_Write(PARTITION_BOOT1, i*ERASE_SECTOR_SIZE,  ERASE_SECTOR_SIZE, (unsigned char *) g_eMMCBuf))
			 return 0;
	}
	
	count = Emmc_GetCapacity(PARTITION_BOOT2);
	count = count/ERASE_SECTOR_SIZE;
	for(i=0; i<count; i++){	
		if(!Emmc_Write(PARTITION_BOOT2, i*ERASE_SECTOR_SIZE,  ERASE_SECTOR_SIZE, (unsigned char *) g_eMMCBuf))
			 return 0;
	}

	return 1;
}

int eMMCReadExt4ffs(EFI_PARTITION_INDEX part, const char *filename, unsigned int len, unsigned char *buf)
{
#ifndef NULL_EFI_AND_EXT4
	{
	char *interface = "mmc";
	int filelen;
	if (ext4fs_mount(interface, 1, part) == -1) {
		goto fail;
	}

	filelen = ext4fs_open(filename);
	if (filelen < 0) {
		goto fail;
	}
	if(filelen == len)
	{
		if (ext4fs_read((char*)buf, filelen) != filelen) {
			goto fail;
		}
	}
	else
		goto fail;
	ext4fs_close();
	return 1;
fail:
	ext4fs_close();
	return 0;
}
#else
	return 1;
#endif
}

int eMMCWriteExt4ffs(EFI_PARTITION_INDEX part, const char *filename, unsigned int len, unsigned char *buf)
{
#ifndef NULL_EFI_AND_EXT4
	char *interface = "mmc";
	if (ext4fs_mount(interface, 1, part) == -1) {
		goto fail;
	}

	
	if (ext4fs_write(filename, buf, len) == -1){
		goto fail;
	}
	ext4fs_close();
	return 1;
fail:
	ext4fs_close();
	return 0;	
#else
	return 1;
#endif
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


#define MAGIC_DATA	0xAA55A5A5
#define SPL_CHECKSUM_LEN	0x6000
#define CHECKSUM_START_OFFSET	0x28

#define MAGIC_DATA_SAVE_OFFSET	(0x20/4)
#define CHECKSUM_SAVE_OFFSET	(0x24/4)

unsigned short eMMCCheckSum(const unsigned int *src, int len)
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

void splFillCheckData(unsigned int * splBuf,  int len)
{
	*(splBuf + MAGIC_DATA_SAVE_OFFSET) = MAGIC_DATA;
	*(splBuf + CHECKSUM_SAVE_OFFSET) = (unsigned int)eMMCCheckSum((unsigned int *)&splBuf[CHECKSUM_START_OFFSET/4], SPL_CHECKSUM_LEN - CHECKSUM_START_OFFSET);
//	*(splBuf + CHECKSUM_SAVE_OFFSET) = splCheckSum(splBuf);
}

//#define LOAD_UBOOT_FOR_TEST
#ifdef LOAD_UBOOT_FOR_TEST
#define UBOOT_RUN_ADDR 0x000F00000
#define UBOOT_MAX_LOAD_SIZE 128*1024	//64K, 128K, 256K, 512K

void load_uBoot2Sdram_forTest(uint32 loadSize)
{
	uint32 i, nCount, nSectorCount;
	uint8* pLoadAddr = (uint8*) UBOOT_RUN_ADDR;
	void(*JumpFunPtr)(void);

#if 0
	nCount = loadSize/UBOOT_MAX_LOAD_SIZE;
	for(i=0; i<nCount; i++)
	{
		Emmc_Read(PARTITION_BOOT2, i*UBOOT_MAX_LOAD_SIZE/EFI_SECTOR_SIZE, UBOOT_MAX_LOAD_SIZE/EFI_SECTOR_SIZE, (uint8*)(pLoadAddr + i*UBOOT_MAX_LOAD_SIZE));
	}
	nSectorCount = (loadSize - nCount*UBOOT_MAX_LOAD_SIZE)/EFI_SECTOR_SIZE + 1;
	Emmc_Read(PARTITION_BOOT2, nCount*UBOOT_MAX_LOAD_SIZE/EFI_SECTOR_SIZE, nSectorCount, (uint8*)(pLoadAddr + nCount*UBOOT_MAX_LOAD_SIZE));
#else
	nSectorCount = loadSize/EFI_SECTOR_SIZE + 1;
	Emmc_Read(PARTITION_BOOT2, 0, nSectorCount, (uint8*)pLoadAddr);
#endif
	JumpFunPtr = (void(*)())UBOOT_RUN_ADDR;
	(*JumpFunPtr)();
//	*((void(*)())UBOOT_RUN_ADDR)();
}
#endif


int FDL2_eMMC_DataStart (PACKET_T *packet, void *arg)
{
	unsigned long *data = (unsigned long *) (packet->packet_body.content);
	unsigned long start_addr = *data;
	unsigned long size = * (data + 1);
#if defined(CHIP_ENDIAN_LITTLE)
	start_addr = EndianConv_32 (start_addr);
	size = EndianConv_32 (size);
#endif
	g_status.total_size  = size;

	g_dl_eMMCStatus.curUserPartition = addr2part(start_addr);
	if (PARTITION_FIX_NV == g_dl_eMMCStatus.curUserPartition)
	{
		if(packet->packet_body.size > 8){
		g_dl_eMMCStatus.curEMMCArea = PARTITION_USER;

		if ((g_dl_eMMCStatus.curUserPartition < 0) || (g_dl_eMMCStatus.curUserPartition >= MAX_PARTITION_INFO)){
			FDL2_eMMC_SendRep (EMMC_SYSTEM_ERROR);
			return 0;
		}

		g_dl_eMMCStatus.part_total_size = efi_GetPartSize(g_dl_eMMCStatus.curUserPartition);
		if ((size > g_dl_eMMCStatus.part_total_size) || (size > EMMC_FIXNV_SIZE)){
			FDL2_eMMC_SendRep (EMMC_INVALID_SIZE);
			return 0;
		}
		g_dl_eMMCStatus.base_sector = efi_GetPartBaseSec(g_dl_eMMCStatus.curUserPartition);
		
		is_nv_flag = 1;
		memset(g_eMMCBuf, 0xff, EMMC_FIXNV_SIZE + 4);
		g_checksum = * (data+2);
		g_sram_addr = (unsigned long)g_eMMCBuf;

		} else {
			is_nv_flag = 0;
			if (0 == (g_checksum & 0xffffff))
			{
			//The fixnv checksum is error.
			SEND_ERROR_RSP (BSL_EEROR_CHECKSUM); /*lint !e527*/
			}
		}
	}else if(PARTITION_PROD_INFO == g_dl_eMMCStatus.curUserPartition){
		g_dl_eMMCStatus.curEMMCArea = PARTITION_USER;

		if ((g_dl_eMMCStatus.curUserPartition < 0) || (g_dl_eMMCStatus.curUserPartition >= MAX_PARTITION_INFO)){
			FDL2_eMMC_SendRep (EMMC_SYSTEM_ERROR);
			return 0;
		}

		g_dl_eMMCStatus.part_total_size = efi_GetPartSize(g_dl_eMMCStatus.curUserPartition);
		if ((size > g_dl_eMMCStatus.part_total_size) || (size > EMMC_FIXNV_SIZE)){
			FDL2_eMMC_SendRep (EMMC_INVALID_SIZE);
			return 0;
		}
		g_dl_eMMCStatus.base_sector = efi_GetPartBaseSec(g_dl_eMMCStatus.curUserPartition);
		
		memset(g_eMMCBuf, 0xff, EMMC_PROD_INFO_SIZE);
		g_sram_addr = (unsigned long)g_eMMCBuf;	
		is_ProdInfo_flag = 1;
	}	else if(PARTITION_SPL_LOADER == g_dl_eMMCStatus.curUserPartition){
		g_dl_eMMCStatus.curEMMCArea = PARTITION_BOOT1;
		g_dl_eMMCStatus.part_total_size = EFI_SECTOR_SIZE * Emmc_GetCapacity(PARTITION_BOOT1);
		g_dl_eMMCStatus.base_sector =  0;
		memset(g_eMMCBuf, 0xff, SPL_CHECKSUM_LEN);
		g_dl_eMMCStatus.isLastPakFlag = 0;
#ifdef EMMC_UNDATA_WRITE
		memset(g_eMMCBuf, 0xff, 32*1024 + 4);
#endif
	}
	else if(PARTITION_UBOOT == g_dl_eMMCStatus.curUserPartition){
		g_dl_eMMCStatus.curEMMCArea = PARTITION_BOOT2;
		g_dl_eMMCStatus.part_total_size = EFI_SECTOR_SIZE * Emmc_GetCapacity(PARTITION_BOOT2);
		g_dl_eMMCStatus.base_sector =  0;
		g_dl_eMMCStatus.isLastPakFlag = 0;	
		if ((size > g_dl_eMMCStatus.part_total_size) /* ||(size > cardHandle->Boot2_Capacity)*/){
			FDL2_eMMC_SendRep (EMMC_INVALID_SIZE);
			return 0;
		}
		
	} else{
		g_dl_eMMCStatus.curEMMCArea = PARTITION_USER;

		if ((g_dl_eMMCStatus.curUserPartition < 0) || (g_dl_eMMCStatus.curUserPartition >= MAX_PARTITION_INFO)){
			//The fixnv checksum is error.
			SEND_ERROR_RSP (BSL_INCOMPATIBLE_PARTITION); /*lint !e527*/
			return 0;
		}
		g_dl_eMMCStatus.part_total_size = efi_GetPartSize(g_dl_eMMCStatus.curUserPartition);

		if (size > g_dl_eMMCStatus.part_total_size){
			FDL2_eMMC_SendRep (EMMC_INVALID_SIZE);
			return 0;
		}
		g_dl_eMMCStatus.base_sector = efi_GetPartBaseSec(g_dl_eMMCStatus.curUserPartition);
		g_dl_eMMCStatus.isLastPakFlag = 0;
	}
	
	g_status.total_recv_size   = 0;
	g_status.unsave_recv_size   = 0;
#ifdef EMMC_UNDATA_WRITE
	g_sram_addr = (unsigned long)g_eMMCBuf;
#endif
	g_prevstatus = EMMC_SUCCESS;
	//        set_dl_op_val(start_addr, size, STARTDATA, SUCCESS, 1);
	FDL_SendAckPacket (BSL_REP_ACK);
	return 1;
}

int FDL2_eMMC_DataMidst (PACKET_T *packet, void *arg)
{
	unsigned long size, lastSize, nSectorCount;
#ifndef EMMC_UNDATA_WRITE
	unsigned long nSectorOffset;
#endif
	/* The previous download step failed. */
	if ((EMMC_SUCCESS != g_prevstatus) || (g_dl_eMMCStatus.isLastPakFlag))
	{
		//	set_dl_op_val(0, 0, MIDSTDATA, FAIL, 1);
		FDL2_eMMC_SendRep (EMMC_SYSTEM_ERROR);
		return 0;
	}

	size = packet->packet_body.size;


	if ( (g_status.total_recv_size + size) > g_status.total_size)
	{
		g_prevstatus = NAND_INVALID_SIZE;
		//	set_dl_op_val(0, 0, MIDSTDATA, FAIL, 2);
		FDL2_eMMC_SendRep (g_prevstatus);
		return 0;
	}    
	g_status.total_recv_size += size;

  	 if (is_nv_flag || is_ProdInfo_flag)
	{
		memcpy ( (unsigned char *) g_sram_addr, (char *) (packet->packet_body.content), size);
		g_sram_addr += size;
		g_status.unsave_recv_size += size;
		//	        set_dl_op_val(0, 0, MIDSTDATA, SUCCESS, 8);
		FDL_SendAckPacket (BSL_REP_ACK);
		return 1;
   	 }else{
#ifndef EMMC_UNDATA_WRITE
	   nSectorOffset = g_status.unsave_recv_size/EFI_SECTOR_SIZE;
	   if(0 == (size%EFI_SECTOR_SIZE)){
	   	nSectorCount = size/EFI_SECTOR_SIZE;
	   }
	   else{
	   	nSectorCount = size/EFI_SECTOR_SIZE + 1;
		g_dl_eMMCStatus.isLastPakFlag = 1;
	   }
	   	
	       if(!Emmc_Write(g_dl_eMMCStatus.curEMMCArea, g_dl_eMMCStatus.base_sector + nSectorOffset,  2*nSectorCount, (unsigned char *) (packet->packet_body.content)))
	        {
		g_status.unsave_recv_size = 0;
		//The fixnv checksum is error.
		SEND_ERROR_RSP (BSL_WRITE_ERROR); /*lint !e527*/
		return 0;
	        }
	       g_status.unsave_recv_size = 0;
#else
	if(EMMC_BUF_SIZE >= (g_status.unsave_recv_size + size))
	{
		memcpy ( (unsigned char *) g_sram_addr, (char *) (packet->packet_body.content), size);
		g_sram_addr += size;
		if(EMMC_BUF_SIZE == (g_status.unsave_recv_size + size))
		{
			if(!Emmc_Write(g_dl_eMMCStatus.curEMMCArea, g_dl_eMMCStatus.base_sector,  EMMC_BUF_SIZE/EFI_SECTOR_SIZE, (unsigned char *) g_eMMCBuf))
			 {
			 	g_status.unsave_recv_size = 0;
				//The fixnv checksum is error.
				SEND_ERROR_RSP (BSL_WRITE_ERROR); /*lint !e527*/
				 return 0;
			}
			g_status.unsave_recv_size = 0;
			g_dl_eMMCStatus.base_sector += EMMC_BUF_SIZE/EFI_SECTOR_SIZE;
			g_sram_addr = (unsigned long)g_eMMCBuf;
		}else if(g_status.total_recv_size == g_status.total_size){
			g_status.unsave_recv_size  += size;
			if(g_status.unsave_recv_size  != 0){
				if(0 == (g_status.unsave_recv_size%EFI_SECTOR_SIZE)){
				     nSectorCount = g_status.unsave_recv_size/EFI_SECTOR_SIZE;
				}
				else{
				     nSectorCount = g_status.unsave_recv_size/EFI_SECTOR_SIZE + 1;
				}
				if(PARTITION_SPL_LOADER == g_dl_eMMCStatus.curUserPartition){
					if(g_status.total_recv_size < SPL_CHECKSUM_LEN)
						nSectorCount = SPL_CHECKSUM_LEN/EFI_SECTOR_SIZE;				
					splFillCheckData((unsigned int *) g_eMMCBuf, (int)g_status.total_recv_size);
				}			
				if(!Emmc_Write(g_dl_eMMCStatus.curEMMCArea, g_dl_eMMCStatus.base_sector,  nSectorCount, (unsigned char *) g_eMMCBuf))
				 {
					g_status.unsave_recv_size = 0;
					//The fixnv checksum is error.
					SEND_ERROR_RSP (BSL_WRITE_ERROR); /*lint !e527*/
					 return 0;
				}
				g_status.unsave_recv_size = 0;	 
			}
		}else g_status.unsave_recv_size += size;		
	}else{
		{	
			lastSize = EMMC_BUF_SIZE - g_status.unsave_recv_size;
			memcpy ( (unsigned char *) g_sram_addr, (char *) (packet->packet_body.content), lastSize);
			g_status.unsave_recv_size = EMMC_BUF_SIZE;		
			if(!Emmc_Write(g_dl_eMMCStatus.curEMMCArea, g_dl_eMMCStatus.base_sector,  EMMC_BUF_SIZE/EFI_SECTOR_SIZE, (unsigned char *) g_eMMCBuf))
			 {
			 	g_status.unsave_recv_size = 0;
				//The fixnv checksum is error.
				SEND_ERROR_RSP (BSL_WRITE_ERROR); /*lint !e527*/
				 return 0;
			}
			g_status.unsave_recv_size = size - lastSize;
			g_sram_addr = (unsigned long)g_eMMCBuf;		
			memcpy ( (unsigned char *) g_sram_addr, (char *) (&packet->packet_body.content[lastSize]), g_status.unsave_recv_size);		
			g_sram_addr += g_status.unsave_recv_size;
			g_dl_eMMCStatus.base_sector += EMMC_BUF_SIZE/EFI_SECTOR_SIZE;
		}
	}
#endif
//	        set_dl_op_val(0, 0, MIDSTDATA, FAIL, 4);
	 g_prevstatus = EMMC_SUCCESS;
	 FDL2_eMMC_SendRep (g_prevstatus);
	return  1;
    }    
}

int FDL2_eMMC_DataEnd (PACKET_T *packet, void *arg)
{
	unsigned long  fix_nv_checksum;
	char *fixnvfilename = "/fixnv/fixnv.bin";
	char *backupfixnvfilename =  "/backupfixnv/fixnv.bin";
	char *productinfofilename = "/productinfo/productinfo.bin";
	
	if(is_nv_flag){
	        	fix_nv_checksum = Get_CheckSum ( (unsigned char *) g_eMMCBuf, g_status.total_recv_size);
	        	fix_nv_checksum = EndianConv_32 (fix_nv_checksum);
		g_eMMCBuf[EMMC_FIXNV_SIZE + 0] = g_eMMCBuf[EMMC_FIXNV_SIZE + 1] = 0x5a;
		g_eMMCBuf[EMMC_FIXNV_SIZE + 2] = g_eMMCBuf[EMMC_FIXNV_SIZE + 3] = 0x5a;
			
	        	if (fix_nv_checksum != g_checksum){
	            		SEND_ERROR_RSP(BSL_CHECKSUM_DIFF);
			return 0;
	        	}
 		eMMCFormatParttion(PARTITION_FIX_NV);
		if(!eMMCWriteExt4ffs(PARTITION_FIX_NV, fixnvfilename, EMMC_FIXNV_SIZE + 4, (unsigned char *) g_eMMCBuf))
		{
			//The fixnv checksum is error.
			SEND_ERROR_RSP (BSL_WRITE_ERROR); /*lint !e527*/
			return 0;
		}
		eMMCFormatParttion(PARTITION_BACK_NV);
		if(!eMMCWriteExt4ffs(PARTITION_BACK_NV, backupfixnvfilename, EMMC_FIXNV_SIZE + 4, (unsigned char *) g_eMMCBuf))
		{
			//The fixnv checksum is error.
			SEND_ERROR_RSP (BSL_WRITE_ERROR); /*lint !e527*/
			return 0;
		}
		
	}else if(is_ProdInfo_flag){
		is_factorydownload_flag= 1;
		g_eMMCBuf[EMMC_PROD_INFO_SIZE + 0] = g_eMMCBuf[EMMC_PROD_INFO_SIZE + 1] = 0x5a;
		g_eMMCBuf[EMMC_PROD_INFO_SIZE + 2] = g_eMMCBuf[EMMC_PROD_INFO_SIZE + 3] = 0x5a;
			
 		eMMCFormatParttion(PARTITION_PROD_INFO);
		if(!eMMCWriteExt4ffs(PARTITION_PROD_INFO, productinfofilename, EMMC_PROD_INFO_SIZE + 4, (unsigned char *) g_eMMCBuf))
		{
			//The fixnv checksum is error.
			SEND_ERROR_RSP (BSL_WRITE_ERROR); /*lint !e527*/
			return 0;
		}
	
	}else{
#if 0//def EMMC_UNDATA_WRITE	
		if(g_status.unsave_recv_size != 0){
			if(0 == (g_status.unsave_recv_size%EFI_SECTOR_SIZE)){
			     nSectorCount = g_status.unsave_recv_size/EFI_SECTOR_SIZE;
			}
			else{
			     nSectorCount = g_status.unsave_recv_size/EFI_SECTOR_SIZE + 1;
			}
			if(!Emmc_Write(g_dl_eMMCStatus.curEMMCArea, g_dl_eMMCStatus.base_sector,  nSectorCount, (unsigned char *) g_eMMCBuf))
			 {
			 	g_status.unsave_recv_size = 0;
				//The fixnv checksum is error.
				SEND_ERROR_RSP (BSL_WRITE_ERROR); /*lint !e527*/
				 return 0;
			}
			g_status.unsave_recv_size = 0;   
		}
#endif
	}
	
//	set_dl_op_val(0, 0, ENDDATA, SUCCESS, 1);	
//	is_nv_flag = 0;
	g_dl_eMMCStatus.isLastPakFlag = 0;
	g_prevstatus = EMMC_SUCCESS;
    	FDL2_eMMC_SendRep (g_prevstatus);

#ifdef LOAD_UBOOT_FOR_TEST
	if(PARTITION_UBOOT == g_dl_eMMCStatus.curUserPartition)
		load_uBoot2Sdram_forTest(g_status.total_size);
#endif
	is_ProdInfo_flag = 0;
	is_nv_flag = 0;
	g_status.total_size  = 0;	
    	return 1;
}

static int read_nv_check = 0;
#define FIX_NV_IS_OK		1
#define FIX_BACKUP_NV_IS_OK	2

int FDL2_eMMC_Read (PACKET_T *packet, void *arg)
{
    	unsigned long nSectorCount, nSectorOffset;
    	unsigned long *data = (unsigned long *) (packet->packet_body.content);
    	unsigned long addr = *data;
    	unsigned long size = * (data + 1);
    	unsigned long off = * (data + 2);
    	int           ret = EMMC_SUCCESS;
	char *fixnvfilename = "/fixnv/fixnv.bin";
	char *backupfixnvfilename =  "/backupfixnv/fixnv.bin";
	char *productinfofilename = "/productinfo/productinfo.bin";

#if defined(CHIP_ENDIAN_LITTLE)
    	addr = EndianConv_32 (addr);
    	size = EndianConv_32 (size);
	off = EndianConv_32 (off);
#endif

	if (size > MAX_PKT_SIZE) {
		FDL_SendAckPacket (BSL_REP_DOWN_SIZE_ERROR);
		return 0;
	}

	g_dl_eMMCStatus.curUserPartition = addr2part(addr);
    	if (PARTITION_FIX_NV == g_dl_eMMCStatus.curUserPartition){
//	if(packet->packet_body.size == 12){
		g_dl_eMMCStatus.curEMMCArea = PARTITION_USER;

		if ((g_dl_eMMCStatus.curUserPartition < 0) || (g_dl_eMMCStatus.curUserPartition >= MAX_PARTITION_INFO)){
			FDL2_eMMC_SendRep (EMMC_SYSTEM_ERROR);
			return 0;
		}

		g_dl_eMMCStatus.part_total_size = efi_GetPartSize(g_dl_eMMCStatus.curUserPartition);
		if ((size > g_dl_eMMCStatus.part_total_size) || (size > EMMC_FIXNV_SIZE)){
			FDL2_eMMC_SendRep (EMMC_INVALID_SIZE);
			return 0;
		}
		g_dl_eMMCStatus.base_sector = efi_GetPartBaseSec(g_dl_eMMCStatus.curUserPartition);

		is_nv_flag = 1;
		is_ProdInfo_flag = 0;
//		memset(g_eMMCBuf, 0xff, EMMC_FIXNV_SIZE + 4);
//		}
	}else  if(PARTITION_SPL_LOADER == g_dl_eMMCStatus.curUserPartition){
		g_dl_eMMCStatus.curEMMCArea = PARTITION_BOOT1;
		g_dl_eMMCStatus.part_total_size = EFI_SECTOR_SIZE * Emmc_GetCapacity(PARTITION_BOOT1);
		g_dl_eMMCStatus.base_sector =  0;
		is_nv_flag = 0;
		is_ProdInfo_flag = 0;
	}
	else if(PARTITION_UBOOT == g_dl_eMMCStatus.curUserPartition){
		g_dl_eMMCStatus.curEMMCArea = PARTITION_BOOT2;
		g_dl_eMMCStatus.part_total_size = EFI_SECTOR_SIZE * Emmc_GetCapacity(PARTITION_BOOT2);
		g_dl_eMMCStatus.base_sector =  0;
		is_nv_flag = 0;
		is_ProdInfo_flag = 0;		
		if (size > g_dl_eMMCStatus.part_total_size){
			FDL2_eMMC_SendRep (EMMC_INVALID_SIZE);
			return 0;
		}		
	}else if(PARTITION_PROD_INFO == g_dl_eMMCStatus.curUserPartition){
		g_dl_eMMCStatus.curEMMCArea = PARTITION_USER;

		if ((g_dl_eMMCStatus.curUserPartition < 0) || (g_dl_eMMCStatus.curUserPartition >= MAX_PARTITION_INFO)){
			FDL2_eMMC_SendRep (EMMC_SYSTEM_ERROR);
			return 0;
		}

		g_dl_eMMCStatus.part_total_size = efi_GetPartSize(g_dl_eMMCStatus.curUserPartition);
		if ((size > g_dl_eMMCStatus.part_total_size) || (size > EMMC_PROD_INFO_SIZE)){
			FDL2_eMMC_SendRep (EMMC_INVALID_SIZE);
			return 0;
		}
		g_dl_eMMCStatus.base_sector = efi_GetPartBaseSec(g_dl_eMMCStatus.curUserPartition);

		is_ProdInfo_flag = 1;
		is_nv_flag = 0;
//		memset(g_eMMCBuf, 0xff, EMMC_PROD_INFO_SIZE + 4);

	}else{
		g_dl_eMMCStatus.curEMMCArea = PARTITION_USER;

		if ((g_dl_eMMCStatus.curUserPartition < 0) || (g_dl_eMMCStatus.curUserPartition >= MAX_PARTITION_INFO)){
			FDL2_eMMC_SendRep (EMMC_SYSTEM_ERROR);
			return 0;
		}

		g_dl_eMMCStatus.part_total_size = efi_GetPartSize(g_dl_eMMCStatus.curUserPartition);
		if (size > g_dl_eMMCStatus.part_total_size){
			FDL2_eMMC_SendRep (EMMC_INVALID_SIZE);
			return 0;
		}
		g_dl_eMMCStatus.base_sector = efi_GetPartBaseSec(g_dl_eMMCStatus.curUserPartition);
		g_dl_eMMCStatus.isLastPakFlag = 0;
		is_nv_flag = 0;
		is_ProdInfo_flag = 0;		
	  }		

	if(is_nv_flag){
		if(read_nv_flag == 0){
			memset(g_fix_nv_buf, 0xff, EMMC_FIXNV_SIZE + 4);
			if(!eMMCReadExt4ffs(PARTITION_FIX_NV, fixnvfilename, EMMC_FIXNV_SIZE + 4, (unsigned char *) g_fix_nv_buf))
			{
				//The fixnv checksum is error.
				SEND_ERROR_RSP (BSL_WRITE_ERROR); /*lint !e527*/				
				return 0;
			}
			read_nv_flag = 1;
			read_nv_check = 0;
			if(eMMC_nv_is_correct(g_fix_nv_buf, EMMC_FIXNV_SIZE))
				read_nv_check= FIX_NV_IS_OK;				
		}
		if(read_bkupnv_flag == 0){
			memset(g_fixbucknv_buf, 0xff, EMMC_FIXNV_SIZE + 4);			
			if(!eMMCReadExt4ffs(PARTITION_BACK_NV, backupfixnvfilename, EMMC_FIXNV_SIZE + 4, (unsigned char *) g_fixbucknv_buf))
			{
				//The fixnv checksum is error.
				SEND_ERROR_RSP (BSL_WRITE_ERROR); /*lint !e527*/				
				return 0;
			}
			read_bkupnv_flag = 1;
			read_nv_check = 0;
			if(eMMC_nv_is_correct(g_fix_nv_buf, EMMC_FIXNV_SIZE))
				read_nv_check= FIX_NV_IS_OK;				
		}
		ret = EMMC_SUCCESS;
		if(read_nv_check == FIX_NV_IS_OK)
			memcpy(packet->packet_body.content, (unsigned char *)(g_fix_nv_buf + off), size);
		else if(read_nv_check == FIX_BACKUP_NV_IS_OK)
			memcpy(packet->packet_body.content, (unsigned char *)(g_fixbucknv_buf + off), size);
		else{
			SEND_ERROR_RSP (BSL_EEROR_CHECKSUM); /*lint !e527*/				
			return 0;
		}		
	}else if(is_ProdInfo_flag){
		if(read_prod_info_flag == 0){
			memset(g_prod_info_buf, 0xff, EMMC_PROD_INFO_SIZE + 4); 		
			if(!eMMCReadExt4ffs(PARTITION_PROD_INFO, productinfofilename, EMMC_PROD_INFO_SIZE + 4, (unsigned char *) g_prod_info_buf))
			{
				//The fixnv checksum is error.
				SEND_ERROR_RSP (BSL_WRITE_ERROR); /*lint !e527*/				
				return 0;
			}
		}
		read_prod_info_flag = 1;
		if(!eMMC_nv_is_correct(g_prod_info_buf, EMMC_PROD_INFO_SIZE))
		{
			//The fixnv checksum is error.
			SEND_ERROR_RSP (BSL_EEROR_CHECKSUM); /*lint !e527*/				
			return 0;
		}
		memcpy(packet->packet_body.content, (unsigned char *)(g_prod_info_buf + off), size);
		ret = EMMC_SUCCESS;
	}else{
		if(0 == (size%EFI_SECTOR_SIZE)){
			 nSectorCount = size/EFI_SECTOR_SIZE;
		}
		else{
			 nSectorCount = size/EFI_SECTOR_SIZE + 1;
		}		
		nSectorOffset = off/EFI_SECTOR_SIZE;	
		if(Emmc_Read(g_dl_eMMCStatus.curEMMCArea, g_dl_eMMCStatus.base_sector + nSectorOffset,  nSectorCount, (unsigned char *) (packet->packet_body.content)))
		{
			ret = EMMC_SUCCESS;
		}else{
			ret = EMMC_SYSTEM_ERROR;
		}
	}
	
/*
    	if (packet->packet_body.size > 8)
		off = EndianConv_32 (* (data + 2));



	if ((strcmp(phy_partition.name, "fixnv") == 0) || (strcmp(phy_partition.name, "productinfo") == 0))
		ret = nand_read_fdl_yaffs(&phy_partition, off, size, (unsigned char *)(packet->packet_body.content));
    	else if ((strcmp(phy_partition.name, "spl") == 0) || (strcmp(phy_partition.name, "2ndbl") == 0))
    		ret = nand_read_fdl(&phy_partition, off, size, (unsigned char *)(packet->packet_body.content));
    	else
		ret = EMMC_INVALID_ADDR;
*/

    	if (EMMC_SUCCESS == ret) {
	        	packet->packet_body.type = BSL_REP_READ_FLASH;
	        	packet->packet_body.size = size;
	        	FDL_SendPacket (packet);
	        	ret = TRUE;
    	} else {
        		FDL2_eMMC_SendRep (ret);
        		ret = FALSE;
    	}	
	return ret;
}

int FDL2_eMMC_Erase (PACKET_T *packet, void *arg)
{
	unsigned long *data = (unsigned long *) (packet->packet_body.content);
	unsigned long addr = *data;
	unsigned long size = * (data + 1);
	int           ret = EMMC_SUCCESS;

	addr = EndianConv_32 (addr);
	size = EndianConv_32 (size);

	if ((addr == 0) && (size = 0xffffffff)) {
		printf("Scrub to erase all of flash\n");
		if(!emmc_erase_allflash())
		{
			SEND_ERROR_RSP (BSL_WRITE_ERROR);			
			return 0;
		}
		ret = NAND_SUCCESS;
	} else {
		g_dl_eMMCStatus.curUserPartition = addr2part(addr);
		if(!emmc_erase_partition(g_dl_eMMCStatus.curUserPartition, 0))
		{
			SEND_ERROR_RSP (BSL_WRITE_ERROR);			
			return 0;
		}
#if 1
		if(PARTITION_RUNTIME_NV == g_dl_eMMCStatus.curUserPartition){
			if(!eMMCFormatParttion(PARTITION_RUNTIME_NV)){
				SEND_ERROR_RSP (BSL_WRITE_ERROR);			
				return 0;
			}
		}

		if(PARTITION_CACHE == g_dl_eMMCStatus.curUserPartition){
			if(!eMMCFormatParttion(PARTITION_CACHE)){
				SEND_ERROR_RSP (BSL_WRITE_ERROR);			
				return 0;
			}
		}

#endif
		ret = NAND_SUCCESS;
	}

	FDL2_eMMC_SendRep (EMMC_SUCCESS);

	return 1;
}

int FDL2_eMMC_Repartition (PACKET_T *pakcet, void *arg)
{
#ifndef NULL_EFI_AND_EXT4
	int i;
	for(i=0; i<3; i++){
		write_uefi_parition_table(g_sprd_emmc_partition_cfg);
		if(!FDL_Check_Partition_Table())
			continue;
		if(eMMCPreFormatRequestExt4FS())
			break;
	}
	if(i < 3){
		FDL2_eMMC_SendRep (EMMC_SUCCESS);
		return 1;
	}else{
		FDL2_eMMC_SendRep (EMMC_SYSTEM_ERROR);
		return 0;
	}
#else
	return 1 ;
#endif
}
#endif

