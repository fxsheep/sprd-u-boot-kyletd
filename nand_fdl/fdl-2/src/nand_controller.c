/******************************************************************************
 ** File Name:      nand_controller.c                                         *
 ** Author:         Nick.zhao                                                 *
 ** DATE:           01/06/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/ 
#include "nand_controller.h"
#include "sc_fdl_reg.h"
#include "nand_ext.h"
/******************************************************************************
                          Macro define
******************************************************************************/
//NandFlash command
#define NF_READ_ID          0x90
#define NF_READ_STATUS      0x70
#define NF_READ_1ST         0x00
#define NF_READ_2ND         0x01
#define NF_RESET            0xFF
#define NF_READ_SPARE       0x50
#define NF_SWITCH_SPARE		0x50fd 
#define NF_ID_ADDR          0x00
#define NF_BKERASE_ID		0x60
#define NF_BKERASE_EXC_ID 	0xD0
#define NF_WRITE_ID			0x80
#define NF_WRITE_EXC_ID		0x10
#define NF_SWITCH_MAIN      0x00fd

#define NF_TIMEOUT_VAL 0x1000000
#define NF_SWITCHMAIN_CMD_TMOUT 100

#define PAGE_SIZE_S         512
#define SPARE_SIZE_S        16
#define PAGE_SIZE_L         2048
#define SPARE_SIZE_L        64

//#define _USE_INT
#undef _NAND_CON_DEBUG
#undef _USE_THREADX

#define NFC_SET_COMMAND                                  \
     {                                                   \
          nfc_cmd=g_CmdSetting|(cmd);                    \
          nandctl_ptr->cmd=nfc_cmd;                      \
     }

#ifndef _USE_INT                             
	#define NFC_WAIT_COMMAND_FINISH                                   \
	      {                                                           \
	         nfc_cmd=nandctl_ptr->cmd;                                \
	         counter=0;                                               \
	         while((nfc_cmd&(0x1<<31))&&(counter<NF_TIMEOUT_VAL))\
	         {                                                        \
		        nfc_cmd=nandctl_ptr->cmd;                             \
			    counter++;                                            \
			 }                                                        \
			 if(NF_TIMEOUT_VAL==counter)                         \
			 {                                                        \
			    return ERR_NF_TIMEOUT;                                \
			 }                                                        \
	      }
#else
    #define NFC_WAIT_COMMAND_FINISH                                  \
           {                                                         \
               nfc_cmd=nandctl_ptr->cmd;                             \
               while(!(g_Nand_Int_flag & BIT_0))                     \
               {                                                     \
                 ;                                                   \
               }                                                     \
               g_Nand_Int_flag&=~(BIT_0);                            \
           }
#endif    

#ifndef _USE_INT                             
	#define NFC_WAIT_SWITCHMAIN_COMMAND_FINISH                           			\
	      {                                                           			    \
	         nfc_cmd = nandctl_ptr->cmd;                                 			\
	         counter = 0;                                                			\
	         while((nfc_cmd & (0x1<<31)) && (counter < NF_SWITCHMAIN_CMD_TMOUT))	\
	         {                                                           			\
		        nfc_cmd = nandctl_ptr->cmd;                              			\
			    counter++;                                               			\
			 }                                                           			\
			 if(NF_SWITCHMAIN_CMD_TMOUT == counter)                         	    \
			 {                                                           			\
			 	nfc_cmd = NF_RESET|(0x1<<31);						  	 			\
			 	nandctl_ptr->cmd = nfc_cmd;						  	     			\
			 	NFC_WAIT_COMMAND_FINISH							         			\
			 }													         			\
	      }
#else
    #define NFC_WAIT_SWITCHMAIN_COMMAND_FINISH                                  		  \
           {                                                         					  \
               nfc_cmd = nandctl_ptr->cmd;                            		    		  \
               counter = 0; 											 	    		  \
               while((!(g_Nand_Int_flag & BIT_0)) && (counter<NF_SWITCHMAIN_CMD_TMOUT) )  \
               {                                                     					  \
                   counter++;                                                   		  \
               }                                                     					  \
               if(NF_SWITCHMAIN_CMD_TMOUT == counter)                         	  		  \
			   {                                                        				  \
			       nfc_cmd = NF_RESET|(0x1<<31);					  				      \
			 	   nandctl_ptr->cmd = nfc_cmd;						  				      \
			 	   NFC_WAIT_COMMAND_FINISH							  				      \
			   }																		  \
			   else																		  \
			   {																		  \
			   	   g_Nand_Int_flag &= ~(BIT_0);                            				  \
			   }  																		  \
           }
#endif  
#define MAX_AHB_TIMING_NUM                             (10)
#define DEFAULT_AHB_VAL                                     (0xffffffff)
#define DEFAULT_NAND_TIMING                            (0xffffffff)
#define ORIGINAL_NAND_TIMING                           (0x7bd07)

/******************************************************************************
                          Struct Definition
******************************************************************************/
typedef struct
{
    uint32 ahb_clk;
    uint32 timing_param;
 }NAND_AHB_TIMING_T;


/******************************************************************************
                          Global Variable
******************************************************************************/
static NAND_PARA_T g_Nand_Para;     //current nand's specification
static uint32 g_CmdSetting;          //used to increase the speed of operate Nand
#ifdef _USE_INT
   volatile static uint32 g_Nand_Int_flag;     //indicate the Nand INT type
#endif

LOCAL uint32 g_Nand_Fisrt_clk;
LOCAL NAND_TIMING_T g_Nand_Timing;
LOCAL NAND_AHB_TIMING_T g_Nand_Ahb_Timing[MAX_AHB_TIMING_NUM] ={
		{DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
		{DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
		{DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
		{DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
		{DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
		{DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
		{DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
		{DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
		{DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
		{DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING}
};
		
/******************************************************************************
                          Local Functions Prototype
******************************************************************************/
LOCAL ERR_NF_E NANDCTL_Ecc_Trans(uint8 *pEccIn, uint8 *pEccOut,uint8 nSct);
/******************************************************************************
                          Functions
******************************************************************************/
/******************************************************************************/
//  Description:   Init nand controller 
//  Parameter:
//      ahb_clk:   the frequency of AHB clock
//	Return:     
//      ERR_NF_SUCCESS    Init nand controller successfully    
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Init(uint32 ahb_clk)
{
#if defined(NAND_FDL_SC6800D)
   	//Enable NFC AHB_CLK
	*(volatile uint32 *)AHB_CTL0|=(0x1<<8);
	*(volatile uint32 *)0x8c000008 = (*(volatile uint32 *)0x8c000008)&(~(0x3))|BIT_1;
#elif defined(NAND_FDL_SC8800H)
   uint32 i;
    unsigned long nand_boot_mode;
    *(volatile uint32 *)AHB_CTL0|=(BIT_8|BIT_9);
#else
   	//Enable NFC AHB_CLK
	*(volatile uint32 *)AHB_CTL0|=(0x1<<5);
#endif

	//Clear all Nand INT
	*(volatile uint32 *)NFC_INT_SRC|=(BIT_0|BIT_4|BIT_5);
	
	//Set NFC_WPN 1     1: read or write  0: only read
	*(volatile uint32 *)NFC_WPN=0x1;
#ifdef NAND_FDL_SC8800H
     REG32(CPC_NFWPN_REG)  = (BIT_0|BIT_4|BIT_5);//NFWPN output enable, 
     REG32(CPC_NFWPN_REG) &= ~(BIT_6|BIT_7);
    
     //Add r/b wpu
     REG32(CPC_NFRB_REG)  = (BIT_0|BIT_3|BIT_4|BIT_5);
     REG32(CPC_NFRB_REG) &= ~(BIT_6|BIT_7);
     
     //Select   NFCLE,NFALE,NFCEN,NFWEN,NFREN,NFD_0-NFD_8 function
     //Increase NFCLE,NFALE,NFCEN,NFWEN,NFREN,NFD_0-NFD_8 driving strength
     for( i=0; i<14; i++ )
     {
         REG32(0x8c000204 + 4*i) |=  (BIT_4 | BIT_5);
         REG32(0x8c000204 + 4*i) &= ~(BIT_6 | BIT_7);
     }

    //Select   NFD_9-NFD_15 fuction
    //Increase NFD_9-NFD_15 driving strength	
    //0x8c00_0338--0x8c00_0350
    for(i = 0;i < 7; i++)
    {
        *(volatile uint32 *)(0x8c000338 + 4*i) |= (BIT_4 | BIT_5);
        *(volatile uint32 *)(0x8c000338 + 4*i) &= ~(BIT_6 | BIT_7);
        *(volatile uint32 *)(0x8c000338 + 4*i) |=BIT_6;
    }	                                    

    REG32(GR_NFC_MEM_DLY) = 0x0; 

    

#else
	//Add r/b wpu
	*(volatile uint32 *)0x8c0000c8|=0x1;
	
	//Set NFC Pin function
	*(volatile uint32 *)CPC_SEL_REG0=0;        //Select NFRB,NFCLE,NFALE,NFCEN,NFWEN,NFREN,NFWPN
	*(volatile uint32 *)CPC_SEL_REG1=0;        //Select NFD_0-NFD_7
	*(volatile uint32 *)CPC_SEL_REG5&=~(0x3<<2|0x3<<8);   //Select NFD_14,NFD_15
	*(volatile uint32 *)CPC_SEL_REG7&=~(0x3);        //Select NFD_13
	*(volatile uint32 *)CPC_SEL_REG8&=~(0x3ff<<6);   //Select NFD_8,NFD_9,NFD_10,NFD_11,NFD_12
#endif	
	//Set Nand Parameter
	//NANDCTL_SetParam(ahb_clk);
	g_Nand_Fisrt_clk = ahb_clk;
	
	//Set timeout parameter
	
#ifdef _USE_INT	
   	g_Nand_Int_flag=0;
   	//Enable NAND timeout int and wpn int
	*(volatile uint32 *)NFC_INT_EN|=(BIT_0|BIT_4|BIT_5);
	
    //Register Nand ISR if needed
    
	//Enable nlc_irq
	*(volatile uint32 *)INT_IRQ_EN|=(BIT_27);
	
#endif
	
	return ERR_NF_SUCCESS;
	
}
/******************************************************************************/
//  Description:   Open nand controller 
//  Parameter:
//      pNandPara    the parameter of nandflash
//	Return:     
//      ERR_NF_SUCCESS    Open nand controller successfully    
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Open(NAND_PARA_T *pNandPara)
{
    uint32 addr_cycle;    //Advance 0: can be set 3 or 4
                          //Advance 1: can be set 4 or 5
    uint32 pagetype;      //0: Small page  1: Large page
    uint32 advance;       //Can be set 0 or 1
    uint32 buswidth;      //0: X8 bus width 1: X16 bus width
   
	if(NULL == pNandPara)
	{
	   return ERR_NF_FAIL;
	}
	//Initialize g_Nand_Para and g_Nand_Cmd
	g_Nand_Para.acycle = pNandPara->acycle;   	
	g_Nand_Para.pagesize = pNandPara->pagesize;
	g_Nand_Para.sparesize = pNandPara->sparesize;
	g_Nand_Para.buswidth = pNandPara->buswidth;
	g_Nand_Para.advance = pNandPara->advance;
   	g_Nand_Para.eccpos = pNandPara->eccpos;
   	if(512==g_Nand_Para.pagesize)
   	{
   	    pagetype=0;
   	}
   	else
   	{
   	    pagetype=1;
   	}
   	if(g_Nand_Para.acycle==3)
   	    addr_cycle=0;
   	else if((g_Nand_Para.acycle==4) &&(g_Nand_Para.advance==1))
   	    addr_cycle=0;
   	else
   	    addr_cycle=1;
   	advance=g_Nand_Para.advance;
   	buswidth=g_Nand_Para.buswidth;
 
    g_CmdSetting=(addr_cycle<<24)|(advance<<23)|(buswidth<<19)|(pagetype<<18)|(0<<16)|(0x1<<31); 
    
    return ERR_NF_SUCCESS;
}

/******************************************************************************/
//  Description:   Close nand controller 
//  Parameter:
//      NULL
//	Return:     
//      ERR_NF_SUCCESS    Close nand controller successfully    
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Close()
{

	//Set g_Nand_Para and g_Nand_Cmd Zero
	g_Nand_Para.acycle = 0;   	
	g_Nand_Para.pagesize = 0;
	g_Nand_Para.sparesize = 0;
	g_Nand_Para.buswidth = 0;
	g_Nand_Para.advance = 0;
   	g_Nand_Para.eccpos = 0;
 
    g_CmdSetting=0; 
    
    return ERR_NF_SUCCESS;
} 


/******************************************************************************/
//  Description:   Set NF parameter according to AHB_CLK 
//  Parameter:
//      ahb_clk    the frequency of AHB_CLK
//	Return:     
//      ERR_NF_SUCCESS    Set NF parameter successfully    
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_SetParam(uint32 ahb_clk)
{
    uint32 counter, i,temp, nCycle;
    BOOLEAN bFind;
    NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
 
    counter = 0;
 #ifdef _USE_THREADX   
    SCI_DisableIRQ();
 #endif
    //Wait until last Nand cmd completed.
    while(((nandctl_ptr->cmd>>31)&0x1)&&(counter<NF_TIMEOUT_VAL))
    {
         counter++;    
    }
 #ifdef _USE_THREADX
    SCI_ASSERT(counter!=NF_TIMEOUT_VAL);
 #else
    if(counter == NF_TIMEOUT_VAL)
    {
       //SCI_TraceLow("Set NandParameter Failed!\n\r");
       return ERR_NF_FAIL;
    }
 #endif
  #ifndef NAND_FDL_SC8800H
 
        bFind = FALSE;
    i=0;
    do
    {
         if(DEFAULT_AHB_VAL==g_Nand_Ahb_Timing[i].ahb_clk)
         {
              g_Nand_Ahb_Timing[i].ahb_clk = ahb_clk;
              break;
         }
         else if(ahb_clk == g_Nand_Ahb_Timing[i].ahb_clk)
        {
             bFind == TRUE;
             break;
         }
         else
             i++;
    }while(i<MAX_AHB_TIMING_NUM);
    
 #ifdef _USE_THREADX
   SCI_ASSERT(MAX_AHB_TIMING_NUM!=i);
#endif
   if(bFind)/*lint !e774*/
   	*(volatile uint32 *)NFC_PARA =  g_Nand_Ahb_Timing[i].timing_param;
   else
   {
        temp = (1000000000>>20)/(ahb_clk>>20);
        nCycle = g_Nand_Timing.nTwcTime/temp;
        g_Nand_Ahb_Timing[i].timing_param = ORIGINAL_NAND_TIMING|(nCycle<<4);/*lint !e661*/
        *(volatile uint32 *)NFC_PARA =  g_Nand_Ahb_Timing[i].timing_param;/*lint !e661*/
   }
#else
#define NF_PARA_20M        0x7ac05      //trwl = 0  trwh = 0
#define NF_PARA_40M        0x7ac15      //trwl = 1  trwh = 0
#define NF_PARA_53M        0x7ad26      //trwl = 2  trwh = 1
#define NF_PARA_80M        0x7ad37      //trwl = 3  trwh = 1
#define NF_PARA_DEFAULT    0x7ad77      //trwl = 7  trwh = 1  
     //Set Nand Parameter 
     ahb_clk /= 1000000;
    switch(ahb_clk)
    {
        case 20:
             REG32(NFC_PARA)=NF_PARA_20M;
             break;
        case 40:
             REG32(NFC_PARA)=NF_PARA_40M;
             break;
        case 53:
             REG32(NFC_PARA)=NF_PARA_53M;
             break;
        case 80:
             REG32(NFC_PARA)=NF_PARA_DEFAULT;
             break;
        default:
             REG32(NFC_PARA)=NF_PARA_DEFAULT;    
    }


#endif
 #ifdef _USE_THREADX   
    SCI_RestoreIRQ();
 #endif
    return ERR_NF_SUCCESS;
}
/******************************************************************************/
//  Description:   Input the nandflash parameter
//  Parameter:
//      pNandTiming    the frequency of AHB_CLK
//	Return:     
//      ERR_NF_SUCCESS    Set NF parameter successfully    
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_InitParam(NAND_TIMING_PTR pNandTiming)
{
        g_Nand_Timing.nTeTime = pNandTiming->nTeTime;
        g_Nand_Timing.nTpTime = pNandTiming->nTpTime;
        g_Nand_Timing.nTrTime = pNandTiming->nTrTime;
        g_Nand_Timing.nTwcTime = pNandTiming->nTwcTime;

        //Set Nand timing Parameter
        return NANDCTL_SetParam(g_Nand_Fisrt_clk);
        
}
LOCAL void NANDCTL_ISR(void)
{
#ifdef _USE_INT

    uint32 int_sts;
    
    int_sts=*(volatile uint32 *)NFC_INT_MSK;
    
    if(int_sts & BIT_4)
    {
       //Programming while Nand in write-protection mode
       //SCI_ASSERT(0);
       g_Nand_Int_flag|=BIT_4;
       //clear this int
       *(volatile uint32 *)NFC_INT_SRC |= BIT_4;
       while(1);
    }
    
    if(int_sts & BIT_5)
    {
       //Timeout while Nand Busy
       //SCI_ASSERT(0);
       g_Nand_Int_flag|=BIT_5;
       *(volatile uint32 *)NFC_INT_SRC |= BIT_5;
       while(1);
    }
    
    if(int_sts & BIT_0)
    {
       //Nand Operation finish
       g_Nand_Int_flag|=BIT_0;
       *(volatile uint32 *)NFC_INT_SRC |= BIT_0;
    } 
#endif       
    return;
}



/******************************************************************************/
//  Description:   Read nandflash ID
//  Parameter:
//      pID      out  the address of nandflash id 
//	Return:     
//      ERR_NF_SUCCESS     Get the ID successfully            
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Read_ID(uint8 *pID)
{
   volatile uint32 nfc_cmd;
   uint32 counter;
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
   
   if(NULL==pID)
      return ERR_NF_FAIL;
      
   nfc_cmd=NF_READ_ID|(0x1<<31);
   
   //Write command
   nandctl_ptr->cmd=nfc_cmd;

   //NFC_WAIT_COMMAND_FINISH
   NFC_WAIT_COMMAND_FINISH                    
                         
   *pID=(nandctl_ptr->idstatus)&0xff;
   *(pID+1)=((nandctl_ptr->idstatus)& 0xff00)>>8;
   *(pID+2)=((nandctl_ptr->idstatus)& 0xff0000)>>16;
   *(pID+3)=((nandctl_ptr->idstatus)& 0xff000000)>>24;	
           
   return ERR_NF_SUCCESS;   

}
/******************************************************************************/
//  Description:   Get nand status
//  Parameter:
//      none
//	Return:     
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Read_Status()
{
   uint32 nfc_cmd;
   uint32 counter;
   uint32 status;
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
   
   nfc_cmd=NF_READ_STATUS|(0x1<<31);
   
   //Write Command
   nandctl_ptr->cmd=nfc_cmd;
   
   
   NFC_WAIT_COMMAND_FINISH
   
   
   //read status 
   status=nandctl_ptr->idstatus; 	       
    
   if((status & 0x1)!= 0) 	
     return ERR_NF_FAIL;
   else if ((status & 0x40)== 0)
     return ERR_NF_BUSY;
   else 							
     return ERR_NF_SUCCESS;  

}

/******************************************************************************/
//  Description:   Reset NandFlash
//  Parameter:
//      None  
//	Return:     
//      ERR_NF_SUCCESS     Reset nandflash successfully    
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Reset()
{
   uint32 nfc_cmd;
   uint32 counter;
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
   
   nfc_cmd=NF_RESET|(0x1<<31);
   
   nandctl_ptr->cmd=nfc_cmd;
   
    //Read nfc_cmd until the command has been executed completely.
    NFC_WAIT_COMMAND_FINISH
    	       
    return ERR_NF_SUCCESS;   
}
/******************************************************************************/
//  Description:   Read main part and spare part of nand, ECC value can be get 
//                 if necessary. This function is used for small page nandflash.
//  Parameter:
//      page_addr:   in   the nand page address.
//      pMBuf:       in   the address of Main part
//      pSBuf:       in   the address of Spare part
//      pEcc:        out  the address of ECC value
//	Return:     
//      ERR_NF_SUCCESS    Read nand successfully  
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_MS_Read_S(
                             uint32 page_addr,
                             uint8 *pMBuf,
                             uint8 *pSBuf,
                             uint8 *pEcc
                             )
{
   uint32 cmd,nfc_cmd;
   uint32 counter;   
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
#if defined(NAND_FDL_SC6800D)||defined(NAND_FDL_SC8800H)
   uint8 Ecc_val_in[4];
   uint8 Ecc_val_out[4];
   uint32 *pEcc_val=(uint32 *)Ecc_val_in;  
#endif

#ifdef _NAND_CON_DEBUG
   SCI_ASSERT((NULL!=pMBuf)||(NULL!=pSBuf));
#endif
   //set start_addr and end_addr
   nandctl_ptr->start_addr0=page_addr;
   nandctl_ptr->end_addr0=0xffffffff;
   
   if(NULL!=pEcc)
   {
      cmd=NF_READ_1ST|(0x1<<21);
    	       
      //Configure nfc_cmd	       
      NFC_SET_COMMAND
   
      //Read nfc_cmd until the command has been executed completely.
      NFC_WAIT_COMMAND_FINISH
#if defined(NAND_FDL_SC6800D)||defined(NAND_FDL_SC8800H)
      //Get ECC
      *pEcc_val = nandctl_ptr->ecc0; 
      NANDCTL_Ecc_Trans(Ecc_val_in,Ecc_val_out,1);
	  pEcc[0] = Ecc_val_out[0];
	  pEcc[1] = Ecc_val_out[1];
	  pEcc[2] = Ecc_val_out[2];
#else
	  pEcc[0] = (uint8)(nandctl_ptr->ecc0 & 0xff);
	  pEcc[1] = (uint8)((nandctl_ptr->ecc0 & 0xff00)>>8);
	  pEcc[2] = (uint8)((nandctl_ptr->ecc0 & 0xff0000)>>16);
#endif	  
   }
   else
   { 
	    cmd=NF_READ_1ST;	    	       
	    //Configure nfc_cmd	       
	    NFC_SET_COMMAND	   
	    //Read nfc_cmd until the command has been executed completely.
	    NFC_WAIT_COMMAND_FINISH
	    
   }
	      	
   NandCopy((unsigned long *)NFC_MBUF_ADDR,(uint32 *)pMBuf,PAGE_SIZE_S);
   NandCopy((unsigned long *)NFC_SBUF_ADDR,(uint32 *)pSBuf,SPARE_SIZE_S);      
   return ERR_NF_SUCCESS;
   
}
/******************************************************************************/
//  Description:   Read main part of nand, ECC value can be get 
//                 if necessary.
//  Parameter:
//      page_addr:   in   the nand page address.
//      pMBuf:       in   the address of Main part
//	Return:     
//      ERR_NF_SUCCESS    Read nand successfully  
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_M_Read_S(
                            uint32 page_addr,
                            uint8 *pMBuf
                          )
{
   
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
   uint32 cmd,nfc_cmd;
   uint32 counter; 

#ifdef _NAND_CON_DEBUG
   SCI_ASSERT((NULL!=pMBuf));
#endif    
   //set start_addr and end_addr
   nandctl_ptr->start_addr0=page_addr;
   nandctl_ptr->end_addr0=page_addr+PAGE_SIZE_S-1;

   cmd=NF_READ_1ST;
   //Configure nfc_cmd	       
   NFC_SET_COMMAND   
   //Read nfc_cmd until the command has been executed completely.
   NFC_WAIT_COMMAND_FINISH

   
   //Can be replaced by DMA        
   NandCopy((unsigned long *)NFC_MBUF_ADDR,(uint32 *)pMBuf,PAGE_SIZE_S);
   return ERR_NF_SUCCESS;
   
}
/******************************************************************************/
//  Description:   Read spare part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      pSBuf:       in   the address of Spare part
//	Return:     
//      ERR_NF_SUCCESS    Read nand successfully  
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_S_Read_S(
                             uint32 page_addr,
                             uint8 *pSBuf
                           )
{
   uint32 cmd,nfc_cmd;
   uint32 counter;   
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;

#ifdef _NAND_CON_DEBUG
   SCI_ASSERT(NULL!=pSBuf);
#endif
	//set start_addr and end_addr
	nandctl_ptr->start_addr0=page_addr;
	nandctl_ptr->end_addr0=page_addr+SPARE_SIZE_S-1;     //It can also be set as (start_addr+g_Nand_Para->sparesize-1)
       
   cmd=NF_READ_SPARE;
   //Configure nfc_cmd	       
   NFC_SET_COMMAND
   //Read nfc_cmd until the command has been executed completely.
   NFC_WAIT_COMMAND_FINISH
   cmd=NF_SWITCH_MAIN;
   //Configure nfc_cmd	       
   NFC_SET_COMMAND
   //Read nfc_cmd until the command has been executed completely.
   NFC_WAIT_SWITCHMAIN_COMMAND_FINISH        
 
 
   
   NandCopy((unsigned long *)NFC_SBUF_ADDR,(uint32 *)pSBuf,SPARE_SIZE_S);
   
   return ERR_NF_SUCCESS;
   
}

/******************************************************************************/
//  Description:   Write main part and spare part of nand
//  Parameter:
//      page_addr: in  the nand page address.
//      pMBuf:     in  the address of main part
//      pSBuf:     in  the address of spare part
//      ecc_en:    in  the flag to enable ECC
//	Return:     
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_MS_Write_S(uint32 page_addr,uint8* pMBuf,uint8* pSBuf,uint8 ecc_en)
{
   uint32 cmd,nfc_cmd;
   uint32 counter;   
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
   ERR_NF_E ret;
   uint8 Ecc_val_in[4];
   uint8 Ecc_val_out[4];
   uint32 *pEcc_val=(uint32 *)Ecc_val_in;
   
#ifdef _NAND_CON_DEBUG
   SCI_ASSERT((NULL!=pMBuf)||(NULL!=pSBuf));
#endif   
   //set start_addr and end_addr
   nandctl_ptr->start_addr0=page_addr;
   nandctl_ptr->end_addr0=0xffffffff;   
   if(1==ecc_en)
   {
       //Open ECC
       *(volatile uint32 *)NFC_ECC_EN =0x1;   
	   //Copy main data to Nand Buffer 
	   NandCopy((uint32 *)pMBuf,(uint32 *)NFC_MBUF_ADDR,PAGE_SIZE_S);		   
	   //Get ECC
	   *pEcc_val = nandctl_ptr->ecc0;
       //Close ECC
       *(volatile uint32 *)NFC_ECC_EN =0x0;      
       //Transite the nand_controller's ECC to XSR ECC
       NANDCTL_Ecc_Trans(Ecc_val_in,Ecc_val_out,1);
       //Put the ecc value into the spare buffer
       pSBuf[g_Nand_Para.eccpos] = Ecc_val_out[0];
       pSBuf[g_Nand_Para.eccpos+1] = Ecc_val_out[1];
       pSBuf[g_Nand_Para.eccpos+2] = Ecc_val_out[2];
   }
   else
   {
	   //Copy main data to Nand Buffer 
	   NandCopy((uint32 *)pMBuf,(uint32 *)NFC_MBUF_ADDR,PAGE_SIZE_S);	
    
   }

   //Copy spare data to Nand Buffer
   NandCopy((uint32 *)pSBuf,(uint32 *)NFC_SBUF_ADDR,SPARE_SIZE_S);
  
   //Begin to write
   cmd=NF_WRITE_ID;
	          
   NFC_SET_COMMAND
	   
   //Read nfc_cmd until the command has been executed completely.
   NFC_WAIT_COMMAND_FINISH

   ret=NANDCTL_Read_Status();

   return ret;
   
}
/******************************************************************************/
//  Description:   Write main part of nand
//  Parameter:
//      page_addr: in  the nand page address.
//      pMBuf:     in  the main part address.
//	Return:     
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_M_Write_S(uint32 page_addr,uint8 *pMBuf)
{
   
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
   ERR_NF_E ret;
   uint32 cmd,nfc_cmd;
   uint32 counter;
   
#ifdef _NAND_CON_DEBUG
   SCI_ASSERT(NULL!=pMBuf);
#endif   
   //Copy main data to Nand Buffer 
   NandCopy((uint32 *)pMBuf,(uint32 *)NFC_MBUF_ADDR,PAGE_SIZE_S);	
      
   //set start_addr and end_addr
   nandctl_ptr->start_addr0=page_addr;
   nandctl_ptr->end_addr0=page_addr +PAGE_SIZE_S - 1;
   
   //Begin to write
   cmd=NF_WRITE_ID;
    	       
   NFC_SET_COMMAND
   
   //Read nfc_cmd until the command has been executed completely.
   NFC_WAIT_COMMAND_FINISH
    
   ret=NANDCTL_Read_Status();

   return ret;
   
}
/******************************************************************************/
//  Description:   Write spare part of nand
//  Parameter:
//      page_addr: in  the nand page address.
//      pSBuf      in  the address of spare part
//	Return:     
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_S_Write_S(uint32 page_addr,uint8 *pSBuf)
{
   uint32 cmd,nfc_cmd;
   uint32 counter;   
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
   ERR_NF_E ret;
   
#ifdef _NAND_CON_DEBUG
   SCI_ASSERT(NULL!=pSBuf);
#endif 
   //Copy spare data to Nand Buffer
   NandCopy((uint32 *)pSBuf,(uint32 *)NFC_SBUF_ADDR,SPARE_SIZE_S);

   //set start_addr and end_addr
   nandctl_ptr->start_addr0=page_addr;
   // nandctl_ptr->end_addr0=0xffffffff;     //It can also be set as (start_addr+g_Nand_Para->sparesize-1)
   nandctl_ptr->end_addr0=page_addr+SPARE_SIZE_S-1;   
   //Begin to write         	   
   //Area switch
   nfc_cmd=NF_SWITCH_SPARE|(0x1<<31);
   nandctl_ptr->cmd=nfc_cmd;   
	      
   NFC_WAIT_COMMAND_FINISH 
   //Send write command
   cmd=NF_WRITE_ID;	       
   //Configure nfc_cmd	       
   NFC_SET_COMMAND   
   //Read nfc_cmd until the command has been executed completely.     
   NFC_WAIT_COMMAND_FINISH 
   //Swith Main part
   cmd=NF_SWITCH_MAIN;	       
   //Configure nfc_cmd	       
   NFC_SET_COMMAND
   //Read nfc_cmd until the command has been executed completely.     
   NFC_WAIT_SWITCHMAIN_COMMAND_FINISH       
 
   
   ret = NANDCTL_Read_Status();
   
   return ret;
   
}
/******************************************************************************/
//  Description:   Erase a block of nandflash
//  Parameter:
//      block_addr:  in   the nand block address.
//	Return:     
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Erase_Block_S(uint32 block_addr)
{
   uint32 cmd,nfc_cmd;
   uint32 counter;
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
   ERR_NF_E ret; 
   
   //set start_addr and end_addr
   nandctl_ptr->start_addr0=block_addr;
   

   cmd=NF_BKERASE_ID;
   
   NFC_SET_COMMAND
   
   //Read nfc_cmd until the command has been executed completely.
   NFC_WAIT_COMMAND_FINISH  
          
   ret = NANDCTL_Read_Status();
   return ret;
}
/******************************************************************************/
//  Description:   Read main part and spare part of nand, ECC value can be get 
//                 if necessary. This function is used for large page nandflash.
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number  
//      pMBuf:       in   the address of Main part
//      pSBuf:       in   the address of Spare part
//      pEcc:        out  the address of ECC value
//	Return:     
//      ERR_NF_SUCCESS    Read nand successfully  
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_MS_Read_L(
                             uint32 page_addr,
                             uint8 index,
                             uint8 nSct,
                             uint8 *pMBuf,
                             uint8 *pSBuf,
                             uint8 *pEcc
                             )
{
   uint32 cmd,nfc_cmd;
   uint32 counter;   
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
#if defined(NAND_FDL_SC6800D)||defined(NAND_FDL_SC8800H)
   uint8 Ecc_val_in[16];
   uint8 Ecc_val_out[16];
   uint32 *pEcc_val=(uint32 *)Ecc_val_in;
#endif

#ifdef _NAND_CON_DEBUG
   SCI_ASSERT((NULL!=pMBuf)||(NULL!=pSBuf));
#endif   
   //set start_addr and end_addr
   nandctl_ptr->start_addr0 = (page_addr + index * 256)*(2-g_Nand_Para.buswidth);
   nandctl_ptr->end_addr0 = (page_addr + (index + nSct) * 256)*(2-g_Nand_Para.buswidth) - 1; 
   
   if(NULL!=pEcc)
   {

      cmd=NF_READ_1ST|(0x1<<21);
    	       
      //Configure nfc_cmd	       
      NFC_SET_COMMAND
   
      //Read nfc_cmd until the command has been executed completely.
      NFC_WAIT_COMMAND_FINISH
      
      switch(nSct)
      {
#if defined(NAND_FDL_SC6800D)||defined(NAND_FDL_SC8800H)
         case 1:
               *pEcc_val = nandctl_ptr->ecc0; 
               NANDCTL_Ecc_Trans(Ecc_val_in,Ecc_val_out,nSct);
	           pEcc[0] = Ecc_val_out[0];
	           pEcc[1] = Ecc_val_out[1];
	           pEcc[2] = Ecc_val_out[2];
	           break;
         case 2:
               pEcc_val[0] = nandctl_ptr->ecc0;
               pEcc_val[1] = nandctl_ptr->ecc1;
               NANDCTL_Ecc_Trans(Ecc_val_in,Ecc_val_out,nSct);
               //Put the ecc value into the spare buffer
               pEcc[0] = Ecc_val_out[0];
               pEcc[1] = Ecc_val_out[1];
               pEcc[2] = Ecc_val_out[2];
               pEcc[4] = Ecc_val_out[4];
               pEcc[5] = Ecc_val_out[5];
               pEcc[6] = Ecc_val_out[6];               
               break;  
         case 3:
               pEcc_val[0] = nandctl_ptr->ecc0;
               pEcc_val[1] = nandctl_ptr->ecc1;
               pEcc_val[2] = nandctl_ptr->ecc2;
               NANDCTL_Ecc_Trans(Ecc_val_in,Ecc_val_out,nSct);
               //Put the ecc value into the spare buffer
               pEcc[0] = Ecc_val_out[0];
               pEcc[1] = Ecc_val_out[1];
               pEcc[2] = Ecc_val_out[2];
               pEcc[4] = Ecc_val_out[4];
               pEcc[5] = Ecc_val_out[5];
               pEcc[6] = Ecc_val_out[6];
               pEcc[8] = Ecc_val_out[8];
               pEcc[9] = Ecc_val_out[9];
               pEcc[10] = Ecc_val_out[10];               
               break;
         case 4:
               pEcc_val[0] = nandctl_ptr->ecc0;
               pEcc_val[1] = nandctl_ptr->ecc1;
               pEcc_val[2] = nandctl_ptr->ecc2;
               pEcc_val[3] = nandctl_ptr->ecc3;
               NANDCTL_Ecc_Trans(Ecc_val_in,Ecc_val_out,nSct);
               //Put the ecc value into the spare buffer
               pEcc[0] = Ecc_val_out[0];
               pEcc[1] = Ecc_val_out[1];
               pEcc[2] = Ecc_val_out[2];
               pEcc[4] = Ecc_val_out[4];
               pEcc[5] = Ecc_val_out[5];
               pEcc[6] = Ecc_val_out[6];
               pEcc[8] = Ecc_val_out[8];
               pEcc[9] = Ecc_val_out[9];
               pEcc[10] = Ecc_val_out[10];
               pEcc[12] = Ecc_val_out[12];
               pEcc[13] = Ecc_val_out[13];
               pEcc[14] = Ecc_val_out[14];
               break;
#else
         case 1:
	           pEcc[0] = (uint8)(nandctl_ptr->ecc0 & 0xff);
	           pEcc[1] = (uint8)((nandctl_ptr->ecc0 & 0xff00)>>8);
	           pEcc[2] = (uint8)((nandctl_ptr->ecc0 & 0xff0000)>>16);
	           break;
         case 2:
	           pEcc[0] = (uint8)(nandctl_ptr->ecc0 & 0xff);
	           pEcc[1] = (uint8)((nandctl_ptr->ecc0 & 0xff00)>>8);
	           pEcc[2] = (uint8)((nandctl_ptr->ecc0 & 0xff0000)>>16);
	           pEcc[4] = (uint8)(nandctl_ptr->ecc1 & 0xff);
	           pEcc[5] = (uint8)((nandctl_ptr->ecc1 & 0xff00)>>8);
	           pEcc[6] = (uint8)((nandctl_ptr->ecc1 & 0xff0000)>>16);
	           
	           break;
         case 3:
	           pEcc[0] = (uint8)(nandctl_ptr->ecc0 & 0xff);
	           pEcc[1] = (uint8)((nandctl_ptr->ecc0 & 0xff00)>>8);
	           pEcc[2] = (uint8)((nandctl_ptr->ecc0 & 0xff0000)>>16);
	           pEcc[4] = (uint8)(nandctl_ptr->ecc1 & 0xff);
	           pEcc[5] = (uint8)((nandctl_ptr->ecc1 & 0xff00)>>8);
	           pEcc[6] = (uint8)((nandctl_ptr->ecc1 & 0xff0000)>>16);
	           pEcc[8] = (uint8)(nandctl_ptr->ecc2 & 0xff);
	           pEcc[9] = (uint8)((nandctl_ptr->ecc2 & 0xff00)>>8);
	           pEcc[10] = (uint8)((nandctl_ptr->ecc2 & 0xff0000)>>16);
	           
	           break;
         case 4:
	           pEcc[0] = (uint8)(nandctl_ptr->ecc0 & 0xff);
	           pEcc[1] = (uint8)((nandctl_ptr->ecc0 & 0xff00)>>8);
	           pEcc[2] = (uint8)((nandctl_ptr->ecc0 & 0xff0000)>>16);
	           pEcc[4] = (uint8)(nandctl_ptr->ecc1 & 0xff);
	           pEcc[5] = (uint8)((nandctl_ptr->ecc1 & 0xff00)>>8);
	           pEcc[6] = (uint8)((nandctl_ptr->ecc1 & 0xff0000)>>16);
	           pEcc[8] = (uint8)(nandctl_ptr->ecc2 & 0xff);
	           pEcc[9] = (uint8)((nandctl_ptr->ecc2 & 0xff00)>>8);
	           pEcc[10] = (uint8)((nandctl_ptr->ecc2 & 0xff0000)>>16);
	           pEcc[12] = (uint8)(nandctl_ptr->ecc3 & 0xff);
	           pEcc[13] = (uint8)((nandctl_ptr->ecc3 & 0xff00)>>8);
	           pEcc[14] = (uint8)((nandctl_ptr->ecc3 & 0xff0000)>>16);
               break;
         default:
               break;
#endif
      }	  

   }
   else
   { 
	    cmd=NF_READ_1ST;	    	       
	    //Configure nfc_cmd	       
	    NFC_SET_COMMAND	   
	    //Read nfc_cmd until the command has been executed completely.
	    NFC_WAIT_COMMAND_FINISH
   }
   	      	
   NandCopy((unsigned long *)NFC_MBUF_ADDR,(uint32 *)pMBuf,nSct*512);
   
   //set start_addr and end_addr
   nandctl_ptr->start_addr0 = (page_addr + 1024 + index*8 )*(2-g_Nand_Para.buswidth);
   nandctl_ptr->end_addr0 = (page_addr + 1024 + (index+nSct)*8 )*(2-g_Nand_Para.buswidth) - 1;
   
   cmd=NF_READ_1ST;	    	       
   //Configure nfc_cmd	       
   NFC_SET_COMMAND	   
   //Read nfc_cmd until the command has been executed completely.
   NFC_WAIT_COMMAND_FINISH
   
   NandCopy((unsigned long *)NFC_SBUF_ADDR,(uint32 *)pSBuf,nSct*16);      
   
   return ERR_NF_SUCCESS;

}
/******************************************************************************/
//  Description:   Read main part of nand, ECC value can be get 
//                 if necessary.
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number  
//      pMBuf:       in   the address of Main part
//      pEcc:        out  the address of ECC value
//	Return:     
//      ERR_NF_SUCCESS    Read nand successfully  
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_M_Read_L(
                            uint32 page_addr,
                            uint8 index,
                            uint8 nSct,
                            uint8 *pMBuf
                          )
{
   uint32 cmd,nfc_cmd;
   uint32 counter;   
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;

#ifdef _NAND_CON_DEBUG
   SCI_ASSERT(NULL!=pMBuf);
#endif   
   //set start_addr and end_addr
   nandctl_ptr->start_addr0 = (page_addr + index * 256)*(2-g_Nand_Para.buswidth);
   nandctl_ptr->end_addr0 = (page_addr + (index + nSct) * 256)*(2-g_Nand_Para.buswidth) - 1; 
   
   cmd=NF_READ_1ST;	    	       
   //Configure nfc_cmd	       
   NFC_SET_COMMAND	   
   //Read nfc_cmd until the command has been executed completely.
   NFC_WAIT_COMMAND_FINISH

   	      	
   NandCopy((unsigned long *)NFC_MBUF_ADDR,(uint32 *)pMBuf,nSct*512);   
   
   return ERR_NF_SUCCESS;
}
/******************************************************************************/
//  Description:   Read spare part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number  
//      pSBuf:       in   the address of Spare part
//	Return:     
//      ERR_NF_SUCCESS    Read nand successfully  
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_S_Read_L(
                             uint32 page_addr,
                             uint8 index,
                             uint8 nSct,
                             uint8 *pSBuf
                           )
{
   uint32 cmd,nfc_cmd;
   uint32 counter;   
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
   
#ifdef _NAND_CON_DEBUG
   SCI_ASSERT(NULL!=pSBuf);
#endif   
   //set start_addr and end_addr
   nandctl_ptr->start_addr0 = (page_addr + 1024 + index*8 )*(2-g_Nand_Para.buswidth);
   nandctl_ptr->end_addr0 = (page_addr + 1024 + (index+nSct)*8 )*(2-g_Nand_Para.buswidth) - 1;
   
   cmd=NF_READ_1ST;	    	       
   //Configure nfc_cmd	       
   NFC_SET_COMMAND	   
   //Read nfc_cmd until the command has been executed completely.
   NFC_WAIT_COMMAND_FINISH
   
   NandCopy((unsigned long *)NFC_SBUF_ADDR,(uint32 *)pSBuf,nSct*16);      
   
   return ERR_NF_SUCCESS;
}
/******************************************************************************/
//  Description:   Write main part and spare part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number  
//      pMBuf:       in   the address of Main part
//      pSBuf:       in   the address of Spare part
//      ecc_en:      in   the flag to enable ECC
//	Return:     
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_MS_Write_L(
                               uint32 page_addr,
                               uint8 index,
                               uint8 nSct,
                               uint8 *pMBuf,
                               uint8 *pSBuf,
                               uint8 ecc_en
                               )
{
   uint32 cmd,nfc_cmd;
   uint32 counter;   
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
   ERR_NF_E ret = ERR_NF_SUCCESS;
   uint8 Ecc_val_in[16];
   uint8 Ecc_val_out[16];
   uint32 *pEcc_val=(uint32 *)Ecc_val_in;
 
#ifdef _NAND_CON_DEBUG
   SCI_ASSERT((NULL!=pMBuf)||(NULL!=pSBuf));
#endif  
   //set start_addr and end_addr
   nandctl_ptr->start_addr0 = (page_addr + index * 256)*(2-g_Nand_Para.buswidth);
   nandctl_ptr->end_addr0 = (page_addr + (index + nSct) * 256)*(2-g_Nand_Para.buswidth) - 1; 
   
   if(1==ecc_en)
   {
       //Open ECC
       *(volatile uint32 *)NFC_ECC_EN =0x1;   
	   //Copy main data to Nand Buffer 
	   NandCopy((uint32 *)pMBuf,(uint32 *)NFC_MBUF_ADDR,nSct*512);		   
	   //Get ECC
	   switch(nSct)
       {
         case 1:
               pEcc_val[0] = nandctl_ptr->ecc0;
               NANDCTL_Ecc_Trans(Ecc_val_in,Ecc_val_out,nSct);
               //Put the ecc value into the spare buffer
               pSBuf[g_Nand_Para.eccpos] = Ecc_val_out[0];
               pSBuf[g_Nand_Para.eccpos+1] = Ecc_val_out[1];
               pSBuf[g_Nand_Para.eccpos+2] = Ecc_val_out[2];
              
               break;
         case 2:
               pEcc_val[0] = nandctl_ptr->ecc0;
               pEcc_val[1] = nandctl_ptr->ecc1;
               NANDCTL_Ecc_Trans(Ecc_val_in,Ecc_val_out,nSct);
               //Put the ecc value into the spare buffer
               pSBuf[g_Nand_Para.eccpos] = Ecc_val_out[0];
               pSBuf[g_Nand_Para.eccpos+1] = Ecc_val_out[1];
               pSBuf[g_Nand_Para.eccpos+2] = Ecc_val_out[2];
               pSBuf[g_Nand_Para.eccpos+16] = Ecc_val_out[4];
               pSBuf[g_Nand_Para.eccpos+17] = Ecc_val_out[5];
               pSBuf[g_Nand_Para.eccpos+18] = Ecc_val_out[6];               
               break;  
         case 3:
               pEcc_val[0] = nandctl_ptr->ecc0;
               pEcc_val[1] = nandctl_ptr->ecc1;
               pEcc_val[2] = nandctl_ptr->ecc2;
               NANDCTL_Ecc_Trans(Ecc_val_in,Ecc_val_out,nSct);
               //Put the ecc value into the spare buffer
               pSBuf[g_Nand_Para.eccpos] = Ecc_val_out[0];
               pSBuf[g_Nand_Para.eccpos+1] = Ecc_val_out[1];
               pSBuf[g_Nand_Para.eccpos+2] = Ecc_val_out[2];
               pSBuf[g_Nand_Para.eccpos+16] = Ecc_val_out[4];
               pSBuf[g_Nand_Para.eccpos+17] = Ecc_val_out[5];
               pSBuf[g_Nand_Para.eccpos+18] = Ecc_val_out[6];
               pSBuf[g_Nand_Para.eccpos+32] = Ecc_val_out[8];
               pSBuf[g_Nand_Para.eccpos+33] = Ecc_val_out[9];
               pSBuf[g_Nand_Para.eccpos+34] = Ecc_val_out[10];               
               break;
         case 4:
               pEcc_val[0] = nandctl_ptr->ecc0;
               pEcc_val[1] = nandctl_ptr->ecc1;
               pEcc_val[2] = nandctl_ptr->ecc2;
               pEcc_val[3] = nandctl_ptr->ecc3;
               NANDCTL_Ecc_Trans(Ecc_val_in,Ecc_val_out,nSct);
               //Put the ecc value into the spare buffer
               pSBuf[g_Nand_Para.eccpos] = Ecc_val_out[0];
               pSBuf[g_Nand_Para.eccpos+1] = Ecc_val_out[1];
               pSBuf[g_Nand_Para.eccpos+2] = Ecc_val_out[2];
               pSBuf[g_Nand_Para.eccpos+16] = Ecc_val_out[4];
               pSBuf[g_Nand_Para.eccpos+17] = Ecc_val_out[5];
               pSBuf[g_Nand_Para.eccpos+18] = Ecc_val_out[6];
               pSBuf[g_Nand_Para.eccpos+32] = Ecc_val_out[8];
               pSBuf[g_Nand_Para.eccpos+33] = Ecc_val_out[9];
               pSBuf[g_Nand_Para.eccpos+34] = Ecc_val_out[10];
               pSBuf[g_Nand_Para.eccpos+48] = Ecc_val_out[12];
               pSBuf[g_Nand_Para.eccpos+49] = Ecc_val_out[13];
               pSBuf[g_Nand_Para.eccpos+50] = Ecc_val_out[14];
               break;
         default:
               break;
       }	  
       //Close ECC
       *(volatile uint32 *)NFC_ECC_EN =0x0;      
   }
   else
   {
       //Copy main data to Nand Buffer 
	   NandCopy((uint32 *)pMBuf,(uint32 *)NFC_MBUF_ADDR,nSct*512);	
    
   }
   
   //Copy spare data to Nand Buffer
   NandCopy((uint32 *)pSBuf,(uint32 *)NFC_SBUF_ADDR,nSct*16);

   //Begin to write
   cmd=NF_WRITE_ID;
          
   NFC_SET_COMMAND
   
   //Read nfc_cmd until the command has been executed completely.
   NFC_WAIT_COMMAND_FINISH
                                                                                                                        
   ret = NANDCTL_Read_Status();
   if(ret==ERR_NF_SUCCESS)
   {
       //set spare start_addr and end_addr
       nandctl_ptr->start_addr0 = (page_addr + 1024 + index*8 )*(2-g_Nand_Para.buswidth);
        nandctl_ptr->end_addr0 = (page_addr + 1024 + (index+nSct)*8 )*(2-g_Nand_Para.buswidth) - 1;
       //Begin to write
       cmd=NF_WRITE_ID;
          
       NFC_SET_COMMAND
   
       //Read nfc_cmd until the command has been executed completely.
       NFC_WAIT_COMMAND_FINISH
       
       ret=NANDCTL_Read_Status();
   }
   return ret;
}
/******************************************************************************/
//  Description:   Write main part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number  
//      pMBuf:       in   the address of Main part
//	Return:     
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_M_Write_L(uint32 page_addr,uint8 index,uint8 nSct,uint8 *pMBuf)
{
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
   ERR_NF_E ret;
   uint32 cmd,nfc_cmd;
   uint32 counter;

#ifdef _NAND_CON_DEBUG
   SCI_ASSERT(NULL!=pMBuf);
#endif   
   //set start_addr and end_addr
   nandctl_ptr->start_addr0 = (page_addr + index * 256)*(2-g_Nand_Para.buswidth);
   nandctl_ptr->end_addr0 = (page_addr + (index + nSct) * 256)*(2-g_Nand_Para.buswidth) - 1; 
    
   //Copy main data to Nand Buffer 
   NandCopy((uint32 *)pMBuf,(uint32 *)NFC_MBUF_ADDR,nSct*512);	
  
   //Begin to write
   cmd=NF_WRITE_ID;
    	       
   NFC_SET_COMMAND
   
   //Read nfc_cmd until the command has been executed completely.
   NFC_WAIT_COMMAND_FINISH
    
   ret = NANDCTL_Read_Status();

   return ret;
}
/******************************************************************************/
//  Description:   Write spare part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number  
//      pSBuf:       in   the address of Spare part
//	Return:     
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_S_Write_L(uint32 page_addr,uint8 index,uint8 nSct,uint8 *pSBuf)
{
   uint32 cmd,nfc_cmd;
   uint32 counter;   
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
   ERR_NF_E ret;

#ifdef _NAND_CON_DEBUG
   SCI_ASSERT(NULL!=pSBuf);
#endif	   	   
   //set spare start_addr and end_addr
   nandctl_ptr->start_addr0 = (page_addr + 1024 + index*8 )*(2-g_Nand_Para.buswidth);
   nandctl_ptr->end_addr0 = (page_addr + 1024 + (index+nSct)*8 )*(2-g_Nand_Para.buswidth) - 1;

   //Copy spare data to Nand Buffer
   NandCopy((uint32 *)pSBuf,(uint32 *)NFC_SBUF_ADDR,nSct*16);
   
   //Begin to write
   cmd=NF_WRITE_ID;
          
   NFC_SET_COMMAND
   
   //Read nfc_cmd until the command has been executed completely.
   NFC_WAIT_COMMAND_FINISH
       
   ret = NANDCTL_Read_Status();
   
   return ret;
}
/******************************************************************************/
//  Description:   Erase a block of nandflash
//  Parameter:
//      block_addr:   in   the nand block address.
//	Return:     
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Erase_Block_L(uint32 block_addr)
{
   uint32 cmd,nfc_cmd;
   uint32 counter;
   NAND_CTL_REG_T  *nandctl_ptr=(NAND_CTL_REG_T  *)NAND_CTL_BASE;
   ERR_NF_E ret; 
   
   //set start_addr and end_addr
   nandctl_ptr->start_addr0=block_addr * (2-g_Nand_Para.buswidth);
   

   cmd=NF_BKERASE_ID;
   
   NFC_SET_COMMAND
   
   //Read nfc_cmd until the command has been executed completely.
   NFC_WAIT_COMMAND_FINISH  
          
   ret = NANDCTL_Read_Status();
   return ret;

}

LOCAL ERR_NF_E NANDCTL_Ecc_Trans(uint8 *pEccIn, uint8 *pEccOut,uint8 nSct)
{
     if(g_Nand_Para.buswidth==0)
     {
        //BUS_WIDTH = 8
        switch(nSct)
        {
          case 1:
                 pEccOut[0] = pEccIn[2]<<2 | pEccIn[3]>>6;
                 pEccOut[1] = pEccIn[1]<<2 | pEccIn[2]>>6;
                 pEccOut[2] = pEccIn[3]<<2 | pEccIn[1]>>6;
                 break;
          case 2:
                 pEccOut[0] = pEccIn[2]<<2 | pEccIn[3]>>6;
                 pEccOut[1] = pEccIn[1]<<2 | pEccIn[2]>>6;
                 pEccOut[2] = pEccIn[3]<<2 | pEccIn[1]>>6;
                 pEccOut[4] = pEccIn[6]<<2 | pEccIn[7]>>6;
                 pEccOut[5] = pEccIn[5]<<2 | pEccIn[6]>>6;
                 pEccOut[6] = pEccIn[7]<<2 | pEccIn[5]>>6;
                 break; 
          case 3:
                 pEccOut[0] = pEccIn[2]<<2 | pEccIn[3]>>6;
                 pEccOut[1] = pEccIn[1]<<2 | pEccIn[2]>>6;
                 pEccOut[2] = pEccIn[3]<<2 | pEccIn[1]>>6;
                 pEccOut[4] = pEccIn[6]<<2 | pEccIn[7]>>6;
                 pEccOut[5] = pEccIn[5]<<2 | pEccIn[6]>>6;
                 pEccOut[6] = pEccIn[7]<<2 | pEccIn[5]>>6;
                 pEccOut[8] = pEccIn[10]<<2 | pEccIn[11]>>6;
                 pEccOut[9] = pEccIn[9]<<2 | pEccIn[10]>>6;
                 pEccOut[10] = pEccIn[11]<<2 | pEccIn[9]>>6;
                 break;                 
          case 4:
                 pEccOut[0] = pEccIn[2]<<2 | pEccIn[3]>>6;
                 pEccOut[1] = pEccIn[1]<<2 | pEccIn[2]>>6;
                 pEccOut[2] = pEccIn[3]<<2 | pEccIn[1]>>6;
                 pEccOut[4] = pEccIn[6]<<2 | pEccIn[7]>>6;
                 pEccOut[5] = pEccIn[5]<<2 | pEccIn[6]>>6;
                 pEccOut[6] = pEccIn[7]<<2 | pEccIn[5]>>6;
                 pEccOut[8] = pEccIn[10]<<2 | pEccIn[11]>>6;
                 pEccOut[9] = pEccIn[9]<<2 | pEccIn[10]>>6;
                 pEccOut[10] = pEccIn[11]<<2 | pEccIn[9]>>6;
                 pEccOut[12] = pEccIn[14]<<2 | pEccIn[15]>>6;
                 pEccOut[13] = pEccIn[13]<<2 | pEccIn[14]>>6;
                 pEccOut[14] = pEccIn[15]<<2 | pEccIn[13]>>6;
                 break;          
          default:
                 break;         
        
        }
     }
     else
     {
        //BUS_WIDTH = 16
        switch(nSct)
        {
           case 1:
                 pEccOut[0] = pEccIn[2];
                 pEccOut[1] = pEccIn[1];
                 pEccOut[2] = pEccIn[3];
                 break;
           case 2:
                 pEccOut[0] = pEccIn[2];
                 pEccOut[1] = pEccIn[1];
                 pEccOut[2] = pEccIn[3];
                 pEccOut[4] = pEccIn[6];
                 pEccOut[5] = pEccIn[5];
                 pEccOut[6] = pEccIn[7];
                 break;
           case 3:
                 pEccOut[0] = pEccIn[2];
                 pEccOut[1] = pEccIn[1];
                 pEccOut[2] = pEccIn[3];
                 pEccOut[4] = pEccIn[6];
                 pEccOut[5] = pEccIn[5];
                 pEccOut[6] = pEccIn[7];
                 pEccOut[8] = pEccIn[10];
                 pEccOut[9] = pEccIn[9];
                 pEccOut[10] = pEccIn[11];
                 break;
           case 4:
                 pEccOut[0] = pEccIn[2];
                 pEccOut[1] = pEccIn[1];
                 pEccOut[2] = pEccIn[3];
                 pEccOut[4] = pEccIn[6];
                 pEccOut[5] = pEccIn[5];
                 pEccOut[6] = pEccIn[7];
                 pEccOut[8] = pEccIn[10];
                 pEccOut[9] = pEccIn[9];
                 pEccOut[10] = pEccIn[11];
                 pEccOut[12] = pEccIn[14];
                 pEccOut[13] = pEccIn[13];
                 pEccOut[14] = pEccIn[15];
                 break;
           default:
                 break;
                 
        }
     }

     return ERR_NF_SUCCESS;
}

