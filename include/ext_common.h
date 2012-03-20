/*
 * (C) Copyright 2011 - 2012 Samsung Electronics
 * EXT4 filesystem implementation in Uboot by
 * Uma Shankar <uma.shankar@samsung.com>
 * Manjunatha C Achar <a.manjunatha@samsung.com>
 *
 * Data structures and headers for ext4 support have been taken from
 * ext2 ls load support in Uboot
 *
 * (C) Copyright 2004
 * esd gmbh <www.esd-electronics.com>
 * Reinhard Arlt <reinhard.arlt@esd-electronics.com>
 *
 * based on code from grub2 fs/ext2.c and fs/fshelp.c by
 * GRUB  --  GRand Unified Bootloader
 * Copyright (C) 2003, 2004  Free Software Foundation, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __EXT_COMMON__
#define __EXT_COMMON__

#define SECTOR_SIZE		0x200
#define SECTOR_BITS		9


/* Magic value used to identify an ext2 filesystem.  */
#define	EXT2_MAGIC		0xEF53



/* Amount of indirect blocks in an inode.  */
#define INDIRECT_BLOCKS		12

/* richardfeng add */
#define EXT4_NDIR_BLOCKS 12
#define EXT4_IND_BLOCK EXT4_NDIR_BLOCKS
#define EXT4_DIND_BLOCK (EXT4_IND_BLOCK + 1)
#define EXT4_TIND_BLOCK (EXT4_DIND_BLOCK + 1)
#define EXT4_N_BLOCKS (EXT4_TIND_BLOCK + 1)

#define EXT4_SECRM_FL 0x00000001  
#define EXT4_UNRM_FL 0x00000002  
#define EXT4_COMPR_FL 0x00000004  
#define EXT4_SYNC_FL 0x00000008  
#define EXT4_IMMUTABLE_FL 0x00000010  
#define EXT4_APPEND_FL 0x00000020  
#define EXT4_NODUMP_FL 0x00000040  
#define EXT4_NOATIME_FL 0x00000080  

#define EXT4_DIRTY_FL 0x00000100
#define EXT4_COMPRBLK_FL 0x00000200  
#define EXT4_NOCOMPR_FL 0x00000400  
#define EXT4_ECOMPR_FL 0x00000800  

#define EXT4_INDEX_FL 0x00001000  
#define EXT4_IMAGIC_FL 0x00002000  
#define EXT4_JOURNAL_DATA_FL 0x00004000  
#define EXT4_NOTAIL_FL 0x00008000  
#define EXT4_DIRSYNC_FL 0x00010000  
#define EXT4_TOPDIR_FL 0x00020000  
#define EXT4_HUGE_FILE_FL 0x00040000  
#define EXT4_EXTENTS_FL 0x00080000  
#define EXT4_EA_INODE_FL 0x00200000  
#define EXT4_EOFBLOCKS_FL 0x00400000  
#define EXT4_RESERVED_FL 0x80000000  

#define EXT4_FL_USER_VISIBLE 0x004BDFFF  
#define EXT4_FL_USER_MODIFIABLE 0x004B80FF  

#define EXT4_FL_INHERITED (EXT4_SECRM_FL | EXT4_UNRM_FL | EXT4_COMPR_FL |  EXT4_SYNC_FL | EXT4_IMMUTABLE_FL | EXT4_APPEND_FL |  EXT4_NODUMP_FL | EXT4_NOATIME_FL |  EXT4_NOCOMPR_FL | EXT4_JOURNAL_DATA_FL |  EXT4_NOTAIL_FL | EXT4_DIRSYNC_FL)

#define EXT4_REG_FLMASK (~(EXT4_DIRSYNC_FL | EXT4_TOPDIR_FL))

#define EXT4_OTHER_FLMASK (EXT4_NODUMP_FL | EXT4_NOATIME_FL)

/* Maximum lenght of a pathname.  */
#define EXT2_PATH_MAX			4096
/* Maximum nesting of symlinks, used to prevent a loop.  */
#define	EXT2_MAX_SYMLINKCNT		8

/* Filetype used in directory entry.  */
#define	FILETYPE_UNKNOWN		0
#define	FILETYPE_REG			1
#define	FILETYPE_DIRECTORY		2
#define	FILETYPE_SYMLINK		7

/* Filetype information as used in inodes.  */
#define FILETYPE_INO_MASK		0170000
#define FILETYPE_INO_REG		0100000
#define FILETYPE_INO_DIRECTORY		0040000
#define FILETYPE_INO_SYMLINK		0120000
#define EXT2_ROOT_INO			2 /* Root inode */

/* Bits used as offset in sector */
#define DISK_SECTOR_BITS		9
/* The size of an ext2 block in bytes.  */
#define EXT2_BLOCK_SIZE(data)	   (1 << LOG2_BLOCK_SIZE(data))

/* Log2 size of ext2 block in 512 blocks.  */
#define LOG2_EXT2_BLOCK_SIZE(data) (__le32_to_cpu \
				(data->sblock.log2_block_size) + 1)

/* Log2 size of ext2 block in bytes.  */
#define LOG2_BLOCK_SIZE(data)	   (__le32_to_cpu \
		(data->sblock.log2_block_size) + 10)
#define INODE_SIZE_FILESYSTEM(data)	(__le32_to_cpu \
			(data->sblock.inode_size))

#define EXT2_FT_DIR	2
#define SUCCESS	1

/* Macro-instructions used to manage several block sizes  */
#define EXT2_MIN_BLOCK_LOG_SIZE	10 /* 1024 */
#define EXT2_MAX_BLOCK_LOG_SIZE	16 /* 65536 */
#define EXT2_MIN_BLOCK_SIZE		(1 << EXT2_MIN_BLOCK_LOG_SIZE)
#define EXT2_MAX_BLOCK_SIZE		(1 << EXT2_MAX_BLOCK_LOG_SIZE)

/* The ext2 superblock.  */
struct ext2_sblock {
	uint32_t total_inodes;
	uint32_t total_blocks;
	uint32_t reserved_blocks;
	uint32_t free_blocks;
	uint32_t free_inodes;
	uint32_t first_data_block;
	uint32_t log2_block_size;
	uint32_t log2_fragment_size;
	uint32_t blocks_per_group;
	uint32_t fragments_per_group;
	uint32_t inodes_per_group;
	uint32_t mtime;
	uint32_t utime;
	uint16_t mnt_count;
	uint16_t max_mnt_count;
	uint16_t magic;
	uint16_t fs_state;
	uint16_t error_handling;
	uint16_t minor_revision_level;
	uint32_t lastcheck;
	uint32_t checkinterval;
	uint32_t creator_os;
	uint32_t revision_level;
	uint16_t uid_reserved;
	uint16_t gid_reserved;
	uint32_t first_inode;
	uint16_t inode_size;
	uint16_t block_group_number;
	uint32_t feature_compatibility;
	uint32_t feature_incompat;
	uint32_t feature_ro_compat;
	uint32_t unique_id[4];
	char volume_name[16];
	char last_mounted_on[64];
	uint32_t compression_info;
};

struct ext4_super_block {
  __le32 s_inodes_count;
 __le32 s_blocks_count_lo;
 __le32 s_r_blocks_count_lo;
 __le32 s_free_blocks_count_lo;
  __le32 s_free_inodes_count;
 __le32 s_first_data_block;
 __le32 s_log_block_size;
 __le32 s_obso_log_frag_size;
  __le32 s_blocks_per_group;
 __le32 s_obso_frags_per_group;
 __le32 s_inodes_per_group;
 __le32 s_mtime;
  __le32 s_wtime;
 __le16 s_mnt_count;
 __le16 s_max_mnt_count;
 __le16 s_magic;
 __le16 s_state;
 __le16 s_errors;
 __le16 s_minor_rev_level;
  __le32 s_lastcheck;
 __le32 s_checkinterval;
 __le32 s_creator_os;
 __le32 s_rev_level;
  __le16 s_def_resuid;
 __le16 s_def_resgid;

 __le32 s_first_ino;
 __le16 s_inode_size;
 __le16 s_block_group_nr;
 __le32 s_feature_compat;
  __le32 s_feature_incompat;
 __le32 s_feature_ro_compat;
  __u8 s_uuid[16];
  char s_volume_name[16];
  char s_last_mounted[64];
  __le32 s_algorithm_usage_bitmap;

 __u8 s_prealloc_blocks;
 __u8 s_prealloc_dir_blocks;
 __le16 s_reserved_gdt_blocks;

  __u8 s_journal_uuid[16];
  __le32 s_journal_inum;
 __le32 s_journal_dev;
 __le32 s_last_orphan;
 __le32 s_hash_seed[4];
 __u8 s_def_hash_version;
 __u8 s_reserved_char_pad;
 __le16 s_desc_size;
  __le32 s_default_mount_opts;
 __le32 s_first_meta_bg;
 __le32 s_mkfs_time;
 __le32 s_jnl_blocks[17];

  __le32 s_blocks_count_hi;
 __le32 s_r_blocks_count_hi;
 __le32 s_free_blocks_count_hi;
 __le16 s_min_extra_isize;
 __le16 s_want_extra_isize;
 __le32 s_flags;
 __le16 s_raid_stride;
 __le16 s_mmp_interval;
 __le64 s_mmp_block;
 __le32 s_raid_stripe_width;
 __u8 s_log_groups_per_flex;
 __u8 s_reserved_char_pad2;
 __le16 s_reserved_pad;
 __le64 s_kbytes_written;
 __u32 s_reserved[160];
};

struct ext2_block_group {
	__u32 block_id;	/* Blocks bitmap block */
	__u32 inode_id;	/* Inodes bitmap block */
	__u32 inode_table_id;	/* Inodes table block */
	__u16 free_blocks;	/* Free blocks count */
	__u16 free_inodes;	/* Free inodes count */
	__u16 used_dir_cnt;	/* Directories count */
	__u16 bg_flags;
	__u32 bg_reserved[2];
	__u16 bg_itable_unused; /* Unused inodes count */
	__u16 bg_checksum;	/* crc16(s_uuid+grouo_num+group_desc)*/
};

struct ext4_group_desc
{
 __le32 bg_block_bitmap_lo;
 __le32 bg_inode_bitmap_lo;
 __le32 bg_inode_table_lo;
 __le16 bg_free_blocks_count_lo;
 __le16 bg_free_inodes_count_lo;
 __le16 bg_used_dirs_count_lo;
 __le16 bg_flags;
 __u32 bg_reserved[2];
 __le16 bg_itable_unused_lo;
 __le16 bg_checksum;
 __le32 bg_block_bitmap_hi;
 __le32 bg_inode_bitmap_hi;
 __le32 bg_inode_table_hi;
 __le16 bg_free_blocks_count_hi;
 __le16 bg_free_inodes_count_hi;
 __le16 bg_used_dirs_count_hi;
 __le16 bg_itable_unused_hi;
 __u32 bg_reserved2[3];
};

/* The ext2 inode. */
struct ext2_inode {
	uint16_t mode;
	uint16_t uid;
	uint32_t size;
	uint32_t atime;
	uint32_t ctime;
	uint32_t mtime;
	uint32_t dtime;
	uint16_t gid;
	uint16_t nlinks;
	uint32_t blockcnt;	/* Blocks of 512 bytes!! */
	uint32_t flags;
	uint32_t osd1;
	union {
		struct datablocks {
			uint32_t dir_blocks[INDIRECT_BLOCKS];
			uint32_t indir_block;
			uint32_t double_indir_block;
			uint32_t triple_indir_block;
		} blocks;
		char symlink[60];
	} b;
	uint32_t version;
	uint32_t acl;
	uint32_t dir_acl;
	uint32_t fragment_addr;
	uint32_t osd2[3];
};
/* The ext4 inode. */
struct ext4_inode {
	uint16_t i_mode;
 	uint16_t i_uid;
	uint32_t i_size_lo;
 	uint32_t i_atime;
 	uint32_t i_ctime;
 	uint32_t i_mtime;
 	uint32_t i_dtime;
 	uint16_t i_gid;
 	uint16_t i_links_count;
 	uint32_t i_blocks_lo;
 	uint32_t i_flags;
 union {
 struct {
 	uint32_t l_i_version;
 } linux1;
 struct {
 	__u32 h_i_translator;
 } hurd1;
 struct {
 	__u32 m_i_reserved1;
 } masix1;
 } osd1;
 	uint32_t i_block[EXT4_N_BLOCKS];
 	uint32_t i_generation;
 	uint32_t i_file_acl_lo;
 	uint32_t i_size_high;
 	uint32_t i_obso_faddr;
 union {
 struct {
 uint16_t l_i_blocks_high;
 uint16_t l_i_file_acl_high;
 uint16_t l_i_uid_high;
 uint16_t l_i_gid_high;
 __u32 l_i_reserved2;
 } linux2;
 struct {
 uint16_t h_i_reserved1;
 __u16 h_i_mode_high;
 __u16 h_i_uid_high;
 __u16 h_i_gid_high;
 __u32 h_i_author;
 } hurd2;
 struct {
 uint16_t h_i_reserved1;
 uint16_t m_i_file_acl_high;
 __u32 m_i_reserved2[2];
 } masix2;
 } osd2;
 uint16_t i_extra_isize;
 uint16_t i_pad1;
 uint32_t i_ctime_extra;
 uint32_t i_mtime_extra;
 uint32_t i_atime_extra;
 uint32_t i_crtime;
 uint32_t i_crtime_extra;
 uint32_t i_version_hi;
};

/* The header of an ext2 directory entry. */
struct ext2_dirent {
	uint32_t inode;
	uint16_t direntlen;
	uint8_t namelen;
	uint8_t filetype;
};

struct ext2fs_node {
	struct ext2_data *data;
	struct ext2_inode inode;
	int ino;
	int inode_read;
};

/* Information about a "mounted" ext2 filesystem. */
struct ext2_data {
	struct ext2_sblock sblock;
	struct ext2_inode *inode;
	struct ext2fs_node diropen;
};
#endif
