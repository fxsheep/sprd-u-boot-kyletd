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

#include "ext4_utils.h"
#include "backed_block.h"

#define ERROR_DATA_BUFFER_LEN	(1024)
struct data_block {
	u32 block;
	u32 len;
	u8 *data;
	const char *filename;
	off_t offset;
	struct data_block *next;
};

static struct data_block *data_blocks = NULL;
static struct data_block *last_used = NULL;
static struct data_block error_data_block;
u8 error_data_buffer[ERROR_DATA_BUFFER_LEN];

void restore_error_data_block(void)
{
	struct data_block *db;

	for (db = data_blocks; db; db = db->next) {
		if (db->filename == NULL)
			continue;
		//printf("oldblock = %d  len = %d  data = 0x%08x filename = %s  offset = %d  next = 0x%08x\n", db->block, db->len, db->data, db->filename, db->offset, db->next);
		memcpy(db, &error_data_block, sizeof(struct data_block));
		//printf("newblock = %d  len = %d  data = 0x%08x filename = %s  offset = %d  next = 0x%08x\n", db->block, db->len, db->data, db->filename, db->offset, db->next);
		break;
	}
}

void dump_queue_db(void)
{
	struct data_block *db;

	printf("\n");
	for (db = data_blocks; db; db = db->next)
		printf("block = %d  len = %d  data = 0x%08x filename = %s  offset = %d  next = 0x%08x\n", db->block, db->len, db->data, db->filename, db->offset, db->next);

	if (data_blocks)
		printf("error_data_block block = %d  len = %d  data = 0x%08x filename = %s  offset = %d  next = 0x%08x\n", error_data_block.block, error_data_block.len, error_data_block.data, error_data_block.filename, error_data_block.offset, error_data_block.next);
	printf("\n");
}

static void queue_db(struct data_block *new_db)
{
	struct data_block *db;

	if (data_blocks == NULL) {
		data_blocks = new_db;
		/* richardfeng : last inqueued data_block is modified by someone */
		memcpy(&error_data_block, new_db, sizeof(struct data_block));
		return;
	}

	if (data_blocks->block > new_db->block) {
		new_db->next = data_blocks;
		data_blocks = new_db;
		/* richardfeng : last inqueued data_block is modified by someone */
		memcpy(&error_data_block, new_db, sizeof(struct data_block));
		return;
	}

	/* Optimization: blocks are mostly queued in sequence, so save the
	   pointer to the last db that was added, and start searching from
	   there if the next block number is higher */
	if (last_used && new_db->block > last_used->block)
		db = last_used;
	else
		db = data_blocks;
	last_used = new_db;

	for (; db->next && db->next->block < new_db->block; db = db->next)
		;

	if (db->next == NULL) {
		db->next = new_db;
	} else {
		new_db->next = db->next;
		db->next = new_db;
	}

	/* richardfeng : last inqueued data_block is modified by someone */
	memcpy(&error_data_block, new_db, sizeof(struct data_block));
}

/* Queues a block of memory to be written to the specified data blocks */
void queue_data_block(u8 *data, u32 len, u32 block)
{
	struct data_block *db = malloc(sizeof(struct data_block));
	if (db == NULL)
		printf("\n\nmalloc failed : %s %d\n\n", __FUNCTION__, __LINE__);

	db->block = block;
	db->len = len;
	db->data = data;
	db->filename = NULL;
	db->next = NULL;
	//printf("%s %d  block = %d  len = %d\n", __FUNCTION__, __LINE__, block, len);
	queue_db(db);
}

/* Queues a chunk of a file on disk to be written to the specified data blocks */
void queue_data_file(const char *filename, off_t offset, u32 len,
	u32 block)
{
	struct data_block *db = malloc(sizeof(struct data_block));
	if (db == NULL)
		printf("\n\nmalloc failed : %s %d\n\n", __FUNCTION__, __LINE__);

	db->block = block;
	db->len = len;
	db->filename = strdup(filename);
	db->offset = offset;
	db->next = NULL;
	//printf("%s %d filename = %s\n", __FUNCTION__, __LINE__, filename);
	queue_db(db);
}

/* Iterates over the queued data blocks, calling data_func for each contiguous
   data block, and file_func for each contiguous file block */
void for_each_data_block(data_block_callback_t data_func,
	data_block_file_callback_t file_func, struct output_file *out)
{
	struct data_block *db;
	u32 last_block = 0;
	u32 error_count;

	for (db = data_blocks; db; db = db->next) {
		if (db->block < last_block)
			printf("data blocks out of order: %u < %u", db->block, last_block);
		last_block = db->block + DIV_ROUND_UP(db->len, info.block_size) - 1;

		if (db->filename)
			file_func(out, (u64)db->block * info.block_size, db->filename, db->offset, db->len);
		else {
			/* richardfeng restore the last inqueued block data */
			if (error_data_block.block == db->block) {
				//printf("error_data_block.block = %d  db->len = %d\n", error_data_block.block, db->len);
				memset(error_data_buffer, 0, ERROR_DATA_BUFFER_LEN);
				restore_error_data_array(error_data_buffer);
////////////////////////////////////////////////////////////////////
for (error_count = 0; error_count < db->len; error_count += ERROR_DATA_BUFFER_LEN) {
	if (error_count < ERROR_DATA_BUFFER_LEN)
		data_func(out, (u64)(db->block * info.block_size + error_count), error_data_buffer, ERROR_DATA_BUFFER_LEN);
    	else {
		memset(error_data_buffer, 0, ERROR_DATA_BUFFER_LEN);
		data_func(out, (u64)(db->block * info.block_size + error_count), error_data_buffer, ERROR_DATA_BUFFER_LEN);
    	}
}
///////////////////////////////////////////////////////////////////
			} else
				data_func(out, (u64)db->block * info.block_size, db->data, db->len);
		}
	}
}

/* Frees the memory used by the linked list of data blocks */
void free_data_blocks()
{
        if (!data_blocks) return;
	struct data_block *db = data_blocks;
	while (db) {
		struct data_block *next = db->next;
		free((void*)db->filename);

                // There used to be a free() of db->data here, but it
		// made the function crash since queue_data_block() is
		// sometimes passed pointers it can't take ownership of
		// (like a pointer into the middle of an allocated
		// block).  It's not clear what the queue_data_block
		// contract is supposed to be, but we'd rather leak
		// memory than crash.

		free(db);
		db = next;
	}
        data_blocks = NULL;
        last_used = NULL;
}
