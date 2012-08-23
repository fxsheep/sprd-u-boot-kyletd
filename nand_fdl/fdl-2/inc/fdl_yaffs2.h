#ifndef FDL_YAFFS2_H
#define FDL_YAFFS2_H


#define YAFFS_MAX_NAME_LENGTH		255
#define YAFFS_MAX_ALIAS_LENGTH		159

#define BOOT_MAGIC_SIZE 8
#define BOOT_NAME_SIZE 16
#define BOOT_ARGS_SIZE 512

#define YAFFS_LOWEST_SEQUENCE_NUMBER	0x00001000
#define YAFFS_HIGHEST_SEQUENCE_NUMBER	0xEFFFFF00

#ifndef __u16
 #define __u16 unsigned short
#endif

#ifndef __u32
 #define __u32 unsigned int
#endif

#ifndef __u8
 #define __u8 unsigned char
#endif

typedef enum {
	YAFFS_ECC_RESULT_UNKNOWN,
	YAFFS_ECC_RESULT_NO_ERROR,
	YAFFS_ECC_RESULT_FIXED,
	YAFFS_ECC_RESULT_UNFIXED
} yaffs_ECCResult;

typedef enum {
	YAFFS_OBJECT_TYPE_UNKNOWN,
	YAFFS_OBJECT_TYPE_FILE,
	YAFFS_OBJECT_TYPE_SYMLINK,
	YAFFS_OBJECT_TYPE_DIRECTORY,
	YAFFS_OBJECT_TYPE_HARDLINK,
	YAFFS_OBJECT_TYPE_SPECIAL
} yaffs_ObjectType;

typedef struct {

	unsigned validMarker0;
	unsigned chunkUsed;	/*  Status of the chunk: used or unused */
	unsigned objectId;	/* If 0 then this is not part of an object (unused) */
	unsigned chunkId;	/* If 0 then this is a header, else a data chunk */
	unsigned byteCount;	/* Only valid for data chunks */

	/* The following stuff only has meaning when we read */
	yaffs_ECCResult eccResult;
	unsigned blockBad;

	/* YAFFS 1 stuff */
	unsigned chunkDeleted;	/* The chunk is marked deleted */
	unsigned serialNumber;	/* Yaffs1 2-bit serial number */

	/* YAFFS2 stuff */
	unsigned sequenceNumber;	/* The sequence number of this block */

	/* Extra info if this is an object header (YAFFS2 only) */

	unsigned extraHeaderInfoAvailable;	/* There is extra info available if this is not zero */
	unsigned extraParentObjectId;	/* The parent object */
	unsigned extraIsShrinkHeader;	/* Is it a shrink header? */
	unsigned extraShadows;		/* Does this shadow another object? */

	yaffs_ObjectType extraObjectType;	/* What object type? */

	unsigned extraFileLength;		/* Length if it is a file */
	unsigned extraEquivalentObjectId;	/* Equivalent object Id if it is a hard link */

	unsigned validMarker1;

} yaffs_ExtendedTags;

/* -------------------------- Object structure -------------------------------*/
/* This is the object structure as stored on NAND */

typedef struct {
	yaffs_ObjectType type;

	/* Apply to everything  */
	int parentObjectId;
	__u16 sum__NoLongerUsed;	/* checksum of name. No longer used */
	char name[YAFFS_MAX_NAME_LENGTH + 1];

	/* Thes following apply to directories, files, symlinks - not hard links */
	__u32 yst_mode;		/* protection */

#ifdef CONFIG_YAFFS_WINCE
	__u32 notForWinCE[5];
#else
	__u32 yst_uid;
	__u32 yst_gid;
	__u32 yst_atime;
	__u32 yst_mtime;
	__u32 yst_ctime;
#endif

	/* File size  applies to files only */
	int fileSize;

	/* Equivalent object id applies to hard links only. */
	int equivalentObjectId;

	/* Alias is for symlinks only. */
	char alias[YAFFS_MAX_ALIAS_LENGTH + 1];

	__u32 yst_rdev;		/* device stuff for block and char devices (major/min) */

#ifdef CONFIG_YAFFS_WINCE
	__u32 win_ctime[2];
	__u32 win_atime[2];
	__u32 win_mtime[2];
	__u32 roomToGrow[4];
#else
	__u32 roomToGrow[10];
#endif

	int shadowsObject;	/* This object header shadows the specified object if > 0 */

	/* isShrink applies to object headers written when we shrink the file (ie resize) */
	__u32 isShrink;

} yaffs_ObjectHeader;


typedef struct {
	unsigned char colParity;
	unsigned lineParity;
	unsigned lineParityPrime;
} yaffs_ECCOther;

typedef struct {
	unsigned sequenceNumber;
	unsigned objectId;
	unsigned chunkId;
	unsigned byteCount;
} yaffs_PackedTags2TagsPart;

typedef struct {
	yaffs_PackedTags2TagsPart t;
	yaffs_ECCOther ecc;
} yaffs_PackedTags2;

typedef struct{
    unsigned char magic[BOOT_MAGIC_SIZE];

    unsigned kernel_size;  /* size in bytes */
    unsigned kernel_addr;  /* physical load addr */

    unsigned ramdisk_size; /* size in bytes */
    unsigned ramdisk_addr; /* physical load addr */

    unsigned second_size;  /* size in bytes */
    unsigned second_addr;  /* physical load addr */

    unsigned tags_addr;    /* physical addr for kernel tags */
    unsigned page_size;    /* flash page size we assume */
    unsigned unused[2];    /* future expansion: should be 0 */

    unsigned char name[BOOT_NAME_SIZE]; /* asciiz product name */
    unsigned char cmdline[BOOT_ARGS_SIZE];

    unsigned id[8]; /* timestamp / checksum / sha1 / etc */
}bootimg_hdr;

void yaffs2_InitialiseTags(yaffs_ExtendedTags * tags);
void yaffs2_PackTags2(yaffs_PackedTags2 * pt, const yaffs_ExtendedTags * t);

#endif /* FDL_YAFFS2_H */
