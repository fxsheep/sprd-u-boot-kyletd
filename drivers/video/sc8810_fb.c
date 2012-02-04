/*
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
#include <lcd.h>
#include <asm/io.h>
#include <asm/errno.h>

#include <asm/arch/sc8810_reg_base.h>
#include <asm/arch/sc8810_lcd.h>
#include <asm/arch/lcdc_reg_v3.h>
#include <asm/arch/lcm_reg_v3.h>

#include <asm/arch/mfp.h>
#include <asm/arch/adi_hal_internal.h>
#include <asm/arch/regs_ana.h>
#include <asm/arch/analog_reg_v3.h>

#include <asm/arch/sc8810_reg_ahb.h>
#include <asm/arch/sc8810_reg_global.h>

#include <asm/arch/gpio_drvapi.h>

#include <asm/arch/regs_global.h>
#include <asm/arch/regs_cpc.h>

#define mdelay(a) udelay(a * 1000)
#define printk printf

DECLARE_GLOBAL_DATA_PTR;

void *lcd_base;		/* Start of framebuffer memory	*/
void *lcd_console_address;	/* Start of console buffer	*/

int lcd_line_length;
int lcd_color_fg;
int lcd_color_bg;

short console_col;
short console_row;

//#define TEST_RRM /* enable rrm test */
#define  FB_DEBUG
#ifdef FB_DEBUG
#define FB_PRINT printk
#else
#define FB_PRINT(...)
#endif

#define BIT20 (1<<20)
#define SPRD_GREG_BASE       GREG_BASE

#define GR_PLL_SRC           (SPRD_GREG_BASE + 0x70)
#define BITS_PER_PIXEL 16

#define LCDC_CYCLES(t, ahb, div)  ((t) * (ahb) + (div) - 1) / (div)
#define MAX_LCDC_TIMING_VALUE 15

struct sc8810fb_info {
	uint32_t cap;
	struct ops_mcu *ops;
	struct lcd_spec *panel;
	uint32_t smem_start;
	uint32_t smem_len;

	uint32_t register_timing;
	uint32_t gram_timing;
};

struct lcd_cfg{
	uint32_t lcd_id;
	struct lcd_spec* panel;
};


#ifdef CONFIG_SC8810_OPENPHONE

extern struct lcd_spec lcd_panel_hx8357;
static struct lcd_cfg lcd_panel[] = {
	[0]={
		.lcd_id = 0x57,
		.panel = &lcd_panel_hx8357,
		},
};

#else

extern struct lcd_spec lcd_panel_hx8369;
static struct lcd_cfg lcd_panel[] = {
	[0]={
		.lcd_id = 0x69,
		.panel = &lcd_panel_hx8369,
		},
};
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
	while(__raw_readl(LCM_CTRL) & BIT20);

	__raw_writel(cmd, LCM_CD0);

	return 0;
}

static int32_t lcm_send_cmd_data (uint32_t cmd, uint32_t data)
{
	/* busy wait for ahb fifo full sign's disappearance */
	while(__raw_readl(LCM_CTRL) & BIT20);

	__raw_writel(cmd, LCM_CD0);

	/* busy wait for ahb fifo full sign's disappearance */
	while(__raw_readl(LCM_CTRL) & BIT20);

	__raw_writel(data, LCM_DATA0);

	return 0;
}

static int32_t lcm_send_data (uint32_t data)
{
	/* busy wait for ahb fifo full sign's disappearance */
	 while(__raw_readl(LCM_CTRL) & BIT20);

	__raw_writel(data, LCM_DATA0);

	return 0;
}

static uint32_t lcm_read_data (void)
{
	/* busy wait for ahb fifo full sign's disappearance */
	while(__raw_readl(LCM_CTRL) & BIT20);
	__raw_writel(1 << 24, LCM_DATA0);

	udelay(50);
	while(__raw_readl(LCM_CTRL) & BIT20);

	return __raw_readl(LCM_RDDATA);
}

static struct ops_mcu lcm_mcu_ops = {
	.send_cmd = lcm_send_cmd,
	.send_cmd_data = lcm_send_cmd_data,
	.send_data = lcm_send_data,
	.read_data = lcm_read_data,
};

static int32_t panel_reset()
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

static void lcdc_mcu_init(void)
{
	//uint32_t reg_val = 0;

	//panel reset
	panel_reset();

	//LCDC module enable
	__raw_bits_or(1<<0, LCDC_CTRL);

	/*FMARK mode*/
	__raw_bits_or(1<<1, LCDC_CTRL);

	/*FMARK pol*/
	__raw_bits_and(~(1<<2), LCDC_CTRL);

	FB_PRINT("[%s] LCDC_CTRL: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_CTRL));

	/* set background*/
	__raw_writel(0xffffff, LCDC_BG_COLOR);   //red

	FB_PRINT("[%s] LCDC_BG_COLOR: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_BG_COLOR));

	/* dithering enable*/
	//__raw_bits_or(1<<4, LCDC_CTRL);

}

static uint32_t lcdc_calc_lcm_timing(struct timing_mcu *timing)
{
	uint32_t ahb_clk;
	uint32_t rcss, rlpw, rhpw, wcss, wlpw, whpw;

	// for sc8810
	ahb_clk = 250; // 250 MHz

	FB_PRINT("@fool2[%s] ahb_clk: 0x%x\n", __FUNCTION__, ahb_clk);

        /************************************************
	* we assume : t = ? ns, AHB = ? MHz   so
        *      1ns  cycle  :  AHB /1000
	*      tns  cycles :  t * AHB / 1000
	*
	*****************************************/
	rcss = LCDC_CYCLES(timing->rcss, ahb_clk, 1000);//ceiling

        if (rcss > MAX_LCDC_TIMING_VALUE) {
		rcss = MAX_LCDC_TIMING_VALUE ; // max 15 cycles
	}

	rlpw = LCDC_CYCLES(timing->rlpw, ahb_clk , 1000);
	if (rlpw > MAX_LCDC_TIMING_VALUE) {
		rlpw = MAX_LCDC_TIMING_VALUE ;
	}

	rhpw = LCDC_CYCLES(timing->rhpw, ahb_clk , 1000);
	if (rhpw > MAX_LCDC_TIMING_VALUE) {
		rhpw = MAX_LCDC_TIMING_VALUE ;
	}

	wcss = LCDC_CYCLES(timing->wcss, ahb_clk, 1000);
	if (wcss > MAX_LCDC_TIMING_VALUE) {
		wcss = MAX_LCDC_TIMING_VALUE ;
	}

	wlpw = LCDC_CYCLES(timing->wlpw, ahb_clk, 1000);
	if (wlpw > MAX_LCDC_TIMING_VALUE) {
		wlpw = MAX_LCDC_TIMING_VALUE ;
	}

	whpw = LCDC_CYCLES(timing->whpw, ahb_clk, 1000);
	if (whpw > MAX_LCDC_TIMING_VALUE) {
		whpw = MAX_LCDC_TIMING_VALUE ;
	}

	/*   LCDC_ChangePulseWidth() */
	return whpw | (wlpw << 4) | (wcss << 8)
                        | (rhpw << 16) |(rlpw << 20) | (rcss << 24);

}

static void lcdc_update_lcm_timing(uint32_t value) 
{
	/* LCD_UpdateTiming() */

	__raw_writel(value, LCM_PARAMETER0); /* FIXME: hardcoded for !CS0 */

	FB_PRINT("@fool2[%s] LCM_PARAMETER0: 0x%x\n", __FUNCTION__, __raw_readl(LCM_PARAMETER0));
}

static int mount_panel(struct sc8810fb_info *fb, struct lcd_spec *panel)
{
	fb->panel = panel;

	panel->info.mcu->ops = fb->ops;

	panel->ops->lcd_reset = panel_reset;

	{
		struct timing_mcu *timing = panel->info.mcu->timing;
		fb->register_timing = lcdc_calc_lcm_timing(timing);
		timing++;
		fb->gram_timing = lcdc_calc_lcm_timing(timing);  
	}
	return 0;
}

static void real_set_layer(struct sc8810fb_info *fb)
{
	uint32_t reg_val;

	/* image layer base */
	reg_val = fb->smem_start;
	__raw_writel(reg_val, LCDC_OSD1_BASE_ADDR);
}

static void real_refresh(struct sc8810fb_info *fb)
{
	fb->panel->ops->lcd_invalidate(fb->panel);
	/* set timing parameters for LCD */
	lcdc_update_lcm_timing(fb->gram_timing);

	__raw_bits_or((1<<3), LCDC_CTRL); /* start refresh */
	
	while(__raw_readl(LCDC_IRQ_RAW) & (1<<0)); // wait util done

	__raw_bits_or((1<<0), LCDC_IRQ_CLR);

	/* set timing parameters for LCD */
	lcdc_update_lcm_timing(fb->register_timing);
}

static void lcdc_lcm_configure(struct sc8810fb_info *fb)
{
	uint32_t reg_val = 0;;
	/* CS0 bus mode [BIT0]: 8080/6800 */
	switch (fb->panel->info.mcu->bus_mode) {
	case LCD_BUS_8080:

		break;
	case LCD_BUS_6800:
		reg_val  |= 1;
		break;
	default:
		break;
	}
	/* CS0 bus width [BIT1:0] */
	switch (fb->panel->info.mcu->bus_width) {
	case 8:
		break;
	case 9:
		reg_val  |= ((1 << 1) | (1 << 4));
		break;
	case 16:
		reg_val  |= (2 << 1);
		break;
	case 18:
		reg_val  |= ((3 << 1) | (1 << 4));
		break;
	case 24:
		reg_val  |= ((4 << 1) | (2 << 4));
		break;
	default:
		break;

	}
	__raw_writel(reg_val, LCM_CTRL);

	FB_PRINT("@fool2[%s] LCM_CTRL: 0x%x\n", __FUNCTION__, __raw_readl(LCM_CTRL));
}



static inline int set_lcdsize(struct lcd_spec *panel)
{
	uint32_t reg_val;

	reg_val = ( panel->width & 0xfff) | (( panel->height & 0xfff )<<16);
	__raw_writel(reg_val, LCDC_DISP_SIZE);

	FB_PRINT("[%s] LCDC_DISP_SIZE: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_DISP_SIZE));

	return 0;
}
static inline int set_lcmrect( struct lcd_spec *panel)
{
	uint32_t reg_val;

	__raw_writel(0, LCDC_LCM_START);

	reg_val = ( panel->width & 0xfff) | (( panel->height & 0xfff )<<16);
	__raw_writel(reg_val, LCDC_LCM_SIZE);

	FB_PRINT("[%s] LCDC_LCM_START: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_LCM_START));
	FB_PRINT("[%s] LCDC_LCM_SIZE: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_LCM_SIZE));

	return 0;
}

int set_lcdc_layers(struct sc8810fb_info *fb)
{
	uint32_t reg_val = 0;
	/******************* OSD1 layer setting **********************/
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
	//__raw_bits_or((1<<0),LCDC_OSD1_CTRL);
	reg_val |= (1 << 0);

	/*color key */
	//__raw_bits_and(~(1<<1),LCDC_OSD1_CTRL);  //disable

	/*alpha mode select*/
	//__raw_bits_or((1<<2),LCDC_OSD1_CTRL);  //block alpha
	reg_val |= (1 << 2);

	reg_val |= (5 << 3); //RGB565
	reg_val |= (2 << 7); //B2B3B0B1

	/*data endian*/
	//__raw_bits_or(1<<8,LCDC_OSD1_CTRL);  //little endian
	//__raw_bits_and(~(1<<7),LCDC_OSD1_CTRL);

	/*alpha endian*/
	/*
	__raw_bits_or(1<<10,LCDC_IMG_CTRL);
	__raw_bits_and(~(1<<9),LCDC_IMG_CTRL);
	*/
	__raw_writel(reg_val, LCDC_OSD1_CTRL);

	FB_PRINT("[%s] LCDC_OSD1_CTRL: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_CTRL));

	/* OSD1 layer base */
	reg_val = fb->smem_start;
	__raw_writel(reg_val, LCDC_OSD1_BASE_ADDR);

	//FB_PRINT("@fool2[%s] LCDC_OSD1_BASE_ADDR: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_BASE_ADDR));

	/*OSD1 layer alpha value*/
	__raw_writel(0xff, LCDC_OSD1_ALPHA);

	FB_PRINT("[%s] LCDC_OSD1_ALPHA: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_ALPHA));

	/*alpha base addr*/
	//__raw_writel(reg_val, LCDC_OSD1_ALPHA_BASE_ADDR);

	/*OSD1 layer size*/
	reg_val = ( fb->panel->width & 0xfff) | (( fb->panel->height & 0xfff )<<16);
	__raw_writel(reg_val, LCDC_OSD1_SIZE_XY);

	FB_PRINT("[%s] LCDC_OSD1_SIZE_XY: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_SIZE_XY));

	/*OSD1 layer start position*/
	__raw_writel(0, LCDC_OSD1_DISP_XY);

	FB_PRINT("[%s] LCDC_OSD1_DISP_XY: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_DISP_XY));

	/*OSD1 layer pitch*/
	reg_val = ( fb->panel->width & 0xfff) ;
	__raw_writel(reg_val, LCDC_OSD1_PITCH);

	FB_PRINT("[%s] LCDC_OSD1_PITCH: 0x%x\n", __FUNCTION__, __raw_readl(LCDC_OSD1_PITCH));

	/*LCDC workplane size*/
	set_lcdsize(fb->panel);

	/*LCDC LCM rect size*/
	set_lcmrect(fb->panel);

	return 0;
}

static void hw_early_init(struct sc8810fb_info *fb)
{
	//select LCD clock source
	__raw_bits_and(~(1<<6), GR_PLL_SRC);    //pll_src=96M
	__raw_bits_and(~(1<<7), GR_PLL_SRC);

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

	__raw_bits_and(~(1<<0), LCDC_IRQ_EN);
	__raw_bits_or((1<<0), LCDC_IRQ_CLR);

	/* init lcdc mcu mode using default configuration */
	lcdc_mcu_init();
}

static void hw_init(struct sc8810fb_info *fb)
{
	/* only MCU mode is supported currently */
	if (LCD_MODE_RGB == fb->panel->mode)
		return;

	//panel reset
	panel_reset(fb->panel);

	/* set lcdc-lcd interface parameters */
	lcdc_lcm_configure(fb);

	/* set timing parameters for LCD */
	lcdc_update_lcm_timing(fb->register_timing);

}

static void hw_later_init(struct sc8810fb_info *fb)
{
	/* init mounted lcd panel */
	fb->panel->ops->lcd_init(fb->panel);

	set_lcdc_layers(fb);
}

#define WHTLED_CTL              ANA_LED_CTL
#define WHTLED_PD_SET           BIT_0
#define WHTLED_PD_RST           BIT_1
#define WHTLED_V_SHIFT          2
#define WHTLED_V_MSK            (0x1F << WHTLED_V_SHIFT)

static void LCD_SetPwmRatio(unsigned short value)
{
	__raw_bits_or(CLK_PWM0_EN, GR_CLK_EN);
	__raw_bits_or(CLK_PWM0_SEL, GR_CLK_EN);
	__raw_bits_or(PIN_PWM0_MOD_VALUE, CPC_LCD_PWM_REG);
	__raw_writel(LCD_PWM_PRESCALE_VALUE, SPRD_PWM0_PRESCALE);
	__raw_writel(value, SPRD_PWM0_CNT);
	__raw_writel(PWM_REG_MSK_VALUE, SPRD_PWM0_PAT_LOW);
	__raw_writel(PWM_REG_MSK_VALUE, SPRD_PWM0_PAT_HIG);

	__raw_bits_or(LCD_PWM0_EN, SPRD_PWM0_PRESCALE);
}

void LCD_SetBackLightBrightness( unsigned long  value)
{
	unsigned long duty_mod= 0;
	if(value > LCD_PWM_MOD_VALUE)
		value = LCD_PWM_MOD_VALUE;

	if(value < 0)
		value = 0;

	duty_mod = (value << 8) | LCD_PWM_MOD_VALUE;
	LCD_SetPwmRatio(duty_mod);
}


void set_backlight(uint32_t value)
{
#ifdef CONFIG_SC8810_OPENPHONE
	ANA_REG_AND(WHTLED_CTL, ~(WHTLED_PD_SET | WHTLED_PD_RST));
	ANA_REG_OR(WHTLED_CTL,  WHTLED_PD_RST);
	ANA_REG_MSK_OR (WHTLED_CTL, ( (value << WHTLED_V_SHIFT) &WHTLED_V_MSK), WHTLED_V_MSK);

#else
	//if (gpio_request(143, "LCD_BL")) {
	//	FB_PRINT("Failed ro request LCD_BL GPIO_%d \n",
	//		143);
	//	return -ENODEV;
	//}
	//gpio_direction_output(143, 1);
	//gpio_set_value(143, 1);
	//__raw_bits_or((1<<5),  0x8B000008);
	//__raw_bits_or((1<<15), 0x8A000384);
	//__raw_bits_or((1<<15), 0x8A000388);
	//__raw_bits_or((1<<15), 0x8A000380);
	LCD_SetBackLightBrightness(value);

#endif
}

static struct sc8810fb_info sc8810fb = {0};

static uint32_t lcd_id_to_kernel = 0;

void save_lcd_id_to_kernel(uint32_t id)
{
	lcd_id_to_kernel = id;
}

uint32_t load_lcd_id_to_kernel()
{
	return lcd_id_to_kernel;
}

static int lcd_readid_default(struct lcd_spec *self)
{
	uint32_t dummy;
	//default id reg is 0
	self->info.mcu->ops->send_cmd(0x0);

	if(self->info.mcu->bus_width == 8)	{
		dummy = (self->info.mcu->ops->read_data())&0xff;
		dummy <<= 8;
		dummy |= (self->info.mcu->ops->read_data())&0xff;
	} else {
		dummy = (self->info.mcu->ops->read_data());
	}
	return dummy;
}


static int find_adapt_from_readid(struct sc8810fb_info *fb)
{
	int i;
	uint32_t id;
	for(i = 0;i<(sizeof(lcd_panel))/(sizeof(lcd_panel[0]));i++) {
		//first ,try mount
		mount_panel(fb,lcd_panel[i].panel);
		//hw init to every panel
		hw_init(fb);
		//readid
		if(fb->panel->ops->lcd_readid) {
			id = fb->panel->ops->lcd_readid(fb->panel);
		} else {
			id = lcd_readid_default(fb->panel);
		}
		//if the id is right?
		if(id == lcd_panel[i].lcd_id) {
			save_lcd_id_to_kernel(id);
			return i;
		}
	}
	return -1;
}


static int sc8810fb_probe(void * lcdbase)
{
	int32_t ret;
	int lcd_adapt;
	struct sc8810fb_info *fb= &sc8810fb;

	FB_PRINT("[%s]\n", __FUNCTION__);

	fb->ops = &lcm_mcu_ops;
	//we maybe readid ,so hardware should be init
	hw_early_init(fb);

	lcd_adapt = find_adapt_from_readid(fb);

	if(lcd_adapt == -1) {
		lcd_adapt = 0;
	}

	ret = mount_panel(fb, lcd_panel[lcd_adapt].panel);
	if (ret) {
		printk("unsupported panel!!");
		return -EFAULT;
	}

	fb->smem_start = (uint32_t)lcdbase;
	fb->smem_len = fb->panel->width * fb->panel->height;

	hw_init(fb);
	hw_later_init(fb);

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
     sc8810fb_probe(lcdbase);
}

void lcd_display(void)
{
    real_refresh(&sc8810fb);
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


