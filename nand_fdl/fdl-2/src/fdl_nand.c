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

#define BLOCK_DATA_OOB		((2048 + 64) * 64)
static unsigned char backupblk[BLOCK_DATA_OOB];
static unsigned long backupblk_len = 0;
static unsigned long backupblk_flag = 0;

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
		ops.mode = MTD_OOB_AUTO;
		ops.len = nand->writesize;
		ops.datbuf = (uint8_t *)buffer;
		ops.oobbuf = (uint8_t *)buffer + nand->writesize; 
		ops.ooblen = 64; 
		ops.ooboffs = 0;
		memset(buffer, 0x5a, 4096);
		ret = nand_do_read_ops(nand, (unsigned long long)pos, &ops);
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

int move2goodblk(struct mtd_info *nand, int yaffs_flag)
{
	int pageno, size, pageall, ret;
	unsigned char buffer[2048 + 64];

write2nextblk:
	printf("old write address : 0x%08x\n", cur_write_pos);
	cur_write_pos = (cur_write_pos + nand->erasesize) & (~(nand->erasesize - 1));
	printf("new write address : 0x%08x\n", cur_write_pos);
	//find a good block to write 
	while(!(cur_write_pos & (nand->erasesize-1))) {
		if (nand_block_isbad(nand, cur_write_pos&(~(nand->erasesize - 1)))) {
			printf("%s skip bad block 0x%x\n", __FUNCTION__, cur_write_pos&(~(nand->erasesize - 1)));
			cur_write_pos = (cur_write_pos + nand->erasesize)&(~(nand->erasesize - 1));
		} else
			break;
	}
	
	if (!yaffs_flag) {
		size = nand->writesize;
		pageall = backupblk_len / nand->writesize;
		for (pageno = 0; pageno < pageall; pageno ++) {
			memset(buffer, 0xff, (2048 + 64));
			memcpy(buffer, backupblk + pageno * size, size);
			ret = nand_write_skip_bad(nand, cur_write_pos, &size, buffer);
			if (0 == ret) {
				//////////////////////
				struct mtd_oob_ops ops;
				ops.mode = MTD_OOB_AUTO;
				ops.len = nand->writesize;
				ops.datbuf = (uint8_t *)buffer;
				ops.oobbuf = (uint8_t *)buffer + nand->writesize; 
				ops.ooblen = 64; 
				ops.ooboffs = 0;
				memset(buffer, 0x0, 2048 + 64);
				ret = nand_do_read_ops(nand,(unsigned long long)(cur_write_pos),&ops);
				if (ret < 0) {
					printf("read error, mark bad block : 0x%08x\n", cur_write_pos);
					nand->block_markbad(nand, cur_write_pos&~(nand->erasesize - 1));
					printf("find next good block to write again\n");
					goto write2nextblk;
				}
				//////////////////////
				cur_write_pos += nand->writesize;
			} else {
				printf("write error, mark bad block : 0x%08x\n", cur_write_pos);
				nand->block_markbad(nand, cur_write_pos&~(nand->erasesize - 1));
				printf("find next good block to write again\n");
				goto write2nextblk;
			}
		} //for (pageno = 0; pageno < pageall; pageno ++)
	} else {//if (!yaffs_flag)
		struct mtd_oob_ops ops;
		size = nand->writesize + nand->oobsize;
		pageall = backupblk_len / size;
		for (pageno = 0; pageno < pageall; pageno ++) {
			memset(buffer, 0xff, size);
			memcpy(buffer, backupblk + pageno * size, size);
			ops.mode = MTD_OOB_AUTO;
			ops.len = nand->writesize;
			ops.datbuf = (uint8_t *)buffer;
			ops.oobbuf = (uint8_t *)buffer + nand->writesize;
			ops.ooblen = sizeof(yaffs_PackedTags2);
			ops.ooboffs = 0;
			ret = nand_do_write_ops(nand, (unsigned long long)cur_write_pos, &ops);
			if (0 == ret) {
				//////////////////////
				ops.mode = MTD_OOB_AUTO;
				ops.len = nand->writesize;
				ops.datbuf = (uint8_t *)buffer;
				ops.oobbuf = (uint8_t *)buffer + nand->writesize; 
				ops.ooblen = 64; 
				ops.ooboffs = 0;
				memset(buffer, 0x0, 2048 + 64);
				ret = nand_do_read_ops(nand,(unsigned long long)(cur_write_pos),&ops);
				if (ret < 0) {
					printf("read error, mark bad block : 0x%08x\n", cur_write_pos);
					nand->block_markbad(nand, cur_write_pos&~(nand->erasesize - 1));
					printf("find next good block to write again\n");
					goto write2nextblk;
				}
				//////////////////////
				cur_write_pos += nand->writesize;
			} else {
				printf("write error, mark bad block : 0x%08x\n", cur_write_pos);
				nand->block_markbad(nand, cur_write_pos&~(nand->erasesize - 1));
				printf("find next good block to write again\n");
				goto write2nextblk;
			}
		} //for (pageno = 0; pageno < pageall; pageno ++)
	}

	return 0;
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
	
	memset(backupblk, 0xff, BLOCK_DATA_OOB);
	backupblk_len = 0;
	backupblk_flag = 0;

	nand_write_addr = addr;
	cur_write_pos = addr;
	nand_write_size = size;

	cur_partition.offset = addr;
	cur_partition.size = 0;
	parse_cmdline_partitions(&cur_partition, (unsigned long long)nand->size);

	if (size >= cur_partition.size) {
		printf("\n\nimage file size : 0x%08x is bigger than partition size : 0x%08x\n", size, cur_partition.size);
		return NAND_INVALID_SIZE;
	}

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
	printf("function: %s   %d\n", __FUNCTION__, __LINE__);
#endif
	struct mtd_info *nand;
	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  return NAND_SYSTEM_ERROR;
	nand = &nand_info[nand_curr_device];
	int ret=0;
	int pos;
	unsigned char buffer[4096];
	
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
		//printf("cur_write_pos: 0x%x size: 0x%x\n", cur_write_pos, size);
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
		/* backup */
		memcpy(backupblk + backupblk_len, buf, size);
		backupblk_len += size;
		if (backupblk_len >= (128 * 1024))
			backupblk_flag = 1; /* full */
		else
			backupblk_flag = 0;
		
		ret = nand_write_skip_bad(nand, cur_write_pos, &size, buf);

#if 0
		/* fcj test */
		if (cur_write_pos == (0x00bc0000 - 0x800))
			ret = -1;
#endif

		if (0 == ret) {
			cur_write_pos += nand->writesize;
		} else {
			printf("\nwrite error, mark bad block : 0x%08x\n", cur_write_pos);
			nand->block_markbad(nand, cur_write_pos&~(nand->erasesize - 1));
			printf("find new good partition to move and write data again\n");
			move2goodblk(nand,is_system_write);
			printf("move and write end. new write pos : 0x%08x\n", cur_write_pos);
			ret = 0;
			/* all success not failed */
			//return NAND_SYSTEM_ERROR;
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

		/* backup */
		memcpy(backupblk + backupblk_len, buf, size);
		backupblk_len += size;
		if (backupblk_len >= BLOCK_DATA_OOB)
			backupblk_flag = 1; /* full */
		else
			backupblk_flag = 0;
		
		ret = nand_do_write_ops(nand, (unsigned long long)cur_write_pos, &(chip->ops));
#if 0
		/* fcj test */
		if ((cur_write_pos == (0x02bc0000 - 0x800)) || (cur_write_pos == (0x0bfc0000 - 0x800)))
			ret = -1;
#endif
		if (0 == ret) {
			cur_write_pos += nand->writesize;
		} else {
			printf("\nwrite error, mark bad block : 0x%08x\n", cur_write_pos);
			nand->block_markbad(nand, cur_write_pos&~(nand->erasesize - 1));
			printf("find new good partition to move and write data again\n");
			move2goodblk(nand, is_system_write);
			printf("move and write end. new write pos : 0x%08x\n", cur_write_pos);
			ret = 0;
			/* all success not failed */
			//return NAND_SYSTEM_ERROR;
		}
	}

	if(ret == 0) {
		struct mtd_oob_ops ops;
		ops.mode = MTD_OOB_AUTO;
		ops.len = nand->writesize;
		ops.datbuf = (uint8_t *)buffer;
		ops.oobbuf = (uint8_t *)buffer + nand->writesize; 
		ops.ooblen = 64; 
		ops.ooboffs = 0;
		memset(buffer, 0x0, 4096);
		ret = nand_do_read_ops(nand,(unsigned long long)(cur_write_pos-nand->writesize),&ops);
		if (ret < 0) {
			cur_write_pos -= nand->writesize;
			printf("\nread error, mark bad block : 0x%08x\n", cur_write_pos);
			nand->block_markbad(nand, cur_write_pos & ~(nand->erasesize - 1));
			printf("find new good partition to move and write data again, then read data\n");
			move2goodblk(nand, is_system_write);
			printf("move and write and read end. new write pos : 0x%08x\n", cur_write_pos);
			/* can not cur_write_pos + nand->writesize */
			//return NAND_SYSTEM_ERROR;
		}

		if (backupblk_flag) {
			memset(backupblk, 0xff, BLOCK_DATA_OOB);
			backupblk_len = 0;
			backupblk_flag = 0;
		}

		return NAND_SUCCESS;
	} else {
		/* can not run here */
		return NAND_SYSTEM_ERROR;
		}
}
int nand_end_write(void)
{
#ifdef FDL2_DEBUG
	printf("function: %s\n", __FUNCTION__);
#endif
	nand_write_addr = NULL;
	nand_write_size = 0;
	cur_write_pos = NULL;

	memset(backupblk, 0xff, BLOCK_DATA_OOB);
	backupblk_len = 0;
	backupblk_flag = 0;
	return NAND_SUCCESS;
}
int nand_read_fdl(unsigned int addr, unsigned int off, unsigned int size, unsigned char *buf)
{
	struct mtd_info *nand;
	int ret=0;
	int pos;
	unsigned char buffer[64];

	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  	return NAND_SYSTEM_ERROR;
	nand = &nand_info[nand_curr_device];

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
	//printf("addr = 0x%08x  size = %d  off = %d\n", addr, size, off);
	while (!(addr & (nand->erasesize - 1))) {
		if (nand_block_isbad(nand, addr & (~(nand->erasesize - 1)))) {
			printf("skip bad block 0x%x\n", addr & (~(nand->erasesize - 1)));
			addr = (addr + nand->erasesize)&(~(nand->erasesize - 1));
		} else {
			//printf("good block 0x%x\n", addr & (~(nand->erasesize - 1)));
			break;
		}
	}

	if(!is_system_write){
		/* for fixnv, read total 64KB */
		if (size != nand->writesize)	
		  	return NAND_INVALID_SIZE;
	
		struct mtd_oob_ops ops;
		ops.mode = MTD_OOB_AUTO;
		ops.len = nand->writesize;
		ops.datbuf = (uint8_t *)buf;
		ops.oobbuf = (uint8_t *)buffer; 
		ops.ooblen = 64; 
		ops.ooboffs = 0;
		memset(buffer, 0xff, 64);
		memset(buf, 0xff, size);
		ret = nand_do_read_ops(nand,(unsigned long long)(addr + off), &ops);
		if (ret < 0) {
			printf("\nread error, mark bad block : 0x%08x\n", addr);
			nand->block_markbad(nand, addr & ~(nand->erasesize - 1));
			return NAND_SYSTEM_ERROR;
		}
	}
	
	return NAND_SUCCESS;
}

int nand_read_NBL(void *buf)
{
	printf("function: %s\n", __FUNCTION__);
	return NAND_SUCCESS;
}
