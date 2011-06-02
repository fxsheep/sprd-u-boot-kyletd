#include "fdl_nand.h"
#include "asm/arch/sci_types.h"
#include "asm/arch/nand_controller.h"
#include <linux/mtd/mtd.h>
#include <nand.h>
#include <linux/mtd/nand.h>
#include <jffs2/jffs2.h>
#include <parsemtdparts.h>


//#define FDL2_DEBUG 1
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

#define ADDR_MASK				0x80000000
#define SYSTEM_WRITE_MASK (0xC0000000)

static unsigned int nand_write_size ;
static unsigned int nand_write_addr;
static unsigned int cur_write_pos;
static unsigned int is_system_write;

int nand_flash_init(void)
{
	nand_init();
	return 0;
}

int nand_format(void)
{
#ifdef FDL2_DEBUG
	printf("function: %s\n", __FUNCTION__);
#endif
	struct mtd_info *nand;
	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  return NAND_SYSTEM_ERROR;
	nand = &nand_info[nand_curr_device];
	
#ifdef FDL2_DEBUG
	printf("function: %s nand_curr_device is %d\n", __FUNCTION__, nand_curr_device);
#endif
	unsigned int off = nand->erasesize; //TO SKIP THE FIRST BLOCK
	unsigned int size = nand->size - off; //to erase the left blocks

	nand_erase_options_t opts;
	memset(&opts, 0, sizeof(opts));
	opts.offset = off;
	opts.length = size;
	opts.jffs2 = 0;
	opts.scrub = 0;
	
#ifdef FDL2_DEBUG
	printf("function: %s erase off 0x%x length: 0x%x jffs2: 0x%x scurb: 0x%x\n",__FUNCTION__, opts.offset, opts.length, opts.jffs2, opts.scrub);
#endif
	return nand_erase_opts(nand, &opts);
}

int nand_erase_partition(unsigned int addr, unsigned int size)
{
	struct mtd_partition cur_partition;
	int erase_blk, ret = 0;
	struct mtd_info *nand;

	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  	return NAND_SYSTEM_ERROR;

	nand = &nand_info[nand_curr_device];

	if(addr & (nand->erasesize - 1))
	  	return NAND_INVALID_ADDR;

	if((addr & SYSTEM_WRITE_MASK)==SYSTEM_WRITE_MASK){
		addr &= ~SYSTEM_WRITE_MASK;
	}else if((addr & ADDR_MASK)==ADDR_MASK){
		addr &= ~ADDR_MASK;
	}else 
	  	return NAND_INVALID_ADDR;


	//printf("function %s, addr 0x%x, size 0x%x\n", __FUNCTION__, addr, size);
	cur_partition.offset = addr;
	cur_partition.size = 0;
	parse_cmdline_partitions(&cur_partition, (unsigned long long)nand->size);
	//printf("\naddr = 0x%08x  size = 0x%08x  offset %08x, size %08x\n", addr, size, cur_partition.offset, cur_partition.size);

	for (erase_blk = 0; erase_blk < (cur_partition.size / nand->erasesize); erase_blk ++) {
		printf("erasing block : %d    %d % \r", (cur_partition.offset / nand->erasesize + erase_blk), (erase_blk * 100 ) / (cur_partition.size / nand->erasesize));
		if (nand_block_isbad(nand, cur_partition.offset + erase_blk * nand->erasesize)) {
			printf("\nerase block : %d is bad\n", (cur_partition.offset / nand->erasesize + erase_blk));
		} else {
			ret = nand_erase_fdl(cur_partition.offset + erase_blk * nand->erasesize, nand->erasesize);
			if (ret != 0) {
				/* erase failed */
				ret = nand->block_markbad(nand, cur_partition.offset + erase_blk * nand->erasesize);
				if (ret != 0) {
					/* mark failed */
					printf("mark 0x%x bad error\n", cur_partition.offset + erase_blk * nand->erasesize);
				   	return NAND_SYSTEM_ERROR + 1;
				} else {
					/* mark success */
					printf("skip bad block 0x%x\n", cur_partition.offset + erase_blk * nand->erasesize);
				}
			}	
		}
	}
	
	return NAND_SUCCESS;
}

/*
*   addr : block start address in bytes
*   size : block length in bytes
*   ret  : < 0 : non-0xff block ; 0 : all-0xff block
*/
int nand_check_data(unsigned int addr, unsigned int size)
{

	struct mtd_info *nand;
	struct mtd_oob_ops ops;
	int ret = 0;
	unsigned char buffer[4096];
	unsigned long pos, cur;
	int aaa;

	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  	return NAND_SYSTEM_ERROR;

	nand = &nand_info[nand_curr_device];

	if(size != nand->erasesize)
	  return NAND_INVALID_SIZE;
	

	for (pos = addr; pos < (addr + size); pos += nand->writesize) {
		//printf("pos = 0x%08x\n", pos);
		ops.mode = MTD_OOB_AUTO;
		ops.len = nand->writesize;
		ops.datbuf = (uint8_t *)buffer;
		ops.oobbuf = (uint8_t *)buffer + nand->writesize; 
		ops.ooblen = 64; 
		ops.ooboffs = 0;
		memset(buffer, 0x5a, 4096);
		ret = nand_do_read_ops(nand, (unsigned long long)pos, &ops);
		/*if ((pos) == 0x39d4000) {
			printf("1 ret = %d\n", ret);
			printf("\n\nread data :\n");
			for (aaa = 0; aaa < 2048; aaa ++) {
				if ((aaa % 16) == 0)
					printf("\n");
				printf(" %02x", buffer[aaa]);
			}
			printf("\n\nread oob :\n");
			for (aaa = 0; aaa < 64; aaa ++) {
				if ((aaa % 16) == 0)
					printf("\n");
				printf(" %02x", buffer[2048 + aaa]);
			}
			printf("\n\n");
		}*/

		if (ret < 0) {
			printf("read data error\n");
			return -1;
		} else {
			/* check data */
			for (cur = 0; cur < nand->writesize; cur ++)
				if (buffer[cur] != 0xff)
					return -1;
			ret = 0;
		}
	}
		
	return ret;
}

int nand_erase_check_partition(unsigned int addr, unsigned int size)
{
	struct mtd_partition cur_partition;
	int erase_blk, ret = 0;
	struct mtd_info *nand;

	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  	return NAND_SYSTEM_ERROR;

	nand = &nand_info[nand_curr_device];

	if(addr & (nand->erasesize - 1))
	  	return NAND_INVALID_ADDR;

	if((addr & SYSTEM_WRITE_MASK)==SYSTEM_WRITE_MASK){
		addr &= ~SYSTEM_WRITE_MASK;
	}else if((addr & ADDR_MASK)==ADDR_MASK){
		addr &= ~ADDR_MASK;
	}else 
	  	return NAND_INVALID_ADDR;

	cur_partition.offset = addr;
	cur_partition.size = 0;
	parse_cmdline_partitions(&cur_partition, (unsigned long long)nand->size);
	//printf("\naddr = 0x%08x  size = 0x%08x  offset %08x, size %08x\n", addr, size, cur_partition.offset, cur_partition.size);

	for (erase_blk = 0; erase_blk < (cur_partition.size / nand->erasesize); erase_blk ++) {
		printf("erasing block : %d     %d  %\r", (cur_partition.offset / nand->erasesize + erase_blk), (erase_blk * 100 ) / (cur_partition.size / nand->erasesize));
		if (nand_block_isbad(nand, cur_partition.offset + erase_blk * nand->erasesize)) {
			printf("\nerase block : %d  is bad\n", (cur_partition.offset / nand->erasesize + erase_blk));
		} else {
			ret = nand_erase_fdl(cur_partition.offset + erase_blk * nand->erasesize, nand->erasesize);
			if (ret != 0) {
				/* erase failed */
				ret = nand->block_markbad(nand, cur_partition.offset + erase_blk * nand->erasesize);
				if (ret != 0) {
					/* mark failed */
					printf("mark 0x%x bad error\n", cur_partition.offset + erase_blk * nand->erasesize);
					return NAND_SYSTEM_ERROR + 1;
				} else {
					/* mark success */
					printf("skip bad block 0x%x\n", cur_partition.offset + erase_blk * nand->erasesize);
				}
			}

		}
	}
	
	//printf("erase finish, then check data\n");
	for (erase_blk = 0; erase_blk < (cur_partition.size / nand->erasesize); erase_blk ++) {
		printf("checking block : %d     %d  %\r", (cur_partition.offset / nand->erasesize + erase_blk), (erase_blk * 100 ) / (cur_partition.size / nand->erasesize));
		if (nand_block_isbad(nand, cur_partition.offset + erase_blk * nand->erasesize)) {
			printf("\ncheck block : %d is bad\n", (cur_partition.offset / nand->erasesize + erase_blk));
		} else {
			ret = nand_check_data(cur_partition.offset + erase_blk * nand->erasesize, nand->erasesize);
			if (ret != 0) {
				ret = nand->block_markbad(nand, cur_partition.offset + erase_blk * nand->erasesize);
				if (ret != 0) {
					/* mark failed */
					printf("mark 0x%x bad error\n", cur_partition.offset + erase_blk * nand->erasesize);
				   	return NAND_SYSTEM_ERROR + 1;
				} else {
					/* mark success */
					printf("skip bad block 0x%x\n", cur_partition.offset + erase_blk * nand->erasesize);
				}
			}
		}
	}

	return NAND_SUCCESS;
}

/*
*   addr : block start address in bytes
*   size : block length in bytes
*   ret  : < 0 : bad block ; 0 : good block
*/
int check_write_read_block(struct mtd_info *nand, unsigned int addr, unsigned int size, int mode)
{
	struct mtd_oob_ops ops;
	int ret = 0;
	unsigned char buffer[4096];
	unsigned long pos, cur;
	unsigned long chunksize;
	int aaa;

	for (pos = addr; pos < (addr + size); pos += nand->writesize) {
		ret = 0;
		for (aaa = 0; aaa < 4096; aaa ++)
				buffer[aaa] = aaa % 0xff;
		if (!mode) {
			chunksize = nand->writesize;
			ret = nand_write_skip_bad(nand, pos, &chunksize, buffer);
			if (0 != ret) {
				return -1;
			}
		} else { // system write
			ops.mode = MTD_OOB_AUTO;
			ops.len = nand->writesize;
			ops.datbuf = (uint8_t *)buffer;
			ops.oobbuf = (uint8_t *)buffer + nand->writesize;
			ops.ooblen = sizeof(yaffs_PackedTags2);
			ops.ooboffs = 0;
			/*if ((pos) == 0x39d4000) {
				printf("1 ret = %d\n", ret);
				printf("\n\nread data :\n");
				for (aaa = 0; aaa < 2048; aaa ++) {
					if ((aaa % 16) == 0)
						printf("\n");
					printf(" %02x", buffer[aaa]);
				}
				printf("\n\nread oob :\n");
				for (aaa = 0; aaa < 64; aaa ++) {
					if ((aaa % 16) == 0)
						printf("\n");
					printf(" %02x", buffer[2048 + aaa]);
				}
				printf("\n\n");
			}*/
			ret = nand_do_write_ops(nand, (unsigned long long)pos, &ops);
			if (0 != ret)
				return -1;
		}
		if (ret == 0) {
			ops.mode = MTD_OOB_AUTO;
			ops.len = nand->writesize;
			ops.datbuf = (uint8_t *)buffer;
			ops.oobbuf = (uint8_t *)buffer + nand->writesize; 
			ops.ooblen = 64; 
			ops.ooboffs = 0;
			memset(buffer, 0x5a, 4096);
			ret = nand_do_read_ops(nand, (unsigned long long)pos, &ops);
			
			if(ret != 0)
				return -1;
		}
	}
	
	return ret;
}

int nand_erase_check_write_partition(unsigned int addr, unsigned int size)
{
	struct mtd_partition cur_partition;
	int erase_blk, ret = 0;
	struct mtd_info *nand;

	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  	return NAND_SYSTEM_ERROR;

	nand = &nand_info[nand_curr_device];

	if(addr & (nand->erasesize - 1))
	  	return NAND_INVALID_ADDR;

	if((addr & SYSTEM_WRITE_MASK)==SYSTEM_WRITE_MASK){
		addr &= ~SYSTEM_WRITE_MASK;
	}else if((addr & ADDR_MASK)==ADDR_MASK){
		addr &= ~ADDR_MASK;
	}else 
	  	return NAND_INVALID_ADDR;

	cur_partition.offset = addr;
	cur_partition.size = 0;
	parse_cmdline_partitions(&cur_partition, (unsigned long long)nand->size);

	for (erase_blk = 0; erase_blk < (cur_partition.size / nand->erasesize); erase_blk ++) {
		printf("erasing block : %d     %d  %\r", (cur_partition.offset / nand->erasesize + erase_blk), (erase_blk * 100 ) / (cur_partition.size / nand->erasesize));
		if (nand_block_isbad(nand, cur_partition.offset + erase_blk * nand->erasesize)) {
			printf("\nerase block : %d  is bad\n", (cur_partition.offset / nand->erasesize + erase_blk));
		} else {
			ret = nand_erase_fdl(cur_partition.offset + erase_blk * nand->erasesize, nand->erasesize);
			if (ret != 0) {
				/* erase failed */
				ret = nand->block_markbad(nand, cur_partition.offset + erase_blk * nand->erasesize);
				if (ret != 0) {
					/* mark failed */
					printf("mark 0x%x bad error\n", cur_partition.offset + erase_blk * nand->erasesize);
					return NAND_SYSTEM_ERROR + 1;
				} else {
					/* mark success */
					printf("skip bad block 0x%x\n", cur_partition.offset + erase_blk * nand->erasesize);
				}
			}

		}
	}
	
	//printf("erase finish, then check data\n");
	for (erase_blk = 0; erase_blk < (cur_partition.size / nand->erasesize); erase_blk ++) {
		printf("checking block : %d    %d  % \r", (cur_partition.offset / nand->erasesize + erase_blk), (erase_blk * 100 ) / (cur_partition.size / nand->erasesize));
		if (nand_block_isbad(nand, cur_partition.offset + erase_blk * nand->erasesize)) {
			printf("\ncheck block :  %d block is bad\n", (cur_partition.offset / nand->erasesize + erase_blk));
		} else {
			ret = nand_check_data(cur_partition.offset + erase_blk * nand->erasesize, nand->erasesize);
			if (ret != 0) {
				ret = nand->block_markbad(nand, cur_partition.offset + erase_blk * nand->erasesize);
				if (ret != 0) {
					/* mark failed */
					printf("mark 0x%x bad error\n", cur_partition.offset + erase_blk * nand->erasesize);
				   	return NAND_SYSTEM_ERROR + 1;
				} else {
					/* mark success */
					printf("skip bad block 0x%x\n", cur_partition.offset + erase_blk * nand->erasesize);
				}
			}
		}
	}

	//printf("erase finish, then check data, last write data\n");
	for (erase_blk = 0; erase_blk < (cur_partition.size / nand->erasesize); erase_blk ++) {
		printf("writing block : %d     %d % \r", (cur_partition.offset / nand->erasesize + erase_blk), (erase_blk * 100 ) / (cur_partition.size / nand->erasesize));
		if (nand_block_isbad(nand, cur_partition.offset + erase_blk * nand->erasesize)) {
			printf("\nwrite block : %d is bad\n", (cur_partition.offset / nand->erasesize + erase_blk));
		} else {
			ret = check_write_read_block(nand, cur_partition.offset + erase_blk * nand->erasesize, nand->erasesize, is_system_write);
			if (ret != 0) {
				ret = nand->block_markbad(nand, cur_partition.offset + erase_blk * nand->erasesize);
				if (ret != 0) {
					/* mark failed */
					printf("mark 0x%x bad error\n", cur_partition.offset + erase_blk * nand->erasesize);
				   	return 1;
				} else {
					/* mark success */
					printf("skip bad block 0x%x\n", cur_partition.offset + erase_blk * nand->erasesize);
				}
			}
		}
	}

	/* erase again */
	for (erase_blk = 0; erase_blk < (cur_partition.size / nand->erasesize); erase_blk ++) {
		printf("erasing block : %d    %d  % \r", (cur_partition.offset / nand->erasesize + erase_blk), (erase_blk * 100 ) / (cur_partition.size / nand->erasesize));
		if (nand_block_isbad(nand, cur_partition.offset + erase_blk * nand->erasesize)) {
			printf("\nerase block : %d is bad bad\n", (cur_partition.offset / nand->erasesize + erase_blk));
		} else {
			ret = nand_erase_fdl(cur_partition.offset + erase_blk * nand->erasesize, nand->erasesize);
			if (ret != 0) {
				/* erase failed */
				ret = nand->block_markbad(nand, cur_partition.offset + erase_blk * nand->erasesize);
				if (ret != 0) {
					/* mark failed */
					printf("mark 0x%x bad error\n", cur_partition.offset + erase_blk * nand->erasesize);
					return NAND_SYSTEM_ERROR + 1;
				} else {
					/* mark success */
					printf("skip bad block 0x%x\n", cur_partition.offset + erase_blk * nand->erasesize);
				}
			}

		}
	}

	return NAND_SUCCESS;

}



int nand_erase_fdl(unsigned int addr, unsigned int size)
{
#ifdef FDL2_DEBUG
	printf("function: %s\n", __FUNCTION__);
#endif
	struct mtd_info *nand;
	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  	return NAND_SYSTEM_ERROR;
	nand = &nand_info[nand_curr_device];

	/*if(addr < nand->erasesize) //to skip the first block erase
	  return NAND_INVALID_ADDR;*/
	if(size != nand->erasesize)
	  return NAND_INVALID_SIZE;
	
	nand_erase_options_t opts;
	memset(&opts, 0, sizeof(opts));
	opts.offset = addr;
	opts.length = size;
	opts.jffs2 = 0;
	opts.quiet = 1;
#ifdef FDL2_DEBUG
	printf("function: %s erase off 0x%x length: 0x%x jffs2: 0x%x scurb: 0x%x\n",__FUNCTION__, opts.offset, opts.length, opts.jffs2, opts.scrub);
#endif
	return nand_erase_opts(nand, &opts);
}

int nand_start_write(unsigned int addr, unsigned int size)
{
	struct mtd_partition cur_partition;
	int erase_blk, ret = 0;

#ifdef FDL2_DEBUG
	printf("function: %s\n", __FUNCTION__);
#endif
	struct mtd_info *nand;
	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  return NAND_SYSTEM_ERROR;
	nand = &nand_info[nand_curr_device];
	//printf("nand->size = 0x%016Lx\n", (unsigned long long)nand->size);
#ifdef FDL2_DEBUG
	printf("function: %s write addr: 0x%x erasesize: 0x%x\n", __FUNCTION__, addr, nand->erasesize);
#endif
	if(addr & (nand->erasesize - 1))
	  return NAND_INVALID_ADDR;

	if((addr & SYSTEM_WRITE_MASK)==SYSTEM_WRITE_MASK){
		addr &= ~SYSTEM_WRITE_MASK;
		is_system_write = 1;
	}else if((addr & ADDR_MASK)==ADDR_MASK){
		addr &= ~ADDR_MASK;
		is_system_write = 0;
	}else 
	  return NAND_INVALID_ADDR;

#ifdef FDL2_DEBUG
	printf("function %s, addr 0x%x, size 0x%x\n", __FUNCTION__, addr, size);
#endif
	
	nand_write_addr = addr;
	cur_write_pos = addr;
	nand_write_size = size;

	cur_partition.offset = addr;
	cur_partition.size = 0;
	parse_cmdline_partitions(&cur_partition, (unsigned long long)nand->size);

	for (erase_blk = 0; erase_blk < (cur_partition.size / nand->erasesize); erase_blk ++) {
		printf("erasing block : %d    %d % \r", (cur_partition.offset / nand->erasesize + erase_blk), (erase_blk * 100 ) / (cur_partition.size / nand->erasesize));
		if (nand_block_isbad(nand, cur_partition.offset + erase_blk * nand->erasesize)) {
			printf("\nerase block : %d is bad\n", (cur_partition.offset / nand->erasesize + erase_blk));
		} else {
			ret = nand_erase_fdl(cur_partition.offset + erase_blk * nand->erasesize, nand->erasesize);
			if (ret != 0) {
				/* erase failed */
				ret = nand->block_markbad(nand, cur_partition.offset + erase_blk * nand->erasesize);
				if (ret != 0) {
					/* mark failed */
					printf("mark 0x%x bad error\n", cur_partition.offset + erase_blk * nand->erasesize);
				   	return NAND_SYSTEM_ERROR + 1;
				} else {
					/* mark success */
					printf("skip bad block 0x%x\n", cur_partition.offset + erase_blk * nand->erasesize);
				}
			}

		}
	}
	
	return NAND_SUCCESS;
}

int nand_do_write_ops(struct mtd_info *mtd, loff_t to,struct mtd_oob_ops *ops);
int nand_do_write_oob(struct mtd_info *mtd, loff_t to,
			     struct mtd_oob_ops *ops);
int nand_do_read_ops(struct mtd_info *mtd, loff_t from,
			    struct mtd_oob_ops *ops);
int nand_write_fdl(unsigned int size, unsigned char *buf)
{
#ifdef FDL2_DEBUG
	printf("function: %s\n", __FUNCTION__);
#endif
	struct mtd_info *nand;
	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  return NAND_SYSTEM_ERROR;
	nand = &nand_info[nand_curr_device];
	int ret=0;
	int pos;
	unsigned char buffer[4096];
	int aaa;

	//find a good block to write 
	while(!(cur_write_pos & (nand->erasesize-1))) {
#ifdef FDL2_DEBUG
		printf("function: %s to check bad block, check address 0x%x\n", __FUNCTION__,cur_write_pos&(~(nand->erasesize - 1)));
#endif
		if (nand_block_isbad(nand, cur_write_pos&(~(nand->erasesize - 1)))) {
#ifdef FDL2_DEBUG
			printf("function: %s block is bad\n",__FUNCTION__);
#endif
			printf("%s skip bad block 0x%x\n", __FUNCTION__, cur_write_pos&(~(nand->erasesize - 1)));
			cur_write_pos = (cur_write_pos + nand->erasesize)&(~(nand->erasesize - 1));
		} else {
#ifdef FDL2_DEBUG
			printf("function: %s block is good\n", __FUNCTION__);
#endif
			break;
		}
	}

	if(!is_system_write){
#ifdef FDL2_DEBUG
		printf("function: %s cur_write_pos: 0x%x size: 0x%x\n", __FUNCTION__, cur_write_pos, size);
#endif
		if(size < nand->writesize) {
			/* set 0xff from buf[size -- writesize] for fixnv and runtimenv */
			for (pos = size; pos < nand->writesize; pos ++)
				buf[pos] = 0xff;
		  	size = nand->writesize;	
		} else if(size > nand->writesize)	
		  	return NAND_INVALID_SIZE;
#ifdef FDL2_DEBUG
		printf("function: %s erase done, now to write it\n", __FUNCTION__);
#endif
		ret = nand_write_skip_bad(nand, cur_write_pos, &size, buf);
		if(0 == ret){
			cur_write_pos += nand->writesize;
		}else{
			nand->block_markbad(nand, cur_write_pos&~(nand->erasesize - 1));
			return NAND_SYSTEM_ERROR;
		}
	}else{ // system write 
#ifdef FDL2_DEBUG
		printf("function: %s system write cur_write_pos: 0x%x size: 0x%x\n", __FUNCTION__, cur_write_pos, size);
#endif
		if(size != (nand->writesize + nand->oobsize))
		  return NAND_INVALID_SIZE;

		struct nand_chip *chip = nand->priv;

		chip->ops.mode = MTD_OOB_AUTO;
		chip->ops.len = nand->writesize;
		chip->ops.datbuf = (uint8_t *)buf;
		chip->ops.oobbuf = (uint8_t *)buf + nand->writesize;
		chip->ops.ooblen = sizeof(yaffs_PackedTags2);
		chip->ops.ooboffs = 0;

#ifdef FDL2_DEBUG
		printf("function: %s system write start to write\n", __FUNCTION__);
#endif
		ret = nand_do_write_ops(nand, (unsigned long long)cur_write_pos, &(chip->ops));
		if(0==ret){
#ifdef FDL2_DEBUG
			printf("function: %s system write write complete\n", __FUNCTION__);
#endif
			cur_write_pos += nand->writesize;
		}else{
			nand->block_markbad(nand, cur_write_pos&~(nand->erasesize - 1));
			return NAND_SYSTEM_ERROR;
		}
	}
	if(ret == 0){
		struct mtd_oob_ops ops;
		uint8_t buf_tmp[2] = { 0, 0 };
		ops.mode = MTD_OOB_AUTO;
		ops.len = nand->writesize;
		ops.datbuf = (uint8_t *)buffer;
		ops.oobbuf = (uint8_t *)buffer + nand->writesize; 
		ops.ooblen = 64; 
		ops.ooboffs = 0;
		memset(buffer, 0x0, 4096);
		ret = nand_do_read_ops(nand,(unsigned long long)(cur_write_pos-nand->writesize),&ops);
		if(ret < 0){
			ret = nand->block_markbad(nand, cur_write_pos & ~(nand->erasesize - 1));
			if (ret) {
				printf("block 0x%08lx NOT marked as bad! \n",cur_write_pos);
			}
			return NAND_SYSTEM_ERROR;
		}	
		return NAND_SUCCESS;
	} else
	  return NAND_SYSTEM_ERROR;
}
int nand_end_write(void)
{
#ifdef FDL2_DEBUG
	printf("function: %s\n", __FUNCTION__);
#endif
	nand_write_addr = NULL;
	nand_write_size = 0;
	cur_write_pos = NULL;
	return NAND_SUCCESS;
}
int nand_read_fdl(unsigned int addr, unsigned int off, unsigned int size, unsigned char *buf)
{
	printf("function: %s\n", __FUNCTION__);
}
int nand_read_NBL(void *buf)
{
	printf("function: %s\n", __FUNCTION__);
}
