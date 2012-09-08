/******************************************************************************
 ** File Name:      sci_log.h                                                 *
 ** Author:         Richard Yang                                              *
 ** DATE:           10/18/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/18/2001     Richard.Yang     Create.                                   *
 ** 12/30/2001     Xueliang.Wang    Modified                                  *
 **                Move log and trace from op_api.h and sci_api.h.            *
 ******************************************************************************/
#ifndef _SCI_LOG_H
#define _SCI_LOG_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "rtos_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define MAX_STR_SIZE            250
#define MAX_LOG_SIZE            1500
#define LOG_BUF_SEND            1
#define LOG_BUF_COVER           0    

// Log flag value.
// User could use the combination of the flowing value 
// to set log filter of the thread.
#define LOG_LEVEL_FATAL         (0x00000001)
#define LOG_LEVEL_ERROR         (0x00000002 | LOG_LEVEL_FATAL)
#define LOG_LEVEL_HIGH          (0x00000004 | LOG_LEVEL_ERROR)
#define LOG_LEVEL_MED           (0x00000008 | LOG_LEVEL_HIGH)
#define LOG_LEVEL_LOW           (0x00000010 | LOG_LEVEL_MED)
#define LOG_TIMER_MODE          (0x00000020)
#define LOG_SEND_MSG_MODE       (0x00000040)
#define LOG_RECEIVE_MSG_MODE    (0x00000080)
#define LOG_DISCARD_MSG_MODE    (0x00000100)
#define LOG_STATE_MODE          (0x00000200)
#define LOG_FLOW_CNT_MODE       (0x00000400)
#define LOG_L1_DSP_MODE         (0x00000800)

// Log all out.
#define LOG_ALL_MODE            (0xFFFFFFFF)
// Don't log anything out.
#define LOG_NONE_MODE           (0x0)

#define LOG_TRACE_MSK           (0x0000001F)

typedef enum
{
	MSG_RECEIVE_LOG = 0x9100,
	MSG_SEND_LOG,
    STATE_LOG,
	TIMER_LOG,
	TRACE_LOG,
	FLOW_CNT_LOG,
    L1_DSP_LOG,
    MSG_DISCARD_LOG
} COMM_LOG_ID_E;

// Default LOG flag.
// The flags of all dynamic threads always equal this value.
// The initialized flags of all static threads equal this value. 
// But they will be changed to the last value saved in NVItem.
#define SCI_DEFAULT_LOG_FLAG \
        (LOG_LEVEL_LOW | LOG_TIMER_MODE | LOG_RECEIVE_MSG_MODE | LOG_DISCARD_MSG_MODE | LOG_STATE_MODE)

// @Xueliang.Wang added to log struct with pointer 
// when process GPRS message(2002-08-05)
typedef enum
{
    LOG_STRUCT_WITHOUT_PTR = 0,
    LOG_STRUCT_WITH_NULLPTR,
    LOG_STRUCT_WITH_BLOCK_SIGNAL_PTR,   // Need to parse
    LOG_STRUCT_WITH_BLOCK_DATA_PTR,     // Need not to parse
    LOG_STRUCT_WITH_STRUCTPTR,
    LOG_STRUCT_WITH_STRUCTLISTPTR
} LOG_STRUCT_POINTER_TYPE_E;

typedef void (* LOG_FPTR)(uint8 **, uint16, const uint8 *, uint16 *);
// @Xueliang.Wang add end(2002-08-05)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef struct LOG_INFO_tag
{
    uint16         lost_frame_count;
} LOG_INFO_T;

typedef struct LOG_FILTER_tag
{
    BLOCK_ID    block_id;     
    uint32      filter;       
} LOG_FILTER_T;

// Message store structure.
typedef struct TRACE_MSG_HEAD_tag
{
    uint16     SignalCode;
	uint16     SignalSize;
} TRACE_LOG_HEAD_T;

typedef struct LOG_FRAME_HEAD_tag
{
    uint16     SignalCode;
	uint16     SignalSize;
    uint8      state;
    uint8      padding;
    uint16     msg_type;
    uint32     sender;
    uint32     receiver;
    uint32     timestamp;
}LOG_FRAME_HEAD_T;
typedef LOG_FRAME_HEAD_T MSG_RECEIVE_LOG_HEAD_T;
typedef LOG_FRAME_HEAD_T MSG_SEND_LOG_HEAD_T;
typedef LOG_FRAME_HEAD_T MSG_DISCARD_LOG_HEAD_T;

typedef struct LOG_STATE_HEAD_tag
{
    uint16     SignalCode;
	uint16     SignalSize;
    uint8      state;
    uint8      padding1;
    uint16     padding2;
    uint32     sender;
    uint32     timestamp;
} STATE_LOG_HEAD_T;

typedef struct LOG_TIMER_HEAD_tag
{
    uint16     SignalCode;
	uint16     SignalSize;
	uint8      action;          // Active or reset.
	uint8      padding1;        // @xueliang.Wang added.
	uint16     padding2;        // @xueliang.Wang added.
    uint32     caller;
	uint32     timer_name;
    uint32     timestamp;
    char       timer_name_string[40];
} TIMER_LOG_HEAD_T;

typedef struct LOG_FLOW_CNT_HEAD_tag
{
    uint16     SignalCode;
	uint16     SignalSize;
    uint32     timestamp;
	uint32     flow_cnt[20];
} FLOW_CNT_LOG_HEAD_T;

typedef struct LOG_L1_DSP_HEAD_tag
{
    uint16     SignalCode;
	uint16     SignalSize;
    uint16     msg_type;
    uint16     src_len;    // Length of the content below.
    uint32     timestamp;
} L1_DSP_LOG_HEAD_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function initialize log. 
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_InitLog(void);

/*****************************************************************************/
//  Description:    This function initialize log switch.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_InitLogSwitch(
    BOOLEAN b_log   // SCI_TRUE: log        SCI_FALSE: not log
    );

/*****************************************************************************/
//  Description:    This function initialize log filter of static thread. 
//                  Read the old values from NVItem, and set log filters 
//                  use these values.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_InitLogFilter(void);
 
/*****************************************************************************/
//  Description:    The function gets log flag of the specified thread.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           If the thread is dynamic thread, return DYNAMIC_THREAD_LOG_FLAG;
//                  else return log flag of the specified thread.
/*****************************************************************************/
PUBLIC uint32 SCI_GetThreadLogFlag(
    BLOCK_ID thread_id              // ID of specified thread
    );
    
/*****************************************************************************/
//  Description:    The function sets log flag of the specified thread.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_SetThreadLogFlag( // If set successfully, return SCI_SUCCESS,
                                    // else return SCI_ERROR.
    BLOCK_ID    thread_id,          // ID of specified thread
    uint32      log_flags           // Log flags used to be.
    );

/*****************************************************************************/
//  Description:    Packet message with pointer, the destination packet saved 
//                  in dest_ptr. If the message with no pointer, don't handle
//                  it, dest_ptr is still NULL.
//	Global resource dependence: 
//  Author: 
//	Note:           Set or cancel thread log filter. now, static thread can 
//                  filter separate, all dynamic threads share one filter flag.
//                  this means if you set one dynamic thread be filter(or not),
//                  all dynamic will be filter(or not). 
/*****************************************************************************/
PUBLIC void SCI_LogSetFilter(
    uint8 *info_ptr, 
    uint16 len
    );

/*****************************************************************************/
//  Description:    Get all static thread log filter information.
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_LogGetFilterInfo(
    uint8 * info_ptr
    );

/*****************************************************************************/
// Description :    
// Global resource dependence : 
// Author : 
// Note :
/*****************************************************************************/
PUBLIC void SCI_LogFlowCnt(
    uint32 * flow_cnt_ptr, 
    int number
    );

/*****************************************************************************/
// Description :    
// Global resource dependence : 
// Author : 
// Note :
/*****************************************************************************/
//void es_log_l1_put(uint16 msg_type, uint8 *content, uint16 len)
PUBLIC void SCI_LogL1Put(
    uint16  msg_type, 
    uint8   *content, 
    uint16  len
    );

/*****************************************************************************/
// Description :    This function is used to regist repacket function to 
//                  handle GPRS message.
// Global resource dependence : 
// Author : Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void SCI_RegLogGprsMsg(
    LOG_FPTR    func_fptr
    );

/*****************************************************************************/
//  Description:    The function is used to log out information of message.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_LogMsg(
    BLOCK_ID    sender,     // The sender ID of this message.         
    BLOCK_ID    receiver,   // The receiver ID of this message.       
    uint8       state,      // State of task who receive this message.
    uint16      msg_type,   // Type of this message.                  
    uint8       * content,  // Pointer to content of this message.    
    uint16      len,        // Length of this message.
    uint32      log_id      // log ID: 	MSG_RECEIVE_LOG
                            //      	MSG_SEND_LOG    
                            //          MSG_DISCARD_LOG
    );
    
/*****************************************************************************/
//  Description:    The function is used to log out state of the special 
//                  thread, which id is self_id.
//	Global resource dependence:
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_LogState(
    BLOCK_ID    self_id,        // ID of the special thread.                 
    uint8       state,          // State of the thread.    
    const char  *state_name     // State name string of the state.
    );
    
/*****************************************************************************/
//  Description:    The function is used to log out the information of the 
//                  special timer.
//	Global resource dependence:
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_LogTimer(
    BLOCK_ID   thread_id,       // ID of the thread.                       
    uint8      action,          // Action on the timer, SET/RESET/ACTIVE.
    uint32     timer_id,        // ID of timer.                          
    const char *name_string     // Timer name string of timer_id.       
    );
/*****************************************************************************/
//  Description:    Trace message out by IO.
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int SendMsgOut(
    char    *format_str_ptr,    // Pointer to message to be sent out.
    int     len                 // Length of message.
    );
//#ifdef _D_PH_R_
//#undef TRACE_INFO_SUPPORT
//#endif

//#ifdef TRACE_INFO_SUPPORT 
#if defined( TRACE_INFO_SUPPORT ) || defined( UEIT_MOCOR )
    #define SCI_LOG_INPUT_MSG(_SENDER, _RECEIVER, _STATE, _MSG_TYPE, _CONTENT, _LEN) \
        SCI_LogMsg(_SENDER, _RECEIVER, _STATE, _MSG_TYPE, _CONTENT, _LEN, MSG_RECEIVE_LOG);
    #define SCI_LOG_OUTPUT_MSG(_SENDER, _RECEIVER, _STATE, _MSG_TYPE, _CONTENT, _LEN) \
        SCI_LogMsg(_SENDER, _RECEIVER, _STATE, _MSG_TYPE, _CONTENT, _LEN, MSG_SEND_LOG);
    #define SCI_LOG_DISCARD_MSG(_SENDER, _RECEIVER, _STATE, _MSG_TYPE, _CONTENT, _LEN) \
        SCI_LogMsg(_SENDER, _RECEIVER, _STATE, _MSG_TYPE, _CONTENT, _LEN, MSG_DISCARD_LOG);        
    #define SCI_LOG_STATE           SCI_LogState
    #define SCI_LOG_TIMER           SCI_LogTimer
#else
    #define SCI_LOG_INPUT_MSG
    #define SCI_LOG_OUTPUT_MSG
    #define SCI_LOG_DISCARD_MSG 
    #define SCI_LOG_STATE
    #define SCI_LOG_TIMER 
#endif  // End of TRACE_INFO_SUPPORT 

/**----------------------------- ----------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /*End sci_log.h*/