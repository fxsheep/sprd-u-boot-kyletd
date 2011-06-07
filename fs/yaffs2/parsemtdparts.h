#ifndef FDL_PARSRMTD_H
#define FDL_PARSRMTD_H

struct mtd_partition {
	char *name;			/* identifier string */
	unsigned long size;			/* partition size */
	unsigned long offset;		/* offset within the master MTD space */
	unsigned long mask_flags;		/* master MTD flags to mask out for this partition */
};

int parse_cmdline_partitions(struct mtd_partition *current, unsigned long long mastersize);
#endif /* FDL_NAND_H */
