/******************************************************************************
 ** File Name:      sc8800h_reg.h                                             *
 ** Author:         Daniel.ding                                               *
 ** DATE:           11/14/2005                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                 Register address map for the sc6600d chip                 *
 **                 Reference to the SC6600D control register document        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07-03-2003     Jimmy.Jia	    Create.                                   *
 ** 11-13-2005     Daniel.ding	    Modify it for SC6800                      *
 ** 01-24-2007     Aiguo.Miao       Port to SC8800H                           *
 ******************************************************************************/
#ifndef _SC8800H_REG_H_
#define _SC8800H_REG_H_

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

#if 0
#define PLL_MN_CLK_P144M			0x48000d
#define PCTL_CLK_P52M_D52M_A13M     0x0003
#define PCTL_CLK_P52M_D52M_A52M   	0x0000
#define PCTL_CLK_P78M_D39M_A13M   	0x0015
#define PCTL_CLK_P144M_D72M_A72M   	0x0011
#define PCTL_CLK_P78M_D39M_A39M     0x0011

//The corresponding bit of LCD_CNT register.
#define LCDCNT_BM_SERIAL_S8             0
#define LCDCNT_BM_SERIAL_I2C            1
#define LCDCNT_BM_8B_PARALLEL_8080      2
#define LCDCNT_BM_8B_PARALLEL_6800      3
#define LCDCNT_CB_MODE_EN               (1 << 4)        //if "1", enable MCU buffer mode. In this mode, MCU can continue send 8 commands to LCD.
#define LCDCNT_INF_RST                  (1 << 5)        //lcd interface reset, "1" reset lcd interface, "0" normal mode.

//The corresponding bit of LCD_ICLR register.
#define LCDICLR_INT_PIN                 1               //Interrupt pin is high when LCD interface is not busy. MCU can write bit0 of register LCDINTCLR to clear interrupt pin when no data need to be sent. After MCU send new command to LCD, the interrupt pin is automatically set high by LCD interface.
#define LCDICLR_CB_WR_DOWN              (1 << 1)        //In cb_mode, if MCU send   command number smaller than 8, MCU can write this bit to enable LCD interface execute LCD command.
#define LCDICLR_CB_RD_DOWN              (1 << 2)        //In cb_mode, after LCD interface execute MCU command, if MCU read back data number smaller than 8, MCU can write this bit to enable LCD interface receive new MCU command.
#endif

#define MPLL_MN_200M_EX26M          0xC8000D
#define MPLL_MN_190M_EX26M          0xBE000D
#define MPLL_MN_180M_EX26M          0xB4000D
#define MPLL_MN_170M_EX26M          0xAA000D
#define MPLL_MN_160M_EX26M          0xA0000D
#define MPLL_MN_164M_EX26M          0xA4000D
#define P340M_ARM170M_AHB85M   	     ( (1) | ((3)<<5) | ((1)<<10) | ((1)<<15))
#define P340M_ARM85M_AHB85M   	       ( (3) | ((3)<<5) | ((1)<<10) )

/*----------memory map address----------*/
#define EXT_MEM_START                   0x00000000      //External Memory (4 CHIP Select).
#define EXT_MEM_END                     0x0FFFFFFF

#define DSP_REG_START       		    0x10000000      //Dsp side Control registers.
#define DSP_REG_END         		    0x1FFFFFFF

#define AHB_REG_START                   0x20000000      //AHB Space
#define AHB_REG_END                     0x2FFFFFFF

#define INT_ROM_START                   0x30000000      //Internal rom
#define INT_ROM_END                     0x3FFFFFFF

#define INT_RAM_START                   0x40000000      //Internal ram
#define INT_RAM_END                     0x4000A7FF

#define NFC_START                       0x60000000      //NFC & LCM
#define NFC_END                         0x6FFFFFFF

#define EXT_MEM1_START                  0x70000000
#define EXT_MEM1_END                    0x7FFFFFFF

#define APB_REG_START                   0x80000000      //APB Space
#define APB_REG_END                     0x8FFFFFFF

#define ITCM_START                      0x90000000      //instruction TCM
#define ITCM_END                        0x9FFFFFFF

#define DTCM_START                      0xA0000000      //DATA TCM
#define DTCM_END                        0xAFFFFFFF

#define RESERVED1_START                 0xB0000000
#define RESERVED1_END                   0xBFFFFFFF

#define EXT_MEM2_START                  0xC0000000
#define EXT_MEM2_END                    0xCFFFFFFF

#define RESERVED2_START                 0xD0000000      //??
#define RESERVED2_END                   0xDFFFFFFF

#define EXT_MEM3_START                  0xE0000000
#define EXT_MEM3_END                    0xEFFFFFFF

#define RESERVED3_START                 0xF0000000
#define RESERVED3_END                   0xFFFFFFFF


#define 	SHARE_MEM_BEGIN 			 0x01C00000       //Internal Shared Memory.
#define 	SHARE_MEM_LEN				 0x00400000	   
#define         SHARE_MEM_END                     0x01FFFFFF	   

#define 	INTER_RAM_BEGIN 			 0x40000000	    //Internal ram
#define 	INTER_RAM_END  		 		 0x4000A7FF 		//The address of the last byte

#define 	DSP_MEM_BEGIN   			 0x01E00000       //DSP memory base address.
#define 	DSP_MEM_LEN     			 0x0  		//length unit:byte.

#define 	EXTERNAL_MEM_CTL_BEGIN  	 0x20000000      //External Memory Control registers.
#define 	EXTERNAL_MEM_CTL_END 		 0x200000FC

#define 	DMA_GEN_CTL_BEGIN 			 0x20100000  //DMA General control registers
#define 	DMA_GEN_CTL_END 			 0x201000B4

#define 	DMA_CHA_CTL_BEGIN 			 0x20100400 //DMA Channel control registers
#define 	DMA_CHA_CTL_END 			 0x201007FC
                     	                                		
#define 	ISP_TOP_MODULE_CTL_BEGIN 	 0x20200000  //ISP top module control register
#define 	ISP_TOP_MODULE_CTL_END 		 0x20200098

#define 	ISP_CAM_INTERFACE_CTL_BEGIN  0x20200100  //ISP Camera Interface Control Registers
#define 	ISP_CAM_INTERFACE_CTL_END 	 0x20200118

#define 	ISP_COLOR_PROCESS_CTL_BEGIN  0x20200200  //ISP Color Process control register
#define 	ISP_COLOR_PROCESS_CTL_END	 0x202002B0

#define 	ISP_SCAL_PROCESS_CTL_BEGIN	 0x20200300  //ISP Scaling Process control register
#define 	ISP_SCAL_PROCESS_CTL_END	 0x202003FC

#define 	ROTATION_CTL_BEGIN 			 0x20800200      //Rotation control registers.
#define 	ROTATION_CTL_END 			 0x2080022C

#define 	USB_CTL_BEGIN 				 0x20300000      //USB Device Space.
#define 	USB_CTL_END 				 0x2030019C      

#define 	BUS_MON_CTL_BEGIN 			 0x20900000     //Bus Monitor Control registers.
#define 	BUS_MON_CTL_END 			 0x20900038

#define 	AHB_GEN_CTL_BEGIN 			 0x20900200     //AHB General Control Registers
#define 	AHB_GEN_CTL_END 			 0x2090028C

#define 	CHIP_ID_BEGIN 				 0x209003FC     //Chip ID Register
#define 	CHIP_ID_END 				 0x209003FC

#define 	NAND_LCM_CTL_BEGIN 			 0x60001c00      //NAND Flash and LCM Control Registers
#define 	NAND_LCM_CTL_END 			 0x60001d44

#define 	LCDC_CTL_BEGIN 				 0x20700000      //LCDC Control Registers
#define 	LCDC_CTL_END 				 0x207002b0

#define 	LCDC_LCM_CTL_BEGIN 			 0x20600000      //LCDC/LCM Control Registers
#define 	LCDC_LCM_CTL_END 			 0x20600030

#define 	INT_CTL_BEGIN 				 0x20A00000      //Interrupt Control Registers
#define 	INT_CTL_END 				 0x20A000BC

#define 	DRM_CTL_BEGIN 				 0x20B00000      //DRM Control Registers
#define 	DRM_CTL_END 				 0x20B000BC

#define 	TIMER_CNT_BEGIN 			 0x81000000      //tiemr counter Registers
#define 	TIMER_CNT_END 				 0x8100002C

#define 	UART0_CTL_BEGIN 			 0x83000000     //UART0SPI0 Control Registers
#define 	UART0_CTL_END 				 0x8300002C

#define 	UART1_CTL_BEGIN 			 0x84000000     //UART1SPI1 Control Registers
#define 	UART1_CTL_END 				 0x8400002C

#define 	UART2_CTL_BEGIN 			 0x8E000000    //UART2SPI2 Control Registers
#define 	UART2_CTL_END 				 0x8E000038

#define 	UART3_CTL_BEGIN 			 0x8F000000    //UART3SPI3 Control Registers
#define 	UART3_CTL_END 				 0x8F000038

#define 	SIM0_CTL_BEGIN 				 0x85000000      //SIMCARD0 Control Registers
#define 	SIM0_CTL_END 				 0x85000038

#define 	SIM1_CTL_BEGIN 				 0x85003000      //SIMCARD1 Control Registers
#define 	SIM1_CTL_END 				 0x85003038

#define 	PCMIA_CTL_BEGIN 			 0x80000200      //PCMIA Control Registers
#define 	PCMIA_CTL_END 				 0x80000208

#define 	I2C_CTL_BEGIN 				 0x86000000      //I2C Control Registers
#define 	I2C_CTL_END 				 0x8600001C

#define 	KEYPAD_CTL_BEGIN 			 0x87000000   //Keypad Control Registers
#define 	KEYPAD_CTL_END 				 0x87000030

#define 	SYS_CNT_BEGIN 				 0x8700001C      //system counter Registers
#define 	SYS_CNT_END 				 0x87000024

#define 	PWM_CTL_BEGIN 				 0x88000000   //pwm Control Registers
#define 	PWM_CTL_END 				 0x880000B0

#define 	RTC_CTL_BEGIN 				 0x89000000   //RTC Control Registers
#define 	RTC_CTL_END 				 0x8900003C

#define 	WATDOG_CTL_BEGIN 			 0x89003000   //watchdog Control Registers
#define 	WATDOG_CTL_END 				 0x89003C00

#define 	GPIO_CTL_BEGIN 				 0x8A000000   //GPIO Control Registers
#define 	GPIO_CTL_END 				 0x8A000548

#define 	GLOBAL_CTL_BEGIN 			 0x8B000000   //GLOBAL Control Registers
#define 	GLOBAL_CTL_END 				 0x8B000080

#define 	CHIPPIN_CTL_BEGIN 			 0x8C000000   //ChipPin Control Registers
#define 	CHIPPIN_CTL_END 			 0x8C000368

#define 	EPT_GEA_CTL_BEGIN 			 0x8D000000   //EPT/GEA Control Registers
#define 	EPT_GEA_CTL_END 			 0x8D000CA4

#define 	SDIO_CTL_BEGIN 				 0x20500000   //SDIO Control Registers
#define 	SDIO_CTL_END 				 0x205000FC

#define 	DCT_IDCT_CTL_BEGIN 			 0x20400000   //DCT/IDCT Control Registers
#define 	DCT_IDCT_CTL_END 			 0x20400028

#define 	MEA_CTL_BEGIN 				 0x20C00100    //MEA Control Registers
#define 	MEA_CTL_END 				 0x20C00140

#define 	VOICE_BAND_CODEC_BEGIN	 	 0x82003000    //Voice Band Codec register
#define 	VOICE_BAND_CODEC_END	 	 0x820032FC


#if 0
    //AHB Module REG base addr
    #define EXT_MEM_CTL_BASE            0x20000000
    #define DMA_REG_BASE                0x20100000
    #define ISP_REG_BASE                0x20200000
    #define USB_REG_BASE                0x20300000
    #define	DCT_CTRL_BASE		        0x20400000
    #define SDIO0_BASE_ADDR             0x20500000
    #define SDIO1_BASE_ADDR             0x20500100
    #define LCDC_MCU_REG_BASE			0x20600000
    #define LCDC_REG_BASE				0x20700000
    #define ROTATION_REG_BASE           0x20800000
    #define AHB_REG_BASE                0x20900200
    #define INT_REG_BASE            	0x20a00000
    #define DRM_REG_BASE                0x20b00000
    #define	MEA_CTRL_BASE		        0x20c00100
#endif

#if 0
    //APB Module REG base addr
    #define PCMCIA_BASE                 0x80000000
    #define TIMER_CTL_BASE              0x81000000
    #define RESERVED                    0x82000000
    #define ARM_VBC_BASE                0x82003000
    #define ARM_UART0_BASE              0x83000000
    #define ARM_UART1_BASE              0x84000000
    #define SIM_BASE                    0x85000000
    #define SIM1_BASE                   0x85003000
    #define I2C_BASE                    0x86000000
    #define KPD_BASE                    0x87000000
    #define PWM_BASE                    0x88000000
    #define RTC_BASE                    0x89000000
    #define WDG_BASE                    0x89003000
    #define GPIO_BASE                   0x8A000000
    #define GLB_BASE                    0x8B000000
    #define PIN_CTL_BASE                0x8C000000
    #define EPT_BASE                    0x8D000000
    #define GEA_BASE                    0x8D000000
    #define ARM_UART2_BASE              0x8E000000
    #define ARM_UART3_BASE              0x8F000000
    #define PCM_BASE                    0x8F000000
#endif

#include "sc8800h_reg_bm.h"
#include "sc8800h_reg_vb.h"
#include "sc8800h_reg_usb.h"
#include "sc8800h_reg_dma.h"
#include "sc8800h_reg_isp.h"
#include "sc8800h_reg_int.h"
#include "sc8800h_reg_ahb.h"
#include "sc8800h_reg_rot.h"
#include "sc8800h_reg_lcm.h"
#include "sc8800h_reg_cpc.h"
#include "sc8800h_reg_sim.h"
#include "sc8800h_reg_kpd.h"
#include "sc8800h_reg_i2c.h"
#include "sc8800h_reg_rtc.h"
#include "sc8800h_reg_emc.h"
#include "sc8800h_reg_spi.h"
#include "sc8800h_reg_gea.h"
#include "sc8800h_reg_ept.h"
#include "sc8800h_reg_wdg.h"
#include "sc8800h_reg_cpc.h"
#include "sc8800h_reg_pwm.h"
#include "sc8800h_reg_lcdc.h"
#include "sc8800h_reg_uart.h"
#include "sc8800h_reg_gpio.h"
#include "sc8800h_reg_sdio.h"
#include "sc8800h_reg_timer.h"
#include "sc8800h_reg_global.h"
#include "sc8800h_reg_pinmap.h"
#ifdef CHIP_VER_8800H5
#include "sc8800h_reg_tpc.h"
#include "sc8800h_reg_auxadc.h"
#endif

#define SYSTEM_CURRENT_CLOCK (*((volatile uint32 *)SYS_CNT0) & 0xFFFFFFFF)

#define REG32(x)      (*((volatile uint32 *)(x)))

#endif  //_SC8800H_REG_H_

