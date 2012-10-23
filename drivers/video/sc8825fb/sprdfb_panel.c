/*
 * Copyright (C) 2012 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <common.h>
#include <lcd.h>

#include <asm/arch/tiger_lcd.h>
#include <asm/arch/dispc_reg.h>

#include "sprdfb.h"


#ifdef CONFIG_SC8810_OPENPHONE

extern struct panel_spec lcd_panel_hx8357;
static struct panel_cfg lcd_panel[] = {
	[0]={
		.lcd_id = 0x57,
		.panel = &lcd_panel_hx8357,
		},
};
#elif defined CONFIG_LCD_QVGA
/*
extern struct panel_spec lcd_panel_ili9341s;
static struct panel_cfg lcd_panel[] = {
    [0]={
        .lcd_id = 0x61,
        .panel = &lcd_panel_ili9341s,
        },
};
*/
extern struct panel_spec lcd_s6d0139_spec;
static struct panel_cfg lcd_panel[] = {
    [0]={
        .lcd_id = 0x139,
        .panel = &lcd_s6d0139_spec ,
        },
};
/*
#elif defined CONFIG_TIGER
extern struct panel_spec lcd_nt35516_mipi_spec;
static struct panel_cfg lcd_panel[] = {
    [0]={
        .lcd_id = 0x16,
        .panel = &lcd_nt35516_mipi_spec ,
        },
};

#elif defined CONFIG_LCD_QHD
extern struct panel_spec lcd_nt35516_rgb_spi_spec;
static struct panel_cfg lcd_panel[] = {
    [0]={
        .lcd_id = 0x16,
        .panel = &lcd_nt35516_rgb_spi_spec ,
        },
};
*/
#else
#ifdef CONFIG_LCD_788
extern struct panel_spec lcd_panel_hx8357;
static struct panel_cfg lcd_panel[] = {
    [0]={
        .lcd_id = 0x57,
        .panel = &lcd_panel_hx8357,
        },
};
#else
extern struct panel_spec lcd_panel_hx8369;
static struct panel_cfg lcd_panel[] = {
	[0]={
		.lcd_id = 0x69,
		.panel = &lcd_panel_hx8369,
		},
};
#endif
#endif

#ifdef CONFIG_LCD_WVGA
vidinfo_t panel_info = {
	.vl_col = 480,
	.vl_bpix = 4,
	.vl_row = 800,
};
#endif

#ifdef CONFIG_LCD_HVGA
vidinfo_t panel_info = {
	.vl_col = 320,
	.vl_bpix = 4,
	.vl_row = 480,
};
#endif

#ifdef CONFIG_LCD_QVGA
vidinfo_t panel_info = {
	.vl_col = 240,
	.vl_bpix = 4,
	.vl_row = 320,
};
#endif

#ifdef CONFIG_LCD_QHD
vidinfo_t panel_info = {
	.vl_col = 540,
	.vl_bpix = 4,
	.vl_row = 960,
};
#endif

extern struct panel_if_ctrl sprdfb_mcu_ctrl;
extern struct panel_if_ctrl sprdfb_rgb_ctrl;
extern struct panel_if_ctrl sprdfb_mipi_ctrl;

void sprdfb_panel_remove(struct sprdfb_device *dev);


static int32_t panel_reset_dispc(struct panel_spec *self)
{
	dispc_write(0, DISPC_RSTN);
	udelay(200);
	dispc_write(1, DISPC_RSTN);

	/* wait 10ms util the lcd is stable */
	mdelay(10);
	return 0;
}


static void panel_reset(struct panel_spec *panel)
{
	FB_PRINT("sprdfb: [%s]\n",__FUNCTION__);

	/* panel reset */
	panel_reset_dispc(panel);
}

static int panel_mount(struct sprdfb_device *dev, struct panel_spec *panel)
{
	uint16_t rval = 1;

	FB_PRINT("sprdfb: [%s], type = %d\n",__FUNCTION__, panel->type);

	switch(panel->type){
	case SPRDFB_PANEL_TYPE_MCU:
		dev->if_ctrl = &sprdfb_mcu_ctrl;
		break;
	case SPRDFB_PANEL_TYPE_RGB:
		dev->if_ctrl = &sprdfb_rgb_ctrl;
		break;
	case SPRDFB_PANEL_TYPE_MIPI:
		dev->if_ctrl = &sprdfb_mipi_ctrl;
		break;
	default:
		FB_PRINT("sprdfb: [%s]: erro panel type.(%d)",__FUNCTION__, panel->type);
		dev->if_ctrl = NULL;
		rval = 0 ;
		break;
	};

	if(dev->if_ctrl->panel_if_check){
		rval = dev->if_ctrl->panel_if_check(panel);
	}

	if(0 == rval){
		FB_PRINT("sprdfb: [%s] check panel fail!\n", __FUNCTION__);
		dev->if_ctrl = NULL;
		return -1;
	}

	dev->panel = panel;

	if(NULL == dev->panel->ops->panel_reset){
		dev->panel->ops->panel_reset = panel_reset_dispc;
	}

	dev->if_ctrl->panel_if_mount(dev);

	return 0;
}


int panel_init(struct sprdfb_device *dev)
{
	if((NULL == dev) || (NULL == dev->panel)){
		FB_PRINT("sprdfb: [%s]: Invalid param\n", __FUNCTION__);
		return -1;
	}

	FB_PRINT("sprdfb: [%s], type = %d\n",__FUNCTION__, dev->panel->type);

	if(NULL != dev->if_ctrl->panel_if_init){
		dev->if_ctrl->panel_if_init(dev);
	}
	return 0;
}

int panel_ready(struct sprdfb_device *dev)
{
	if((NULL == dev) || (NULL == dev->panel)){
		FB_PRINT("sprdfb: [%s]: Invalid param\n", __FUNCTION__);
		return -1;
	}

	FB_PRINT("sprdfb: [%s],  type = %d\n",__FUNCTION__, dev->panel->type);

	if(NULL != dev->if_ctrl->panel_if_ready){
		dev->if_ctrl->panel_if_ready(dev);
	}

	return 0;
}


static struct panel_spec *adapt_panel_from_readid(struct sprdfb_device *dev)
{
	int id, i;

	FB_PRINT("sprdfb: [%s]\n",__FUNCTION__);

	for(i = 0;i<(sizeof(lcd_panel))/(sizeof(lcd_panel[0]));i++) {
		FB_PRINT("sprdfb: [%s]: try panel 0x%x\n", __FUNCTION__, lcd_panel[i].lcd_id);
		panel_reset(lcd_panel[i].panel);
		panel_mount(dev, lcd_panel[i].panel);
		panel_init(dev);
		dev->panel->ops->panel_init(dev->panel);
		id = dev->panel->ops->panel_readid(dev->panel);
		if(id == lcd_panel[i].lcd_id) {
			FB_PRINT("sprdfb: [%s]: LCD Panel 0x%x is attached!\n", __FUNCTION__, lcd_panel[i].lcd_id);
			save_lcd_id_to_kernel(id);
			panel_ready(dev);
			return lcd_panel[i].panel;
		}
		sprdfb_panel_remove(dev);
	}
	FB_PRINT("sprdfb:  [%s]: failed to attach LCD Panel!\n", __FUNCTION__);
	return NULL;
}

uint16_t sprdfb_panel_probe(struct sprdfb_device *dev)
{
	struct panel_spec *panel;

	if(NULL == dev){
		FB_PRINT("sprdfb: [%s]: Invalid param\n", __FUNCTION__);
	}

	FB_PRINT("sprdfb: [%s]\n",__FUNCTION__);

	/* can not be here in normal; we should get correct device id from uboot */
	panel = adapt_panel_from_readid(dev);

	if (panel) {
		FB_PRINT("sprdfb: [%s] got panel\n", __FUNCTION__);
		return 0;
	}

	FB_PRINT("sprdfb: [%s] can not got panel\n", __FUNCTION__);

	return -1;
}

void sprdfb_panel_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	FB_PRINT("sprdfb: [%s]\n, (%d, %d, %d,%d)",__FUNCTION__, left, top, right, bottom);

	if(NULL != self->ops->panel_invalidate_rect){
		self->ops->panel_invalidate_rect(self, left, top, right, bottom);
	}
}

void sprdfb_panel_invalidate(struct panel_spec *self)
{
	FB_PRINT("sprdfb: [%s]\n",__FUNCTION__);

	if(NULL != self->ops->panel_invalidate){
		self->ops->panel_invalidate(self);
	}
}

void sprdfb_panel_before_refresh(struct sprdfb_device *dev)
{
	FB_PRINT("sprdfb: [%s]\n",__FUNCTION__);

	if(NULL != dev->if_ctrl->panel_if_before_refresh)
		dev->if_ctrl->panel_if_before_refresh(dev);
}

void sprdfb_panel_after_refresh(struct sprdfb_device *dev)
{
	FB_PRINT("sprdfb: [%s]\n",__FUNCTION__);

	if(NULL != dev->if_ctrl->panel_if_after_refresh)
		dev->if_ctrl->panel_if_after_refresh(dev);
}

void sprdfb_panel_remove(struct sprdfb_device *dev)
{
	FB_PRINT("sprdfb: [%s]\n",__FUNCTION__);

	if(NULL != dev->if_ctrl->panel_if_uninit){
		dev->if_ctrl->panel_if_uninit(dev);
	}
	dev->panel = NULL;
}

