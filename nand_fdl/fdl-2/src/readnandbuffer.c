#include "sci_types.h"

#if !defined(NAND_FDL_SC6800D)&!defined(NAND_FDL_SC8800H)/*lint !e514*/
#define _USE_DMA
#endif
void NandCopy(uint32 *pSrc,uint32 *pDst,uint32 len);

#ifdef _USE_DMA
   extern void dma_memcpy(uint32 ch_id,uint32 *src_addr,uint32 *dest_addr,uint32 len);
#endif



void NandCopy(uint32 *pSrc,    //Source Address
                  uint32 *pDst,    //Destination Address
                  uint32 len       //the byte length
                  )
{
    uint32 i;
  #ifndef _USE_DMA
	    for(i=0;i<len/4;i++)
	    {
	       *pDst = *pSrc;
	       pDst++;
	       pSrc++;
	    }
  #else
  //*(volatile uint32 *)0x8a000080&=~(0x40);
       dma_memcpy(21,pSrc,pDst,len);
  //*(volatile uint32 *)0x8a000080|=(0x40); 
  #endif

}

