#ifndef FLASH_COMMAND_H
#define FLASH_COMMAND_H

#include "cmd_def.h"
#include "fdl_nand.h"
#include "packet.h"

static __inline DLSTATUS convert_err (int err)
{
    switch (err)
    {
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
int FDL2_DataStart (PACKET_T *packet, void *arg);

/******************************************************************************
 * data_midst
 ******************************************************************************/
int FDL2_DataMidst (PACKET_T *packet, void *arg);

/******************************************************************************
 * data_end
 ******************************************************************************/
int FDL2_DataEnd (PACKET_T *packet, void *arg);

/******************************************************************************
 * read_flash
 ******************************************************************************/
int FDL2_ReadFlash (PACKET_T *packet, void *arg);

/******************************************************************************
 * erase_flash
 ******************************************************************************/
int FDL2_EraseFlash (PACKET_T *packet, void *arg);

/******************************************************************************
 * format_flash
 ******************************************************************************/
int FDL2_FormatFlash (PACKET_T *pakcet, void *arg);
unsigned long FDL2_GetRecvDataSize (void);

#endif /* FLASH_COMMAND_H */