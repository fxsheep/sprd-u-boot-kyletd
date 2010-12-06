/******************************************************************************
 ** File Name:      os_api.h                                                  *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           11/14/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic Application Interface (API)   *
 **                 to the high-performance RTOS.                             *
 **                 All service prototypes for user and some data structure   *
 **                 definitions are defined in this file.                     *
 **                 Basic data type definitions is contained in sci_types.h   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/14/2002     Xueliang.Wang    Create.                                   *
 ** 09/12/2003     Zhemin.Lin       Modify according to CR:MS00004678         *
 ** 10/11/2004     Richard.Yang     Add Trace interface and server interface  *
 ** 11/28/2005     Shujing.Dong     Modify according to CR:MS00035616         *
 ******************************************************************************/
#ifndef _OS_API_H
#define _OS_API_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
// c standard head file.
#ifndef KAL_ON_RTT
#include <linux/string.h>
#include <malloc.h>
//#include <stdlib.h>
//#include <stdio.h>
#endif

// Basic data types.
#include "sci_types.h"
#include "rtos_cfg.h"
#include "sci_service.h"



#ifdef WIN32    
    // Include this file to replace windows.h to avoid compile error
#ifndef KAL_ON_THREADX
    #include "windows_sci_os.h"
#endif
#endif


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
// Invalid state value.
#define SCI_INVALID_STATE           ~0
 
//---------------------------------------------
// API return values.
//---------------------------------------------
//Success, no error.
#define SCI_SUCCESS                 0x00
//Object was deleted. 
#define SCI_DELETED                 0x01
//Invalid pool pointer.
#define SCI_POOL_ERROR              0x02
//Invalid pointer.
#define SCI_PTR_ERROR               0x03
//A wait option other than TX_NO_WAIT was specified on call from a non-thread.
#define SCI_WAIT_ERROR              0x04
//Size is invalid.
#define SCI_SIZE_ERROR              0x05
//Invalid event group pointer.
#define SCI_GROUP_ERROR             0x06
//Service was unable to get the specified events.
#define SCI_NO_EVENTS               0x07
//Invalid option was specified.
#define SCI_OPTION_ERROR            0x08
//Invalid queue pointer.
#define SCI_QUEUE_ERROR             0x09
//Service was unable to retrieve a message because the queue was empty.
#define SCI_QUEUE_EMPTY             0x0A
//Service was unable to send message because the queue was full.
#define SCI_QUEUE_FULL              0x0B
//Invalid counting semaphore pointer.
#define SCI_SEMAPHORE_ERROR         0x0C
//Service was unable to retrieve an instance of the counting semaphore (semaphore count is zero).
#define SCI_NO_INSTANCE             0x0D
//Invalid thread control pointer.
#define SCI_THREAD_ERROR            0x0E
//Invalid thread priority, which is a value outside the range of 0-31.
#define SCI_PRIORITY_ERROR          0x0F
//Service was unable to allocate memory.
#define SCI_NO_MEMORY               0x10
//Specified thread is not in a terminated or completed state.
#define SCI_DELETE_ERROR            0x11
//Specified thread is not suspended.
#define SCI_RESUME_ERROR            0x12
//Invalid caller of this service.
#define SCI_CALLER_ERROR            0x13
//Specified thread is in a terminated or completed state.
#define SCI_SUSPEND_ERROR           0x14
//Invalid application timer pointer.
#define SCI_TIMER_ERROR             0x15
//Invalid value (a zero) supplied for initial ticks.
#define SCI_TICK_ERROR              0x16
//Timer was already active./Invalid activation selected.
#define SCI_ACTIVATE_ERROR          0x17
//Invalid preemption threshold specified. 
//This value must be a valid priority less than or equal to the initial priority of the thread.
#define SCI_THRESH_ERROR            0x18
//Previously set delayed suspension was lifted.
#define SCI_SUSPEND_LIFTED          0x19
//Suspension was aborted by another thread, timer, or ISR.
#define SCI_WAIT_ABORTED            0x1A
//Specified thread is not in a waiting state.
#define SCI_WAIT_ABORT_ERROR        0x1B
//Invalid mutex pointer.
#define SCI_MUTEX_ERROR             0x1C
//Service was unable to get ownership of the mutex.
#define SCI_NOT_AVAILABLE           0x1D
//Mutex is not owned by caller.
#define SCI_NOT_OWNED               0x1E
//Invalid priority inherit parameter.
#define SCI_INHERIT_ERROR           0x1F
//Invalid auto-start selection.
#define SCI_START_ERROR             0x20
//Parameter is invalid.
#define SCI_PARAM_ERROR             0x21
//Normal error.
#define SCI_ERROR                   0xFF

//---------------------------------------------
// API input parameters.
//---------------------------------------------
// Boolean value
#define SCI_TRUE                    TRUE       // Boolean true value
#define SCI_FALSE                   FALSE       // Boolean false value

// Wait option.
#define SCI_NO_WAIT                 0x0
#define SCI_WAIT_FOREVER            0xFFFFFFFF

// Option used to set/get event.
#define SCI_OR                      0
#define SCI_OR_CLEAR                1
#define SCI_AND                     2
#define SCI_AND_CLEAR               3

// Auto start option on thread
#define SCI_DONT_START              0
#define SCI_AUTO_START              1

// Indicates if the thread is preemptable.
#define SCI_NO_PREEMPT              0
#define SCI_PREEMPT                 1

// Auto start option on timer.
#define SCI_NO_ACTIVATE             0
#define SCI_AUTO_ACTIVATE           1

//@Zhemin.Lin, add, 09/12/2003, CR:MS00004678
//priority inherit mode for mutex
#define SCI_NO_INHERIT 0
#define SCI_INHERIT 1

// Thread priority definetion
#define SCI_PRIORITY_KERNEL         0 
#define SCI_PRIORITY_TIME_CRITICAL  5 
#define SCI_PRIORITY_HIGHEST        10
#define SCI_PRIORITY_ABOVE_NORMAL   15
#define SCI_PRIORITY_NORMAL         20
#define SCI_PRIORITY_BELOW_NORMAL   24
#define SCI_PRIORITY_LOWEST         28
#define SCI_PRIORITY_IDLE           31

//---------------------------------------------
// General constants.
//---------------------------------------------
// Max ID of static thread MUST be less than this value.
#define SCI_MAX_STATIC_THREAD_ID    100
// Number of all static threads should less than this value.
#define SCI_MAX_STATIC_THREAD_NUM   100

#define SCI_NULL                    0x0
#define SCI_INVALID_BLOCK_ID        0xFFFFFFFF

// Size of Queue item. Number of unsigned long int.
#define SCI_QUEUE_ITEM_SIZE         1

// Name size of thread, timer, queue, etc... 
#define SCI_MAX_NAME_SIZE           32

// RTOS system state.
#define SCI_RTOS_INITIALIZE_IN_PROGRESS     0
#define SCI_RTOS_INITIALIZE_COMPLETED       1


//define Heap Type 
#define   CONST_HEAP_MEMORY                       0x22222222UL 
#define   DYNAMIC_HEAP_BASE_MEMORY         0x33333333UL 
#define   DYNAMIC_HEAP_APP_MEMORY           0x44444444UL 


#define SCI_TRACE_NULL           SCI_TraceNull
#ifdef WIN32
	extern  void SCI_Trace(const char *, ...);
	#define SCI_TRACE_ERROR      SCI_Trace
	#define SCI_TRACE_HIGH       SCI_Trace
	#define SCI_TRACE_MID        SCI_Trace
	#define SCI_TRACE_LOW        SCI_Trace		
    #define SCI_TRACE_BUF        SCI_TraceBuf
	#define SCI_TRACE_DATA       SCI_TraceCapData
#else
	#define SCI_TRACE_ERROR      SCI_TraceError
	#define SCI_TRACE_HIGH       SCI_TraceHigh
	#define SCI_TRACE_DATA       SCI_TraceCapData
	
    #ifdef TRACE_INFO_SUPPORT 
        #define SCI_TRACE_BUF    SCI_TraceBuf
		#define SCI_TRACE_MID    SCI_TraceMid
		#define SCI_TRACE_LOW    SCI_TraceLow
	#else
        #define SCI_TRACE_BUF(...)
       	#define SCI_TRACE_MID(...)
       	#define SCI_TRACE_LOW(...)
    #endif
#endif  

//---------------------------------------------
// Type define.
//---------------------------------------------
typedef void    *SCI_EVENT_GROUP_PTR;
typedef void    *SCI_THREAD_PTR;
typedef void    *SCI_TIMER_PTR;

//Zhemin.Lin, add, 09/12/2003, CR:MS00004678,
typedef void    *SCI_MUTEX_PTR;
typedef void    *SCI_SEMAPHORE_PTR;

// The prototype of C funtion which execute when timer expires.
typedef void    (*TIMER_FUN)(uint32);

// The prototype of C function for thread execution.
typedef void    (*THREAD_FUN)(uint32, void*);

// Signal head structure.
// Signal vars used when send signals from one task to anther.
// The pointer is a structure whose head is SIGNAL_VARS.
#define _SIGNAL_VARS \
    uint16 SignalCode; \
    uint16 SignalSize; \
    xSignalHeader  Pre; \
    xSignalHeader  Suc; \
    BLOCK_ID       Sender;

#ifndef _BSSIM
    #define SIGNAL_VARS \
            _SIGNAL_VARS
#else
    #define SIGNAL_VARS \
            _SIGNAL_VARS \
            void  *SigP;
#endif // End of _BSSIM

// Signal head structure.
typedef struct xSignalHeaderStruct *xSignalHeader;
typedef struct xSignalHeaderStruct {
  SIGNAL_VARS
} xSignalHeaderRec;

// Memory debug head structure, used to identify the block of memory.
typedef struct MEM_HEADER_tag{
	struct MEM_HEADER_tag * pre;
	struct MEM_HEADER_tag * succ;
	char                  * file_name;
	int                   line;
    unsigned long         size;
    unsigned long         block_num;
} MEM_HEADER_T;    
#define MEM_HEADER_SIZE     sizeof(MEM_HEADER_T)

//@Daniel.Luo for pool optimization
typedef struct POOL_INFO_tag{
	uint16 pool_size;	// size of the pool, in byte
	uint16 pool_num;	// number of the pool
} POOL_INFO_T;


#define  MEM_POOL_DEBUG_MODE        0x0
#define  MEM_POOL_RELEASE_MODE     0x5245


/*================App Memory  Pool Defined==================*/
#define  MPOOL_HANDLE        uint32
typedef struct APP_MEM_CFG_Tag
{
	uint32  min_pool_size;
	uint32  pool_space_size;
	uint32  heap_mem_watermark;
	uint32  pool_mode;
}APP_MEM_CFG_T;


/*===========Shell Debug Used System Pool Information Defined=================*/
#define SYS_POOL_NAME_SIZE  31
typedef struct SYS_POOL_INFO_Tag
{
    uint8   name[SYS_POOL_NAME_SIZE + 1];
	uint32  mem_start_addr;
	uint32  mem_end_addr;
	uint32  total_num;
    uint32  availble_num;
	uint32  max_used_num;
}SYS_POOL_INFO_T;

#define SCI_SYS_POOL_TYPE  uint32
#define SCI_SYS_BLOCK_POOL  0x01
#define SCI_SYS_BYTE_POOL   0x02
#define SCI_SYS_POOL_MAX    0x03

#define POOL_INFO_SEEK_FIRST     0x00
#define POOL_INFO_SEEK_NEXT      0x01
#define SCI_POOL_INFO_END        0x00454E44UL
/*===========Shell Debug Used System Pool Information Defined End=================*/


/**---------------------------------------------------------------------------*
 ** MACROES:
 **---------------------------------------------------------------------------*/
#ifdef _RTOS
    #ifdef _WIN_LOG_TOOL
        #define SCI_ALLOC_CONST(_SIZE)    malloc(_SIZE)
        #define SCI_ALLOC_BASE(_SIZE)    malloc(_SIZE)
        #define SCI_ALLOC_APP(_SIZE)    malloc(_SIZE)
        #define SCI_MPALLOC(_SIZE)    malloc(_SIZE)
        #define SCI_ALLOC(_SIZE)    malloc(_SIZE)
        #define SCI_ALLOCA(_SIZE)    malloc(_SIZE)        
        #define SCI_MPFREE(_MEM_PTR)  free(_MEM_PTR);     (_MEM_PTR) = PNULL;
        #define SCI_FREE(_MEM_PTR)  free(_MEM_PTR);     (_MEM_PTR) = PNULL;
        #define SCI_ASSERT(_EXP)    assert(_EXP)
        #define SCI_PASSERT(_EXP,PRINT_STR)     assert( _EXP );
        #define SCI_DASSERT(_EXP)    assert(_EXP)
    #else
    	#ifdef WIN32
    	   #ifdef UEIT_MOCOR
            #define SCI_ALLOC_CONST(_SIZE)    SCI_Malloc(_SIZE, __FILE__, __LINE__)
            #define SCI_ALLOC_BASE(_SIZE)    SCI_Malloc(_SIZE, __FILE__, __LINE__)
            #define SCI_ALLOC_APP(_SIZE)    SCI_Malloc(_SIZE, __FILE__, __LINE__)             
	        #define SCI_ALLOC(_SIZE)    SCI_Malloc(_SIZE, __FILE__, __LINE__)
	        #define SCI_ALLOCA(_SIZE)    SCI_MallocApp(_SIZE, __FILE__, __LINE__)	        
	        #define SCI_FREE(_MEM_PTR)  SCI_Free(_MEM_PTR); (_MEM_PTR) = PNULL;
	        #define SCI_ASSERT(_EXP)    (_EXP) ? (void)0 : SCI_Assert(#_EXP, __FILE__, __LINE__)
	        #define SCI_PASSERT(_EXP,PRINT_STR)     (_EXP) ? (void)0 : (void )SCI_PAssert(#_EXP, __FILE__, __LINE__, SCI_SaveAssertInfo##PRINT_STR)
	        #define SCI_DASSERT(_EXP)    (_EXP) ? (void)0 : SCI_DAssert(#_EXP, __FILE__, __LINE__)   	       	   
    	   #else
            #define SCI_ALLOC_CONST(_SIZE)    SCI_MallocEx(_SIZE, CONST_HEAP_MEMORY, __FILE__, __LINE__)
            #define SCI_ALLOC_BASE(_SIZE)    SCI_MallocEx(_SIZE, DYNAMIC_HEAP_BASE_MEMORY, __FILE__, __LINE__)
            #define SCI_ALLOC_APP(_SIZE)    SCI_MallocEx(_SIZE, DYNAMIC_HEAP_APP_MEMORY, __FILE__, __LINE__)      	        
            #define SCI_MPALLOC(_SIZE, _HANDLE_)    SCI_MemPoolMalloc(_SIZE, _HANDLE_, __FILE__, __LINE__)
            #define SCI_ALLOC(_SIZE)    SCI_Malloc(_SIZE, __FILE__, __LINE__)
            #define SCI_ALLOCA(_SIZE)    SCI_MallocApp(_SIZE, __FILE__, __LINE__)	        
            #define SCI_MPFREE(_MEM_PTR)  SCI_MemPoolFree(_MEM_PTR); (_MEM_PTR) = PNULL;
            #define SCI_FREE(_MEM_PTR)  SCI_Free(_MEM_PTR); (_MEM_PTR) = PNULL;
            #define SCI_ASSERT(_EXP)    (_EXP) ? (void)0 : SCI_Assert(#_EXP, __FILE__, __LINE__)
            #define SCI_PASSERT(_EXP,PRINT_STR)     (_EXP) ? (void)0 : (void )SCI_PAssert(#_EXP, __FILE__, __LINE__, SCI_SaveAssertInfo##PRINT_STR)
            #define SCI_DASSERT(_EXP)    (_EXP) ? (void)0 : SCI_DAssert(#_EXP, __FILE__, __LINE__)
           #endif 
	#else
            #define SCI_ALLOC_CONST(_SIZE)    SCI_MallocEx(_SIZE, CONST_HEAP_MEMORY, __MODULE__, __LINE__)
            #define SCI_ALLOC_BASE(_SIZE)    SCI_MallocEx(_SIZE, DYNAMIC_HEAP_BASE_MEMORY, __MODULE__, __LINE__)
            #define SCI_ALLOC_APP(_SIZE)    SCI_MallocEx(_SIZE, DYNAMIC_HEAP_APP_MEMORY, __MODULE__, __LINE__)      	        	    
            #define SCI_ALLOC(_SIZE)    SCI_Malloc(_SIZE, __MODULE__, __LINE__)
            #define SCI_MPALLOC(_SIZE, _HANDLE_)    SCI_MemPoolMalloc(_SIZE, _HANDLE_, __MODULE__, __LINE__)
            #define SCI_ALLOCA(_SIZE)    SCI_MallocApp(_SIZE, __MODULE__, __LINE__)	        
            #define SCI_MPFREE(_MEM_PTR)  SCI_MemPoolFree(_MEM_PTR); (_MEM_PTR) = PNULL;
            #define SCI_FREE(_MEM_PTR)  SCI_Free(_MEM_PTR); (_MEM_PTR) = PNULL;
	        
            #ifdef LOW_MEMORY_SUPPORT
            #define SCI_ASSERT(_EXP)    (_EXP) ? (void)0 : SCI_Assert("", __MODULE__, __LINE__)
            #define SCI_PASSERT(_EXP,PRINT_STR)     (_EXP) ? (void)0 : (void )SCI_PAssert("", __MODULE__, __LINE__, SCI_SaveAssertInfo##PRINT_STR)
            #define SCI_DASSERT(_EXP)    (_EXP) ? (void)0 : SCI_DAssert("", __MODULE__, __LINE__)
            #else
            #define SCI_ASSERT(_EXP)    (_EXP) ? (void)0 : SCI_Assert(#_EXP, __MODULE__, __LINE__)
            #define SCI_PASSERT(_EXP,PRINT_STR)     (_EXP) ? (void)0 : (void )SCI_PAssert(#_EXP, __MODULE__, __LINE__, SCI_SaveAssertInfo##PRINT_STR)
            #define SCI_DASSERT(_EXP)    (_EXP) ? (void)0 : SCI_DAssert(#_EXP, __MODULE__, __LINE__)
            #endif
	#endif
    #endif  // End of _WIN_LOG_TOOL
#else 

    #define SCI_ALLOC_CONST(_SIZE)    
    #define SCI_ALLOC_BASE(_SIZE)    
    #define SCI_ALLOC_APP(_SIZE)    
    #define SCI_ALLOC(_SIZE)    
    #define SCI_ALLOCA(_SIZE)    
    #define SCI_FREE(_MEM_PTR)  
    #define SCI_ASSERT(_EXP)    
	#define SCI_PASSERT(_EXP,PRINT_STR) 
	#define SCI_DASSERT(_EXP)    

    #define SCI_MPALLOC(_SIZE)    malloc(_SIZE)    
    #define SCI_MPFREE(_MEM_PTR)  free(_MEM_PTR);     (_MEM_PTR) = PNULL;
#endif  // End of _RTOS

#ifdef LOW_MEMORY_SUPPORT
    #define SCI_MEMCPY(_DEST_PTR, _SRC_PTR, _SIZE) \
        do  \
        { \
            if ((_SIZE)>0) { memcpy((_DEST_PTR), (_SRC_PTR), (_SIZE));} \
        }while(0);

    #define SCI_MEMSET(_DEST_PTR, _VALUE, _SIZE) \
        do\
        {\
            if ((_SIZE)>0) { memset((_DEST_PTR), (_VALUE), (_SIZE));  }	\
        }while(0);
#else
    #define SCI_MEMCPY(_DEST_PTR, _SRC_PTR, _SIZE) \
        do  \
        { \
            SCI_ASSERT(PNULL != (_DEST_PTR)); \
            if ((_SIZE)>0) { memcpy((_DEST_PTR), (_SRC_PTR), (_SIZE));} \
        }while(0);

    #define SCI_MEMSET(_DEST_PTR, _VALUE, _SIZE) \
        do\
        {\
            SCI_ASSERT(PNULL != (_DEST_PTR)); \
            if ((_SIZE)>0) { memset((_DEST_PTR), (_VALUE), (_SIZE));  }	\
        }while(0);
#endif

#define SCI_MEM16CPY(_DEST_PTR, _SRC_PTR, _SIZE_IN_BYTES) \
        SCI_Mem16Cpy((_DEST_PTR), (_SRC_PTR), (_SIZE_IN_BYTES))
        
#define SCI_MEM16SET(_DEST_PTR, _VALUE, _SIZE_IN_BYTES) \
        SCI_Mem16Set((_DEST_PTR), (_VALUE), (_SIZE_IN_BYTES))


#define  SCI_GetMaxConstHeapAvalidSpace()      SCI_GetByteHeapAvailableSpace(CONST_HEAP_MEMORY)   
#define  SCI_GetMaxBaseHeapAvalidSpace()       SCI_GetByteHeapAvailableSpace(DYNAMIC_HEAP_BASE_MEMORY)   
#define  SCI_GetMaxAppHeapAvalidSpace()         SCI_GetByteHeapAvailableSpace(DYNAMIC_HEAP_APP_MEMORY)   
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 ** OS Kernel 
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function initialize RTOS kernel.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_InitRtos(void);

/*****************************************************************************/
//  Description:    The function is to define the user's initialization 
//                  function. User implement this function.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_ApplicationDefine(void *first_unused_memory);

/*****************************************************************************/
//  Description:    The function is used to enter RTOS kernel.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_KernelEnter(void);

/*****************************************************************************/
//  Description:    The function is used to get RTOS ststem state.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_GetRtosState(void);
 
/**---------------------------------------------------------------------------*
 ** ASSERT
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function is typically used to identify logic errors 
//                  during program development, by implementing the expression 
//                  argumnet to evaluate to SCI_FALSE only when the program 
//                  is operating incorrectly. 
//                  After debugging is complete, assertion checking can be 
//                  turned off without modifying the source file by deleting 
//                  the predifinition _DEBUG.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_Assert(
    void    *exp,     // Expression (including pointers) that evaluates 
                      // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line      // Line number where assert
    );

/*****************************************************************************/
//  Description:    The SCI_PAssert function supports printing message and 
//                  variables when asserting
//  Author:		Bin.Lu
//	Note:
/*****************************************************************************/
PUBLIC void SCI_PAssert(
    void    *exp,             // Expression (including pointers) that evaluates 
                              // to nonzero or 0
    void    *file,            // File name where assert
    uint32  line,             // Line number where assert
	char    *assert_info_ptr  // Assert information string
);

/*****************************************************************************/
//  Description:    The SCI_DAssert function will provide debug MMI regardless of release mode
//                  Called by diag task receive assert cmd and other uplayer artificial assert
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_DAssert(
    void    *exp,     // Expression (including pointers) that evaluates 
                      // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line      // Line number where assert
);
    
/*****************************************************************************/
//  Description:    The function is typically used to save the input parameter 
//                  information as a string. Used by SCI_PAssert
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           This function is reserved.
/*****************************************************************************/
PUBLIC char * SCI_SaveAssertInfo(
    const char *x_format, 
	...
	);

/*****************************************************************************/
//  Description:    The SCI_SetAssertMode function used to set assert mode.
//					If b_debug_mode is SCI_TURE, it prints assert information
//					when assert; else reset.
//  Author:			Xueliang.Wang
//	Note:			
/*****************************************************************************/
PUBLIC void SCI_SetAssertMode(
	BOOLEAN	b_debug_mode
	);
	
/**---------------------------------------------------------------------------*
 ** EVENT
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function creates a group of 32 event flags.  
//                  All 32 event flags in the group are initialized to zero.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC SCI_EVENT_GROUP_PTR SCI_CreateEvent( // If create successfully, 
                            // return pointer to the control block of created
                            // event; else return SCI_NULL.
    const char *name_ptr    // Pointer to the name of the event flags group.
    );
    
/*****************************************************************************/
//  Description:    This function sets the specified flags in the event group
//                  based on the set option specified.  All threads suspended
//                  on the group whose get request can now be satisfied are 
//                  resumed.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_SetEvent(     // If set successfully, return SCI_SUCCESS;
                                // else return SCI_ERROR
    SCI_EVENT_GROUP_PTR group_ptr,  // Pointer to an event flags group control 
                                    // block. 
    uint32      flags_to_set,   // Specifies the event flags to set or      
                                // clear based upon the set option selected.
    uint32      set_option      // Specifies whether the event flags        
                                // specified are ANDed or ORed into the     
                                // current event flags of the group.        
                                // The following are valid selections:      
                                // SCI_AND (0x02)                           
                                // SCI_OR (0x00)                            
                                // Selecting SCI_AND specifies that the     
                                // specified event flags are ANDed into the 
                                // current event flags in the group. This   
                                // option is often used to clear event flags
                                // in a group. Otherwise, if SCI_OR is      
                                // specified, the specified event flags are 
                                // ORed with the current event in the group.
    );

/*****************************************************************************/
//  Description:    This function gets the specified event flags from the 
//                  group, acording to the get option.  The get option also 
//                  specifies whether or not the retrieved flags are cleared. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_GetEvent(     // If get it successfully, return SCI_SUCCESS; 
                                // else return SCI_ERROR                
    SCI_EVENT_GROUP_PTR group_ptr,  // Pointer to an event flags group control
                                    // block. 
    uint32      requested_flags,    // 32-bit unsigned variable that represents   
                                    // the requested event flags.                 
    uint32      get_option,     // Specifies whether all or any of the        
                                // requested event flagsare required. The     
                                // following are valid selections:            
                                // SCI_AND (0x02)                             
                                // SCI_AND_CLEAR (0x03                        
                                // SCI_OR (0x00)                              
                                // SCI_OR_CLEAR (0x01)                        
                                // Selecting SCI_AND or SCI_AND_CLEAR         
                                // specifies that all event flags must be     
                                // present in the group. Selecting SCI_OR or  
                                // SCI_OR_CLEAR specifies that any event flag
                                // is satisfactory. Event flags that satisfy  
                                // the request are cleared (set to zero) if   
                                // SCI_AND_CLEAR or SCI_OR_CLEAR are          
                                // specified.                                 
    uint32      *actual_flags_ptr,  // Pointer to destination of where the
                                    // retrieved event flags are placed. Note 
                                    // that the actual flags obtained may
                                    // contain flags that were not requested.
    uint32      wait_option     // Defines how the service behaves if the
                                // selected event flags are not set. The
                                // wait options are defined as follows:
                                // SCI_NO_WAIT (0x00000000)
                                // SCI_WAIT_FOREVER (0xFFFFFFFF)
                                // timeout value (0x00000001 - 0xFFFFFFFE)
    );

/*****************************************************************************/
//  Description:    The function deletes a group of 32 event flags. 
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_DeleteEvent(  // If delete successfully, return SCI_SUCCESS;
                                // else return SCI_ERROR
    SCI_EVENT_GROUP_PTR group_ptr	// Pointer to a previously created event 
                                    // flags group. 
    );
    
    
/**---------------------------------------------------------------------------*
 ** INTERRUPT
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function is used to disable IRQ and save old status to
//                  stack. 
//	Global resource dependence: 
//  Author:         lin.liu
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_DisableIRQ(void);

/*****************************************************************************/
//  Description:    The function is used to restore the old IRQ status. 
//	Global resource dependence: 
//  Author:         lin.liu
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_RestoreIRQ(void);

/*****************************************************************************/
//  Description:    The function is used to disable FIQ and save old status to
//                  stack. 
//	Global resource dependence: 
//  Author:         lin.liu
//	Note:           This opertion must be in SCI_DisableIRQ-SCI_RestoreIRQ
/*****************************************************************************/

PUBLIC void SCI_DisableFIQ(void);

/*****************************************************************************/
//  Description:    The function is used to restore FIQ and save old status to
//                  stack. 
//	Global resource dependence: 
//  Author:         lin.liu
//	Note:           This opertion must be in SCI_DisableIRQ-SCI_RestoreIRQ
/*****************************************************************************/

PUBLIC void SCI_RestoreFIQ(void);



/**---------------------------------------------------------------------------*
 ** MEMORY MANAGE
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The SCI_Malloc function is used to Allocates memory blocks
//                  in embedded system. It allocates memory from different 
//                  memory pool heap according to the size applications needed.
//                  This can avoid memory fragments and speed up memory 
//                  allocating, the shortcoming is that we might waste some 
//                  memory.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           Don't use it directly. Please use SCI_ALLOC
/*****************************************************************************/
PUBLIC void * SCI_Malloc(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size,        // Bytes to allocate.
    const char * file,  // The name of the file which call this function.
    uint32 line         // The line number in the file which call this function.
    );


PUBLIC void * SCI_MallocEx(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size,        // Bytes to allocate.
    uint32 type, 
    const char * file,  // The name of the file which call this function.
    uint32 line         // The line number in the file which call this function.
    );



 

/*****************************************************************************/
//  Description:    The SCI_Free function is used to Deallocates or frees 
//                  a memory block.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           Don't use it directly. Please use SCI_FREE
/*****************************************************************************/
PUBLIC uint32 SCI_Free(     // SCI_SUCCESS or SCI_ERROR
    void *memory_ptr        // Pointer to memory block to be deleted.
    );
 
/*****************************************************************************/
//  Description:    The SCI_Malloc function is used to Allocates memory blocks
//                  in embedded system. It allocates memory from different 
//                  memory pool heap according to the size applications needed.
//                  This can avoid memory fragments and speed up memory 
//                  allocating, the shortcoming is that we might waste some 
//                  memory.
//					This function will return SCI_NULL if no memory fits need.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           Don't use it directly. Please use SCI_ALLOCA
/*****************************************************************************/
PUBLIC void * SCI_MallocApp(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size,        // Bytes to allocate.
    const char * file,  // The name of the file which call this function.
    uint32 line         // The line number in the file which call this function.
    );

/**---------------------------------------------------------------------------*
 ** MESSAGE
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function send signal to proper thread.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_SendSignal(   // If send successfully, return SCI_SUCCESS,
                                // else return SCI_ERROR
    xSignalHeader   signal_ptr, // Signal pointer to be sended
    BLOCK_ID        dest_id     // Dest thread ID
    );

/*****************************************************************************/
//  Description:    The function send signal to specified thread, and put the 
//                  signal to the header of queue.
//                  This function return immediately regardless of whether or
//                  not it was successful.
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_SendSignalFront(   // If send successfully, return SCI_SUCCESS,
                                // else return SCI_ERROR
    xSignalHeader   signal_ptr, // Signal pointer to be sended
    BLOCK_ID        dest_id     // Dest thread ID
    );


/*****************************************************************************/
//  Description:    The function gets a signal from a thread.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC xSignalHeader SCI_GetSignal(     // Return received signal.
    BLOCK_ID thread_id                  // ID of the thread which receives signal
    );    

/*****************************************************************************/
//  Description:    This function gets a signal from a thread immediatelly, 
//                  if no signal, function returns immediatelly.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC xSignalHeader SCI_PeekSignal(    // Return received signal.
                                        // If no signal, return SCI_NULL.
    BLOCK_ID thread_id                  // ID of the thread which receives signal
    );

/**---------------------------------------------------------------------------*
 ** THREAD
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function creates a dynamic thread.
//                  The control block, stack and queue used in the thread to 
//                  be created are alloced in this function.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC BLOCK_ID SCI_CreateBaseThread(       // If successful, returns the thread ID
                                        // of the new created thread, 
                                        // else returns SCI_INVALID_BLOCK_ID
    const char  *thread_name,           // Name string of the thread
    const char  *queue_name,            // Queue name string of the thread
    void    (*entry)(uint32, void*),    // Entry function of the thread
    uint32  argc,                       // First parameter for entry function,
    void    *argv,                      // Second parameter for entry function,
    uint32  stack_size,                 // Size of the thread stack in bytes
    uint32  queue_num,          // Number of messages which can be enqueued
    uint32  priority,           // Prority of the thread.
    uint32  preempt,            // Indicates if the thread is preemptable.
    uint32  auto_start          // Specifies whether the thread starts 
                                // immediately or stays in a pure suspended
                                // state. Legal options are SCI_AUTO_START
                                // and SCI_DONT_START.
    );

/**---------------------------------------------------------------------------*
 ** THREAD
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function creates a dynamic thread.
//                  The control block, stack and queue used in the thread to 
//                  be created are alloced in this function.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC BLOCK_ID SCI_CreateAppThread(       // If successful, returns the thread ID
                                        // of the new created thread, 
                                        // else returns SCI_INVALID_BLOCK_ID
    const char  *thread_name,           // Name string of the thread
    const char  *queue_name,            // Queue name string of the thread
    void    (*entry)(uint32, void*),    // Entry function of the thread
    uint32  argc,                       // First parameter for entry function,
    void    *argv,                      // Second parameter for entry function,
    uint32  stack_size,                 // Size of the thread stack in bytes
    uint32  queue_num,          // Number of messages which can be enqueued
    uint32  priority,           // Prority of the thread.
    uint32  preempt,            // Indicates if the thread is preemptable.
    uint32  auto_start          // Specifies whether the thread starts 
                                // immediately or stays in a pure suspended
                                // state. Legal options are SCI_AUTO_START
                                // and SCI_DONT_START.
    );

/**---------------------------------------------------------------------------*
 ** THREAD
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function creates a dynamic thread.
//                  The control block, stack and queue used in the thread to 
//                  be created are alloced in this function.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC BLOCK_ID SCI_CreateThread(       // If successful, returns the thread ID
                                        // of the new created thread, 
                                        // else returns SCI_INVALID_BLOCK_ID
    const char  *thread_name,           // Name string of the thread
    const char  *queue_name,            // Queue name string of the thread
    void    (*entry)(uint32, void*),    // Entry function of the thread
    uint32  argc,                       // First parameter for entry function,
    void    *argv,                      // Second parameter for entry function,
    uint32  stack_size,                 // Size of the thread stack in bytes
    uint32  queue_num,          // Number of messages which can be enqueued
    uint32  priority,           // Prority of the thread.
    uint32  preempt,            // Indicates if the thread is preemptable.
    uint32  auto_start          // Specifies whether the thread starts 
                                // immediately or stays in a pure suspended
                                // state. Legal options are SCI_AUTO_START
                                // and SCI_DONT_START.
    );

/*****************************************************************************/
//  Description:    The function creates a static thread to execute. 
//                  The stack of the thread should be alloced before call 
//                  this function.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_CreateStaticThread(   // If create successful, returns 
                                        // SCI_SUCCESS, else return SCI_ERROR.
    BLOCK_ID    thread_id,              // Static thread id, it must be less 
                                        // than SCI_MAX_STATIC_THREAD_ID.
    const char  *thread_name,           // Name string of the thread
    void    (*entry)(uint32, void*),    // Entry function of the thread
    uint32  argc,                       // First parameter for entry function,
    void    *argv,                      // Second parameter for entry function,
    void    *stack_start,       // Start pointer of the thread stack
    uint32  stack_size,         // Size of the thread stack in bytes
    uint32  priority,           // Prority of the thread
    uint32  preempt,            // Indicates if the thread is preemptable.
    const char  *queue_name,    // Name of the queue
    void    *queue_start,       // Start pointer of the queue.
    uint32  queue_num,          // Number of messages which can be enqueued.
    uint32  auto_start          // If auto start the thread after creation
    );
    
/*****************************************************************************/
//  Description:    The function deletes a thread created before.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_DeleteThread( // If delete successful,return SCI_SUCCESS;
                                // else return SCI_ERROR.
    BLOCK_ID    thread_id       // ID of the thread to be deleted.
    );
    
/*****************************************************************************/
//  Description:    The function exit current thread.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_ThreadExit(void);

/*****************************************************************************/
//  Description:    The function gets the information of a specified thread.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_GetThreadInfo(    // If get the information successful,  
                                    // return SCI_SUCCESS; else return SCI_ERROR.
    BLOCK_ID    thread_id,	        // ID of the thread
    char        *thread_name,       // Pointer to an SCI_MAX_NAME_SIZE character
                                    // destination area for the thread¡¯s name.
    char        *queue_name,        // Pointer to an SCI_MAX_NAME_SIZE character
                                    // destination area for the queue¡¯s name.
    uint32      *state,             // state of the thread.
    uint32      *priority,          // Priority of the thread.
    uint32      *preempt            // Indicates if the thread is preemptable.
    );
    
/*****************************************************************************/
//  Description:    The function returns the current inused thread ID. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC BLOCK_ID SCI_IdentifyThread(void);   // If the thread is created by 
                                            // user, return thread id; else 
                                            // return SCI_INVALID_BLOCK_ID.


/*****************************************************************************/
//  Description:    The function returns the specify thread exist, or not. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           If thread exist, return SCI_SUCCESS, or return SCI_ERROR
/*****************************************************************************/
PUBLIC uint32 SCI_IsThreadExist(BLOCK_ID  thread_id);   



/*****************************************************************************/
//  Description:    The function returns the specify thread queue aviable, or not. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           If thread queue avilable, return SCI_SUCCESS, or return 
//                  SCI_ERROR/SCI_QUEUE_FULL
/*****************************************************************************/
PUBLIC uint32 SCI_IsThreadQueueAvilable(BLOCK_ID  thread_id); 



/*****************************************************************************/
//  Description:    This function changes the priority of the specified thread. 
//                  Valid priorities range from 0 through 31, where 0 
//                  represents the highest priority level.
//	Global resource dependence: 
//                  The thread should be a previously created thread.
//  Author:         Benjamin.Wang
//	Note:           The preemption threshold of the specified thread is 
//                  automatically set to the new priority. 
/*****************************************************************************/
PUBLIC uint32 SCI_ChangeThreadPriority( // TX_SUCCESS (0x00) Successful 
                                        //  priority change.
                                        // TX_THREAD_ERROR (0x0E) Invalid 
                                        //  application thread pointer.
                                        // TX_PRIORITY_ERROR (0x0F) Specified 
                                        //  new priority is not valid (a value 
                                        //  other than 0-31).
                                        // TX_PTR_ERROR (0x03) Invalid pointer 
                                        //  to previous priority storage location.
                                        // TX_CALLER_ERROR (0x13) Invalid caller 
                                        //  of this service.
    BLOCK_ID    thread_id,              // ID of the thread to be deleted., 
    uint32      new_priority,           // New thread priority level (0-31).
    uint32*     old_priority_ptr        // Pointer to a location to return the 
                                        // thread¡¯s previous priority.
    );
    
/*****************************************************************************/
//  Description:    This function moves the currently executing thread to the 
//                  end of the list of threads ready at the same priority. 
//                  If no other threads of the same or higher priority are 
//                  ready, this function simply returns.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_RelinquishThread(void);

/*****************************************************************************/
//  Description:    The function resumes a specified thread. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_ResumeThread( // If resume successful, return SCI_SUCCESS;
                                // else return SCI_ERROR.
    BLOCK_ID    thread_id       // ID of the thread to be resumed.
    );

/*****************************************************************************/
//  Description:    The function suspends a specified thread. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_SuspendThread(    // If suspend successful, return 
                                    // SCI_SUCCESS; else return SCI_ERROR.
    BLOCK_ID    thread_id           // ID of the thread to be suspended.
    );

/*****************************************************************************/
//  Description:    The function terminates a specified thread.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_TerminateThread(  // If terminate successful, return 
                                    // SCI_SUCCESS; else return SCI_ERROR.
    BLOCK_ID    thread_id           // ID of the thread to be terminated.
    );

/*****************************************************************************/
//  Description:    Suspends the execution of the current thread for a 
//                  specified interval. 
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_Sleep(
    uint32  time_in_ms      // Number of milliseconds to sleep.
    );

/*****************************************************************************/
//  Description:    The SCI_SetThreadEvent function sets or clears event flags 
//                  in an event flag group of the thread, depending upon the 
//                  specified set-option. All suspended threads whose event 
//                  flag request is now satisfied are resumed.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_SetThreadEvent(   // If set it successfully, return 
                                    // SCI_SUCCESS; else return SCI_FAILED             
    BLOCK_ID    thread_id,          // ID fo thread which event to be set      
    uint32      flags_to_set,       // Specifies the event flags to set or      
                                    // clear based upon the set option selected.
    uint32      set_option          // Specifies whether the event flags        
                                    // specified are ANDed or ORed into the     
                                    // current event flags of the group.        
                                    // The following are valid selections:      
                                    // SCI_AND (0x02)                           
                                    // SCI_OR (0x00)                            
                                    // Selecting SCI_AND specifies that the     
                                    // specified event flags are ANDed into the 
                                    // current event flags in the group. This   
                                    // option is often used to clear event flags
                                    // in a group. Otherwise, if SCI_OR is      
                                    // specified, the specified event flags are 
                                    // ORed with the current event in the group.
    );
    
/*****************************************************************************/
//  Description:    The function SCI_GetThreadEvent retrieves event flags from
//                  the specified event flag group of the thread. Each event  
//                  flag group contains 32 event flags. Each flag is 
//                  represented by a single bit. This service can retrieve a 
//                  variety of event flag combinations, as selected by the 
//                  input parameters.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_GetThreadEvent(   // If get it successfully, return 
                                    // SCI_SUCCESS; else return SCI_FAILED                
    BLOCK_ID    thread_id,          // ID fo thread which event to be get        
    uint32      requested_flags,    // 32-bit unsigned variable that represents   
                                    // the requested event flags.                 
    uint32      get_option,     // Specifies whether all or any of the        
                                // requested event flagsare required. The     
                                // following are valid selections:            
                                // SCI_AND (0x02)                             
                                // SCI_AND_CLEAR (0x03                        
                                // SCI_OR (0x00)                              
                                // SCI_OR_CLEAR (0x01)                        
                                // Selecting SCI_AND or SCI_AND_CLEAR         
                                // specifies that all event flags must be     
                                // present in the group. Selecting SCI_OR or  
                                // SCI_OR_CLEAR specifies that any event flag
                                // is satisfactory. Event flags that satisfy  
                                // the request are cleared (set to zero) if   
                                // SCI_AND_CLEAR or SCI_OR_CLEAR are          
                                // specified.                                 
    uint32      *actual_flags_ptr,  // Pointer to destination of where the
                                    // retrieved event flags are placed. Note 
                                    // that the actual flags obtained may
                                    // contain flags that were not requested.
    uint32      wait_option     // Defines how the service behaves if the
                                // selected event flags are not set. The
                                // wait options are defined as follows:
                                // SCI_NO_WAIT (0x00000000)
                                // SCI_WAIT_FOREVER (0xFFFFFFFF)
                                // timeout value (0x00000001 - 0xFFFFFFFE)
    );

/*****************************************************************************/
//  Description:    The function gets the last error code and error information 
//                  of current thread.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_GetLastError( // Return last error code of the thread.
    char ** err_str             // Pointer to destination for the pointer to 
                                // the error information string.
    );

/*****************************************************************************/
//  Description:    The function get the SaveQ of the thread.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC xSignalHeader SCI_GetSaveQ(  // If the SaveQ is null, return SCI_NULL; 
                                    // else return the first signal pointer 
                                    // in the SaveQ.
    BLOCK_ID    thread_id           // ID fo thread which SaveQ to be getten.
    );

/*****************************************************************************/
//  Description:    Add the signal to the SaveQ of the thread.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_AddToSaveQ(   // If add it successfully, return the 
                                // SCI_SUCCESS, else return SCI_ERROR.
    BLOCK_ID        thread_id,  // ID fo thread.
    xSignalHeader   sig_ptr     // Pointer to the signal to be added.
    );

/*****************************************************************************/
//  Description:    Deleted the signal from the SaveQ of the thread.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_DelFromSaveQ( // If delete successfully, return SCI_SUCCESS;
                                // else return SCI_ERROR.
    BLOCK_ID        thread_id,  // ID fo thread.
    xSignalHeader   sig_ptr     // Pointer to the signal to be deleted.
    );

/*****************************************************************************/
//  Description:    The function clear the SaveQ of the thread.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_ClrSaveQ(     // If clear successfully, return the 
                                // SCI_SUCCESS; else return SCI_ERROR.
    BLOCK_ID    thread_id       // ID fo thread which SaveQ to be getten.
    );

/*****************************************************************************/
//  Description:    Get the next signal of the specified signal.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC xSignalHeader SCI_GetNextSig(    // Return the next signal.
    xSignalHeader   sig_ptr             // Pointer to the specified signal.
    );

   
/**---------------------------------------------------------------------------*
 ** TIMER 
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function create a timer with call back function. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC SCI_TIMER_PTR SCI_CreateTimer(   // If successful, returns pointer to
                                        // the control block of the timer,
                                        // else return SCI_NULL.
    const char  *timer_name,    // Name string of the timer
    TIMER_FUN   timer_fun,      // Timer callback function
    uint32      input,          // Input value for timer callback function 
    uint32      timer_expire,   // Specifies the timer expire value in     
                                // milliseconds.                           
    uint32      auto_activate   // Option to check if auto active the timer
                                // after create.    
    );

/*****************************************************************************/
//  Description:    The function create a period timer with call back function.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC SCI_TIMER_PTR SCI_CreatePeriodTimer( // If successful, returns pointer to
                                            // the control block of the timer,
                                            // else return SCI_NULL.
    const char  *timer_name,    // Name string of the timer
    TIMER_FUN   timer_fun,      // Timer callback function
    uint32      input,          // Input value for timer callback function 
    uint32      timer_expire,   // Specifies the timer expire value in
                                // milliseconds. 
    uint32      auto_activate   // Option to check if auto active the timer
                                // after create.
    );

/*****************************************************************************/
//  Description:    The function changed timer's callback function and 
//                  expire time.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_ChangeTimer(  // If successful, returns SCI_SUCCESS,
                                // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr,    // Timer control block
    TIMER_FUN   timer_fun,      // Timer callback function
    uint32      timer_expire    // Specifies the expire value in milliseconds. 
    );

/*****************************************************************************/
//  Description:    The function activate a timer created before.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_ActiveTimer(  // If active successful, returns SCI_SUCCESS,
                                // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr     // Pointer to a previously created application
                                // timer.
    );
    
/*****************************************************************************/
//  Description:    The function deactive a timer created before.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_DeactiveTimer(    // If successful, returns SCI_SUCCESS,
                                    // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr         // Pointer to a previously created
                                    // application timer.
    );

/*****************************************************************************/
//  Description:    The function checks if the timer is still active.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsTimerActive(   // If it is active, returns SCI_TRUE,
                                    // else return SCI_FALSE 
    SCI_TIMER_PTR timer_ptr         // Pointer to a previously created 
                                    // application timer.
    );
        
/*****************************************************************************/
//  Description:    The function deletes a timer created before.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_DeleteTimer(  // If successful, returns SCI_SUCCESS,
                                // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr     // Pointer to a previously created application
                                // timer.
    );

/*****************************************************************************/
//  Description:    The function exit the specified timer.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_TimerExit(
    SCI_TIMER_PTR timer_ptr     // Pointer to a previously created application
                                // timer.
    );
    
/*****************************************************************************/
//  Description:    The function retrieves information about the specified 
//                  application timer.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_GetTimerInfo( // If get successful, returns SCI_SUCCESS,
                                // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr,    // Pointer to a previously created application
                                // timer.
    char    *name,              // Pointer to an SCI_MAX_NAME_SIZE character
                                // destination area for the time¡¯s name.
    BOOLEAN *active,            // Pointer to destination for the timer active
                                // indication. If this value is SCI_TRUE, 
                                // the timer is active, else is deactive.
    uint32  *remaining_time,    // Pointer to destination for the number of 
                                // timer ticks left before the timer expires.
    uint32  *reschedule_time    // Pointer to destination for the number of 
                                // timer ticks that will be used to 
                                // automatically reschedule this timer.
    );

/**---------------------------------------------------------------------------*
 ** TIME MACRO:  
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
PUBLIC uint32 SCI_GetCurrentTime(void);

/*****************************************************************************/
//  Description:    This function retrieves the number of milliseconds that 
//                  have elapsed since the system was started.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:           The elapsed time is stored as a uint32 value. Therefore, 
//                  the time will wrap around to zero if the system is run 
//                  continuously for 49.7 days. 
/*****************************************************************************/
PUBLIC uint32 SCI_GetTickCount(void);    // Number of ticks.


/**---------------------------------------------------------------------------*
 ** Mutex
 **---------------------------------------------------------------------------*/
//@Zhemin.lin, 09/12/2003, CR:MS00004678, begin
/*****************************************************************************/
//  Description:    The function is used to create a mutex for inter-thread
//                  mutual exclusion for resource protection.
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
SCI_MUTEX_PTR SCI_CreateMutex( //if successful, return the muxtex pointer,
                                          //else return SCI_NULL
    const char *name_ptr,     //mutex name
    uint32 priority_inherit  //inherit option, SCI_INHERIT or SCI_NO_INHERIT
);

/*****************************************************************************/
//  Description:    The function is used to destroy a mutex.
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
uint32 SCI_DeleteMutex( //if successful, return SCI_SUCCESS,
                                          //else return SCI_ERROR
    SCI_MUTEX_PTR mutex_ptr  //mutex pointer
);

/*****************************************************************************/
//  Description:    The function is used to obtain exclusive ownership of the
//                  specified mutex.
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
uint32 SCI_GetMutex( //if successful, return SCI_SUCCESS,
                              //else return SCI_ERROR
    SCI_MUTEX_PTR mutex_ptr, //mutex pointer
    uint32 	wait_option     //wait option: SCI_WAIT_FOREVER, SCI_NO_WAIT,
                              //             1~0x0FFFFFFFE wait time(ms)
);

/*****************************************************************************/
//  Description:    The function is used to renounce ownership of a mutex
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
uint32 SCI_PutMutex( //if successful, return SCI_SUCCESS,
                              //else return SCI_ERROR
    SCI_MUTEX_PTR mutex_ptr //mutex pointer
);

/**---------------------------------------------------------------------------*
 ** Semaphore
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function is used to create a counting semaphore.
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
SCI_SEMAPHORE_PTR SCI_CreateSemaphore( //if successful, return semaphore
                                                     //pointer, else return SCI_NULL
    const char *name_ptr, //name of the semaphore
    uint32 initial_count //initial value of semaphore counter
);

/*****************************************************************************/
//  Description:    The function is used to destroy the specified counting semaphore
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
uint32 SCI_DeleteSemaphore ( //if successful return SCI_SUCCESS,
                                           //else return SCI_ERROR
    SCI_SEMAPHORE_PTR sem_ptr //semaphore pointer
);

/*****************************************************************************/
//  Description:    The function is used to retrieve an instance from the specified
//                  counting semaphore. As a result, the specified semaphore's count
//                  is decreased by one.
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
uint32 SCI_GetSemaphore (//if successful return SCI_SUCCESS,
                                     //else return SCI_ERROR
    SCI_SEMAPHORE_PTR sem_ptr,//semaphore pointer
    uint32 wait_option       //wait option: SCI_WAIT_FOREVER, SCI_NO_WAIT,
                               //             1~0x0FFFFFFFE wait time(ms)
);

/*****************************************************************************/
//  Description:    The function is used to puts an instance into the specified
//                  counting semaphore, which in reality increments the counting
//                  semaphore by one.
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
uint32 SCI_PutSemaphore ( //if successful return SCI_SUCCESS,
                                      //else return SCI_ERROR
    SCI_SEMAPHORE_PTR sem_ptr //semaphore pointer
);

/**---------------------------------------------------------------------------*
 ** Mutex
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
    
/*****************************************************************************/
//  Description:    This function put a message to the trace buffer which log
//                  level is no more than LOG_LEVEL_MED. 
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_TraceMid(
    const char *x_format, ...);

/*****************************************************************************/
//  Description:    This function put a message to the trace buffer which log
//                  level is no more than LOG_LEVEL_HIGH. 
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_TraceHigh(
    const char *x_format, ...);

/*****************************************************************************/
//  Description:    This function put a message to the trace buffer, and then 
//                  assert. 
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_TraceError(
    const char *x_format, ...);    

/*****************************************************************************/
//  Description:    This function trace out buffer.
//	Global resource dependence: DEBUG_PORT_SEND_BUF_SIZE
//  Author: Richard.Yang
//	Note:           1. Size should be not too big because of 
//                  DEBUG_PORT_SEND_BUF_SIZE is limited.
//                  2. Address should be access in byte or word.
/*****************************************************************************/
PUBLIC void SCI_TraceBuf(
    const char  *string_ptr,    // A string inputed.
    void        *addr,          // Beginer of address to be traced out.
    uint32      size            // Size in bytes to be traced out.
    );    

/*****************************************************************************/
//  Description:    This function trace data in hex mode.
//	Global resource dependence: DEBUG_PORT_SEND_BUF_SIZE
//  Author: Richard.Yang
//	Note:           1. Size should be not too big because of 
//                  DEBUG_PORT_SEND_BUF_SIZE is limited.
//                  2. Address should be access in byte .
/*****************************************************************************/
PUBLIC void SCI_TraceCapData(
    uint16      data_type,      // sub data type
    const void  *addr,          // Beginer of address to be traced out.
    uint32      size            // Size in bytes to be traced out.
    );    

PUBLIC BOOLEAN 	SCI_InThreadContext(void);
/* This function should be called only after fiq and irq are both diabled */
PUBLIC void		SCI_EnterAssertMode(void);

/*****************************************************************************/
//  Description:    This function is used to get the pointer to the head pointer
//  				of the created thread list.
//	Global resource dependence: 
//  Author: kevin.wang
//	Note:
/*****************************************************************************/
PUBLIC void *SCI_ThreadListHeadPtr(void);

/*****************************************************************************/
//  Description:    This function judge if mem pool is nearly to empty
//	Global resource dependence: 
//  Author: yi.su
//	Note:       
/*****************************************************************************/
PUBLIC uint32 SCI_GetMempoolInfo(uint32 poolsize, uint32 *available_num);

/*****************************************************************************/
//  Description:    This function is to get the max availd fragment space in the special byte heap
//	Global resource dependence: 
//  Author: Thomas.Chai
//	Note:       
/*****************************************************************************/
PUBLIC uint32  SCI_GetByteHeapAvailableSpace(uint32  heap_type);


/*****************************************************************************/
//  Description:    The SCI_CreateDynamicPool function is used to create  memory pools configuated by application 
//                 with a special memory space. The  pool number should be not less than one. And the total memory space 
//                of  all pools should be equal with the special memory space, if the total space is less than the specail space,
//                the left memory would not be used. And if the total space is more than the special space, then some pools
//                would not be created for not enough memory.
//	Global resource dependence: 
//  Author: thomas.chai
/*****************************************************************************/
PUBLIC  MPOOL_HANDLE  SCI_CreateMemPool(  // return: 0 -- Error,  others --- the created dynamic pool handle
      const char* pool_name,   //Pool Name
      uint8* mem_addr,         //start address of the specail memory 
      uint32 mem_size,          //the spacial memory space size
      APP_MEM_CFG_T*   mem_cfg_ptr   //Memory Pool config information
     );

/*****************************************************************************/
//  Description:    The SCI_DeleteDynamicPool function is used to Release  all memory pools Created 
//                          whith SCI_CreateDynamicPool
//	Global resource dependence: 
//  Author: thomas.chai
/*****************************************************************************/
PUBLIC  uint32 SCI_DeleteMemPool( //return: SCI_SUCCESS -- Successful,  SCI_ERROR--error
	MPOOL_HANDLE   pool_handle,    //pool handle which created by  SCI_CreateDynamicPool
	BOOLEAN   is_force_delete         //FALSE -- will check whether all memory had been free, TRUE --Not Check whether all memory had been free
	);
	
/*****************************************************************************/
//  Description:    The SCI_DPMalloc function is used to alloc memory blocks
//                  in embedded system. It allocates memory first from  dynamic memory pool ,if there is not enough memory,
//                  then  alloc from system memory pool, if they also have not enough memory,then alloc from system heap.
//	Global resource dependence: 
//  Author: thomas.chai
/*****************************************************************************/
PUBLIC void*  SCI_MemPoolMalloc(  // Returns a void pointer to the allocated space, or SCI_NULL if there is insufficient memory
	uint32 size,     // Bytes to allocate.
       MPOOL_HANDLE  pool_handle,    //pool handle which created by  SCI_CreateDynamicPool
       const char * file,    // The name of the file which call this function.
       uint32 line             // The line number in the file which call this function.
       );


/*****************************************************************************/
//  Description:    The SCI_DFFree function is used to Release the memory which alloced  with  SCI_DPMalloc
//	Global resource dependence: 
//  Author: thomas.chai
//	Note:           Don't use it directly. Please use SCI_FREE
/*****************************************************************************/
PUBLIC uint32 SCI_MemPoolFree(     // SCI_SUCCESS or SCI_ERROR
    void *memory_ptr        // Pointer to memory block to be deleted.
    );

/*****************************************************************************/
//  Description:    The SCI_DumpAppMemInfo function is used to dump app heap memory
//	Global resource dependence: 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC void SCI_DumpAppMemInfo(void);

/*****************************************************************************/
//  Description: The SCI_GetSysPoolInfo function is used to get system pool and
//                pool information  
//	Global resource dependence: 
//  Author: 
//	Note:   
/*****************************************************************************/
uint32 SCI_GetSysPoolInfo(           //SCI_SUCCESS -- successful; SCI_POOL_INFO_END -- the latest pool/heap information; others--error
								SYS_POOL_INFO_T* pool_ptr,  //output the system pool information
								uint32 flag  // POOL_INFO_SEEK_FIRST -- first time get the pool information; POOL_INFO_SEEK_NEXT -- get the next pool information
								);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End of _OS_API_H
