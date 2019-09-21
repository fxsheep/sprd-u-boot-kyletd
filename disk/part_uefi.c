
//uefi patition can see http://en.wikipedia.org/wiki/GUID_Partition_Table for more information

#include <common.h>
#include "part_efi.h"
#include "part_uefi.h"
#include "part_cfg.h"


wchar_t default_name[]={L"NONAME"};

//big end and little end will different

//#define WORDS_BIGENDIAN

/**
 *
 *	swap function
 *
**/
#define _GET_BYTE(x, n)		( ((x) >> (8 * (n))) & 0xff )

#define _PED_SWAP16(x)		( (_GET_BYTE(x, 0) << 8)	\
				+ (_GET_BYTE(x, 1) << 0) )

#define _PED_SWAP32(x)		( (_GET_BYTE(x, 0) << 24)	\
				+ (_GET_BYTE(x, 1) << 16)	\
				+ (_GET_BYTE(x, 2) << 8)	\
				+ (_GET_BYTE(x, 3) << 0) )

#define _PED_SWAP64(x)		( (_GET_BYTE(x, 0) << 56)	\
				+ (_GET_BYTE(x, 1) << 48)	\
				+ (_GET_BYTE(x, 2) << 40)	\
				+ (_GET_BYTE(x, 3) << 32)	\
				+ (_GET_BYTE(x, 4) << 24)	\
				+ (_GET_BYTE(x, 5) << 16)	\
				+ (_GET_BYTE(x, 6) << 8)	\
				+ (_GET_BYTE(x, 7) << 0) )

#define PED_SWAP16(x)		((unsigned short int) _PED_SWAP16( (unsigned short int) (x) ))
#define PED_SWAP32(x)		((unsigned long int) _PED_SWAP32( (unsigned long int) (x) ))
#define PED_SWAP64(x)		((unsigned long long int) _PED_SWAP64( (unsigned long long int) (x) ))

/**
 *
 *	big endian or not
 *
**/
#ifdef WORDS_BIGENDIAN

#define PED_CPU_TO_LE16(x)	PED_SWAP16(x)
#define PED_CPU_TO_BE16(x)	(x)
#define PED_CPU_TO_LE32(x)	PED_SWAP32(x)
#define PED_CPU_TO_BE32(x)	(x)
#define PED_CPU_TO_LE64(x)	PED_SWAP64(x)
#define PED_CPU_TO_BE64(x)	(x)

#define PED_LE16_TO_CPU(x)	PED_SWAP16(x)
#define PED_BE16_TO_CPU(x)	(x)
#define PED_LE32_TO_CPU(x)	PED_SWAP32(x)
#define PED_BE32_TO_CPU(x)	(x)
#define PED_LE64_TO_CPU(x)	PED_SWAP64(x)
#define PED_BE64_TO_CPU(x)	(x)

#else /* !WORDS_BIGENDIAN */

#define PED_CPU_TO_LE16(x)	(x)
#define PED_CPU_TO_BE16(x)	PED_SWAP16(x)
#define PED_CPU_TO_LE32(x)	(x)
#define PED_CPU_TO_BE32(x)	PED_SWAP32(x)
#define PED_CPU_TO_LE64(x)	(x)
#define PED_CPU_TO_BE64(x)	PED_SWAP64(x)

#define PED_LE16_TO_CPU(x)	(x)
#define PED_BE16_TO_CPU(x)	PED_SWAP16(x)
#define PED_LE32_TO_CPU(x)	(x)
#define PED_BE32_TO_CPU(x)	PED_SWAP32(x)
#define PED_LE64_TO_CPU(x)	(x)
#define PED_BE64_TO_CPU(x)	PED_SWAP64(x)

#endif /* !WORDS_BIGENDIAN */

typedef struct _gpt_entry_block
{
	gpt_entry _gpt_entry[MAX_PARTITION_INFO];
} __attribute__ ((packed)) gpt_entry_block;

gpt_entry_block g_gpt_entry_block = {0};


typedef struct _align_pmbr
{
	unsigned char dummy[2];
	legacy_mbr pmbr;
} __attribute__ ((packed)) align_pmbr;


align_pmbr pmbr __attribute__ ((aligned(4))) = {0};
gpt_header gpt_head __attribute__ ((aligned(4))) = {0};

unsigned long long int _cur_lba_num = 0; 
unsigned long long int all_used_size = 0;

PARTITION_CFG g_partition_cfg[]={
	{PARTITION_VM, 512, PARTITION_RAW},		/* 512KB */
	{PARTITION_MODEM, 10 * 1024, PARTITION_RAW},	/* 10 * 1024KB */
	{PARTITION_DSP, 5 * 1024, PARTITION_RAW},
	{PARTITION_FIX_NV1, 3840, PARTITION_RAW},
	{PARTITION_FIX_NV2, 3840, PARTITION_RAW},
	{PARTITION_RUNTIME_NV1, 3840, PARTITION_RAW},
	{PARTITION_RUNTIME_NV2, 3840, PARTITION_RAW},
	{PARTITION_PROD_INFO1, 3840, PARTITION_RAW},
	{PARTITION_PROD_INFO2, 3840, PARTITION_RAW},
	{PARTITION_PROD_INFO3, 5 * 1024, PARTITION_RAW}, /* must be large or equal than 5MB */
	{PARTITION_EFS, 10 * 1024, PARTITION_RAW},
	{PARTITION_KERNEL, 10 * 1024, PARTITION_RAW},
	{PARTITION_LOGO, 1 * 1024, PARTITION_RAW},
	{PARTITION_RECOVERY, 10 * 1024, PARTITION_RAW},
	{PARTITION_FASTBOOT_LOGO, 1 * 1024, PARTITION_RAW},
	{PARTITION_MISC, 256, PARTITION_RAW},
	{PARTITION_SYSTEM, 768 * 1024, PARTITION_RAW},
	{PARTITION_CACHE, 20 * 1024, PARTITION_RAW},
	{PARTITION_USER_DAT, MAX_SIZE_FLAG, PARTITION_RAW},
	{0,0,0}
};


/* Convert char[2] in little endian format to the host format integer
 */
static inline unsigned short le16_to_int(unsigned char *le16)
{
	return ((le16[1] << 8) + le16[0]);
}

/* Convert char[4] in little endian format to the host format integer
 */
static inline unsigned long le32_to_int(unsigned char *le32)
{
	return ((le32[3] << 24) + (le32[2] << 16) + (le32[1] << 8) + le32[0]);
}

/* Convert char[8] in little endian format to the host format integer
 */
static inline unsigned long long le64_to_int(unsigned char *le64)
{
	return (((unsigned long long)le64[7] << 56) +
		((unsigned long long)le64[6] << 48) +
		((unsigned long long)le64[5] << 40) +
		((unsigned long long)le64[4] << 32) +
		((unsigned long long)le64[3] << 24) +
		((unsigned long long)le64[2] << 16) +
		((unsigned long long)le64[1] << 8) +
		(unsigned long long)le64[0]);
}
/**
 *
 *	CRC32
 *
**/
static inline unsigned long uefi_crc32(const void *buf, unsigned long len)
{
	return crc32(0, buf, len);
}


/**
 *
 *	some internal function for partition
 *
**/
int _gen_pmbr_part(struct partition *part)
{
	part->boot_ind = 0x80;
	part->sys_ind = EFI_PMBR_OSTYPE_EFI_GPT;
	part->sector = 1;
	part->end_head = 0xFF;
	part->end_sector = 0xFF;
	part->end_cyl = 0xFF;

	*(unsigned long int*) part->start_sect = PED_CPU_TO_LE32(1UL);

	//must get device sector number
	if ( emmc_part_device.total_sector > 0xFFFFFFF)
	{
		*(unsigned long int*) part->nr_sects = PED_CPU_TO_LE32(0xFFFFFFE);
	}
	else
	{
		*(unsigned long int*) part->nr_sects = PED_CPU_TO_LE32(emmc_part_device.total_sector);		
	}
       return 1;
}

int _gen_pmbr(legacy_mbr *pmbr)
{	
	memset(pmbr->partition_record, 0, sizeof(pmbr->partition_record));
	
	*(unsigned short int*)pmbr->signature = PED_CPU_TO_LE16(MSDOS_MBR_SIGNATURE);
	_gen_pmbr_part(&pmbr->partition_record[0]);

	return 1;
}

unsigned int _write_pmbr(legacy_mbr *pmbr)
{
	//write ro disk
	emmc_part_device._device_io->_write(0,1,(unsigned char *)pmbr);
       return 1;
}

efi_guid_t _gen_guid(int part_index)
{
	efi_guid_t new_guid = EFI_GUID(0x11111111,0x2222,0x3333,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0+part_index);
	return new_guid;
}

unsigned int _gen_gpt_entry(int part_index , gpt_entry *g_entry,PARTITION_CFG *p_partition_cfg)
{
	int i = 0;

	//maybe
	if( PARTITION_RAW == p_partition_cfg[part_index].partition_attr)
		g_entry->partition_type_guid = PARTITION_BASIC_DATA_GUID;
	else if( PARTITION_EXT4 == p_partition_cfg[part_index].partition_attr)
		g_entry->partition_type_guid = PARTITION_BASIC_DATA_GUID;
	else
		g_entry->partition_type_guid = PARTITION_BASIC_DATA_GUID;
	
	//gen guid
	g_entry->unique_partition_guid =_gen_guid(part_index+1) ;
	
	*(unsigned long long int*)g_entry->starting_lba = _cur_lba_num;
	
	//judge if user data partition
	if( MAX_SIZE_FLAG == p_partition_cfg[part_index].partition_size)
	{
		_cur_lba_num = _cur_lba_num + (emmc_part_device.total_sector-2-MAX_PARTITION_INFO/4-MAX_PARTITION_INFO/4-1-all_used_size*2);
		*(unsigned long long int*)g_entry->ending_lba = _cur_lba_num-1;
	}else
	{
		_cur_lba_num = _cur_lba_num + p_partition_cfg[part_index].partition_size * 2;
		*(unsigned long long int*)g_entry->ending_lba = _cur_lba_num - 1;
	}
	

	memset (&g_entry->attributes, 0, sizeof (gpt_entry_attributes));

  	//for (i = 0; i < 72 / sizeof (efi_char16_t); i++)
	for(i=0;i<sizeof(default_name)/2;i++)
    		g_entry->partition_name[i] = (efi_char16_t) PED_CPU_TO_LE16 ((unsigned short int*) default_name[i]);


	return 1;
}

unsigned int _parser_cfg(unsigned int *total_partition,PARTITION_CFG *p_partition_cfg)
{
	int count = 0;
	for(count=0;count<MAX_PARTITION_INFO;count++)
	{	
		if(0 == p_partition_cfg[count].partition_size)
		{			
			*total_partition = count;
			break;
		} else
		{
			if( MAX_SIZE_FLAG !=  p_partition_cfg[count].partition_size)
				all_used_size += p_partition_cfg[count].partition_size;		
		}
	}
	return 1;
}

unsigned int _gen_gpt(gpt_header *g_header,PARTITION_CFG *p_partition_cfg)
{
	int i = 0 ,gpt_partition_number = 0;
	unsigned long crc = 0;
	_parser_cfg(&gpt_partition_number,p_partition_cfg);
	
	*(unsigned long long int*)g_header->signature = PED_CPU_TO_LE64 (GPT_HEADER_SIGNATURE);

	*(unsigned long int*)g_header->revision = PED_CPU_TO_LE32 (GPT_HEADER_REVISION_V1);

	*(unsigned long int*)g_header->header_size = PED_CPU_TO_LE32(92UL);
	
	*(unsigned long int*)g_header->reserved1 = PED_CPU_TO_LE32(0);

	*(unsigned long long int*)g_header->my_lba = PED_CPU_TO_LE64 (1);

	*(unsigned long long int*)g_header->alternate_lba = PED_CPU_TO_LE64 (emmc_part_device.total_sector-1);
	
	*(unsigned long long int*)g_header->first_usable_lba = PED_CPU_TO_LE64 (MAX_PARTITION_INFO/4 + 2);
	
	*(unsigned long long int*)g_header->last_usable_lba = PED_CPU_TO_LE64 (emmc_part_device.total_sector-2-MAX_PARTITION_INFO/4);

	g_header->disk_guid = _gen_guid(0);

	*(unsigned long long int*)g_header->partition_entry_lba = PED_CPU_TO_LE64 (2);	
	
	*(unsigned long int*)g_header->num_partition_entries = PED_CPU_TO_LE32(gpt_partition_number);

	*(unsigned long int*)g_header->sizeof_partition_entry = PED_CPU_TO_LE32(sizeof(gpt_entry));
	
	memset(g_header->reserved2,0,GPT_BLOCK_SIZE - 92);	

	_cur_lba_num = MAX_PARTITION_INFO/4 + 2;

	for(i=0;i<gpt_partition_number;i++)
	{	
		_gen_gpt_entry(i,&g_gpt_entry_block._gpt_entry[i],p_partition_cfg);
	}
	
	//*(unsigned long int*)g_header->partition_entry_array_crc32 = ;
	crc = uefi_crc32(&g_gpt_entry_block,(le32_to_int(g_header->num_partition_entries)) *(le32_to_int( g_header->sizeof_partition_entry)));
	*(unsigned long int*)g_header->partition_entry_array_crc32 = PED_CPU_TO_LE32(crc);

	//CRC32 check
	crc = uefi_crc32(g_header,le32_to_int(g_header->header_size));
	*(unsigned long int*)g_header->header_crc32 = PED_CPU_TO_LE32(crc);

	

	return 1;
}

unsigned int _gen_backup_gpt(gpt_header *g_header,PARTITION_CFG *p_partition_cfg)
{
	unsigned long crc = 0;
	*(unsigned long long int*)g_header->my_lba = PED_CPU_TO_LE64 (emmc_part_device.total_sector-1);
	*(unsigned long long int*)g_header->alternate_lba = PED_CPU_TO_LE64 (1);
	*(unsigned long long int*)g_header->partition_entry_lba = PED_CPU_TO_LE64 (emmc_part_device.total_sector-1-MAX_PARTITION_INFO/4);
	*(unsigned long int*)g_header->header_crc32 = 0;
	crc = uefi_crc32(g_header,le32_to_int(g_header->header_size));
	*(unsigned long int*)g_header->header_crc32 = PED_CPU_TO_LE32(crc);
       return 1;
}

unsigned int _write_gpt(gpt_header *g_header)
{
	//write gpt entry
	emmc_part_device._device_io->_write(1,1,(unsigned char*)g_header);
	//write gpt header
	emmc_part_device._device_io->_write(2,MAX_PARTITION_INFO/4,(unsigned char*)&g_gpt_entry_block);

	return 1;
}

unsigned int _write_backup_gpt(gpt_header *g_header)
{
	//write gpt entry

	emmc_part_device._device_io->_write((emmc_part_device.total_sector-1),1,(unsigned char*)g_header);
	//write gpt header
	emmc_part_device._device_io->_write((emmc_part_device.total_sector-1-MAX_PARTITION_INFO/4),MAX_PARTITION_INFO/4,(unsigned char*)&g_gpt_entry_block);

	return 1;
}

/**
 *
 *	UEFI partition interface
 *
**/
int ueft_mount()
{
       return 1;
}

int ueft_unmount()
{
	return 1;
}

unsigned int read_uefi_partition_table(PARTITION_TABLE *p_partition_table)
{
	return 1;
}

unsigned int write_uefi_parition_table(PARTITION_CFG *p_partition_cfg)
{
	//mount devices
	all_used_size = 0;
	memset(&gpt_head, 0x0, GPT_BLOCK_SIZE);
	emmc_part_device_init();

	//write pmbr
	_gen_pmbr(&pmbr.pmbr);
	_write_pmbr(&pmbr.pmbr);

	//write gpt
	if( NULL == p_partition_cfg )
		p_partition_cfg = &g_partition_cfg;
	_gen_gpt(&gpt_head,p_partition_cfg);
	_write_gpt(&gpt_head);

	//backup
	_gen_backup_gpt(&gpt_head,p_partition_cfg);
	_write_backup_gpt(&gpt_head);
	return 1;
}

unsigned int get_uefi_partition_info(int partition_index,PARTITION_INFO *p_partition_info)
{
	return 1;
}

/**
 *
 *	UEFI partition test unit
 *
**/
unsigned int uefi_unit_test(int test_case)
{
	int ret = 0;
	switch(test_case)
	{
	case 1:	//write partition test
		break;
	case 2: //read partition test call part_efi.c function
		break;  
	default:
		;//first write partition than read partition.
	}
	return ret;  
}
