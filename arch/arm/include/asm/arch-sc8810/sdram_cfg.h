/******************************************************************************
 ** File Name:    sdram_cfg.h                                                 *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         6/25/2006                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 6/25/2006      Daniel.Ding     Create.                                    *
 ******************************************************************************/
#ifndef _SDRAM_CFG_H_
#define _SDRAM_CFG_H_

#include "sci_types.h"
#include "sdram_drvapi.h"

struct sc8810_ddr_reset_para {
	char id[5];

	// clock
	u32 emc_clk;

	// driver strength
	u8 dqs_drv;
	u8 dat_drv;
	u8 ctl_drv;
	u8 clk_drv;

	// clk wr
	u8 clk_wr;
	u16 read_value;  // value from 0x20000174
};

struct sc8810_ddr_reset_data {
struct sc8810_ddr_reset_para *ddr_para;
int 						 size;
};
#endif /* SDRAM_CFG_H */
