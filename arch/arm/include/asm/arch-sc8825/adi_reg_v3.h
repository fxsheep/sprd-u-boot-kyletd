/******************************************************************************
 ** File Name:    adi_reg_v3.h                                                *
 ** Author:       Tim.Luo                                                     *
 ** DATE:         03/03/2010                                                  *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 03/03/2010    Tim.Luo         Create.                                     *
 ******************************************************************************/

#ifndef _ADI_REG_V3_H_
#define _ADI_REG_V3_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/

///#define  ADI_BASE            0x82000000
#define  ADI_BASE_ADDR          ADI_BASE
#define  ADI_CLK_DIV            (ADI_BASE + 0x0 )
#define  ADI_CTL_REG            (ADI_BASE + 0x4 )
#define  ADI_CHANNEL_PRI        (ADI_BASE + 0x8 )
#define  ADI_INT_EN             (ADI_BASE + 0xC )
#define  ADI_INT_RAW_STS        (ADI_BASE + 0x10)
#define  ADI_INT_MASK_STS       (ADI_BASE + 0x14)
#define  ADI_INT_CLR            (ADI_BASE_ADDR + 0x18)
//#define  RESERVED             (ADI_BASE_ADDR + 0x1C)
//#define  RESERVED             (ADI_BASE_ADDR + 0x20)
#define  ADI_ARM_RD_CMD         (ADI_BASE + 0x24)
#define  ADI_RD_DATA            (ADI_BASE + 0x28)
#define  ADI_FIFO_STS           (ADI_BASE + 0x2C)
#define  ADI_STS                (ADI_BASE + 0x30)
#define  ADI_REQ_STS            (ADI_BASE + 0x34)

//ADI_EIC
#define ADI_EIC_DATA	(ADI_BASE + 0x700)
#define ADI_EIC_MASK	(ADI_BASE + 0x704)

//ADI_CTL_REG
#define ANA_INT_STEAL_EN        BIT_0
#define ARM_SERCLK_EN           BIT_1
#define DSP_SERCLK_EN           BIT_2

//ADI_FIFO_STS
#define   ADI_FIFO_EMPTY        BIT_10
#define   ADI_FIFO_FULL         BIT_11



//ADI_CHANNEL_PRI bit define
#define    INT_STEAL_PRI        0
#define    STC_WR_PRI           2
#define    ARM_WR_PRI           4
#define    ARM_RD_PRI           6
#define    DSP_WR_PRI           8
#define    DSP_RD_PRI           10
#define    RFT_WR_PRI           12
#define    PD_WR_PRI            14

#define ANA_REG_ADDR_START      0x82000040
#define ANA_REG_ADDR_END        0x82000780

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

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

#endif  //_ADI_REG_V3_H_


