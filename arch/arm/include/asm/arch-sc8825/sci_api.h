/******************************************************************************
 ** File Name:      sci_api.h                                                 *
 ** Author:         Richard Yang                                              *
 ** DATE:           11/10/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/10/2001     Richard.Yang     Create.                                   *
 ** 08/01/2002     Xueliang.Wang    Add some macros.                          *
 ** 11/15/2002     Xueliang.Wang    Move some function prototype to os_api.h  *
 ******************************************************************************/

#ifndef _SCI_API_H
#define _SCI_API_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_log.h"
#include "os_api.h"

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
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
// TIMER structure pointer type.
typedef void    * SCI_TMID;

// Operation on timer.
typedef enum
{
	SET_TIMER,
    RESET_TIMER,
    TIMER_OUT
} SDL_TIMER_OP_T;

// Operation on signal.
typedef enum
{
    SCI_SIG_OPERATE_INPUT,
    SCI_SIG_OPERATE_DISCARD,
    SCI_SIG_OPERATE_SAVE
} SCI_SIG_OPERATE_E;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

     
/**---------------------------------------------------------------------------*
 ** LIST
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Init a save_queue.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void     SCI_InitSaveQ(
    xSignalHeader  *siglist_ptr     // Pointer to the signal list head
    );

/*****************************************************************************/
//  Description:    Save the signal to the current signal list. Just Insert
//                  the signal to the tail of the list.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void     SCI_PutSignalToSaveQ(
    xSignalHeader *siglist_ptr,     // Pointer to the signal list head
    xSignalHeader input_signal      // Pointer to the input signal 
    );

/*****************************************************************************/
//  Description:    Pickout the oldest signal from the signal list. 
//                  And delete it.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC xSignalHeader SCI_GetSignalFromSaveQ(    // Return signal list head
    xSignalHeader *siglist_ptr      // Pointer to the signal list head 
    );

/*****************************************************************************/
//  Description:    Free all the signals in the signal list. 
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void     SCI_DeleteSaveQ(
    xSignalHeader  *siglist_ptr     // Pointer to the signal list head
    );


/**---------------------------------------------------------------------------*
 ** MEMORY MANAGE
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The SCI_Mem16Cpy function is used to copy short characters
//                  between buffers, it is mainly used by layer1, when access
//                  share memory, it has to access the memory by 16 bit.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           Here count is the total size of the 16 number!
/*****************************************************************************/
PUBLIC void *SCI_Mem16Cpy(      // None
    uint16  *dest_ptr,          // New buffer
    const uint16 *src_ptr,      // Buffer to copy from
    uint16  count               // Number of word(16 bit) to copy
    );
        
/*****************************************************************************/
//  Description:    The SCI_Mem16Cpy function is used to set buffers to a 
//                  specified character, it is mainly used by layer1, when 
//                  access share memory, it has to access the memory by 16 bit.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           Here count is the total size of byte
/*****************************************************************************/
PUBLIC void *SCI_Mem16Set(  // None
    uint16  *dest_ptr,      // Pointer to destination
    int16   c,              // Character to set
    uint32  count           // Number of word(byte)
    );


/**---------------------------------------------------------------------------*
 ** TIMER
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Create a timer if there is no timer, or it changes
//                  the timer property and restart the timer.
//	Global resource dependence:
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_SetSdlTimer(            // None
        BLOCK_ID    thread_id,          // ID of thread which sets timer
        uint32      time_dur_in_ms,     // Expire time in milliseconds
        uint32      timer_id,           // Id of the timer, used to identify
        SCI_TMID    *tmid_ptr,          // Pointer to pointer to timer to be set
        const char  *timer_name_string  // Pointer to timer name string
        );

/*****************************************************************************/
//  Description:    Get the remain milliseconds of the current timer.
//	Global resource dependence:
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_GetSdlTimerRemainTime(    // Return the remain milliseconds
                                            // of a timer
    SCI_TMID *tmid_ptr                      // Pointer to a previously created 
                                            // application timer.
);

/*****************************************************************************/
//  Description:    Delete the proper timer, or delete any timer signal 
//                  either in save_queue or input queue.
//	Global resource dependence:
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_KillSdlTimer(   // None
        BLOCK_ID    thread_id,  // ID of thread which kills timer
        uint32      timer_id,   // Id of the timer, used to identify
        SCI_TMID    *tmid_ptr   // Pointer to pointer to timer to be killed
        );

/*****************************************************************************/
//  Description:    Check the timer, if it is active.
//	Global resource dependence:
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsSdlTimerActive(   // Active return SCI_TRUE, 
                                    // else return SCI_FALSE
        BLOCK_ID    self_id,        // ID of thread which checks timer
        uint32      timer_id,       // Id of the timer, used to identify
        SCI_TMID    *tmid_ptr       // Pointer to pointer to timer to be checked
        );

/**---------------------------------------------------------------------------*
 **                         OS API MACROS                                     *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 ** IRQ INTERRUPT MACRO:  
 **         SCI_DISABLE_IRQ
 **         SCI_RESTORE_IRQ
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Disable IRQ interrupt
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      None
//  Return:         
//	Note:
/*****************************************************************************/
/******************************************************************************
void    SCI_DISABLE_IRQ
******************************************************************************/
#define SCI_DISABLE_IRQ \
        SCI_DisableIRQ();

/*****************************************************************************/
//  Description:    Enable IRQ interrupt
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      None
//  Return:         
//	Note:
/*****************************************************************************/
/******************************************************************************
void    SCI_RESTORE_IRQ
******************************************************************************/
#define SCI_RESTORE_IRQ \
        SCI_RestoreIRQ();

/**---------------------------------------------------------------------------*
 ** LIST MACRO: 
 **         SCI_SIGLIST_INIT
 **         SCI_SIGLIST_ADDTAIL
 **         SCI_SIGLIST_GETHEAD
 **         SCI_SIGLIST_DELALL
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Initialize the signal list.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _LIST_PTR           Pointer to the signal list.
//  Return:         None
//	Note:
/*****************************************************************************/
/******************************************************************************    
void     SCI_SIGLIST_INIT(
    xSignalHeader  *_LIST_PTR
    );
******************************************************************************/
#define SCI_SIGLIST_INIT(_LIST_PTR) \
        SCI_InitSaveQ(_LIST_PTR)

/*****************************************************************************/
//  Description:    Add a signal to the tail of  signal list.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _LIST_PTR           Pointer to the signal list.
//                  _SIG_PTR            Pointer to the sinal to be added.
//  Return:         None
//	Note:
/*****************************************************************************/
/******************************************************************************
void     SCI_SIGLIST_ADDTAIL(
    xSignalHeader *_LIST_PTR,
    xSignalHeader _SIG_PTR 
    );
******************************************************************************/
#define SCI_SIGLIST_ADDTAIL(_LIST_PTR, _SIG_PTR) \
        SCI_PutSignalToSaveQ(_LIST_PTR, _SIG_PTR)
    
/*****************************************************************************/
//  Description:    Get the head of the signal list. 
//                  And receive it from the list.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _LIST_PTR           Pointer to the signal list.
//  Return:         Signal list head.
//	Note:
/*****************************************************************************/
/******************************************************************************    
xSignalHeader SCI_SIGLIST_GETHEAD(
    xSignalHeader *_LIST_PTR
    );
******************************************************************************/
#define SCI_SIGLIST_GETHEAD(_LIST_PTR) \
        SCI_GetSignalFromSaveQ(_LIST_PTR)

/*****************************************************************************/
//  Description:    Delete the signal list.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _LIST_PTR           Pointer to the signal list.
//  Return:         None
//	Note:
/*****************************************************************************/
/******************************************************************************    
void     SCI_SIGLIST_DELALL(
    xSignalHeader  *_LIST_PTR
    );
******************************************************************************/
#define SCI_SIGLIST_DELALL(_LIST_PTR) \
        SCI_DeleteSaveQ(_LIST_PTR)


/**---------------------------------------------------------------------------*
 ** NEXTSTATE MACRO: 
 **         SCI_NEXTSTATE
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Set the next state of the module.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _STATE              State variable.
//                  _STATE_VALUE        Value of next state.
//                  _STATE_NAME_STRING  Name of next state.
//                  _SELF_ID            ID of thread which call this function. 
//  Return:         None
//	Note:
/*****************************************************************************/
/******************************************************************************    
SCI_NEXTSTATE(
        uint8       _STATE, 
        uint8       _STATE_VALUE, 
        char        * _STATE_NAME_STRING, 
        BLOCK_ID    _SELF_ID
        )
******************************************************************************/
#define SCI_NEXTSTATE(_STATE, _STATE_VALUE, _STATE_NAME_STRING, _SELF_ID)\
		_STATE = _STATE_VALUE; \
		SCI_LOG_STATE(_SELF_ID, _STATE_VALUE, _STATE_NAME_STRING);



/**---------------------------------------------------------------------------*
 ** THREAD MACRO: 
 **     SCI_CREATE_THREAD
 **     SCI_CREATE_STATIC_THREAD
 **     SCI_STOP_THREAD
 **     SCI_IDENTIFY_THREAD
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Create dynamic thread.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _THREAD_NAME    Name string of the thread                
//                  _QUEUE_NAME     Queue string of the thread                   
//                  _ENTRY          Entry function of the thread
//                  _ARGC           First parameter for entry function 
//                  _ARGV           Second parameter for entry function,     
//                                  argc has been used as thread id          
//                  _STACK_SIZE     Size of the thread stack in bytes            
//                  _QUEUE_NUM      Number of messages which can be enqueued     
//                  _PRIO           Prority of the thread.                       
//                  _PREEMPT        Indicates if the thread is preemptable.
//                  _AUTO_START     If auto start the thread after creation      
//  Return:         ID of thread 
//	Note:
/*****************************************************************************/
/******************************************************************************
BLOCK_ID SCI_CREATE_THREAD(
    char   *_THREAD_NAME,              
    char   *_QUEUE_NAME,               
    void   (*_ENTRY) (uint32, void *),
    uint32 _ARGC,
    void   *_ARGV, 
    uint32 _STACK_SIZE,                
    uint32 _QUEUE_NUM,                 
    uint32 _PRIO,                      
    uint32 _PREEMPT,  
    uint32 _AUTO_START                 
    );
******************************************************************************/
#define SCI_CREATE_THREAD(_THREAD_NAME, _QUEUE_NAME, _ENTRY, _ARGC, _ARGV, \
            _STACK_SIZE, _QUEUE_NUM, _PRIO, _PREEMPT, _AUTO_START) \
        SCI_CreateThread(_THREAD_NAME, _QUEUE_NAME, _ENTRY, _ARGC, _ARGV, \
            _STACK_SIZE, _QUEUE_NUM, _PRIO, _PREEMPT, _AUTO_START)

/*****************************************************************************/
//  Description:    Creates a static thread to execute. 
//	Global resource dependence: 
//  Author: Richard.Yang
//  Parameter:      _THREAD_ID      Static thread ID
//                  _THREAD_NAME    Name string of the thread                
//                  _ENTRY          Entry function of the thread 
//                  _ARGC           First parameter for entry function 
//                  _ARGV           Second parameter for entry function,     
//                                  argc has been used as thread id          
//                  _STACK_START    Start pointer of the thread stack
//                  _STACK_SIZE     Size of the thread stack in bytes            
//                  _PRIO           Prority of the thread.                       
//                  _PREEMPT        Indicates if the thread is preemptable.
//                  _QUEUE_NAME     Queue string of the thread
//                  _QUEUE_START    Start pointer of the queue 
//                  _QUEUE_NUM      Number of messages which can be enqueued     
//                  _AUTO_START     If auto start the thread after creation      
//  Return:         ID of thread 
//	Note:           
/*****************************************************************************/
/******************************************************************************
PUBLIC BLOCK_ID SCI_CREATE_STATIC_THREAD(   // If successful, returns the 
                                            // thread ID of the new created 
                                            // thread, else returns Zero.
    BLOCK_ID    _THREAD_ID,
    char        *_THREAD_NAME, 
    void        *_ENTRY,
    uint32      _ARGC,
    void        *_ARGV,
    void        *_STACK_START, 
    uint32      _STACK_SIZE,   
    uint32      _PRIO,         
    uint32      _PREEMPT, 
    char        *_QUEUE_NAME,  
    void        *_QUEUE_START, 
    uint32      _QUEUE_NUM,    
    uint32      _AUTO_START    
    );
******************************************************************************/
#define SCI_CREATE_STATIC_THREAD(_THREAD_ID, _THREAD_NAME, _ENTRY, _ARGC, \
            _ARGV, _STACK_START, _STACK_SIZE, _PRIO, _PREEMPT, _QUEUE_NAME, \
            _QUEUE_START, _QUEUE_NUM, _AUTO_START) \
        SCI_CreateStaticThread(_THREAD_ID, _THREAD_NAME, _ENTRY, _ARGC, \
            _ARGV, _STACK_START, _STACK_SIZE, _PRIO, _PREEMPT, _QUEUE_NAME, \
            _QUEUE_START, _QUEUE_NUM, _AUTO_START)

/*****************************************************************************/
//  Description:    Stop the thread which call this function.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:
//  Return:         None.
//	Note:
/*****************************************************************************/
/******************************************************************************
PUBLIC void SCI_STOP_THREAD(void);
******************************************************************************/
#define SCI_STOP_THREAD() \
        SCI_ThreadExit()

/*****************************************************************************/
//  Description:    Get ID of the thread which call this function.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:
//  Return:         ID of thread 
//	Note:
/*****************************************************************************/
/******************************************************************************
BLOCK_ID    SCI_IDENTIFY_THREAD(void);
******************************************************************************/
#define SCI_IDENTIFY_THREAD() \
        SCI_IdentifyThread()

/*****************************************************************************/
//  Description:    Suspends the execution of the current thread for a 
//                  specified interval. 
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
/******************************************************************************
void    SCI_SLEEP(uint32 _TIME_IN_MS);
******************************************************************************/
#define SCI_SLEEP(_TIME_IN_MS) \
        SCI_Sleep(_TIME_IN_MS)
    
/**---------------------------------------------------------------------------*
 ** TIME MACRO:  
 **     SCI_GET_CURRENT_TIME
 **     SCI_GET_CURRENT_TICKCOUNT
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function gets the system time.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:           In win32, return the number of seconds elapsed since
//                  midnight (00:00:00), January 1, 1970, coordinated 
//                  universal time, according to the system clock.
//                  In ARM, it is not defined.
/*****************************************************************************/
/******************************************************************************
uint32  SCI_GET_CURRENT_TIME(void);
******************************************************************************/
#define SCI_GET_CURRENT_TIME() \
		SCI_GetCurrentTime()

/*****************************************************************************/
//  Description:    This function retrieves the number of milliseconds that 
//                  have elapsed since the system was started.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:           The elapsed time is stored as a uint32 value. Therefore, 
//                  the time will wrap around to zero if the system is run 
//                  continuously for 49.7 days. 
/*****************************************************************************/
/******************************************************************************
uint32 SCI_GET_CURRENT_TICKCOUNT(void);
******************************************************************************/
#define SCI_GET_CURRENT_TICKCOUNT() \
		SCI_GetTickCount()

/**---------------------------------------------------------------------------*
 ** TIMER MACRO: 
 **     SCI_SET_TIMER
 **     SCI_KILL_TIMER
 **     SCI_IS_TIMER_ACTIVE
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Create a timer if there is no timer, or it changes
//                  the timer property and restart the timer.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _SELF_ID            ID of thread which sets timer
//                  _TIME_DUR_IN_MS     Expire time in milliseconds
//                  _TIMER_ID           Id of the timer, used to identify
//                  _TMID_PTR           Pointer to tmid to be set
//                  _TIMER_NAME_STRING  Pointer to timer name string
//  Return:         None   
//	Note:
/*****************************************************************************/
/******************************************************************************
void SCI_SET_TIMER(               
        BLOCK_ID    _SELF_ID,            
        uint32      _TIME_DUR_IN_MS,      
        uint32      _TIMER_ID,            
        SCI_TMID    * _TMID_PTR,     
        const char  *_TIMER_NAME_STRING   
        )
******************************************************************************/
#define SCI_SET_TIMER(_SELF_ID, _TIME_DUR_IN_MS, _TIMER_ID, _TMID_PTR, \
            _TIMER_NAME_STRING) \
        SCI_LOG_TIMER(_SELF_ID, SET_TIMER, _TIMER_ID, _TIMER_NAME_STRING); \
        SCI_SetSdlTimer(_SELF_ID, _TIME_DUR_IN_MS, _TIMER_ID, _TMID_PTR, \
            _TIMER_NAME_STRING);

/*****************************************************************************/
//  Description:    Delete the proper timer, or delete any timer signal 
//                  either in save_queue or input queue.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _SELF_ID            ID of thread which kills timer
//                  _TIMER_ID           ID of the timer, used to identify
//                  _TMID_PTR           Pointer to tmid to be killed
//                  _TIMER_NAME_STRING  Pointer to timer name string
//  Return:         None   
//	Note:
/*****************************************************************************/
/******************************************************************************
void SCI_KILL_TIMER(         
        BLOCK_ID    _SELF_ID,          
        uint32      _TIMER_ID,          
        SCI_TIMID   * _TMID_PTR  
        )
******************************************************************************/
#define SCI_KILL_TIMER(_SELF_ID, _TIMER_ID, _TMID_PTR, _TIMER_NAME_STRING) \
        SCI_LOG_TIMER(_SELF_ID, RESET_TIMER, _TIMER_ID, _TIMER_NAME_STRING); \
        SCI_KillSdlTimer(_SELF_ID, _TIMER_ID, _TMID_PTR);

/*****************************************************************************/
//  Description:    Check the timer, if it is active.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _SELF_ID            ID of thread which checks timer
//                  _TIMER_ID           Id of the timer, used to identify
//                  _TMID_PTR           Pointer to timer
//  Return:         Active return SCI_TRUE, else return SCI_FALSE
//	Note:
/*****************************************************************************/
/******************************************************************************
PUBLIC BOOLEAN SCI_IS_TIMER_ACTIVE( // Active return SCI_TRUE, 
                                    // else return SCI_FALSE
        BLOCK_ID    _SELF_ID,       // ID of thread which checks timer
        uint32      _TIMER_ID,      // Id of the timer, used to identify
        SCI_TMID    * _TMID_PTR     // Pointer to timer to be checked
        );
******************************************************************************/
#define SCI_IS_TIMER_ACTIVE(_SELF_ID, _TIMER_ID, _TMID_PTR) \
        SCI_IsSdlTimerActive(_SELF_ID, _TIMER_ID, _TMID_PTR)

/**---------------------------------------------------------------------------*
 ** SIGNAL MACRO: 
 **         SCI_GET_SIGCODE
 **         SCI_GET_SIGSENDER
 **         SCI_CREATE_SIGNAL
 **         SCI_SEND_SIGNAL
 **         SCI_RECEIVE_SIGNAL
 **         SCI_PEEK_SIGNAL
 **         SCI_FREE_SIGNAL
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Get signal code from the pointer to signal.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _SIG_PTR        Pointer to the signal
//  Return:         Signal code of the signal.
//	Note:
/*****************************************************************************/
/******************************************************************************
uint16  SCI_GET_SIGCODE(
    xSignalHeader   _SIG_PTR
    )
******************************************************************************/
#define SCI_GET_SIGCODE(_SIG_PTR) \
        (((xSignalHeader)(_SIG_PTR))->SignalCode)

/*****************************************************************************/
//  Description:    Get sender from the pointer to signal.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _SIG_PTR        Pointer to the signal
//  Return:         Sender of the signal.
//	Note:
/*****************************************************************************/
/******************************************************************************
BLOCK_ID    SCI_GET_SIGSENDER(
    xSignalHeader   _SIG_PTR
    )
******************************************************************************/
#define SCI_GET_SIGSENDER(_SIG_PTR) \
        (((xSignalHeader)(_SIG_PTR))->Sender)


/*****************************************************************************/
//  Description:    Create a signal.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _SIG_PTR        Pointer to the signal which will be created
//                  _SIG_CODE       Signal code.
//                  _SIG_SIZE       Number of bytes will be alloc for the signal
//                  _SENDER         Sender of this signal.
//  Return:         None.
//	Note:
/*****************************************************************************/
/******************************************************************************
void    SCI_CREATE_SIGNAL(
    xSignalHeader   _SIG_PTR, 
    uint16      _SIG_CODE, 
    uint16      _SIG_SIZE, 
    BLOCK_ID    _SENDER
    )
******************************************************************************/
#define SCI_CREATE_SIGNAL(_SIG_PTR, _SIG_CODE, _SIG_SIZE, _SENDER) \
		(_SIG_PTR) = (void*)SCI_ALLOC_BASE(_SIG_SIZE);\
		SCI_ASSERT((_SIG_PTR) != SCI_NULL); \
		SCI_MEMSET((void*)(_SIG_PTR), 0 , _SIG_SIZE); \
		((xSignalHeader)(_SIG_PTR))->SignalSize = _SIG_SIZE; \
		((xSignalHeader)(_SIG_PTR))->SignalCode = _SIG_CODE; \
		((xSignalHeader)(_SIG_PTR))->Sender     = _SENDER;

/*****************************************************************************/
//  Description:    Send a signal.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _SIG_PTR        Pointer to the signal which will be sent
//                  _RECEIVER       ID of thread whihc receives this signal.
//  Return:         None.
//	Note:
/*****************************************************************************/
/******************************************************************************
void SCI_SEND_SIGNAL(
    xSignalHeader   _SIG_PTR,     // Signal pointer to be sent
    BLOCK_ID        _RECEIVER     // Dest thread ID
    )
******************************************************************************/
#define SCI_SEND_SIGNAL(_SIG_PTR, _RECEIVER) \
        SCI_SendSignal((xSignalHeader)(_SIG_PTR), _RECEIVER);

/*****************************************************************************/
//  Description:    Receive a signal.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _SIG_PTR        Pointer to a signal to save received signal.
//                  _SELF_ID        ID of thread which receives this signal.
//  Return:         None.
//	Note:
/*****************************************************************************/
/******************************************************************************
SCI_RECEIVE_SIGNAL(
    xSignalHeader   _SIG_PTR,
    BLOCK_ID        _SELF_ID
    )
******************************************************************************/
#define SCI_RECEIVE_SIGNAL(_SIG_PTR, _SELF_ID) \
		_SIG_PTR = SCI_GetSignal(_SELF_ID); \
        SCI_LOG_INPUT_MSG(((xSignalHeader)(_SIG_PTR))->Sender, _SELF_ID, \
            SCI_INVALID_STATE, ((xSignalHeader)(_SIG_PTR))->SignalCode, \
            (uint8*)_SIG_PTR, ((xSignalHeader)(_SIG_PTR))->SignalSize);

/*****************************************************************************/
//  Description:    Gets a signal from a thread immediatelly, if no signal, 
//                  function returns immediatelly.
//	Global resource dependence: 
//  Author: Richard.Yang
//  Parameter:      _SIG_PTR        Pointer to a signal to save received signal.
//                  _SELF_ID        ID of thread which receives this signal.
//  Return:         None.
//	Note:
/*****************************************************************************/
/******************************************************************************
PUBLIC xSignalHeader SCI_PEEK_SIGNAL(   // Return received signal.
    xSignalHeader   _SIG_PTR,
    BLOCK_ID        _SELF_ID
    );
******************************************************************************/
#define SCI_PEEK_SIGNAL(_SIG_PTR, _SELF_ID) \
        _SIG_PTR = SCI_PeekSignal(_SELF_ID)
    

/*****************************************************************************/
//  Description:    Free a signal.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _SIG_PTR        Pointer to the signal which will be freed.
//  Return:         None   
//	Note:
/*****************************************************************************/
/******************************************************************************
void    SCI_FREE_SIGNAL(
    xSignalHeader   _SIG_PTR
    );
******************************************************************************/
#define SCI_FREE_SIGNAL(_SIG_PTR) \
        SCI_FREE(_SIG_PTR)
    
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif // End _SCI_API_H