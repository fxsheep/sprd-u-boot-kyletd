#include "part_cfg.h"
#include "../drivers/mmc/card_sdio.h"

PART_DEVICE_IO g_part_device_io =
{
	//emmc_get_id,
	_get_device_info,
	_read_block,
	_write_block
};

PART_DEVICE emmc_part_device={0};

/**
 *
 *	block device access interface
 *
**/
unsigned int _get_device_info(unsigned int *total_sector_number)
{
                  *total_sector_number =  Emmc_GetCapacity(0);
                  return 1;
}

unsigned int _read_block(unsigned int start_sector,unsigned int sector_number,unsigned char *buf)
{
	//emmc read sector 

	if(TRUE == Emmc_Read(0, start_sector, sector_number, buf))
                        return 1;
                  else
                        return -1;
}

unsigned int _write_block(unsigned int start_sector,unsigned int sector_number,unsigned char *buf)
{
	//emmc write sector
	if(TRUE == Emmc_Write(0, start_sector, sector_number, buf))
                       return 1;
                   else
                        return -1;
}

unsigned int _erase_block(unsigned int start_sector,unsigned int sector_number)
{
	//for nand only
	if(TRUE == Emmc_Erase(0, start_sector, sector_number))
                         return 1;
                   else
                        return -1;
}

unsigned int emmc_part_device_init()
{
	emmc_part_device._device_io = &g_part_device_io;
	g_part_device_io._info(&emmc_part_device.total_sector);
       return 0;
}
