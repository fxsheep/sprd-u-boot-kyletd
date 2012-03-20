//#include "emmc.h"

#ifndef _DISK_PART_CFG_H
#define _DISK_PART_CFG_H

#include "part_uefi.h"

/**
 *
 *	devide wrapped interface
 *
**/
unsigned int _get_device_info(unsigned int *total_sector_number);
unsigned int _read_block(unsigned int start_sector,unsigned int sector_number,unsigned char *buf);
unsigned int _write_block(unsigned int start_sector,unsigned int sector_number,unsigned char *buf);

/**
 *
 *	partition device relative
 *
**/
//typedef unsigned int (*PART_DEVICE_ID)(unsigned int *id);
typedef unsigned int (*PART_DEVICE_INFO)(unsigned int *total_sector_number);
typedef unsigned int (*PART_READ)(unsigned int start_sector,unsigned int sector_number,unsigned char *buf);
typedef unsigned int (*PART_WRITE)(unsigned int start_sector,unsigned int sector_number,unsigned char *buf);

typedef struct _PART_DEVICE_IO
{
	//PART_DEVICE_ID _get_id;
	PART_DEVICE_INFO _info;
	PART_READ	_read;
	PART_WRITE	_write;
} __attribute__ ((packed)) PART_DEVICE_IO,*PPART_DEVICE_IO;

typedef struct _PART_DEVICE
{
	//unsigned int device_id;
	unsigned int total_sector;
	PART_DEVICE_IO *_device_io;
} __attribute__ ((packed)) PART_DEVICE,*PPART_DEVICE;

/**
 *
 *	partition relative
 *
**/

typedef struct _PARTITION_INFO
{
	unsigned int partition_index;
	unsigned int partition_start_sector;
	unsigned int partition_total_sector;
	unsigned int partition_attr;	//must to check by filesystem interface
} __attribute__ ((packed)) PARTITION_INFO,*PPARTITION_INFO;


typedef struct _PARTITION_TABLE
{
	unsigned int total_partition;
	PARTITION_INFO _partition_info[MAX_PARTITION_INFO]; 
} __attribute__ ((packed)) PARTITION_TABLE,*PPARTITION_TABLE;

extern PART_DEVICE emmc_part_device;

#endif //_DISK_PART_CFG_H
