/******************************************************************************
    David.Jia   2007.10.29      share_version_union

    TCC -fpu None -O2 -bi -g+ -apcs /interwork -D__RUN_IN_SDRAM main.c
SC6600R    -D_BL_NF_SC6600R_
SC6600H    -D_BL_NF_SC6600H_
SC6600I    -D_BL_NF_SC6600I_
SC6800     -gtp -cpu ARM926EJ-S -D_REF_SC6800_ -D_BL_NF_SC6800_
******************************************************************************/
#ifndef BL_TRACE_H
#define BL_TRACE_H

#ifdef BL_TRACE_ENABLED
#include "sio_drv.h"

#if 0
#define BL_TRACE_OPEN()     sio_open()
#define BL_TRACE0           sio_putstr
#else
#define BL_TRACE_OPEN(...)
#define BL_TRACE0(...)
#endif


#if (defined PLATFORM_SC8800H)||(defined PLATFORM_SC6800H)||(defined PLATFORM_SC8800G)||(defined CONFIG_SC8810)
#define BL_TRACE(...)
#endif  //#if (defined PLATFORM_SC8800H)||(defined PLATFORM_SC6800H)||(defined PLATFORM_SC8800G)||(defined CONFIG_SC8810)

#else
__inline int sio_putstr_stub (const char *str)
{
    (void) str;
    return 0;
}

__inline int printk_stub (const char *fmt, ...)
{
    (void) fmt;
    return 0;
}

#define BL_TRACE_OPEN()     ( ( void )0 )
#define BL_TRACE0           sio_putstr_stub
#define BL_TRACE            printk_stub

#endif /* BL_TRACE_ENABLED */

#endif /* BL_TRACE_H */