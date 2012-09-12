/* drivers/video/sc8810/sc8810_lcd.h
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

#ifndef _SC8810_LCD_H_
#define _SC8810_LCD_H_

#include <linux/types.h>
#include <common.h>

#define LCD_DelayMS(ms)  udelay(1000*(ms))

#define LCD_REGISTER_TIMING 0
#define LCD_GRAM_TIMING     1

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

#define PIN_PWM0_MOD_VALUE 0x20

#define SPRD_PWM_BASE 			0x88000000
#define SPRD_PWM_REG(off) 		(SPRD_PWM_BASE + (off))
#define SPRD_PWM0_PRESCALE   	SPRD_PWM_REG(0x0000)
#define SPRD_PWM0_CNT 			SPRD_PWM_REG(0x0004)
#define SPRD_PWM0_TONE_DIV 	SPRD_PWM_REG(0x0008)
#define SPRD_PWM0_PAT_LOW 	SPRD_PWM_REG(0x000C)
#define SPRD_PWM0_PAT_HIG 	SPRD_PWM_REG(0x0010)

#define LCD_PWM_PRESCALE_VALUE 	0x01
#define LCD_PWM_MOD_VALUE 		0xFF
#define PWM_REG_MSK_VALUE			0xFFFF

#define LCD_PWM0_EN BIT_8

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

typedef int32_t (*Send_cmd)(uint32_t data);
typedef int32_t (*Send_data)(uint32_t data);
typedef int32_t (*Send_cmd_data)(uint32_t cmd, uint32_t data);
typedef uint32_t (*Read_data)(void);

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

extern void LCD_SetBackLightBrightness( unsigned long  value);
#endif

