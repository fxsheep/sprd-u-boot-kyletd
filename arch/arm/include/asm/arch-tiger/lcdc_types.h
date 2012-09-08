#ifndef _LCDC_TYPES_H_
#define _LCDC_TYPES_H_

#include "sci_types.h"

//interrupt handle function declare
typedef void (*LCD_IRQ_FUNC) (void);

typedef enum _lcd_int_type_tag
{
    LCD_INT_LCDC_DONE = 0,
    LCD_INT_LCM_EOF,
    LCD_INT_LCM_SOF,
    LCD_INT_FMARK,
    LCD_INT_MAX
} LCD_INT_TYPE_E;

typedef enum _lcd_layer_id_tag
{
    LCD_LAYER_IMAGE,
    LCD_LAYER_OSD1,
    LCD_LAYER_OSD2,
    LCD_LAYER_OSD3,
#if defined(PLATFORM_SC8800G) || defined(CONFIG_SC8810)
    LCD_LAYER_OSD4,
    LCD_LAYER_OSD5,
#endif
    LCD_LAYER_ID_MAX
} LCD_LAYER_ID_E;

typedef enum _lcd_fmark_mode_tag
{
    LCD_FMARK_DEVICE,
    LCD_FMARK_NONE,
    LCD_FMARK_MODE_MAX
} LCD_FMARK_MODE_E;

typedef enum _lcd_fmark_pol_tag
{
    LCD_FMARK_HIGH,
    LCD_FMARK_LOW,
    LCD_FMARK_POL_MAX
} LCD_FMARK_POL_E;

typedef enum _lcd_upsample_mode_tag
{
    LCD_UPSAMPLE_DUPLICATE,
    LCD_UPSAMPLE_AVERAGE,
    LCD_UPSAMPLE_MODE_MAX
} LCD_UPSAMPLE_MODE_E;

typedef enum _lcd_data_format_tag
{
    LCD_YUV422 = 0,
    LCD_YUV420,
    LCD_YUV400,
    LCD_RGB888,
    LCD_RGB666,
    LCD_RGB565,
    LCD_RGB555,
    LCD_GREY,
    LCD_MAX
} LCD_DATA_FORMAT_E;

typedef enum _lcd_osd_alpha_tag
{
    LCD_OSD_PIXEL_ALPHA = 0,
    LCD_OSD_BLOCK_ALPHA,
    LCD_OSD_MAX
} LCD_OSD_ALPHA_E;

typedef struct _lcd_postion_tag
{
    uint16 x;
    uint16 y;
} LCDC_POS_T;

typedef struct _lcdc_size_tag
{
    uint16 w;
    uint16 h;
} LCDC_SIZE_T;

typedef struct _lcd_rect_tag //rectangle
{
    uint16  x;
    uint16  y;
    uint16  w;
    uint16  h;
} LCDC_RECT_T;

typedef struct _lcd_data_addr_tag
{
    uint32 y_addr;
    uint32 uv_addr;
} LCD_DATA_ADDR_T;

typedef struct _lcd_img_blk_param_tag
{
    LCDC_SIZE_T             src_size;
    LCDC_RECT_T             src_trim_rect;
    LCDC_POS_T              disp_pos;
    LCD_DATA_ADDR_T         src_base_addr;
    LCD_DATA_FORMAT_E       format;
#if defined(PLATFORM_SC8800G) || defined(CONFIG_SC8810)
    uint32                 is_little_endian;
#else
    BOOLEAN                 is_little_endian;
#endif
    BOOLEAN                 layer_en;
} LCD_IMG_LAYER_PARAM_T;

typedef struct _lcd_osd_blk_param_tag
{
    LCDC_SIZE_T             src_size;
    LCDC_RECT_T             src_trim_rect;
    LCDC_POS_T              disp_pos;
    uint32                  src_base_addr;
    uint32                  alpha_base_addr;
    uint32                  grey_rgb;
    uint8                   alpha;
    LCD_DATA_FORMAT_E       format;
    LCD_OSD_ALPHA_E         alpha_mode;
#if defined(PLATFORM_SC8800G) || defined(CONFIG_SC8810)
    uint32                 is_little_endian;
#else
    BOOLEAN                 is_little_endian;
#endif
    BOOLEAN                 colorkey_en;
    uint32                  colorkey;
    BOOLEAN                 layer_en;
} LCD_OSD_LAYER_PARAM_T;

typedef struct _lcd_yuv2rgb_param_tag
{
    LCD_UPSAMPLE_MODE_E     upsample_mode;
    uint8                   contrast;           //0-255
    uint8                   saturation;         //0-255
    int16                   brightness;         //-256-256
} LCD_YUV2RGB_PARAM_T;

typedef struct _lcdc_cap_param_tag
{
    LCDC_RECT_T             cap_rect;
    LCDC_SIZE_T             dst_size;
    LCDC_RECT_T             dst_trim_rect;
    LCD_DATA_FORMAT_E       format;
    uint32                  dst_base_addr;
    BOOLEAN                 is_little_endian;
    BOOLEAN                 cap_en;
} LCDC_CAP_PARAM_T;

#endif
