/******************************************************************************
 ** File Name:    sc8810_module_ver_extern.h                                            *
 ** Author:       Tim.Luo                                                    *
 ** DATE:         07/08/2010                                                  *
 ** Copyright:    2010  Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 07/08/2010    Tim.Luo         Create.                                     *
 ******************************************************************************/
#ifndef _SC8810_MODULE_VER_EXTERN_H_
#define _SC8810_MODULE_VER_EXTERN_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "sc8810_reg_ahb.h"
//#include "sc8800g_reg_global.h"


//Chip All module register miscs definitions
//#include "adc_reg_v3.h"
//#include "adi_reg_v3.h"
//#include "analog_reg_v3.h"
//#include "audio_reg_v3.h"
//#include "busmonitor_reg_v3.h"
//#include "..\..\chip_module\charge\v3\charge_reg_v3.h"
//#include "dma_reg_v3.h"
#include "emc_reg_v3.h"
//#include "gea_reg_v3.h"
//#include "gpio_reg_v3.h"
//#include "i2c_reg_v0.h"
#include "int_reg_v3.h"
//#include "kpd_reg_v0.h"
#include "lcdc_reg_v3.h"
//#include "lcm_reg_v3.h"
//#include "ldo_reg_v3.h"
//#include "..\..\chip_module\mmu\v3\mmu_reg_v3.h"
//#include "..\..\chip_module\nand\v0\nand_reg_v0.h"
//#include "pcm_reg_v3.h"
#include "pin_reg_v3.h"
//#include "pwm_reg_v1.h"
//#include "rotation_reg_v0.h"
//#include "rtc_reg_v3.h"
//#include "sdio_reg_v3.h"
//#include "sim_reg_v3.h"
//#include "spi_reg_v0.h"
#include "sys_timer_reg_v0.h"
//#include "timer_reg_v3.h"
//#include "tpc_reg_v3.h"
//#include "uart_reg_v3.h"
#include "usb20_reg_v3.h"
#include "watchdog_reg_v3.h"



/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
////PLL MN
//#define MPLL_MN_192M_EX13M          0xC0000D
//#define MPLL_MN_192M_EX26M          0xC0001A
//#define MPLL_MN_160M_EX13M          0xA0000D
//#define MPLL_MN_160M_EX26M          0xA0001A
//
////UPLL MN
//#define UPLL_MN_192M_EX13M          0xC0000D
//#define UPLL_MN_192M_EX26M          0xC0001A
//#define UPLL_MN_96M_EX13M           0x60000D
//#define UPLL_MN_96M_EX26M           0x60001A
//
////ARM AHB DIV
//#define P192M_ARM192M_AHB96M        0x0100
//#define P192M_ARM96M_AHB96M         0x0101
//#define P160M_ARM160M_AHB80M        0x0100
//#define P160M_ARM80M_AHB80M     0x0101

/*----------Chip Pin Control Register----------*/
//PIN_CTL_BASE  0x8C000000

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
