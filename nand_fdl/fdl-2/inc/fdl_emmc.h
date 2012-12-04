#ifndef _FDL_EMMC_H
#define _FDL_EMMC_H

#include <asm/arch/fdl_stdio.h>
#include <asm/arch/cmd_def.h>
#include <asm/arch/packet.h>
#ifdef CONFIG_EMMC_BOOT

#include "../../../disk/part_uefi.h"
#include "../../../include/common.h"
#include "../../../include/part.h"


#define EMMC_SUCCESS                0
#define EMMC_SYSTEM_ERROR           1
#define EMMC_DEVICE_INIT_ERROR      2
#define EMMC_INVALID_DEVICE_SIZE    3
#define EMMC_INCOMPATIBLE_PART      4
#define EMMC_INVALID_ADDR           5
#define EMMC_INVALID_SIZE           6


int FDL_BootIsEMMC(void);
int FDL_Check_Partition_Table(void);

static __inline DLSTATUS convert_err (int err)
{
    switch (err)
    {
        case EMMC_SUCCESS:
            return BSL_REP_ACK;
        case EMMC_INVALID_ADDR:
            return BSL_REP_DOWN_DEST_ERROR;
        case EMMC_INVALID_SIZE:
            return BSL_REP_DOWN_SIZE_ERROR;
        case EMMC_DEVICE_INIT_ERROR:
            return BSL_UNKNOWN_DEVICE;
        case EMMC_INVALID_DEVICE_SIZE:
            return BSL_INVALID_DEVICE_SIZE;
        case EMMC_INCOMPATIBLE_PART:
            return BSL_INCOMPATIBLE_PARTITION;
        default:
            return BSL_REP_OPERATION_FAILED;
    }
}

#ifndef FPGA_TRACE_DOWNLOAD
/******************************************************************************
 * data_start
 ******************************************************************************/
int FDL2_eMMC_DataStart (PACKET_T *packet, void *arg);

/******************************************************************************
 * data_midst
 ******************************************************************************/
int FDL2_eMMC_DataMidst (PACKET_T *packet, void *arg);

/******************************************************************************
 * data_end
 ******************************************************************************/
int FDL2_eMMC_DataEnd (PACKET_T *packet, void *arg);
#endif
/******************************************************************************
 * read_flash
 ******************************************************************************/
int FDL2_eMMC_Read (PACKET_T *packet, void *arg);

/******************************************************************************
 * erase_flash
 ******************************************************************************/
int FDL2_eMMC_Erase (PACKET_T *packet, void *arg);

/******************************************************************************
 * format_flash
 ******************************************************************************/
int FDL2_eMMC_Repartition (PACKET_T *pakcet, void *arg);

#endif 	//CONFIG_EMMC_BOOT
#endif	//_DISK_PART_UEFI_H 
