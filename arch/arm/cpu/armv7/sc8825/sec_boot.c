#include <common.h>
#include <asm/arch/common.h>
#include <malloc.h>

#ifdef CONFIG_NAND_SPL
#define panic(x...) do{}while(0)
#define printf(x...) do{}while(0)
#endif
int test_rsa(void);
#if 1
//#ifdef CONFIG_NAND_SPL 
#define dump_all_buffer(x...) do{}while(0)
#else
void dump_all_buffer(unsigned char *buf, unsigned long len)
{
    unsigned long row, col;
    unsigned int offset;
    unsigned long total_row, remain_col;
    unsigned long flag = 1;

    total_row = len / 16;
    remain_col = len - total_row * 16;
    offset = 0;
    for (row = 0; row < total_row; row ++) {
        if (flag == 1) {
            printf("%08xh: ", offset);
            for (col = 0; col < 16; col ++)
                printf("%02x ", buf[offset + col]);
            printf("\n");
        }
        offset += 16;
    }

    if (remain_col > 0) {
        if (flag == 1) {
            printf("%08xh: ", offset);
            for (col = 0; col < remain_col; col ++)
                printf("%02x ", buf[offset + col]);
            printf("\n");
        }
    }

    printf("\n");
}

#endif

int secureboot_enabled(void)
{
	return 0;
}
int harshVerify(uint8_t *data, uint32_t data_len, uint8_t *data_hash, uint8_t *data_key)
{
	return 1;
}

void secure_check(uint8_t *data, uint32_t data_len, uint8_t *data_hash, uint8_t *data_key)
{
	return;
}

#ifndef CONFIG_NAND_SPL
int cal_md5(void *data, uint32_t orig_len, void *harsh_data)
{
	return 0;
}
#endif

