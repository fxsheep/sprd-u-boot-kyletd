#ifndef _SECURE_BOOT_H_
#define _SECURE_BOOT_H_

#include "sc8810_reg_base.h"
#define KEY_INFO_SIZ (512)
#define CUSTOM_DATA_SIZ (1024)
#define VLR_INFO_SIZ (512)
#define VLR_INFO_OFF (512)

void secure_check(uint8_t *data, uint32_t data_len, uint8_t *data_hash, uint8_t *data_key);
int cal_md5(void *data, uint32_t len, void *harsh_data);
int secureboot_enabled(void);

#endif
