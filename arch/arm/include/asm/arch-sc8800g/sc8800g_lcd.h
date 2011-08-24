/* drivers/video/sc8800g/sc8800g_lcd.h
 *
 * Spreadtrum LCD abstraction
 *
 * Copyright (C) 2010 Spreadtrum.com
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

#ifndef _SC8800G_LCD_H_
#define _SC8800G_LCD_H_

#include <linux/types.h>

/* LCD mode */
#define LCD_MODE_RGB 0
#define LCD_MODE_MCU 1

/* bus mode */
#define LCD_BUS_8080 0
#define LCD_BUS_6800 1
#define LCD_BUS_SPI  2

/* lcd directions */
#define LCD_DIRECT_NORMAL  0
#define LCD_DIRECT_ROT_90  1
#define LCD_DIRECT_ROT_180 2
#define LCD_DIRECT_ROT_270 3
#define LCD_DIRECT_MIR_H   4
#define LCD_DIRECT_MIR_V   5
#define LCD_DIRECT_MIR_HV  6

struct lcd_spec;

/* LCD operations */
struct lcd_operations {
	int32_t (*lcd_init)(struct lcd_spec *self);
	int32_t (*lcd_close)(struct lcd_spec *self);
	int32_t (*lcd_reset)(struct lcd_spec *self);
	int32_t (*lcd_enter_sleep)(struct lcd_spec *self, uint8_t is_sleep);
	int32_t (*lcd_set_contrast)(struct lcd_spec *self, uint16_t contrast);
	int32_t (*lcd_set_brightness)(struct lcd_spec *self,
				uint16_t brightness);
	int32_t (*lcd_set_window)(struct lcd_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom);
	int32_t (*lcd_invalidate)(struct lcd_spec *self);
	int32_t (*lcd_invalidate_rect)(struct lcd_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom);
	int32_t (*lcd_rotate_invalidate_rect)(struct lcd_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom,
				uint16_t angle);
	int32_t (*lcd_set_direction)(struct lcd_spec *self, uint16_t direction);
	uint32_t (*lcd_readid)(struct lcd_spec *self);
};

/* RGB LCD specific properties */
struct timing_rgb {
	uint16_t bfw;
	uint16_t efw;
	uint16_t vsw;
	uint16_t blw;
	uint16_t elw;
	uint16_t hsw;
};

struct ops_rgb {
	int32_t (*send_cmd)(uint32_t cmd);
	int32_t (*send_cmd_data)(uint32_t cmd, uint32_t data);
};

struct info_rgb {
	/* under construction... */
	struct timing_rgb timing;
	struct ops_rgb *ops;
};

/* MCU LCD specific properties */
struct timing_mcu {
	uint16_t rcss;
	uint16_t rlpw;
	uint16_t rhpw;
	uint16_t wcss;
	uint16_t wlpw;
	uint16_t whpw;
};

struct ops_mcu {
	int32_t (*send_cmd)(uint32_t cmd);
	int32_t (*send_cmd_data)(uint32_t cmd, uint32_t data);
	int32_t (*send_data)(uint32_t data);
	uint32_t (*read_data)(void);
};

struct info_mcu {
	uint16_t bus_mode;
	uint16_t bus_width;
	struct timing_mcu *timing;
	struct ops_mcu *ops;
};

/* LCD abstraction */
struct lcd_spec {
	uint16_t width;
	uint16_t height;
	uint16_t mode;
	uint16_t direction;
	union {
		struct info_rgb *rgb;
		struct info_mcu *mcu;
	} info;
	struct lcd_operations *ops;
};

extern struct lcd_spec lcd_panel_hx8357;
extern struct lcd_spec lcd_panel_ili9328;
extern struct lcd_spec lcd_panel_r61581;
extern struct lcd_spec lcd_panel_rm68040;
#endif
