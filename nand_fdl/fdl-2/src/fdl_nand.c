#include "fdl_nand.h"
#include "asm/arch/sci_types.h"
#include "asm/arch/nand_controller.h"
#include <linux/mtd/mtd.h>
#include <nand.h>

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

int nand_erase_fdl(unsigned int addr, unsigned int size)
{
#ifdef FDL2_DEBUG
	printf("function: %s\n", __FUNCTION__);
#endif
	struct mtd_info *nand;
	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  return NAND_SYSTEM_ERROR;
	nand = &nand_info[nand_curr_device];

	if(addr < nand->erasesize) //to skip the first block erase
	  return NAND_INVALID_ADDR;
	if(size != nand->erasesize)
	  return NAND_INVALID_SIZE;
	
	nand_erase_options_t opts;
	memset(&opts, 0, sizeof(opts));
	opts.offset = addr;
	opts.length = size;
	opts.jffs2 = 0;

#ifdef FDL2_DEBUG
	printf("function: %s erase off 0x%x length: 0x%x jffs2: 0x%x scurb: 0x%x\n",__FUNCTION__, opts.offset, opts.length, opts.jffs2, opts.scrub);
#endif
	return nand_erase_opts(nand, &opts);
}
int nand_start_write(unsigned int addr, unsigned int size)
{
#ifdef FDL2_DEBUG
	printf("function: %s\n", __FUNCTION__);
#endif
	struct mtd_info *nand;
	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  return NAND_SYSTEM_ERROR;
	nand = &nand_info[nand_curr_device];

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
	//printf("current device no: %d erase size: 0x%x write size: 0x%x\n", nand_curr_device, nand->erasesize, nand->writesize);
	nand_write_addr = addr;
	cur_write_pos = addr;
	nand_write_size = size;
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

	//find a good block to write 
	while(!(cur_write_pos & (nand->erasesize-1))){
#ifdef FDL2_DEBUG
		printf("function: %s to check bad block, check address 0x%x\n", __FUNCTION__,cur_write_pos&(~(nand->erasesize - 1)));
#endif
		if(nand_block_isbad(nand, cur_write_pos&(~(nand->erasesize - 1)))){
#ifdef FDL2_DEBUG
			printf("function: %s block is bad\n",__FUNCTION__);
#endif
			printf("%s skip bad block 0x%x\n", __FUNCTION__, cur_write_pos&(~(nand->erasesize - 1)));
			cur_write_pos = (cur_write_pos + nand->erasesize)&(~(nand->erasesize - 1));
			continue;
		}else {
#ifdef FDL2_DEBUG
			printf("function: %s block is not bad, erase it\n", __FUNCTION__);
#endif
			ret = nand_erase_fdl(cur_write_pos, nand->erasesize);
			if(ret != 0){
				ret = nand->block_markbad(nand, cur_write_pos);
				if(ret != 0){
				  printf("function: %s mark 0x%x bad error\n", __FUNCTION__, cur_write_pos);
				  return NAND_SYSTEM_ERROR + 1;
				}else {
					printf("function: %s skip bad block 0x%x\n", __FUNCTION__, cur_write_pos);
					cur_write_pos = (cur_write_pos + nand->erasesize)&(~(nand->erasesize - 1));
					continue;
				}
			}else
				break;
		}
	}

	if(!is_system_write){
#ifdef FDL2_DEBUG
		printf("function: %s cur_write_pos: 0x%x size: 0x%x\n", __FUNCTION__, cur_write_pos, size);
#endif
		if(size < nand->writesize)
		  size = nand->writesize;
		else if(size > nand->writesize)
		  return NAND_INVALID_SIZE;
#ifdef FDL2_DEBUG
		printf("function: %s erase done, now to write it\n", __FUNCTION__);
#endif
		ret = nand_write_skip_bad(nand, cur_write_pos, &size, buf);
		if(0==ret){
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
		ops.datbuf = (uint8_t *)buf;
		ops.oobbuf = NULL; 
		ops.ooblen = 0; 
		ops.ooboffs = 0;
		ret = nand_do_read_ops(nand,(unsigned long long)(cur_write_pos-nand->writesize),&ops);
		if(ret<0){
			ret = nand->block_markbad(nand, cur_write_pos&~(nand->erasesize - 1));
			if (ret) {
				printf("block 0x%08lx NOT marked as bad! \n",cur_write_pos);
			}
			return NAND_SYSTEM_ERROR;
		}
		return NAND_SUCCESS;
	}else
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
