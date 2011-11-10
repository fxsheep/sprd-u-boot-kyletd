/******************************************************************************
 ** File Name:                                                                *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         9/1/2003                                                    *
 ** Copyright:    2002 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 9/1/2003      Daniel.Ding     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sdram_init.h"

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
//typedef unsigned long int	uint32;

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
/*--------------------------- Local Data ------------------------------------*/
/*--------------------------- Global Data -----------------------------------*/
/*--------------------------- External Data ---------------------------------*/
#if defined (NAND_FDL_SC8800H)
extern void MMU_Init(void);
#endif
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Use this function to init chip for all hardware setting ;
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void DRV_Set(void)
{
#if defined (NAND_FDL_SC6800D)
    //dclk_drv.
	*(volatile uint32 *)0x8c0000ac |= 0x3010;
	//EMCS0/1_DRV,EMDQM0-4_DRV
	*(volatile uint32 *)0x8c00006c |= 0xfff0;
	//EMCKE0_drv
	*(volatile uint32 *)0x8c000074 |= 0xc000;
	//EMA0_DRV - EMA12_DRV
	*(volatile uint32 *)0x8c000088 = 0xFFFF;
	*(volatile uint32 *)0x8c00008c = 0x3FF;
	*(volatile uint32 *)0x8c000090 = 0xFFFF;
	//EMD0_drv - EMD15_drv
	*(volatile uint32 *)0x8c000094 = 0xFFFF;
	*(volatile uint32 *)0x8c000098 = 0xFFFF;

	*(volatile uint32 *)0x8c0000b0 |= 0xFFC0;
	
	*(volatile uint32 *)0x8c0000b4 |= 0xFFFF;
	
	*(volatile uint32 *)0x8c0000b8 |= 0xFFFF;
#elif defined(NAND_FDL_SC8800H)
    uint32 i;
    for( i=0x8C0000EC; i<=0x8C0001F0; i+=4 )
	{        
			*(volatile uint32 *)i= 0x31;		//sdram_mode
	}  
	// If use SDRAM/DDR, EMBA[1]'S default function is NOR-FLASH'S reset£¬so we should change it
	*(volatile uint32 *)0x8C0001A0 |= BIT_6;
#else
	    //increase address bus drv
	*(volatile uint32 *)0x8c00006c |= (0x3f<<8);   //BL, BH, CS0
	*(volatile uint32 *)0x8c000074 |= (0x3<<14);   //CKE
	*(volatile uint32 *)0x8c000078 |= 0x30;        //EMA24
	*(volatile uint32 *)0x8c00007c |= 0xf;         //EMA22,EMA23
	*(volatile uint32 *)0x8c000088 = 0xffff;       //EMA0-7
	*(volatile uint32 *)0x8c00008c = 0xffff;       //EMA8-15
	*(volatile uint32 *)0x8c000090 = 0xffff;       //EMA16-21, OE, WE
	
	//increase data bus drv	
	*(volatile uint32 *)0x8c000094 = 0xffff;	   //EMD0-7
	*(volatile uint32 *)0x8c000098 = 0xffff;	   //EMD8-15
	
	//increase clock mcu drv		
	*(volatile uint32 *)0x8c0000ac |= 3<<12;	   //CLKMCU
#endif
}
/*****************************************************************************/
//  Description:    Use this function to init chip for all hardware setting ;
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void Chip_Init (void)
{
	DRV_Set();

	SDRAM_Init(80);
	
#if defined (NAND_FDL_SC8800H) 
    MMU_Init(0);
#endif
}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 

