#ifndef DL_ENGINE_H
#define DL_ENGINE_H

#include "cmd_def.h"

typedef enum 
{
    DL_STAGE_NONE,
    DL_STAGE_CONNECTED,    
    DL_STAGE_DATA_TRANSFER
} DL_STAGE;

/* The prototype of hook of packet handler.
 * If the packet is handled successfully, 1 is returned.
 * Otherwise, 0 is returned.
 */
typedef int (*CMDPROC)(PKT_HEADER*, void*);

/******************************************************************************
 * dl_init
 *
 * Description	:
 *
 * Parameters	:
 *
 * Returns		:
 * 		0  - Failed to initialize dl engine.
 *		!0 - ok.
 ******************************************************************************/
unsigned char dl_init(void);

/******************************************************************************
 * dl_reg
 *
 * Description	:
 *
 * Parameters	:
 *
 * Returns		:
 *		0  - Failed to register the handler.
 *		!0 - OK.
 ******************************************************************************/
unsigned char dl_reg(CMD_TYPE cmd, CMDPROC proc, void * arg); 

/******************************************************************************
 * dl_entry
 *
 * Description	:
 * 		When the function is called, a infinite loop is entered to receive and 
 *		handle packet from PC for ever.
 ******************************************************************************/ 
int dl_entry(DL_STAGE start);

#endif /* DL_ENGINE_H */
