/******************************************************************************
 ** File Name:       lcd.h                                                    *
 ** Author:          Jim zhang                                                *
 ** DATE:            08/21/2003                                               *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE              NAME             DESCRIPTION                            *
 ** 08/21/2003     Jim zhang        Create.                                   *
 ******************************************************************************/


#ifndef _LCD_CONTROLLER_H_
#define _LCD_CONTROLLER_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "lcdc_data_type.h"



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/

//TV mode
typedef enum
{
    TV_PAL_LINE_BY_LINE = 0  ,
    TV_PAL_INTERLEAVING_LINE ,
    TV_NTSC_LINE_BY_LINE     ,
    TV_NTSC_INTERLEAVING_LINE
}
TV_MODE_E;

/*
 * Funtion pointer struct
 */


/**---------------------------------------------------------------------------*
 **                         Constant Variables
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Prototypes
 **---------------------------------------------------------------------------*/


/******************************************************************************/
/*            LCDC Controller Driver Interfaces                               */
/******************************************************************************/
/**********************************************************************************************************/
// Description: set LCDC work mode and lcd infomation
// Global resource dependence: g_lcdc_type
// Author: Jianping.wang
// Note:
//      p_lcdc - pointer to info structure
/**********************************************************************************************************/
PUBLIC void LCDC_SetLcdType (LCDC_TYPE_T *lcdc_ptr, uint32 lcd_cs);
/**********************************************************************************************************/
// Description: set lcd display block information
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      operation successes,provided use correct access mask
//      input:
//          p_blk - pointer to display block information
//          p_mask - pointer to block access mask
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_SetLcdBlock (LCD_BLOCK_T *blk_ptr,uint32 *mask_ptr);
/**********************************************************************************************************/
// Description: initialize LCDC
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC int32 LCDC_Init (void);
/**********************************************************************************************************/
// Description: configure LCDC
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      input:
//          spec_ptr - pointer to LCD information struction
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_Configure (LCDC_SPEC_T *spec_ptr);
/**********************************************************************************************************/
// Description: get LCDC configure information that is related with LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      input:
//          none
//      output:
//          p_spec - pointer to information structure
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_GetInfo (LCDC_INFO_T *info_ptr);
/**********************************************************************************************************/
// Description: send the command and data to LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      input:
//          cmd - lcd command
//          lcd_data - lcd command data
//          lcd_id - only is used for lcdc mcu mode
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_SendCmdData (uint32 cmd,uint32 lcd_data,uint32 lcd_id);
/**********************************************************************************************************/
// Description: send command to lcd
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      lcd_id parameter is used when lcdc mcu mode
//      input:
//          cmd - lcd command
//          lcd_id - LCD ID
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_SendCmd (uint32 cmd,uint32 lcd_id);
/**********************************************************************************************************/
// Description: send display data to LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      this interface is used in lcdc mcu mode
//      input:
//          lcd_data - display data
//          lcd_id - LCD ID
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_SendData (uint32 lcd_data,uint32 lcd_id);
/**********************************************************************************************************/
// Description: get LCD index register value
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      lcd_id parameter is used when lcdc mcu mode
//      input:
//          cmd - LCD command
//          lcd_id - LCD ID
//      output:
//          data_ptr - pointer to data that is LCD index register value
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_ReadLcdIndex (uint32 *data_ptr,uint32 cmd,uint32 lcd_id);
/**********************************************************************************************************/
// Description: start LCDC to write display data to LCD or TV
// Global resource dependence: g_lcdc_mode
// Author: Jianping
// Note:
//      lcd_id parameter is used when lcdc mcu mode
//      input:
//          lcd_width - lcd width
//          lcd_height - lcd height
//          lcd_id - LCD ID
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_TransCmd (uint16 lcd_width,uint16 lcd_height,uint32 lcd_cs);
/**********************************************************************************************************/
// Description:
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_GetConfigInfo (LCDC_CONFIG_T *cfg_ptr);
/**********************************************************************************************************/
// Description:
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_SetConfig (LCDC_CONFIG_T *cfg_ptr);
/**********************************************************************************************************/
// Description: initialize LCDC TV mode
// Global resource dependence: g_lcd_background
// Author: Jianping.wang
// Note:
//      lcdc tv mode is prior to mcu mode and rgb mode
/**********************************************************************************************************/
PUBLIC int32 LCDC_StartTv (uint32 tv_mode);
/**********************************************************************************************************/
// Description: close LCDC TV mode,resume to LCDC rgb mode or mcu mode
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_CloseTv (void);
/**********************************************************************************************************/
// Description: reset LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      delay_ms parameter is used in lcdc mcu mode
//      input:
//          delay_ms - delay time(ms)
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_ResetLcd (uint32 delay_ms);
/**********************************************************************************************************/
// Description: select lcdc output data flow
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      this function is used in lcdc rgb mode
//      feed_back - 1:LCDC output data to ISP module
//                 0:LCDC don't output data to ISP
//      disable_lcdout - 1:disable lcdc output to lcd
//                       0:enable lcdc output to lcd
/**********************************************************************************************************/
PUBLIC void LCDC_ConfigDataFlow (uint32 feed_back,uint32 disable_lcdout);
/**********************************************************************************************************/
// Description: adjust transfer timing
// Global resource dependence:
// Author: Jianping.wang
// Note:
//
/**********************************************************************************************************/
PUBLIC int32 LCDC_ChangePulseWidth (LCD_MODE_T mode, uint32 cs_id, LCD_TIMING_U timing_para);
/**********************************************************************************************************/
// Description: close LCDC
// Global resource dependence:
// Author: jianping.wang
// Note:
//      flag - 1:disable lcdc;0:enable lcdc
//---------------------------------------------------------------------------------------------------------
PUBLIC void LCDC_Close (uint32 flag);
/**********************************************************************************************************/
// Description: set color correction matrix in rgb mode and mcu mode;
//              or set rgb to yuv conversion matrix in tv mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      matrix_ptr - pointer to the matrix that is color correct matrix,or rgb to yuv conversion matrix
/**********************************************************************************************************/
PUBLIC void LCDC_SetColorMatrix (COLORMATRIX_T *matrix_ptr);
/**********************************************************************************************************/
// Description: updata tv display
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_RefreshTv (void);
/**********************************************************************************************************/
// Description: start LCDC,use background refresh LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_Start (void);
/**********************************************************************************************************/
// Description: Update LCDC Register setting
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_UpdateRegisters (void);
/**********************************************************************************************************/
// Description: set LCD background color
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_SetBackground (uint32 color);
/**********************************************************************************************************/
// Description: disable use display block
// Global resource dependence:  g_lcd_blk
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_DisableLcdBlock (void);
/**********************************************************************************************************/
// Description: software reset LCDC and TV Encoder
// Global resource dependence:
// Author: Jianping.wang
// Note:wirte 1-0
/**********************************************************************************************************/
PUBLIC void LCDC_Reset (void);
/**********************************************************************************************************/
// Description: Check if LCDC finish one brush LCD operation or not
// Global resource dependence:
// Author: Jianping.wang
// Note:
//     input  - none
//     output - none
//     return - 1:finish brush operation
//              0:don't finish brush operation
/**********************************************************************************************************/
PUBLIC BOOLEAN LCDC_CheckBrushLcdStatue (void);
/**********************************************************************************************************/
// Description:
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_ClearStatus (void);
/**********************************************************************************************************/
// Description:
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          is_open_rst - 1:open rst pin of lcd
//                        0:close rst pin of lcd
/**********************************************************************************************************/
PUBLIC void LCDC_SelectRstFuc (BOOLEAN is_open_rst);
/**********************************************************************************************************/
// Description: LCDC module handle interrupt
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_irq (uint32 param);
/**********************************************************************************************************/
// Description: register handle function
// Global resource dependence: g_lcdc_int_list
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC BOOLEAN LCDC_RegisterInterrupt (uint32 int_num, lcdc_func_t func);
/**********************************************************************************************************/
// Description: provide lcdc interrupt
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      input :
//          int_ptr - pointer to interrupt structure
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_SpecialMode (LCD_INT_ENA_T *int_ptr);
/**********************************************************************************************************/
// Description: close LCDC TV DAC
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_CloseTvDAC (void);





/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _LCDC_H
