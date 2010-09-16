#ifndef MCU_COMMAND_H
#define MCU_COMMAND_H

#include "cmd_def.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
 * mcu_reset_normal
 ******************************************************************************/
int mcu_reset_normal(PKT_HEADER *packet, void *arg);

/******************************************************************************
 * mcu_reset_boot
 * 
 * This function is for testing FDL.
 ******************************************************************************/
int mcu_reset_boot(PKT_HEADER *pakcet, void *arg);

/******************************************************************************
 * mcu_read_chip_type
 ******************************************************************************/
int mcu_read_chip_type(PKT_HEADER *packet, void *arg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MCU_COMMAND_H */
