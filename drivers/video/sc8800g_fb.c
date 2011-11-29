/*
 * Copyright (C) 2009
 * Guennadi Liakhovetski, DENX Software Engineering, <lg@denx.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <lcd.h>
#ifdef PLATFORM_SC8800G
#include <asm/arch/sc8800g_reg_base.h>
#include <asm/arch/sc8800g_lcd.h>
#endif
#ifdef CONFIG_SC8810
#include <asm/arch/sc8810_reg_base.h>
#include <asm/arch/sc8810_lcd.h>
#endif
#include <asm/arch/mfp.h>
#include <asm/arch/adi_hal_internal.h>
#include <asm/arch/regs_ana.h>
#include <asm/arch/analog_reg_v3.h>
#ifdef PLATFORM_SC8800G
#include <asm/arch/sc8800g_reg_ahb.h>
#include <asm/arch/sc8800g_reg_global.h>
#endif
#ifdef CONFIG_SC8810
#include <asm/arch/sc8810_reg_ahb.h>
#include <asm/arch/sc8810_reg_global.h>
#endif
#include <asm/io.h>
#include <asm/errno.h>
#include <lcd.h>

#define mdelay(a) udelay(a * 1000)
#define printk printf

DECLARE_GLOBAL_DATA_PTR;

void *lcd_base;			/* Start of framebuffer memory	*/
void *lcd_console_address;	/* Start of console buffer	*/

int lcd_line_length;
int lcd_color_fg;
int lcd_color_bg;

short console_col;
short console_row;

//#define  FB_DEBUG 
 #define DEBUG 
#ifdef FB_DEBUG
#define FB_PRINT printk
#else
#define FB_PRINT(...)
#endif

#define SPRD_LCDC_BASE LCDC_CTL_BEGIN

#define LCDC_CTRL                   (SPRD_LCDC_BASE + 0x0000)
#define LCDC_DISP_SIZE              (SPRD_LCDC_BASE + 0x0004)
#define LCDC_LCM_START              (SPRD_LCDC_BASE + 0x0008)
#define LCDC_LCM_SIZE               (SPRD_LCDC_BASE + 0x000c)
#define LCDC_BG_COLOR               (SPRD_LCDC_BASE + 0x0010)
#define LCDC_FIFO_STATUS            (SPRD_LCDC_BASE + 0x0014)

#define LCDC_IMG_CTRL                    (SPRD_LCDC_BASE + 0x0020)
#define LCDC_IMG_Y_BASE_ADDR         (SPRD_LCDC_BASE + 0x0024)
#define LCDC_IMG_UV_BASE_ADDR            (SPRD_LCDC_BASE + 0x0028)
#define LCDC_IMG_SIZE_XY             (SPRD_LCDC_BASE + 0x002c)
#define LCDC_IMG_PITCH                   (SPRD_LCDC_BASE + 0x0030)
#define LCDC_IMG_DISP_XY             (SPRD_LCDC_BASE + 0x0034)

#define LCDC_OSD1_CTRL                    (SPRD_LCDC_BASE + 0x0040)
#define LCDC_OSD2_CTRL                    (SPRD_LCDC_BASE + 0x0070)
#define LCDC_OSD3_CTRL                    (SPRD_LCDC_BASE + 0x0090)
#define LCDC_OSD4_CTRL                    (SPRD_LCDC_BASE + 0x00b0)
#define LCDC_OSD5_CTRL                    (SPRD_LCDC_BASE + 0x00d0)

#define LCDC_OSD1_BASE_ADDR                 (SPRD_LCDC_BASE + 0x0044)
#define LCDC_OSD1_ALPHA_BASE_ADDR           (SPRD_LCDC_BASE + 0x0048)
#define LCDC_OSD1_SIZE_XY                   (SPRD_LCDC_BASE + 0x004c)
#define LCDC_OSD1_PITCH                     (SPRD_LCDC_BASE + 0x0050)
#define LCDC_OSD1_DISP_XY                   (SPRD_LCDC_BASE + 0x0054)
#define LCDC_OSD1_ALPHA                     (SPRD_LCDC_BASE + 0x0058)
#define LCDC_OSD1_GREY_RGB                  (SPRD_LCDC_BASE + 0x005c)
#define LCDC_OSD1_CK                        (SPRD_LCDC_BASE + 0x0060)

#define LCDC_IRQ_EN             (SPRD_LCDC_BASE + 0x0120)
#define LCDC_IRQ_CLR                (SPRD_LCDC_BASE + 0x0124)
#define LCDC_IRQ_STATUS         (SPRD_LCDC_BASE + 0x0128)
#define LCDC_IRQ_RAW                (SPRD_LCDC_BASE + 0x012c)

#define LCM_CTRL                (SPRD_LCDC_BASE + 0x140)
#define LCM_PARAMETER0                (SPRD_LCDC_BASE + 0x144)
#define LCM_PARAMETER1                (SPRD_LCDC_BASE + 0x148)
#define LCM_IFMODE                (SPRD_LCDC_BASE + 0x14c)
#define LCM_RGBMODE              (SPRD_LCDC_BASE + 0x150)
#define LCM_RDDATA               (SPRD_LCDC_BASE + 0x154)
#define LCM_STATUS               (SPRD_LCDC_BASE + 0x158)
#define LCM_RSTN                 (SPRD_LCDC_BASE + 0x15c)
#define LCM_CD0                  (SPRD_LCDC_BASE + 0x180)
#define LCM_DATA0                (SPRD_LCDC_BASE + 0x184)
#define LCM_CD1                  (SPRD_LCDC_BASE + 0x190)
#define LCM_DATA1                (SPRD_LCDC_BASE + 0x194)

#define BITS_PER_PIXEL 16

//#include "sc8800g_rrm.h"
//#include "sc8800g_lcdc_manager.h" /* TEMP */
//#include "sc8800g_copybit_lcdc.h" /* TEMP */
/* TEMP, software make-up for lcdc's 4-byte-align only limitation */
//unsigned int fb_pa;
//unsigned int fb_va;
//unsigned int fb_va_cached;
/* TEMP, end */

struct sc8800fb_info {
//	struct fb_info *fb;
	uint32_t cap;
	struct ops_mcu *ops;
	struct lcd_spec *panel;
    uint32_t smem_start;
    uint32_t smem_len;
//	struct rrmanager *rrm;
};

struct lcd_cfg{ 
	uint32_t lcd_id;
	struct lcd_spec* panel;
};


//overlord for lcd adapt
static struct lcd_cfg lcd_panel[] = {
	[0]={
		.lcd_id = 0x1,
		.panel = &lcd_panel_r61581,
		},

	[1]={
		.lcd_id = 0x9481,
		.panel = &lcd_panel_rm68040,
		},
	[2]={
		.lcd_id = 0x3,
		.panel = &lcd_panel_hx8357,
		},
};

vidinfo_t panel_info = {
	.vl_col = 320,
	.vl_bpix = 4,
	.vl_row = 480,
};


static void __raw_bits_and(unsigned int v, unsigned int a)
{
        __raw_writel((__raw_readl(a) & v), a);
            
}

static void __raw_bits_or(unsigned int v, unsigned int a)
{
        __raw_writel((__raw_readl(a) | v), a);
}

static int32_t lcm_send_cmd (uint32_t cmd)
{
	/* busy wait for ahb fifo full sign's disappearance */
	while(__raw_readl(LCM_STATUS) & 0x2);

	__raw_writel(cmd, LCM_CD0);

	return 0;
}

static int32_t lcm_send_cmd_data (uint32_t cmd, uint32_t data)
{
	/* busy wait for ahb fifo full sign's disappearance */
	while(__raw_readl(LCM_STATUS) & 0x2);

	__raw_writel(cmd, LCM_CD0);

	/* busy wait for ahb fifo full sign's disappearance */
	while(__raw_readl(LCM_STATUS) & 0x2);

	__raw_writel(data, LCM_DATA0);

	return 0;
}

static int32_t lcm_send_data (uint32_t data)
{
	/* busy wait for ahb fifo full sign's disappearance */
	while(__raw_readl(LCM_STATUS) & 0x2);

	__raw_writel(data, LCM_DATA0);

	return 0;
}

static uint32_t lcm_read_data ()
{
	volatile uint32_t i =32;
	/* busy wait for ahb fifo full sign's disappearance */
	while(__raw_readl(LCM_STATUS) & 0x2);
	/* bit18 means 'read' */
	__raw_writel(1<<18, LCM_DATA0);

	/* busy wait for ahb fifo full sign's disappearance */
	while(__raw_readl(LCM_STATUS) & 0x2);

	/*wait*/
	while(i--);
	return __raw_readl(LCM_RDDATA);
}

static struct ops_mcu lcm_mcu_ops = {
	.send_cmd = lcm_send_cmd,
	.send_cmd_data = lcm_send_cmd_data,
	.send_data = lcm_send_data,
	.read_data = lcm_read_data,
};

//extern struct lcdc_manager lm; /* TEMP */
//extern struct semaphore copybit_wait; /* TEMP */
//static irqreturn_t lcdc_isr(int irq, void *data)
//{
//	uint32_t val ;
//	struct sc8800fb_info *fb = (struct sc8800fb_info *)data;
//
//	val = __raw_readl(LCDC_IRQ_STATUS);
//	if (val & (1<<0)){      /* lcdc done */
//		FB_PRINT("--> lcdc_isr lm.mode=%d\n", lm.mode);
//			__raw_bits_or((1<<0), LCDC_IRQ_CLR);
//			if(lm.mode == LMODE_DISPLAY) /* TEMP */
//			rrm_interrupt(fb->rrm);
//			else
//				up(&copybit_wait);
//	}
//
//	return IRQ_HANDLED;
//}
static void set_pins(void)
{	

#define SPRD_CPC_BASE PIN_CTL_BASE
	__raw_writel(0x1fff00, SPRD_CPC_BASE);

	/*LCDC pin config*/
    static unsigned long lcd_func_cfg[] = {

    //MFP_CFG_X(LCD_CSN1,AF0,DS1,F_PULL_NONE,S_PULL_NONE,IO_Z),//LCD_CSN1
    MFP_CFG_X(LCD_RSTN,AF0,DS1,F_PULL_NONE,S_PULL_UP,IO_Z),//LCD_RSTN
    MFP_CFG_X(LCD_CD,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_CD
    MFP_CFG_X(LCD_D0,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[0]
    MFP_CFG_X(LCD_D1,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[1]
    MFP_CFG_X(LCD_D2,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[2]
    MFP_CFG_X(LCD_D3,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[3]
    MFP_CFG_X(LCD_D4,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[4]
    MFP_CFG_X(LCD_D5,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[5]
    MFP_CFG_X(LCD_D6,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[6]
    MFP_CFG_X(LCD_D7,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[7]
    MFP_CFG_X(LCD_D8,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[8]
    MFP_CFG_X(LCD_WRN,AF0,DS1,F_PULL_NONE,S_PULL_UP,IO_Z),//LCD_WRN
    MFP_CFG_X(LCD_RDN,AF0,DS1,F_PULL_NONE,S_PULL_UP,IO_Z),//LCD_RDN
    MFP_CFG_X(LCD_CSN0,AF0,DS1,F_PULL_NONE,S_PULL_UP,IO_Z),//LCD_CSN0	
    MFP_CFG_X(LCD_D9,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[9]
    MFP_CFG_X(LCD_D10,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[10]
    MFP_CFG_X(LCD_D11,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[11]
    MFP_CFG_X(LCD_D12,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[12]
    MFP_CFG_X(LCD_D13,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[13]
    MFP_CFG_X(LCD_D14,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[14]
    MFP_CFG_X(LCD_D15,AF0,DS1,F_PULL_NONE,S_PULL_DOWN,IO_Z),//LCD_D[15]
    MFP_CFG_X(LCD_D16,AF0,DS1,F_PULL_NONE,S_PULL_NONE,IO_Z),//LCD_D[16]
    MFP_CFG_X(LCD_D17,AF0,DS1,F_PULL_NONE,S_PULL_NONE,IO_Z),//LCD_D[17]
    MFP_CFG_X(LCD_FMARK,AF0,DS1,F_PULL_NONE,S_PULL_NONE,IO_Z),//LCD_FMARK
    
    };

    sprd_mfp_config(lcd_func_cfg,ARRAY_SIZE(lcd_func_cfg));
}
static uint32_t panel_reset(struct lcd_spec *self)
{
	//panel reset
	__raw_writel(0x1, LCM_RSTN);	
	mdelay(0x10);
	__raw_writel(0x0, LCM_RSTN);
	mdelay(0x10);
	__raw_writel(0x1, LCM_RSTN);
	mdelay(0x10);

    return 0;
}
static void lcdc_mcu_init(struct sc8800fb_info *sc8800fb)
{
	//LCDC module enable
	__raw_bits_or(1<<0, LCDC_CTRL);

	/*FMARK mode*/
	__raw_bits_or(1<<1, LCDC_CTRL);

	/*FMARK pol*/
	__raw_bits_and(~(1<<2), LCDC_CTRL);
	
	FB_PRINT("@fool2[%s] LCDC_CTRL: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_CTRL));
	
	/* set background*/
	__raw_writel(0x0, LCDC_BG_COLOR);   //red

	FB_PRINT("@fool2[%s] LCDC_BG_COLOR: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_BG_COLOR));

	/* dithering enable*/
	//__raw_bits_or(1<<4, LCDC_CTRL);   
	
}
static int mount_panel(struct sc8800fb_info *sc8800fb, struct lcd_spec *panel)
{
	/* TODO: check whether the mode/res are supported */

	sc8800fb->panel = panel;

	panel->info.mcu->ops = sc8800fb->ops;

	panel->ops->lcd_reset = panel_reset;
	
	return 0;
}
//static int setup_fbmem(struct sc8800fb_info *sc8800fb, struct platform_device *pdev)
//{
//	uint32_t len, addr;
//	
//	len = sc8800fb->panel->width * sc8800fb->panel->height * (BITS_PER_PIXEL/8) * 2;
//
//	/* the addr should be 8 byte align */
//	addr = __get_free_pages(GFP_ATOMIC | __GFP_ZERO, get_order(len));
//	if (!addr)
//		return -ENOMEM;
//
//	printk("sc8800g_fb got %d bytes mem at 0x%x\n", len, addr);
//	sc8800fb->fb->fix.smem_start = __pa(addr);
//	sc8800fb->fb->fix.smem_len = len;
//	sc8800fb->fb->screen_base = (char*)addr;
//
//	/* TEMP, software make-up for lcdc's 4-byte-align only limitation */
//	fb_pa = sc8800fb->fb->fix.smem_start;
//	fb_va_cached = sc8800fb->fb->screen_base;
//	fb_va = (unsigned int)ioremap(sc8800fb->fb->fix.smem_start, 
//		sc8800fb->fb->fix.smem_len);
//	printk("sc8800g_fb fb_va=0x%x\n", fb_va, sc8800fb->fb->fix.smem_len);
//	/* TEMP, end */
//	
//	return 0;
//}
//static int sc8800fb_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
//{
//	if ((var->xres != info->var.xres) ||
//	    (var->yres != info->var.yres) ||
//	    (var->xres_virtual != info->var.xres_virtual) ||
//	    (var->yres_virtual != info->var.yres_virtual) ||
//	    (var->xoffset != info->var.xoffset) ||
//	    (var->bits_per_pixel != info->var.bits_per_pixel) ||
//	    (var->grayscale != info->var.grayscale))
//		 return -EINVAL;
//	return 0;
//}

static void real_set_layer(void *data)
{
	struct sc8800fb_info *info = (struct sc8800fb_info *)data;
	uint32_t reg_val;

	/* image layer base */
	reg_val = info->smem_start;
	reg_val = (reg_val>>2) & 0x3fffffff;
	__raw_writel(reg_val, LCDC_OSD1_BASE_ADDR);
}

static void real_refresh(void *para)
{
	struct sc8800fb_info *sc8800fb = (struct sc8800fb_info *)para;
	uint32_t reg_val;
	//struct fb_info *info = sc8800fb->fb;

	sc8800fb->panel->ops->lcd_invalidate(sc8800fb->panel);

	reg_val = sc8800fb->panel->width * sc8800fb->panel->height;
	reg_val |= (1<<20); /* for device 0 */
	reg_val &=~ ((1<<26)|(1<<27) | (1<<28)); /* FIXME: hardcoded cs 1 */
	__raw_writel(reg_val, LCM_CTRL);

	__raw_bits_or((1<<3), LCDC_CTRL); /* start refresh */
}

//static int real_pan_display(struct fb_var_screeninfo *var, struct fb_info *info)
//{
//	rrm_refresh(LID_OSD1, NULL, info);
//
//	FB_PRINT("@fool2[%s] LCDC_CTRL: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_CTRL));
//	FB_PRINT("@fool2[%s] LCDC_DISP_SIZE: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_DISP_SIZE));
//	FB_PRINT("@fool2[%s] LCDC_LCM_START: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_LCM_START));
//	FB_PRINT("@fool2[%s] LCDC_LCM_SIZE: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_LCM_SIZE));
//	FB_PRINT("@fool2[%s] LCDC_BG_COLOR: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_BG_COLOR));
//	
//    FB_PRINT("@fool2[%s] LCDC_OSD1_CTRL: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_CTRL));
//	FB_PRINT("@fool2[%s] LCDC_OSD1_BASE_ADDR: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_BASE_ADDR));
//	FB_PRINT("@fool2[%s] LCDC_OSD1_ALPHA_BASE_ADDR: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_ALPHA_BASE_ADDR));
//	FB_PRINT("@fool2[%s] LCDC_OSD1_SIZE_XY: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_SIZE_XY));
//	FB_PRINT("@fool2[%s] LCDC_OSD1_PITCH: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_PITCH));
//	FB_PRINT("@fool2[%s] LCDC_OSD1_DISP_XY: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_DISP_XY));
//	FB_PRINT("@fool2[%s] LCDC_OSD1_ALPHA: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_ALPHA));
//	FB_PRINT("@fool2[%s] LCDC_OSD1_GREY_RGB: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_GREY_RGB));
//	FB_PRINT("@fool2[%s] LCDC_OSD1_CK: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_CK));
//
//	FB_PRINT("@fool2[%s] LCM_CTRL: 0x%x\n", __FUNCTION__, __raw_readl(LCM_CTRL));
//	FB_PRINT("@fool2[%s] LCM_PARAMETER0: 0x%x\n", __FUNCTION__, __raw_readl(LCM_PARAMETER0));
//	FB_PRINT("@fool2[%s] LCM_PARAMETER1: 0x%x\n", __FUNCTION__, __raw_readl(LCM_PARAMETER1));
//	FB_PRINT("@fool2[%s] LCM_IFMODE: 0x%x\n", __FUNCTION__, __raw_readl(LCM_IFMODE));
//	FB_PRINT("@fool2[%s] LCM_RGBMODE: 0x%x\n", __FUNCTION__, __raw_readl(LCM_RGBMODE));
//    return 0;
//}
//
//static struct fb_ops sc8800fb_ops = {
//	.owner = THIS_MODULE,
//	.fb_check_var = sc8800fb_check_var,
//	.fb_pan_display = real_pan_display,
//	.fb_fillrect = cfb_fillrect,
//	.fb_copyarea = cfb_copyarea,
//	.fb_imageblit = cfb_imageblit,
//};

static unsigned PP[16];
//static void setup_fb_info(struct sc8800fb_info *sc8800fb)
//{
//	struct fb_info *fb= sc8800fb->fb;
//	int r;
//
//	fb->fbops = &sc8800fb_ops;
//	fb->flags = FBINFO_DEFAULT;
//	
//	/* finish setting up the fb_info struct */
//	strncpy(fb->fix.id, "sc8800fb", 16);
//	fb->fix.ypanstep = 1;
//	fb->fix.type = FB_TYPE_PACKED_PIXELS;
//	fb->fix.visual = FB_VISUAL_TRUECOLOR;
//	fb->fix.line_length = sc8800fb->panel->width * 16/8;
//
//	fb->var.xres = sc8800fb->panel->width;
//	fb->var.yres = sc8800fb->panel->height;
//	fb->var.width = sc8800fb->panel->width;
//	fb->var.height = sc8800fb->panel->height;
//	fb->var.xres_virtual = sc8800fb->panel->width;
//	fb->var.yres_virtual = sc8800fb->panel->height * 2;
//	fb->var.bits_per_pixel = BITS_PER_PIXEL;
//	fb->var.accel_flags = 0;
//
//	fb->var.yoffset = 0;
//
//	fb->var.red.offset = 11;
//	fb->var.red.length = 5;
//	fb->var.red.msb_right = 0;
//	fb->var.green.offset = 5;
//	fb->var.green.length = 6;
//	fb->var.green.msb_right = 0;
//	fb->var.blue.offset = 0;
//	fb->var.blue.length = 5;
//	fb->var.blue.msb_right = 0;
//
//	r = fb_alloc_cmap(&fb->cmap, 16, 0);
//	fb->pseudo_palette = PP;
//
//	PP[0] = 0;
//	for (r = 1; r < 16; r++)
//		PP[r] = 0xffffffff;
//}
static void lcdc_lcm_configure(struct sc8800fb_info *sc8800fb)
{
	uint32_t bits;
	
	//wait  until AHB FIFO is empty
	while(!(__raw_readl(LCM_STATUS) & (1<<2)));
	
	//__raw_writel(0, LCDC_LCM0PARAMETER0); /* LCM_PARAMETER0 */

	/* CS0 bus mode [BIT0]: 8080/6800 */
	switch (sc8800fb->panel->info.mcu->bus_mode) {
	case LCD_BUS_8080:
		bits = 0;
		break;
	case LCD_BUS_6800:
		bits = 1;
		break;
	default:
		bits = 0;
		break;
	}
	__raw_writel((bits&0x1), LCM_IFMODE);
	FB_PRINT("@fool2[%s] LCM_IFMODE: 0x%x\n", __FUNCTION__, __raw_readl(LCM_IFMODE));

	/* CS0 bus width [BIT1:0] */
	switch (sc8800fb->panel->info.mcu->bus_width) {
	case 16:
		bits = 0;
		break;
	case 18:
		bits = 1;
		break;
	case 8:
		bits = 2;
		break;
	case 9:
		bits = 3;
		break;
	default:
		bits = 0;
		break;
	}
	__raw_writel((bits&0x3), LCM_RGBMODE);
	FB_PRINT("@fool2[%s] LCM_RGBMODE: 0x%x\n", __FUNCTION__, __raw_readl(LCM_RGBMODE));

}
uint32_t CHIP_GetMcuClk (void)
{
	uint32_t clk_mcu_sel;
    	uint32_t clk_mcu = 0;
    	
         clk_mcu_sel = __raw_readl(AHB_ARM_CLK);
	clk_mcu_sel = (clk_mcu_sel >>  23)  &  0x3;
         switch (clk_mcu_sel)
         {
            case 0:
            	clk_mcu = 400000000;
            	break;
            case 1:
            	clk_mcu = 153600000;
            	break;
            case 2:
            	clk_mcu = 64000000;
            	break;
            case 3:
            	clk_mcu = 26000000;
            	break;
            default:
             	// can't go there
            	 break;
            }       
         return clk_mcu;
	
}
static void lcdc_update_lcm_timing(struct sc8800fb_info *sc8800fb)
{
	uint32_t  reg_value;
	uint32_t  ahb_div,ahb_clk;   
	uint32_t  ahb_clk_cycle;
	uint32_t rcss, rlpw, rhpw, wcss, wlpw, whpw;

	reg_value = __raw_readl(AHB_ARM_CLK);
	
	ahb_div = ( reg_value>>4 ) & 0x7;
	
	ahb_div = ahb_div + 1;

	if(__raw_readl (AHB_ARM_CLK) & (1<<30))
    {
            ahb_div = ahb_div << 1;
    }
    if(__raw_readl (AHB_ARM_CLK) & (1<<31))
    {
            ahb_div=ahb_div<<1;
    }	
	ahb_clk = CHIP_GetMcuClk()/ahb_div;

	FB_PRINT("@fool2[%s] ahb_clk: 0x%x\n", __FUNCTION__, ahb_clk);

	/* LCD_UpdateTiming() */
	ahb_clk_cycle = (100000000 >> 17)/(ahb_clk >> 20 );

	rcss = ((sc8800fb->panel->info.mcu->timing->rcss/ahb_clk_cycle+1)<3)?
	       (sc8800fb->panel->info.mcu->timing->rcss/ahb_clk_cycle+1):3;
	rlpw = ((sc8800fb->panel->info.mcu->timing->rlpw/ahb_clk_cycle+1)<14)?
	       (sc8800fb->panel->info.mcu->timing->rlpw/ahb_clk_cycle+1):14;
	rhpw = ((sc8800fb->panel->info.mcu->timing->rhpw/ahb_clk_cycle+1)<14)?
	       (sc8800fb->panel->info.mcu->timing->rhpw/ahb_clk_cycle+1):14;
	wcss = ((sc8800fb->panel->info.mcu->timing->wcss/ahb_clk_cycle+1)<3)?
	       (sc8800fb->panel->info.mcu->timing->wcss/ahb_clk_cycle+1):3;
	wlpw = ((sc8800fb->panel->info.mcu->timing->wlpw/ahb_clk_cycle+1)<14)?
	       (sc8800fb->panel->info.mcu->timing->wlpw/ahb_clk_cycle+1):14;
	whpw = ((sc8800fb->panel->info.mcu->timing->whpw/ahb_clk_cycle+1)<14)?
	       (sc8800fb->panel->info.mcu->timing->whpw/ahb_clk_cycle+1):14;
	
	//wait  until AHB FIFO if empty
	while(!(__raw_readl(LCM_STATUS) & (1<<2)));
	
	/*   LCDC_ChangePulseWidth() */
	__raw_writel(whpw |(wlpw<<4) |(wcss<<8) | (rhpw<<10) |	(rlpw<<14) |(rcss<<18),LCM_PARAMETER0); /* FIXME: hardcoded for !CS0 */

	//__raw_writel( 0x77555, LCDC_LCMPARAMETER0); /* @fool2, tmp */
	FB_PRINT("@fool2[%s] LCM_PARAMETER0: 0x%x\n", __FUNCTION__, __raw_readl(LCM_PARAMETER0));
}
static inline int set_lcdsize( void )
{
	uint32_t reg_val;
	
	//reg_val = ( info->var.xres & 0x3ff) | (( info->var.yres & 0x3ff )<<16);
	reg_val = ( 640 & 0x3ff) | (( 640 & 0x3ff )<<16);
	__raw_writel(reg_val, LCDC_DISP_SIZE);
	
	FB_PRINT("@fool2[%s] LCDC_DISP_SIZE: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_DISP_SIZE));

	return 0;
}
static inline int set_lcmrect( struct sc8800fb_info *info)
{
	uint32_t reg_val;
	
	__raw_writel(0, LCDC_LCM_START);
	
	reg_val = ( info->panel->width & 0x3ff) | (( info->panel->height & 0x3ff )<<16);
	__raw_writel(reg_val, LCDC_LCM_SIZE);
	
	FB_PRINT("@fool2[%s] LCDC_LCM_START: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_LCM_START));
	FB_PRINT("@fool2[%s] LCDC_LCM_SIZE: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_LCM_SIZE));

	return 0;
}
int set_lcdc_layers(struct sc8800fb_info *info)
{
	uint32_t reg_val;
    /******************* OSD1 layer setting **********************/
#if 1
    {
	/* we assume that
	 * 1. there's only one fbdev, and only block0 is used
	 * 2. the pan operation is a sync one
	 */

	__raw_bits_and(~(1<<0),LCDC_IMG_CTRL);  
	__raw_bits_and(~(1<<0),LCDC_OSD2_CTRL);  
	__raw_bits_and(~(1<<0),LCDC_OSD3_CTRL);  
	__raw_bits_and(~(1<<0),LCDC_OSD4_CTRL);  
	__raw_bits_and(~(1<<0),LCDC_OSD5_CTRL);  
	/*enable OSD1 layer*/
	__raw_bits_or((1<<0),LCDC_OSD1_CTRL);  

    /*color key */
	__raw_bits_and(~(1<<1),LCDC_OSD1_CTRL);  //disable
    
	/*alpha mode select*/
	__raw_bits_or((1<<2),LCDC_OSD1_CTRL);  //block alpha

    /*data format*/
	__raw_bits_and(~(1<<6),LCDC_OSD1_CTRL);  //RGB565
	__raw_bits_or(1<<5,LCDC_OSD1_CTRL);  
	__raw_bits_and(~(1<<4),LCDC_OSD1_CTRL);  
	__raw_bits_or(1<<3,LCDC_OSD1_CTRL);  	

    /*data endian*/
	__raw_bits_or(1<<8,LCDC_OSD1_CTRL);  //little endian
	__raw_bits_and(~(1<<7),LCDC_OSD1_CTRL);  
	
    /*alpha endian*/
    /*
    __raw_bits_or(1<<10,LCDC_IMG_CTRL);  
	__raw_bits_and(~(1<<9),LCDC_IMG_CTRL);  
	*/
	
	FB_PRINT("@fool2[%s] LCDC_OSD1_CTRL: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_CTRL));

	/* OSD1 layer base */
	//reg_val = (info->var.yoffset)?info->fix.smem_start:	(info->fix.smem_start+ info->fix.smem_len/2);
    reg_val = info->smem_start;
	reg_val = (reg_val >>2)& 0x3fffffff;
	__raw_writel(reg_val, LCDC_OSD1_BASE_ADDR);
	
    FB_PRINT("@fool2[%s] LCDC_OSD1_BASE_ADDR: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_BASE_ADDR));

    /*OSD1 layer alpha value*/
	__raw_writel(0xff, LCDC_OSD1_ALPHA);

	FB_PRINT("@fool2[%s] LCDC_OSD1_ALPHA: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_ALPHA));
    
    /*alpha base addr*/
	//__raw_writel(reg_val, LCDC_OSD1_ALPHA_BASE_ADDR); 
	
	/*OSD1 layer size*/
	reg_val = ( info->panel->width & 0x3ff) | (( info->panel->height & 0x3ff )<<16);
	__raw_writel(reg_val, LCDC_OSD1_SIZE_XY);

	FB_PRINT("@fool2[%s] LCDC_OSD1_SIZE_XY: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_SIZE_XY));
	
	/*OSD1 layer start position*/
	__raw_writel(0, LCDC_OSD1_DISP_XY);
	
	FB_PRINT("@fool2[%s] LCDC_OSD1_DISP_XY: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_DISP_XY));
	
    /*OSD1 layer pitch*/
	reg_val = ( info->panel->width & 0x3ff) ;
	__raw_writel(reg_val, LCDC_OSD1_PITCH);
	
	FB_PRINT("@fool2[%s] LCDC_OSD1_PITCH: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_PITCH));

    /*OSD1 color_key value*/
    //Fix me
    /*OSD1 grey RGB*/
    //Fix me
    }
#endif
    /******************** Image layer settting **********************/
#if 0
	{
    /* we assume that
	 * 1. there's only one fbdev, and only block0 is used
	 * 2. the pan operation is a sync one
	 */

	__raw_bits_and(~(1<<0),LCDC_OSD1_CTRL);  
	__raw_bits_and(~(1<<0),LCDC_OSD2_CTRL);  
	__raw_bits_and(~(1<<0),LCDC_OSD3_CTRL);  
	__raw_bits_and(~(1<<0),LCDC_OSD4_CTRL);  
	__raw_bits_and(~(1<<0),LCDC_OSD5_CTRL);  
	/*enable imge layer*/
	__raw_bits_or((1<<0),LCDC_IMG_CTRL);  

	/*little endian*/
	__raw_bits_or(1<<6,LCDC_IMG_CTRL);  
	__raw_bits_and(~(1<<5),LCDC_IMG_CTRL);  
	//__raw_bits_or(1<<7,LCDC_IMG_CTRL);  
	//__raw_bits_and(~(1<<8),LCDC_IMG_CTRL);  
	
	/*data format*/
	__raw_bits_or(1<<1,LCDC_IMG_CTRL);  //RGB565
	__raw_bits_and(~(1<<2),LCDC_IMG_CTRL);  
	__raw_bits_or(1<<3,LCDC_IMG_CTRL);  
	__raw_bits_and(~(1<<4),LCDC_IMG_CTRL);  	

	FB_PRINT("@fool2[%s] LCDC_IMG_CTRL: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_IMG_CTRL));
	
	/* image layer base */
	reg_val = (info->var.yoffset)?info->fix.smem_start:	(info->fix.smem_start+ info->fix.smem_len/2);
	reg_val = (reg_val >>2)& 0x3fffffff;
	__raw_writel(reg_val, LCDC_IMG_Y_BASE_ADDR);

	FB_PRINT("@fool2[%s] LCDC_IMG_Y_BASE_ADDR: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_IMG_Y_BASE_ADDR));
	
	/*image layer size*/
	reg_val = ( info->var.xres & 0x3ff) | (( info->var.yres & 0x3ff )<<16);
	__raw_writel(reg_val, LCDC_IMG_SIZE_XY);

	FB_PRINT("@fool2[%s] LCDC_IMG_SIZE_XY: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_IMG_SIZE_XY));
	
	/*image layer start position*/
	__raw_writel(0, LCDC_IMG_DISP_XY);
	
	/*image layer pitch*/
	reg_val = ( info->var.xres & 0x3ff) ;
	__raw_writel(reg_val, LCDC_IMG_PITCH);
	
	FB_PRINT("@fool2[%s] LCDC_IMG_DISP_XY: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_IMG_DISP_XY));
    }
#endif

	/*LCDC workplane size*/
	set_lcdsize();
	
	/*LCDC LCM rect size*/
	set_lcmrect(info);

	return 0;
}

static void hw_early_init(struct sc8800fb_info *sc8800fb)
{
	int ret;

	set_pins();
	//select LCD clock source	
	__raw_bits_and(~(1<<6), GR_PLL_SCR);    //pll_src=96M
	__raw_bits_and(~(1<<7), GR_PLL_SCR);

	//set LCD divdior
	__raw_bits_and(~(1<<0), GR_GEN4);  //div=0
	__raw_bits_and(~(1<<1), GR_GEN4); 
	__raw_bits_and(~(1<<2), GR_GEN4);  

	//enable LCD clock
	__raw_bits_or(1<<3, AHB_CTL0); 

	//LCD soft reset
	__raw_bits_or(1<<3, AHB_SOFT_RST);
	mdelay(10);	
	__raw_bits_and(~(1<<3), AHB_SOFT_RST); 


	/* enable LCDC_DONE IRQ */
	__raw_bits_or((1<<0), LCDC_IRQ_EN);

	/* init lcdc mcu mode using default configuration */
	lcdc_mcu_init(sc8800fb);
}

static void hw_init(struct sc8800fb_info *sc8800fb)
{
	/* only MCU mode is supported currently */
	if (LCD_MODE_RGB == sc8800fb->panel->mode)
		return;
	
	//__raw_bits_or((1<<0), LCDC_DAC_CONTROL_REG); /*close tv_out */	
	//panel reset
	panel_reset(sc8800fb->panel);

	/* set lcdc-lcd interface parameters */
	lcdc_lcm_configure(sc8800fb);

	/* set timing parameters for LCD */
	lcdc_update_lcm_timing(sc8800fb);
}

static void hw_later_init(struct sc8800fb_info *sc8800fb)
{
	/* init mounted lcd panel */
	sc8800fb->panel->ops->lcd_init(sc8800fb->panel);

	set_lcdc_layers(sc8800fb); 
}


void set_backlight(uint32_t value)
{
	
	ANA_REG_AND(WHTLED_CTL, ~(WHTLED_PD_SET | WHTLED_PD_RST));
	ANA_REG_OR(WHTLED_CTL,  WHTLED_PD_RST);
	ANA_REG_MSK_OR (WHTLED_CTL, ( (value << WHTLED_V_SHIFT) &WHTLED_V_MSK), WHTLED_V_MSK);
}

struct sc8800fb_info *sc8800fb;

static uint32_t lcd_id_to_kernel = 0;

void save_lcd_id_to_kernel(uint32_t id)
{
	lcd_id_to_kernel = id;
}

uint32_t load_lcd_id_to_kernel()
{
	return lcd_id_to_kernel;
}

static int find_adapt_from_saved()
{
	int i;
	//we will decide how to save lcdid later
	/*
	uint32_t id = savedvalue;
	for(i = 0;i<(sizeof(lcd_panel))/(sizeof(lcd_panel[0]));i++)
	{
		if(id == lcd_panel[i].lcd_id)
		{
			save_lcd_id_to_kernel(id);
			return i;
			
		}
	}*/
	return -1;		
}

static int lcd_readid_default(struct lcd_spec *self)
{
	uint32_t dummy;
	//default id reg is 0
	self->info.mcu->ops->send_cmd(0x0);

	if(self->info.mcu->bus_width == 8)
	{
		dummy = (self->info.mcu->ops->read_data())&0xff;
		dummy <<= 8;
		dummy |= (self->info.mcu->ops->read_data())&0xff;
	}
	else
	{
		dummy = (self->info.mcu->ops->read_data())&0xffff;
	}
	return dummy;
}


static int find_adapt_from_readid(struct sc8800fb_info *sc8800fb)
{
	int i;
	uint32_t id;
	for(i = 0;i<(sizeof(lcd_panel))/(sizeof(lcd_panel[0]));i++)
	{
		//first ,try mount
		mount_panel(sc8800fb,lcd_panel[i].panel);
		//hw init to every panel
		hw_init(sc8800fb);
		//readid
		if(sc8800fb->panel->ops->lcd_readid)
		{
			id = sc8800fb->panel->ops->lcd_readid(sc8800fb->panel);
		}
		else
		{
			id = lcd_readid_default(sc8800fb->panel);
		}
		//if the id is right?
		if(id == lcd_panel[i].lcd_id)
		{
			save_lcd_id_to_kernel(id);
			return i;
		}
	}
	return -1;		
}


static int sc8800fb_probe(void * lcdbase)
{
	//struct fb_info *fb;
	int32_t ret;
	int lcd_adapt;

	FB_PRINT("@fool2[%s]\n", __FUNCTION__);
	FB_PRINT("sc8800g_fb initialize!\n");

	//lm_init(4); /* TEMP */
	//
	//fb = framebuffer_alloc(sizeof(struct sc8800fb_info), &pdev->dev);
	//if (!fb)
	//	return -ENOMEM;
	//sc8800fb = fb->par;
	//sc8800fb->fb = fb;
	//sc8800fb->ops = &lcm_mcu_ops;
	//sc8800fb->rrm = rrm_init(real_refresh, (void*)sc8800fb);
	//rrm_layer_init(LID_OSD1, 2, real_set_layer);

    sc8800fb = malloc(sizeof(struct sc8800fb_info));
    if(!sc8800fb){
        printk("not enough memory\n");
        return -ENOMEM;
    }
    sc8800fb->ops = &lcm_mcu_ops;

	//we maybe readid ,so hardware should be init
	hw_early_init(sc8800fb);

//only test 6810 ,do not want get troubles on other plateform
#ifdef CONFIG_LCD_SP6810A

	lcd_adapt = find_adapt_from_saved();
	if(lcd_adapt == -1)
	{
		lcd_adapt = find_adapt_from_readid(sc8800fb);
	}
	if(lcd_adapt == -1)
	{
		lcd_adapt = 0;
	}
#endif
#ifdef CONFIG_LCD_SP8805GA
		lcd_adapt = 0;
#endif
#ifdef CONFIG_LCD_OPENPHONE
		lcd_adapt = 2;
#endif

	ret = mount_panel(sc8800fb, lcd_panel[lcd_adapt].panel);
	if (ret) {
		printk("unsupported panel!!");
		return -EFAULT;
	}

    sc8800fb->smem_start = lcdbase;
    sc8800fb->smem_len = sc8800fb->panel->width * sc8800fb->panel->height;

	//ret = setup_fbmem(sc8800fb, pdev);
	//if (ret)
	//	return ret;

	//setup_fb_info(sc8800fb);

	//ret = register_framebuffer(fb);
	//if (ret) {
	//	framebuffer_release(fb);
	//	return ret;
	//}

	hw_init(sc8800fb);
	hw_later_init(sc8800fb);
    real_set_layer(sc8800fb);

	//copybit_lcdc_init(); /* TEMP */

	/* FIXME: put the BL stuff to where it belongs. */
	//set_backlight(50);

	
if(0){ /* in-kernel test code */
	int size = sc8800fb->panel->width * sc8800fb->panel->height;
    short adie_chip_id = ANA_REG_GET(ANA_ADIE_CHIP_ID);
	
	/* set color */
	if (adie_chip_id == 0) {
		unsigned short *ptr=(unsigned short*)sc8800fb->smem_start;
        FB_PRINT("adi v0 buffur prt is 0x%x\n", ptr);
        
		int len = size /3 ; /* 1/3 frame pixels */
		int offset;

		for(offset=0;offset< len;offset++)
			(* (volatile unsigned short *)(ptr++))= 0xf800; //red 
		for(offset=0;offset< len;offset++)
			(* (volatile unsigned short *)(ptr++))= 0x07e0; //green
		for(offset=0;offset< len;offset++)
			(* (volatile unsigned short *)(ptr++))= 0x001f; //blue
    } else {
		unsigned short *ptr=(unsigned short*)sc8800fb->smem_start;
        FB_PRINT("adi v1 buffur prt is 0x%x\n", ptr);
		int len = size ; /* 1/3 frame pixels */
		int offset;

		for(offset=0;offset< len;offset++)
			(* (volatile unsigned short *)(ptr++))= 0xf800; //red 
    }


#ifndef TEST_RRM
	//real_pan_display(&sc8800fb->fb->var, &test_info);
	real_refresh(sc8800fb);
#endif
}

#ifdef TEST_RRM
	setup_rrm_test(fb);
#endif

	return 0;
}

void lcd_initcolregs(void)
{
}

void lcd_disable(void)
{
}


/* References in this function refer to respective Linux kernel sources */
void lcd_enable(void)
{
}

void lcd_ctrl_init(void *lcdbase)
{
     sc8800fb_probe(lcdbase);
}

void lcd_display(void)
{
    real_refresh(sc8800fb);
}
#ifdef CONFIG_LCD_INFO
#include <nand.h>
extern nand_info_t nand_info[];

void lcd_show_board_info(void)
{
    ulong dram_size, nand_size;
    int i;
    char temp[32];

    dram_size = 0;
    for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++)
      dram_size += gd->bd->bi_dram[i].size;
    nand_size = 0;
    for (i = 0; i < CONFIG_SYS_MAX_NAND_DEVICE; i++)
      nand_size += nand_info[i].size;

    lcd_printf("\n%s\n", U_BOOT_VERSION);
    lcd_printf("  %ld MB SDRAM, %ld MB NAND\n",
                dram_size >> 20,
                nand_size >> 20 );
    lcd_printf("  Board            : esd ARM9 \n");
    lcd_printf("  Mach-type        : %lu\n", gd->bd->bi_arch_number);
}
#endif /* CONFIG_LCD_INFO */
