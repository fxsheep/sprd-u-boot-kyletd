#ifndef PACKET_H
#define PACKET_H

#include "cmd_def.h"


/******************************************************************************
 * packet_init
 ******************************************************************************/
void packet_init(void);

/******************************************************************************
 * malloc_packet
 *
 * Description		:
 *
 * Parameters		:
 *		size - The size of the data field of the packet in bytes.
 *
 * Returns			:
 *		0 - If the required size exceeds MAX_PKT_SIZE or there is no free memory,
 *			0 will be returned.
 ******************************************************************************/
PKT_HEADER * malloc_packet(unsigned long size);

/******************************************************************************
 * free_packet
 ******************************************************************************/
void free_packet(PKT_HEADER * ptr);

/******************************************************************************
 * get_packet
 *
 * Get the next packet. After it has been handled, free_packet should be called
 * to release it.
 ******************************************************************************/
PKT_HEADER * get_packet(void);

/******************************************************************************
 * send_packet
 ******************************************************************************/
void send_packet(PKT_HEADER * packet);

/******************************************************************************
 * send_ack_packet
 ******************************************************************************/
void send_ack_packet(DLSTATUS status);

/******************************************************************************
 * peek_packet
 *
 * Description	:
 *		This function is to be called when system is in idle to process the input 
 * 		character.
 *
 * Parameters	:
 *		None.
 *
 * Returns		:
 *		1  - A packet has been received successfully.
 *		0  - No character or free slot available.
 *		-1 - An error has occured.
 ******************************************************************************/
int peek_packet(void);

#endif  // PACKET_H


