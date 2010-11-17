#ifndef MCU_COMMAND_H
#define MCU_COMMAND_H

#include "cmd_def.h"
#include "packet.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
 * mcu_reset_normal
 ******************************************************************************/
int FDL_McuResetNormal (PACKET_T *packet, void *arg);

/******************************************************************************
 * mcu_reset_boot
 *
 * This function is for testing FDL.
 ******************************************************************************/
int FDL_McuResetBoot (PACKET_T *pakcet, void *arg);

/******************************************************************************
 * mcu_read_chip_type
 ******************************************************************************/
int FDL_McuReadChipType (PACKET_T *packet, void *arg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MCU_COMMAND_H */