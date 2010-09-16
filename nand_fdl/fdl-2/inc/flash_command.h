#ifndef FLASH_COMMAND_H
#define FLASH_COMMAND_H

#include "cmd_def.h"
#include "fdl_nand.h"

static __inline DLSTATUS convert_err(int err) 
{
	switch (err) {
	case NAND_SUCCESS:	  	
		return BSL_REP_ACK;
	case NAND_INVALID_ADDR: 
		return BSL_REP_DOWN_DEST_ERROR;
	case NAND_INVALID_SIZE: 
		return BSL_REP_DOWN_SIZE_ERROR;
	case NAND_UNKNOWN_DEVICE: 
		return BSL_UNKNOWN_DEVICE;
	case NAND_INVALID_DEVICE_SIZE: 
		return BSL_INVALID_DEVICE_SIZE;
	case NAND_INCOMPATIBLE_PART:
		return BSL_INCOMPATIBLE_PARTITION;
	default:
		return BSL_REP_OPERATION_FAILED;
	}
}

/******************************************************************************
 * data_start
 ******************************************************************************/
int data_start(PKT_HEADER * packet, void * arg);

/******************************************************************************
 * data_midst
 ******************************************************************************/
int data_midst(PKT_HEADER * packet, void * arg);

/******************************************************************************
 * data_end
 ******************************************************************************/ 
int data_end(PKT_HEADER * packet, void * arg);

/******************************************************************************
 * read_flash
 ******************************************************************************/
int read_flash(PKT_HEADER * packet, void * arg);

/******************************************************************************
 * erase_flash
 ******************************************************************************/
int erase_flash(PKT_HEADER * packet, void * arg);

/******************************************************************************
 * format_flash
 ******************************************************************************/
int format_flash(PKT_HEADER *pakcet, void *arg);

#endif /* FLASH_COMMAND_H */
