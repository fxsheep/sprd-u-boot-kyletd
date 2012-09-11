/******************************************************************************
 ** File Name:      dm_log.h                                                    *
 ** Author:                                                                   *
 ** DATE:           06/26/2007                                                *
 ** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define                                          *	
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/26/2007                      Create                                    *
 ******************************************************************************/

#ifndef LOG_H
#define LOG_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines.                               
 **---------------------------------------------------------------------------*/
//#define	FOTA_DEBUG
 #ifdef	FOTA_DEBUG
#define UA_TRACE 		SCI_TraceLow
#else
#define	UA_TRACE(x)
#endif
#define  UA_TRACE_ERR	SCI_TraceLow
/**---------------------------------------------------------------------------*
 **                         Struct defines.                               
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function put a message to the trace buffer which log
//                  level is no more than LOG_LEVEL_LOW. 
//                  This function put a message to the trace buffer. 
//                  1.msg_trace_info.buf_is_send == TRACE_BUF_SEND
//                    Send the messages to the application tool when buffer 
//                      is full.
//                  2.msg_trace_info.buf_is_send == TRACE_BUF_COVER
//                    Always cover the oldest message when buffer is full.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/ 
PUBLIC uint32 SCI_TraceLow(
    const char *x_format, ...);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif //_SIO_H