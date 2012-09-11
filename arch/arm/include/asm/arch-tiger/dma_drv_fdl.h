/******************************************************************************
    David.Jia   2007.10.29      share_version_union

    TCC -fpu None -O2 -bi -g+ -apcs /interwork -D__RUN_IN_SDRAM sdram_init.c
SC6600R    -D_BL_NF_SC6600R_
SC6600H    -D_BL_NF_SC6600H_
SC6600I    -D_BL_NF_SC6600I_
SC6800     -gtp -cpu ARM926EJ-S -D_REF_SC6800_ -D_BL_NF_SC6800_
******************************************************************************/
#ifndef _DMA_DRV_H
#define _DMA_DRV_H
#include "sci_types.h"
#include "sc_reg.h"

//define DC_DMA Base Address
#define DMA_BASE       0x20100400

#define DMA_CHEN             0x20100004
#define DMA_HWEN             0x20100008
#define DMA_LINKEN           0x2010000C
#define DMA_PRIREG0          0x20100020
#define DMA_PRIREG1          0x20100024
#define DMA_INTMASKSTS       0x20100030
#define DMA_INTRAWSTS        0x20100034
#define DMA_ALLINTEN         0x20100040
#define DMA_BURSTINTEN       0x20100044
#define DMA_ALLINTMASKSTS    0x20100050
#define DMA_BURSTINTMASKSTS  0x20100054
#define DMA_ALLINTRAWSTS     0x20100060
#define DMA_BURSTINTRAWSTS   0x20100064
#define DMA_ALLINTCLR        0x20100070
#define DMA_BURSTINTCLR      0x20100074
#define DMA_SOFTREQ          0x20100080
#define DMA_SOFTACK          0x20100084
#define DMA_CHDONE           0x20100088
#define DMA_LINKLOADDONE     0x2010008C
#define DMA_STARTADDR        0x20100090
#define DMA_ENDADDR          0x20100094





typedef struct DMA_S
{
    volatile uint32 cfg0;
    volatile uint32 cfg1;
    volatile uint32 srcAddr;
    volatile uint32 destAddr;
    volatile uint32 llptr;
    volatile uint32 di;
    volatile uint32 sbi;
    volatile uint32 dbi;
} DMA_T;

void dma_init();

#endif
