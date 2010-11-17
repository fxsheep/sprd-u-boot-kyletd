/******************************************************************************
 ** File Name:    sc8800g_int_cfg.h                                            *
 ** Author:       steve.zhan                                                 *
 ** DATE:         11/13/2005                                                  *
 ** Copyright:    2010  Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.zhan      Create.                                     *
 ******************************************************************************/
#ifndef __SC8800G_INT_CFG_H__
#define __SC8800G_INT_CFG_H__

#include "sci_types.h"
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
**                               Macro Define for int                        **
**---------------------------------------------------------------------------*/

//#define TB_MAX_INT_NUM          0x20 // last int num
//#define FIQ_MAX_INT_NUM             0x20


#define TB_UINT                 0x00
#define TB_SOFT_INT             0x01
#define TB_UART0_INT                0x02
#define TB_UART1_INT                0x03
#define TB_UART2_INT                0x04
#define TB_TIMER0_INT               0x05
#define TB_TIMER1_INT               0x06
#define TB_TIMER2_INT               0x07
#define TB_GPIO_INT             0x08
#define TB_SPI_INT              0x09
#define TB_KPD_INT              0x0a
#define TB_I2C_INT              0x0b
#define TB_SIM0_INT             0x0c
#define TB_SIM1_INT             0x0c
#define TB_CX_SEM_INT               0x0d
#define TB_CX_CR_INT                0x0e
#define TB_DSP_INT              0x0f
#define TB_DSP_INT1             0x10
#define TB_SYST_INT             0x11
#define TB_EPT_INT              0x12
#define TB_IIS_INT                      0x13
#define TB_DSP_OR_INT               0x14
#define TB_DMA_INT                      0x15
#define TB_VBC_INT                      0x16
#define TB_VSP_INT                      0x17
#define TB_ANA_DIE_INT                  0x18
#define TB_ADI_INT                      0x19
#define TB_USB_INT                      0x1A
#define TB_DCAM_INT                     0x1B
#define TB_NLC_INT                      0x1C
#define TB_LCDC_INT                     0x1D
#define TB_SDIO_INT                     0x1E
#define TB_BM_INT                   0x1F

#define PCM_IRQ_INT              TB_IIS_INT
#define TB_ADC_INT              0x20
#define TB_ANA_GPIO_INT         0x21
#define TB_RTC_INT              0x22
#define TB_WDG_INT              0x23
#define TB_TPC_INT              0x24


#define TB_TIMER0               TB_TIMER0_INT
#define TB_TIMER1               TB_TIMER1_INT
#define TB_TIMER2               TB_TIMER2_INT
#define TB_SIM_INT              TB_SIM0_INT

#define ISR_UNIT_0  0 //root
#define ISR_UNIT_1  1
#define ISR_UNIT_2  2
#define ISR_UNIT_3  3

typedef void (* CALLBACK_T) (void *);

typedef struct
{
    uint32 intLogicNum;
    uint32 IntRegValueIndex;
    uint32 IntRegBitMask;

    uint32 (*Handlefunc) (uint32);
    CALLBACK_T callback;
    uint32 (*isrDefaultFunc) (uint32);
} PRI_INDEX_GET_STATUS_T;

//interrupt module configuration datas.
#define ONE_UNIT_MAX_NUMBER (32)
#define ISR_UNIT_NUM        (2)

#define ISR_UNIT_0_NUMBER   (32)
#define ISR_UNIT_1_NUMBER   (5)
#define ISR_UNIT_2_NUMBER   (0)
#define ISR_UNIT_3_NUMBER   (0)

#define MAX_ISR_NUM (ISR_UNIT_0_NUMBER + ISR_UNIT_1_NUMBER + ISR_UNIT_2_NUMBER + ISR_UNIT_3_NUMBER)

PUBLIC void INT_HAL_IRQEnable (uint32 logic_num);
PUBLIC void INT_HAL_IRQDisable (uint32 logic_num);
PUBLIC void INT_HAL_IRQDisableAll(void);
PUBLIC uint32 INT_HAL_IRQGetUint1Status(void);
PUBLIC void INT_HAL_FIQEnable (uint32 logicNum);
PUBLIC void INT_HAL_FIQDisable (uint32 logicNum);
PUBLIC void INT_HAL_FIQDisableAll(void);
PUBLIC void INT_HAL_DisableINT (void);

#define _CHIPDRV_EnableIRQINT(int_num) do { \
        INT_HAL_IRQEnable(int_num); \
    } while(0)

#define _CHIPDRV_DisableIRQINT(int_num) do { \
        INT_HAL_IRQDisable(int_num); \
    } while(0)

#define _CHIPDRV_DisableIRQINTALL() do { \
        INT_HAL_IRQDisableAll(); \
    } while(0)

#define _CHIPDRV_GetIRQStatus() INT_HAL_IRQGetUint1Status()


#define _CHIPDRV_EnableFIQINT(int_num) do { \
        INT_HAL_FIQEnable(int_num); \
    } while(0)

#define _CHIPDRV_DisableFIQINT(int_num) do { \
        INT_HAL_FIQDisable(int_num); \
    } while(0)

#define _CHIPDRV_DisableFIQINTALL() do { \
        INT_HAL_FIQDisableAll(); \
    } while(0)

#define _CHIPDRV_DisableINT() do { \
        INT_HAL_DisableINT(); \
    } while(0)
//end

extern uint32 MAP_TO_BIT_UNIT[ISR_UNIT_NUM][ONE_UNIT_MAX_NUMBER];


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                                                                               **
**----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function's source code in interrupt module/v0(x)/,
//  for process default  interrupt, this function is related chip type(platform).
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC uint32 INT_PHY_IrqHandleDefaultFunc (uint32 bit);


/*****************************************************************************/
//  Description:    This function's source code in interrupt module/v0(x)/,
//  for process default  interrupt, this function is related chip type(platform).
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC uint32 INT_PHY_FiqHandleDefaultFunc (uint32 bit);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //__SC8800G_INT_CFG_H__
// End
