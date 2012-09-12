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

#ifndef _SPRDFB_H_
#define _SPRDFB_H_

#define printk printf
#define  FB_DEBUG
#ifdef FB_DEBUG
#define FB_PRINT printk
#else
#define FB_PRINT(...)
#endif

#define mdelay(a) udelay(a * 1000)


enum{
	SPRDFB_PANEL_IF_DBI = 0,
	SPRDFB_PANEL_IF_DPI,
	SPRDFB_PANEL_IF_EDPI,
	SPRDFB_PANEL_IF_LIMIT
};


enum{
	MCU_LCD_REGISTER_TIMING = 0,
	MCU_LCD_GRAM_TIMING,
	MCU_LCD_TIMING_KIND_MAX
};

enum{
	RGB_LCD_H_TIMING = 0,
	RGB_LCD_V_TIMING,
	RGB_LCD_TIMING_KIND_MAX
};

struct sprdfb_device;


struct panel_if_ctrl{
	const char *if_name;

	int32_t (*panel_if_check)(struct panel_spec *self);
	void (*panel_if_mount)(struct sprdfb_device *dev);
	void (*panel_if_init)(struct sprdfb_device *dev);
	void (*panel_if_ready)(struct sprdfb_device *dev);
	void (*panel_if_uninit)(struct sprdfb_device *dev);
	void (*panel_if_before_refresh)(struct sprdfb_device *dev);
	void (*panel_if_after_refresh)(struct sprdfb_device *dev);
	void (*panel_if_suspend)(struct sprdfb_device *dev);
	void (*panel_if_resume)(struct sprdfb_device *dev);
};


struct sprdfb_device {
	uint32_t smem_start;

	union{
		uint32_t	mcu_timing[MCU_LCD_TIMING_KIND_MAX];
		uint32_t	rgb_timing[RGB_LCD_TIMING_KIND_MAX];
	}panel_timing;

	uint16_t		panel_if_type; /*panel IF*/
	struct panel_spec	*panel;
	struct panel_if_ctrl *if_ctrl;
	struct display_ctrl	*ctrl;
};

struct display_ctrl {
	const char	*name;

	int32_t	(*early_init)	  (struct sprdfb_device *dev);
	int32_t	(*init)		  (struct sprdfb_device *dev);
	int32_t	(*uninit)		  (struct sprdfb_device *dev);

	int32_t 	(*refresh)	  (struct sprdfb_device *dev);
};

#endif
