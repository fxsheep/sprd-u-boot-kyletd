/*****************************************************************************/
/*                                                                           */
/* PROJECT : AnyStore II                                                     */
/* MODULE  : XSR FIL                                                         */
/* NAME    : SW Error Correction Code (Hamming Algorithm) header file        */
/* FILE    : SWEccNew.h                                                      */
/* PURPOSE : Definitions and Declarations of SWECC                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        COPYRIGHT 2003-2004, SAMSUNG ELECTRONICS CO., LTD.                 */
/*                      ALL RIGHTS RESERVED                                  */
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
/* - 27/DEC/2002 [Kwangyoon Lee] : first writing                             */
/* - 15/JUL/2003 [SeWook Na] : code modification                             */
/* - 11/AUG/2003 [Janghwan Kim] : code modification                          */
/* - 02/OCT/2003 [Janghwan Kim] : reorganization                             */
/*                                                                           */
/*****************************************************************************/

#ifndef _SWECC_H_
#define _SWECC_H_

/*****************************************************************************/
/* Common Constant Definition                                                */
/*****************************************************************************/
#define M_AREA                       0
#define S_AREA                       1


/*****************************************************************************/
/* ECC Data Strcutuers                                                       */
/*****************************************************************************/
#define SWECC_E_ERROR                1    /* ECC error                       */
#define SWECC_N_ERROR                0    /* no error                        */
#define SWECC_C_ERROR               -1    /* one bit data error              */
#define SWECC_U_ERROR               -2    /* uncorrectable error             */

#define BW_X08                       LLD_BW_X08 /* 0 */
#define BW_X16                       LLD_BW_X16 /* 1 */

/*****************************************************************************/
/* ECC External Function Declarations                                        */
/*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
VOID    ECC_Gen  (UINT8 *pEcc,
                  UINT8 *pBuf,  UINT8  nArea, UINT8 nBW);
INT32   ECC_Comp (UINT8 *pEcc1, UINT8 *pEcc2,
                  UINT8 *pBuf,  UINT8  nArea, UINT8 nBW);
*/
VOID  ECC_GenM (UINT8 *pEcc,  UINT32 *pBuf, UINT8 nBW);
VOID  ECC_GenS (UINT8 *pEcc,  UINT8  *pBuf, UINT8 nBW);

INT32 ECC_CompM (UINT8 *pEcc1, UINT8 *pEcc2, UINT8 *pBuf, UINT8 nBW);
INT32 ECC_CompS (UINT8 *pEcc1, UINT8 *pEcc2, UINT8 *pBuf, UINT8 nBW);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _SWECC_H_ */
