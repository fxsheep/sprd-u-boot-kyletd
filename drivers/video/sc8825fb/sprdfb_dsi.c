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
#include <common.h>

#include <asm/arch/tiger_lcd.h>

#include <asm/io.h>
#include <asm/arch/sc8810_reg_ahb.h>
#include <asm/arch/sc8810_reg_base.h>


#include "sprdfb.h"

#include "mipi_dsih_local.h"
#include "mipi_dsih_dphy.h"
#include "mipi_dsih_hal.h"
#include "mipi_dsih_api.h"

#define DSI_SOFT_RST (26)
#define DSI_PHY_REF_CLOCK (26*1000)

#define DSI_EDPI_CFG (0x6c)


struct sprdfb_dsi_context {
	uint16_t			is_inited;

	dsih_ctrl_t	dsi_inst;
};

static struct sprdfb_dsi_context dsi_ctx;

static uint32_t dsi_core_read_function(uint32_t addr, uint32_t offset)
{
	return __raw_readl(addr + offset);
}

static void dsi_core_write_function(uint32_t addr, uint32_t offset, uint32_t data)
{
	__raw_writel(data, addr + offset);
}


static void dsi_reset(void)
{
	__raw_writel(__raw_readl(AHB_SOFT_RST) | (1<<DSI_SOFT_RST), AHB_SOFT_RST);
	udelay(10);
	__raw_writel(__raw_readl(AHB_SOFT_RST) & (~(1<<DSI_SOFT_RST)),AHB_SOFT_RST);
}

int32_t dsi_early_int(void)
{
	FB_PRINT("sprdfb:[%s]\n", __FUNCTION__);

	if(dsi_ctx.is_inited){
		FB_PRINT("sprdfb: dispc early init warning!(has been inited)");
		return 0;
	}

	//TODO:Enable DSI clock

	dsi_reset();

/*	memset(&(dsi_ctx.dsi_inst), 0, sizeof(dsi_ctx.dsi_inst));*/

	dsi_ctx.is_inited = 1;
	return 0;
}

static int32_t dsi_edpi_setbuswidth(struct info_mipi * mipi)
{
	dsih_color_coding_t color_coding;

	switch(mipi->video_bus_width){
	case 16:
		color_coding = COLOR_CODE_16BIT_CONFIG1;
		break;
	case 18:
		color_coding = COLOR_CODE_18BIT_CONFIG1;
		break;
	case 24:
		color_coding = COLOR_CODE_24BIT;
		break;
	default:
		FB_PRINT("sprdfb:[%s] fail, invalid video_bus_width\n", __FUNCTION__);
		break;
	}

	dsi_core_write_function(DSI_CTL_BEGIN,  R_DSI_HOST_DPI_CFG, (uint32_t)color_coding);
	return 0;
}


static int32_t dsi_edpi_init(void)
{
	dsi_core_write_function((uint32_t)DSI_CTL_BEGIN,  (uint32_t)DSI_EDPI_CFG, 0x10500);
	return 0;
}

static int32_t dsi_dpi_init(struct panel_spec* panel)
{
	dsih_dpi_video_t dpi_param;
	dsih_error_t result;
	struct info_mipi * mipi = panel->info.mipi;

	dpi_param.no_of_lanes = mipi->lan_number;
	dpi_param.byte_clock = mipi->phy_feq / 8;
	dpi_param.pixel_clock = DSI_PHY_REF_CLOCK / 4;

	switch(mipi->video_bus_width){
	case 16:
		dpi_param.color_coding = COLOR_CODE_16BIT_CONFIG1;
		break;
	case 18:
		dpi_param.color_coding = COLOR_CODE_18BIT_CONFIG1;
		break;
	case 24:
		dpi_param.color_coding = COLOR_CODE_24BIT;
		break;
	default:
		FB_PRINT("sprdfb:[%s] fail, invalid video_bus_width\n", __FUNCTION__);
		break;
	}

	if(SPRDFB_POLARITY_POS == mipi ->h_sync_pol){
		dpi_param.h_polarity = 1;
	}

	if(SPRDFB_POLARITY_POS == mipi ->v_sync_pol){
		dpi_param.v_polarity = 1;
	}

	if(SPRDFB_POLARITY_POS == mipi ->de_pol){
		dpi_param.data_en_polarity = 1;
	}

	dpi_param.h_active_pixels = panel->width;
	dpi_param.h_sync_pixels = mipi->timing->hsync;
	dpi_param.h_back_porch_pixels = mipi->timing->hbp;
	dpi_param.h_total_pixels = panel->width + mipi->timing->hsync + mipi->timing->hbp + mipi->timing->hfp;

	dpi_param.v_active_lines = panel->height;
	dpi_param.v_sync_lines = mipi->timing->vsync;
	dpi_param.v_back_porch_lines = mipi->timing->vbp;
	dpi_param.v_total_lines = panel->height + mipi->timing->vsync + mipi->timing->vbp + mipi->timing->vfp;

	dpi_param.receive_ack_packets = 0;
	dpi_param.video_mode = VIDEO_BURST_WITH_SYNC_PULSES;
	dpi_param.virtual_channel = 0;
	dpi_param.is_18_loosely = 0;

	result = mipi_dsih_dpi_video(&(dsi_ctx.dsi_inst), &dpi_param);
	if(result != OK){
		FB_PRINT("sprdfb: [%s] mipi_dsih_dpi_video fail (%d)!\n", __FUNCTION__, result);
		return -1;
	}

	return 0;
}

static void dsi_log_error(const char * string)
{
	FB_PRINT(string);
}


int32_t sprdfb_dsi_init(struct sprdfb_device *dev)
{
	dsih_error_t result = OK;
	dsih_ctrl_t* dsi_instance = &(dsi_ctx.dsi_inst);
	dphy_t *phy = &(dsi_instance->phy_instance);
	struct info_mipi * mipi = dev->panel->info.mipi;

	FB_PRINT("sprdfb:[%s]\n", __FUNCTION__);

	dsi_early_int();

	phy->address = DSI_CTL_BEGIN;
	phy->core_read_function = dsi_core_read_function;
	phy->core_write_function = dsi_core_write_function;
	phy->log_error = dsi_log_error;
	phy->log_info = NULL;
	phy->reference_freq = DSI_PHY_REF_CLOCK;

	dsi_instance->address = DSI_CTL_BEGIN;
	dsi_instance->color_mode_polarity =mipi->color_mode_pol;
	dsi_instance->shut_down_polarity = mipi->shut_down_pol;
	dsi_instance->core_read_function = dsi_core_read_function;
	dsi_instance->core_write_function = dsi_core_write_function;
	dsi_instance->log_error = dsi_log_error;
	dsi_instance->log_info = NULL;
	dsi_instance->max_bta_cycles = 0;//10;
	dsi_instance->max_hs_to_lp_cycles = 4;//110;
	dsi_instance->max_lp_to_hs_cycles = 15;//10;
	dsi_instance->max_lanes = mipi->lan_number;

	if(SPRDFB_MIPI_MODE_CMD == mipi->work_mode){
		dsi_edpi_init();
	}/*else{
		dsi_dpi_init(dev->panel);
	}*/

/*
	result = mipi_dsih_unregister_all_events(dsi_instance);
	if(OK != result){
		FB_PRINT("sprdfb: [%s]: mipi_dsih_unregister_all_events fail (%d)!\n", __FUNCTION__, result);
		return -1;
	}
*/
	dsi_core_write_function(DSI_CTL_BEGIN,  R_DSI_HOST_ERROR_MSK0, 0x1fffff);
	dsi_core_write_function(DSI_CTL_BEGIN,  R_DSI_HOST_ERROR_MSK1, 0x3ffff);

	result = mipi_dsih_open(dsi_instance);
	if(OK != result){
		FB_PRINT("sprdfb: [%s]: mipi_dsih_open fail (%d)!\n", __FUNCTION__, result);
		return -1;
	}

	result = mipi_dsih_dphy_configure(phy,  mipi->lan_number, mipi->phy_feq);
	if(OK != result){
		FB_PRINT("sprdfb: [%s]: mipi_dsih_dphy_configure fail (%d)!\n", __FUNCTION__, result);
		return -1;
	}

	while(5 != (dsi_core_read_function(DSI_CTL_BEGIN, R_DSI_HOST_PHY_STATUS) & 5));

	if(SPRDFB_MIPI_MODE_CMD == mipi->work_mode){
		dsi_edpi_setbuswidth(mipi);
	}

	result = mipi_dsih_enable_rx(dsi_instance, 1);
	if(OK != result){
		FB_PRINT("sprdfb: [%s]: mipi_dsih_enable_rx fail (%d)!\n", __FUNCTION__, result);
		return -1;
	}

	result = mipi_dsih_ecc_rx(dsi_instance, 1);
	if(OK != result){
		FB_PRINT("sprdfb: [%s]: mipi_dsih_ecc_rx fail (%d)!\n", __FUNCTION__, result);
		return -1;
	}

	result = mipi_dsih_eotp_rx(dsi_instance, 1);
	if(OK != result){
		FB_PRINT("sprdfb: [%s]: mipi_dsih_eotp_rx fail (%d)!\n", __FUNCTION__, result);
		return -1;
	}

	result = mipi_dsih_eotp_tx(dsi_instance, 1);
	if(OK != result){
		FB_PRINT("sprdfb: [%s]: mipi_dsih_eotp_tx fail (%d)!\n", __FUNCTION__, result);
		return -1;
	}

	if(SPRDFB_MIPI_MODE_VIDEO == mipi->work_mode){
		dsi_dpi_init(dev->panel);
	}

	return 0;
}

int32_t sprdfb_dsi_uninit(struct sprdfb_device *dev)
{
	dsih_error_t result;
	result = mipi_dsih_close(&(dsi_ctx.dsi_inst));
	if(OK != result){
		FB_PRINT("sprdfb: [%s]: sprdfb_dsi_uninit fail (%d)!\n", __FUNCTION__, result);
		return -1;
	}
	return 0;
}

int32_t sprdfb_dsi_ready(struct sprdfb_device *dev)
{
	struct info_mipi * mipi = dev->panel->info.mipi;

	if(SPRDFB_MIPI_MODE_CMD == mipi->work_mode){
		mipi_dsih_cmd_mode(&(dsi_ctx.dsi_inst), 1);
		dsi_core_write_function(DSI_CTL_BEGIN, R_DSI_HOST_CMD_MODE_CFG, 0x1);
		dsi_core_write_function(DSI_CTL_BEGIN, R_DSI_HOST_PHY_IF_CTRL, 0x1);
	}else{
		mipi_dsih_video_mode(&(dsi_ctx.dsi_inst), 1);
		dsi_core_write_function(DSI_CTL_BEGIN, R_DSI_HOST_PWR_UP, 0);
		udelay(100);
		dsi_core_write_function(DSI_CTL_BEGIN, R_DSI_HOST_PWR_UP, 1);
		udelay(10*1000);
	}
	return 0;
}

static int32_t sprdfb_dsi_set_cmd_mode(void)
{
	mipi_dsih_cmd_mode(&(dsi_ctx.dsi_inst), 1);
	return 0;
}

static int32_t sprdfb_dsi_set_video_mode(void)
{
	mipi_dsih_video_mode(&(dsi_ctx.dsi_inst), 1);
	return 0;
}

static int32_t sprdfb_dsi_gen_write(uint8_t *param, uint16_t param_length)
{
	dsih_error_t result;
	result = mipi_dsih_gen_wr_cmd(&(dsi_ctx.dsi_inst), 0, param, param_length);
	if(OK != result){
		FB_PRINT("sprdfb: [%s] error (%d)\n", __FUNCTION__, result);
		return -1;
	}
	return 0;
}

static int32_t sprdfb_dsi_gen_read(uint8_t *param, uint16_t param_length, uint8_t bytes_to_read, uint8_t *read_buffer)
{
	uint16_t result;
	result = mipi_dsih_gen_rd_cmd(&(dsi_ctx.dsi_inst), 0, param, param_length, bytes_to_read, read_buffer);
	if(0 != result){
		FB_PRINT("sprdfb: [%s] error (%d)\n", __FUNCTION__, result);
		return -1;
	}
	return 0;
}

static int32_t sprdfb_dsi_dcs_write(uint8_t *param, uint16_t param_length)
{
	dsih_error_t result;
	result = mipi_dsih_dcs_wr_cmd(&(dsi_ctx.dsi_inst), 0, param, param_length);
	if(OK != result){
		FB_PRINT("sprdfb: [%s] error (%d)\n", __FUNCTION__, result);
		return -1;
	}
	return 0;
}

static int32_t sprdfb_dsi_dcs_read(uint8_t command, uint8_t bytes_to_read, uint8_t *read_buffer)
{
	uint16_t result;
	mipi_dsih_dcs_rd_cmd(&(dsi_ctx.dsi_inst), 0, command, bytes_to_read, read_buffer);
	if(0 != result){
		FB_PRINT("sprdfb: [%s] error (%d)\n", __FUNCTION__, result);
		return -1;
	}
	return 0;
}



struct ops_mipi sprdfb_mipi_ops = {
	.mipi_set_cmd_mode = sprdfb_dsi_set_cmd_mode,
	.mipi_set_video_mode = sprdfb_dsi_set_video_mode,
	.mipi_gen_write = sprdfb_dsi_gen_write,
	.mipi_gen_read = sprdfb_dsi_gen_read,
	.mipi_dcs_write = sprdfb_dsi_dcs_write,
	.mipi_dcs_read = sprdfb_dsi_dcs_read,
};



