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

#define MD5NUM_SZ (16)

typedef struct{
	struct{
		uint32_t e;
		uint8_t m[128];
		uint8_t r2[128];
	}key;
	uint8_t reserved[4];
}bsc_info_t;

#define VLR_MAGIC (0x524c56ff)
typedef struct {
	uint32_t magic;
	uint8_t hash[128];
	uint32_t setting;
	uint32_t length;
	uint8_t reserved[4];
}vlr_info_t;

typedef struct {
	uint32_t *g_TraceValue;
	uint8_t *efuse_harsh_data;
	uint8_t *soft_harsh_data;
	int (*CheckSecureBootEnable)(void);
	int (*HarshVerify)(uint32_t *, uint32_t);
	void (*RSA_ModPower)(uint32_t *p, uint32_t *m, uint32_t *r2, uint32_t e);
	void (*MD5Init)(unsigned int *g_data_prt);
	void (*MD5Final)(unsigned int *input, unsigned int inputLen, unsigned int *g_data_ptr);
}harsh_func_t;

const uint32_t __harsh_func_hack[] ={
	0x40006000,
	0x40006658,
	0x40006668,
	0xffff1405,
	0xffff1481,
	0xffff2604,
	0xffff26ec,
	0xffff314c,
};

/*
 * p 128B
 * m 128B
 * r2 128B
 * e 4B
 */
#define MAKE_DWORD(a,b,c,d) (uint32_t)(((uint32_t)(a)<<24) | (uint32_t)(b)<<16 | ((uint32_t)(c)<<8) | ((uint32_t)(d)))

void RSA_Decrypt(unsigned char *p, unsigned char *m, unsigned char *r2, unsigned char *e)
{
	return;
}

int secureboot_enabled(void)
{
	return 0;
}
unsigned int md5_buf[20] = {0};
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

