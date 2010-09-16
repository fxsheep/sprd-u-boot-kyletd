/******************************************************************************
 ** File Name:    sc8800_reg_lcdc.h                                           *
 ** Author:       Jianping.Wang                                               *
 ** DATE:         01/21/2006                                                  *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 01/21/2006    Jianping.Wang   Create.                                     *
 ******************************************************************************/
#ifndef _SC8800_REG_LCDC_
#define _SC8800_REG_LCDC_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                          Compiler Flag                                    *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/
#define LCDC_REG_BASE				0x20700000
#define LCDC_LCD_MODE				(LCDC_REG_BASE + 0x0000) 
#define LCDC_LCD_SIZE				(LCDC_REG_BASE + 0x0004)
#define LCDC_LCD_STATUS				(LCDC_REG_BASE + 0x0008)
#define LCDC_LCD_BACKGROUND			(LCDC_REG_BASE + 0x000c)
#define LCDC_LCD_BLOCK0ADDR			(LCDC_REG_BASE + 0x0010)
#define LCDC_LCD_BLOCK0START		(LCDC_REG_BASE + 0x0014)
#define LCDC_LCD_BLOCK0END			(LCDC_REG_BASE + 0x0018)
#define LCDC_LCD_BLOCK0CONFIG		(LCDC_REG_BASE + 0x001c)
#define LCDC_LCD_BLOCK1ADDR			(LCDC_REG_BASE + 0x0020)
#define LCDC_LCD_BLOCK1START		(LCDC_REG_BASE + 0x0024)
#define LCDC_LCD_BLOCK1END			(LCDC_REG_BASE + 0x0028)
#define LCDC_LCD_BLOCK1CONFIG		(LCDC_REG_BASE + 0x002c)
#define LCDC_LCD_BLOCK2ADDR			(LCDC_REG_BASE + 0x0030)
#define LCDC_LCD_BLOCK2START		(LCDC_REG_BASE + 0x0034)
#define LCDC_LCD_BLOCK2END			(LCDC_REG_BASE + 0x0038)
#define LCDC_LCD_BLOCK2CONFIG		(LCDC_REG_BASE + 0x003c)
#define LCDC_LCD_BLOCK3ADDR			(LCDC_REG_BASE + 0x0040)
#define LCDC_LCD_BLOCK3START		(LCDC_REG_BASE + 0x0044)
#define LCDC_LCD_BLOCK3END			(LCDC_REG_BASE + 0x0048)
#define LCDC_LCD_BLOCK3CONFIG		(LCDC_REG_BASE + 0x004c)
#define LCDC_LCD_BLOCK4ADDR			(LCDC_REG_BASE + 0x0050)
#define LCDC_LCD_BLOCK4START		(LCDC_REG_BASE + 0x0054)
#define LCDC_LCD_BLOCK4END			(LCDC_REG_BASE + 0x0058)
#define LCDC_LCD_BLOCK4CONFIG		(LCDC_REG_BASE + 0x005c)
#define LCDC_LCD_BLOCK5ADDR			(LCDC_REG_BASE + 0x0060)
#define LCDC_LCD_BLOCK5START		(LCDC_REG_BASE + 0x0064)
#define LCDC_LCD_BLOCK5END			(LCDC_REG_BASE + 0x0068)
#define LCDC_LCD_BLOCK5CONFIG		(LCDC_REG_BASE + 0x006c)

#define LCDC_COLOR_COEFF_A1			(LCDC_REG_BASE + 0x0070)
#define LCDC_COLOR_COEFF_A2			(LCDC_REG_BASE + 0x0074)
#define LCDC_COLOR_COEFF_A3			(LCDC_REG_BASE + 0x0078)
#define LCDC_COLOR_COEFF_B1			(LCDC_REG_BASE + 0x007c)
#define LCDC_COLOR_COEFF_B2			(LCDC_REG_BASE + 0x0080)
#define LCDC_COLOR_COEFF_B3			(LCDC_REG_BASE + 0x0084)
#define LCDC_COLOR_COEFF_C1			(LCDC_REG_BASE + 0x0088)
#define LCDC_COLOR_COEFF_C2			(LCDC_REG_BASE + 0x008c)
#define LCDC_COLOR_COEFF_C3			(LCDC_REG_BASE + 0x0090)


#define LCDC_LCD_INT_ENABLE			(LCDC_REG_BASE + 0x0094)
#define LCDC_LCD_INT_STATUS			(LCDC_REG_BASE + 0x0098)
#define LCDC_LCD_INT_CLEAR			(LCDC_REG_BASE + 0x009c)

#define LCDC_FIFO_THRESHOLD         (LCDC_REG_BASE + 0x00a0)
#define LCDC_FIFO_CURR_STATUS       (LCDC_REG_BASE + 0x00a4)
#define LCDC_FIFO_MIN_STATUS        (LCDC_REG_BASE + 0x00a8)
#define LCDC_DISP_WIN_START_ADDR    (LCDC_REG_BASE + 0x00ac)
#define LCDC_DISP_WIN_END_ADDR      (LCDC_REG_BASE + 0x00b0)
	
#define LCDC_LCD_TIMING0			(LCDC_REG_BASE + 0x0100)
#define LCDC_LCD_TIMING1			(LCDC_REG_BASE + 0x0104)
#define LCDC_LCD_CTRL_REG			(LCDC_REG_BASE + 0x0108)
#define LCDC_TV_CONTROL_REG			(LCDC_REG_BASE + 0x010c)
#define LCDC_DAC_CONTROL_REG		(LCDC_REG_BASE + 0x0110)

#define LCD_MOD_LCM_SEL             ((*(volatile uint32 *)LCDC_LCD_MODE) & BIT_11)   //LCM0=FALSE, LCM1=TRUE
#define LCM_RW_MD                   0x60001c64
#if 1
#define LCDC_MCU_REG_BASE			(LCD_MOD_LCM_SEL?0x20600000:0x60000000)
#define LCDC_LCMPARAMETER0		    (LCD_MOD_LCM_SEL?(LCDC_MCU_REG_BASE + 0x0000):(LCDC_MCU_REG_BASE + 0x1c40))
#define LCDC_LCMPARAMETER1		    (LCD_MOD_LCM_SEL?(LCDC_MCU_REG_BASE + 0x0004):(LCDC_MCU_REG_BASE + 0x1c4c))
#define LCDC_LCMRSTN				(LCD_MOD_LCM_SEL?(LCDC_MCU_REG_BASE + 0x0008):(LCDC_MCU_REG_BASE + 0x1c44))
#define LCDC_LCMDATANUMBER		    (LCD_MOD_LCM_SEL?(LCDC_MCU_REG_BASE + 0x000C):(LCDC_MCU_REG_BASE + 0x1C68))    
#define LCDC_LCMRDDATA				(LCD_MOD_LCM_SEL?(LCDC_MCU_REG_BASE + 0x0010):(LCDC_MCU_REG_BASE + 0x1c54))
#define LCDC_LCMSTATUS				(LCD_MOD_LCM_SEL?(LCDC_MCU_REG_BASE + 0x0014):(LCDC_MCU_REG_BASE + 0x1c58))
#define LCDC_LCMMODESEL			    (LCD_MOD_LCM_SEL?(LCDC_MCU_REG_BASE + 0x0018):(LCDC_MCU_REG_BASE + 0x1c50))
#define LCDC_LCM_RGB_MODE			(LCD_MOD_LCM_SEL?(LCDC_MCU_REG_BASE + 0x001C):(LCDC_MCU_REG_BASE + 0x1c5c))
#define LCDC_LCM_SELPIN				(LCD_MOD_LCM_SEL?(LCDC_MCU_REG_BASE + 0x0020):(LCDC_MCU_REG_BASE + 0x1c60))
#define LCDC_LCM_INT_STATUS		    (LCD_MOD_LCM_SEL?(LCDC_MCU_REG_BASE + 0x0024):(LCDC_MCU_REG_BASE + 0x1c84))
#define LCDC_LCM_INT_EN			    (LCD_MOD_LCM_SEL?(LCDC_MCU_REG_BASE + 0x0028):(LCDC_MCU_REG_BASE + 0x1c88))
#define LCDC_LCM_INT_VECTOR		    (LCD_MOD_LCM_SEL?(LCDC_MCU_REG_BASE + 0x002C):(LCDC_MCU_REG_BASE + 0x1c8c))
#else
#define LCDC_MCU_REG_BASE			0x60000000
#define LCDC_LCMPARAMETER0		    (LCDC_MCU_REG_BASE + 0x1c40)
#define LCDC_LCMPARAMETER1		    (LCDC_MCU_REG_BASE + 0x1c4c)
#define LCDC_LCMRSTN				(LCDC_MCU_REG_BASE + 0x1c44)
#define LCDC_LCMDATANUMBER		    (LCDC_MCU_REG_BASE + 0x1C68)
#define LCDC_LCMRDDATA				(LCDC_MCU_REG_BASE + 0x1c54)
#define LCDC_LCMSTATUS				(LCDC_MCU_REG_BASE + 0x1c58)
#define LCDC_LCMMODESEL			    (LCDC_MCU_REG_BASE + 0x1c50)
#define LCDC_LCM_RGB_MODE			(LCDC_MCU_REG_BASE + 0x1c5c)
#define LCDC_LCM_SELPIN				(LCDC_MCU_REG_BASE + 0x1c60)
#define LCDC_LCM_INT_STATUS		    (LCDC_MCU_REG_BASE + 0x1c84)
#define LCDC_LCM_INT_EN			    (LCDC_MCU_REG_BASE + 0x1c88)
#define LCDC_LCM_INT_VECTOR		    (LCDC_MCU_REG_BASE + 0x1c8c)
#endif


#define LCDC_LCM_REMAIN			    (LCDC_MCU_REG_BASE + 0x0030)
#define LCDC_LCM_CS0_ADDR0		    (LCDC_MCU_REG_BASE + 0x1400)
#define LCDC_LCM_CS0_ADDR1		    (LCDC_MCU_REG_BASE + 0x1404)
#define LCDC_LCM_CS1_ADDR0		    (LCDC_MCU_REG_BASE + 0x1500)
#define LCDC_LCM_CS1_ADDR1		    (LCDC_MCU_REG_BASE + 0x1504)



/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/
typedef union _lcdc_lcd_mode_tag {
	struct _lcdc_lcd_mode_map {
		volatile unsigned int reserved       	:18;//Reserved ;
		
        volatile unsigned int update_evenfield  :1;
        volatile unsigned int update_2field_mode:1;
		volatile unsigned int lcm_select        :1; 		
		volatile unsigned int lcm1_enable       :1; 
		volatile unsigned int dither_enable     :1; 
		volatile unsigned int matrix_bp         :1;  
		volatile unsigned int invert_tv_field   :1;     	
		volatile unsigned int tvread_mode		:1;	//0:read line by line;1:interleaving read
		volatile unsigned int start_refresh		:1;	//
		volatile unsigned int refresh_mode		:1;//0:continue refresh frame;1:single frame
		volatile unsigned int shadow_update     :1;//0:Shadow register update disable;1:Shadow register updata enable
		volatile unsigned int lcd_enable		:1;//0:LCDC Disable;1:LCDC Enable
		volatile unsigned int tv_active			:1;//0:TV I/F Disable;1:TV I/F Enable
		volatile unsigned int lcdc_mode			:1;//0:RGB LCD I/F;1:MCU LCD I/F
	}mBits ;
	volatile unsigned int dwValue ;
}LCDC_LCD_MODE_U;


typedef union _lcdc_lcd_size_tag {
	struct _lcdc_lcd_size_map {
		volatile unsigned int reserved_1		:4;//Reserved
		volatile unsigned int lcd_size_y		:12;//
		volatile unsigned int reserved_2		:4;//Reserved
		volatile unsigned int lcd_size_x		:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_SIZE_U;

typedef union _lcdc_lcd_status_tag {
	struct _lcdc_lcd_status_map {
		volatile unsigned int reserved 			:31;//Reserved
		volatile unsigned int frame_status		:1;//0:Even frame;1:Odd frame
	}mBits;
	volatile unsigned int dValue;
}LCDC_LCD_STATUS_U;
	
typedef union _lcdc_lcd_background_tag {
	struct _lcdc_lcd_background_map {
		volatile unsigned int Reserved			:8;//Reserved
		volatile unsigned int background_r		:8;
		volatile unsigned int background_g		:8;
		volatile unsigned int background_b		:8;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BACKGROUND_U;

typedef union _lcdc_lcd_block0addr_tag {
	struct _lcdc_lcd_block0addr_map {
		volatile unsigned int start_addr		:32;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK0ADDR_U;

typedef union _lcdc_lcd_block0start_tag {
	struct _lcdc_lcd_block0start_map {
		volatile unsigned int enable			:1;
		volatile unsigned int priority			:3;
		volatile unsigned int start_addry		:12;
		volatile unsigned int reserved			:4;
		volatile unsigned int start_addx		:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK0START_U;

typedef union _lcdc_lcd_block0end_tag {
	struct _lcdc_lcd_block0end_map {
		volatile unsigned int width				:8;
		volatile unsigned int end_addry			:12;
		volatile unsigned int end_addrx			:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK0END_U;

typedef union _lcdc_lcd_block0config_tag {
	struct _lcdc_lcd_block0config_map {
		volatile unsigned int reserved			:3;
		volatile unsigned int type		        :1;
		volatile unsigned int colorkey_en 		:1;
		volatile unsigned int alpha_sel 		:1;
		volatile unsigned int resolution        :2;
		volatile unsigned int colorkey			:16;
		volatile unsigned int alpha				:8;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK0CONFIG_U;

typedef union _lcdc_lcd_block1addr_tag {
	struct _lcdc_lcd_block1addr_map {
		volatile unsigned int start_addr		:32;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK1ADDR_U;

typedef union _lcdc_lcd_block1start_tag {
	struct _lcdc_lcd_block1start_map {
		volatile unsigned int enable			:1;
		volatile unsigned int priority			:3;
		volatile unsigned int start_addry		:12;
		volatile unsigned int reserved			:4;
		volatile unsigned int start_addx		:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK1START_U;

typedef union _lcdc_lcd_block1end_tag {
	struct _lcdc_lcd_block1end_map {
		volatile unsigned int width				:8;
		volatile unsigned int end_addry			:12;
		volatile unsigned int end_addrx			:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK1END_U;

typedef union _lcdc_lcd_block1config_tag {
	struct _lcdc_lcd_block1config_map {
		volatile unsigned int reserved			:3;
		volatile unsigned int type		        :1;
		volatile unsigned int colorkey_en 		:1;
		volatile unsigned int alpha_sel 		:1;
		volatile unsigned int resolution        :2;
		volatile unsigned int colorkey			:16;
		volatile unsigned int alpha				:8;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK1CONFIG_U;

typedef union _lcdc_lcd_block2addr_tag {
	struct _lcdc_lcd_block2addr_map {
		volatile unsigned int start_addr		:32;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK2ADDR_U;

typedef union _lcdc_lcd_block2start_tag {
	struct _lcdc_lcd_block2start_map {
		volatile unsigned int enable			:1;
		volatile unsigned int priority			:3;
		volatile unsigned int start_addry		:12;
		volatile unsigned int reserved			:4;
		volatile unsigned int start_addx		:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK2START_U;

typedef union _lcdc_lcd_block2end_tag {
	struct _lcdc_lcd_block2end_map {
		volatile unsigned int width				:8;
		volatile unsigned int end_addry			:12;
		volatile unsigned int end_addrx			:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK2END_U;

typedef union _lcdc_lcd_block2config_tag {
	struct _lcdc_lcd_block2config_map {
		volatile unsigned int reserved			:3;
		volatile unsigned int type		        :1;
		volatile unsigned int colorkey_en 		:1;
		volatile unsigned int alpha_sel 		:1;
		volatile unsigned int resolution        :2;
		volatile unsigned int colorkey			:16;
		volatile unsigned int alpha				:8;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK2CONFIG_U;

typedef union _lcdc_lcd_block3addr_tag {
	struct _lcdc_lcd_block3addr_map {
		volatile unsigned int start_addr		:32;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK3ADDR_U;

typedef union _lcdc_lcd_block3start_tag {
	struct _lcdc_lcd_block3start_map {
		volatile unsigned int enable			:1;
		volatile unsigned int priority			:3;
		volatile unsigned int start_addry		:12;
		volatile unsigned int reserved			:4;
		volatile unsigned int start_addx		:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK3START_U;

typedef union _lcdc_lcd_block3end_tag {
	struct _lcdc_lcd_block3end_map {
		volatile unsigned int width				:8;
		volatile unsigned int end_addry			:12;
		volatile unsigned int end_addrx			:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK3END_U;

typedef union _lcdc_lcd_block3config_tag {
	struct _lcdc_lcd_block3config_map {
		volatile unsigned int reserved			:3;
		volatile unsigned int type		        :1;
		volatile unsigned int colorkey_en 		:1;
		volatile unsigned int alpha_sel 		:1;
		volatile unsigned int resolution        :2;
		volatile unsigned int colorkey			:16;
		volatile unsigned int alpha				:8;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK3CONFIG_U;

typedef union _lcdc_lcd_block4addr_tag {
	struct _lcdc_lcd_block4addr_map {
		volatile unsigned int start_addr		:32;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK4ADDR_U;

typedef union _lcdc_lcd_block4start_tag {
	struct _lcdc_lcd_block4start_map {
		volatile unsigned int enable			:1;
		volatile unsigned int priority			:3;
		volatile unsigned int start_addry		:12;
		volatile unsigned int reserved			:4;
		volatile unsigned int start_addx		:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK4START_U;

typedef union _lcdc_lcd_block4end_tag {
	struct _lcdc_lcd_block4end_map {
		volatile unsigned int width				:8;
		volatile unsigned int end_addry			:12;
		volatile unsigned int end_addrx			:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK4END_U;

typedef union _lcdc_lcd_block4config_tag {
	struct _lcdc_lcd_block4config_map {
		volatile unsigned int reserved			:3;
		volatile unsigned int type		        :1;
		volatile unsigned int colorkey_en 		:1;
		volatile unsigned int alpha_sel 		:1;
		volatile unsigned int resolution        :2;
		volatile unsigned int colorkey			:16;
		volatile unsigned int alpha				:8;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK4CONFIG_U;

typedef union _lcdc_lcd_block5addr_tag {
	struct _lcdc_lcd_block5addr_map {
		volatile unsigned int start_addr		:32;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK5ADDR_U;

typedef union _lcdc_lcd_block5start_tag {
	struct _lcdc_lcd_block5start_map {
		volatile unsigned int enable			:1;
		volatile unsigned int priority			:3;
		volatile unsigned int start_addry		:12;
		volatile unsigned int reserved			:4;
		volatile unsigned int start_addx		:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK5START_U;

typedef union _lcdc_lcd_block5end_tag {
	struct _lcdc_lcd_block5end_map {
		volatile unsigned int width				:8;
		volatile unsigned int end_addry			:12;
		volatile unsigned int end_addrx			:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK5END_U;

typedef union _lcdc_lcd_block5config_tag {
	struct _lcdc_lcd_block5config_map {
		volatile unsigned int reserved			:3;
		volatile unsigned int type		        :1;
		volatile unsigned int colorkey_en 		:1;
		volatile unsigned int alpha_sel 		:1;
		volatile unsigned int resolution        :2;
		volatile unsigned int colorkey			:16;
		volatile unsigned int alpha				:8;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_BLOCK5CONFIG_U;

typedef union _lcdc_lcd_int_enable_tag {
	struct _lcdc_lcd_int_enable_map {
		volatile unsigned int reserved 			:29;//Reserved
		volatile unsigned int start_of_frame	:1;
		volatile unsigned int end_of_frame		:1;
		volatile unsigned int underflow_int		:1;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_INT_ENABLE_U;

typedef union _lcdc_lcd_int_status_tag {
	struct _lcdc_lcd_int_status_map {
		volatile unsigned int reserved 			:29;//Reserved
		volatile unsigned int start_of_frame	:1;
		volatile unsigned int end_of_frame		:1;
		volatile unsigned int underflow_int		:1;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_INT_STATUS_U;

typedef union _lcdc_lcd_int_clear_tag {
	struct _lcdc_lcd_int_clear_map {
		volatile unsigned int reserved 			:29;//Reserved
		volatile unsigned int start_of_frame	:1;
		volatile unsigned int end_of_frame		:1;
		volatile unsigned int underflow_int  	:1;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_INT_CLEAR_U;

typedef union _lcdc_color_coeff_a1_tag {
	struct _lcdc_color_coeff_a1_map {
		volatile unsigned int reserved			:20;//Reserved
		volatile unsigned int a1				:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_COLOR_COEFF_A1_U;

typedef union _lcdc_color_coeff_a2_tag {
	struct _lcdc_color_coeff_a2_map {
		volatile unsigned int reserved			:20;//Reserved
		volatile unsigned int a2				:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_COLOR_COEFF_A2_U;

typedef union _lcdc_color_coeff_a3_tag {
	struct _lcdc_color_coeff_a3_map {
		volatile unsigned int reserved			:20;//Reserved
		volatile unsigned int a3				:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_COLOR_COEFF_A3_U;

typedef union _lcdc_color_coeff_b1_tag {
	struct _lcdc_color_coeff_b1_map {
		volatile unsigned int reserved			:20;//Reserved
		volatile unsigned int b1				:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_COLOR_COEFF_B1_U;

typedef union _lcdc_color_coeff_b2_tag {
	struct _lcdc_color_coeff_b2_map {
		volatile unsigned int reserved			:20;//Reserved
		volatile unsigned int b2				:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_COLOR_COEFF_B2_U;

typedef union _lcdc_color_coeff_b3_tag {
	struct _lcdc_color_coeff_b3_map {
		volatile unsigned int reserved			:20;//Reserved
		volatile unsigned int b3				:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_COLOR_COEFF_B3_U;

typedef union _lcdc_color_coeff_c1_tag {
	struct _lcdc_color_coeff_c1_map {
		volatile unsigned int reserved			:20;//Reserved
		volatile unsigned int c1				:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_COLOR_COEFF_C1_U;

typedef union _lcdc_color_coeff_c2_tag {
	struct _lcdc_color_coeff_c2_map {
		volatile unsigned int reserved			:20;//Reserved
		volatile unsigned int c2				:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_COLOR_COEFF_C2_U;

typedef union _lcdc_color_coeff_c3_tag {
	struct _lcdc_color_coeff_c3_map {
		volatile unsigned int reserved			:20;//Reserved
		volatile unsigned int c3				:12;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_COLOR_COEFF_C3_U;

typedef union _lcdc_lcd_timing0_tag {
	struct _lcdc_lcd_timing0_map {
		volatile unsigned int reserved			:8;//Reserved
		volatile unsigned int vsw				:8;
		volatile unsigned int efw				:8;
		volatile unsigned int bfw				:8;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_TIMING0_U;

typedef union _lcdc_lcd_timing1_tag {
	struct _lcdc_lcd_timing1_map {
		volatile unsigned int reserved			:8;//Reserved
		volatile unsigned int hsw				:8;
		volatile unsigned int elw				:8;
		volatile unsigned int blw				:8;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_TIMING1_U;

typedef union _lcdc_lcd_ctrl_reg_tag {
	struct _lcdc_lcd_ctrl_reg_map {
		volatile unsigned int reserved 			:21;
		volatile unsigned int rgb_lcd_reset		:1;
		volatile unsigned int disable_lcd_out	:1;
		volatile unsigned int feedback_mode		:1;
		volatile unsigned int rb_format			:1;
		volatile unsigned int trans_cycle		:1;
		volatile unsigned int lcd_bpp			:2;
		volatile unsigned int enable_pol		:1;
		volatile unsigned int vsync_pol			:1;
		volatile unsigned int hsync_pol			:1;
		volatile unsigned int scalingup_mode	:1;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCD_CTRL_REG_U;

typedef union _lcdc_tv_control_reg_tag {
	struct _lcdc_tv_control_reg_map {
		volatile unsigned int reserved 			:16;
		volatile unsigned int yuv_delay			:2;
		volatile unsigned int gamma_sel			:1;
		volatile unsigned int lnv_uv			:1;
		volatile unsigned int tv_offset_y		:1;
		volatile unsigned int tv_enc_sw_rest	:1;
		volatile unsigned int tv_enc_dactest	:1;
		volatile unsigned int tv_enc_ninvt		:1;
		volatile unsigned int tv_lowf_mode		:1;
		volatile unsigned int tv_enc_pedt		:1;
		volatile unsigned int tv_enc_svideo		:1;
		volatile unsigned int tv_enc_out_mode	:2;
		volatile unsigned int tv_enc_palmn		:2;
		volatile unsigned int tv_enc_pal		:1;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_TV_CONTROL_REG_U;

typedef union _lcdc_dac_control_reg_tag {
	struct _lcdc_dac_control_reg_map {
		volatile unsigned int resrved_1			:23;//Reserved
		volatile unsigned int clk_ctrl			:1;
		volatile unsigned int clk_flip_ctrl		:1;
		volatile unsigned int reserved_2		:2;
		volatile unsigned int dac_ctrl			:4;
		volatile unsigned int power_down		:1;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_DAC_CONTROL_REG_U;

typedef union _lcdc_lcmparameter0_tag {
	struct _lcdc_lcmparameter0_map {
		volatile unsigned int reserved 			:12;//Reserved
		volatile unsigned int rcss				:2;
		volatile unsigned int rlpw				:4;
		volatile unsigned int rhpw				:4;
		volatile unsigned int wcss				:2;
		volatile unsigned int wlpw				:4;
		volatile unsigned int whpw				:4;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCMPARAMTER0_U;

typedef union _lcdc_lcmparameter1_tag {
	struct _lcdc_lcmparameter1_map {
		volatile unsigned int reserved 			:12;//Reserved
		volatile unsigned int rcss				:2;
		volatile unsigned int rlpw				:4;
		volatile unsigned int rhpw				:4;
		volatile unsigned int wcss				:2;
		volatile unsigned int wlpw				:4;
		volatile unsigned int whpw				:4;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCMPARAMTER1_U;

typedef union _lcdc_lcmrstn_tag {
	struct _lcdc_lcmrstn_map {
		volatile unsigned int reserved 			:31;//Reserved
		volatile unsigned int lcm_rstn			:1;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCMRSTN_U;

typedef union _lcdc_lcmdatanumber_tag {
	struct _lcdc_lcmdatanumber_map {
		volatile unsigned int reserved			:3;//Reserved
		volatile unsigned int selcmcs			:3;
		volatile unsigned int selcmaddr			:6;
		volatile unsigned int data_num			:20;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCMDATANUMBER_U;

typedef union _lcdc_lcmrddata_tag {
	struct _lcdc_lcmrddata_map {
		volatile unsigned int rd_data			:32;
	}mBits;
	volatile unsigned int dValue;
}LCDC_LCMRDDATA_U;

typedef union _lcdc_lcmstatus_tag {
	struct _lcdc_lcmstatus_map {
		volatile unsigned int reserved			:29;//Reserved
		volatile unsigned int fifo_empty		:1;
		volatile unsigned int fifo_full			:1;
		volatile unsigned int lcm_busy			:1;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCMSTATUS_U;

typedef union _lcdc_lcmmodesel_tag {
	struct _lcdc_lcmmodesel_map {
		volatile unsigned int reserved 			:30;//Reserved
		volatile unsigned int modesel_1			:1;
		volatile unsigned int modesel_2			:1;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCMMODESEL_U;

typedef union _lcdc_lcm_rgb_mode_tag {
	struct _lcdc_lcm_rgb_mode_map {
		volatile unsigned int reserved			:28;//Reserved
		volatile unsigned int cs1_rgb_mode		:2;
		volatile unsigned int cs0_rgb_mode		:2;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCM_RGB_MODE_U;

typedef union _lcdc_lcm_selpin_tag {
	struct _lcdc_lcm_selpin_map {
		volatile unsigned int reserved 			:22;//Reserved
		volatile unsigned int selpin			:10;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCM_SELPIN_U;

typedef union _lcdc_lcm_int_status_tag {
	struct _lcdc_lcm_int_status_map {
		volatile unsigned int reserved			:30;//Reserved
		volatile unsigned int fifo_overflow		:1;
		volatile unsigned int osd_done			:1;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCM_INT_STATUS_U;

typedef union _lcdc_lcm_int_en_tag {
	struct _lcdc_lcm_int_en_map {
		volatile unsigned int reserved 			:30;//Reserved
		volatile unsigned int overflow_en		:1;
		volatile unsigned int osd_done_en		:1;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCM_INT_EN_U;

typedef union _lcdc_lcm_int_vector_tag {
	struct _lcdc_lcm_int_vector_map {
		volatile unsigned int reserved			:30;//Reserved
		volatile unsigned int fifooverflow_vec	:1;
		volatile unsigned int osd_done_vec		:1;
	}mBits;
	volatile unsigned int dwValue;
}LCDC_LCM_INT_VECTOR_U;

typedef union _lcm_int_status_tag {
	struct _lcm_int_status_map {
		volatile unsigned int reserved 			:28;//Reserved
		volatile unsigned int fifo_overflow		:1;
        volatile unsigned int int_err  		:1;
		volatile unsigned int osd_done		    :1;
        volatile unsigned int nand_done 		:1;
	}mBits;
	volatile unsigned int dwValue;
}LCM_INT_STATUS_U;

typedef union _lcm_int_en_tag {
	struct _lcm_int_en_map {
		volatile unsigned int reserved 			:28;//Reserved
		volatile unsigned int overflow_en		:1;
        volatile unsigned int int_err_en   		:1;
		volatile unsigned int osd_done_en		:1;
        volatile unsigned int nand_done_en 		:1;
	}mBits;
	volatile unsigned int dwValue;
}LCM_INT_EN_U;

typedef union _lcm_int_vector_tag {
	struct _lcm_int_vector_map {
		volatile unsigned int reserved			:28;//Reserved
		volatile unsigned int fifooverflow_vec	:1;
        volatile unsigned int int_err_vec		:1;
		volatile unsigned int osd_done_vec		:1;
        volatile unsigned int nand_done_vec		:1;
	}mBits;
	volatile unsigned int dwValue;
}LCM_INT_VECTOR_U;


typedef union _lcdc_lcmremain_tag {
	struct _lcdc_lcmremain_map {
		volatile unsigned int reserved			:12;//Reserved
		volatile unsigned int remain			:20;
	}mBits;
	volatile unsigned int dValue;
}LCDC_LCMREMAIN_U;

typedef union _lcdc_lcm_cs0_addr0_tag {
	struct _lcdc_lcm_cs0_addr0_map {
		volatile unsigned int reserved 			:14;//Reserved
		volatile unsigned int addr0				:18;
	}mBits;
	volatile unsigned int wValue;
}LCDC_LCM_CS0_ADDR0_U;

typedef union _lcdc_lcm_cs0_addr1_tag {
	struct _lcdc_lcm_cs0_addr1_map {
		volatile unsigned int reserved 			:14;//Reserved
		volatile unsigned int addr1				:18;
	}mBits;
	volatile unsigned int wValue;
}LCDC_LCM_CS0_ADDR1_U;

typedef union _lcdc_lcm_cs1_addr0_tag {
	struct _lcdc_lcm_cs1_addr0_map {
		volatile unsigned int reserved 			:14;//Reserved
		volatile unsigned int addr0				:18;
	}mBits;
	volatile unsigned int wValue;
}LCDC_LCM_CS1_ADDR0_U;

typedef union _lcdc_lcm_cs1_addr1_tag {
	struct _lcdc_lcm_cs1_addr1_map {
		volatile unsigned int reserved 			:14;//Reserved
		volatile unsigned int addr1				:18;
	}mBits;
	volatile unsigned int wValue;
}LCDC_LCM_CS1_ADDR1_U;


typedef union _lcdc_fifo_threshold_tag {
	struct _lcdc_fifo_threshold_map {
		volatile unsigned int reserved 			:27;//Reserved
		volatile unsigned int addr1				:5;
	}mBits;
	volatile unsigned int wValue;
}LCDC_FIFO_THRESHOLD_U;

typedef union _lcdc_fifo_curr_status_tag {
	struct _lcdc_fifo_curr_status_map {
		volatile unsigned int reserved 			:27;//Reserved
		volatile unsigned int addr1				:5;
	}mBits;
	volatile unsigned int wValue;
}LCDC_FIFO_CURR_STATUS_U;

typedef union _lcdc_fifo_min_status_tag {
	struct _lcdc_fifo_min_status_map {
		volatile unsigned int reserved 			:27;//Reserved
		volatile unsigned int addr1				:5;
	}mBits;
	volatile unsigned int wValue;
}LCDC_FIFO_MIN_STATUS_U;

typedef union _lcdc_disp_win_start_addr_tag {
	struct _lcdc_win_start_addr_map {
		volatile unsigned int reserved2 	    :4;//Reserved
		volatile unsigned int start_y   		:12;
		volatile unsigned int reserved1 	    :4;//Reserved
		volatile unsigned int start_x			:12;
	}mBits;
	volatile unsigned int wValue;
}LCDC_DISP_WIN_START_ADDR_U;

typedef union _lcdc_disp_win_end_addr_tag {
	struct _lcdc_win_end_addr_map {
		volatile unsigned int reserved2 	    :4;//Reserved
		volatile unsigned int end_y   		    :12;
		volatile unsigned int reserved1 	    :4;//Reserved
		volatile unsigned int end_x			    :12;
	}mBits;
	volatile unsigned int wValue;
}LCDC_DISP_WIN_END_ADDR_U;

typedef struct lcdc_dev_reg_tag
{
	volatile LCDC_LCD_MODE_U			lcd_mode;
	volatile LCDC_LCD_SIZE_U			lcd_size;
	volatile LCDC_LCD_STATUS_U			lcd_status;
	volatile LCDC_LCD_BACKGROUND_U		lcd_background;
	volatile LCDC_LCD_BLOCK0ADDR_U		blk0_addr;
	volatile LCDC_LCD_BLOCK0START_U		blk0_start;
	volatile LCDC_LCD_BLOCK0END_U		blk0_end;
	volatile LCDC_LCD_BLOCK0CONFIG_U	blk0_config;
	volatile LCDC_LCD_BLOCK1ADDR_U		blk1_addr;
	volatile LCDC_LCD_BLOCK1START_U		blk1_start;
	volatile LCDC_LCD_BLOCK1END_U		blk1_end;
	volatile LCDC_LCD_BLOCK1CONFIG_U	blk1_config;
	volatile LCDC_LCD_BLOCK2ADDR_U		blk2_addr;
	volatile LCDC_LCD_BLOCK2START_U		blk2_start;
	volatile LCDC_LCD_BLOCK2END_U		blk2_end;
	volatile LCDC_LCD_BLOCK2CONFIG_U	blk2_config;
	volatile LCDC_LCD_BLOCK3ADDR_U		blk3_addr;
	volatile LCDC_LCD_BLOCK3START_U		blk3_start;
	volatile LCDC_LCD_BLOCK3END_U		blk3_end;
	volatile LCDC_LCD_BLOCK3CONFIG_U	blk3_vonfig;
	volatile LCDC_LCD_BLOCK4ADDR_U		blk4_addr;
	volatile LCDC_LCD_BLOCK4START_U		blk4_start;
	volatile LCDC_LCD_BLOCK4END_U		blk4_end;
	volatile LCDC_LCD_BLOCK4CONFIG_U	blk4_config;
	volatile LCDC_LCD_BLOCK5ADDR_U		blk5_addr;
	volatile LCDC_LCD_BLOCK5START_U		blk5_start;
	volatile LCDC_LCD_BLOCK5END_U		blk5_end;
	volatile LCDC_LCD_BLOCK5CONFIG_U	blk5_config;

	volatile LCDC_COLOR_COEFF_A1_U      coeff_a1;
	volatile LCDC_COLOR_COEFF_A2_U		coeff_a2;
	volatile LCDC_COLOR_COEFF_A3_U		coeff_a3;
	volatile LCDC_COLOR_COEFF_B1_U		coeff_b1;
	volatile LCDC_COLOR_COEFF_B2_U		coeff_b2;
	volatile LCDC_COLOR_COEFF_B3_U		coeff_b3;
	volatile LCDC_COLOR_COEFF_C1_U		coeff_c1;
	volatile LCDC_COLOR_COEFF_C2_U		coeff_c2;
	volatile LCDC_COLOR_COEFF_C3_U		coeff_c3;
	
    volatile LCDC_LCD_INT_ENABLE_U      int_en;
	volatile LCDC_LCD_INT_STATUS_U		int_status;
	volatile LCDC_LCD_INT_CLEAR_U		int_clear;
    volatile LCDC_FIFO_THRESHOLD_U      fifo_threshold;
    volatile LCDC_FIFO_CURR_STATUS_U    fifo_curr_status;
    volatile LCDC_FIFO_MIN_STATUS_U     fifo_min_status;
    volatile LCDC_DISP_WIN_START_ADDR_U disp_win_start;
    volatile LCDC_DISP_WIN_END_ADDR_U   disp_win_end;
	volatile LCDC_LCD_TIMING0_U  		timing0;
	volatile LCDC_LCD_TIMING1_U			timing1;
	volatile LCDC_LCD_CTRL_REG_U        lcd_ctrl;
	volatile LCDC_TV_CONTROL_REG_U      tv_ctrl;
	volatile LCDC_DAC_CONTROL_REG_U     dac_ctrl;
	volatile LCDC_LCMPARAMTER0_U  		para0;
	volatile LCDC_LCMPARAMTER1_U		para1;
	volatile LCDC_LCMRSTN_U				lcm_rst;
	volatile LCDC_LCMDATANUMBER_U		data_sum;
	volatile LCDC_LCMRDDATA_U           read_data;
	volatile LCDC_LCMSTATUS_U           lcm_status;
	volatile LCDC_LCMMODESEL_U          lcm_mode;
	volatile LCDC_LCM_RGB_MODE_U        rgb_mode;
	volatile LCDC_LCM_SELPIN_U          sel_pin;
	volatile LCDC_LCM_INT_STATUS_U      lcm_int_status;
	volatile LCDC_LCM_INT_EN_U          lcm_int_en;
	volatile LCDC_LCM_INT_VECTOR_U      lcm_int_vec;
	volatile LCDC_LCMREMAIN_U			lcm_remain;
	volatile LCDC_LCM_CS0_ADDR0_U       cs0_addr0;
	volatile LCDC_LCM_CS0_ADDR1_U       cs0_addr1;
	volatile LCDC_LCM_CS1_ADDR0_U		cs1_addr0;
	volatile LCDC_LCM_CS1_ADDR1_U       cs1_addr1;
}LCDC_DEV_REG_T;


//LCD Interface Control.
typedef struct lcd_tag
{
    VOLATILE uint32 data;
    VOLATILE uint32 clk;
    VOLATILE uint32 cnt;
    VOLATILE uint32 sts;
    VOLATILE uint32 iclr;
}lcd_s;


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 



