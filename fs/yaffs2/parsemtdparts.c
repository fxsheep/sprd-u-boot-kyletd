#include <linux/ctype.h>
#include "parsemtdparts.h"
#include <linux/string.h>
#include <malloc.h>

#include "asm/arch/sci_types.h"
#include "asm/arch/nand_controller.h"
#include <linux/mtd/mtd.h>
#include <nand.h>
#include <linux/mtd/nand.h>

#define MTD_WRITEABLE		0x400	/* Device is writeable */
#define MTD_BIT_WRITEABLE	0x800	/* Single bits can be flipped */
#define MTD_NO_ERASE		0x1000	/* No erase necessary */
#define MTD_POWERUP_LOCK	0x2000	/* Always locked after reset */

#define SIZE_REMAINING 		0xffffffff
#define OFFSET_CONTINUOUS	0xffffffff
#define MTDPARTITION_MAX		40

struct cmdline_mtd_partition {
	struct cmdline_mtd_partition *next;
	char *mtd_id;
	int num_parts;
	struct mtd_partition *parts;
};

static struct mtd_partition realpart[MTDPARTITION_MAX];
static char realpartname[MTDPARTITION_MAX][255];
static int current_part = MTDPARTITION_MAX;

/* the command line passed to mtdpart_setupd() */
static char *cmdline;
static int cmdline_parsed = 0;


#define TOLOWER(x) ((x) | 0x20)

static unsigned int simple_guess_base(const char *cp)
{
	if (cp[0] == '0') {
		if (TOLOWER(cp[1]) == 'x' && isxdigit(cp[2]))
			return 16;
		else
			return 8;
	} else {
		return 10;
	}
}

static unsigned long long memparse(const char *ptr, char **retptr)
{
	char *endptr;	/* local pointer to end of parsed string */
	unsigned long long ret = simple_strtoull(ptr, &endptr, 0);

	switch (*endptr) {
	case 'G':
	case 'g':
		ret <<= 10;
	case 'M':
	case 'm':
		ret <<= 10;
	case 'K':
	case 'k':
		ret <<= 10;
		endptr++;
	default:
		break;
	}

	if (retptr)
		*retptr = endptr;

	return ret;
}

static size_t strlcpy(char *dest, char *src, size_t size)
{
	size_t ret = strlen(src);

	if (size) {
		size_t len = (ret >= size) ? size - 1 : ret;
		memcpy(dest, src, len);
		dest[len] = '\0';
	}
	return ret;
}

static struct mtd_partition * newpart(char *s,
                                      char **retptr,
                                      int *num_parts,
                                      int this_part,
                                      unsigned char **extra_mem_ptr,
                                      int extra_mem_size)
{
	struct mtd_partition *parts;
	unsigned long size;
	unsigned long offset = OFFSET_CONTINUOUS;
	char *name;
	int name_len;
	unsigned char *extra_mem;
	char delim;
	unsigned int mask_flags;

	/* fetch the partition size */
	if (*s == '-')
	{	/* assign all remaining space to this partition */
		size = SIZE_REMAINING;
		s++;
	}
	else
	{
		size = memparse(s, &s);
		if (size < PAGE_SIZE)
		{
			printf("partition size too small (%lx)\n", size);
			return 0;
		}
	}

	/* fetch partition name and flags */
	mask_flags = 0; /* this is going to be a regular partition */
	delim = 0;
        /* check for offset */
        if (*s == '@')
	{
                s++;
                offset = memparse(s, &s);
        }
        /* now look for name */
	if (*s == '(')
	{
		delim = ')';
	}

	if (delim)
	{
		char *p;

	    	name = ++s;
		p = strchr(name, delim);
		if (!p)
		{
			printf("no closing %c found in partition name\n", delim);
			return 0;
		}
		name_len = p - name;
		s = p + 1;
	}
	else
	{
	    	name = 0;
		name_len = 13; /* Partition_000 */
	}

	/* record name length for memory allocation later */
	extra_mem_size += name_len + 1;

        /* test for options */
        if (strncmp(s, "ro", 2) == 0)
	{
		mask_flags |= MTD_WRITEABLE;
		s += 2;
        }

        /* if lk is found do NOT unlock the MTD partition*/
        if (strncmp(s, "lk", 2) == 0)
	{
		mask_flags |= MTD_POWERUP_LOCK;
		s += 2;
        }

	/* test if more partitions are following */
	if (*s == ',')
	{
		if (size == SIZE_REMAINING)
		{
			printf("no partitions allowed after a fill-up partition\n");
			return 0;
		}
		/* more partitions follow, parse them */
		parts = newpart(s + 1, &s, num_parts, this_part + 1,
				&extra_mem, extra_mem_size);
		if (!parts)
			return 0;
	}
	else
	{	/* this is the last partition: allocate space for all */
		int alloc_size;

		*num_parts = this_part + 1;
		alloc_size = *num_parts * sizeof(struct mtd_partition) +
			     extra_mem_size;
		parts = (struct mtd_partition *)malloc(alloc_size);
		if (!parts)
		{
			printf("out of memory\n");
			return 0;
		}
		extra_mem = (unsigned char *)(parts + *num_parts);
	}
	/* enter this partition (offset will be calculated later if it is zero at this point) */
	parts[this_part].size = size;
	parts[this_part].offset = offset;
	parts[this_part].mask_flags = mask_flags;
	if (name)
	{
		strlcpy(extra_mem, name, name_len + 1);
	}
	else
	{
		sprintf(extra_mem, "Partition_%03d", this_part);
	}
	parts[this_part].name = extra_mem;
	extra_mem += name_len + 1;

	/*printf("partition %02d: name <%14s>, offset %08x, size %08x, mask flags %08x\n",
	     this_part,
	     parts[this_part].name,
	     parts[this_part].offset,
	     parts[this_part].size,
	     parts[this_part].mask_flags);*/
	
	     if (current_part >= 0) {
		current_part --;
		memset(&(realpartname[current_part]), 0, 255);
		strcpy(realpartname[current_part], parts[this_part].name);
		realpart[current_part].name = realpartname[current_part];
		//printf("realname = %14s  partname = %14s\n", realpart[current_part].name, parts[this_part].name);
		realpart[current_part].offset = parts[this_part].offset;
		realpart[current_part].size = parts[this_part].size;
	    } else
		printf("mtdparts partition is too much\n");


	/* return (updated) pointer to extra_mem memory */
	if (extra_mem_ptr)
	  *extra_mem_ptr = extra_mem;

	/* return (updated) pointer command line string */
	*retptr = s;

	/* return partition table */
	return parts;
}

/*
 * Parse the command line.
 */
static int mtdpart_setup_real(char *s)
{
	cmdline_parsed = 1;

	for( ; s != '\0'; )
	{
		struct cmdline_mtd_partition *this_mtd;
		struct mtd_partition *parts;
	    	int mtd_id_len;
		int num_parts;
		char *p, *mtd_id;

	    	mtd_id = s;
		/* fetch <mtd-id> */
		if (!(p = strchr(s, ':')))
		{
			printf("no mtd-id\n");
			return 0;
		}
		mtd_id_len = p - mtd_id;

		//printf("\nparsing <%s>\n", p+1);

		/*
		 * parse one mtd. have it reserve memory for the
		 * struct cmdline_mtd_partition and the mtd-id string.
		 */
		parts = newpart(p + 1,		/* cmdline */
				&s,		/* out: updated cmdline ptr */
				&num_parts,	/* out: number of parts */
				0,		/* first partition */
				(unsigned char**)&this_mtd, /* out: extra mem */
				mtd_id_len + 1 + sizeof(*this_mtd) +
				sizeof(void*)-1 /*alignment*/);
		if(!parts)
		{
			/*
			 * An error occurred. We're either:
			 * a) out of memory, or
			 * b) in the middle of the partition spec
			 * Either way, this mtd is hosed and we're
			 * unlikely to succeed in parsing any more
			 */
			 return 0;
		 }

		//printf("mtdid=<%s> num_parts=<%d>\n", this_mtd->mtd_id, this_mtd->num_parts);
		/* EOS - we're done */
		if (*s == 0)
			break;

		/* does another spec follow? */
		if (*s != ';')
		{
			printf("bad character after partition (%c)\n", *s);
			return 0;
		}
		s++;
	}
	return 1;
}


int yaffs_parse_cmdline_partitions(struct mtd_partition *current, unsigned long long mastersize)
{
	unsigned long offset;
	int i;
	struct cmdline_mtd_partition *part;

	cmdline = MTDPARTS_DEFAULT;

	/* parse command line */
	if (!cmdline_parsed) {
		mtdpart_setup_real(cmdline);

		for (i = current_part, offset = 0; i < MTDPARTITION_MAX; i ++) {
			if (realpart[i].offset == OFFSET_CONTINUOUS)
			  		realpart[i].offset = offset;
				else
			  		offset = realpart[i].offset;
			
				if (realpart[i].size == SIZE_REMAINING)
			  		realpart[i].size = mastersize - offset;

				if (offset + realpart[i].size > mastersize) {
					printf("partitioning exceeds flash size, truncating\n");
					realpart[i].size = mastersize - offset;
				}
				offset += realpart[i].size;


				/*printf("realpart %02d : offset %08x, size %08x  name %14s\n",
	     			(i - current_part),
	     			realpart[i].offset,
	     			realpart[i].size,
				realpart[i].name);*/
		}
	}

	
	for (i = current_part; i < MTDPARTITION_MAX; i ++) {
		if (realpart[i].offset == current->offset) {
			current->size = realpart[i].size;
			strcpy(current->name, realpart[i].name);
			return 0;
		}

		if (strcmp(realpart[i].name, current->name) == 0) {
			current->size = realpart[i].size;
			current->offset = realpart[i].offset;
			return 0;
		}
	}

	return 1;
}


