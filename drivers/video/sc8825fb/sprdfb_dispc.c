/******************************************************************************
 ** File Name:    sprdfb_dispc.c                                            *
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

#include <asm/arch/tiger_lcd.h>
#include <asm/arch/dispc_reg.h>
#include <asm/arch/sc8810_reg_ahb.h>
#include <asm/arch/sc8810_reg_global.h>

#include <asm/arch/regs_global.h>

#include "sprdfb.h"

#define DISPC_SOFT_RST (20)


static uint16_t		is_first_frame = 1;

extern void sprdfb_panel_before_refresh(struct sprdfb_device *dev);
extern void sprdfb_panel_after_refresh(struct sprdfb_device *dev);
extern void sprdfb_panel_invalidate(struct panel_spec *self);
extern void sprdfb_panel_invalidate_rect(struct panel_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom);

static void __raw_bits_and(unsigned int v, unsigned int a)
{
	__raw_writel((__raw_readl(a) & v), a);
}

static void __raw_bits_or(unsigned int v, unsigned int a)
{
	__raw_writel((__raw_readl(a) | v), a);
}

/* dispc soft reset */
static void dispc_reset(void)
{
	FB_PRINT("sprdfb:[%s]\n",__FUNCTION__);

	__raw_writel(__raw_readl(AHB_SOFT_RST) | (1<<DISPC_SOFT_RST), AHB_SOFT_RST);
	udelay(10);
	__raw_writel(__raw_readl(AHB_SOFT_RST) & (~(1<<DISPC_SOFT_RST)), AHB_SOFT_RST);
}

static inline void dispc_set_bg_color(uint32_t bg_color)
{
	FB_PRINT("sprdfb:[%s]\n",__FUNCTION__);

	dispc_write(bg_color, DISPC_BG_COLOR);
}

static inline void dispc_set_osd_ck(uint32_t ck_color)
{
	FB_PRINT("sprdfb:[%s]\n",__FUNCTION__);

	dispc_write(ck_color, DISPC_OSD_CK);
}

static void dispc_dithering_enable(uint16_t enable)
{
	FB_PRINT("sprdfb:[%s]\n",__FUNCTION__);

	if(enable){
		dispc_set_bits((1<<6), DISPC_CTRL);
	}else{
		dispc_clear_bits((1<<6), DISPC_CTRL);
	}
}

static void dispc_set_exp_mode(uint16_t exp_mode)
{
	uint32_t reg_val = dispc_read(DISPC_CTRL);

	FB_PRINT("sprdfb:[%s]\n",__FUNCTION__);

	reg_val &= ~(0x3 << 16);
	reg_val |= (exp_mode << 16);
	dispc_write(reg_val, DISPC_CTRL);
}

static void dispc_module_enable(void)
{
	FB_PRINT("sprdfb:[%s]\n",__FUNCTION__);

	/*dispc module enable */
	dispc_write((1<<0), DISPC_CTRL);

	/*disable dispc INT*/
	dispc_write(0x0, DISPC_INT_EN);

	/* clear dispc INT */
	dispc_write(0x1F, DISPC_INT_CLR);
}

static inline int32_t  dispc_set_disp_size(struct sprdfb_device *dev)
{
	uint32_t reg_val;

	FB_PRINT("sprdfb:[%s]\n",__FUNCTION__);

	reg_val = (dev->panel->width& 0xfff) | ((dev->panel->height & 0xfff ) << 16);
	dispc_write(reg_val, DISPC_SIZE_XY);

	return 0;
}

static void dispc_layer_init(struct sprdfb_device *dev)
{
	uint32_t reg_val = 0;

	FB_PRINT("sprdfb:[%s]\n",__FUNCTION__);

	dispc_clear_bits((1<<0),DISPC_IMG_CTRL);
	dispc_clear_bits((1<<0),DISPC_OSD_CTRL);

	/******************* OSD layer setting **********************/

	/*enable OSD layer*/
	reg_val |= (1 << 0);

	/*disable  color key */

	/* alpha mode select  - block alpha*/
	reg_val |= (1 << 2);

	/* data format */
	/* RGB565 */
	reg_val |= (5 << 4);
	/* B2B3B0B1 */
	reg_val |= (2 << 8);

	dispc_write(reg_val, DISPC_OSD_CTRL);

	/* OSD layer alpha value */
	dispc_write(0xff, DISPC_OSD_ALPHA);

	/* OSD layer size */
	reg_val = ( dev->panel->width& 0xfff) | ((dev->panel->height & 0xfff ) << 16);
	dispc_write(reg_val, DISPC_OSD_SIZE_XY);

	/* OSD layer start position */
	dispc_write(0, DISPC_OSD_DISP_XY);

	/* OSD layer pitch */
	reg_val = ( dev->panel->width & 0xfff) ;
	dispc_write(reg_val, DISPC_OSD_PITCH);

	/*OSD base address*/
	dispc_write(dev->smem_start, DISPC_OSD_BASE_ADDR);

	/* OSD color_key value */
	dispc_set_osd_ck(0x0);

	/* DISPC workplane size */
	dispc_set_disp_size(dev);

	FB_PRINT("DISPC_OSD_CTRL: 0x%x\n", dispc_read(DISPC_OSD_CTRL));
	FB_PRINT("DISPC_OSD_ALPHA: 0x%x\n", dispc_read(DISPC_OSD_ALPHA));
	FB_PRINT("DISPC_OSD_SIZE_XY: 0x%x\n", dispc_read(DISPC_OSD_SIZE_XY));
	FB_PRINT("DISPC_OSD_DISP_XY: 0x%x\n", dispc_read(DISPC_OSD_DISP_XY));
	FB_PRINT("DISPC_OSD_PITCH: 0x%x\n", dispc_read(DISPC_OSD_PITCH));
	FB_PRINT("DISPC_OSD_BASE_ADDR: 0x%x\n", dispc_read(DISPC_OSD_BASE_ADDR));
}

static int32_t sprdfb_dispc_early_init(struct sprdfb_device *dev)
{
	FB_PRINT("sprdfb:[%s]\n", __FUNCTION__);

	//select DISPC clock source
	__raw_bits_and(~(1<<1), AHB_DISPC_CLK);    //pll_src=256M
	__raw_bits_and(~(1<<2), AHB_DISPC_CLK);

	//set DISPC divdior
	__raw_bits_and(~(1<<3), AHB_DISPC_CLK);  //div=0
	__raw_bits_and(~(1<<4), AHB_DISPC_CLK);
	__raw_bits_and(~(1<<5), AHB_DISPC_CLK);

	//select DBI clock source
	__raw_bits_and(~(1<<9), AHB_DISPC_CLK);    //pll_src=256M
	__raw_bits_and(~(1<<10), AHB_DISPC_CLK);

	//set DBI divdior
	__raw_bits_and(~(1<<11), AHB_DISPC_CLK);  //div=0
	__raw_bits_and(~(1<<12), AHB_DISPC_CLK);
	__raw_bits_and(~(1<<13), AHB_DISPC_CLK);

	//select DPI clock source
	__raw_bits_and(~(1<<17), AHB_DISPC_CLK);    //pll_src=384M
	__raw_bits_and(~(1<<18), AHB_DISPC_CLK);

	//set DPI divdior
	__raw_bits_and(~(1<<19), AHB_DISPC_CLK);  //div=10, dpi_clk = pll_src/(10+1)
	__raw_bits_or((1<<20), AHB_DISPC_CLK);
	__raw_bits_and(~(1<<21), AHB_DISPC_CLK);
	__raw_bits_or((1<<22), AHB_DISPC_CLK);
	__raw_bits_and(~(1<<23), AHB_DISPC_CLK);
	__raw_bits_and(~(1<<24), AHB_DISPC_CLK);
	__raw_bits_and(~(1<<25), AHB_DISPC_CLK);
	__raw_bits_and(~(1<<26), AHB_DISPC_CLK);

	//enable dispc matric clock
	__raw_bits_or((1<<9), AHB_CTL2);  //core_clock_en
	__raw_bits_or((1<<11), AHB_CTL2);  //matrix clock en

	//enable DISPC clock
	__raw_bits_or(1<<22, AHB_CTL0);

	printf("0x20900200 = 0x%x\n", __raw_readl(0x20900200));
	printf("0x20900208 = 0x%x\n", __raw_readl(0x20900208));
	printf("0x20900220 = 0x%x\n", __raw_readl(0x20900220));

	dispc_reset();
	dispc_module_enable();
	is_first_frame = 1;

	return 0;
}


static int32_t sprdfb_dispc_init(struct sprdfb_device *dev)
{
	FB_PRINT("sprdfb:[%s]\n",__FUNCTION__);

	/*set bg color*/
	dispc_set_bg_color(0xFFFFFFFF);
	/*enable dithering*/
	dispc_dithering_enable(1);
	/*use MSBs as img exp mode*/
	dispc_set_exp_mode(0x0);

	dispc_layer_init(dev);

	if(SPRDFB_PANEL_IF_DPI == dev->panel_if_type){
		if(is_first_frame){
			/*set dpi register update only with SW*/
			dispc_set_bits((1<<4), DISPC_DPI_CTRL);
		}else{
			/*set dpi register update with SW & VSYNC*/
			dispc_clear_bits((1<<4), DISPC_DPI_CTRL);
		}
		/*enable dispc update done INT*/
//		dispc_write((1<<4), DISPC_INT_EN);
	}else{
		/* enable dispc DONE  INT*/
//		dispc_write((1<<0), DISPC_INT_EN);
	}
	return 0;
}

static int32_t sprdfb_dispc_uninit(struct sprdfb_device *dev)
{
	FB_PRINT("sprdfb:[%s]\n",__FUNCTION__);

	//disable DISPC clock
	__raw_bits_and(~(1<<22), AHB_CTL0);
	return 0;
}

static int32_t sprdfb_dispc_refresh (struct sprdfb_device *dev)
{
	FB_PRINT("sprdfb:[%s]\n",__FUNCTION__);
	uint32_t i;

	if(SPRDFB_PANEL_IF_DPI != dev->panel_if_type){
		sprdfb_panel_invalidate(dev->panel);
	}

	sprdfb_panel_before_refresh(dev);

	if(SPRDFB_PANEL_IF_DPI == dev->panel_if_type){
		//dispc_write(0x0, DISPC_OSD_CTRL);
		//dispc_write(0x0, DISPC_BG_COLOR);
		/*dpi register update*/
		dispc_set_bits((1<<5), DISPC_DPI_CTRL);
		if(is_first_frame){
			udelay(30);
			/*dpi register update with SW and VSync*/
			dispc_clear_bits((1<<4), DISPC_DPI_CTRL);
			/* start refresh */
			dispc_set_bits((1 << 4), DISPC_CTRL);
			is_first_frame = 0;
		}else{
			for(i=0;i<1000;i++){
				if(!(dispc_read(DISPC_INT_RAW) & (0x10))){
					udelay(100);
				}else{
					break;
				}
			}
			if(i >= 1000){
				FB_PRINT("sprdfb:[%s] wait dispc update  int time out!! (0x%x)\n", __FUNCTION__, dispc_read(DISPC_INT_RAW));
			}else{
				FB_PRINT("sprdfb:[%s] got dispc update int (0x%x)\n", __FUNCTION__, dispc_read(DISPC_INT_RAW));
			}
			dispc_set_bits((1<<5), DISPC_INT_CLR);
                   }
	}else{
		/* start refresh */
		dispc_set_bits((1 << 4), DISPC_CTRL);
		for(i=0;i<500;i++){
			if(0x1 != (dispc_read(DISPC_INT_RAW) & (1<<0))){
				udelay(1000);
			}else{
				break;
			}
		}
		if(i >= 1000){
			FB_PRINT("sprdfb:[%s] wait dispc done int time out!! (0x%x)\n", __FUNCTION__, dispc_read(DISPC_INT_RAW));
		}else{
			FB_PRINT("sprdfb:[%s] got dispc done int (0x%x)\n", __FUNCTION__, dispc_read(DISPC_INT_RAW));
		}
		dispc_set_bits((1<<0), DISPC_INT_CLR);
	}

	sprdfb_panel_after_refresh(dev);

	return 0;
}

struct display_ctrl sprdfb_dispc_ctrl = {
	.name		= "dispc",
	.early_init		= sprdfb_dispc_early_init,
	.init		 	= sprdfb_dispc_init,
	.uninit		= sprdfb_dispc_uninit,
	.refresh		= sprdfb_dispc_refresh,
};

