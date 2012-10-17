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

const uint32_t mf__harsh_func_hack[] ={
	0x40006000,
	0x40006658,
	0x40006668,
	0xffff1405,
	0xffff1481,
	0xffff2604,
	0xffff26ec,
	0xffff314c,
};
const uint32_t v0__harsh_func_hack[] ={
	0x40006000,
	0x40006658,
	0x40006668,
	0xffff1439,
	0xffff14b5,
	0xffff2638,
	0xffff2720,
	0xffff3180,
};

harsh_func_t * get_harsh_func(void)
{
	uint32_t chip_type = 0;
	static harsh_func_t * harsh_func = NULL;
	if(harsh_func != NULL)
		return harsh_func;

	chip_type = CHIP_REG_GET(CHIP_TYPE);

	if(chip_type == CHIP_ID_VER_0 || chip_type == CHIP_ID_VER_1)
		harsh_func = (harsh_func_t *) v0__harsh_func_hack;
	else if(chip_type == CHIP_ID_VER_MF)
		harsh_func = (harsh_func_t *) mf__harsh_func_hack;
	else {
		/* not supported chip id */
		while(1){}
	}

	return harsh_func;
}

/*
 * p 128B
 * m 128B
 * r2 128B
 * e 4B
 */
#define MAKE_DWORD(a,b,c,d) (uint32_t)(((uint32_t)(a)<<24) | (uint32_t)(b)<<16 | ((uint32_t)(c)<<8) | ((uint32_t)(d)))

void RSA_Decrypt(unsigned char *p, unsigned char *m, unsigned char *r2, unsigned char *e)
{
	unsigned int _e = 0;
	unsigned int _m[32] = {0};
	unsigned int _p[32] = {0};
	unsigned int _r2[32] = {0};
	int i = 0;
	harsh_func_t *harsh = get_harsh_func();

	_e = MAKE_DWORD(e[0], e[1], e[2], e[3]);

	for(i=31; i>=0; i--){
		_m[31-i] = MAKE_DWORD(m[4*i], m[4*i+1], m[4*i+2], m[4*i+3]); 
		_p[31-i] = MAKE_DWORD(p[4*i], p[4*i+1], p[4*i+2], p[4*i+3]); 
		_r2[31-i] = MAKE_DWORD(r2[4*i], r2[4*i+1], r2[4*i+2], r2[4*i+3]); 
	}

	harsh->RSA_ModPower(_p, _m, _r2, _e);

	for(i=31;i>=0;i--){
		p[4*(31-i)] = (unsigned char)(_p[i]>>24);
		p[4*(31-i)+1] = (unsigned char)(_p[i]>>16);
		p[4*(31-i)+2] = (unsigned char)(_p[i]>>8);
		p[4*(31-i)+3] = (unsigned char)(_p[i]);
	}
}

int secureboot_enabled(void)
{
#ifdef SECURE_BOOT_ENABLE
	harsh_func_t *harsh = get_harsh_func();
	return harsh->CheckSecureBootEnable();
#else
	return 0;
#endif
}
unsigned int md5_buf[20] = {0};
int harshVerify(uint8_t *data, uint32_t data_len, uint8_t *data_hash, uint8_t *data_key)
{
	int ret = 1;
	harsh_func_t *harsh = get_harsh_func();

	//test_rsa();

	printf("check secure boot enable %d\n", secureboot_enabled());
	if(!secureboot_enabled()) return ret;

	if(!data_hash && !data_key) 
		return harsh->HarshVerify(data, data_len >> 2);
	else{
		bsc_info_t *bsc = (bsc_info_t *)data_key;
		printf("data key\n");
		dump_all_buffer(data_key, 512);
		if(data_len) 
			memcpy(harsh->efuse_harsh_data, data_hash, MD5NUM_SZ);
		else{
			printf("data_hash\n");
			dump_all_buffer(data_hash, 512);
			vlr_info_t *vlr = (vlr_info_t *)data_hash;
			if(vlr->magic != VLR_MAGIC) {
				printf("magic verify fail\n");
				return 0;
			}

			data_len = vlr->length;

#ifndef CONFIG_NAND_SPL
			cal_md5(data, data_len, md5_buf); 
#endif
			printf("hash before decrypt\n");
			dump_all_buffer(vlr->hash, 512);
			RSA_Decrypt(vlr->hash, bsc->key.m, bsc->key.r2, &bsc->key.e);
			printf("hash after decrypt\n");
			dump_all_buffer(vlr->hash, 512);
			memcpy(harsh->efuse_harsh_data, &vlr->hash[sizeof(vlr->hash)-MD5NUM_SZ], MD5NUM_SZ);
			printf("efuse harsh data\n");
			dump_all_buffer(harsh->efuse_harsh_data, MD5NUM_SZ);
		}
		ret = harsh->HarshVerify(data, data_len >> 2);
	}
	return ret;
}

void secure_check(uint8_t *data, uint32_t data_len, uint8_t *data_hash, uint8_t *data_key)
{
	printf("secure_check: data %p len %u hash %p key %p\n", data, data_len, data_hash, data_key);
	if(0 == harshVerify(data, data_len, data_hash, data_key)){
		printf("secure boot check fail\n");
		while(1){}
	}
}

#ifndef CONFIG_NAND_SPL
int cal_md5(void *data, uint32_t orig_len, void *harsh_data)
{
#ifdef SECURE_BOOT_ENABLE
	int i;
	unsigned int *harsh_p = (unsigned int *)harsh_data;
	harsh_func_t *harsh = get_harsh_func();
	uint32_t len = orig_len/4;

	if(secureboot_enabled()) return 0;

	harsh->MD5Init((unsigned int *)harsh_data);
	harsh->MD5Final((unsigned int *)data, len, harsh_p);
	for(i = 0; i<4; i++)
	{
		printf("md5 harsh data[%d] 0x%08x\n", i, harsh_p[i]);
	}

	return 1;
#else
	return 0;
#endif
}
#endif

