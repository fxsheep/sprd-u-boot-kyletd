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

#include <linux/stat.h>
#include "ext4.h"
#include "make_ext4fs.h"
#include "output_file.h"
#include "ext4_utils.h"
#include "allocate.h"
#include "contents.h"
#include "uuid.h"
#include "dirent.h"

#define ANDROID		(1)
#define EXIT_FAILURE 	(1)
#define EXIT_SUCCESS 	(0)

#ifdef ANDROID
#include "android_filesystem_config.h"
#endif


/* TODO: Not implemented:
   Allocating blocks in the same block group as the file inode
   Hash or binary tree directories
   Special files: sockets, devices, fifos
 */

static int filter_dot(const struct dirent *d)
{
	return (strcmp(d->d_name, "..") && strcmp(d->d_name, "."));
}

static u32 build_default_directory_structure()
{
	u32 inode;
	u32 root_inode;
	struct dentry dentries = {
			.filename = "lost+found",
			.file_type = EXT4_FT_DIR,
			.mode = S_IRWXU,
			.uid = 0,
			.gid = 0,
			.mtime = 0,
	};
	root_inode = make_directory(0, 1, &dentries, 1);
	inode = make_directory(root_inode, 0, NULL, 0);
	*dentries.inode = inode;
	inode_set_permissions(inode, dentries.mode,
		dentries.uid, dentries.gid, dentries.mtime);

	return root_inode;
}

/* Read a local directory and create the same tree in the generated filesystem.
   Calls itself recursively with each directory in the given directory */
static u32 build_directory_structure(const char *full_path, const char *dir_path,
		u32 dir_inode, int android)
{
	int entries = 0;
	struct dentry *dentries;
	struct dirent **namelist;
	struct stat stat;
	int ret;
	int i;
	u32 inode;
	u32 entry_inode;
	u32 dirs = 0;

	/* richardfeng : return 0 when dir_path is not any file */
	//entries = scandir(full_path, &namelist, filter_dot, (void*)alphasort);
	entries = 0;

	if (entries < 0) {
		printf("scandir failed : %s %d\n", __FUNCTION__, __LINE__);
		return EXT4_ALLOCATE_FAILED;
	}
	
	//printf("full_path = %s  dir_path = %s  dir_inode = 0x%08x  android = %d  entries = %d\n", full_path, dir_path, dir_inode, android, entries);

	dentries = calloc(entries, sizeof(struct dentry));
	if (dentries == NULL)
		printf("\n\ncalloc failed : %s %d\n\n", __FUNCTION__, __LINE__);
	for (i = 0; i < entries; i++) {
		dentries[i].filename = strdup(namelist[i]->d_name);
		if (dentries[i].filename == NULL)
			printf("strdup");

		sprintf(&dentries[i].path, "%s/%s", dir_path, namelist[i]->d_name);
		sprintf(&dentries[i].full_path, "%s/%s", full_path, namelist[i]->d_name);

		free(namelist[i]);

		ret = 10/*richard feng lstat(dentries[i].full_path, &stat)*/;
		if (ret < 0) {
			printf("lstat");
			i--;
			entries--;
			continue;
		}

		dentries[i].size = stat.st_size;
		dentries[i].mode = stat.st_mode & (S_ISUID|S_ISGID|S_ISVTX|S_IRWXU|S_IRWXG|S_IRWXO);
		dentries[i].mtime = stat.st_mtime;
		if (android) {
#ifdef ANDROID
			unsigned int mode = 0;
			unsigned int uid = 0;
			unsigned int gid = 0;
			int dir = S_ISDIR(stat.st_mode);
			//fs_config(dentries[i].path, dir, &uid, &gid, &mode);
			dentries[i].mode = stat.st_mode;
			dentries[i].uid = stat.st_uid;
			dentries[i].gid = stat.st_gid;
#else
			printf("can't set android permissions - built without android support");
#endif
		}

		if (S_ISREG(stat.st_mode)) {
			dentries[i].file_type = EXT4_FT_REG_FILE;
		} else if (S_ISDIR(stat.st_mode)) {
			dentries[i].file_type = EXT4_FT_DIR;
			dirs++;
		} else if (S_ISCHR(stat.st_mode)) {
			dentries[i].file_type = EXT4_FT_CHRDEV;
		} else if (S_ISBLK(stat.st_mode)) {
			dentries[i].file_type = EXT4_FT_BLKDEV;
		} else if (S_ISFIFO(stat.st_mode)) {
			dentries[i].file_type = EXT4_FT_FIFO;
		} else if (S_ISSOCK(stat.st_mode)) {
			dentries[i].file_type = EXT4_FT_SOCK;
		} else if (S_ISLNK(stat.st_mode)) {
			dentries[i].file_type = EXT4_FT_SYMLINK;
			dentries[i].link = calloc(info.block_size, 1);
			readlink(dentries[i].full_path, dentries[i].link, info.block_size - 1);
		} else {
			printf("unknown file type on %s", dentries[i].path);
			i--;
			entries--;
		}
	}

	free(namelist);

	inode = make_directory(dir_inode, entries, dentries, dirs);
	for (i = 0; i < entries; i++) {
		if (dentries[i].file_type == EXT4_FT_REG_FILE) {
			entry_inode = make_file(dentries[i].full_path, dentries[i].size);
		} else if (dentries[i].file_type == EXT4_FT_DIR) {
			entry_inode = build_directory_structure(dentries[i].full_path,
					dentries[i].path, inode, android);
		} else if (dentries[i].file_type == EXT4_FT_SYMLINK) {
			entry_inode = make_link(dentries[i].full_path, dentries[i].link);
		} else {
			printf("unknown file type on %s", dentries[i].path);
			entry_inode = 0;
		}
		*dentries[i].inode = entry_inode;

		ret = inode_set_permissions(entry_inode, dentries[i].mode,
			dentries[i].uid, dentries[i].gid,
			dentries[i].mtime);
		if (ret)
			printf("failed to set permissions on %s\n", dentries[i].path);

		free(dentries[i].path);
		free(dentries[i].full_path);
		free(dentries[i].link);
		free((void *)dentries[i].filename);
	}

	free(dentries);
	return inode;
}

static u32 compute_block_size()
{
	return 4096;
}

static u32 compute_journal_blocks()
{
	u32 journal_blocks = DIV_ROUND_UP(info.len, info.block_size) / 64;
	if (journal_blocks < 1024)
		journal_blocks = 1024;
	if (journal_blocks > 32768)
		journal_blocks = 32768;
	return journal_blocks;
}

static u32 compute_blocks_per_group()
{
	u32 ret;
	u32 max_blocks_per_group = info.block_size * 8;
	u32 block_count = info.len / info.block_size;

	if (block_count < max_blocks_per_group)
		ret = block_count;
	else
		ret = max_blocks_per_group;

	return ret;
}

static u32 compute_inodes()
{
	return DIV_ROUND_UP(info.len, info.block_size) / 4;
}

static u32 compute_inodes_per_group()
{
	u32 blocks = DIV_ROUND_UP(info.len, info.block_size);
	u32 block_groups = DIV_ROUND_UP(blocks, info.blocks_per_group);
	return DIV_ROUND_UP(info.inodes, block_groups);
}

void reset_ext4fs_info() {
    // Reset all the global data structures used by make_ext4fs so it
    // can be called again.
    memset(&info, 0, sizeof(info));
    memset(&aux_info, 0, sizeof(aux_info));
    free_data_blocks();
}

int make_ext4fs(const char *filename, const char *directory,
                char *mountpoint, int android, int gzip, int sparse, unsigned long partstart, unsigned long blocksize)
{
        u32 root_inode_num;
        u16 root_mode;

	if (info.len == 0)
		info.len = get_file_size(filename);

	if (info.len <= 0) {
		printf("Need size of filesystem\n");
                return EXIT_FAILURE;
	}

	if (info.block_size <= 0)
		info.block_size = compute_block_size();

	if (info.journal_blocks == 0)
		info.journal_blocks = compute_journal_blocks();

	if (info.no_journal == 0)
		info.feat_compat = EXT4_FEATURE_COMPAT_HAS_JOURNAL;
	else
		info.journal_blocks = 0;

	if (info.blocks_per_group <= 0)
		info.blocks_per_group = compute_blocks_per_group();

	if (info.inodes <= 0)
		info.inodes = compute_inodes();

	if (info.inode_size <= 0)
		info.inode_size = 256;

	if (info.label == NULL)
		info.label = "";

	info.inodes_per_group = compute_inodes_per_group();
	info.feat_compat |=
			EXT4_FEATURE_COMPAT_RESIZE_INODE;

	info.feat_ro_compat |=
			EXT4_FEATURE_RO_COMPAT_SPARSE_SUPER |
			EXT4_FEATURE_RO_COMPAT_LARGE_FILE;

	info.feat_incompat |=
			EXT4_FEATURE_INCOMPAT_EXTENTS |
			EXT4_FEATURE_INCOMPAT_FILETYPE;

	//printf("len = 0x%16Lx  block_size = %d, blocks_per_group = %d, inodes_per_group = %d, inode_size = %d, inodes = %d, journal_blocks = %d, feat_ro_compat = %d, feat_compat = %d, feat_incompat = %d, label = %s, no_journal = %d\n", info.len, info.block_size, info.blocks_per_group, info.inodes_per_group, info.inode_size, info.inodes, info.journal_blocks, info.feat_ro_compat, info.feat_compat, info.feat_incompat, info.label, info.no_journal);

	printf("Creating filesystem with parameters:\n");
	printf("    Size: %llu\n", info.len);
	printf("    Block size: %d\n", info.block_size);
	printf("    Blocks per group: %d\n", info.blocks_per_group);
	printf("    Inodes per group: %d\n", info.inodes_per_group);
	printf("    Inode size: %d\n", info.inode_size);
	printf("    Journal blocks: %d\n", info.journal_blocks);
	printf("    Label: %s\n", info.label);

	ext4_create_fs_aux_info();

	printf("    Blocks: %llu\n", aux_info.len_blocks);
	printf("    Block groups: %d\n", aux_info.groups);
	printf("    Reserved block group size: %d\n", aux_info.bg_desc_reserve_blocks);

	block_allocator_init();
	ext4_fill_in_sb();
	if (reserve_inodes(0, 10) == EXT4_ALLOCATE_FAILED)
		printf("failed to reserve first 10 inodes");
	if (info.feat_compat & EXT4_FEATURE_COMPAT_HAS_JOURNAL)
		ext4_create_journal_inode();
	if (info.feat_compat & EXT4_FEATURE_COMPAT_RESIZE_INODE)
		ext4_create_resize_inode();
	printf("directory = %s\n", directory);
	if (directory)
		root_inode_num = build_directory_structure(directory, mountpoint, 0, android);
	else
		root_inode_num = build_default_directory_structure();

	root_mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
	inode_set_permissions(root_inode_num, root_mode, 0, 0, 0);
	ext4_update_free();
	printf("Created filesystem with %d/%d inodes and %d/%d blocks\n",
			aux_info.sb->s_inodes_count - aux_info.sb->s_free_inodes_count,
			aux_info.sb->s_inodes_count,
			aux_info.sb->s_blocks_count_lo - aux_info.sb->s_free_blocks_count_lo,
			aux_info.sb->s_blocks_count_lo);

	write_ext4_image(filename, gzip, sparse, partstart, blocksize);

	return 0;
}
