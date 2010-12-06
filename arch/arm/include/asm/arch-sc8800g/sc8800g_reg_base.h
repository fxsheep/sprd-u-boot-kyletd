/******************************************************************************
 ** File Name:      sc8800g_reg_base.h                                            *
 ** Author:         Tim.Luo                                                   *
 ** DATE:           07/08/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/08/2010     Tim.Luo          Create.                                   *
  ******************************************************************************/

#ifndef _SC8800G_REG_BASE_H_
#define _SC8800G_REG_BASE_H_

#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/*----------memory map address----------*/
//Memory Address Map.

#define SHARE_MEM_BEGIN                 0x50000000    //Internal Shared Memory.
#define SHARE_MEM_END                   0x50000fff    //The address of the last byte

#define INTER_RAM_BEGIN                 0x40000000    //Internal ram
#define INTER_RAM_END                   0x4000A7FF    //The address of the last byte

#define DSP_MEM_BEGIN                   0x00400000    //DSP memory base address.
#define DSP_MEM_LEN                     0x00400000    //length unit:byte.


//CONTROL REGISTER
#define EXTERNAL_MEM_CTL_BEGIN          0x20000000    //External Memory Control registers.
#define EXTERNAL_MEM_CTL_END            0x200001FC

#define DMA_GEN_CTL_BEGIN               0x20100000    //DMA General Control registers.
#define DMA_GEN_CTL_END                 0x201000C4

#define DMA_CHA_CTL_BEGIN               0x20100400    //DMA Channel Control registers.
#define DMA_CHA_CTL_END                 0x201007FC

#define DCAM_CTL_BEGIN                  0x20200000    //DCAM Control registers. 
#define DCAM_CTL_END                    0x202027FF    //the begin address of the last word

#define ROTATION_CTL_BEGIN              0x20800200    //ROTATION Device Space.                      
#define ROTATION_CTL_END                0x2080022C    //length unit:word.

#define USB_CTL_BEGIN                   0x20300000    //USB Device Space.                           
#define USB_CTL_END                     0x20300E00    //the begin address of the last word

#define BUS_MON0_CTL_BEGIN              0x20400000    //Bus Monitor 0 Control registers.              
#define BUS_MON0_CTL_END                0x20400024    //The address of the last byte

#define BUS_MON1_CTL_BEGIN              0x20401000    //Bus Monitor 1 Control registers.              
#define BUS_MON1_CTL_END                0x20401024    //The address of the last byte


#define AHB_GEN_CTL_BEGIN               0x20900200    //Bus Monitor Control registers.
#define AHB_GEN_CTL_END                 0x209002A0

#define CHIP_ID_BEGIN                   0x209003FC    //CHIP ID registers.
#define CHIP_ID_END                     0x209003FC

#define NAND_LCM_CTL_BEGIN              0x60001C00    //NAND Flash and LCM Control Registers
#define NAND_LCM_CTL_END                0x60001D44

#define LCDC_CTL_BEGIN                  0x20700000    //LCDC Control Registers                      
#define LCDC_CTL_END                    0x2070012C    //length unit:word.

#define LCDC_LCM_CTL_BEGIN              0x20700140    //LCDC/LCM Control Registers              
#define LCDC_LCM_CTL_END                0x20700194    //length unit:word.

#define INT_CTL_BEGIN                   0x80003000    //Interrupt Control Registers                 
#define INT_CTL_END                     0x80000040    //the begin address of the last word

#define TIMER_CNT_BEGIN                 0x81000000    //tiemr counter Registers
#define TIMER_CNT_END                   0x8100004C    //the begin address of the last word

#define ADI_CTL_BEGAIN                  0x82000000  //adi master control registers
#define ADI_CTL_END                     0x82000034

#define UART0_CTL_BEGIN                 0x83000000    //UART0,SPI0 Control Registers
#define UART0_CTL_END                   0x8300002C    //the begin address of the last word

#define UART1_CTL_BEGIN                 0x84000000    //UART1,SPI1 Control Registers
#define UART1_CTL_END                   0x8400002C    //the begin address of the last word

#define UART2_CTL_BEGIN                 0x8E000000    //UART2,SPI2 Control Registers
#define UART2_CTL_END                   0x8E00002C    //the begin address of the last word

#define SIM0_CTL_BEGIN                  0x85000000    //SIMCARD Control Registers
#define SIM0_CTL_END                    0x85000038    //the begin address of the last word

#define SIM1_CTL_BEGIN                  0x85003000    //SIMCARD Control Registers
#define SIM1_CTL_END                    0x85003038    //the begin address of the last word


#define I2C_CTL_BEGIN                   0x86000000    //I2C Control Registers
#define I2C_CTL_END                     0x86000014    //the begin address of the last word

#define KEYPAD_CTL_BEGIN                0x87000000    //Keypad Control Registers        
#define KEYPAD_CTL_END                  0x87000038    //the begin address of the last word

#define SYS_CNT_BEGIN                   0x87003000    //system counter Registers
#define SYS_CNT_END                     0x87003008    //the begin address of the last word

#define PWM_CTL_BEGIN                   0x88000000    //Keypad Control Registers
#define PWM_CTL_END                     0x88000070    //the begin address of the last word

#define RTC_CTL_BEGIN                   0x82000080    //RTC Control Registers
#define RTC_CTL_END                     0x820000BC    //the begin address of the last word

#define WATDOG_CTL_BEGIN                0x82000040    //watchdog Control Registers
#define WATDOG_CTL_END                  0x82000060    //the begin address of the last word

#define GPIO_CTL_BEGIN                  0x8A000000    //GPIO Control Registers              ///digital die
#define GPIO_CTL_END                    0x8A0004A4

#define GLOBAL_CTL_BEGIN                0x8B000000    //GLOBAL Control Registers
#define GLOBAL_CTL_END                  0x8B000080    //the begin address of the last word

#define CHIPPIN_CTL_BEGIN               0x8C000000    //ChipPin Control Registers
#define CHIPPIN_CTL_END                 0x8C0003EC

#define VOICE_BAND_CODEC_BEGIN          0x82000100    //Voice Band Codec register       ///digital die
#define VOICE_BAND_CODEC_END            0x82000154


/*----------Peripheral Address Space------------*/
#define INTC_BASE                       0x80003000
#define TIMER_CTL_BASE                  0x81000000  //Timer0 (RTC)
#define ADI_BASE                        0x82000000  //ADI master
#define WDG_BASE                        0x82000040  //Analog die register   
#define RTC_BASE                        0x82000080
#define ANA_DOLPHIN_BASE                0x82000100  //Analog die register   
#define ANA_PINMAP_BASE                 0x82000180
#define TPC_BASE                        0x82000280
#define ADC_BASE                        0x82000300
#define ANA_INTC_BASE                   0x82000380
#define ANA_REG_BASE                    0x82000480
#define ANA_GPIO_BASE                   0x82000600
#define ARM_VBC_BASE                    0x82003000
#define ARM_UART0_BASE                  0x83000000
#define ARM_UART1_BASE                  0x84000000
#define ARM_UART2_BASE                  0x8E000000
#define SIM0_BASE                       0x85000000  //SIM0
#define SIM1_BASE                       0x85003000  //SIM1
#define I2C_BASE                        0x86000000
#define KPD_BASE                        0x87000000
#define SYSTIMER_BASE                   0x87003000  //System timer
#define PWM_BASE                        0x88000000
#define EFUSE_BASE                      0x89000000  //efuse 
#define GPIO_BASE                       0x8A000000
#define GREG_BASE                       0x8B000000  //Global Registers
#define PIN_CTL_BASE                    0x8C000000
#define ANA_PIN_CTL_BASE                0x82000180
#define EPT_BASE                        0x8D000000
#define PCM_CTL_BASE                    0x8E001000
#define SPI_BASE                        0x8E002000


#define INT_REG_BASE                    INTC_BASE
#define EXT_MEM_CTL_BASE                0x20000000
#define DMA_REG_BASE                    0x20100000
#define DCAM_BASE                       0x20200000
#define USB_REG_BASE                    0x20300000
#define BUS_MONx_CTL_BASE               0x20400000
#define BUS_MON0_CTL_BASE               0x20400000
#define BUS_MON1_CTL_BASE               0x20401000
#define BUS_MON2_CTL_BASE               0x20402000
#define BUS_MON_CTL_BASE                BUS_MON0_CTL_BASE

#define SDIO0_BASE_ADDR                 0x20500000
#define SDIO1_BASE_ADDR                 0x20500100
#define REG_LCDC_REG_BASE               0x20700000
#define LCM_REG_BASE                    0x20700140
#define ROT_REG_BASE                    0x20800200
#define NAND_CTL_BASE                   0x60001c00
#define NF_LCM_CTL_BEGIN                0x60000000      //NAND Flash and LCM Control Registers



#define GEA_BASE    EPT_BASE
#ifdef   __cplusplus
}
#endif

#endif  //_SC6800H_REG_H_

