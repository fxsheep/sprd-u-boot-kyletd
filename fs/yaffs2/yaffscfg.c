/*
 * YAFFS: Yet Another Flash File System. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2007 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * yaffscfg.c  The configuration for the "direct" use of yaffs.
 *
 * This file is intended to be modified to your requirements.
 * There is no need to redistribute this file.
 */

/* XXX U-BOOT XXX */
#include <common.h>

#include <config.h>
#include "nand.h"
#include "yaffscfg.h"
#include "yaffsfs.h"
#include "yaffs_packedtags2.h"
#include "yaffs_mtdif.h"
#include "yaffs_mtdif2.h"
#include "parsemtdparts.h"
#if 0
#include <errno.h>
#else
#include "malloc.h"
#endif

unsigned yaffs_traceMask = 0x0; /* Disable logging */
//unsigned yaffs_traceMask = 0xFFFFFFFF; /* Enable logging */
static int yaffs_errno = 0;

void yaffsfs_SetError(int err)
{
	//Do whatever to set error
	yaffs_errno = err;
}

int yaffsfs_GetError(void)
{
	return yaffs_errno;
}

void yaffsfs_Lock(void)
{
}

void yaffsfs_Unlock(void)
{
}

__u32 yaffsfs_CurrentTime(void)
{
	return 0;
}

void *yaffs_malloc(size_t size)
{
	return malloc(size);
}

void yaffs_free(void *ptr)
{
	free(ptr);
}

void yaffsfs_LocalInitialisation(void)
{
	// Define locking semaphore.
}

// Configuration for:
// /ram  2MB ramdisk
// /boot 2MB boot disk (flash)
// /flash 14MB flash disk (flash)
// NB Though /boot and /flash occupy the same physical device they
// are still disticnt "yaffs_Devices. You may think of these as "partitions"
// using non-overlapping areas in the same device.
//

#include "yaffs_ramdisk.h"
#include "yaffs_flashif.h"

static int isMounted = 0;
#define MOUNT_POINT "/flash"

#define SPRD_MOUNT_PARTITION	1
#ifdef  SPRD_MOUNT_PARTITION
#define MOUNT_POINT1 "/backupfixnv"
#define MOUNT_POINT2 "/runtimenv"
#define MOUNT_POINT3 "/productinfo"
#define MOUNT_POINT4 "/fixnv"
#define MOUNT_POINT5 "/cache"
#endif

extern nand_info_t nand_info[];

/* XXX U-BOOT XXX */
#if 0
static yaffs_Device ramDev;
static yaffs_Device bootDev;
static yaffs_Device flashDev;
#endif

static yaffsfs_DeviceConfiguration yaffsfs_config[] = {
/* XXX U-BOOT XXX */
#if 0
	{ "/ram", &ramDev},
	{ "/boot", &bootDev},
	{ "/flash", &flashDev},
#else
	{ MOUNT_POINT, 0},
#ifdef  SPRD_MOUNT_PARTITION
	{ MOUNT_POINT1, 0},
	{ MOUNT_POINT2, 0},
	{ MOUNT_POINT3, 0},
	{ MOUNT_POINT4, 0},
	{ MOUNT_POINT5, 0},
#endif
#endif
	{(void *)0,(void *)0}
};

static void yaffs_dump_dev(yaffs_Device * dev)
{
	printf("\n\n");
	printf("startBlock......... %d\n", dev->startBlock);
	printf("endBlock........... %d\n", dev->endBlock);
	//printf("totalBytesPerChunk. %d\n", dev->totalBytesPerChunk);
	printf("nDataBytesPerChunk. %d\n", dev->nDataBytesPerChunk);
	printf("chunkGroupBits..... %d\n", dev->chunkGroupBits);
	printf("chunkGroupSize..... %d\n", dev->chunkGroupSize);
	printf("nErasedBlocks...... %d\n", dev->nErasedBlocks);
	printf("nReservedBlocks.... %d\n", dev->nReservedBlocks);
	printf("blocksInCheckpoint. %d\n", dev->blocksInCheckpoint);
	printf("nTnodesCreated..... %d\n", dev->nTnodesCreated);
	printf("nFreeTnodes........ %d\n", dev->nFreeTnodes);
	printf("nObjectsCreated.... %d\n", dev->nObjectsCreated);
	printf("nFreeObjects....... %d\n", dev->nFreeObjects);
	printf("nFreeChunks........ %d\n", dev->nFreeChunks);
	printf("nPageWrites........ %d\n", dev->nPageWrites);
	printf("nPageReads......... %d\n", dev->nPageReads);
	printf("nBlockErasures..... %d\n", dev->nBlockErasures);
	printf("nGCCopies.......... %d\n", dev->nGCCopies);
	printf("garbageCollections. %d\n", dev->garbageCollections);
	printf("passiveGCs......... %d\n", dev->passiveGarbageCollections);
	printf("nRetriedWrites..... %d\n", dev->nRetriedWrites);
	printf("nShortOpCaches..... %d\n", dev->nShortOpCaches);
	printf("nRetireBlocks...... %d\n", dev->nRetiredBlocks);
	printf("eccFixed........... %d\n", dev->eccFixed);
	printf("eccUnfixed......... %d\n", dev->eccUnfixed);
	printf("tagsEccFixed....... %d\n", dev->tagsEccFixed);
	printf("tagsEccUnfixed..... %d\n", dev->tagsEccUnfixed);
	printf("cacheHits.......... %d\n", dev->cacheHits);
	printf("nDeletedFiles...... %d\n", dev->nDeletedFiles);
	printf("nUnlinkedFiles..... %d\n", dev->nUnlinkedFiles);
	printf("nBackgroudDeletions %d\n", dev->nBackgroundDeletions);
	printf("useNANDECC......... %d\n", dev->useNANDECC);
	printf("isYaffs2........... %d\n", dev->isYaffs2);
	//printf("inbandTags......... %d\n", dev->inbandTags);
	printf("\n\n");
}


int yaffs_StartUp(void)
{
	struct mtd_info *mtd = &nand_info[0];
	int yaffsVersion = 2;
	int nBlocks;
	struct mtd_partition cur_partition;
	int aaa;
	char partname[255];

	yaffs_Device *flashDev = calloc(1, sizeof(yaffs_Device));
#ifdef  SPRD_MOUNT_PARTITION
	yaffs_Device *backupfixnvDev = calloc(1, sizeof(yaffs_Device));
	yaffs_Device *runtimenvDev = calloc(1, sizeof(yaffs_Device));
	yaffs_Device *productinfoDev = calloc(1, sizeof(yaffs_Device));
	yaffs_Device *fixnvDev = calloc(1, sizeof(yaffs_Device));
	yaffs_Device *dataDev= calloc(1, sizeof(yaffs_Device));
#endif

	yaffsfs_config[0].dev = flashDev;
#ifdef  SPRD_MOUNT_PARTITION
	yaffsfs_config[1].dev = backupfixnvDev;
	yaffsfs_config[2].dev = runtimenvDev;
	yaffsfs_config[3].dev = productinfoDev;
	yaffsfs_config[4].dev = fixnvDev;
	yaffsfs_config[5].dev = dataDev;
#endif

	/* store the mtd device for later use */
	flashDev->genericDevice = mtd;
#ifdef  SPRD_MOUNT_PARTITION
	backupfixnvDev->genericDevice = mtd;
	runtimenvDev->genericDevice = mtd;
	productinfoDev->genericDevice = mtd;
	fixnvDev->genericDevice = mtd;
	dataDev->genericDevice = mtd;
#endif

	// Stuff to configure YAFFS
	// Stuff to initialise anything special (eg lock semaphore).
	yaffsfs_LocalInitialisation();

	// Set up devices

/* XXX U-BOOT XXX */
#if 0
	// /ram
	ramDev.nBytesPerChunk = 512;
	ramDev.nChunksPerBlock = 32;
	ramDev.nReservedBlocks = 2; // Set this smaller for RAM
	ramDev.startBlock = 1; // Can't use block 0
	ramDev.endBlock = 127; // Last block in 2MB.
	ramDev.useNANDECC = 1;
	ramDev.nShortOpCaches = 0;	// Disable caching on this device.
	ramDev.genericDevice = (void *) 0;	// Used to identify the device in fstat.
	ramDev.writeChunkWithTagsToNAND = yramdisk_WriteChunkWithTagsToNAND;
	ramDev.readChunkWithTagsFromNAND = yramdisk_ReadChunkWithTagsFromNAND;
	ramDev.eraseBlockInNAND = yramdisk_EraseBlockInNAND;
	ramDev.initialiseNAND = yramdisk_InitialiseNAND;

	// /boot
	bootDev.nBytesPerChunk = 612;
	bootDev.nChunksPerBlock = 32;
	bootDev.nReservedBlocks = 5;
	bootDev.startBlock = 1; // Can't use block 0
	bootDev.endBlock = 127; // Last block in 2MB.
	bootDev.useNANDECC = 0; // use YAFFS's ECC
	bootDev.nShortOpCaches = 10; // Use caches
	bootDev.genericDevice = (void *) 1;	// Used to identify the device in fstat.
	bootDev.writeChunkToNAND = yflash_WriteChunkToNAND;
	bootDev.readChunkFromNAND = yflash_ReadChunkFromNAND;
	bootDev.eraseBlockInNAND = yflash_EraseBlockInNAND;
	bootDev.initialiseNAND = yflash_InitialiseNAND;
#endif

		// /flash
	flashDev->nReservedBlocks = 5;
	//flashDev->nShortOpCaches = (options.no_cache) ? 0 : 10;
	flashDev->nShortOpCaches = 10; // Use caches
	flashDev->useNANDECC = 0; // use YAFFS's ECC
	if (yaffsVersion == 2)
	{
		flashDev->writeChunkWithTagsToNAND = nandmtd2_WriteChunkWithTagsToNAND;
		flashDev->readChunkWithTagsFromNAND = nandmtd2_ReadChunkWithTagsFromNAND;
		flashDev->markNANDBlockBad = nandmtd2_MarkNANDBlockBad;
		flashDev->queryNANDBlock = nandmtd2_QueryNANDBlock;
		flashDev->spareBuffer = YMALLOC(mtd->oobsize);
		flashDev->isYaffs2 = 1;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17))
		flashDev->nDataBytesPerChunk = mtd->writesize;
		flashDev->nChunksPerBlock = mtd->erasesize / mtd->writesize;
#else
		flashDev->nDataBytesPerChunk = mtd->oobblock;
		flashDev->nChunksPerBlock = mtd->erasesize / mtd->oobblock;
#endif
		nBlocks = mtd->size / mtd->erasesize;
		//printf("nBlocks = %d\n", nBlocks);

		flashDev->nCheckpointReservedBlocks = 10;

#if 0
		flashDev->startBlock = 0;
		flashDev->endBlock = nBlocks - 1;
#else
		flashDev->startBlock = 237;
		flashDev->endBlock = flashDev->startBlock + 800  - 1;	
#endif
		//printf("\n\nflash device from %d to %d\n\n", flashDev->startBlock, flashDev->endBlock);
	}
	else
	{
		flashDev->writeChunkToNAND = nandmtd_WriteChunkToNAND;
		flashDev->readChunkFromNAND = nandmtd_ReadChunkFromNAND;
		flashDev->isYaffs2 = 0;
		nBlocks = mtd->size / (YAFFS_CHUNKS_PER_BLOCK * YAFFS_BYTES_PER_CHUNK);
		flashDev->startBlock = 320;
		flashDev->endBlock = nBlocks - 1;
		flashDev->nChunksPerBlock = YAFFS_CHUNKS_PER_BLOCK;
		flashDev->nDataBytesPerChunk = YAFFS_BYTES_PER_CHUNK;
	}

	/* ... and common functions */
	flashDev->eraseBlockInNAND = nandmtd_EraseBlockInNAND;
	flashDev->initialiseNAND = nandmtd_InitialiseNAND;

#ifdef  SPRD_MOUNT_PARTITION
	// /backupfixnv
	backupfixnvDev->nReservedBlocks = 5;
	backupfixnvDev->nShortOpCaches = 10; // Use caches
	backupfixnvDev->useNANDECC = 1; // use YAFFS's ECC

	backupfixnvDev->skipCheckpointRead = 1;
	backupfixnvDev->skipCheckpointWrite = 1;

	if (yaffsVersion == 2)
	{
		backupfixnvDev->writeChunkWithTagsToNAND = nandmtd2_WriteChunkWithTagsToNAND;
		backupfixnvDev->readChunkWithTagsFromNAND = nandmtd2_ReadChunkWithTagsFromNAND;
		backupfixnvDev->markNANDBlockBad = nandmtd2_MarkNANDBlockBad;
		backupfixnvDev->queryNANDBlock = nandmtd2_QueryNANDBlock;
		backupfixnvDev->spareBuffer = YMALLOC(mtd->oobsize);
		backupfixnvDev->isYaffs2 = 1;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17))
		backupfixnvDev->nDataBytesPerChunk = mtd->writesize;
		backupfixnvDev->nChunksPerBlock = mtd->erasesize / mtd->writesize;
#else
		backupfixnvDev->nDataBytesPerChunk = mtd->oobblock;
		backupfixnvDev->nChunksPerBlock = mtd->erasesize / mtd->oobblock;
#endif
		nBlocks = mtd->size / mtd->erasesize;
		//printf("size = 0x%016Lx  erasesize = 0x%016Lx  nBlocks = %d\n", (unsigned long long)mtd->size, (unsigned long long)mtd->erasesize, nBlocks);

		backupfixnvDev->nCheckpointReservedBlocks = 0;
		
		memset(&cur_partition, 0 , sizeof(struct mtd_partition));
		memset(partname, 0, 255);
		strcpy(partname, MOUNT_POINT1);
		cur_partition.name = (char *)(partname + 1); /* skip '/' charater */
		cur_partition.offset = 0xffffffff;
		yaffs_parse_cmdline_partitions(&cur_partition, (unsigned long long)mtd->size);
		//printf("offset = 0x%08x  size = 0x%08x\n", cur_partition.offset, cur_partition.size);
		backupfixnvDev->startBlock = cur_partition.offset / mtd->erasesize;
		backupfixnvDev->endBlock = backupfixnvDev->startBlock + cur_partition.size / mtd->erasesize  - 1;
		//printf("\nbackupfixnv device from %d to %d\n", backupfixnvDev->startBlock, backupfixnvDev->endBlock);
	}
	else
	{
		backupfixnvDev->writeChunkToNAND = nandmtd_WriteChunkToNAND;
		backupfixnvDev->readChunkFromNAND = nandmtd_ReadChunkFromNAND;
		backupfixnvDev->isYaffs2 = 0;
		nBlocks = mtd->size / (YAFFS_CHUNKS_PER_BLOCK * YAFFS_BYTES_PER_CHUNK);
		backupfixnvDev->startBlock = 320;
		backupfixnvDev->endBlock = nBlocks - 1;
		backupfixnvDev->nChunksPerBlock = YAFFS_CHUNKS_PER_BLOCK;
		backupfixnvDev->nDataBytesPerChunk = YAFFS_BYTES_PER_CHUNK;
	}

	/* ... and common functions */
	backupfixnvDev->eraseBlockInNAND = nandmtd_EraseBlockInNAND;
	backupfixnvDev->initialiseNAND = nandmtd_InitialiseNAND;

	//yaffs_dump_dev(backupfixnvDev);


	// /runtimenv
	runtimenvDev->nReservedBlocks = 5;
	runtimenvDev->nShortOpCaches = 10; // Use caches
	runtimenvDev->useNANDECC = 1; // use YAFFS's ECC

	runtimenvDev->skipCheckpointRead = 1;
	runtimenvDev->skipCheckpointWrite = 1;

	if (yaffsVersion == 2)
	{
		runtimenvDev->writeChunkWithTagsToNAND = nandmtd2_WriteChunkWithTagsToNAND;
		runtimenvDev->readChunkWithTagsFromNAND = nandmtd2_ReadChunkWithTagsFromNAND;
		runtimenvDev->markNANDBlockBad = nandmtd2_MarkNANDBlockBad;
		runtimenvDev->queryNANDBlock = nandmtd2_QueryNANDBlock;
		runtimenvDev->spareBuffer = YMALLOC(mtd->oobsize);
		runtimenvDev->isYaffs2 = 1;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17))
		runtimenvDev->nDataBytesPerChunk = mtd->writesize;
		runtimenvDev->nChunksPerBlock = mtd->erasesize / mtd->writesize;
#else
		runtimenvDev->nDataBytesPerChunk = mtd->oobblock;
		runtimenvDev->nChunksPerBlock = mtd->erasesize / mtd->oobblock;
#endif
		nBlocks = mtd->size / mtd->erasesize;
		//printf("size = 0x%016Lx  erasesize = 0x%016Lx  nBlocks = %d\n", (unsigned long long)mtd->size, (unsigned long long)mtd->erasesize, nBlocks);

		runtimenvDev->nCheckpointReservedBlocks = 0;
		
		memset(&cur_partition, 0 , sizeof(struct mtd_partition));
		memset(partname, 0, 255);
		strcpy(partname, MOUNT_POINT2);
		cur_partition.name = (char *)(partname + 1); /* skip '/' charater */
		cur_partition.offset = 0xffffffff;
		yaffs_parse_cmdline_partitions(&cur_partition, (unsigned long long)mtd->size);
		//printf("offset = 0x%08x  size = 0x%08x\n", cur_partition.offset, cur_partition.size);
		runtimenvDev->startBlock = cur_partition.offset / mtd->erasesize;
		runtimenvDev->endBlock = runtimenvDev->startBlock + cur_partition.size / mtd->erasesize  - 1;
		//printf("\nruntimenv device from %d to %d\n", runtimenvDev->startBlock, runtimenvDev->endBlock);
	}
	else
	{
		runtimenvDev->writeChunkToNAND = nandmtd_WriteChunkToNAND;
		runtimenvDev->readChunkFromNAND = nandmtd_ReadChunkFromNAND;
		runtimenvDev->isYaffs2 = 0;
		nBlocks = mtd->size / (YAFFS_CHUNKS_PER_BLOCK * YAFFS_BYTES_PER_CHUNK);
		runtimenvDev->startBlock = 320;
		runtimenvDev->endBlock = nBlocks - 1;
		runtimenvDev->nChunksPerBlock = YAFFS_CHUNKS_PER_BLOCK;
		runtimenvDev->nDataBytesPerChunk = YAFFS_BYTES_PER_CHUNK;
	}

	/* ... and common functions */
	runtimenvDev->eraseBlockInNAND = nandmtd_EraseBlockInNAND;
	runtimenvDev->initialiseNAND = nandmtd_InitialiseNAND;

	//yaffs_dump_dev(runtimenvDev);


	// /productinfo
	productinfoDev->nReservedBlocks = 5;
	productinfoDev->nShortOpCaches = 10; // Use caches
	productinfoDev->useNANDECC = 1; // use YAFFS's ECC

	productinfoDev->skipCheckpointRead = 1;
	productinfoDev->skipCheckpointWrite = 1;

	if (yaffsVersion == 2)
	{
		productinfoDev->writeChunkWithTagsToNAND = nandmtd2_WriteChunkWithTagsToNAND;
		productinfoDev->readChunkWithTagsFromNAND = nandmtd2_ReadChunkWithTagsFromNAND;
		productinfoDev->markNANDBlockBad = nandmtd2_MarkNANDBlockBad;
		productinfoDev->queryNANDBlock = nandmtd2_QueryNANDBlock;
		productinfoDev->spareBuffer = YMALLOC(mtd->oobsize);
		productinfoDev->isYaffs2 = 1;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17))
		productinfoDev->nDataBytesPerChunk = mtd->writesize;
		productinfoDev->nChunksPerBlock = mtd->erasesize / mtd->writesize;
#else
		productinfoDev->nDataBytesPerChunk = mtd->oobblock;
		productinfoDev->nChunksPerBlock = mtd->erasesize / mtd->oobblock;
#endif
		nBlocks = mtd->size / mtd->erasesize;
		//printf("size = 0x%016Lx  erasesize = 0x%016Lx  nBlocks = %d\n", (unsigned long long)mtd->size, (unsigned long long)mtd->erasesize, nBlocks);

		productinfoDev->nCheckpointReservedBlocks = 0;
		
		memset(&cur_partition, 0 , sizeof(struct mtd_partition));
		memset(partname, 0, 255);
		strcpy(partname, MOUNT_POINT3);
		cur_partition.name = (char *)(partname + 1); /* skip '/' charater */
		cur_partition.offset = 0xffffffff;
		yaffs_parse_cmdline_partitions(&cur_partition, (unsigned long long)mtd->size);
		//printf("offset = 0x%08x  size = 0x%08x\n", cur_partition.offset, cur_partition.size);
		productinfoDev->startBlock = cur_partition.offset / mtd->erasesize;
		productinfoDev->endBlock = productinfoDev->startBlock + cur_partition.size / mtd->erasesize  - 1;
		//printf("\productinfo device from %d to %d\n", productinfoDev->startBlock, productinfoDev->endBlock);
	}
	else
	{
		productinfoDev->writeChunkToNAND = nandmtd_WriteChunkToNAND;
		productinfoDev->readChunkFromNAND = nandmtd_ReadChunkFromNAND;
		productinfoDev->isYaffs2 = 0;
		nBlocks = mtd->size / (YAFFS_CHUNKS_PER_BLOCK * YAFFS_BYTES_PER_CHUNK);
		productinfoDev->startBlock = 320;
		productinfoDev->endBlock = nBlocks - 1;
		productinfoDev->nChunksPerBlock = YAFFS_CHUNKS_PER_BLOCK;
		productinfoDev->nDataBytesPerChunk = YAFFS_BYTES_PER_CHUNK;
	}

	/* ... and common functions */
	productinfoDev->eraseBlockInNAND = nandmtd_EraseBlockInNAND;
	productinfoDev->initialiseNAND = nandmtd_InitialiseNAND;

	//yaffs_dump_dev(productinfoDev);

	// /fixnv
	fixnvDev->nReservedBlocks = 5;
	fixnvDev->nShortOpCaches = 10; // Use caches
	fixnvDev->useNANDECC = 1; // use YAFFS's ECC

	fixnvDev->skipCheckpointRead = 1;
	fixnvDev->skipCheckpointWrite = 1;

	if (yaffsVersion == 2)
	{
		fixnvDev->writeChunkWithTagsToNAND = nandmtd2_WriteChunkWithTagsToNAND;
		fixnvDev->readChunkWithTagsFromNAND = nandmtd2_ReadChunkWithTagsFromNAND;
		fixnvDev->markNANDBlockBad = nandmtd2_MarkNANDBlockBad;
		fixnvDev->queryNANDBlock = nandmtd2_QueryNANDBlock;
		fixnvDev->spareBuffer = YMALLOC(mtd->oobsize);
		fixnvDev->isYaffs2 = 1;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17))
		fixnvDev->nDataBytesPerChunk = mtd->writesize;
		fixnvDev->nChunksPerBlock = mtd->erasesize / mtd->writesize;
#else
		fixnvDev->nDataBytesPerChunk = mtd->oobblock;
		fixnvDev->nChunksPerBlock = mtd->erasesize / mtd->oobblock;
#endif
		nBlocks = mtd->size / mtd->erasesize;
		//printf("size = 0x%016Lx  erasesize = 0x%016Lx  nBlocks = %d\n", (unsigned long long)mtd->size, (unsigned long long)mtd->erasesize, nBlocks);

		fixnvDev->nCheckpointReservedBlocks = 0;
		
		memset(&cur_partition, 0 , sizeof(struct mtd_partition));
		memset(partname, 0, 255);
		strcpy(partname, MOUNT_POINT4);
		cur_partition.name = (char *)(partname + 1); /* skip '/' charater */
		cur_partition.offset = 0xffffffff;
		yaffs_parse_cmdline_partitions(&cur_partition, (unsigned long long)mtd->size);
		//printf("offset = 0x%08x  size = 0x%08x\n", cur_partition.offset, cur_partition.size);
		fixnvDev->startBlock = cur_partition.offset / mtd->erasesize;
		fixnvDev->endBlock = fixnvDev->startBlock + cur_partition.size / mtd->erasesize  - 1;
		//printf("fixnv device from %d to %d\n", fixnvDev->startBlock, fixnvDev->endBlock);
	}
	else
	{
		fixnvDev->writeChunkToNAND = nandmtd_WriteChunkToNAND;
		fixnvDev->readChunkFromNAND = nandmtd_ReadChunkFromNAND;
		fixnvDev->isYaffs2 = 0;
		nBlocks = mtd->size / (YAFFS_CHUNKS_PER_BLOCK * YAFFS_BYTES_PER_CHUNK);
		fixnvDev->startBlock = 320;
		fixnvDev->endBlock = nBlocks - 1;
		fixnvDev->nChunksPerBlock = YAFFS_CHUNKS_PER_BLOCK;
		fixnvDev->nDataBytesPerChunk = YAFFS_BYTES_PER_CHUNK;
	}

	/* ... and common functions */
	fixnvDev->eraseBlockInNAND = nandmtd_EraseBlockInNAND;
	fixnvDev->initialiseNAND = nandmtd_InitialiseNAND;

	//yaffs_dump_dev(fixnvDev);

	//data partition 
	dataDev->nReservedBlocks = 5;
	dataDev->nShortOpCaches = 10; // Use caches
	dataDev->useNANDECC = 1; // use YAFFS's ECC

	dataDev->skipCheckpointRead = 1;
	dataDev->skipCheckpointWrite = 1;

	if (yaffsVersion == 2)
	{
		dataDev->writeChunkWithTagsToNAND = nandmtd2_WriteChunkWithTagsToNAND;
		dataDev->readChunkWithTagsFromNAND = nandmtd2_ReadChunkWithTagsFromNAND;
		dataDev->markNANDBlockBad = nandmtd2_MarkNANDBlockBad;
		dataDev->queryNANDBlock = nandmtd2_QueryNANDBlock;
		dataDev->spareBuffer = YMALLOC(mtd->oobsize);
		dataDev->isYaffs2 = 1;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17))
		dataDev->nDataBytesPerChunk = mtd->writesize;
		dataDev->nChunksPerBlock = mtd->erasesize / mtd->writesize;
#else
		dataDev->nDataBytesPerChunk = mtd->oobblock;
		dataDev->nChunksPerBlock = mtd->erasesize / mtd->oobblock;
#endif
		nBlocks = mtd->size / mtd->erasesize;
		//printf("size = 0x%016Lx  erasesize = 0x%016Lx  nBlocks = %d\n", (unsigned long long)mtd->size, (unsigned long long)mtd->erasesize, nBlocks);

		dataDev->nCheckpointReservedBlocks = 0;
		
		memset(&cur_partition, 0 , sizeof(struct mtd_partition));
		memset(partname, 0, 255);
		strcpy(partname, MOUNT_POINT5);
		cur_partition.name = (char *)(partname + 1); /* skip '/' charater */
		cur_partition.offset = 0xffffffff;
		yaffs_parse_cmdline_partitions(&cur_partition, (unsigned long long)mtd->size);
		//printf("offset = 0x%08x  size = 0x%08x\n", cur_partition.offset, cur_partition.size);
		dataDev->startBlock = cur_partition.offset / mtd->erasesize;
		dataDev->endBlock = dataDev->startBlock + cur_partition.size / mtd->erasesize  - 1;
		//printf("fixnv device from %d to %d\n", fixnvDev->startBlock, fixnvDev->endBlock);
	}
	else
	{
		dataDev->writeChunkToNAND = nandmtd_WriteChunkToNAND;
		dataDev->readChunkFromNAND = nandmtd_ReadChunkFromNAND;
		dataDev->isYaffs2 = 0;
		nBlocks = mtd->size / (YAFFS_CHUNKS_PER_BLOCK * YAFFS_BYTES_PER_CHUNK);
		dataDev->startBlock = 320;
		dataDev->endBlock = nBlocks - 1;
		dataDev->nChunksPerBlock = YAFFS_CHUNKS_PER_BLOCK;
		dataDev->nDataBytesPerChunk = YAFFS_BYTES_PER_CHUNK;
	}

	/* ... and common functions */
	dataDev->eraseBlockInNAND = nandmtd_EraseBlockInNAND;
	dataDev->initialiseNAND = nandmtd_InitialiseNAND;

	//yaffs_dump_dev(dataDev);
#endif

	yaffs_initialise(yaffsfs_config);

	return 0;
}


//void make_a_file(char *yaffsName,char bval,int sizeOfFile)
void make_a_file(char *yaffsName,unsigned long bval,int sizeOfFile)
{
	int outh;
	int i;
	unsigned char buffer[100];
	int aaa;

	outh = yaffs_open(yaffsName, O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE);
	if (outh < 0)
	{
		printf("Error opening file: %d\n", outh);
		return;
	}

#if 1
	memset(buffer,bval,100);
	for (aaa = 0; aaa < 100; aaa ++) {
		//printf("buffer[%d] = 0x%x\n", aaa, buffer[aaa]);
		buffer[aaa] = aaa;
	}

	do{
		i = sizeOfFile;
		if(i > 100) i = 100;
		sizeOfFile -= i;
		yaffs_write(outh,buffer,i);
	} while (sizeOfFile > 0);
#else
	
	printf("bval = 0x%08x\n", bval);
	yaffs_write(outh,&bval,4);
	
#endif


	yaffs_close(outh);
}

void read_a_file(char *fn)
{
	int h;
	int i = 0;
	unsigned char b;

	h = yaffs_open(fn, O_RDWR,0);
	if(h<0)
	{
		printf("File not found\n");
		return;
	}

	while(yaffs_read(h,&b,1)> 0)
	{
		printf(" %02x",b);
		i++;
		if(i > 15)
		{
		   printf("\n");
		   i = 0;
		 }
	}
	printf("\n");
	yaffs_close(h);
}

void cmd_yaffs_mount(char *mp)
{
	yaffs_StartUp();
	int retval = yaffs_mount(mp);
	if( retval != -1)
		isMounted = 1;
	else
		printf("Error mounting %s, return value: %d\n", mp, yaffsfs_GetError());
}

static void checkMount(void)
{
	if( !isMounted )
	{
		cmd_yaffs_mount(MOUNT_POINT);
	}
}

void cmd_yaffs_umount(char *mp)
{
	checkMount();
	if( yaffs_unmount(mp) == -1)
		printf("Error umounting %s, return value: %d\n", mp, yaffsfs_GetError());
}

//void cmd_yaffs_write_file(char *yaffsName,char bval,int sizeOfFile)
void cmd_yaffs_write_file(char *yaffsName,unsigned long bval,int sizeOfFile)
{
	printf("bval = 0x%08x  sizeOfFile = %d\n", bval, sizeOfFile);
	checkMount();
	make_a_file(yaffsName,bval,sizeOfFile);
}


void cmd_yaffs_read_file(char *fn)
{
	checkMount();
	read_a_file(fn);
}


void cmd_yaffs_mread_file(char *fn, unsigned char *addr)
{
	int h;
	struct yaffs_stat s;

	checkMount();

	yaffs_stat(fn,&s);

	printf ("Copy %s to 0x%08x... ", fn, addr);
	h = yaffs_open(fn, O_RDWR,0);
	if(h<0)
	{
		printf("File not found\n");
		return;
	}
	printf("st_size = %d\n", (int)s.st_size);
	yaffs_read(h,addr,(int)s.st_size);
	//printf("\t[DONE]\n");

	yaffs_close(h);
}


void cmd_yaffs_mwrite_file(char *fn, char *addr, int size)
{
	int outh;

	checkMount();
	outh = yaffs_open(fn, O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE);
	if (outh < 0)
	{
		printf("Error opening file: %d\n", outh);
	}

	yaffs_write(outh,addr,size);

	yaffs_close(outh);
}


void cmd_yaffs_ls(const char *mountpt, int longlist)
{
	int i;
	yaffs_DIR *d;
	yaffs_dirent *de;
	struct yaffs_stat stat;
	char tempstr[255];
	
	checkMount();
	d = yaffs_opendir(mountpt);

	if(!d)
	{
		printf("opendir failed\n");
	}
	else
	{
		for(i = 0; (de = yaffs_readdir(d)) != NULL; i++)
		{
			if (longlist)
			{
				memset(tempstr, 0, 255);
				sprintf(tempstr, "%s/%s", mountpt, de->d_name);
				yaffs_stat(tempstr, &stat);
				printf("%-25s\t%7d\n",de->d_name, stat.st_size);
			}
			else
			{
				printf("%s\n",de->d_name);
			}
		}
	}
}

/*
*  retval : -1 file is not here ;  > 0 file is here and return size of file
*/
int cmd_yaffs_ls_chk(char *dirfilename)
{
	int i;
	yaffs_DIR *d;
	yaffs_dirent *de;
	struct yaffs_stat stat;
	char tempstr[255];
	int ret = -1;
	char filename[255];
	char mountpt[255];
	char *ptr = NULL;

	memset(tempstr, 0, 255);
	memset(mountpt, 0, 255);
	memset(filename, 0, 255);

	strcpy(tempstr, dirfilename);
	ptr = strrchr(tempstr, '/');
	if (ptr == NULL)
		return ret;
	*ptr = '\0'; 
	strcpy(filename, (ptr + 1));
	strcpy(mountpt, tempstr);

	checkMount();
	d = yaffs_opendir(mountpt);

	if(!d)
	{
		printf("opendir failed\n");
	}
	else
	{
		for(i = 0; (de = yaffs_readdir(d)) != NULL; i++)
		{
				if (strcmp(filename, de->d_name) == 0) {
					memset(tempstr, 0, 255);
					sprintf(tempstr, "%s/%s", mountpt, de->d_name);
					yaffs_stat(tempstr, &stat);
					//printf("%-25s\t%7d\n",de->d_name, stat.st_size);
					//printf("%-25s\t%7d\n",tempstr, stat.st_size);
					ret = stat.st_size;
					return ret;
				}
		}
	}

	return ret;
}

void cmd_yaffs_mkdir(const char *dir)
{
	checkMount();

	int retval = yaffs_mkdir(dir, 0);

	if ( retval < 0)
		printf("yaffs_mkdir returning error: %d\n", retval);
}

void cmd_yaffs_rmdir(const char *dir)
{
	checkMount();

	int retval = yaffs_rmdir(dir);

	if ( retval < 0)
		printf("yaffs_rmdir returning error: %d\n", retval);
}

void cmd_yaffs_rm(const char *path)
{
	checkMount();

	int retval = yaffs_unlink(path);

	if ( retval < 0)
		printf("yaffs_unlink returning error: %d\n", retval);
}

void cmd_yaffs_mv(const char *oldPath, const char *newPath)
{
	checkMount();

	int retval = yaffs_rename(newPath, oldPath);

	if ( retval < 0)
		printf("yaffs_unlink returning error: %d\n", retval);
}
