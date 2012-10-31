/******************************************************************************
 ** File Name:    sprdfb_main.h                                            *
 ** Author:                                                           *
 ** DATE:                                                           *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                            *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 **
 ******************************************************************************/

#include <common.h>
#include <asm/io.h>
#include <asm/errno.h>

#include <asm/arch/tiger_lcd.h>

#include <asm/arch/sc8810_reg_global.h>

#include <asm/arch/regs_global.h>
#include <asm/arch/regs_cpc.h>

#include "sprdfb.h"


void *lcd_base;		/* Start of framebuffer memory	*/
void *lcd_console_address;	/* Start of console buffer	*/

int lcd_line_length;
int lcd_color_fg;
int lcd_color_bg;

short console_col;
short console_row;



extern int sprdfb_panel_probe(struct sprdfb_device *dev);
extern void sprdfb_panel_remove(struct sprdfb_device *dev);

extern struct display_ctrl sprdfb_dispc_ctrl ;

static struct sprdfb_device tigerdev = {0};

static uint32_t lcd_id_to_kernel = 0;


#define WHTLED_CTL              ANA_LED_CTL
#define WHTLED_PD_SET           BIT_0
#define WHTLED_PD_RST           BIT_1
#define WHTLED_V_SHIFT          2
#define WHTLED_V_MSK            (0x1F << WHTLED_V_SHIFT)

static void __raw_bits_and(unsigned int v, unsigned int a)
{
	__raw_writel((__raw_readl(a) & v), a);
}

static void __raw_bits_or(unsigned int v, unsigned int a)
{
	__raw_writel((__raw_readl(a) | v), a);
}

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
#if defined (CONFIG_SP8825) || defined (CONFIG_SP8825EA) || defined (CONFIG_SP8825EB)
 	__raw_writel(0x101, 0x4C000138);
	__raw_bits_or((1<<5), 0x4B000008);
	__raw_bits_or((1<<8), 0x4A000384);
	__raw_bits_or((1<<8), 0x4A000388);
	__raw_bits_or((1<<8), 0x4A000380);
#endif


#ifdef CONFIG_SC8810_OPENPHONE
	ANA_REG_AND(WHTLED_CTL, ~(WHTLED_PD_SET | WHTLED_PD_RST));
	ANA_REG_OR(WHTLED_CTL,  WHTLED_PD_RST);
	ANA_REG_MSK_OR (WHTLED_CTL, ( (value << WHTLED_V_SHIFT) &WHTLED_V_MSK), WHTLED_V_MSK);
#elif CONFIG_MACH_CORI
	__raw_bits_or((1<<5),  0x8B000008);
	__raw_bits_or((1<<10), 0x8A000384);
	__raw_bits_or((1<<10), 0x8A000388);
	__raw_bits_or((1<<10), 0x8A000380);
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
#ifndef CONFIG_SP8810EA
	LCD_SetBackLightBrightness(value);
#else
	__raw_writel(0x101, 0x8C0003e0);
	__raw_bits_or((1<<5),  0x8B000008);
	__raw_bits_or((1<<15), 0x8A000384);
	__raw_bits_or((1<<15), 0x8A000388);
	__raw_bits_or((1<<15), 0x8A000380);
#endif

#endif
}

void save_lcd_id_to_kernel(uint32_t id)
{
	lcd_id_to_kernel = id;
}

uint32_t load_lcd_id_to_kernel(void)
{
	return lcd_id_to_kernel;
}


static int real_refresh(struct sprdfb_device *dev)
{
	int32_t ret;

	FB_PRINT("sprdfb: [%s]\n", __FUNCTION__);

	if(NULL == dev->panel){
		FB_PRINT("sprdfb: [%s] fail (no panel!)\n", __FUNCTION__);
		return -1;
	}

	ret = dev->ctrl->refresh(dev);
	if (ret) {
		FB_PRINT("sprdfb: failed to refresh !!!!\n");
		return -1;
	}

	return 0;
}
static int tiger_probe(void * lcdbase)
{
	struct sprdfb_device *dev = &tigerdev;

	FB_PRINT("sprdfb:[%s]\n", __FUNCTION__);

#ifdef CONFIG_MACH_CORI
	LDO_SetVoltLevel(LDO_LDO_SIM3, LDO_VOLT_LEVEL1);
	LDO_TurnOnLDO(LDO_LDO_SIM3);
	LDO_SetVoltLevel(LDO_LDO_VDD28, LDO_VOLT_LEVEL3);
	LDO_TurnOnLDO(LDO_LDO_VDD28);
#endif
/*
	__raw_writel((__raw_readl(0x20900208) | 0xAFE), 0x20900208);
	__raw_writel((__raw_readl(0x20900200) | 0xFFFFFFFF), 0x20900200);
	__raw_writel((__raw_readl(0x20900220) | 0x00500000), 0x20900220);
*/

	dev->ctrl = &sprdfb_dispc_ctrl;
	dev->ctrl->early_init(dev);

	if (0 != sprdfb_panel_probe(dev)) {
		sprdfb_panel_remove(dev);
		dev->ctrl->uninit(dev);
		FB_PRINT("sprdfb: failed to probe\n");
		return -EFAULT;
	}

	dev->smem_start = (uint32_t)lcdbase;

	dev->ctrl->init(dev);
	return 0;
}


void lcd_initcolregs(void)
{
	FB_PRINT("sprdfb:[%s]\n", __FUNCTION__);
}

void lcd_disable(void)
{
	FB_PRINT("sprdfb:[%s]\n", __FUNCTION__);
	sprdfb_panel_remove(&tigerdev);
	tigerdev.ctrl->uninit(&tigerdev);
}


/* References in this function refer to respective Linux kernel sources */
void lcd_enable(void)
{
	FB_PRINT("sprdfb:[%s]\n", __FUNCTION__);
}

void lcd_ctrl_init(void *lcdbase)
{
	FB_PRINT("sprdfb:[%s]\n", __FUNCTION__);
	tiger_probe(lcdbase);
}

void lcd_display(void)
{
	FB_PRINT("sprdfb:[%s]\n", __FUNCTION__);
	real_refresh(&tigerdev);
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

