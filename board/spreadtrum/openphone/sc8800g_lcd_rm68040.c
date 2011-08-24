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


static int32_t rm68040_init(struct lcd_spec *self)
{
	LCD_PRINT("rm68040_init\n");

	self->info.mcu->ops->send_cmd(0x11); 		
	mdelay(20);

	self->info.mcu->ops->send_cmd(0xd0); 		
	self->info.mcu->ops->send_data(0x07);
	self->info.mcu->ops->send_data(0x41);//42
	self->info.mcu->ops->send_data(0x1d);//0c

	self->info.mcu->ops->send_cmd(0xd1); 		
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x02);//15
	self->info.mcu->ops->send_data(0x12);//13

	self->info.mcu->ops->send_cmd(0xd2); 		
	self->info.mcu->ops->send_data(0x01);
	self->info.mcu->ops->send_data(0x22);//22

	self->info.mcu->ops->send_cmd(0xc0); 		
	self->info.mcu->ops->send_data(0x00);//00
	self->info.mcu->ops->send_data(0x3b);//3b
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x02);
	self->info.mcu->ops->send_data(0x11);  

	self->info.mcu->ops->send_cmd(0xc5); 		
	self->info.mcu->ops->send_data(0x03);  


	//gamma
	self->info.mcu->ops->send_cmd(0xc8); 		
	self->info.mcu->ops->send_data(0x02);
	self->info.mcu->ops->send_data(0x75);
	self->info.mcu->ops->send_data(0x77);
	self->info.mcu->ops->send_data(0x05);
	self->info.mcu->ops->send_data(0x03);
	self->info.mcu->ops->send_data(0x01);
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x20);
	self->info.mcu->ops->send_data(0x57);
	self->info.mcu->ops->send_data(0x50);
	self->info.mcu->ops->send_data(0x01);
	self->info.mcu->ops->send_data(0x03);


	self->info.mcu->ops->send_cmd(0xF8); 		
	self->info.mcu->ops->send_data(0x01);


	self->info.mcu->ops->send_cmd(0xfe); 		
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x02);


	self->info.mcu->ops->send_cmd(0x36); 	//set scan direction	
	self->info.mcu->ops->send_data(0x0a);	//


	self->info.mcu->ops->send_cmd(0x3a); //set pixel format		
	self->info.mcu->ops->send_data(0x55);	//16bpp

	self->info.mcu->ops->send_cmd(0x2a); 	//set hor address 	
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x00);	//start
	self->info.mcu->ops->send_data(0x01);
	self->info.mcu->ops->send_data(0x3f);	//end

	self->info.mcu->ops->send_cmd(0x2b); 	//set ver address	
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x00);	//start
	self->info.mcu->ops->send_data(0x01);
	self->info.mcu->ops->send_data(0xdf);	//end

	mdelay(120);

	self->info.mcu->ops->send_cmd(0x29); 	//display on
	self->info.mcu->ops->send_cmd(0x2c); 	//start refresh
	
	while(0)
	{
		//self->info.mcu->ops->send_cmd(0x29); 	//display on
		self->info.mcu->ops->send_data(0xffff);
//		self->info.mcu->ops->send_data(0x00ff);
		mdelay(1);
		printk("lcd test\n");
	}
	
    if (0){
	    int i;
	    for (i=0; i<320*480/2; i++)
	    	{
		    self->info.mcu->ops->send_data(0xff00);
		    printk("lcd write 0xff00");
	    	}
	    for (i=0; i< 320*480/2; i++)
		    self->info.mcu->ops->send_data(0x00ff);
		    printk("lcd write 0x00ff");		
    }

	return 0;
}

static int32_t rm68040_set_window(struct lcd_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	LCD_PRINT("rm68040_set_window\n");
	
	self->info.mcu->ops->send_cmd(0x2a); 	//set hor address 	
	self->info.mcu->ops->send_data((left >> 8));
	self->info.mcu->ops->send_data((left & 0xff));	//start
	self->info.mcu->ops->send_data((right >> 8));
	self->info.mcu->ops->send_data((right & 0xff));	//end

	self->info.mcu->ops->send_cmd(0x2b); 	//set ver address	
	self->info.mcu->ops->send_data((top >> 8));
	self->info.mcu->ops->send_data((top & 0xff));	//start
	self->info.mcu->ops->send_data((bottom >> 8));
	self->info.mcu->ops->send_data((bottom & 0xff));	//end

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
	self->info.mcu->ops->send_cmd(0x29);
	self->info.mcu->ops->send_cmd(0x2c); 	//start refresh

	return 0;
}

static int32_t rm68040_invalidate(struct lcd_spec *self)
{
	LCD_PRINT("rm68040_invalidate\n");

	return self->ops->lcd_set_window(self, 0, 0, 
			self->width-1, self->height-1);
	
}

static int32_t rm68040_set_direction(struct lcd_spec *self, uint16_t direction)
{
	LCD_PRINT("hx8357_set_direction\n");

	switch (direction) {
	case LCD_DIRECT_NORMAL:
		//self->info.mcu->ops->send_cmd_data(0x0016, 0x0009);
		break;
	case LCD_DIRECT_ROT_90:
		//self->info.mcu->ops->send_cmd_data(0x0016, 0x0069);
		break;
	case LCD_DIRECT_ROT_180:
		//self->info.mcu->ops->send_cmd_data(0x0016, 0x00c9);
		break;
	case LCD_DIRECT_ROT_270:
		//self->info.mcu->ops->send_cmd_data(0x0016, 0x00a9);
		break;
	case LCD_DIRECT_MIR_H:
		//self->info.mcu->ops->send_cmd_data(0x0016, 0x00c8);
		break;
	case LCD_DIRECT_MIR_V:
		//self->info.mcu->ops->send_cmd_data(0x0016, 0x0008);
		break;
	case LCD_DIRECT_MIR_HV:
		//self->info.mcu->ops->send_cmd_data(0x0016, 0x0048);
		break;
	default:
		LCD_PRINT("unknown lcd direction!\n");
		//self->info.mcu->ops->send_cmd_data(0x0016, 0x0009);
		//direction = LCD_DIRECT_NORMAL;
		break;
	}

	self->direction = direction;
	
	return 0;
}

static int32_t rm68040_enter_sleep(struct lcd_spec *self, uint8_t is_sleep)
{
	if(is_sleep) {
		self->info.mcu->ops->send_cmd(0x10);
	}
	else{
		self->info.mcu->ops->send_cmd(0x11);
		mdelay(120);
	}
	return 0;
}

static uint32_t rm68040_readid(struct lcd_spec *self)
{
	uint32_t dummy;
	self->info.mcu->ops->send_cmd(0xBF);

	self->info.mcu->ops->read_data();
	self->info.mcu->ops->read_data();
	self->info.mcu->ops->read_data();
	dummy = (self->info.mcu->ops->read_data())&0xff;
	dummy <<= 8;
	dummy |= (self->info.mcu->ops->read_data())&0xff;
	if(self->info.mcu->ops->read_data() == 0xff)
	{
		return dummy;
	}
	else
	{
		return 0;
	}
}

static struct lcd_operations lcd_rm68040_operations = {
	.lcd_init = rm68040_init,
	.lcd_set_window = rm68040_set_window,
	.lcd_invalidate = rm68040_invalidate,
	.lcd_set_direction = rm68040_set_direction,
	.lcd_enter_sleep = rm68040_enter_sleep,
	.lcd_readid = rm68040_readid,
};

static struct timing_mcu lcd_rm68040_timing = {
	.rcss = 20,
	.rlpw = 170,
	.rhpw = 250,
	.wcss = 20,
	.wlpw = 25,
	.whpw = 30,
};

/*
static struct timing_mcu lcd_rm68040_timing = {
	.rcss = 30,
	.rlpw = 180,
	.rhpw = 260,
	.wcss = 30,
	.wlpw = 35,
	.whpw = 40,
};
*/
static struct info_mcu lcd_rm68040_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 16,
	.timing = &lcd_rm68040_timing,
	.ops = NULL,
};

struct lcd_spec lcd_panel_rm68040 = {
	.width = 320,
	.height = 480,
	.mode = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_rm68040_info},
	.ops = &lcd_rm68040_operations,
};

//vidinfo_t panel_info = {
    //.vl_col = 320,
    //.vl_bpix = 4,
    //.vl_row = 480,
//};

