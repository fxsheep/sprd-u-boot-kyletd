/******************************************************************************
 ** File Name:    gea_reg_v3.h                                         *
 ** Author:       mingwei.zhang                                                 *
 ** DATE:         06/11/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/11/2010    mingwei.zhang   Create.                                     *
 ******************************************************************************/
#ifndef _GEA_REG_V3_H_
#define _GEA_REG_V3_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/

/*----------GEA Interface Control Registers----------*/         ///mingweiflag? not check
#define GEA_KST_START                   (GEA_BASE + 0x0000)
#define GEA_MODE                        (GEA_BASE + 0x0C80)
#define GEA_WORD_LEN                    (GEA_BASE + 0x0C84)
#define GEA_KC0                         (GEA_BASE + 0x0C88)
#define GEA_KC1                         (GEA_BASE + 0x0C8C)
#define GEA_KC2                         (GEA_BASE + 0x0C90)
#define GEA_KC3                         (GEA_BASE + 0x0C94)
#define GEA_INPUT0                      (GEA_BASE + 0x0C98)
#define GEA_INPUT1                      (GEA_BASE + 0x0C9C)
#define GEA_START                       (GEA_BASE + 0x0CA0)
#define GEA_POLL                        (GEA_BASE + 0x0CA4)

#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_GEA_REG_V3_H_





