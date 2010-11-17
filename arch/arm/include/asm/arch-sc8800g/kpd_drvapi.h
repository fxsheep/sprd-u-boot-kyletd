/******************************************************************************
 ** File Name:      kpd_drvapi.h                                                 *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           09/10/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/10/2002     Xueliang.Wang    Create.                                   *
 ** 19/11/2004     Benjamin.Wang    Modify because of the keymap's new define.      *
 ******************************************************************************/

#ifndef _KPD_DRVAPI_H_
#define _KPD_DRVAPI_H_


// Keypad constant.
#define TB_KPD_CONST_BASE       0x80
#define TB_KPD_RELEASED         TB_KPD_CONST_BASE
#define TB_KPD_PRESSED          (TB_KPD_CONST_BASE + 1)
#define TB_KPD_INVALID_KEY      0x0FFFF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
extern const uint16    keymap[];

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function initialize keypad.
//                  1. Enable keypad device.
//                  2. Enable keypad interrupt.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 KPD_Init (void);

/*****************************************************************************/
//  Description:    This function close keypad.
//                  1. Disable keypad device.
//                  2. Disable keypad interrupt.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void KPD_Close (void);

/*below are removed from driver_export.c, wait clean up*/
/*****************************************************************************/
//  Description:    check pbutton's state
//  Global resource dependence:
//  Author:         Jimmy.Jia
//  Note:           return SCI_TRUE if pressed, else return SCI_FALSE
/*****************************************************************************/
uint32 CheckPowerButtonState (void);

/*****************************************************************************/
//  Description:    check if pbutton is released
//  Global resource dependence:
//  Author:         Jimmy.Jia
//  Note:           return SCI_TRUE if released, else return SCI_FALSE
/*****************************************************************************/
uint32  IsPowerButtonReleased (void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _KPD_DRVAPI_H_
