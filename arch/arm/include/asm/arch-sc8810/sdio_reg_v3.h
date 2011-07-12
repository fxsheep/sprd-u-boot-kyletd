/******************************************************************************
 ** File Name:    sdio_reg_v3.h                                           *
 ** Author:       Binggo.Zhou                                               *
 ** DATE:         02/03/2010                                                  *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 02/03/2010    Binggo.Zhou     Create.                                     *
 ******************************************************************************/


#ifndef _SDIO_REG_V3_H_
#define _SDIO_REG_V3_H_


//-jason.wu confirm start

#define SDIO0_SYS_ADDR          (SDIO0_BASE_ADDR+0x0)
#define SDIO0_BLK_SIZE          (SDIO0_BASE_ADDR+0x4)
#define SDIO0_BLK_CNT           (SDIO0_BASE_ADDR+0x6)
#define SDIO0_ARGU_REG          (SDIO0_BASE_ADDR+0x8)
#define SDIO0_TRANS_MODE        (SDIO0_BASE_ADDR+0xc)
#define SDIO0_CMD_REG           (SDIO0_BASE_ADDR+0xE)
#define SDIO0_RESPONSE_REG      (SDIO0_BASE_ADDR+0x10)
#define SDIO0_BUF_DATA_PORT (SDIO0_BASE_ADDR+0x20)
#define SDIO0_PRESENT_STATE (SDIO0_BASE_ADDR+0x24)
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
#define SDIO0_ACMD12_ERRSTS (SDIO0_BASE_ADDR+0x3C)
#define SDIO0_CAPBILITY_REG (SDIO0_BASE_ADDR+0x40)
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
#define SDIO1_BUF_DATA_PORT (SDIO1_BASE_ADDR+0x20)
#define SDIO1_PRESENT_STATE (SDIO1_BASE_ADDR+0x24)
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
#define SDIO1_ACMD12_ERRSTS (SDIO1_BASE_ADDR+0x3C)
#define SDIO1_CAPBILITY_REG (SDIO1_BASE_ADDR+0x40)
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


//---

//=====
//define transfer mode and command mode...
//command mode
#define SDIO_CMD_TYPE_ABORT                 (3<<22)
#define SDIO_CMD_TYPE_RESUME                    (2<<22)
#define SDIO_CMD_TYPE_SUSPEND                   (1<<22)
#define SDIO_CMD_TYPE_NML                       (0<<22)

#define SDIO_CMD_DATA_PRESENT                   BIT_21

#define SDIO_CMD_INDEX_CHK                  BIT_20
#define SDIO_CMD_CRC_CHK                        BIT_19
#define SDIO_CMD_NO_RSP                     (0x00<<16)
#define SDIO_CMD_RSP_136                        (0x01<<16)
#define SDIO_CMD_RSP_48                     (0x02<<16)
#define SDIO_CMD_RSP_48_BUSY                    (0x03<<16)

#define SDIO_NO_RSP     0x0;
#define SDIO_R1     ( SDIO_CMD_RSP_48 | SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R2     ( SDIO_CMD_RSP_136 | SDIO_CMD_CRC_CHK )
#define SDIO_R3     SDIO_CMD_RSP_48
#define SDIO_R4     SDIO_CMD_RSP_48
#define SDIO_R5     ( SDIO_CMD_RSP_48 | SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R6     ( SDIO_CMD_RSP_48 | SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R7     ( SDIO_CMD_RSP_48 | SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R1B    ( SDIO_CMD_RSP_48_BUSY | SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R5B    ( SDIO_CMD_RSP_48_BUSY | SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
//transfer mode
#define SDIO_TRANS_COMP_ATA         BIT_6
#define SDIO_TRANS_MULTIBLK         BIT_5
#define SDIO_TRANS_DIR_READ         BIT_4
#define SDIO_TRANS_AUTO_CMD12_EN        BIT_2
#define SDIO_TRANS_BLK_CNT_EN           BIT_1
#define SDIO_TRANS_DMA_EN               BIT_0

//=====
//define normal and error sts index...
#define SDIO_VENDOR_SPEC_ERR        (BIT_29|BIT_30|BIT_31)
#define SDIO_TARGET_RESP_ERR        (BIT_28)
#define SDIO_AUTO_CMD12_ERR         (BIT_24)
#define SDIO_CURRENT_LMT_ERR        (BIT_23)
#define SDIO_DATA_ENDBIT_ERR        (BIT_22)
#define SDIO_DATA_CRC_ERR           (BIT_21)
#define SDIO_DATA_TMOUT_ERR         (BIT_20)
#define SDIO_CMD_INDEX_ERR          (BIT_19)
#define SDIO_CMD_ENDBIT_ERR         (BIT_18)
#define SDIO_CMD_CRC_ERR            (BIT_17)
#define SDIO_CMD_TMOUT_ERR          (BIT_16)
#define SDIO_ERROR_INT              (BIT_15)
#define SDIO_CARD_INT               (BIT_8)
#define SDIO_CARD_REMOVAL           (BIT_7)
#define SDIO_CARD_INSERTION         (BIT_6)
#define SDIO_BUF_READ_RDY           (BIT_5)
#define SDIO_BUF_WRITE_RDY          (BIT_4)
#define SDIO_DMA_INT                (BIT_3)
#define SDIO_BLK_GAP_EVT            (BIT_2)
#define SDIO_TRANSFER_CMPLETE       (BIT_1)
#define SDIO_CMD_CMPLETE            (BIT_0)

//-jason.wu confirm end



#endif //_SDIO_REG_V3_H_

