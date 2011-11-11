#include "parsemtdparts.h"
#ifndef FDL_NAND_H
#define FDL_NAND_H

#define NAND_SUCCESS                0
#define NAND_SYSTEM_ERROR           1
#define NAND_UNKNOWN_DEVICE         2
#define NAND_INVALID_DEVICE_SIZE    3
#define NAND_INCOMPATIBLE_PART      4
#define NAND_INVALID_ADDR           5
#define NAND_INVALID_SIZE           6


#define ECC_NBL_SIZE 0x4000
//bootloader header flag offset from the beginning
#define BOOTLOADER_HEADER_OFFSET   32
#define NAND_PAGE_LEN              512
#define NAND_MAGIC_DATA            0xaa55a5a5
//define nand data bus len
#define NAND_BUS_SIZE_8              8
#define NAND_BUS_SIZE_16              16
#define NAND_BUS_SIZE_32              32

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************
 * change the header of first bootloader page
 ******************************************************************************/
int nand_change_bootloader_header (unsigned int *bl_start_addr);

int nand_flash_init (void);
int nand_format (void);
int nand_erase_fdl (unsigned int addr, unsigned int size);
int nand_start_write (struct real_mtd_partition *phypart, unsigned int size);
int nand_write_fdl (unsigned int size, unsigned char *buf);
int nand_end_write (void);
int nand_read_fdl (struct real_mtd_partition *phypart, unsigned int off, unsigned int size, unsigned char *buf);
int nand_read_NBL (void *buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FDL_NAND_H */
