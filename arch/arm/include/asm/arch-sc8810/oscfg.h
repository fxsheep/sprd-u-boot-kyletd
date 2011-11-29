/******************************************************************************
 ** File Name:      oscfg.h                                                   *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           11/08/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file is used to do some definition about task        *
 **                 creation. If static task is created or thread entry is    *
 **                 defined, you should include this file.                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/08/2006     Benjamin.Wang    Create.                                   *
 ******************************************************************************/

#ifndef _OSCFG_H
#define _OSCFG_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/
/* For dynamic malloc task stack mode*/
#define DEF_TASK(__name, __prio, __stack_size, __queue_num) \
{ \
    void* __name##_STACK_ADDR = (void*)SCI_ALLOC_CONST(((__stack_size >> 2) << 2) + 3); \
    char* __name##_QUEUE_ADDR = (char*)SCI_ALLOC_CONST(__queue_num * SCI_QUEUE_ITEM_SIZE * 4 + 3); \
    extern void thread_entry_##__name(uint32, void*); \
    SCI_ASSERT((__name##_STACK_ADDR != SCI_NULL) && (__name##_QUEUE_ADDR != SCI_NULL)); \
    __name##_STACK_ADDR = (void*)((((uint32)__name##_STACK_ADDR + 3) >> 2) << 2); \
    __name##_QUEUE_ADDR = (char*)((((uint32)__name##_QUEUE_ADDR + 3) >> 2) << 2); \
    SCI_CreateStaticThread(__name, "T_"#__name, thread_entry_##__name, 0, 0, \
    __name##_STACK_ADDR, __stack_size, __prio, SCI_PREEMPT, "Q_"#__name, \
    __name##_QUEUE_ADDR, __queue_num, SCI_AUTO_START); \
}

/* For static task stack mode*/
/*
#define DEF_TASK(__name, __prio, __stack_size, __queue_num) \
{ \
    static uint32 __name##_STACK_ADDR[__stack_size >> 2]; \
    static uint32 __name##_QUEUE_ADDR[__queue_num * SCI_QUEUE_ITEM_SIZE]; \
    extern void thread_entry_##__name(uint32, void*); \
    SCI_CreateStaticThread(__name, "T_"#__name, thread_entry_##__name, 0, 0, \
    __name##_STACK_ADDR, __stack_size, __prio, SCI_PREEMPT, "Q_"#__name, \
    __name##_QUEUE_ADDR, __queue_num, SCI_AUTO_START); \
}
*/    

#define STATIC_TASK_DEF_BEGIN(__type) PUBLIC void SCI_CreateTasks##__type(void){ 

#define STATIC_TASK_DEF_END }

#define STATIC_TASK_CREATE(__type) \
{ \
    extern void SCI_CreateTasks##__type(void); \
    SCI_CreateTasks##__type(); \
}

#define THREAD_ENTRY(__name) void thread_entry_##__name(uint32 argc, void* argv)
/* Thread entry Example:
 * THREAD_ENTRY(CM) //Entry name(__name) should be the same one as task define.
 * {
 *     // Attention: Notice module's initialization order. Avoid system's block.
 *     while(1)
 *     {
 *         //wait event or get signal
 *     }
 * }
 */

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/      

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _OSCFG_H

// End of oscfg.h

