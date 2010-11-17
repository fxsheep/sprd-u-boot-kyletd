/***************************************************************************************
** File Name:      lcdc_data_type.h                                                    *
** DATE:           28/09/2005                                                          *
** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.                   *
** Description:    this file defines macro and data type for LCM and LCDC              *
****************************************************************************************

****************************************************************************************
**                         Edit History                                                *
**-------------------------------------------------------------------------------------*
** DATE                  NAME                 DESCRIPTION                              *
** 28/09/2005            Jianping.wang        Create.                                  *
****************************************************************************************/

#ifndef _LCDC_DATA_TYPE_H_
#define _LCDC_DATA_TYPE_H_

/**------------------------------------------------------------------------------------*
 **                         Dependencies                                               *
 **------------------------------------------------------------------------------------*/
#include "sci_types.h"
#include "lcd.h"
/**------------------------------------------------------------------------------------*
 **                         Compiler Flag                                              *
 **------------------------------------------------------------------------------------*/
#ifdef __plusplus
extern "C"
{
#endif

typedef void (* lcdc_func_t) (void);


typedef struct lcdc_config_tag
{
    uint8  mode;
    uint8  background_b;
    uint8  background_g;
    uint8  backgrounf_r;
    uint16 frame_mode;
    uint16 size_x;
    uint16 size_y;
    uint16 color_a1;
    uint16 color_a2;
    uint16 color_a3;
    uint16 color_b1;
    uint16 color_b2;
    uint16 color_b3;
    uint16 color_c1;
    uint16 color_c2;
    uint16 color_c3;
    uint32 timing0;
    uint32 timing1;
    uint32 lcd_ctrl;
    uint32 para0;
    uint32 para1;
    uint32 modesel;
    uint32 rgb_mode;
    uint32 selpin;
} LCDC_CONFIG_T;

typedef struct lcm_config_tag
{
    uint32 para0;
    uint32 para1;
    uint32 modesel;
    uint32 rgb_mode;
    uint32 selpin;
} LCM_CONFIG_T;

typedef struct lcd_controller_cfg_tag
{
    LCDC_CONFIG_T lcdc_cfg;
    LCM_CONFIG_T  lcm_cfg;
} LCD_CONTROLLER_CFG_T;
/**********************************************************************************************************/
// LCDC TYPE STRUCTURE
/**********************************************************************************************************/
typedef struct _LCDC_T_tag
{
    uint32 lcdc_mode;        //0:LCDC RGB mode,1:LCDC MCU mode
    uint16 main_lcd_width;   //main lcd width
    uint16 main_lcd_height;  //main lcd height
    uint16 sub_lcd_width;    //sub lcd widht
    uint16 sub_lcd_height;   //sub lcd height
    uint16 main_lcd_cd;      //0 means that cd pin is 0 for command,cd pin is 1 for data
    //1 means that cd pin is 1 for command,cd pin is 0 for data
    uint16 sub_lcd_cd;       //0 means that cd pin is 0 for command,cd pin is 1 for data
    //1 means that cd pin is 1 for command,cd pin is 0 for data
    uint32 lcd_background;   //lcd background color
    uint32 mcu_lcd_sum;      //when LCDC is MCU mode,lcd sum with lcdc
    uint32 lcd_data_bits;
    uint32 lcdc_refresh_mode;
    uint16 main_lcd_cs_pin;
    uint16 sub_lcd_cs_pin;
    uint32 controller;
} LCDC_TYPE_T;

/**********************************************************************************************************/
// DISPLAY BLOCK DATA STRUCTURE
/**********************************************************************************************************/
typedef union _BLOCK_START_REG_U_tag
{
    struct START_REG_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint  start_x    :12;//block start horizontal coordinate to LCD left top point
        volatile uint  res        :4; //reserved
        volatile uint  start_y    :12;//block start vertical coordinate to LCD left top point
        volatile uint  priority   :3; //0~7,high:7,low:0
        volatile uint  enable     :1; //1:enable block;0:disable block;
#else
        volatile uint  enable     :1; //1:enable block;0:disable block;
        volatile uint  priority   :3; //0~7,high:7,low:0
        volatile uint  start_y    :12;//block start vertical coordinate to LCD left top point
        volatile uint  res        :4; //reserved
        volatile uint  start_x    :12;//block start horizontal coordinate to LCD left top point
#endif
    } mBits;
    volatile uint32 value;
} BLOCK_START_REG_U;

typedef union _BLOCK_END_REG_U_tag
{
    struct END_REG_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint end_x       :12;//block end horizontal coordinate to LCD left top point
        volatile uint end_y       :12;//block end vertical coordinate to LCD left top point
        volatile uint width       :8; //block real data width in SDRAM should be sdram_blk_width*8
#else
        volatile uint width       :8; //block real data width in SDRAM should be sdram_blk_width*8
        volatile uint end_y       :12;//block end vertical coordinate to LCD left top point
        volatile uint end_x       :12;//block end horizontal coordinate to LCD left top point
#endif
    } mBits;
    volatile uint32 value;
} BLOCK_END_REG_U;

typedef union _BLOCK_CONFIG_REG_U_tag
{
    struct CONFIG_REG_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint alpha             :8;
        volatile uint colorkey          :16;
        volatile uint resolution        :2;
        volatile uint alpha_sel         :1;
        volatile uint colorkey_en       :1;
        volatile uint type              :1;
        volatile uint res               :3;
#else
        volatile uint res               :3;
        volatile uint type              :1;
        volatile uint colorkey_en       :1;
        volatile uint alpha_sel         :1;
        volatile uint resolution        :2;
        volatile uint colorkey          :16;
        volatile uint alpha             :8;
#endif
    } mBits;
    volatile uint32 value;
} BLOCK_CONFIG_REG_U;

typedef struct _BLOCK_T_tag
{
    BLOCK_START_REG_U   blk_start;
    BLOCK_END_REG_U     blk_end;
    BLOCK_CONFIG_REG_U  blk_config;
    uint32              sdram_addr; //start address of block pixel data
    uint32              blk_num;    //0~5
} LCD_BLOCK_T;

typedef struct _BLOCK_ACCESS_T_tag
{
    uint32   blk_num;   //0~5
    uint32   blk_used;  //0:don't use this block;1:use this block
    uint32   blk_mask;  //access the block mask.If mask is correct,can access block
} BLOCK_ACCESS_T;

/**********************************************************************************************************/
// LCD COMMAND STRUCTURE
/**********************************************************************************************************/
typedef struct _LCD_CMD_T_tag
{
    volatile uint32 left_cmd;  //set LCD display window left position command
    volatile uint32 top_cmd;   //set LCD display window top position command
    volatile uint32 right_cmd; //set LCD display window right position command
    volatile uint32 bottom_cmd;//set LCD display window bottom position command
} LCD_CMD_T;
/**********************************************************************************************************/
// LCDC COLOR STRUCTURE
/**********************************************************************************************************/
typedef union _COLOR_REG_U_tag
{
    struct _COLOR_REG_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint colorCoeff   :12;
        volatile uint res          :20;//reserved
#else
        volatile uint res          :20;//reserved
        volatile uint colorCoeff   :12;
#endif
    } mBits;
    volatile uint32 value;
} COLOR_REG_U;

typedef struct _COLORMATRIX_T_tag
{
    COLOR_REG_U coeffA1;
    COLOR_REG_U coeffA2;
    COLOR_REG_U coeffA3;
    COLOR_REG_U coeffB1;
    COLOR_REG_U coeffB2;
    COLOR_REG_U coeffB3;
    COLOR_REG_U coeffC1;
    COLOR_REG_U coeffC2;
    COLOR_REG_U coeffC3;
} COLORMATRIX_T;
/**********************************************************************************************************/
// LCDC RGB MODE DATA STRUCTURE
/**********************************************************************************************************/
typedef union _LCDC_RGB_LCD_TIMING0_U
{
    struct RGB_TIMING0_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint bfw :8;//bit7~bit0,beginning of frame horizontal sync clock wait count - 1
        volatile uint efw :8;//bit15~bit8,end of frame horizontal sync clock wait count - 1
        volatile uint vsw :8;//bit23~vit16,Vertical Sync Pulse Width - 1
        volatile uint res :8;//bit31~bit24,reserved
#else
        volatile uint res :8;//bit31~bit24,reserved
        volatile uint vsw :8;//bit23~vit16,Vertical Sync Pulse Width - 1
        volatile uint efw :8;//bit15~bit8,end of frame horizontal sync clock wait count - 1
        volatile uint bfw :8;//bit7~bit0,beginning of frame horizontal sync clock wait count - 1
#endif
    } mBits;
    volatile uint32 value;
} LCDC_RGB_LCD_TIMING0_U;

typedef union _LCDC_RGB_LCD_TIMING1_U
{
    struct RGB_TIMING1_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint blw :8;//8bits,beginning of line pixel cloxk wait count - 1
        volatile uint elw :8;//8bits,end of line pixel cloxk wait count - 1
        volatile uint hsw :8;//8bits,Horizontal sync Pulse width - 1
        volatile uint res :8;//bit31~bit24,reserved
#else
        volatile uint res :8;//bit31~bit24,reserved
        volatile uint hsw :8;//8bits,Horizontal sync Pulse width - 1
        volatile uint elw :8;//8bits,end of line pixel cloxk wait count - 1
        volatile uint blw :8;//8bits,beginning of line pixel cloxk wait count - 1

#endif
    } mBits;
    volatile uint32 value;
} LCDC_RGB_LCD_TIMING1_U;

typedef union _LCDC_RGB_LCD_CTRL_U
{
    struct RGB_LCD_CTRL_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint res_1       :1; //1bit,ScalingUp,for LCD,bypass scalingUP mode
        volatile uint hsync_pol   :1; //1bit,0:LCD_HSYNC is valid when it is 1'b1;1:LCD_HSYNC is valid when it is 1'b0
        volatile uint vsync_pol   :1; //1bit,0:LCD_VSYNC is valid when it is 1'b1;1:LCD_VSYNC is valid when it is 1'b0
        volatile uint enable_pol  :1; //1bit,0:LCD_Enable is valid when it is 1'b1;1:LCD_Enable is valid when it is 1'b0
        volatile uint lcd_bpp     :2; //2bits,Bit Per Pixel;00¡êo888 01¡êo666;10: 565  11: Rsved
        volatile uint trans_cycle :1; //1bits,0: 1 cycle per Pixel;1: 3 cycle per Pixel
        volatile uint rb_format   :1; //1bits,0: pixel output sequence is RGB  1: output sequence is BGR
        volatile uint res         :24;//bit31~bit8
#else
        volatile uint res         :24;//bit31~bit8
        volatile uint rb_format   :1; //1bits,0: pixel output sequence is RGB  1: output sequence is BGR
        volatile uint trans_cycle :1; //1bits,0: 1 cycle per Pixel;1: 3 cycle per Pixel
        volatile uint lcd_bpp     :2; //2bits,Bit Per Pixel;00¡êo888 01¡êo666;10: 565  11: Rsved
        volatile uint enable_pol  :1; //1bit,0:LCD_Enable is valid when it is 1'b1;1:LCD_Enable is valid when it is 1'b0
        volatile uint vsync_pol   :1; //1bit,0:LCD_VSYNC is valid when it is 1'b1;1:LCD_VSYNC is valid when it is 1'b0
        volatile uint hsync_pol   :1; //1bit,0:LCD_HSYNC is valid when it is 1'b1;1:LCD_HSYNC is valid when it is 1'b0
        volatile uint res_1       :1; //1bit,ScalingUp,for LCD,bypass scalingUP mode
#endif
    } mBits;
    volatile uint32 value;
} LCDC_RGB_LCD_CTRL_U;

typedef struct _LCDC_RGB_INT_ENA_T_tag
{
    uint16 end_of_frame;  //1bit:0:disable;1:enable
    uint16 start_of_frame;//1bit:0:disable;1:enable
} LCDC_RGB_INT_ENA_T;

typedef struct _LCDC_RGB_SPEC_T_tag
{
    //  LCDC_RGB_LCD_TIMING0_U  timing0;
    //  LCDC_RGB_LCD_TIMING1_U  timing1;
    LCDC_RGB_LCD_CTRL_U     lcd_ctrl;
} LCDC_RGB_SPEC_T;

typedef struct _LCDC_RGB_INFO_T_tag
{
    LCDC_RGB_LCD_TIMING0_U  timing0;
    LCDC_RGB_LCD_TIMING1_U  timing1;
    LCDC_RGB_LCD_CTRL_U     lcd_ctrl;
} LCDC_RGB_INFO_T;
/**********************************************************************************************************/
// LCDC MCU MODE DATA STRUCTURE
/**********************************************************************************************************/
typedef union _LCDC_MCU_LCD_PARAMETER0_U
{
    struct MCU_PARAMETER0_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint whpw    :4; //Write High Pulse Width
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint wcss    :2; //Write CS Status
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rcss    :2; //Read CS Status
        volatile uint res     :12;//reserved
#else
        volatile uint res     :12;//reserved
        volatile uint rcss    :2; //Read CS Status
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint wcss    :2; //Write CS Status
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint whpw    :4; //Write High Pulse Width
#endif
    } mBits;
    volatile uint32 value;
} LCDC_MCU_LCD_PARAMETER0_U;

typedef union _LCDC_MCU_LCD_PARAMETER1_U
{
    struct MCU_PARAMETER1_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint whpw    :4; //Write High Pulse Width
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint wcss    :2; //Write CS Status
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rcss    :2; //Read CS Status
        volatile uint res     :12;//reserved
#else
        volatile uint res     :12;//reserved
        volatile uint rcss    :2; //Read CS Status
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint wcss    :2; //Write CS Status
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint whpw    :4; //Write High Pulse Width
#endif
    } mBits;
    volatile uint32 value;
} LCDC_MCU_LCD_PARAMETER1_U;

typedef union _LCDC_MCU_MODESEL_U
{
    struct MCU_MODESEL_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint modesel0    :1; //ModeSel0:0: 8080;1: 6800
        volatile uint modesel1    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint res         :30;//reserved
#else
        volatile uint res         :30;//reserved
        volatile uint modesel1    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint modesel0    :1; //ModeSel0:0: 8080;1: 6800
#endif
    } mBits;
    volatile uint32 value;
} LCDC_MCU_LCD_MODESEL_U;

typedef union _LCDC_MCU_RGBMODE_U
{
    struct MCU_RGBMODE_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint cs0_rgb_mode    :2; //CS0 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs1_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint res             :28;//reserved
#else
        volatile uint res             :28;//reserved
        volatile uint cs1_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs0_rgb_mode    :2; //CS0 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
#endif
    } mBits;
    volatile uint32 value;
} LCDC_MCU_LCD_RGBMODE_U;

typedef struct _LCDC_MCU_SPEC_T_tag
{
    LCDC_MCU_LCD_PARAMETER0_U parameter0;
    LCDC_MCU_LCD_PARAMETER1_U parameter1;
    LCDC_MCU_LCD_MODESEL_U    modesel;
    LCDC_MCU_LCD_RGBMODE_U    rgbmode;
} LCDC_MCU_SPEC_T;


typedef struct _LCDC_MCU_INFO_T_tag
{
    LCDC_MCU_LCD_PARAMETER0_U parameter0;
    LCDC_MCU_LCD_PARAMETER1_U parameter1;
    LCDC_MCU_LCD_MODESEL_U    modesel;
    LCDC_MCU_LCD_RGBMODE_U    rgbmode;
    uint32                    lcd_cd[2];//0:command is 0,data is 1,1:command is 1,data is 0
    uint32                    lcd_sum;  //lcd total number
} LCDC_MCU_INFO_T;

typedef struct _LCDC_MCU_CMD_DATA_ADDR_tag
{
    uint32 cmd_addr;
    uint32 data_addr;
} LCDC_MCU_ADDR_T;
/**********************************************************************************************************/
// LCD INTERRUPT STRUCTURE
/**********************************************************************************************************/
typedef struct _LCD_INT_ENA_T_tag
{
    LCDC_RGB_INT_ENA_T rgb_int_enable;//is used only when lcdc rgb mode
    uint32 mcu_endofosd_en;           //1:enable;0:disable,is used only when lcdc mcu mode
} LCD_INT_ENA_T;
/**********************************************************************************************************/
// LCDC SPEC STRUCTURE
/**********************************************************************************************************/
typedef struct _LCDC_SPEC_T_tag
{
    LCDC_RGB_SPEC_T rgb_lcd_spec;
    LCDC_MCU_SPEC_T mcu_lcd_spec;
} LCDC_SPEC_T;
/**********************************************************************************************************/
// LCD INFORMATION STRUCTURE
/**********************************************************************************************************/
typedef struct _LCDC_INFO_T_tag
{
    LCDC_RGB_INFO_T rgb_info;
    LCDC_MCU_INFO_T mcu_info;
} LCDC_INFO_T;

/**********************************************************************************************************/
// LCM TYPE STRUCTURE
/**********************************************************************************************************/
typedef struct _LCM_TYPE_T_tag
{
    uint16 lcm_lcd_sum;          //when use LCM module,lcd sum
    uint16 lcm_lcd_databits_max; //LCD linked to LCM data bits number maximum
} LCM_TYPE_T;
/**********************************************************************************************************/
// LCM CONFIGURE STRUCTURE
/**********************************************************************************************************/
typedef union _LCM_LCD_PARAMETER_U_tag
{
    struct LCM_PARAMETER_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint whpw    :4; //Write High Pulse Width
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint wcss    :2; //Write CS Setup time
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rcss    :2; //Read CS Setup time
        volatile uint res     :12;//reserved
#else
        volatile uint res     :12;//reserved
        volatile uint rcss    :2; //Read CS Setup time
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint wcss    :2; //Write CS Setup time
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint whpw    :4; //Write High Pulse Width
#endif
    } mBits;
    volatile uint32 value;
} LCM_LCD_PARAMETER_U;
typedef union _LCM_LCD_PARAMETER0_U_tag
{
    struct LCM_PARAMETER0_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint whpw    :4; //Write High Pulse Width
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint wcss    :2; //Write CS Setup time
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rcss    :2; //Read CS Setup time
        volatile uint res     :12;//reserved
#else
        volatile uint res     :12;//reserved
        volatile uint rcss    :2; //Read CS Status
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint wcss    :2; //Write CS Status
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint whpw    :4; //Write High Pulse Width
#endif
    } mBits;
    volatile uint32 value;
} LCM_LCD_PARAMETER0_U;

typedef union _LCM_LCD_PARAMETER1_U_tag
{
    struct LCM_PARAMETER1_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint whpw    :4; //Write High Pulse Width
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint wcss    :2; //Write CS Setup time
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rcss    :2; //Read CS Setup time
        volatile uint res     :12;//reserved
#else
        volatile uint res     :12;//reserved
        volatile uint rcss    :2; //Read CS Status
        volatile uint rlpw    :4; //Read Lower Pulse Width
        volatile uint rhpw    :4; //Read High Pulse Width
        volatile uint wcss    :2; //Write CS Status
        volatile uint wlpw    :4; //Write Lower Pulse Width
        volatile uint whpw    :4; //Write High Pulse Width
#endif
    } mBits;
    volatile uint32 value;
} LCM_LCD_PARAMETER1_U;

typedef union _LCM_MODESEL_U
{
    struct MODESEL_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint modesel0    :1; //ModeSel0:0: 8080;1: 6800
        volatile uint modesel1    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint modesel2    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint modesel3    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint modesel4    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint modesel5    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint res         :26;//reserved
#else
        volatile uint res         :26;//reserved
        volatile uint modesel5    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint modesel4    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint modesel3    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint modesel2    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint modesel1    :1; //ModeSel1:0: 8080;1: 6800
        volatile uint modesel0    :1; //ModeSel0:0: 8080;1: 6800
#endif
    } mBits;
    volatile uint32 value;
} LCM_LCD_MODESEL_U;

typedef union _LCM_RGBMODE_U
{
    struct RGBMODE_map
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        volatile uint cs0_rgb_mode    :2; //CS0 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs1_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs2_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs3_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs4_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs5_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint res             :20;//reserved
#else
        volatile uint res             :20;//reserved
        volatile uint cs5_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs4_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs3_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs2_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs1_rgb_mode    :2; //CS1 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
        volatile uint cs0_rgb_mode    :2; //CS0 device RGB mode:2'b00¡êo16bit (565);2'b01¡êo18bit (666);2'b10: 8bit;2'b11: 9bit
#endif
    } mBits;
    volatile uint32 value;
} LCM_LCD_RGBMODE_U;
typedef struct _LCM_LCDC_SPEC_T_tag
{
    //  LCM_LCD_PARAMETER0_U    para0;
    //  LCM_LCD_PARAMETER1_U    para1;
    LCM_LCD_MODESEL_U       mode_sel;
    LCM_LCD_RGBMODE_U       rgb_mode;
} LCM_LCD_SPEC_T;
/**********************************************************************************************************/
// LCM LCD INFORMATION STRUCTURE
/**********************************************************************************************************/
typedef struct _LCM_LCDC_INFO_T_tag
{
    LCM_LCD_PARAMETER0_U    para0;
    LCM_LCD_PARAMETER1_U    para1;
    LCM_LCD_MODESEL_U       mode_sel;
    LCM_LCD_RGBMODE_U       rgb_mode;
    uint32                  lcd_sum;
} LCM_LCD_INFO_T;
/**********************************************************************************************************/
// LCM PIN USE STRUCTURE
/**********************************************************************************************************/
typedef struct _LCM_LCD_PIN_T_tag
{
    uint8  cd_pin;  //0~5
    uint8  cs_pin;  //0~5
    uint8  lcd_used;//0:don't used;1:is used
    uint8  cd_level;
} LCM_LCD_PIN_T;

typedef struct _LCM_PIN_USE_T_tag
{
    LCM_LCD_PIN_T lcd0;
    LCM_LCD_PIN_T lcd1;
    LCM_LCD_PIN_T lcd2;
    LCM_LCD_PIN_T lcd3;
    LCM_LCD_PIN_T lcd4;
    LCM_LCD_PIN_T lcd5;
} LCM_PIN_USE_T;

/**********************************************************************************************************/
// OPERATION RETURN RESULT
/**********************************************************************************************************/
#define LCD_ERROR_BASE              0
#define LCD_OK                      (LCD_ERROR_BASE - 0) //operation success
#define LCD_ERROR_RGB_BPP           (LCD_ERROR_BASE - 1) //RGB mode,LCD pixel bit sum is wrong  
#define LCD_ERROR_MALLOC_BLOCK      (LCD_ERROR_BASE - 2) //malloc block fail
#define LCD_ERROR_DISPLAYWINDOW     (LCD_ERROR_BASE - 3) //lcd display window is wrong
#define LCD_ERROR_POINTER_NULL      (LCD_ERROR_BASE - 4) //pointer is null
#define LCD_ERROR_LCD_ID            (LCD_ERROR_BASE - 5) //lcd id is wrong
#define LCD_ERROR_LCD_DISPLAY_SIZE  (LCD_ERROR_BASE - 6) //lcd display size is wrong
#define LCD_ERROR_MODE              (LCD_ERROR_BASE - 7) //lcdc mode is wrong
#define LCD_ERROR_MODE_OPERATION    (LCD_ERROR_BASE - 8) //mode and operation are mismatches
#define LCD_ERROR_BLOCK_NUMBER      (LCD_ERROR_BASE - 9) //display block num is wrong
#define LCD_ERROR_ACCESS_MASK       (LCD_ERROR_BASE - 10)//can't access display block 
#define LCD_ERROR_RESOLUTION        (LCD_ERROR_BASE - 11)//display data resolution is wrong
#define LCD_ERROR_BLOCK_POSITION    (LCD_ERROR_BASE - 12)//block position is wrong
#define LCD_ERROR_TV_MODE           (LCD_ERROR_BASE - 13)//tv mode is wrong
#define LCD_ERROR_LCM_PIN_USE       (LCD_ERROR_BASE - 14)//LCM pin reuse is wrong
#define LCD_ERROR_LCM_RGBMODE       (LCD_ERROR_BASE - 15)//LCM RGB mode is wrong
#define LCD_ERROR_LCM_LCD_SUM       (LCD_ERROR_BASE - 16)//LCD sum is mismatch with LCD rgb mode for LCM
#define LCD_ERROR_CD_LEVEL          (LCD_ERROR_BASE - 17)//LCD cd level is wrong
#define LCD_ERROR_NO_SUPPORT        (LCD_ERROR_BASE - 18)//not support interface
#define LCD_ERROR_LCD_BITS          (LCD_ERROR_BASE - 19)//LCD bits error
#define LCD_ERROR_LCD_PIXEL_NUM     (LCD_ERROR_BASE - 20)//refresh pixels number is wrong
#define LCD_ERROR_NO_FOUND          (LCD_ERROR_BASE - 100)
/**********************************************************************************************************/

/**-----------------------------------------------------------------------------------------*
 **                         Compiler Flag                                                   *
 **-----------------------------------------------------------------------------------------*/
#ifdef __plusplus
{
#endif


#endif//end of _LCDC_DATA_TYPE_H

    //end of lcdc_data_type.h