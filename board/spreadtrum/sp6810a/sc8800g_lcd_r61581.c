/* drivers/video/sc8800g/sc8800g_lcd_hx8357.c
 *
 * Support for HX8357 LCD device
 *
 * Copyright (C) 2010 Spreadtrum
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
//#include <linux/delay.h>

#include <asm/arch/sc8800g_lcd.h>
#include <lcd.h>
#define mdelay(_ms) udelay(_ms*1000)
#define printk printf

//#define  LCD_DEBUG
#ifdef LCD_DEBUG
#define LCD_PRINT printk
#else
#define LCD_PRINT(...)
#endif

static int32_t r61581_init(struct lcd_spec *self)
{
	LCD_PRINT("r61581_init\n");

	self->info.mcu->ops->send_cmd_data(0xB0,0x00);
	
	self->info.mcu->ops->send_cmd_data(0xB3,0x02);
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x00);
	
	self->info.mcu->ops->send_cmd_data(0xB4,0x00);

	self->info.mcu->ops->send_cmd_data(0xC0,0x03);
	self->info.mcu->ops->send_data(0x3B);
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x02);
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x01);
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x43);
	

	self->info.mcu->ops->send_cmd_data(0xC1,0x08);
	self->info.mcu->ops->send_data(0x17);
	self->info.mcu->ops->send_data(0x08);
	self->info.mcu->ops->send_data(0x08);
	
	self->info.mcu->ops->send_cmd_data(0xC4,0x22);
	self->info.mcu->ops->send_data(0x02);
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x00);
	
	self->info.mcu->ops->send_cmd_data(0xC8,0x09);
	self->info.mcu->ops->send_data(0x08);
	self->info.mcu->ops->send_data(0x10);
	self->info.mcu->ops->send_data(0x85);
	self->info.mcu->ops->send_data(0x07);
	self->info.mcu->ops->send_data(0x08);
	self->info.mcu->ops->send_data(0x16);
	self->info.mcu->ops->send_data(0x05);
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x32);
	self->info.mcu->ops->send_data(0x05);
	self->info.mcu->ops->send_data(0x16);
	self->info.mcu->ops->send_data(0x08);
	self->info.mcu->ops->send_data(0x88);
	self->info.mcu->ops->send_data(0x09);	
	self->info.mcu->ops->send_data(0x10);	
	self->info.mcu->ops->send_data(0x09);	
	self->info.mcu->ops->send_data(0x04);	
	self->info.mcu->ops->send_data(0x32);		
	self->info.mcu->ops->send_data(0x00);	
	
	self->info.mcu->ops->send_cmd_data(0x2A,0x00);	
	self->info.mcu->ops->send_data(0x00);	
	self->info.mcu->ops->send_data(0x01);		
	self->info.mcu->ops->send_data(0x3F);	
	
	self->info.mcu->ops->send_cmd_data(0x2B,0x00);	
	self->info.mcu->ops->send_data(0x00);	
	self->info.mcu->ops->send_data(0x01);		
	self->info.mcu->ops->send_data(0xDF);	
	
	self->info.mcu->ops->send_cmd_data(0x35,0x00);		
	self->info.mcu->ops->send_cmd_data(0x3A,0x05);		

	self->info.mcu->ops->send_cmd_data(0x44,0x00);	
	self->info.mcu->ops->send_data(0x01);		
	
	self->info.mcu->ops->send_cmd(0x2C);
	self->info.mcu->ops->send_cmd(0x11);
	mdelay(150);
	
	self->info.mcu->ops->send_cmd_data(0xD0,0x07);	
	self->info.mcu->ops->send_data(0x07);
	self->info.mcu->ops->send_data(0x16);		
	self->info.mcu->ops->send_data(0x72);	
	
	self->info.mcu->ops->send_cmd_data(0xD1,0x03);	
	self->info.mcu->ops->send_data(0x3A);
	self->info.mcu->ops->send_data(0x0A);
	
	self->info.mcu->ops->send_cmd_data(0xD2,0x02);	
	self->info.mcu->ops->send_data(0x44);
	self->info.mcu->ops->send_data(0x04);
	
	self->info.mcu->ops->send_cmd(0x29);	
	mdelay(150);	

//	self->info.mcu->ops->send_cmd(0x002c); //refresh

	return 0;
}

static int32_t r61581_set_window(struct lcd_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	LCD_PRINT("r61581_set_window\n");
    
    /* set window size  */
	self->info.mcu->ops->send_cmd_data(0x002A, (left >> 8));
	self->info.mcu->ops->send_data( (left & 0xff));
	self->info.mcu->ops->send_data((right >> 8));
	self->info.mcu->ops->send_data((right & 0xff));
//	self->info.mcu->ops->send_cmd_data(0x002A,  (left & 0xff));
//	self->info.mcu->ops->send_cmd_data(0x002A, (right >> 8));
//	self->info.mcu->ops->send_cmd_data(0x002A, (right & 0xff));

	self->info.mcu->ops->send_cmd_data(0x002B, (top >> 8));
	self->info.mcu->ops->send_data( (top & 0xff));
	self->info.mcu->ops->send_data((bottom >> 8));
	self->info.mcu->ops->send_data((bottom & 0xff));
//	self->info.mcu->ops->send_cmd_data(0x002B, (top & 0xff));
//	self->info.mcu->ops->send_cmd_data(0x002B,(bottom >> 8));
//	self->info.mcu->ops->send_cmd_data(0x002B,(bottom & 0xff));

#if 0
	switch (self->direction) {
	case LCD_DIRECT_NORMAL:
    default:
		self->info.mcu->ops->send_cmd_data(0x0080, (left >> 8));
		self->info.mcu->ops->send_cmd_data(0x0081, (left & 0xff));
		self->info.mcu->ops->send_cmd_data(0x0082, (top >> 8));
		self->info.mcu->ops->send_cmd_data(0x0083, (top & 0xff));
        break;
	case LCD_DIRECT_ROT_90:
		self->info.mcu->ops->send_cmd_data(0x0080, (top >> 8));
		self->info.mcu->ops->send_cmd_data(0x0081, (top & 0xff));
		self->info.mcu->ops->send_cmd_data(0x0082, (left >> 8));
		self->info.mcu->ops->send_cmd_data(0x0083, (left & 0xff));
	    break;
    case LCD_DIRECT_ROT_180:
	case LCD_DIRECT_MIR_HV:
		self->info.mcu->ops->send_cmd_data(0x0080, (right >> 8));
		self->info.mcu->ops->send_cmd_data(0x0081, (right & 0xff));
		self->info.mcu->ops->send_cmd_data(0x0082, (bottom >> 8));
		self->info.mcu->ops->send_cmd_data(0x0083, (bottom & 0xff));
	    break;
	case LCD_DIRECT_ROT_270:
		self->info.mcu->ops->send_cmd_data(0x0080, (bottom >> 8));
		self->info.mcu->ops->send_cmd_data(0x0081, (bottom & 0xff));
		self->info.mcu->ops->send_cmd_data(0x0082, (left >> 8));
		self->info.mcu->ops->send_cmd_data(0x0083, (left & 0xff));
	    break;
	case LCD_DIRECT_MIR_H:
		self->info.mcu->ops->send_cmd_data(0x0080, (left >> 8));
		self->info.mcu->ops->send_cmd_data(0x0081, (left & 0xff));
		self->info.mcu->ops->send_cmd_data(0x0082, (bottom >> 8));
		self->info.mcu->ops->send_cmd_data(0x0083, (bottom & 0xff));
	    break;
	case LCD_DIRECT_MIR_V:
		self->info.mcu->ops->send_cmd_data(0x0080, (right >> 8));
		self->info.mcu->ops->send_cmd_data(0x0081, (right & 0xff));
		self->info.mcu->ops->send_cmd_data(0x0082, (top >> 8));
		self->info.mcu->ops->send_cmd_data(0x0083, (top & 0xff));
	    break;
	}
#endif
	self->info.mcu->ops->send_cmd(0x002c);

	return 0;
}

static int32_t r61581_invalidate(struct lcd_spec *self)
{
	LCD_PRINT("r61581_invalidate\n");

	return self->ops->lcd_set_window(self, 0, 0, 
			self->width-1, self->height-1);
	
}

static int32_t r61581_set_direction(struct lcd_spec *self, uint16_t direction)
{
	LCD_PRINT("r61581_set_direction\n");
#if 0
	switch (direction) {
	case LCD_DIRECT_NORMAL:
		self->info.mcu->ops->send_cmd_data(0x0016, 0x0009);
		break;
	case LCD_DIRECT_ROT_90:
		self->info.mcu->ops->send_cmd_data(0x0016, 0x0069);
		break;
	case LCD_DIRECT_ROT_180:
		self->info.mcu->ops->send_cmd_data(0x0016, 0x00c9);
		break;
	case LCD_DIRECT_ROT_270:
		self->info.mcu->ops->send_cmd_data(0x0016, 0x00a9);
		break;
	case LCD_DIRECT_MIR_H:
		self->info.mcu->ops->send_cmd_data(0x0016, 0x00c8);
		break;
	case LCD_DIRECT_MIR_V:
		self->info.mcu->ops->send_cmd_data(0x0016, 0x0008);
		break;
	case LCD_DIRECT_MIR_HV:
		self->info.mcu->ops->send_cmd_data(0x0016, 0x0048);
		break;
	default:
		LCD_PRINT("unknown lcd direction!\n");
		self->info.mcu->ops->send_cmd_data(0x0016, 0x0009);
		direction = LCD_DIRECT_NORMAL;
		break;
	}
#endif
	self->direction = direction;
	
	return 0;
}

static int32_t r61581_enter_sleep(struct lcd_spec *self, uint8_t is_sleep)
{
	if ( is_sleep ){
		self->info.mcu->ops->send_cmd(0x0028);
		self->info.mcu->ops->send_cmd(0x0010);
		mdelay(150);
	}
	else{
		self->info.mcu->ops->send_cmd(0xFF);
		self->info.mcu->ops->send_cmd(0xFF);
		self->info.mcu->ops->send_cmd(0xFF);
		self->info.mcu->ops->send_cmd(0xFF);
		self->info.mcu->ops->send_cmd(0xFF);
		self->info.mcu->ops->send_cmd(0xFF);
		mdelay(15);

		self->info.mcu->ops->send_cmd(0x29);
		mdelay(60);

		self->info.mcu->ops->send_cmd(0x11);
		mdelay(60);
	}
	return 0;
}

static struct lcd_operations lcd_r61581_operations = {
	.lcd_init = r61581_init,
	.lcd_set_window = r61581_set_window,
	.lcd_invalidate = r61581_invalidate,
	.lcd_set_direction = r61581_set_direction,
	.lcd_enter_sleep = r61581_enter_sleep,
};

static struct timing_mcu lcd_r61581_timing = {
	.rcss = 450,
	.rlpw = 170,
	.rhpw = 250,
	.wcss = 60,
	.wlpw = 30,
	.whpw = 30,
};

static struct info_mcu lcd_r61581_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 16,
	.timing = &lcd_r61581_timing,
	.ops = NULL,
};

struct lcd_spec lcd_panel_r61581 = {
	.width = 320,
	.height = 480,
	.mode = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_r61581_info},
	.ops = &lcd_r61581_operations,
};

//vidinfo_t panel_info = {
    //.vl_col = 320,
    //.vl_bpix = 4,
    //.vl_row = 480,
//};


