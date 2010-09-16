/******************************************************************************
 ** File Name:      sc_fdl_reg.h                                                  *
 ** Author:         Jimmy.Jia                                                 *
 ** DATE:           07/03/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                 Register header file for all spreadtrum chip			      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07-03-2003     Jimmy.Jia	    Create.                                   *
 ******************************************************************************/
#ifndef _SC_FDL_REG_H_
#define _SC_FDL_REG_H_


/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */    

#if defined NAND_FDL_SC6600R
	#include "sc6600r_reg.h"
#elif defined NAND_FDL_SC6800D
	#include "sc6800_reg.h"
#elif defined NAND_FDL_SC6600I
	#include "sc6600m_reg.h"
#elif defined NAND_FDL_SC8800D
	#include"sc8800_reg.h"	
#elif defined NAND_FDL_SC8800H
	#include"sc8800h_reg.h"	
#else
#error choice a platform please!
#endif

#endif  //_SC_REG_H_

