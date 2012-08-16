/* drivers/video/sc8800g/sc8800g_lcd_eR6158_truly.c
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
#include <asm/arch/sc8810_lcd.h>
#define mdelay(a) udelay(a * 1000)
#define printk printf

//#define  LCD_DEBUG
#ifdef LCD_DEBUG
#define LCD_PRINT printk
#else
#define LCD_PRINT(...)
#endif

//#define CONFIG_FB_LCD_NOFMARK

static int32_t eR6158_truly_init(struct lcd_spec *self)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("eR6158_truly_init\n");
    printk("[tong][uboot]eR6158_truly_init~~~~~~~~~~~~~~~~~~~~~\n");//tong test

/*
  HW reset
*/
      send_cmd(0x01);//soft reset
      mdelay(120);

      send_cmd(0xFF);
	send_cmd(0xFF);
	mdelay(5);
	send_cmd(0xFF);
	send_cmd(0xFF);
	send_cmd(0xFF);
	send_cmd(0xFF);
	mdelay(10);

	send_cmd(0xB0);//{setc, [107], W, 0x000B0}
	send_data(0x00);//{setp, [104], W, 0x00000}

	send_cmd(0xB3);
	send_data(0x02);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);

	send_cmd(0xB4);
	send_data(0x00);

	send_cmd(0xC0);
	send_data(0x17);
	send_data(0x3B);//480
	send_data(0x00);
	send_data(0x00);//BLV=0,PTL=0
	send_data(0x00);
	send_data(0x01);
	send_data(0x00);//
	send_data(0x77);

	send_cmd(0xC1);
	send_data(0x08);// BC=1 //div=0
	send_data(0x16);//CLOCK
	send_data(0x08);
	send_data(0x08);

	send_cmd(0xC4);
	send_data(0x11);
	send_data(0x07);
	send_data(0x03);
	send_data(0x03);

	send_cmd(0xC8);//GAMMA
	send_data(0x04);
	send_data(0x0C);
	send_data(0x0A);
	send_data(0x59);
	send_data(0x06);
	send_data(0x08);
	send_data(0x0f);
	send_data(0x07);

	send_data(0x00);
	send_data(0x32);

	send_data(0x07);
	send_data(0x0f);
	send_data(0x08);
	send_data(0x56);//43/55
	send_data(0x09);
	send_data(0x0A);
	send_data(0x0C);
	send_data(0x04);

	send_data(0x32);
	send_data(0x00);

	send_cmd(0x2A);
	send_data(0x00);
	send_data(0x00);
	send_data(0x01);
	send_data(0x3F);//320

	send_cmd(0x2B);
	send_data(0x00);
	send_data(0x00);
	send_data(0x01);
	send_data(0xDF);//480

#ifndef CONFIG_FB_LCD_NOFMARK
	send_cmd(0x35);
	send_data(0x00);
#endif

	send_cmd(0x36);
	//send_data(0x80);
	send_data(0x40);//tong test

	send_cmd(0x3A);
	send_data(0x66);

	send_cmd(0x44);
	send_data(0x00);
	send_data(0x01);

	send_cmd(0x11);
	mdelay(150);

	send_cmd(0xD0);
	send_data(0x07);
	send_data(0x07);
	send_data(0x1E); //
	send_data(0x33);


	send_cmd(0xD1);
	send_data(0x03);
	send_data(0x4B);//VCM40
	send_data(0x10);//VDV

	send_cmd(0xD2);
	send_data(0x03);
	send_data(0x04);//0X24
	send_data(0x04);

	send_cmd(0x29);
	mdelay(10);  

	send_cmd(0xB4);
	send_data(0x00);
	send_cmd(0x2C);
	mdelay(20);  

	return 0;
}

static int32_t eR6158_truly_set_window(struct lcd_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("eR6158_truly_set_window: %d, %d, %d, %d\n",left, top, right, bottom);
    
	/* set window size  */

	send_cmd(0x2A);
	send_data(left  >> 8);
	send_data(left  & 0xff);
	send_data(right  >> 8);
	send_data(right  & 0xff);
	
	
	send_cmd(0x2B);
	send_data(top  >> 8);
	send_data(top  & 0xff);
	send_data(bottom  >> 8);
	send_data(bottom  & 0xff);
	
	send_cmd(0x2C);


	return 0;
}

static int32_t eR6158_truly_invalidate(struct lcd_spec *self)
{
	LCD_PRINT("eR6158_truly_invalidate\n");

	return self->ops->lcd_set_window(self, 0, 0, 
			self->width - 1, self->height - 1);
	
}

static int32_t eR6158_truly_invalidate_rect(struct lcd_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	//Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;

	LCD_PRINT("eR6158_truly_invalidate_rect \n");

	return self->ops->lcd_set_window(self, left, top, 
			right, bottom);
}

static int32_t eR6158_truly_set_direction(struct lcd_spec *self, uint16_t direction)
{
	//Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;

	LCD_PRINT("eR6158_truly_set_direction\n");

	self->direction = direction;
	
	return 0;
}

static int32_t eR6158_truly_enter_sleep(struct lcd_spec *self, uint8_t is_sleep)
{
	//Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	//Send_data send_data = self->info.mcu->ops->send_data;

      if(is_sleep) 
	{
		send_cmd(0x28);
		LCD_DelayMS(50);
             send_cmd(0x10);
		LCD_DelayMS(200);
	}
	else
	{
		send_cmd(0x11); // SLPOUT
		LCD_DelayMS(120);
		send_cmd(0x29);		
		LCD_DelayMS(50);		
	}
    
	return 0;
}

static uint32_t eR6158_truly_read_id(struct lcd_spec *self)
{

      Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Read_data read_data = self->info.mcu->ops->read_data;
      Send_data send_data = self->info.mcu->ops->send_data;
      uint32_t uID = 0;
      uint32_t uICID[5] = {0};
      uint32_t i;


      send_cmd(0x01);//soft reset
      mdelay(120);
#if 0
      send_cmd(0xB0);
      send_data(0x00);
      
      send_cmd(0xBF);
      for(i = 0; i < 5; i++)
      {
          uICID[i] = read_data();
          printk("[tong][uboot]eR6158_truly_read_id: uICID[%d] = 0x%x\n", i, uICID[i]);
      }

      if((uICID[1] == 0x01) && (uICID[2] == 0x22) && (uICID[3] == 0x15) && (uICID[4] == 0x81))
      {
          printk("[tong][uboot]LCD driver IC: r61581\n");          
      }
      else
      {
          printk("[tong][uboot]LCD driver IC: hx8357c\n");
          return -1;
      }
#else
      send_cmd(0xB9);
      send_data(0xFF);
      send_data(0x83);
      send_data(0x57);

      send_cmd(0xD0);
      for(i = 0; i < 2; i++)
      {
          uICID[i] = read_data();
          printk("[tong][uboot]eR6158_truly_read_id: uICID[%d] = 0x%x\n", i, uICID[i]);        
      }
      
      if((uICID[1] == 0x90))
      {
          printk("[tong][uboot]LCD driver IC: hx8357c\n");
          return -1;
      }
      else
      {  
          printk("[tong][uboot]LCD driver IC: r61581\n");
          
      }
     
      
#endif         
      send_cmd(0xA1);
      uID = read_data();
      printk("[tong][uboot]eR6158_truly_read_id: 0x%x from addr:0xA1\n", uID);
     
	return (0x6158|uID); 
}

static struct lcd_operations lcd_eR6158_truly_operations = {
	.lcd_init = eR6158_truly_init,
	.lcd_set_window = eR6158_truly_set_window,
	.lcd_invalidate_rect= eR6158_truly_invalidate_rect,
	.lcd_invalidate = eR6158_truly_invalidate,
	.lcd_set_direction = eR6158_truly_set_direction,
	.lcd_enter_sleep = eR6158_truly_enter_sleep,
	.lcd_readid          = eR6158_truly_read_id,
};

static struct timing_mcu lcd_eR6158_truly_timing[] = {
    
    [LCD_REGISTER_TIMING] = {                    // read/write register timing
		.rcss = 170,  
		.rlpw = 190,
		.rhpw = 260,
		.wcss = 30,
		.wlpw = 30,
		.whpw = 30,
    },
    [LCD_GRAM_TIMING] = {                    // read/write gram timing
	.rcss = 170,
	.rlpw = 170,
	.rhpw = 250,
	.wcss = 30,
	.wlpw = 30,
	.whpw = 30,
	},
};

static struct info_mcu lcd_eR6158_truly_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 18,
	.timing = lcd_eR6158_truly_timing,
	.ops = NULL,
};

struct lcd_spec lcd_panel_eR6158_truly = {
	.width = 320,
	.height = 480,
	.mode = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_eR6158_truly_info},
	.ops = &lcd_eR6158_truly_operations,
};
