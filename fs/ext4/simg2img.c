/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <linux/types.h>
#include "common.h"
#include "part.h"
#include "sparse_format.h"
#include "sparse_crc32.h"


typedef struct _emmc_handle
{
	u8  cardPartiton;
	u32 startBlock;
	u32 offset;
	disk_partition_t info;
	block_dev_desc_t *pdev;
}T_Emmc_Handle;

#define COPY_BUF_SIZE (1024*1024)
#define EMMC_BLOCK_SIZE 512
u8 copybuf[COPY_BUF_SIZE];

/* This will be malloc'ed with the size of blk_sz from the sparse file header */
u8* zerobuf;
static unsigned int g_buf_index = 0;
static T_Emmc_Handle g_emmc_handle;

#define SPARSE_HEADER_MAJOR_VER 1
#define SPARSE_HEADER_LEN       (sizeof(sparse_header_t))
#define CHUNK_HEADER_LEN (sizeof(chunk_header_t))

static int read_all(void *buf_src, u32 src_index, void *buf_dest, size_t len)
{
	memcpy(buf_dest, (void*)((u32)buf_src + src_index), len);
	g_buf_index += len;
	return len;
}

static int write_all(T_Emmc_Handle *p_handle, void *buf, size_t len)
{
	u32 blocknum;
	block_dev_desc_t *pdev = p_handle->pdev;
	if(len%EMMC_BLOCK_SIZE){
		blocknum = len/EMMC_BLOCK_SIZE + 1;
	}else{
		blocknum = len/EMMC_BLOCK_SIZE;
	}
	if(pdev->block_write(pdev->dev, p_handle->startBlock + p_handle->offset,  blocknum, (unsigned char *)buf) == blocknum){
		p_handle->offset += blocknum;
		return len;	
	}else{
		return 0;
	}
}

int process_raw_chunk(void *buf, T_Emmc_Handle *p_handle, u32 blocks, u32 blk_sz, u32 *crc32)
{
	u64 len = (u64)blocks * blk_sz;
	int ret;
	int chunk;

	while (len) {
		chunk = (len > COPY_BUF_SIZE) ? COPY_BUF_SIZE : len;
		ret = read_all(buf, g_buf_index, copybuf, chunk);
		if (ret != chunk) {
			printf("read returned an error copying a raw chunk: %d %d\n",
					ret, chunk);
			return -1;
		}
		*crc32 = sparse_crc32(*crc32, copybuf, chunk);
		ret = write_all(p_handle, copybuf, chunk);
		if (ret != chunk) {
			printf("write returned an error copying a raw chunk\n");
			return -1;
		}
		len -= chunk;
	}

	return blocks;
}


int process_fill_chunk(void *buf, T_Emmc_Handle *p_handle, u32 blocks, u32 blk_sz, u32 *crc32)
{
	u64 len = (u64)blocks * blk_sz;
	int ret;
	int chunk;
	u32 fill_val;
	u32 *fillbuf;
	unsigned int i;

	/* Fill copy_buf with the fill value */
	ret = read_all(buf, g_buf_index, &fill_val, sizeof(fill_val));
	fillbuf = (u32 *)copybuf;
	for (i = 0; i < (COPY_BUF_SIZE / sizeof(fill_val)); i++) {
		fillbuf[i] = fill_val;
	}

	while (len) {
		chunk = (len > COPY_BUF_SIZE) ? COPY_BUF_SIZE : len;
		*crc32 = sparse_crc32(*crc32, copybuf, chunk);
		ret = write_all(p_handle, copybuf, chunk);
		if (ret != chunk) {
			printf("write returned an error copying a raw chunk\n");
			return -1;
		}
		len -= chunk;
	}

	return blocks;
}

int process_skip_chunk(T_Emmc_Handle *p_handle, u32 blocks, u32 blk_sz, u32 *crc32)
{
	/* len needs to be 64 bits, as the sparse file specifies the skip amount
	 * as a 32 bit value of blocks.
	 */
	u64 len = (u64)blocks * blk_sz;
	p_handle->offset += len/EMMC_BLOCK_SIZE;

	return blocks;
}

int process_crc32_chunk(void *buf, u32 crc32)
{
	u32 file_crc32;
	int ret;

	ret = read_all(buf, g_buf_index, &file_crc32, 4);
	if (ret != 4) {
		printf("read returned an error copying a crc32 chunk\n");
		return -1;
	}

	if (file_crc32 != crc32) {
		printf("computed crc32 of 0x%8.8x, expected 0x%8.8x\n",
			 crc32, file_crc32);
		return -1;
	}

	return 0;
}

int write_simg2emmc(char* interface, int  dev, int part, u8* buf)
{
	unsigned int i;
	sparse_header_t sparse_header;
	chunk_header_t chunk_header;
	u32 crc32 = 0;
	u32 total_blocks = 0;
	int ret;
	g_buf_index = 0;
	
	g_emmc_handle.pdev = get_dev(interface, dev);
	if (g_emmc_handle.pdev == NULL) {
		printf("Block device %s %d not supported\n", interface, dev);
		goto fail;
	}
	if (get_partition_info(g_emmc_handle.pdev, part, &g_emmc_handle.info))
		goto fail;
	g_emmc_handle.cardPartiton = 0;
	
	g_emmc_handle.startBlock = g_emmc_handle.info.start;
	g_emmc_handle.offset = 0;
	ret = read_all(buf, g_buf_index, &sparse_header, sizeof(sparse_header));
	if (ret != sizeof(sparse_header)) {
		printf("Error reading sparse file header\n");
		goto fail;
	}
	if (sparse_header.magic != SPARSE_HEADER_MAGIC) {
		printf("Bad magic\n");
		goto fail;
	}

	if (sparse_header.major_version != SPARSE_HEADER_MAJOR_VER) {
		printf("Unknown major version number\n");
		goto fail;
	}

	if (sparse_header.file_hdr_sz > SPARSE_HEADER_LEN) {
		/* Skip the remaining bytes in a header that is longer than
		 * we expected.
		 */
		g_buf_index += (sparse_header.file_hdr_sz - SPARSE_HEADER_LEN);
	}

	for (i=0; i<sparse_header.total_chunks; i++) {
		ret = read_all((void*)buf, g_buf_index, &chunk_header, sizeof(chunk_header));
		if (ret != sizeof(chunk_header)) {
			printf("Error reading chunk header\n");
			goto fail;
		}
 
		if (sparse_header.chunk_hdr_sz > CHUNK_HEADER_LEN) {
			/* Skip the remaining bytes in a header that is longer than
			 * we expected.
			 */
			g_buf_index += sparse_header.chunk_hdr_sz - CHUNK_HEADER_LEN;			 
		}

		switch (chunk_header.chunk_type) {
			case CHUNK_TYPE_RAW:
			if (chunk_header.total_sz != (sparse_header.chunk_hdr_sz +
				 (chunk_header.chunk_sz * sparse_header.blk_sz)) ) {
				printf("Bogus chunk size for chunk %d, type Raw\n", i);
				goto fail;
			}
			total_blocks += process_raw_chunk((void*)buf, &g_emmc_handle,
					 chunk_header.chunk_sz, sparse_header.blk_sz, &crc32);
			break;
			case CHUNK_TYPE_FILL:
			if (chunk_header.total_sz != (sparse_header.chunk_hdr_sz + sizeof(u32)) ) {
				printf("Bogus chunk size for chunk %d, type Fill\n", i);
				goto fail;
			}
			total_blocks += process_fill_chunk((void*)buf, &g_emmc_handle,
					 chunk_header.chunk_sz, sparse_header.blk_sz, &crc32);
			break;
			case CHUNK_TYPE_DONT_CARE:
			if (chunk_header.total_sz != sparse_header.chunk_hdr_sz) {
				printf("Bogus chunk size for chunk %d, type Dont Care\n", i);
				goto fail;
			}
			total_blocks += process_skip_chunk(&g_emmc_handle,
					 chunk_header.chunk_sz, sparse_header.blk_sz, &crc32);
			break;
			case CHUNK_TYPE_CRC32:
			if(process_crc32_chunk((void*)buf, crc32) != 0){
				goto fail;
			}
			break;
			default:
			printf("Unknown chunk type 0x%4.4x\n", chunk_header.chunk_type);
		}

	}

	if (sparse_header.total_blks != total_blocks) {
		printf("Wrote %d blocks, expected to write %d blocks\n",
			 total_blocks, sparse_header.total_blks);
		goto fail;
	}
	return 0;
fail:
	return -1;
}


