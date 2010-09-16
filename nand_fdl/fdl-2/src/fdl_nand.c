#include "fdl_nand.h"
#include "sci_types.h"
#include "nand_controller.h"
#include <linux/mtd/mtd.h>
#include <nand.h>

#define ADDR_MASK				0x80000000

static unsigned int nand_write_size ;
static unsigned int nand_write_addr;
static unsigned int cur_write_pos;

int nand_flash_init(void)
{
	nand_init();
	return 0;
}

int nand_format(void)
{
	printf("function: %s\n", __FUNCTION__);
	struct mtd_info *nand;
	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  return NAND_SYSTEM_ERROR;
	nand = &nand_info[nand_curr_device];
	
	printf("function: %s nand_curr_device is %d\n", __FUNCTION__, nand_curr_device);
	unsigned int off = nand->erasesize; //TO SKIP THE FIRST BLOCK
	unsigned int size = nand->size - off; //to erase the left blocks

	nand_erase_options_t opts;
	memset(&opts, 0, sizeof(opts));
	opts.offset = off;
	opts.length = size;
	opts.jffs2 = 0;
	opts.scrub = 0;
	
	printf("function: %s erase off 0x%x length: 0x%x jffs2: 0x%x scurb: 0x%x\n",__FUNCTION__, opts.offset, opts.length, opts.jffs2, opts.scrub);
	return nand_erase_opts(nand, &opts);
}

int nand_erase_fdl(unsigned int addr, unsigned int size)
{
	printf("function: %s\n", __FUNCTION__);
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

	printf("function: %s erase off 0x%x length: 0x%x jffs2: 0x%x scurb: 0x%x\n",__FUNCTION__, opts.offset, opts.length, opts.jffs2, opts.scrub);
	return nand_erase_opts(nand, &opts);
}
int nand_start_write(unsigned int addr, unsigned int size)
{
	printf("function: %s\n", __FUNCTION__);
	struct mtd_info *nand;
	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  return NAND_SYSTEM_ERROR;
	nand = &nand_info[nand_curr_device];

	if(addr & (nand->erasesize - 1))
	  return NAND_INVALID_ADDR;
	if(addr & ADDR_MASK)
	  addr &= ~ADDR_MASK;
	else return NAND_INVALID_ADDR;

	printf("function %s, addr 0x%x, size 0x%x\n", __FUNCTION__, addr, size);
	printf("current device no: %d erase size: 0x%x write size: 0x%x\n", nand_curr_device, nand->erasesize, nand->writesize);
	nand_write_addr = addr;
	cur_write_pos = addr;
	nand_write_size = size;
	return NAND_SUCCESS;
}
int nand_write_fdl(unsigned int size, unsigned char *buf)
{
	printf("function: %s\n", __FUNCTION__);
	struct mtd_info *nand;
	if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  return NAND_SYSTEM_ERROR;
	nand = &nand_info[nand_curr_device];
	int ret=0;

	if(size < nand->writesize)
	  size = nand->writesize;
	else if(size > nand->writesize)
	  return NAND_INVALID_SIZE;

	printf("function: %s cur_write_pos: 0x%x size: 0x%x\n", __FUNCTION__, cur_write_pos, size);
	while(!(cur_write_pos & (nand->erasesize-1))){
		if(nand_block_isbad(nand, cur_write_pos&(~(nand->erasesize - 1)))){
			printf("%s skip bad block 0x%x\n", __FUNCTION__, cur_write_pos&(~(nand->erasesize - 1)));
			cur_write_pos = (cur_write_pos + nand->erasesize)&(~(nand->erasesize - 1));
			continue;
		}else {
			ret = nand_erase_fdl(cur_write_pos, nand->erasesize);
			if(ret != 0)
			  return NAND_SYSTEM_ERROR + 1;
			break;
		}
	}

	ret = nand_write_skip_bad(nand, cur_write_pos, &size, buf);
	if(0==ret){
		cur_write_pos += size;
		return NAND_SUCCESS;
	}
	return NAND_SYSTEM_ERROR + 2;
}
int nand_end_write(void)
{
	printf("function: %s\n", __FUNCTION__);
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
