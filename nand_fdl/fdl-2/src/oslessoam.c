/*****************************************************************************/
/*                                                                           */
/* PROJECT : AnyStore II                                                     */
/* MODULE  : XSR OAM                                                         */
/* NAME    : OSLess OAM                                                      */
/* FILE    : OSLessOAM.cpp                                                   */
/* PURPOSE : This file contain the OS Adaptation Modules for OSless platform */
/*           such as BootLoader                                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*          COPYRIGHT 2003-2004 SAMSUNG ELECTRONICS CO., LTD.                */
/*                          ALL RIGHTS RESERVED                              */
/*                                                                           */
/*   Permission is hereby granted to licensees of Samsung Electronics        */
/*   Co., Ltd. products to use or abstract this computer program for the     */
/*   sole purpose of implementing a product based on Samsung                 */
/*   Electronics Co., Ltd. products. No other rights to reproduce, use,      */
/*   or disseminate this computer program, whether in part or in whole,      */
/*   are granted.                                                            */
/*                                                                           */
/*   Samsung Electronics Co., Ltd. makes no representation or warranties     */
/*   with respect to the performance of this computer program, and           */
/*   specifically disclaims any responsibility for any damages,              */
/*   special or consequential, connected with the use of this program.       */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* REVISION HISTORY                                                          */
/*                                                                           */
/*   05-AUG-2003 [SongHo Yoon]  : first writing                              */
/*   07-AUG-2003 [Janghwan Kim] : added and modified source code to compile  */
/*   09-AUG-2003 [Janghwan Kim] : added OAM_Memcpy, OAM_Memset, OAM_Memcmp   */
/*   19-JAN-2003 [SongHo Yoon]  : added OAM_USE_STDLIB definition            */
/*   19-JAN-2003 [SongHo Yoon]  : fixed a bug in OAM_AcquireSM()             */
/*                                change the return value (FALSE32 -> TRUE32)*/
/*                                                                           */
/*****************************************************************************/
#include "xsrtypes.h"
#include "oam.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*****************************************************************************/
/* Global variables definitions                                              */
/*****************************************************************************/

/*****************************************************************************/
/* Local #defines                                                            */
/*****************************************************************************/
#define		LOCAL_MEM_SIZE		((800 * 1024) / sizeof(UINT32))

#define		OAM_USE_STDLIB

/*****************************************************************************/
/* Local typedefs                                                            */
/*****************************************************************************/

/*****************************************************************************/
/* Local constant definitions                                                */
/*****************************************************************************/

/*****************************************************************************/
/* Static variables definitions                                              */
/*****************************************************************************/
static UINT32  aMemBuf[LOCAL_MEM_SIZE];
static UINT32  nMallocPtr = 0;

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      OAM_Malloc                                                           */
/* DESCRIPTION                                                               */
/*      This function allocates memory for XSR                               */
/*                                                                           */
/* PARAMETERS                                                                */
/*      nSize       [IN]                                                     */
/*            Size to be allocated                                           */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      Pointer of allocated memory                                          */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called by function that wants to use memory         */
/*                                                                           */
/*****************************************************************************/
VOID *
OAM_Malloc(UINT32 nSize)
{
	UINT32  nAlignSize;

	nAlignSize = nSize / sizeof(UINT32);
	if (nSize % sizeof(UINT32))
		nAlignSize++;

	nMallocPtr += nAlignSize;
 	if (nMallocPtr > LOCAL_MEM_SIZE)
 		return NULL;

	return (VOID *) &(aMemBuf[nMallocPtr - nAlignSize]);
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_Free                                                                 */
/* DESCRIPTION                                                               */
/*      This function free memory that XSR allocated                         */
/*                                                                           */
/* PARAMETERS                                                                */
/*  pMem        Pointer to be free                                           */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      none                                                                 */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called by function that wants to free memory        */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
VOID
OAM_Free(VOID  *pMem)
{
//	_OAM_ASSERT(0);
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_Memcpy                                                               */
/* DESCRIPTION                                                               */
/*      This function copies data from source to destination.                */
/*                                                                           */
/* PARAMETERS                                                                */
/*  pDst        Destination array Pointer to be copied                       */
/*  pSrc        Source data allocated Pointer                                */
/*  nLen        length to be copied                                          */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      none                                                                 */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called by function that wants to copy source buffer */
/*  to destination buffer.                                                   */
/*                                                                           */
/*****************************************************************************/
VOID
OAM_Memcpy(VOID *pDst, VOID *pSrc, UINT32 nLen)
{
#if defined(OAM_USE_STDLIB)
	memcpy(pDst, pSrc, nLen);
#else /* OAM_USE_STDLIB */
	register INT32 	nCnt;
	register UINT8	*pD8, *pS8;
	register INT32	nL = nLen;
	register UINT32	*pD32, *pS32;

	pD8 = (UINT8*)pDst;
	pS8 = (UINT8*)pSrc;
	
	if ( ((INT32)pD8 % sizeof(UINT32)) == ((INT32)pS8 % sizeof(UINT32)) )
	{
		while ( (INT32)pD8 % sizeof(UINT32) )
		{
			*pD8++ = *pS8++;
			nL--;

			if( nL <= 0 )
			    return;
		}
	
		pD32 = (UINT32*)pD8;
		pS32 = (UINT32*)pS8;
		
		for (nCnt = 0; nCnt <(INT32)(nL / sizeof(UINT32)); nCnt++)
			*pD32++ = *pS32++;
	
		pD8 = (UINT8*)pD32;
		pS8 = (UINT8*)pS32;
			
		while( nL % sizeof(UINT32) )
		{
			*pD8++ = *pS8++;
			nL--;
		}
	}
	else
	{
		for( nCnt = 0; nCnt < nL; nCnt++)
			*pD8++ = *pS8++;
	}
#endif /* OAM_USE_STDLIB */		
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_Memset                                                               */
/* DESCRIPTION                                                               */
/*      This function set data of specific buffer.                           */
/*                                                                           */
/* PARAMETERS                                                                */
/*  pSrc        Source data allocated Pointer                                */
/*  nV          Value to be setted                                           */
/*  nLen        length to be setted                                          */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      none                                                                 */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called by function that wants to set source buffer  */
/*  own data.                                                                */
/*                                                                           */
/*****************************************************************************/
VOID
OAM_Memset(VOID *pDst, UINT8 nV, UINT32 nLen)
{
#if defined(OAM_USE_STDLIB)
	memset(pDst, nV, nLen);
#else /* OAM_USE_STDLIB */
	register UINT8	*pD8;
	register UINT32	*pD32;
	register UINT8	nV8 = nV;
	register UINT32	nV32 = (UINT32)( nV << 24 | nV << 16 | nV << 8 | nV ); 
	register INT32	nL = (INT32)nLen;
	register UINT32 nCnt;

	pD8 = (UINT8*)pDst;
	
	while ( (INT32)pDst % sizeof(UINT32) )
	{
		*pD8++ = nV8;
		nL--;

		if( nL <= 0 )
		    return;
	}
	
	pD32 = (UINT32*)pD8;
	for (nCnt = 0; nCnt <(INT32)(nL / sizeof(UINT32)); nCnt++)
		*pD32++ = nV32;

	pD8 = (UINT8*)pD32;	
	while( nL % sizeof(UINT32) )
	{
		*pD8++ = nV8;
		nL--;
	}
#endif /* OAM_USE_STDLIB */
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_Memcmp                                                               */
/* DESCRIPTION                                                               */
/*      This function compare data of two buffer.                            */
/*                                                                           */
/* PARAMETERS                                                                */
/*  pSrc        Source data allocated Pointer                                */
/*  pDst        Destination array Pointer to be compared                     */
/*  nLen        length to be compared                                        */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      none                                                                 */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called by function that wants to compare source     */
/*  buffer and destination buffer.                                           */
/*                                                                           */
/*****************************************************************************/
INT32
OAM_Memcmp(VOID  *pSrc, VOID  *pDst, UINT32 nLen)
{
#if defined(OAM_USE_STDLIB)
	return memcmp(pSrc, pDst, nLen);
#else /* OAM_USE_STDLIB */
	UINT8 *pS1 = (UINT8 *)pSrc;
	UINT8 *pD1 = (UINT8 *)pDst;

	while (nLen--)
	{
		if (*pS1 != *pD1)
			return (*pS1 - *pD1);
        pS1++;
        pD1++;
	}

	return 0;
#endif /* OAM_USE_STDLIB */
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_CreateSM                                                             */
/* DESCRIPTION                                                               */
/*      This function creates semaphore.                                     */
/*                                                                           */
/* PARAMETERS                                                                */
/*  pHandle        Handle of semaphore                                       */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      If this function creates semaphore successfully, it returns TRUE32.  */
/*      else it returns FALSE32.                                             */
/* NOTES                                                                     */
/*      This function is called by function that wants to create semaphore   */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
BOOL32
OAM_CreateSM(SM32 *pHandle)
{
	return TRUE32;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_DestroySM                                                            */
/* DESCRIPTION                                                               */
/*      This function destroys semaphore.                                    */
/*                                                                           */
/* PARAMETERS                                                                */
/*  nHandle        Handle of semaphore to be destroyed                       */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      If this function destroys semaphore successfully, it returns TRUE32. */
/*      else it returns FALSE32.                                             */
/* NOTES                                                                     */
/*      This function is called by function that wants to destroy semaphore  */
/*                                                                           */
/*****************************************************************************/
BOOL32
OAM_DestroySM(SM32 nHandle)
{
	return TRUE32;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_AcquireSM                                                            */
/* DESCRIPTION                                                               */
/*      This function acquires semaphore.                                    */
/*                                                                           */
/* PARAMETERS                                                                */
/*  nHandle        Handle of semaphore to be acquired                        */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      If this function acquires semaphore successfully, it returns TRUE32. */
/*      else it returns FALSE32.                                             */
/* NOTES                                                                     */
/*      This function is called by function that wants to acquire semaphore  */
/*                                                                           */
/*****************************************************************************/
BOOL32
OAM_AcquireSM(SM32 nHandle)
{
	return TRUE32;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_ReleaseSM                                                            */
/* DESCRIPTION                                                               */
/*      This function releases semaphore.                                    */
/*                                                                           */
/* PARAMETERS                                                                */
/*      nHandle    [IN]                                                      */
/*            Handle of semaphore to be released                             */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      If this function releases semaphore successfully, it returns TRUE32. */
/*      else it returns FALSE32.                                             */
/* NOTES                                                                     */
/*      This function is called by function that wants to release semaphore  */
/*                                                                           */
/*****************************************************************************/
BOOL32
OAM_ReleaseSM(SM32 nHandle)
{
	return TRUE32;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_Pa2Va                                                                */
/* DESCRIPTION                                                               */
/*      This function gets virtual address for NAND device physical address  */
/*                                                                           */
/* PARAMETERS                                                                */
/*  nPAddr        physical address of NAND device                            */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      Virtual address of NAND device that Symbian OS using                 */
/*                                                                           */
/* NOTES                                                                     */
/*      This function makes virtual address for NAND device when system      */
/*  initialized                                                              */
/*                                                                           */
/*****************************************************************************/
UINT32
OAM_Pa2Va(UINT32 nPAddr)
{
    return (nPAddr);
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_InitInt                                                              */
/* DESCRIPTION                                                               */
/*      This function initialize interrupt for NAND device                   */
/*                                                                           */
/* PARAMETERS                                                                */
/*  Caller        Class that this function called                            */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      none                                                                 */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called in Media driver interface layer              */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

VOID
OAM_InitInt(VOID *Caller)
{
}


/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_BindInt                                                              */
/* DESCRIPTION                                                               */
/*      This function binds interrupt for NAND device                        */
/*                                                                           */
/* PARAMETERS                                                                */
/*  none                                                                     */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      none                                                                 */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called in Media driver interface layer              */
/*                                                                           */
/*****************************************************************************/
VOID
OAM_BindInt(VOID)
{
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_EnableInt                                                            */
/* DESCRIPTION                                                               */
/*      This function enables interrupt for NAND device                      */
/*                                                                           */
/* PARAMETERS                                                                */
/*  none                                                                     */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      none                                                                 */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called in Media driver interface layer              */
/*                                                                           */
/*****************************************************************************/
VOID
OAM_EnableInt(VOID)
{    
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_DisableInt                                                           */
/* DESCRIPTION                                                               */
/*      This function disables interrupt for NAND device                     */
/*                                                                           */
/* PARAMETERS                                                                */
/*  none                                                                     */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      none                                                                 */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called in Media driver interface layer              */
/*                                                                           */
/*****************************************************************************/
VOID
OAM_DisableInt(VOID)
{    
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_ClearInt                                                             */
/* DESCRIPTION                                                               */
/*      This function clear interrupt for NAND device                        */
/*                                                                           */
/* PARAMETERS                                                                */
/*  none                                                                     */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      none                                                                 */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called in Media driver interface layer              */
/*                                                                           */
/*****************************************************************************/
VOID
OAM_ClearInt(VOID)
{    
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_ResetTimer                                                           */
/* DESCRIPTION                                                               */
/*      This function reset timer for using asynchronous write operation     */
/*                                                                           */
/* PARAMETERS                                                                */
/*  none                                                                     */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      none                                                                 */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called in STL layer                                 */
/*                                                                           */
/*****************************************************************************/

static UINT32 nTimerCounter = 0;
static UINT16 nTimerPrevCnt = 0;

VOID 
OAM_ResetTimer(VOID)
{    
    nTimerPrevCnt = 0;
    nTimerCounter = 0;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_GetTime                                                              */
/* DESCRIPTION                                                               */
/*      This function returns cureent timer value                            */
/*                                                                           */
/* PARAMETERS                                                                */
/*  none                                                                     */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      Current timer value                                                  */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called in STL layer                                 */
/*                                                                           */
/*****************************************************************************/
UINT32 
OAM_GetTime(VOID)
{
    return nTimerCounter++;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_Debug                                                                */
/* DESCRIPTION                                                               */
/*      This function prints debug message                                   */
/*                                                                           */
/* PARAMETERS                                                                */
/*  pFmt    data to be printed                                               */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      none                                                                 */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is used when system can not support debug              */
/*  print function                                                           */
/*                                                                           */
/*****************************************************************************/
VOID
OAM_Debug(VOID  *pFmt, ...)
{
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_WaitNMSec                                                            */
/* DESCRIPTION                                                               */
/*      This function waits N msec                                           */
/*                                                                           */
/* PARAMETERS                                                                */
/*  nNMSec        msec time for waiting                                      */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      none                                                                 */
/*                                                                           */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
VOID
OAM_WaitNMSec(UINT32 nNMSec)
{
	static volatile UINT32 nCnt1;
	static volatile UINT32 nCnt2;


	for (nCnt1 = 0; nCnt1 < nNMSec; nCnt1++)
	{
		for (nCnt2 = 0; nCnt2 < 1000; nCnt2++)
		{
		}
	}
}
