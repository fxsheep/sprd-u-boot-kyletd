/******************************************************************************
 ** File Name:        tpc_reg_v3.h                                       *
 ** Author:           Yi.Qiu                                                  *
 ** DATE:             09/16/2009                                              *
 ** Copyright:        2009 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE            NAME            DESCRIPTION                               *
 ** 09/16/2009      Yi.Qiu          Reconstruct TPC driver                    *
 ** 05/07/2010      Mingwei.zhang   Modify it for SC8800G.                    *
 ******************************************************************************/
#ifndef _TPC_REG_V3_H_
#define _TPC_REG_V3_H_
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
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
/* TPC control registers */
#define TPC_REG_BASE                TPC_BASE
#define TPC_CTRL                    (TPC_REG_BASE + 0x0000)
#define TPC_SAMPLE_CTRL0            (TPC_REG_BASE + 0x0004)
#define TPC_SAMPLE_CTRL1            (TPC_REG_BASE + 0x0008)
#define TPC_BOUNCE_CTRL             (TPC_REG_BASE + 0x000C)
#define TPC_FILTER_CTRL             (TPC_REG_BASE + 0x0010)
#define TPC_CALC_CTRL               (TPC_REG_BASE + 0x0014)
#define TPC_CALC_X_COEF_A           (TPC_REG_BASE + 0x0018)
#define TPC_CALC_X_COEF_B           (TPC_REG_BASE + 0x001C)
#define TPC_CALC_Y_COEF_A           (TPC_REG_BASE + 0x0020)
#define TPC_CALC_Y_COEF_B           (TPC_REG_BASE + 0x0024)
#define TPC_INT_EN                  (TPC_REG_BASE + 0x0028)
#define TPC_INT_STS                 (TPC_REG_BASE + 0x002C)
#define TPC_INT_RAW                 (TPC_REG_BASE + 0x0030)
#define TPC_INT_CLR                 (TPC_REG_BASE + 0x0034)
#define TPC_BUF_CTRL                (TPC_REG_BASE + 0x0038)
#define TPC_X_DATA                  (TPC_REG_BASE + 0x003C)
#define TPC_Y_DATA                  (TPC_REG_BASE + 0x0040)
#define TPC_Z_DATA                  (TPC_REG_BASE + 0x0044)

//TPC_CTRL BIT map
#define TPC_STOP_BIT                BIT_5
#define TPC_RUN_BIT                 BIT_4
#define TPC_TPC_MODE_BIT            BIT_3
#define TPC_PEN_REQ_POL_BIT     BIT_1
#define TPC_EN_BIT                  BIT_0

#define TPC_PRESCALE_OFFSET     0x08
#define TPC_PRESCALE_MSK            (0xFF << TPC_PRESCALE_OFFSET)

//TPC_SAMPLE_CTRL0 BIT map
#define TPC_SAMPLE_INTERVAL_OFFSET  0
#define TPC_SAMPLE_INTERVAL_MSK     (0xFF << TPC_SAMPLE_INTERVAL_OFFSET)
#define TPC_POINT_INTERVAL_OFFSET       8
#define TPC_POINT_INTERVAL_MSK      (0xFF << TPC_POINT_INTERVAL_OFFSET)

//TPC_SAMPLE_CTRL1 BIT map
#define TPC_DATA_INTERVAL_OFFSET        0
#define TPC_DATA_INTERVAL_MSK           (0xFFF << TPC_DATA_INTERVAL_OFFSET)
#define TPC_SAMPLE_NUM_OFFSET           12
#define TPC_SAMPLE_NUM_MSK          (0xF << TPC_SAMPLE_NUM_OFFSET)

//TPC_BOUNCE_CTRL BIT map
#define TPC_DEBOUNCE_EN_BIT         BIT_0
#define TPC_DEBOUNCE_NUM_OFFSET     1
#define TPC_DEBOUNCE_NUM_MSK            (0xFF << TPC_DEBOUNCE_NUM_OFFSET)


//TPC_FILTER_CTRL BIT map
#define TPC_FILTER_EN_BIT               BIT_0
#define TPC_FILTER_MODE_BIT         BIT_1
#define TPC_FILTER_MODE_OFFSET      2
#define TPC_FILTER_MODE_MSK         (0xF << TPC_FILTER_MODE_OFFSET)

//TPC_CALC_CTRL BIT map
#define TPC_CALC_EN_BIT             BIT_0

//TPC INT BIT MAP
#define TPC_DONE_IRQ_MSK_BIT    BIT_2
#define TPC_UP_IRQ_MSK_BIT      BIT_1
#define TPC_DOWN_IRQ_MSK_BIT    BIT_0
#define TPC_INT_ALL_MSK         (TPC_DONE_IRQ_MSK_BIT|TPC_UP_IRQ_MSK_BIT|TPC_DOWN_IRQ_MSK_BIT)

//TPC_CALC_X_COEF_A
#define TPC_X_COEF_A_MSK        0x3FF
#define TPC_X_COEF_B_MSK        0x7FF
#define TPC_Y_COEF_A_MSK        0x3FF
#define TPC_Y_COEF_B_MSK        0x7FF

//TPC_BUF_CTRL BIT map
#define TPC_BUF_EMP_BIT     BIT_5
#define TPC_BUF_FULL            BIT_4
#define TPC_BUF_LENGTH_OFFSET   0
#define TPC_BUF_LENGTH_MSK  (0xF << TPC_BUF_LENGTH_OFFSET)

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


