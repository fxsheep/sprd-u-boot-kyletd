#ifndef _SDIO_REG_H_
#define _SDIO_REG_H_



#include "sci_types.h"



#ifdef CHIP_VER_8800H5
#define 	SDIO_BASE_CLK_36M		36000000		// 36 MHz
#else
#define 	SDIO_BASE_CLK_80M		80000000		// 80 MHz
#define 	SDIO_BASE_CLK_64M		64000000		// 64 MHz
#define 	SDIO_BASE_CLK_50M		50000000		// 50 MHz	,should cfg MPLL to 300/350/400???
#endif
#define 	SDIO_BASE_CLK_32M		32000000		// 32 MHz
#define 	SDIO_BASE_CLK_25M		25000000			// 25  MHz
#define 	SDIO_BASE_CLK_20M		20000000			// 20  MHz
#define 	SDIO_BASE_CLK_16M		16000000		// 16 MHz
#define 	SDIO_BASE_CLK_8M		8000000			// 8  MHz

#define 	SDIO_CLK_50M			50000000		// 50   MHz, 1 div for 50MHz
#define 	SDIO_CLK_40M			40000000		// 40  MHz
#define 	SDIO_CLK_32M			32000000		// 32  MHz
#define 	SDIO_CLK_25M			25000000		// 25   MHz, 2 div for 50MHz
#define 	SDIO_CLK_20M			20000000		// 20   MHz
#define 	SDIO_CLK_16M			16000000		// 16  MHz
#define 	SDIO_CLK_12P5M			12500000		// 12.5   MHz, 4 div for 50MHz
#define 	SDIO_CLK_10M			10000000		// 10   MHz
#define 	SDIO_CLK_8M			8000000			// 8   MHz
#define 	SDIO_CLK_6P25M			6250000			// 6.25   MHz, 8 div for 50MHz
#define 	SDIO_CLK_4M			4000000			// 4   MHz
#define 	SDIO_CLK_3P125M		3125000			// 3.125 MHz, 16 div for 50MHz
#define 	SDIO_CLK_2M			2000000			// 2   MHz
#define 	SDIO_CLK_1P56M			1562500			// 1.562 MHz, 32 div for 50MHz
#define 	SDIO_CLK_1M			1000000			// 1   MHz
#define 	SDIO_CLK_781K			781250			// 781 KHz, 64 div for 50MHz
#define 	SDIO_CLK_390K			390625			// 390 KHz, 128 div for 50MHz
#define 	SDIO_CLK_250K			250000			// 250 KHz
#define 	SDIO_CLK_195K			195312			// 195 KHz, 256 div for 50MHz
#define 	SDIO_CLK_125K			125000			// 125 KHz
//-jason.wu confirm start
#define SDIO0_BASE_ADDR            0x20500000
#define SDIO1_BASE_ADDR            0x20500100

#define SDIO0_CAPBILITY_REG	(SDIO0_BASE_ADDR+0x40)
#define SDIO0_MAX_CUR_CAP_REG	(SDIO0_BASE_ADDR+0x48)
#define SDIO0_SLOT_INT_STS	(SDIO0_BASE_ADDR+0xFC)
#define SDIO0_HC_VER_REG		(SDIO0_BASE_ADDR+0xFE)

#define SDIO1_CAPBILITY_REG	(SDIO1_BASE_ADDR+0x40)
#define SDIO1_MAX_CUR_CAP_REG	(SDIO1_BASE_ADDR+0x48)
#define SDIO1_SLOT_INT_STS	(SDIO1_BASE_ADDR+0xFC)
#define SDIO1_HC_VER_REG		(SDIO1_BASE_ADDR+0xFE)
#ifdef CHIP_VER_8800H5
#define SDIO1_CTL1                        (SDIO1_BASE_ADDR+0x28)
#endif

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
#define SDIO_CMD_TYPE_ABORT					(3<<22)
#define SDIO_CMD_TYPE_RESUME					(2<<22)
#define SDIO_CMD_TYPE_SUSPEND					(1<<22)
#define SDIO_CMD_TYPE_NML						(0<<22)

#define SDIO_CMD_DATA_PRESENT					BIT_21

#define SDIO_CMD_INDEX_CHK					BIT_20
#define SDIO_CMD_CRC_CHK						BIT_19
#define SDIO_CMD_NO_RSP						(0x00<<16)
#define SDIO_CMD_RSP_136						(0x01<<16)
#define SDIO_CMD_RSP_48						(0x02<<16)
#define SDIO_CMD_RSP_48_BUSY					(0x03<<16)

#define SDIO_NO_RSP		0x0;
#define SDIO_R1		( SDIO_CMD_RSP_48		| SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R2		( SDIO_CMD_RSP_136		| SDIO_CMD_CRC_CHK )
#define SDIO_R3		SDIO_CMD_RSP_48
#define SDIO_R4		SDIO_CMD_RSP_48
#define SDIO_R5		( SDIO_CMD_RSP_48		| SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R6		( SDIO_CMD_RSP_48		| SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R7		( SDIO_CMD_RSP_48		| SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R1B	( SDIO_CMD_RSP_48_BUSY	| SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
#define SDIO_R5B	( SDIO_CMD_RSP_48_BUSY	| SDIO_CMD_INDEX_CHK | SDIO_CMD_CRC_CHK )
//transfer mode
#define SDIO_TRANS_COMP_ATA			BIT_6
#define SDIO_TRANS_MULTIBLK			BIT_5
#define SDIO_TRANS_DIR_READ			BIT_4
#define SDIO_TRANS_AUTO_CMD12_EN		BIT_2
#define SDIO_TRANS_BLK_CNT_EN			BIT_1
#define SDIO_TRANS_DMA_EN				BIT_0

//=====
//define normal and error sts index...
#define SDIO_VENDOR_SPEC_ERR		(BIT_29|BIT_30|BIT_31)
#define SDIO_TARGET_RESP_ERR		(BIT_28)
#define SDIO_AUTO_CMD12_ERR		(BIT_24)
#define SDIO_CURRENT_LMT_ERR		(BIT_23)
#define SDIO_DATA_ENDBIT_ERR		(BIT_22)
#define SDIO_DATA_CRC_ERR			(BIT_21)
#define SDIO_DATA_TMOUT_ERR		(BIT_20)
#define SDIO_CMD_INDEX_ERR		(BIT_19)
#define SDIO_CMD_ENDBIT_ERR		(BIT_18)
#define SDIO_CMD_CRC_ERR			(BIT_17)
#define SDIO_CMD_TMOUT_ERR		(BIT_16)
#define SDIO_ERROR_INT				(BIT_15)
#define SDIO_CARD_INT				(BIT_8)
#define SDIO_CARD_REMOVAL			(BIT_7)
#define SDIO_CARD_INSERTION		(BIT_6)
#define SDIO_BUF_READ_RDY			(BIT_5)
#define SDIO_BUF_WRITE_RDY		(BIT_4)
#define SDIO_DMA_INT				(BIT_3)
#define SDIO_BLK_GAP_EVT			(BIT_2)
#define SDIO_TRANSFER_CMPLETE		(BIT_1)
#define SDIO_CMD_CMPLETE			(BIT_0)

//-jason.wu confirm end



#endif

