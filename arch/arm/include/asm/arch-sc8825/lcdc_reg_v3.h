/******************************************************************************
 ** File Name:    lcdc_v3_reg.h                                     *
 ** Author:       Shan.He                                           *
 ** DATE:         01/22/2009                                        *
 ** Copyright:    2009 Spreatrum, Incoporated. All Rights Reserved. *
 ** Description:                                                    *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                               *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                       *

 ******************************************************************************/
#ifndef _LCDC_V3_REG_H_
#define _LCDC_V3_REG_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                           *
 **---------------------------------------------------------------------------*/
#include "lcm_reg_v3.h"
/**---------------------------------------------------------------------------*
 **                          Compiler Flag                          *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                      **
**----------------------------------------------------------------------------*/

#define REG_LCDC_BASE       LCDC_CTL_BEGIN

#define LCDC_CTRL                 (REG_LCDC_BASE + 0x0000)
#define LCDC_DISP_SIZE            (REG_LCDC_BASE + 0x0004)
#define LCDC_LCM_START            (REG_LCDC_BASE + 0x0008)
#define LCDC_LCM_SIZE             (REG_LCDC_BASE + 0x000c)
#define LCDC_BG_COLOR             (REG_LCDC_BASE + 0x0010)
#define LCDC_FIFO_STATUS          (REG_LCDC_BASE + 0x0014)


#define LCDC_IMG_CTRL             (REG_LCDC_BASE + 0x0020)
#define LCDC_IMG_Y_BASE_ADDR      (REG_LCDC_BASE + 0x0024)
#define LCDC_IMG_UV_BASE_ADDR     (REG_LCDC_BASE + 0x0028)
#define LCDC_IMG_SIZE_XY          (REG_LCDC_BASE + 0x002c)
#define LCDC_IMG_PITCH            (REG_LCDC_BASE + 0x0030)
#define LCDC_IMG_DISP_XY          (REG_LCDC_BASE + 0x0034)


#define LCDC_OSD1_CTRL            (REG_LCDC_BASE + 0x0050)  
#define LCDC_OSD1_BASE_ADDR       (REG_LCDC_BASE + 0x0054) 
#define LCDC_OSD1_ALPHA_BASE_ADDR (REG_LCDC_BASE + 0x0058) 
#define LCDC_OSD1_SIZE_XY         (REG_LCDC_BASE + 0x005c) 
#define LCDC_OSD1_PITCH           (REG_LCDC_BASE + 0x0060) 
#define LCDC_OSD1_DISP_XY         (REG_LCDC_BASE + 0x0064)  
#define LCDC_OSD1_ALPHA           (REG_LCDC_BASE + 0x0068) 
#define LCDC_OSD1_GREY_RGB        (REG_LCDC_BASE + 0x006c) 
#define LCDC_OSD1_CK              (REG_LCDC_BASE + 0x0070) 

#define LCDC_OSD2_CTRL            (REG_LCDC_BASE + 0x0080)  
#define LCDC_OSD2_BASE_ADDR       (REG_LCDC_BASE + 0x0084) 
#define LCDC_OSD2_SIZE_XY         (REG_LCDC_BASE + 0x008c) 
#define LCDC_OSD2_PITCH           (REG_LCDC_BASE + 0x0090) 
#define LCDC_OSD2_DISP_XY         (REG_LCDC_BASE + 0x0094)  
#define LCDC_OSD2_ALPHA           (REG_LCDC_BASE + 0x0098) 
#define LCDC_OSD2_GREY_RGB        (REG_LCDC_BASE + 0x009c) 
#define LCDC_OSD2_CK              (REG_LCDC_BASE + 0x00a0) 

#define LCDC_OSD3_CTRL            (REG_LCDC_BASE + 0x00b0)  
#define LCDC_OSD3_BASE_ADDR       (REG_LCDC_BASE + 0x00b4) 
#define LCDC_OSD3_SIZE_XY         (REG_LCDC_BASE + 0x00bc) 
#define LCDC_OSD3_PITCH           (REG_LCDC_BASE + 0x00c0) 
#define LCDC_OSD3_DISP_XY         (REG_LCDC_BASE + 0x00c4)  
#define LCDC_OSD3_ALPHA           (REG_LCDC_BASE + 0x00c8) 
#define LCDC_OSD3_GREY_RGB        (REG_LCDC_BASE + 0x00cc) 
#define LCDC_OSD3_CK              (REG_LCDC_BASE + 0x00d0) 

#define LCDC_OSD4_CTRL            (REG_LCDC_BASE + 0x00e0)  
#define LCDC_OSD4_BASE_ADDR       (REG_LCDC_BASE + 0x00e4) 
#define LCDC_OSD4_SIZE_XY         (REG_LCDC_BASE + 0x00ec) 
#define LCDC_OSD4_PITCH           (REG_LCDC_BASE + 0x00f0) 
#define LCDC_OSD4_DISP_XY         (REG_LCDC_BASE + 0x00f4)  
#define LCDC_OSD4_ALPHA           (REG_LCDC_BASE + 0x00f8) 
#define LCDC_OSD4_GREY_RGB        (REG_LCDC_BASE + 0x00fc) 
#define LCDC_OSD4_CK              (REG_LCDC_BASE + 0x0100) 

#define LCDC_OSD5_CTRL            (REG_LCDC_BASE + 0x0110)  
#define LCDC_OSD5_BASE_ADDR       (REG_LCDC_BASE + 0x0114) 
#define LCDC_OSD5_SIZE_XY         (REG_LCDC_BASE + 0x011c) 
#define LCDC_OSD5_PITCH           (REG_LCDC_BASE + 0x0120) 
#define LCDC_OSD5_DISP_XY         (REG_LCDC_BASE + 0x0124)  
#define LCDC_OSD5_ALPHA           (REG_LCDC_BASE + 0x0128) 
#define LCDC_OSD5_GREY_RGB        (REG_LCDC_BASE + 0x012c) 
#define LCDC_OSD5_CK              (REG_LCDC_BASE + 0x0130) 




#define LCDC_IRQ_EN               (REG_LCDC_BASE + 0x0170)  
#define LCDC_IRQ_CLR              (REG_LCDC_BASE + 0x0174)  
#define LCDC_IRQ_STATUS           (REG_LCDC_BASE + 0x0178)  
#define LCDC_IRQ_RAW              (REG_LCDC_BASE + 0x017c)  

/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/
#ifdef CHIP_ENDIAN_LITTLE
typedef union _lcdc_ctrl_tag
{
    struct _lcdc_ctrl_map
    {
        volatile unsigned int lcd_enable        :1; //[0] 0:LCDC Disable;1:LCDC Enable
        volatile unsigned int fmark_mode        :1; //[1] 0: fmark device; 1: non-fmark device
        volatile unsigned int fmark_pol         :1; //[2] 0: fmark valid at 1; 1: fmark valid at 0
        volatile unsigned int lcdc_run          :1; //[3] 0: stop; 1:run
        volatile unsigned int dither_en         :1; //[4] 0:disable; 1:enable
        volatile unsigned int reserved_2        :3;  //[7:5] Reserved ;
        volatile unsigned int req_gap           :8;  //[15:8] The interval between 2 AHB master requests for each client.
        volatile unsigned int reserved_1        :16; //[31:16] Reserved ;
    } mBits ;
    volatile unsigned int dwValue ;
} LCDC_CTRL_U;


typedef union _lcdc_disp_size_tag
{
    struct _lcdc_disp_size_map
    {
        volatile unsigned int disp_size_x       :10;  //[9:0] display window horizontal size, should be >0 and <1024
        volatile unsigned int reserved_2        :6;  //[15: 10] Reserved
        volatile unsigned int disp_size_y       :10;  //[25:16] display window vertical size, should be >0 and <1024
        volatile unsigned int reserved_1        :6;  //[31:26] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_DISP_SIZE_U;

typedef union _lcdc_lcm_start_tag
{
    struct _lcdc_lcm_start_map
    {
        volatile unsigned int lcm_start_x   :10;  //[9:0] LCM refresh window start X, should be >0 and <1024, guarantee the window in display region.
        volatile unsigned int reserved_2        :6;  //[15: 10] Reserved
        volatile unsigned int lcm_start_y   :10;  //[25:16] LCM refresh window start Y, should be >0 and <1024, guarantee the window in display region.
        volatile unsigned int reserved_1        :6;  //[31:26] Reserved
    } mBits;
    volatile unsigned int dValue;
} LCDC_LCM_START_U;

typedef union _lcdc_lcm_size_tag
{
    struct _lcdc_lcm_size_map
    {
        volatile unsigned int lcm_size_x        :10;  //[9:0] LCM refresh window horizontal size, should be >0 and <1024, guarantee the window in display region.
        volatile unsigned int reserved_2        :6;  //[15: 10] Reserved
        volatile unsigned int lcm_size_y        :10;  //[25:16] LCM refresh window vertical size, should be >0 and <1024, guarantee the window in display region.
        volatile unsigned int reserved_1        :6;  //[31:26] Reserved
    } mBits;
    volatile unsigned int dValue;
} LCDC_LCM_SIZE_U;

typedef union _lcdc_bg_color_tag
{
    struct _lcdc_bg_color_map
    {
        volatile unsigned int bg_b              :8; //[7:0] Background blue
        volatile unsigned int bg_g              :8; //[15:8] Background green
        volatile unsigned int bg_r              :8; //[23:16] Background red
        volatile unsigned int reserved          :8; //[31:24] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_BG_COLOR_U;

typedef union _lcdc_fifo_status_tag
{
    struct _lcdc_fifo_status_map
    {
        volatile unsigned int img_y_fifo_status     :2;  //[1:0] Image layer FIFO info. When image is YUV format, it is for Y data; when image is RGB format, it is for RGB data.
        //status, '1' for full; [0] - FIFO empty status, '1' for empty.
        volatile unsigned int img_uv_fifo_status    :2;  //[3:2] Image layer FIFO info. It is active when image is YUV422 and YUV420 format. It is for UV data.
        //[3] - FIFO full status, '1' for full; [2] - FIFO empty status, '1' for empty.
        volatile unsigned int osd1_alpha_fifo_status  :2; //[5:4] Osd1 layer FIFO info. It is active when osd1 is RGB565 format with pixel alpha.
        //[5] - FIFO full status, '1' for full; [4] - FIFO empty status, '1' for empty.
        volatile unsigned int osd1_fifo_status      :2;  //[7:6] Osd1 layer FIFO info. It is for osd1 layer data.
        //[7] - FIFO full status, '1' for full; [6] - FIFO empty status, '1' for empty.
        volatile unsigned int osd2_fifo_status      :2;  //[9:8] Osd2 layer FIFO info.It is for osd2 layer data.
        //[9] - FIFO full status, '1' for full; [8] - FIFO empty status, '1' for empty.
        volatile unsigned int osd3_fifo_status      :2;  //[11:10]
        volatile unsigned int osd4_fifo_status      :2;  //[13:12]
        volatile unsigned int osd5_fifo_status      :2;  //[15:14]
        volatile unsigned int y2r_fifo_status       :2;  //[17:16] YUV to RGB work FIFO
        //[11] - FIFO full status, '1' for full; [10] - FIFO empty status, '1' for empty.
        volatile unsigned int dither_fifo_status    :2;  //[19:18] Dithering output FIFO, it is shared by capture and display.
        //[13] - FIFO full status, '1' for full; [12]-FIFO empty status, '1' for empty.
        volatile unsigned int output_fifo_status    :2;  //[21:20] Cross domain FIFO;
        //[15] - FIFO full status, '1' for full; [14] - FIFO empty status, '1' for empty.
        volatile unsigned int reserved              :10;  //[31:16] reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_FIFO_STATUS_U;

typedef union _img_ctrl_tag
{
    struct _img_ctrl_map
    {
        volatile unsigned int img_en            :1;     //[0] Image layer enable; 0: disable; 1: enable
        volatile unsigned int img_format        :4;     //[1:4] Image layer data format, it supports following ones:
        //0000-YUV422;
        //0001-YUV420;
        //0010-YUV400;
        //0011-RGB888;
        //0100-RGB666;
        //0101-RGB565;
        //0110-RGB555;
        volatile unsigned int img_y_endian      :2;     //[5:6] Image layer data endian; 0: big endian(0123); 1: little endian(3210)
        volatile unsigned int img_uv_endian     :2;           //[7:8]
        volatile unsigned int reserved          :23;    //[31:9]
    } mBits;
    volatile unsigned int dwValue;
} IMG_CTRL_U;

typedef union _img_y_base_addr_tag
{
    struct _img_y_base_addr_map
    {
        volatile unsigned int img_y_base_addr   :30;    //[29:0]  //When image is YUV format, it is Y data base address;
        volatile unsigned int reserved          :2;     //[31:30]
    } mBits;
    volatile unsigned int dwValue;
} IMG_Y_BASE_ADDR_U;

typedef union _img_uv_base_addr_tag
{
    struct _img_uv_base_addr_map
    {
        volatile unsigned int img_uv_base_addr  :30;    //[29:0]  //When image is YUV format, it is UV base address;
        volatile unsigned int reserved          :2;     //[31:30]
    } mBits;
    volatile unsigned int dwValue;
} IMG_UV_BASE_ADDR_U;

typedef union _img_size_xy_tag
{
    struct _img_size_xy_map
    {
        volatile unsigned int img_size_x        :10;  //[9:0] Image layer window size in X, should be >0 and <1024
        volatile unsigned int reserved_2        :6;   //[15: 10] Reserved
        volatile unsigned int img_size_y        :10;  //[25:16] Image layer window size in Y, should be >0 and <1024
        volatile unsigned int reserved_1        :6;   //[31:26] Reserved
    } mBits;
    volatile unsigned int dwValue;
} IMG_SIZE_XY_U;

typedef union _img_pitch_tag
{
    struct _img_pitch_map
    {
        volatile unsigned int img_pitch         :10;      //[9:0] Image layer data storage pitch, should be >0 and <1024.
        volatile unsigned int reserved_1        :22;      //[31:10] Reserved
    } mBits;
    volatile unsigned int dwValue;
} IMG_PITCH_U;

typedef union _img_disp_xy_tag
{
    struct _img_disp_xy_map
    {
        volatile unsigned int img_disp_x        :10;  //[9:0] Image layer window start X position in display plane, should be >0 and <1024
        volatile unsigned int reserved_2        :6;   //[15: 10] Reserved
        volatile unsigned int img_disp_y        :10;  //[25:16] Image layer window start Y position in display plane, should be >0 and <1024
        volatile unsigned int reserved_1        :6;   //[31:26] Reserved
    } mBits;
    volatile unsigned int dwValue;
} IMG_DISP_XY_U;

typedef union _osd1_ctrl_tag
{
    struct _osd1_ctrl_map
    {
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_alpha_sel     :1;     //[1] OSD layer1 block1 alpha selection, 0: pixel alpha; 1: block alpha
        volatile unsigned int blk_format        :4;     //[6:3] osd data format
        volatile unsigned int blk_endian        :2;     //[7] block data endian, 0: big endian (0123); 1: little endian (3210)
        volatile unsigned int alpha_endian      :2;
        volatile unsigned int reserved          :21;    //[31:8] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD1_CTRL_U;

typedef union _osd2_ctrl_tag
{
    struct _osd2_ctrl_map
    {
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_alpha_sel     :1;     //[2] OSD layer1 block1 alpha selection, 0: pixel alpha; 1: block alpha
        volatile unsigned int blk_format        :4;     //[6:3] osd data format
        volatile unsigned int blk_endian        :2;     //[8:7] block data endian, 0: big endian (0123); 1: little endian (3210)
        volatile unsigned int reserved          :23;    //[31:10] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD2_CTRL_U;

typedef union _osd3_ctrl_tag
{
    struct _osd3_ctrl_map
    {
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_alpha_sel     :1;     //[2] OSD layer1 block1 alpha selection, 0: pixel alpha; 1: block alpha
        volatile unsigned int blk_format        :4;     //[6:3] osd data format
        volatile unsigned int blk_endian        :2;     //[8:7] block data endian, 0: big endian (0123); 1: little endian (3210)
        volatile unsigned int reserved          :23;    //[31:10] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD3_CTRL_U;

typedef union _osd4_ctrl_tag
{
    struct _osd4_ctrl_map
    {
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_alpha_sel     :1;     //[2] OSD layer1 block1 alpha selection, 0: pixel alpha; 1: block alpha
        volatile unsigned int blk_format        :4;     //[6:3] osd data format
        volatile unsigned int blk_endian        :2;     //[8:7] block data endian, 0: big endian (0123); 1: little endian (3210)
        volatile unsigned int reserved          :23;    //[31:10] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD4_CTRL_U;

typedef union _osd5_ctrl_tag
{
    struct _osd5_ctrl_map
    {
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_alpha_sel     :1;     //[2] OSD layer1 block1 alpha selection, 0: pixel alpha; 1: block alpha
        volatile unsigned int blk_format        :4;     //[6:3] osd data format
        volatile unsigned int blk_endian        :2;     //[8:7] block data endian, 0: big endian (0123); 1: little endian (3210)
        volatile unsigned int reserved          :24;    //[31:9] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD5_CTRL_U;

typedef union _osd_ctrl_tag
{
    struct _osd_ctrl_map
    {
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_alpha_sel     :1;     //[2] OSD layer1 block1 alpha selection, 0: pixel alpha; 1: block alpha
        volatile unsigned int blk_format        :4;     //[6:3] osd data format
        volatile unsigned int blk_endian        :2;     //[8:7] block data endian, 0: big endian (0123); 1: little endian (3210)
        volatile unsigned int reserved          :23;    //[31:9] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_CTRL_U;

typedef union _osd_colorkey_rgb_tag
{
    struct _osd_colorkey_map
    {
        volatile unsigned int osd_ck            :24;    //[23:0] Color-key value in OSD layer
        volatile unsigned int reserved          :8;     //[31:24] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_COLORKEY_U;


typedef union _osd_base_addr_ctrl_tag
{
    struct _osd_base_addr_map
    {
        volatile unsigned int blk_base_addr     :30;    //[29:0] block base address
        volatile unsigned int reserved          :2;     //[31:30] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_BASE_ADDR_U;

typedef union _osd_alpha_base_addr_ctrl_tag
{
    struct _osd_alpha_base_addr_map
    {
        volatile unsigned int blk_alpha_base_addr   :30;    //[29:0] block base address
        volatile unsigned int reserved              :2;     //[31:30] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_ALPHA_BASE_ADDR_U;

typedef union _osd_size_xy_tag
{
    struct _osd_size_xy_map
    {
        volatile unsigned int blk_size_x        :10;  //[9:0] Image layer window size in X, should be >0 and <1024
        volatile unsigned int reserved_2        :6;   //[15: 10] Reserved
        volatile unsigned int blk_size_y        :10;  //[25:16] Image layer window size in Y, should be >0 and <1024
        volatile unsigned int reserved_1        :6;   //[31:26] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_SIZE_XY_U;

typedef union _osd_pitch_tag
{
    struct _osd_pitch_map
    {
        volatile unsigned int blk_pitch         :10;      //[9:0] Image layer data storage pitch, should be >0 and <1024.
        volatile unsigned int reserved_1        :22;      //[31:10] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_PITCH_U;

typedef union _osd_disp_xy_tag
{
    struct _osd_disp_xy_map
    {
        volatile unsigned int blk_disp_x    :10;  //[9:0] Image layer window start X position in display plane, should be >0 and <1024
        volatile unsigned int reserved_2    :6;   //[15: 10] Reserved
        volatile unsigned int blk_disp_y    :10;  //[25:16] Image layer window start Y position in display plane, should be >0 and <1024
        volatile unsigned int reserved_1    :6;   //[31:26] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_DISP_XY_U;

typedef union _osd_alpha_tag
{
    struct _osd_alpha_map
    {
        volatile unsigned int blk_alpha         :8;       //[7:0] Block alpha for block1 in OSD layer1, it is in 0~255
        volatile unsigned int reserved_1        :24;      //[31:8] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_ALPHA_U;

typedef union _osd_grey_rgb_tag
{
    struct _osd_grey_rgb_map
    {
        volatile unsigned int blk_grey_rgb      :24;    //[23:0] Constant RGB for GREY data format.
        volatile unsigned int reserved_1        :8;     //[31:24] Reserved
    } mBits;
    volatile unsigned int dwValue;
} OSD_GREY_RGB_U;

typedef union _lcdc_cap_ctrl_tag
{
    struct _lcdc_cap_ctrl_map
    {
        volatile unsigned int cap_en            :1;     //[1] Capture blended data control, 0: disable, 1: enable
        volatile unsigned int cap_format        :2;     //[2:1] data save format, 00: RGB888, 01: rgb666, 10: RGB565, 11: reserved
        volatile unsigned int cap_endian        :2;     //[3] Capture endian, 0: big endian, 1: little endian
        volatile unsigned int reserved          :27;     //[31:4] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_CAP_CTRL_U;

typedef union _cap_base_addr_tag
{
    struct _cap_base_addr_map
    {
        volatile unsigned int base_addr         :30;    //[29:0] Capture base address
        volatile unsigned int reserved          :2;     //[31:30] Reserved
    } mBits;
    volatile unsigned int dwValue;
} CAP_BASE_ADDR_U;

typedef union _cap_size_xy_tag
{
    struct _cap_size_xy_map
    {
        volatile unsigned int cap_size_x        :10;  //[9:0] Image layer window size in X, should be >0 and <1024
        volatile unsigned int reserved_2        :6;   //[15: 10] Reserved
        volatile unsigned int cap_size_y        :10;  //[25:16] Image layer window size in Y, should be >0 and <1024
        volatile unsigned int reserved_1        :6;   //[31:26] Reserved
    } mBits;
    volatile unsigned int dwValue;
} CAP_SIZE_XY_U;

typedef union _cap_pitch_tag
{
    struct _cap_pitch_map
    {
        volatile unsigned int cap_pitch         :10;      //[9:0] Image layer data storage pitch, should be >0 and <1024.
        volatile unsigned int reserved          :22;      //[31:10] Reserved
    } mBits;
    volatile unsigned int dwValue;
} CAP_PITCH_U;

typedef union _cap_start_xy_tag
{
    struct _cap_start_xy_map
    {
        volatile unsigned int cap_start_x       :10;  //[9:0] Image layer window start X position in display plane, should be >0 and <1024
        volatile unsigned int reserved_2        :6;   //[15: 10] Reserved
        volatile unsigned int cap_start_y       :10;  //[25:16] Image layer window start Y position in display plane, should be >0 and <1024
        volatile unsigned int reserved_1        :6;   //[31:26] Reserved
    } mBits;
    volatile unsigned int dwValue;
} CAP_START_XY_U;

typedef union _y2r_ctrl_tag
{
    struct _y2r_ctrl_map
    {
        volatile unsigned int upsample_mode     :1;     //[0] UV horizontal up-sampling mode, 0: duplicate, 1: average
        volatile unsigned int reserved          :31;    //[31:1] Reserved
    } mBits;
    volatile unsigned int dwValue;
} Y2R_CTRL_U;

typedef union _y2r_contrast_tag
{
    struct _y2r_contrast_map
    {
        volatile unsigned int contrast          :8;     //[7:0] Contrast config, 0~255
        volatile unsigned int reserved          :24;    //[31:8] Reserved
    } mBits;
    volatile unsigned int dwValue;
} Y2R_CONTRAST_U;

typedef union _y2r_saturation_tag
{
    struct _y2r_saturation_map
    {
        volatile unsigned int saturation        :8;     //[7:0] Saturation config, 0~255
        volatile unsigned int reserved          :24;    //[31:8] Reserved
    } mBits;
    volatile unsigned int dwValue;
} Y2R_SATURATION_U;

typedef union _y2r_brightness_tag
{
    struct _y2r_brightness_map
    {
        volatile unsigned int brightness        :9;     //[8:0] Brightness config (S1.8), -256~255
        volatile unsigned int reserved          :23;    //[31:9] Reserved
    } mBits;
    volatile unsigned int dwValue;
} Y2R_BRIGHTNESS_U;


typedef union _lcdc_irq_en_tag
{
    struct _lcdc_irq_en_map
    {
        volatile unsigned int irq_lcdc_done_en  :1;     //[0] Enable LCDC_DONE interrupt
        volatile unsigned int irq_lcm_eof_en    :1;     //[1] Enable FRAME_END Interrupt
        volatile unsigned int irq_lcm_sof_en    :1;     //[2] Enable CAP_DONE interrupt
        volatile unsigned int irq_fmark_en      :1;     //[3] Enable FMARK interrupt, the interrupt is for LCDC detect a FMARK input.
        volatile unsigned int reserved          :28;    //[31:3] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_EN_U;

typedef union _lcdc_irq_clr_tag
{
    struct _lcdc_irq_clr_map
    {
        volatile unsigned int irq_lcdc_done_clr :1;     //[0] Write '1' to clear the DISP_DONE interrupt bit, and itself is cleared by HW.
        volatile unsigned int irq_lcm_eof_clr   :1;     //[1] Write '1' to clear the FRAME_END interrupt bit, and itself is cleared by HW.
        volatile unsigned int irq_lcm_sof_clr   :1;     //[2] Write '1' to clear the CAP_DONE interrupt bit, and itself is cleared by HW.
        volatile unsigned int irq_fmark_clr     :1;     //[3] Write '1' to clear the FMARK interrupt bit, and itself is cleared by HW.
        volatile unsigned int reserved          :28;    //[31:3] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_CLR_U;


typedef union _lcdc_irq_status_tag
{
    struct _lcdc_irq_status_map
    {
        volatile unsigned int irq_lcdc_done_status  :1;     //[0] DISP_DONE interrupt status
        volatile unsigned int irq_lcm_eof_status    :1;     //[1] FRAME_END interrupt status
        volatile unsigned int irq_lcm_sof_status    :1;     //[2] CAP DONE interrupt status
        volatile unsigned int irq_fmark_status      :1;     //[3] FMARK interrupt status
        volatile unsigned int reserved              :28;    //[31:3] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_STATUS_U;

typedef union _lcdc_irq_raw_tag
{
    struct _lcdc_irq_raw_map
    {
        volatile unsigned int irq_lcdc_done_raw :1;     //[0] DISP_DONE raw interrupt
        volatile unsigned int irq_lcm_eof_raw   :1;     //[1] FRAME_END raw interrupt
        volatile unsigned int irq_lcm_sof_raw   :1;     //[2] CAP DONE raw interrupt
        volatile unsigned int irq_fmark_raw     :1;     //[3] FMARK raw interrupt
        volatile unsigned int reserved          :28;    //[31:3] Reserved
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_RAW_U;
#else
typedef union _lcdc_ctrl_tag
{
    struct _lcdc_ctrl_map
    {
        volatile unsigned int reserved_1        :16; //[31:16] Reserved ;
        volatile unsigned int req_gap           :8;  //[15:8] The interval between 2 AHB master requests for each client.
        volatile unsigned int reserved_2        :3;  //[7:5] Reserved ;
        volatile unsigned int dither_en         :1; //[4] 0:disable; 1:enable
        volatile unsigned int lcdc_run          :1; //[3] 0: stop; 1:run
        volatile unsigned int fmark_pol         :1; //[2] 0: fmark valid at 1; 1: fmark valid at 0
        volatile unsigned int fmark_mode        :1; //[1] 0: fmark device; 1: non-fmark device
        volatile unsigned int lcd_enable        :1; //[0] 0:LCDC Disable;1:LCDC Enable
    } mBits ;
    volatile unsigned int dwValue ;
} LCDC_CTRL_U;


typedef union _lcdc_disp_size_tag
{
    struct _lcdc_disp_size_map
    {
        volatile unsigned int reserved_1        :6;  //[31:26] Reserved
        volatile unsigned int disp_size_y       :10;  //[25:16] display window vertical size, should be >0 and <1024
        volatile unsigned int reserved_2        :6;  //[15: 10] Reserved
        volatile unsigned int disp_size_x       :10;  //[9:0] display window horizontal size, should be >0 and <1024
    } mBits;
    volatile unsigned int dwValue;
} LCDC_DISP_SIZE_U;

typedef union _lcdc_lcm_start_tag
{
    struct _lcdc_lcm_start_map
    {
        volatile unsigned int reserved_1        :6;  //[31:26] Reserved
        volatile unsigned int lcm_start_y   :10;  //[25:16] LCM refresh window start Y, should be >0 and <1024, guarantee the window in display region.
        volatile unsigned int reserved_2        :6;  //[15: 10] Reserved
        volatile unsigned int lcm_start_x   :10;  //[9:0] LCM refresh window start X, should be >0 and <1024, guarantee the window in display region.
    } mBits;
    volatile unsigned int dValue;
} LCDC_LCM_START_U;

typedef union _lcdc_lcm_size_tag
{
    struct _lcdc_lcm_size_map
    {
        volatile unsigned int reserved_1        :6;  //[31:26] Reserved
        volatile unsigned int lcm_size_y        :10;  //[25:16] LCM refresh window vertical size, should be >0 and <1024, guarantee the window in display region.
        volatile unsigned int reserved_2        :6;  //[15: 10] Reserved
        volatile unsigned int lcm_size_x        :10;  //[9:0] LCM refresh window horizontal size, should be >0 and <1024, guarantee the window in display region.
    } mBits;
    volatile unsigned int dValue;
} LCDC_LCM_SIZE_U;

typedef union _lcdc_bg_color_tag
{
    struct _lcdc_bg_color_map
    {
        volatile unsigned int reserved          :8; //[31:24] Reserved
        volatile unsigned int bg_r              :8; //[23:16] Background red
        volatile unsigned int bg_g              :8; //[15:8] Background green
        volatile unsigned int bg_b              :8; //[7:0] Background blue
    } mBits;
    volatile unsigned int dwValue;
} LCDC_BG_COLOR_U;

typedef union _lcdc_fifo_status_tag
{
    struct _lcdc_fifo_status_map
    {
        volatile unsigned int reserved              :10;  //[31:16] reserved
        volatile unsigned int output_fifo_status    :2;  //[21:20] Cross domain FIFO;
        //[15] - FIFO full status, '1' for full; [14] - FIFO empty status, '1' for empty.
        volatile unsigned int dither_fifo_status    :2;  //[19:18] Dithering output FIFO, it is shared by capture and display.
        //[13] - FIFO full status, '1' for full; [12]-FIFO empty status, '1' for empty.
        volatile unsigned int y2r_fifo_status       :2;  //[17:16] YUV to RGB work FIFO
        //[11] - FIFO full status, '1' for full; [10] - FIFO empty status, '1' for empty.
        volatile unsigned int osd5_fifo_status        :2;  //[15:14]
        volatile unsigned int osd4_fifo_status      :2;  //[13:12]
        volatile unsigned int osd3_fifo_status      :2;  //[11:10]
        volatile unsigned int osd2_fifo_status      :2;  //[9:8] Osd2 layer FIFO info.It is for osd2 layer data.
        //[9] - FIFO full status, '1' for full; [8] - FIFO empty status, '1' for empty.
        volatile unsigned int osd1_fifo_status      :2;  //[7:6] Osd1 layer FIFO info. It is for osd1 layer data.
        //[7] - FIFO full status, '1' for full; [6] - FIFO empty status, '1' for empty.
        volatile unsigned int osd1_alpha_fifo_status  :2; //[5:4] Osd1 layer FIFO info. It is active when osd1 is RGB565 format with pixel alpha.
        //[5] - FIFO full status, '1' for full; [4] - FIFO empty status, '1' for empty.
        volatile unsigned int img_uv_fifo_status    :2;  //[3:2] Image layer FIFO info. It is active when image is YUV422 and YUV420 format. It is for UV data.
        //[3] - FIFO full status, '1' for full; [2] - FIFO empty status, '1' for empty.
        volatile unsigned int img_y_fifo_status     :2;  //[1:0] Image layer FIFO info. When image is YUV format, it is for Y data; when image is RGB format, it is for RGB data.
        //status, '1' for full; [0] - FIFO empty status, '1' for empty.
    } mBits;
    volatile unsigned int dwValue;
} LCDC_FIFO_STATUS_U;

typedef union _img_ctrl_tag
{
    struct _img_ctrl_map
    {
        volatile unsigned int reserved            :23;    //[31:6]
        volatile unsigned int img_y_endian        :2;
        volatile unsigned int img_uv_endian      :2;
        volatile unsigned int img_format        :4;     //[1:4] Image layer data format, it supports following ones:
        //0000-YUV422;
        //0001-YUV420;
        //0010-YUV400;
        //0011-RGB888;
        //0100-RGB666;
        //0101-RGB565;
        //0110-RGB555;

        volatile unsigned int img_en            :1;     //[0] Image layer enable; 0: disable; 1: enable

    } mBits;
    volatile unsigned int dwValue;
} IMG_CTRL_U;

typedef union _img_y_base_addr_tag
{
    struct _img_y_base_addr_map
    {
        volatile unsigned int reserved          :2;     //[31:30]
        volatile unsigned int img_y_base_addr   :30;    //[29:0]  //When image is YUV format, it is Y data base address;
    } mBits;
    volatile unsigned int dwValue;
} IMG_Y_BASE_ADDR_U;

typedef union _img_uv_base_addr_tag
{
    struct _img_uv_base_addr_map
    {
        volatile unsigned int reserved          :2;     //[31:30]
        volatile unsigned int img_uv_base_addr  :30;    //[29:0]  //When image is YUV format, it is UV base address;
    } mBits;
    volatile unsigned int dwValue;
} IMG_UV_BASE_ADDR_U;

typedef union _img_size_xy_tag
{
    struct _img_size_xy_map
    {
        volatile unsigned int reserved_1        :6;   //[31:26] Reserved
        volatile unsigned int img_size_y        :10;  //[25:16] Image layer window size in Y, should be >0 and <1024
        volatile unsigned int reserved_2        :6;   //[15: 10] Reserved
        volatile unsigned int img_size_x        :10;  //[9:0] Image layer window size in X, should be >0 and <1024
    } mBits;
    volatile unsigned int dwValue;
} IMG_SIZE_XY_U;

typedef union _img_pitch_tag
{
    struct _img_pitch_map
    {
        volatile unsigned int reserved_1        :22;      //[31:10] Reserved
        volatile unsigned int img_pitch         :10;      //[9:0] Image layer data storage pitch, should be >0 and <1024.
    } mBits;
    volatile unsigned int dwValue;
} IMG_PITCH_U;

typedef union _img_disp_xy_tag
{
    struct _img_disp_xy_map
    {
        volatile unsigned int reserved_1        :6;   //[31:26] Reserved
        volatile unsigned int img_disp_y        :10;  //[25:16] Image layer window start Y position in display plane, should be >0 and <1024
        volatile unsigned int reserved_2        :6;   //[15: 10] Reserved
        volatile unsigned int img_disp_x        :10;  //[9:0] Image layer window start X position in display plane, should be >0 and <1024
    } mBits;
    volatile unsigned int dwValue;
} IMG_DISP_XY_U;

typedef union _osd1_ctrl_tag
{
    struct _osd1_ctrl_map
    {
        volatile unsigned int reserved          :21;    //[31:8] Reserved
        volatile unsigned int blk_endian        :2;     //[7] block data endian, 0: big endian (0123); 1: little endian (3210)
        volatile unsigned int alpha_endian        :2;
        volatile unsigned int blk_format        :4;     //[6:3] osd data format
        volatile unsigned int blk_alpha_sel     :1;     //[1] OSD layer1 block1 alpha selection, 0: pixel alpha; 1: block alpha
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
    } mBits;
    volatile unsigned int dwValue;
} OSD1_CTRL_U;

typedef union _osd2_ctrl_tag
{
    struct _osd2_ctrl_map
    {
        volatile unsigned int reserved          :23;    //[31:10] Reserved
        volatile unsigned int blk_endian        :2;     //[9] block data endian, 0: big endian (0123); 1: little endian (3210)
        volatile unsigned int blk_format        :4;     //[6:3] osd data format
        volatile unsigned int blk_alpha_sel     :1;     //[2] OSD layer1 block1 alpha selection, 0: pixel alpha; 1: block alpha
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
    } mBits;
    volatile unsigned int dwValue;
} OSD2_CTRL_U;

typedef union _osd3_ctrl_tag
{
    struct _osd3_ctrl_map
    {
        volatile unsigned int reserved          :23;    //[31:10] Reserved
        volatile unsigned int blk_endian        :2;
        volatile unsigned int blk_format        :4;     //[6:3] osd data format
        volatile unsigned int blk_alpha_sel     :1;     //[2] OSD layer1 block1 alpha selection, 0: pixel alpha; 1: block alpha
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
    } mBits;
    volatile unsigned int dwValue;
} OSD3_CTRL_U;

typedef union _osd4_ctrl_tag
{
    struct _osd4_ctrl_map
    {
        volatile unsigned int reserved          :23;    //[31:10] Reserved
        volatile unsigned int blk_endian        :2;
        volatile unsigned int blk_format        :4;     //[6:3] osd data format
        volatile unsigned int blk_alpha_sel     :1;     //[2] OSD layer1 block1 alpha selection, 0: pixel alpha; 1: block alpha
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
    } mBits;
    volatile unsigned int dwValue;
} OSD4_CTRL_U;

typedef union _osd5_ctrl_tag
{
    struct _osd5_ctrl_map
    {
        volatile unsigned int reserved          :23;    //[31:10] Reserved
        volatile unsigned int blk_endian        :2;
        volatile unsigned int blk_format        :4;     //[6:3] osd data format
        volatile unsigned int blk_alpha_sel     :1;     //[2] OSD layer1 block1 alpha selection, 0: pixel alpha; 1: block alpha
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
    } mBits;
    volatile unsigned int dwValue;
} OSD5_CTRL_U;

typedef union _osd_ctrl_tag
{
    struct _osd_ctrl_map
    {
        volatile unsigned int reserved          :23;    //[31:10] Reserved
        volatile unsigned int blk_endian        :2;
        volatile unsigned int blk_format        :4;     //[6:3] osd data format
        volatile unsigned int blk_alpha_sel     :1;     //[2] OSD layer1 block1 alpha selection, 0: pixel alpha; 1: block alpha
        volatile unsigned int blk_ck_en         :1;     //[1] block color key enable, 0: disable; 1: enable
        volatile unsigned int blk_en            :1;     //[0] block enable, 0: disable; 1: enable
    } mBits;
    volatile unsigned int dwValue;
} OSD_CTRL_U;

typedef union _osd_colorkey_rgb_tag
{
    struct _osd_colorkey_map
    {
        volatile unsigned int reserved          :8;     //[31:24] Reserved
        volatile unsigned int osd_ck            :24;    //[23:0] Color-key value in OSD layer
    } mBits;
    volatile unsigned int dwValue;
} OSD_COLORKEY_U;


typedef union _osd_base_addr_ctrl_tag
{
    struct _osd_base_addr_map
    {
        volatile unsigned int reserved          :2;     //[31:30] Reserved
        volatile unsigned int blk_base_addr     :30;    //[29:0] block base address
    } mBits;
    volatile unsigned int dwValue;
} OSD_BASE_ADDR_U;

typedef union _osd_alpha_base_addr_ctrl_tag
{
    struct _osd_alpha_base_addr_map
    {
        volatile unsigned int reserved              :2;     //[31:30] Reserved
        volatile unsigned int blk_alpha_base_addr   :30;    //[29:0] block base address
    } mBits;
    volatile unsigned int dwValue;
} OSD_ALPHA_BASE_ADDR_U;

typedef union _osd_size_xy_tag
{
    struct _osd_size_xy_map
    {
        volatile unsigned int reserved_1        :6;   //[31:26] Reserved
        volatile unsigned int blk_size_y        :10;  //[25:16] Image layer window size in Y, should be >0 and <1024
        volatile unsigned int reserved_2        :6;   //[15: 10] Reserved
        volatile unsigned int blk_size_x        :10;  //[9:0] Image layer window size in X, should be >0 and <1024
    } mBits;
    volatile unsigned int dwValue;
} OSD_SIZE_XY_U;

typedef union _osd_pitch_tag
{
    struct _osd_pitch_map
    {
        volatile unsigned int reserved_1        :22;      //[31:10] Reserved
        volatile unsigned int blk_pitch         :10;      //[9:0] Image layer data storage pitch, should be >0 and <1024.
    } mBits;
    volatile unsigned int dwValue;
} OSD_PITCH_U;

typedef union _osd_disp_xy_tag
{
    struct _osd_disp_xy_map
    {
        volatile unsigned int reserved_1    :6;   //[31:26] Reserved
        volatile unsigned int blk_disp_y    :10;  //[25:16] Image layer window start Y position in display plane, should be >0 and <1024
        volatile unsigned int reserved_2    :6;   //[15: 10] Reserved
        volatile unsigned int blk_disp_x    :10;  //[9:0] Image layer window start X position in display plane, should be >0 and <1024
    } mBits;
    volatile unsigned int dwValue;
} OSD_DISP_XY_U;

typedef union _osd_alpha_tag
{
    struct _osd_alpha_map
    {
        volatile unsigned int reserved_1        :24;      //[31:8] Reserved
        volatile unsigned int blk_alpha         :8;       //[7:0] Block alpha for block1 in OSD layer1, it is in 0~255
    } mBits;
    volatile unsigned int dwValue;
} OSD_ALPHA_U;

typedef union _osd_grey_rgb_tag
{
    struct _osd_grey_rgb_map
    {
        volatile unsigned int reserved_1        :8;     //[31:24] Reserved
        volatile unsigned int blk_grey_rgb      :24;    //[23:0] Constant RGB for GREY data format.
    } mBits;
    volatile unsigned int dwValue;
} OSD_GREY_RGB_U;

typedef union _lcdc_cap_ctrl_tag
{
    struct _lcdc_cap_ctrl_map
    {
        volatile unsigned int reserved          :27;     //[31:4] Reserved
        volatile unsigned int cap_endian        :2;
        volatile unsigned int cap_format        :2;     //[2:1] data save format, 00: RGB888, 01: rgb666, 10: RGB565, 11: reserved
        volatile unsigned int cap_en            :1;     //[1] Capture blended data control, 0: disable, 1: enable
    } mBits;
    volatile unsigned int dwValue;
} LCDC_CAP_CTRL_U;

typedef union _cap_base_addr_tag
{
    struct _cap_base_addr_map
    {
        volatile unsigned int reserved          :2;     //[31:30] Reserved
        volatile unsigned int base_addr         :30;    //[29:0] Capture base address
    } mBits;
    volatile unsigned int dwValue;
} CAP_BASE_ADDR_U;

typedef union _cap_size_xy_tag
{
    struct _cap_size_xy_map
    {
        volatile unsigned int reserved_1        :6;   //[31:26] Reserved
        volatile unsigned int cap_size_y        :10;  //[25:16] Image layer window size in Y, should be >0 and <1024
        volatile unsigned int reserved_2        :6;   //[15: 10] Reserved
        volatile unsigned int cap_size_x        :10;  //[9:0] Image layer window size in X, should be >0 and <1024
    } mBits;
    volatile unsigned int dwValue;
} CAP_SIZE_XY_U;

typedef union _cap_pitch_tag
{
    struct _cap_pitch_map
    {
        volatile unsigned int reserved          :22;      //[31:10] Reserved
        volatile unsigned int cap_pitch         :10;      //[9:0] Image layer data storage pitch, should be >0 and <1024.
    } mBits;
    volatile unsigned int dwValue;
} CAP_PITCH_U;

typedef union _cap_start_xy_tag
{
    struct _cap_start_xy_map
    {
        volatile unsigned int reserved_1        :6;   //[31:26] Reserved
        volatile unsigned int cap_start_y       :10;  //[25:16] Image layer window start Y position in display plane, should be >0 and <1024
        volatile unsigned int reserved_2        :6;   //[15: 10] Reserved
        volatile unsigned int cap_start_x       :10;  //[9:0] Image layer window start X position in display plane, should be >0 and <1024
    } mBits;
    volatile unsigned int dwValue;
} CAP_START_XY_U;

typedef union _y2r_ctrl_tag
{
    struct _y2r_ctrl_map
    {
        volatile unsigned int reserved          :31;    //[31:1] Reserved
        volatile unsigned int upsample_mode     :1;     //[0] UV horizontal up-sampling mode, 0: duplicate, 1: average
    } mBits;
    volatile unsigned int dwValue;
} Y2R_CTRL_U;

typedef union _y2r_contrast_tag
{
    struct _y2r_contrast_map
    {
        volatile unsigned int reserved          :24;    //[31:8] Reserved
        volatile unsigned int contrast          :8;     //[7:0] Contrast config, 0~255
    } mBits;
    volatile unsigned int dwValue;
} Y2R_CONTRAST_U;

typedef union _y2r_saturation_tag
{
    struct _y2r_saturation_map
    {
        volatile unsigned int reserved          :24;    //[31:8] Reserved
        volatile unsigned int saturation        :8;     //[7:0] Saturation config, 0~255
    } mBits;
    volatile unsigned int dwValue;
} Y2R_SATURATION_U;

typedef union _y2r_brightness_tag
{
    struct _y2r_brightness_map
    {
        volatile unsigned int reserved          :23;    //[31:9] Reserved
        volatile unsigned int brightness        :9;     //[8:0] Brightness config (S1.8), -256~255
    } mBits;
    volatile unsigned int dwValue;
} Y2R_BRIGHTNESS_U;


typedef union _lcdc_irq_en_tag
{
    struct _lcdc_irq_en_map
    {
        volatile unsigned int reserved          :28;    //[31:3] Reserved
        volatile unsigned int irq_fmark_en      :1;     //[3] Enable FMARK interrupt, the interrupt is for LCDC detect a FMARK input.
        volatile unsigned int irq_lcm_sof_en    :1;     //[2] Enable CAP_DONE interrupt
        volatile unsigned int irq_lcm_eof_en    :1;     //[1] Enable FRAME_END Interrupt
        volatile unsigned int irq_lcdc_done_en  :1;     //[0] Enable LCDC_DONE interrupt
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_EN_U;

typedef union _lcdc_irq_clr_tag
{
    struct _lcdc_irq_clr_map
    {
        volatile unsigned int reserved          :28;    //[31:3] Reserved
        volatile unsigned int irq_fmark_clr     :1;     //[3] Write '1' to clear the FMARK interrupt bit, and itself is cleared by HW.
        volatile unsigned int irq_lcm_sof_clr   :1;     //[2] Write '1' to clear the CAP_DONE interrupt bit, and itself is cleared by HW.
        volatile unsigned int irq_lcm_eof_clr   :1;     //[1] Write '1' to clear the FRAME_END interrupt bit, and itself is cleared by HW.
        volatile unsigned int irq_lcdc_done_clr :1;     //[0] Write '1' to clear the DISP_DONE interrupt bit, and itself is cleared by HW.
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_CLR_U;


typedef union _lcdc_irq_status_tag
{
    struct _lcdc_irq_status_map
    {
        volatile unsigned int reserved              :28;    //[31:3] Reserved
        volatile unsigned int irq_fmark_status      :1;     //[3] FMARK interrupt status
        volatile unsigned int irq_lcm_sof_status    :1;     //[2] CAP DONE interrupt status
        volatile unsigned int irq_lcm_eof_status    :1;     //[1] FRAME_END interrupt status
        volatile unsigned int irq_lcdc_done_status  :1;     //[0] DISP_DONE interrupt status
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_STATUS_U;

typedef union _lcdc_irq_raw_tag
{
    struct _lcdc_irq_raw_map
    {
        volatile unsigned int reserved          :28;    //[31:3] Reserved
        volatile unsigned int irq_fmark_raw     :1;     //[3] FMARK raw interrupt
        volatile unsigned int irq_lcm_sof_raw   :1;     //[2] CAP DONE raw interrupt
        volatile unsigned int irq_lcm_eof_raw   :1;     //[1] FRAME_END raw interrupt
        volatile unsigned int irq_lcdc_done_raw :1;     //[0] DISP_DONE raw interrupt
    } mBits;
    volatile unsigned int dwValue;
} LCDC_IRQ_RAW_U;
#endif
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End













