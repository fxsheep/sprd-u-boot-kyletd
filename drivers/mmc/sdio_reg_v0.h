/******************************************************************************
 ** File Name:    sdio_reg_v0.h                                            *
 ** Author:       Steve.Zhan                                                 *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.Zhan      Create.                                     *
 ******************************************************************************/
#ifndef _SDIO_REG_V0_H_
#define _SDIO_REG_V0_H_
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

#define SDIO0_SYS_ADDR          (SDIO0_BASE_ADDR+0x0)
#define SDIO0_BLK_SIZE          (SDIO0_BASE_ADDR+0x4)
#define SDIO0_BLK_CNT           (SDIO0_BASE_ADDR+0x6)
#define SDIO0_ARGU_REG          (SDIO0_BASE_ADDR+0x8)
#define SDIO0_TRANS_MODE        (SDIO0_BASE_ADDR+0xc)
#define SDIO0_CMD_REG           (SDIO0_BASE_ADDR+0xE)
#define SDIO0_RESPONSE_REG      (SDIO0_BASE_ADDR+0x10)
#define SDIO0_BUF_DATA_PORT     (SDIO0_BASE_ADDR+0x20)
#define SDIO0_PRESENT_STATE     (SDIO0_BASE_ADDR+0x24)
#define SDIO0_HC_CTL            (SDIO0_BASE_ADDR+0x28)
#define SDIO0_PWR_CTL           (SDIO0_BASE_ADDR+0x29)
#define SDIO0_BLK_GAP           (SDIO0_BASE_ADDR+0x2A)
#define SDIO0_WAKEUP_CTL        (SDIO0_BASE_ADDR+0x2B)
#define SDIO0_CLK_CTL           (SDIO0_BASE_ADDR+0x2C)
#define SDIO0_TIMEOUT_CTL       (SDIO0_BASE_ADDR+0x2E)
#define SDIO0_SW_RESET          (SDIO0_BASE_ADDR+0x2F)
#define SDIO0_NML_INT_STS       (SDIO0_BASE_ADDR+0x30)
#define SDIO0_ERR_INT_STS       (SDIO0_BASE_ADDR+0x32)
#define SDIO0_NML_INT_STS_EN    (SDIO0_BASE_ADDR+0x34)
#define SDIO0_ERR_INT_STS_EN    (SDIO0_BASE_ADDR+0x36)
#define SDIO0_NML_INT_SIG_EN    (SDIO0_BASE_ADDR+0x38)
#define SDIO0_ERR_INT_SIG_EN    (SDIO0_BASE_ADDR+0x3A)
#define SDIO0_ACMD12_ERRSTS     (SDIO0_BASE_ADDR+0x3C)
#define SDIO0_CAPBILITY_REG     (SDIO0_BASE_ADDR+0x40)
#define SDIO0_MAX_CUR_CAP_REG   (SDIO0_BASE_ADDR+0x48)
#define SDIO0_SLOT_INT_STS      (SDIO0_BASE_ADDR+0xFC)
#define SDIO0_HC_VER_REG        (SDIO0_BASE_ADDR+0xFE)

#define SDIO1_SYS_ADDR          (SDIO1_BASE_ADDR+0x0)
#define SDIO1_BLK_SIZE          (SDIO1_BASE_ADDR+0x4)
#define SDIO1_BLK_CNT           (SDIO1_BASE_ADDR+0x6)
#define SDIO1_ARGU_REG          (SDIO1_BASE_ADDR+0x8)
#define SDIO1_TRANS_MODE        (SDIO1_BASE_ADDR+0xc)
#define SDIO1_CMD_REG           (SDIO1_BASE_ADDR+0xE)
#define SDIO1_RESPONSE_REG      (SDIO1_BASE_ADDR+0x10)
#define SDIO1_BUF_DATA_PORT     (SDIO1_BASE_ADDR+0x20)
#define SDIO1_PRESENT_STATE     (SDIO1_BASE_ADDR+0x24)
#define SDIO1_HC_CTL            (SDIO1_BASE_ADDR+0x28)
#define SDIO1_PWR_CTL           (SDIO1_BASE_ADDR+0x29)
#define SDIO1_BLK_GAP           (SDIO1_BASE_ADDR+0x2A)
#define SDIO1_WAKEUP_CTL        (SDIO1_BASE_ADDR+0x2B)
#define SDIO1_CLK_CTL           (SDIO1_BASE_ADDR+0x2C)
#define SDIO1_TIMEOUT_CTL       (SDIO1_BASE_ADDR+0x2E)
#define SDIO1_SW_RESET          (SDIO1_BASE_ADDR+0x2F)
#define SDIO1_NML_INT_STS       (SDIO1_BASE_ADDR+0x30)
#define SDIO1_ERR_INT_STS       (SDIO1_BASE_ADDR+0x32)
#define SDIO1_NML_INT_STS_EN    (SDIO1_BASE_ADDR+0x34)
#define SDIO1_ERR_INT_STS_EN    (SDIO1_BASE_ADDR+0x36)
#define SDIO1_NML_INT_SIG_EN    (SDIO1_BASE_ADDR+0x38)
#define SDIO1_ERR_INT_SIG_EN    (SDIO1_BASE_ADDR+0x3A)
#define SDIO1_ACMD12_ERRSTS     (SDIO1_BASE_ADDR+0x3C)
#define SDIO1_CAPBILITY_REG     (SDIO1_BASE_ADDR+0x40)
#define SDIO1_MAX_CUR_CAP_REG   (SDIO1_BASE_ADDR+0x48)
#define SDIO1_SLOT_INT_STS      (SDIO1_BASE_ADDR+0xFC)
#define SDIO1_HC_VER_REG        (SDIO1_BASE_ADDR+0xFE)

typedef struct SDIO_REG_CFG_TAG
{
    volatile uint32 DMA_SYS_ADD;
    volatile uint32 BLK_SIZE_COUNT;
    volatile uint32 CMD_ARGUMENT;
    volatile uint32 CMD_TRANSMODE;
    volatile uint32 RSP0;
    volatile uint32 RSP1;
    volatile uint32 RSP2;
    volatile uint32 RSP3;
    volatile uint32 BUFFER_PORT;
    volatile uint32 PRESENT_STAT;
    volatile uint32 HOST_CTL0;
    volatile uint32 HOST_CTL1;
    volatile uint32 INT_STA;
    volatile uint32 INT_STA_EN;
    volatile uint32 INT_SIG_EN;
    volatile uint32 CMD12_ERR_STA;
    volatile uint32 CAPBILITY;
    volatile uint32 CAPBILITY_RES;
    volatile uint32 CURR_CAPBILITY;
    volatile uint32 CURR_CAPBILITY_RES;
}
SDIO_REG_CFG;


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
