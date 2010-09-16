#include "dl_engine.h"
#include "packet.h"

typedef struct _DL_STATUS {
	DL_STAGE stage;
	int		 data_verify;	/* Record the result of the latest file download 
	 						 * operation. */
} DL_STATUS, *PDL_STATUS;

typedef struct _CMDPROC_TAB
{
    CMDPROC    proc;
    void	 * arg;
} CMDPROC_TAB, *PCMDPROC_TAB;

CMDPROC_TAB g_proctab[BSL_CMD_TYPE_MAX - BSL_CMD_TYPE_MIN] = {0, 0};

#define IS_VALID_CMD(cmd)			((cmd >= BSL_CMD_TYPE_MIN) && (cmd < BSL_CMD_TYPE_MAX))
#define CMD_IND(cmd)				((cmd) - BSL_CMD_TYPE_MIN)				
#define CMD_PROC(cmd)				(g_proctab[CMD_IND(cmd)].proc)
#define CMD_ARG(cmd)				(g_proctab[CMD_IND(cmd)].arg)
#define CALL_PROC(cmd, packet)		(CMD_PROC(cmd)((packet), CMD_ARG(cmd)))

unsigned char dl_init(void)
{
	memset(g_proctab, 0, sizeof(g_proctab));
	return 1;
}

unsigned char dl_reg(CMD_TYPE cmd, CMDPROC proc, void * arg)
{	
	if (!IS_VALID_CMD(cmd))
		return 0;
	
	CMD_PROC(cmd) = proc;
	CMD_ARG(cmd) = arg;
	return 1; 
}

/* 
 * The finite state machine of dl_entry
 *     _______________
 *    |               |
 *    | DL_STAGE_NONE |   
 *    |_______________|
 *            |
 *            | BSL_CMD_CONNECT
 *  _________\|/________                       ________________________
 * |                    | BSL_CMD_START_DATA  |                        |
 * |                    |-------------------->|                        |
 * | DL_STAGE_CONNECTED |                     | DL_STAGE_DATA_TRANSFER |
 * |				    |<--------------------|                        |
 * |____________________|  BSL_CMD_END_DATA   |________________________|
 *      |         /|\                              |            /|\
 *      |__________|                               |_____________|
 *      other command                             BSL_CMD_MIDST_DATA
 *
 * Note: 
 * If the operations of BSL_CMD_START_DATA, BSL_CMD_MIDST_DATA and 
 * BSL_CMD_END_DATA fail, the stage of the machine will return back 
 * to BSL_STAGE_CONNECTED, which though is not shown.
 */  
 
int dl_entry(DL_STAGE start)
{
	CMD_TYPE cmd;
	PKT_HEADER * packet = 0;
	DL_STATUS status; 
	status.stage = start;
	status.data_verify = 0;
	
	while (1) {
		packet = get_packet();
		cmd =(CMD_TYPE)packet->type;
		
		/* Check the validity of the command. */
		if (!IS_VALID_CMD(cmd) || (0 == CMD_PROC(cmd))) {
			send_ack_packet(BSL_REP_UNKNOW_CMD);
			free_packet(packet);
			continue;
		}
		
		switch (status.stage) {
		case DL_STAGE_NONE:
			if (BSL_CMD_CONNECT != cmd) {
				send_ack_packet(BSL_REP_INVALID_CMD);
				break;
			}
				
			if (CALL_PROC(cmd, packet))
				status.stage = DL_STAGE_CONNECTED;
			break;
				
		case DL_STAGE_DATA_TRANSFER:
			if ((BSL_CMD_END_DATA != cmd) && (BSL_CMD_MIDST_DATA != cmd)) {
				send_ack_packet(BSL_REP_INVALID_CMD);
				break;
			}
				
			if (CALL_PROC(cmd, packet)) { 
				if (BSL_CMD_END_DATA == cmd) {
					status.stage = DL_STAGE_CONNECTED;
					status.data_verify = 1;
				}
			} else {
				status.stage = DL_STAGE_CONNECTED;
			}
			break;
				
		default: /* DL_STAGE_CONNECTED */
			if ((BSL_CMD_CONNECT == cmd) || (BSL_CMD_MIDST_DATA == cmd)
				|| (BSL_CMD_END_DATA == cmd)) {
				send_ack_packet(BSL_REP_INVALID_CMD);
				break;
			}
				
			if ((BSL_CMD_EXEC_DATA == cmd) && !status.data_verify) {
				send_ack_packet(BSL_REP_NOT_VERIFY);
				break;
			}
				
			if (BSL_CMD_START_DATA == cmd) {
				if (CALL_PROC(cmd, packet)) {
					status.stage = DL_STAGE_DATA_TRANSFER;
					status.data_verify = 0;
				}
			} else {
				/* Other command */
				CALL_PROC(cmd, packet);
			}
			break;
		} /* switch */
		free_packet(packet);
	} /* while */
	/* How could we get here. */
	return 0;/*lint !e527 */
}


