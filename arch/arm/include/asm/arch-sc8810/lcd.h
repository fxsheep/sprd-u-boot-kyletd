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


#ifndef _LCD_H_
#define _LCD_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"
#include "com_drvapi.h"
#include "chip.h"

#if defined(PLATFORM_SC6800H) || defined(PLATFORM_SC8800G)|| defined(CONFIG_SC8810)
#include "lcdc_types.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

//error code define
//TV size .
#define TV_WIDTH    360
#define TV_HEIGHT   288
typedef enum
{
    LCD_ROT_MODE_LCD = 0x00,
    LCD_ROT_MODE_DMA,

    LCD_ROT_MODE_MAX

}
LCD_ROT_MODE_E;

typedef enum
{
    ERR_LCD_NONE = 0,       // Success,no error
    ERR_LCD_FUNC_NOT_SUPPORT,       // LCD not support this function
    ERR_LCD_PARAMETER_WRONG,        // Parameter is wrong
    ERR_LCD_POINTER_NULL,           // Input pointer is PNULL
    ERR_LCD_OPERATE_FAIL,           // Operate fail
    ERR_LCD_NOT_FOUND,              // LCD not found
    ERR_LCD_SELPIN_WRONG,           // LCD SELPIN WRONG
    ERR_LCD_SPEC_WRONG              // LCD SPEC WRONG
} ERR_LCD_E;

typedef enum
{
    CMD_LEVEL   = 0,                //command:0, data:1. nomal
    DATA_LEVEL  = 1             //command:1, data:0.
} LCD_CD_LEVEL_E;

typedef enum
{
    LCD_RGB = 0,
    LCD_MCU
} LCD_MODE_T;


typedef enum
{
    BUS_MODE_8080   = 0,
    BUS_MODE_6800 ,
    BUS_MODE_SPI
} LCD_BUS_MODE_T;

typedef enum
{
    WIDTH_16  =0,
    WIDTH_18 ,
    WIDTH_8  ,
    WIDTH_9
} LCD_BUS_WIDTH_T;

// LCD Contrller type
typedef enum
{
    LCD_CTRL_MODULE_LCDC = 0   ,    //LCDC
    LCD_CTRL_MODULE_LCM        ,    //LCM
    LCD_CTRL_MODULE_LCDC_LCM0,      //LCDC+LCM0
    LCD_CTRL_MODULE_EMC
} LCD_CTRL_MODULE_E;


typedef enum
{
    LCD_ANGLE_0   ,
    LCD_ANGLE_90  ,
    LCD_ANGLE_180 ,
    LCD_ANGLE_270
} LCD_ANGLE_E;

typedef enum
{
    LCD_DIRECT_NORMAL = 0x00,
    LCD_DIRECT_ROT_90,      // Rotation 90
    LCD_DIRECT_ROT_180,     // Rotation 180
    LCD_DIRECT_ROT_270,     // Rotation 270
    LCD_DIRECT_MIR_H,       // Mirror horizontal
    LCD_DIRECT_MIR_V,       // Mirror vertical
    LCD_DIRECT_MIR_HV,      // Mirror horizontal and vertical

    LCD_DIRECT_MAX

} LCD_DIRECT_E;

typedef enum
{
    LCD_ID_0 = 0,
    LCD_ID_1    ,
    LCD_ID_2    ,
    LCD_ID_3    ,
    LCD_ID_4    ,
    LCD_ID_5    ,
    LCD_ID_6    ,
    LCD_ID_7    ,
    TV_ID       ,                   //be used for TVOUT
    MAX_LCD_ID                      //Reserved, can not be used by user.
} LCD_ID_E;

/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/


// lcd information struct, which will be useful for upper layer.
typedef union _LCD_TIMING_U
{
    struct RGB_LCD_TIMING_tag
    {
        uint16 bfw;    //Beginning-of-Frame Horizontal Sync Clock Wait Count-1
        uint16 efw;    //End-of-Frame Horizontal Sync Clock Wait Count-1
        uint16 vsw;    //Vertical Sync Pulse Width - 1
        uint16 blw;    //Beginning-of-Line Pixel Clock Wait Count -1
        uint16 elw;    //End-of-Line Pixel Clock Wait Count -1
        uint16 hsw;    //Horizontal Sync Pulse Width - 1
    } rgb_mode_timing;
    struct MCU_LCD_TIMING_tag
    {
        uint16 rcss;// CS setup time for LCM read (optional)
        uint16 rlpw;// low pulse width for LCM read (according spec)
        uint16 rhpw;// high pulse width for LCM read (according spec)
        uint16 wcss;// CS setup time for LCM write  (optional)
        uint16 wlpw;// low pulse width for LCM write (according spec)
        uint16 whpw;// high pulse width for LCM write (according spec)
    } mcu_mode_timing;
} LCD_TIMING_U;



typedef struct lcd_operations_tag
{
    ERR_LCD_E (*lcd_Init) (void);
    ERR_LCD_E (*lcd_EnterSleep) (BOOLEAN is_sleep);
    ERR_LCD_E (*lcd_SetContrast) (uint16 contrast);
    ERR_LCD_E (*lcd_SetBrightness) (uint16 brightness);
    ERR_LCD_E (*lcd_SetDisplayWindow) (uint16 left, uint16 top, uint16 right, uint16 bottom);
    ERR_LCD_E (*lcd_InvalidateRect) (uint16 left, uint16 top, uint16 right, uint16 bottom);
    ERR_LCD_E (*lcd_Invalidate) (void);
    void (*lcd_Close) (void);
    ERR_LCD_E (*lcd_RotationInvalidateRect) (uint16 left, uint16 top, uint16 right, uint16 bottom, LCD_ANGLE_E angle);
    ERR_LCD_E (*lcd_SetBrushDirection) (LCD_DIRECT_E direct_type);
    void (*lcd_Rst) (void);
} LCD_OPERATIONS_T;

typedef struct lcd_spec_tag
{
    uint32                  width;
    uint32                  height;
    LCD_MODE_T          mode;       // rgb or mcu
    LCD_BUS_MODE_T        bus_mode;
    LCD_BUS_WIDTH_T      bus_width;
    LCD_TIMING_U           *timing;         // lcd read/write timing parameter
    LCD_OPERATIONS_T   *operation;
} LCD_SPEC_T, *LCD_SPEC_T_PTR;

typedef struct
{
    uint16                      cs_pin;                // cs pin connected with LCM
    uint16                      cd_pin;               // cd pin connected with LCM
    LCD_CTRL_MODULE_E                 controller;
    LCD_SPEC_T_PTR                       spec_ptr;
} LCD_SPEC_INFO_T,*LCD_SPEC_INFO_T_PTR;

typedef struct _lcd_simple_init_param_tag
{
    uint32 lcd_id;
    uint32 cs_id;
    uint32 interface_type;  // 0: 8080;1: 6800
    uint32 cd_pol;
    uint32 lcd_bits;//0£º16bit ;1£º18bit;2: 8bit;3: 9bit
    LCD_TIMING_U timing;
    LCD_CTRL_MODULE_E controller;
} LCD_SIMPLE_INIT_PARAM_T;
/**---------------------------------------------------------------------------*
 **                         Function Prototypes
 **---------------------------------------------------------------------------*/

/******************************************************************************/
//  Description:  return the lcd driver funtion pointer
//  Global resource dependence:
//  Author: Jim.zhang
//  Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T **LCD_GetOperations (void);

/******************************************************************************/
//  Description:  return the sub lcd buffer pointer
//  Global resource dependence:
//  Author: Jim.zhang
//  Note:
/******************************************************************************/
PUBLIC void *LCD_GetSubLCDBuffer (void);

/******************************************************************************/
//  Description:  return the main lcd buffer  pointer
//  Global resource dependence:
//  Author: Jim.zhang
//  Note:
/******************************************************************************/
PUBLIC void *LCD_GetLCDBuffer (void);

/******************************************************************************/
//  Description:  let the main lcd display on to display assert information
//  Global resource dependence:
//  Author: Jim.zhang
//  Note:
/******************************************************************************/
PUBLIC void LCD_DisplayOn (void);

/*****************************************************************************/
//  Description:  Get LCD number int the system.
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
PUBLIC uint32 LCD_GetUsedNum (void);

/*******************************************************************************/
// Description:
// Global resource dependence:
// Author: Younger.yang
// Note:
/********************************************************************************/
PUBLIC uint32 LCD_GetBusRatio (void);
/*******************************************************************************/
// Description:
// Global resource dependence:
// Author: Ming.zhang & Younger.yang
// Note:
/********************************************************************************/

PUBLIC void LCD_Wait (uint32 ms);

/*********************************************************************/
//  Description:   Read LCD ID
//  Global resource dependence:
//  Author: Younger.Yang
//  Note: You may read LCD ID before lcd init,otherwise
//  the read data may be incorrect.
/*********************************************************************/
PUBLIC uint32 LCD_Read_ID (uint32 lcd_cs, uint32 lcd_cd);


/*****************************************************************************/
//  Description:  LCD Reg Info
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/

PUBLIC void LCD_RegInfo (LCD_ID_E lcd_id, LCD_SPEC_INFO_T_PTR lcd_spec_ptr);

/*****************************************************************************/
//  Description:  clear the whole lcd to one color.
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_Clear (
    LCD_ID_E lcd_id,     //id of lcd to operate.
    uint32 color        //color to fill the whole lcd.
);

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
PUBLIC int32 LCD_SendCmd (uint32 cmd,uint32 lcd_id);

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
PUBLIC int32 LCD_SendData (uint32 lcd_data,uint32 lcd_id);

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
PUBLIC int32 LCD_SendCmdData (uint32 cmd,uint32 lcd_data,uint32 lcd_id);

/**********************************************************************************************************/
// Description:Read data from lcm
// Global resource dependence: NONE
// Author: Younger.yang
// Note:
//      this interface is read data from lcm
//      input:
//            index_value - cd value
//                  lcd_id-MAIN_LCD_ID/SUB_LCD_ID
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC uint32 LCD_ReadRegVal (uint32 lcd_id,uint32 reg_addr);


/*****************************************************************************/
//  Description:  Set LCD invalidate direction
//  Input:
//      lcd_id,lcd_angle
//  Return:
//      None.
//  Note: Application should correct invalidate direction right after current
//        image displayed
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/

PUBLIC ERR_LCD_E  LCD_SetDirection (
    LCD_ID_E lcd_id,//ID of lcd to operate
    LCD_DIRECT_E     direct_type
);

/*****************************************************************************/
//  Description:    LCD delay
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LCD_Delayms (uint32 ms);


PUBLIC ERR_LCD_E LCD_Init (void);
/**********************************************************************************************************/
// Description:Read data from lcm
// Global resource dependence: NONE
// Author: Jianping.Wang
// Note:
//      this interface is read data from lcm
//      input:
//            param_ptr - parameter pointer
//      return:
//          ERR_LCD_NONE,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC ERR_LCD_E LCD_SimpleInit (LCD_SIMPLE_INIT_PARAM_T *param_ptr);
#if defined(PLATFORM_SC6800H) || defined(PLATFORM_SC8800G)|| defined(CONFIG_SC8810)
/*****************************************************************************/
//  Description: config lcd block of 6800h
//  Global resource dependence:
//  Author: Shan.he
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_ConfigBlockEx (LCD_ID_E lcd_id, LCD_LAYER_ID_E layer_id,
                                    void *blk_param_ptr);


/*****************************************************************************/
//  Description: config capture parameters of 6800h
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
//PUBLIC ERR_LCD_E LCD_ConfigCapture(LCD_ID_E lcd_id, LCD_CAP_PARAM_T *param_ptr);

#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif // _LCD_H
