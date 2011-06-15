#ifndef YAFFS2_PARSRMTD_H
#define YAFFS2_PARSRMTD_H

struct mtd_partition {
	char *name;			/* identifier string */
	unsigned long size;			/* partition size */
	unsigned long offset;		/* offset within the master MTD space */
	unsigned long mask_flags;		/* master MTD flags to mask out for this partition */
};

int yaffs_parse_cmdline_partitions(struct mtd_partition *current, unsigned long long mastersize);
#endif /* YAFFS2_NAND_H */
