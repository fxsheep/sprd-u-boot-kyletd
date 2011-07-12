/******************************************************************************
 ** File Name:      dal_lcd.h                                                 *
 ** Author:         Jim zhang                                                 *
 ** DATE:           06/03/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the color LCD API function for up layer  *
 **                 1. LCD driver support a LCD buffer according to lcd size  *
 **                    to hold the pixel color. Up layer can get the buffer   *
 **                    pointer and operate on the LCD buffer.                 *
 **                 2. LCD driver is only responsible of copying the pixel    *
 **                    color in LCD buffer to LCD (hardware).                 *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/03/2004     Jim.zhang        Update file used for lcd api for up layer *
 ** 10/22/2004     Jim.zhang        Delete some api.                          *
 ******************************************************************************/

#ifndef _DAL_LCD_H_
#define _DAL_LCD_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

#include "sci_types.h"
#include "os_api.h"
#include "lcd.h"
#include "lcd_controller.h"
#include "lcdc_types.h"

/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/

#define LCD_VERSION_1       1

//LCD id enum struct
#define MAIN_LCD_ID         0
#define SUB_LCD_ID          1

#define LCD_SUM                 8
#define BLOCK_SUM                   6

#define LCD_SUPPORT_MAX 6

#define MAX_LCM_PARAM_VALUE ((0x3ee << 10)|(0x3ee))

#define  LCD_BACKGROUND    0x0

//Priority field configures the time ratio of LCM operation to NAND operation.
#define LCD_BUSRATIO            1  // Configure the bus priority.

#define BLOCK_ID_WITH_SINGLE_PIXEL_ALPHA 1
#define BLOCK_IMAGE_ID                      0

//resolution of lcd data
//0:RGB888; 1:RGB666; 2:RGB565;3:RGB555;4:GREY,5:YUV422;6:YUV420;7:YUV400
#define  LCD_RESOLUTION_RGB888          0
#define  LCD_RESOLUTION_RGB666          1
#define  LCD_RESOLUTION_RGB565          2
#define  LCD_RESOLUTION_RGB555          3
#define  LCD_RESOLUTION_GREY              4
#define  LCD_RESOLUTION_YUV422          5
#define  LCD_RESOLUTION_YUV420          6
#define  LCD_RESOLUTION_YUV400          7

#define LCD_USE_PIXEL_ALPHA        0
#define LCD_USE_BLOCK_ALPHA               1


// LCD operation fashion
typedef enum
{
    BLOCK_OPERATION = 0   ,
    AHB_COMMAND_OPERATION
}
LCD_OPERATION_E;



//Brush LCD mode
typedef enum
{
    LCD_BRUSH_MODE_SYNC = 0,
    LCD_BRUSH_MODE_ASYNC
} LCD_BRUSH_MODE_E;

//LCD status
typedef enum
{
    LCD_STATUS_IDLE   = 0,
    LCD_STATUS_BUSY
} LCD_STATUS_E;


typedef enum
{
    DATA_SIZE_BYTE = 0,             //0:DATA_SIZE_BYTE
    DATA_SIZE_HALFWORD,             //1:DATA_SIZE_HALFWORD
    DATA_SIZE_WORD                  //2:DATA_SIZE_WORD
} LCD_DATA_SIZE_E;
typedef enum
{
    MEMORY_IDLE = 0,
    MEMORY_READY   ,
    MEMORY_BUSY    ,
    MEMORY_ERROR
} LCD_MEMORY_STATE_E;

typedef enum
{
    LCD_IDLE = 0,
    LCD_READY   ,
    LCD_BUSY    ,
    LCD_ERROR
} LCD_STATE_E;


typedef enum
{
    SYS_STATE_ASSERT = 0,           // Assert state
    SYS_STATE_NORMAL    ,           // Normal Run
    SYS_STATE_INT                   // Interrupt state
} SYS_STATE_E;


/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/

typedef struct _osd_rect_tag //rectangle
{
    uint16  x;
    uint16  y;
    uint16  w;
    uint16  h;
} OSD_RECT_T;

typedef struct _osd_size_tag
{
    uint16 w;
    uint16 h;
} OSD_SIZE_T;

typedef struct _osd_info_tag
{
    OSD_RECT_T rect;
    uint32 addr;
    uint16 colorkey;
    uint16  alpha;
    struct _osd_info_tag *next_ptr;
} OSD_INFO_T;

typedef struct _isp_osd_info_tag
{
    uint32 enable;
    OSD_SIZE_T background_size;
    OSD_INFO_T *inf_ptr;
} ISP_OSD_PARAM_T;

//Block Configuration
typedef struct blockcfg_tag
{
    uint16 start_x;
    uint16 start_y;
    uint16 end_x;
    uint16 end_y;
    uint16 colorkey;
    uint16 width;                //real data width in SDRAM
    uint8  resolution;           //0:RGB888; 1:RGB666; 2:RGB565(RGB555 if OSD)
    uint8  type;                 //0:OSD; 1:image
    uint8  priority;             //0~7, high:7,low:0
    uint8  alpha;
    uint8  colorkey_en;
    uint8  alpha_sel;          //0-pixel alpha;1-block alpha
    uint8  is_little_endian;   //support for 6800H
    uint8  reserved;
    uint32 grey_rgb;//RGB888 value  //support for 6800H
} BLOCKCFG_T;



typedef struct block_tag
{
    uint16      is_enable;  //1: enable this block;0: disable this block
    uint32      *mem_ptr;
    //#ifdef PLATFORM_SC6800H
    uint32      *alpha_addr;
    uint32      *uv_addr;
    //#endif
    LCD_STATE_E state;      //{idle,ready,busy,error}
    BLOCKCFG_T  cfg;
} BLOCK_T;

typedef struct block_group_tag
{
    uint32    lcd_id;
    BLOCK_T   block[BLOCK_SUM];
} BLOCK_GROUP_T;

typedef struct lcd_property_tag
{
    SCI_SEMAPHORE_PTR   semaphore;
    LCD_BRUSH_MODE_E    brush_mode;// 1 if brush lcd with synchronistic mean, or else 0
    LCD_OPERATION_E         operation;  //block operation or AHB command operation
    LCD_STATE_E             state ;     //{idle, ready,busy, error};
    uint32                      blk_group_num;
    uint32                      sleep_state;
    uint32               *buffer;
    LCD_SPEC_INFO_T     lcd_spec;
} LCD_PROPERTY_T;



typedef struct lcd_info_tag
{
    uint32  r_bitmask;              // R(Red color) bit mask
    uint32  g_bitmask;              // G(Green color) bit mask
    uint32  b_bitmask;              // B(Black color) bit mask
    uint16  bits_per_pixel;         // bit number of one pixel
    uint16  lcd_width;              // lcd width
    uint16  lcd_height;             // lcd height.
    uint16  contrast_min;           // min value of contrast
    uint16  contrast_max;           // max value of contrast
    uint16  contrast_defaut;        // default value of contrast
    void    *lcdbuff_ptr;           // lcd buffer pointer
} LCD_INFO_T;

typedef struct _lcd_out_position_tag
{
    LCD_ID_E lcd_id;   //lcd id
    uint16   start_x;  //output start x at lcd
    uint16   start_y;  //output start y at lcd
    uint16   width;    //output data width
    uint16   height;   //output data height
    uint32   *data_buf;//memory address which lcd output data to
} LCD_OUT_POSITION_T;



typedef struct _lcd_size_tag
{
    uint16 w;
    uint16 h;
} LCD_SIZE_T;
typedef struct _lcd_rect_t_tag
{
    uint16 left;  //LCD display window left position
    uint16 top;   //LCD display window top position
    uint16 right; //LCD display window right position
    uint16 bottom;//LCD display window bottom position
} LCD_RECT_T;

typedef LCDC_CAP_PARAM_T LCD_CAP_PARAM_T;
typedef struct _lcd_memory_info_tag
{
    uint32 buf_addr;
    LCD_RECT_T cap_rect;
    LCD_SIZE_T dst_size;
    LCD_RECT_T dst_trim_rect;
    LCD_DATA_FORMAT_E data_type;
    uint32 data_endian_mode;
} LCD_MEMORY_INFO_T;
#if 0
typedef struct _lcd_block_info_tag
{
    uint32             block_id;
    BLOCKCFG_T  cfg;
    BOOLEAN        is_enable;
    uint32             buf_addr;
    struct _lcd_block_info_tag   *next_block_info_ptr;
} LCD_BLOCK_INFO_T;

typedef struct _lcd_invalidate_param_tag
{
    LCD_RECT_T rect;
    LCD_ANGLE_E rot_angle;
    LCD_BLOCK_INFO_T blk_info;
} LCD_INVALIDATE_PARAM_T;

typedef struct _lcd_convert_input_tag
{
    uint16 logic_lcd_width;
    uint16 logic_lcd_height;
    LCD_RECT_T  logic_coordinate;
    LCD_ANGLE_E logic_angle;
} LCD_CONVERT_INPUT_T;

typedef struct _lcd_convert_output_tag
{
    LCD_RECT_T  physical_coordinate;
    LCD_ANGLE_E rotation_angle;
} LCD_CONVERT_OUTPUT_T;
#endif
//#ifdef PLATFORM_SC8800H
typedef void (* lcd_invalidate_done_func_t) (uint32 param); //callback when invalidate end
//#endif
/**---------------------------------------------------------------------------*
 **                         Function Prototypes
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    The function used to initlize the lcd. If have dual lcd,
//                  sub lcd will also  be initlized by this function.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:           If have many lcd, this function will init all lcd.

/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_Init (void);

/*****************************************************************************/
//  Description:  reset lcd module.
//  Global resource dependence:
//  Author: Jim.zhang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_Reset (void);

/*****************************************************************************/
//  Description:    The function is used to close lcd when power off.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:           When power off , call this function. If have many lcd,
//                  this function will close all lcd.
/*****************************************************************************/
PUBLIC void LCD_Close (void);

/*****************************************************************************/
//  Description:    Get the lcd basic information..
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/*****************************************************************************/
PUBLIC  ERR_LCD_E  LCD_GetInfo (//Return the operate status
    LCD_ID_E  lcd_id,           //id of lcd to operate
    LCD_INFO_T *lcd_info_ptr    //lcd information struct pointer
);

/*****************************************************************************/
//  Description:    Invalidate a rectangle of LCD--Refresh it.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Return:         error code.
//  Note:1. the left up point is (0,0)
//       2. Before call InvalidateRect()£¬write LCD buffer with color,
//          and then InvalidateRect() the spefic area.
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InvalidateRect ( //Return the operate status
    LCD_ID_E lcd_id,                //id of lcd to operate
    uint16 left,                    //left of the windows to be refreshed.
    uint16 top,                     //top of the widonws to be refreshed.
    uint16 right,                   //right of the windows to be refreshed.
    uint16 bottom                   //bottom of the windows to be refreshed.
);

/*****************************************************************************/
//  Description:   invalidate a pixel(called by mmi special effect)
//    Global resource dependence:
//  Author: bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void LCD_InvalidatePixel (
    LCD_ID_E lcd_id,
    uint16 x,
    uint16 y,
    uint16 color
);

/*****************************************************************************/
//  Description:    Invalidate a rectangle of LCD--Refresh it with angle.
//  Global resource dependence:
//  Author:         Jianping.wang
//  Return:         error code.
//  Note:1. the left up point is (0,0)
//       2. Before call InvalidateRect()£¬write LCD buffer with color,
//          and then InvalidateRect() the spefic area.
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_RotateInvalidateRect ( //Return the operate status
    LCD_ID_E    lcd_id,                 //id of lcd to operate
    uint16      left,                   //left of the windows to be refreshed.
    uint16      top,                    //top of the widonws to be refreshed.
    uint16      right,                  //right of the windows to be refreshed.
    uint16      bottom,                 //bottom of the windows to be refreshed.
    LCD_ANGLE_E angle                   //rotation angle
);
/*****************************************************************************/
//  Description:    Invalidate a rectangle of LCD--Refresh it by vertical scan mode.
//  Global resource dependence:
//  Author:         Jianping.wang
//  Return:         error code.
//  Note:1. the left up point is (0,0)
//       2. Before call InvalidateRect()£¬write LCD buffer with color,
//          and then InvalidateRect() the spefic area.
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_VerticalInvalidateRect ( //Return the operate status
    LCD_ID_E lcd_id,                //id of lcd to operate
    uint16 left,                    //left of the windows to be refreshed.
    uint16 top,                     //top of the widonws to be refreshed.
    uint16 right,                   //right of the windows to be refreshed.
    uint16 bottom                   //bottom of the windows to be refreshed.
);
/*****************************************************************************/
//  Description:    Invalidate the whole lcd.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_Invalidate ( //Retrurn the operate status
    LCD_ID_E  lcd_id            //id of lcd to operate
);

/*****************************************************************************/
//  Description:    Enter/Exit sleep mode to control power consume.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Return:         error code.
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E   LCD_EnterSleep ( //Retrun the operate status
    LCD_ID_E  lcd_id,             //id of lcd to operate
    BOOLEAN is_sleep              //SCI_TRUE:enter sleep: SCI_FALSE:exit sleep mode
);


/*****************************************************************************/
//  Description:    Set the contrast value of lcd.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Return:         return the operate status.
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetContrast ( //return the operate status
    LCD_ID_E  lcd_id,           //id of lcd to operate
    uint16  contrast            //contrast value to set to lcd.
);

/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBrightness (
    LCD_ID_E lcd_id,    //ID of lcd to operate.
    uint16 brightness   //brightness value to set.
);


/*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//  Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
PUBLIC ERR_LCD_E  LCD_SetDisplayWindow (
    LCD_ID_E lcd_id,//ID of lcd to operate
    uint16 left,  //left of the display window
    uint16 top,   //top of the display window
    uint16 right,     //right of the display window
    uint16 bottom //bottom of the display window
);




/*****************************************************************************/
//  Description: get lcd controller type
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC LCD_CTRL_MODULE_E LCD_GetCtrlModule (LCD_ID_E lcd_id);
/*****************************************************************************/
//  Description: set brush lcd fashion
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBrushMode (LCD_ID_E lcd_id, LCD_BRUSH_MODE_E mode);
/*****************************************************************************/
//  Description: get brush lcd fashion
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id - specify LCD
//      output:
//          none
//      return:
//          brush LCD mode
/*****************************************************************************/
PUBLIC LCD_BRUSH_MODE_E LCD_GetBrushMode (LCD_ID_E lcd_id);
/*****************************************************************************/
//  Description:get memory status
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//      output:
//          none
//      return:
//          lcd status: LCD_IDLE  - don't use LCD ID
//                      LCD_BUSY  - is brushing this LCD
//                      LCD_READY - finish brushing this LCD
//                      LCD_ERROR - error
/*****************************************************************************/
PUBLIC LCD_STATE_E LCD_GetLCDState (LCD_ID_E lcd_id);
/*****************************************************************************/
//  Description: configure block parameters
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_ConfigBlock (
    LCD_ID_E lcd_id,
    uint32 blk_num,
    BLOCKCFG_T config
);

/*****************************************************************************/
//  Description: get block parameters
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_GetConfigBlock (
    LCD_ID_E lcd_id,
    uint32 blk_num,
    BLOCKCFG_T *cfg_ptr
);
/*****************************************************************************/
//  Description: Enable Block
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_EnableBlock (LCD_ID_E lcd_id, uint32 blk_num);
/*****************************************************************************/
//  Description:disable block
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_DisableBlock (LCD_ID_E lcd_id, uint32 blk_num);
/*****************************************************************************/
//  Description: Malloc memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 *LCD_Malloc (uint32 size);
/*****************************************************************************/
//  Description:  free memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_Free (uint32 *addr_ptr);
/*****************************************************************************/
//  Description: set display memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBlockBuffer (
    LCD_ID_E lcd_id,
    uint32 blk_num ,
    uint32 *buf_ptr
);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC LCD_MEMORY_STATE_E LCD_GetMemoryStatus (LCD_ID_E lcd_id, uint32 *mem_ptr);
/*****************************************************************************/
//  Description: Update LCD Timing
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_UpdateTiming (uint32 ahb_clk);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void TV_Open (TV_MODE_E mode);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void TV_Refresh (void);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void TV_Close (void);
/*****************************************************************************/
// Description: set LCDC Clock
// Global resource dependence:
// Author: Jianping.wang
// Note: this interface isn't called in interrupt handle function
/****************************************************************************/
PUBLIC int32 LCDC_SetClock (uint32 lcdc_clk);
/**********************************************************************************************************/
// Description: restore LCDC Clock
// Global resource dependence:
// Author: Jianping.wang
// Note: this interface isn't called in interrupt handle function
/**********************************************************************************************************/
PUBLIC int32 LCDC_RestoreClock (void);
/*****************************************************************************/
//  Description:Get Block is enabled or not
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//          blk_num - LCDC Block number ,0~5
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC BOOLEAN LCD_GetBlockIsEnable (LCD_ID_E lcd_id, uint32 blk_num);
/*****************************************************************************/
//  Description: get display memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//          blk_num - LCDC Block number ,0~5
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC uint32 *LCD_GetBlockBuffer (LCD_ID_E lcd_id, uint32 blk_num);
/*****************************************************************************/
//  Description: get maximum block number
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//      output:
//          none
//      return:
//          maximum block number
/*****************************************************************************/
PUBLIC uint32 LCD_GetMaxBlockNum (LCD_ID_E lcd_id);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          is_used - SCI_TRUE:use scaling down; SCI_FALSE:don't use scaling down
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void LCD_IsSelectScalingDown (BOOLEAN is_used);



/*****************************************************************************/
//  Description:  set dest LCD's RGB mode to host
//  Global resource dependence:
//  Author: Younger.Yang
//  Note:
/*****************************************************************************/

PUBLIC void LCD_SetRGBMode (LCD_ID_E lcd_id);

/*****************************************************************************/
//  Description:  set the number of pixes in the next frame from OSD
//  Global resource dependence:
//  Author: Younger.Yang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_SetDataNumber (LCD_ID_E lcd_id, uint32 number);

/*****************************************************************************/
//  Description:  change lcd set timing when ahb clk changed
//  Global resource dependence:
//  Author: Younger.Yang
//  Note:
/*****************************************************************************/

PUBLIC ERR_LCD_E LCD_SetParam (uint32 ahb_clk);


PUBLIC void LCD_SetBackground (uint32 color);

/*****************************************************************************/
//  Description:    get the main lcd 's important parameter for Digital cammera
//  Global resource dependence:
//  Author:         Zhemin.lin
//  Note:
/*****************************************************************************/
PUBLIC LCD_SPEC_INFO_T_PTR LCD_GetLcdSpec (
    LCD_ID_E lcd_id  //id of lcd to operate.
);


/*****************************************************************************/
//  Description:  disable/enable lcd invalidate rect
/*****************************************************************************/
PUBLIC void LCD_InvalidateRect_Enable (BOOLEAN is_enable);

/*****************************************************************************/
//  Description:  set rotation mode
/*****************************************************************************/
PUBLIC void LCD_SetRotateMode (LCD_ROT_MODE_E mode);

/*****************************************************************************/
//  Description: get rotation mode
/*****************************************************************************/
PUBLIC LCD_ROT_MODE_E _LCD_GetRotateMode (void);


/*****************************************************************************/
//  Description: free semaphore when use LCDC by asyn fashion
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_FreeSemaphore (void);
/*****************************************************************************/
//  Description: set brush lcd fashion
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id - specify LCD
//          mode   - sync or async
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBrushMode (LCD_ID_E lcd_id, LCD_BRUSH_MODE_E mode);
/*****************************************************************************/
//  Description: get brush lcd fashion
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id - specify LCD
//      output:
//          none
//      return:
//          brush LCD mode
/*****************************************************************************/
PUBLIC LCD_BRUSH_MODE_E LCD_GetBrushMode (LCD_ID_E lcd_id);
/*****************************************************************************/
//  Description: set lcd status
//  Global resource dependence: s_lcd_status
//  Author: Younger yang
//  Note:
/*****************************************************************************/

PUBLIC void LCD_SetLCDStatus (LCD_STATUS_E lcd_status);

/*****************************************************************************/
//  Description: get lcd status
//  Global resource dependence: s_lcd_status
//  Author: Younger yang
//  Note:
/*****************************************************************************/
PUBLIC LCD_STATUS_E LCD_GetLCDStatus (void);

/*****************************************************************************/
//  Description: set alpha buffer address when use pixel alpha ,data format of block is RGB565
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetPixelAlphaBuffer (
    LCD_ID_E lcd_id,
    uint32 blk_num,
    uint32 *buf_ptr
);
/*****************************************************************************/
//  Description: get alpha buffer address when use pixel alpha ,data format of block is RGB565
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 *LCD_GetPixelAlphaBuffer (LCD_ID_E lcd_id, uint32 blk_num);
/*****************************************************************************/
//  Description: set UV buffer for block 0
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetUVBuffer (
    LCD_ID_E lcd_id,
    uint32 blk_num,
    uint32 *buf_ptr
);
/*****************************************************************************/
//  Description: config capture parameters of 6800h
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_ConfigCapture (LCD_ID_E lcd_id, LCD_CAP_PARAM_T *param_ptr);
/*****************************************************************************/
//  Description: set fmark mode
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetFmarkMode (
    LCD_FMARK_MODE_E fmark_mode,
    LCD_FMARK_POL_E fmark_pol
);

//#ifdef PLATFORM_SC8800H
/*****************************************************************************/
// Description:register callback when invalidate done
// Author: Jianping.Wang
/*****************************************************************************/
PUBLIC void LCD_RegisterCallBack (lcd_invalidate_done_func_t func);
/*****************************************************************************/
// Description:register callback when invalidate done
// Author: Jianping.Wang
/*****************************************************************************/
PUBLIC void LCD_UnRegisterCallBack (void);
//#endif
/**********************************************************************************************************/
// Description: get block information
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC ERR_LCD_E LCD_GetBlockInfo (LCD_ID_E lcd_id,BLOCK_T *blk_info_ptr);
/*****************************************************************************/
//  Description: LCD out data to memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          pos_param - out position parameter
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InvalidateRectToMemory (LCD_ID_E lcd_id,LCD_MEMORY_INFO_T *param_ptr);
/*****************************************************************************/
//  Description: set UV buffer for block 0
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetUVBuffer (
    LCD_ID_E lcd_id,
    uint32 blk_num,
    uint32 *buf_ptr
);
/*****************************************************************************/
//  Description: get UV memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//          blk_num - LCDC Block number ,0~5
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC uint32 *LCD_GetBlockUVBuffer (LCD_ID_E lcd_id, uint32 blk_num);
PUBLIC void LCD_DisplayText (uint16 x, uint16 y, char *text, uint16 len);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_DAL_LCD_H
