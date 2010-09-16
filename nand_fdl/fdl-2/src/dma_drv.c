#include "sci_types.h"
#include "sc_fdl_reg.h"
#include "dma_drv_fdl.h"



//extern uint8 jpeg_enc_buf[256*1024];
//extern uint8 jpeg_dec_buf[512*1024];
//extern uint8 mjpeg_enc_buf[512*1024];
//extern uint8 mjpeg_dec_buf[512*1024];


void dma_init()
{

  //Enable DMA AHB_CLK
   *(volatile uint32 *)(AHB_CTL0)|= 0x1 << 3;			     // enable dma part
   
   //clear all DMA int
   *(volatile uint32 *)DMA_ALLINTCLR=0x3fffff;
   //Enable DMA IRQ
   //*(volatile uint32 *)(INT_IRQ_EN )|=(BIT_20);
   
}

void dma_memcpy(uint32 ch_id,uint32 *src_addr,uint32 *dest_addr,uint32 len)
{

    uint32 dma_int_flag;
    
    
    //Set DMA/ARM Arbiter 
    //*(volatile uint32 *)(AHB_ARBITER_CTL) = 0x00010106;	     
    
    switch(ch_id)
    {
       case 20:
       {    
            DMA_T *dma_ptr =(DMA_T *)(DMA_BASE+20*32);
            //Set HardBurstWait to Maxium 255
            *(volatile uint32 *)DMA_CFG |= (0xff<<8);
            *(volatile uint32 *)DMA_HWEN=(0x1<<20);
            *(volatile uint32 *)DMA_ALLINTEN=(0x1<<20); 
            //Enable Hardware REQ           
            dma_ptr->cfg0=0x0a800010;
            dma_ptr->cfg1=len;          
            dma_ptr->srcAddr=(uint32)src_addr;
            dma_ptr->destAddr=(uint32)dest_addr; 
            dma_ptr->llptr=0;        
            dma_ptr->di=0x00048000;          
            dma_ptr->sbi=0x4;
            dma_ptr->dbi=0x80000004; 
            //Enable ch21
            *(volatile uint32 *)DMA_CHEN|=(0x1<<20);
        }
            break;
            
       case 21:
       {
            DMA_T *dma_ptr =(DMA_T *)(DMA_BASE+21*32);
             //Set SoftBurstWait to 0x01,min value
            *(volatile uint32 *)DMA_CFG |= (0x1<<16);
            *(volatile uint32 *)DMA_ALLINTEN=(0x1<<21); 
            //source byte num                  
            dma_ptr->cfg0=0x0a800200;
            dma_ptr->cfg1=len;          
            dma_ptr->srcAddr=(uint32)src_addr;
            dma_ptr->destAddr=(uint32)dest_addr; 
            dma_ptr->llptr=0;        
            dma_ptr->di=0x00040004;          
            dma_ptr->sbi=0x4;
            dma_ptr->dbi=0x4;           
            
            //Enable ch21
            *(volatile uint32 *)DMA_CHEN|=(0x1<<21);
            //Send a Soft request
            *(volatile uint32 *)DMA_SOFTREQ=(0x1<<21);
        }             
            break;
       
       default:
            break;
            
    
    }
   
    while(1)
    {
        dma_int_flag=*(volatile uint32 *)DMA_CHDONE;
        if(dma_int_flag &(1<<ch_id))
        {
             //clear DMA int
             *(volatile uint32 *)DMA_ALLINTCLR=1<<ch_id;
             break;
        }
    
    }
    return;
    
}

