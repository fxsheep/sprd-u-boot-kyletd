/******************************************************************************
 ** File Name:      card_sdio.c
 ** Author:         Jason.wu
 ** DATE:           09/17/2007
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.
 ** Description:    This file describe operation of SD host.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION
 ** 09/17/2007     Jason.wu        Create.
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "asm/arch/sci_types.h"
#include "asm/arch/sci_api.h"
#include "card_sdio.h"
#include "sdio_card_pal.h"
#ifdef MBBMS_SUPPORT  
#include "sdua_config.h"
#endif

#define CARD_SDIO_HIGHSPEED_SUPPORT

//#define CARD_SDIO_PRINT(x)
#define CARD_SDIO_PRINT(x) SCI_TRACE_LOW x
#define CARD_SDIO_ASSERT SCI_ASSERT
#define DEFAULT_CARD_BLOCKLEN 512
#define CARD_CMD_MAX_TIME 5000

typedef enum CARD_VERTION_E_TAG
{
	CARD_V_UNKONWN,
	CARD_SD_V1_X,
	CARD_SD_V2_0_STANDARD,
	CARD_SD_V2_0_HIGHCAP,
	CARD_MMC_331,
	CARD_MMC_441_STANDARD,
	CARD_MMC_441_HIGHCAP
}CARD_VERTION_E;


/*****************************************************************************/
//	The struct of Card IDentification (CID) register of MMC. please refer to <Multi media card System Specification version 3.31>
/*****************************************************************************/
typedef struct CARD_MMC_CID_STRUCT_TAG
{
	uint8 MID;	// 8 bit	Manufacturer ID
	uint16 OID;	// 16 bit	OEM/Application ID
	uint8 PNM[6];	// 48 bit	Product name
	uint8 PRV;	// 8 bit	Product revision
	uint32 PSN;	// 32 bit	Product serial number
	uint8 MDT;	// 8bit	Manufacturing date
//	uint8 CRC;	// 8 bit
//	uint8 lastBit;	// 1 bit :always 1
}CARD_MMC_CID_T;

/*****************************************************************************/
// The struct of Card-Specific Data register of MMC. please refer to <Multi media card System Specification version 3.31>
/*****************************************************************************/
typedef struct CARD_MMC_CSD_STRUCT_TAG
{
//R
	uint8 CSD_STRUCTURE;	// 2 bit
	uint8 SPEC_VERS;			// 4 bit
	uint8 reserved1;			// 2 bit
	uint8 TAAC;				// 8 bit
	uint8 NSAC;				// 8 bit
	uint8 TRAN_SPEED;		// 8 bit
	uint16 CCC;				//12 bit
	uint8 READ_BL_LEN;		// 4 bit
	uint8 READ_BL_PARTIAL;	// 1 bit
	uint8 WRITE_BLK_MISALIGN;	// 1 bit
	uint8 READ_BLK_MISALIGN;	// 1 bit
	uint8 DSR_IMP;			// 1 bit
	uint8 reserved2;			// 2 bit
	uint16 C_SIZE;			//12 bit
	uint8 VDD_R_CURR_MIN;	// 3 bit
	uint8 VDD_R_CURR_MAX;	// 3 bit
	uint8 VDD_W_CURR_MIN;	// 3 bit
	uint8 VDD_W_CURR_MAX;	// 3 bit
	uint8 C_SIZE_MULT;		// 3 bit
	uint8 ERASE_GRP_SIZE;	// 5 bit
	uint8 ERASE_GRP_MULT;	// 5 bit
	uint8 WP_GRP_SIZE;		// 5 bit
	uint8 WP_GRP_ENABLE;	// 1 bit
	uint8 DEFAULT_ECC;		// 2 bit
	uint8 R2W_FACTOR;		// 3 bit
	uint8 WRITE_BL_LEN;		// 4 bit
	uint8 WRITE_BL_PARTIAL;	// 1 bit
	uint8 reserved3;			// 4 bit
	uint8 CONTENT_PROT_APP;	// 1 bit
//RW
	uint8 FILE_FORMAT_GRP;	// 1 bit
	uint8 COPY;				// 1 bit
	uint8 PERM_WRITE_PROTECT;	// 1 bit
	uint8 TMP_WRITE_PROTECT;	// 1 bit
	uint8 FILE_FORMAT;		// 2 bit
	uint8 ECC;				// 2 bit
//	uint8 CRC;				// 7 bit
//	uint8 LastBit;				// 1 bit: always 1
}CARD_MMC_CSD_T;

/*****************************************************************************/
//	The struct of Card IDentification (CID) register of SD. please refer to <SD Specifications Part 1 Physical Layer Specification Version 2.00>
/*****************************************************************************/
typedef struct CARD_SD_CID_STRUCT_TAG
{
	uint8 MID;	// 8 bit
	uint16 OID;	// 16 bit
	uint8 PNM[5];	// 40 bit
	uint8 PRV;	// 8 bit
	uint32 PSN;	// 32 bit
	uint16 MDT;	// 12bit
//	uint8 CRC;	// 1 bit
//	uint8 lastBit;	// 1 bit :always 1
}CARD_SD_CID_T;

/*****************************************************************************/
// The struct of Card-Specific Data register of SD. please refer to <SD Specifications Part 1 Physical Layer Specification Version 2.00>
/*****************************************************************************/
typedef struct CARD_SD_CSD10_STRUCT_TAG
{
//R
	uint8 CSD_STRUCTURE;	// 2 bit
	uint8 reserved1;			// 6 bit
	uint8 TAAC;				// 8 bit
	uint8 NSAC;				// 8 bit
	uint8 TRAN_SPEED;		// 8 bit
	uint16 CCC;				//12 bit
	uint8 READ_BL_LEN;		// 4 bit
	uint8 READ_BL_PARTIAL;	// 1 bit
	uint8 WRITE_BLK_MISALIGN;	// 1 bit
	uint8 READ_BLK_MISALIGN;	// 1 bit
	uint8 DSR_IMP;			// 1 bit

	uint8 reserved2;			// 2 bit
	uint16 C_SIZE;			//12 bit
	uint8 VDD_R_CURR_MIN;	// 3 bit
	uint8 VDD_R_CURR_MAX;	// 3 bit
	uint8 VDD_W_CURR_MIN;	// 3 bit
	uint8 VDD_W_CURR_MAX;	// 3 bit
	uint8 C_SIZE_MULT;		// 3 bit

	uint8 ERASE_BLK_EN;		// 1 bit
	uint8 SECTOR_SIZE;		// 7 bit
	uint8 WP_GRP_SIZE;		// 7 bit
	uint8 WP_GRP_ENABLE;	// 1 bit
	uint8 reserved3;		// 2 bit

	uint8 R2W_FACTOR;		// 3 bit
	uint8 WRITE_BL_LEN;		// 4 bit
	uint8 WRITE_BL_PARTIAL;	// 1 bit
	uint8 reserved4;			// 5 bit
//RW
	uint8 FILE_FORMAT_GRP;	// 1 bit
	uint8 COPY;				// 1 bit
	uint8 PERM_WRITE_PROTECT;	// 1 bit
	uint8 TMP_WRITE_PROTECT;	// 1 bit
	uint8 FILE_FORMAT;		// 2 bit
	uint8 reserved5;			// 2 bit
//	uint8 CRC;				// 7 bit
//	uint8 LastBit;				// 1 bit: always 1
}CARD_SD_CSD10_T;

/*****************************************************************************/
// The struct of Card-Specific Data register of SD. please refer to <SD Specifications Part 1 Physical Layer Specification Version 2.00>
/*****************************************************************************/
typedef struct CARD_SD_CSD20_STRUCT_TAG
{
//R
	uint8 CSD_STRUCTURE;	// 2 bit
	uint8 reserved1;			// 6 bit
	uint8 TAAC;				// 8 bit
	uint8 NSAC;				// 8 bit
	uint8 TRAN_SPEED;		// 8 bit
	uint16 CCC;				//12 bit
	uint8 READ_BL_LEN;		// 4 bit
	uint8 READ_BL_PARTIAL;	// 1 bit
	uint8 WRITE_BLK_MISALIGN;	// 1 bit
	uint8 READ_BLK_MISALIGN;	// 1 bit
	uint8 DSR_IMP;			// 1 bit

	uint8 reserved2;			// 6 bit
	uint32 C_SIZE;			//22 bit
	uint8 reserved3;			// 1 bit

	uint8 ERASE_BLK_EN;		// 1 bit
	uint8 SECTOR_SIZE;		// 7 bit
	uint8 WP_GRP_SIZE;		// 7 bit
	uint8 WP_GRP_ENABLE;	// 1 bit
	uint8 reserved4;		// 2 bit

	uint8 R2W_FACTOR;		// 3 bit
	uint8 WRITE_BL_LEN;		// 4 bit
	uint8 WRITE_BL_PARTIAL;	// 1 bit
	uint8 reserved5;			// 5 bit
//RW
	uint8 FILE_FORMAT_GRP;	// 1 bit
	uint8 COPY;				// 1 bit
	uint8 PERM_WRITE_PROTECT;	// 1 bit
	uint8 TMP_WRITE_PROTECT;	// 1 bit
	uint8 FILE_FORMAT;		// 2 bit
	uint8 reserved6;			// 2 bit
//	uint8 CRC;				// 7 bit
//	uint8 LastBit;				// 1 bit: always 1
}CARD_SD_CSD20_T;

typedef union CARD_CSD_STRUCT_TAG
{
	CARD_SD_CSD10_T SD_CSD10;
	CARD_SD_CSD20_T SD_CSD20;
	CARD_MMC_CSD_T MMC_CSD;
}CARD_CSD_T;

typedef union CARD_CID_STRUCT_TAG
{
	CARD_SD_CID_T SD_CID;
	CARD_MMC_CID_T MMC_CID;
}CARD_CID_T;
/*lint -save -e749*/
typedef enum CARD_BUS_WIDTH_TAG
{
	CARD_WIDTH_1_BIT,
	CARD_WIDTH_4_BIT,
	CARD_WIDTH_8_BIT
}CARD_BUS_WIDTH_E;

#define PARTITION_CFG_BOOT_PARTITION_ENABLE_OFFSET   3
#define PARTITION_CFG_BOOT_PARTITION_ENABLE_MASK   0x38
#define PARTITION_CFG_BOOT_PARTITION_ENABLE_NONE   (0<<PARTITION_CFG_BOOT_PARTITION_ENABLE_OFFSET)
#define PARTITION_CFG_BOOT_PARTITION_ENABLE_BOOT1   (1<<PARTITION_CFG_BOOT_PARTITION_ENABLE_OFFSET)
#define PARTITION_CFG_BOOT_PARTITION_ENABLE_BOOT2   (2<<PARTITION_CFG_BOOT_PARTITION_ENABLE_OFFSET)
#define PARTITION_CFG_BOOT_PARTITION_ENABLE_USER   (7<<PARTITION_CFG_BOOT_PARTITION_ENABLE_OFFSET)

#define PARTITION_CFG_BOOT_PARTITION_ACCESS_OFFSET  00
#define PARTITION_CFG_BOOT_PARTITION_ACCESS_MASK   0x07
#define PARTITION_CFG_BOOT_PARTITION_ACCESS_PARTITION_USER	0
#define PARTITION_CFG_BOOT_PARTITION_ACCESS_PARTITION_BOOT1	1
#define PARTITION_CFG_BOOT_PARTITION_ACCESS_PARTITION_BOOT2	2
#define PARTITION_CFG_BOOT_PARTITION_ACCESS_PARTITION_RPMB	3
#define PARTITION_CFG_BOOT_PARTITION_ACCESS_PARTITION_GENERAL_P1	4
#define PARTITION_CFG_BOOT_PARTITION_ACCESS_PARTITION_GENERAL_P2 	5
#define PARTITION_CFG_BOOT_PARTITION_ACCESS_PARTITION_GENERAL_P3 	6
#define PARTITION_CFG_BOOT_PARTITION_ACCESS_PARTITION_GENERAL_P4	7

#define EXT_CSD_CARD_WIDTH_1_BIT		0
#define EXT_CSD_CARD_WIDTH_4_BIT		1
#define EXT_CSD_CARD_WIDTH_8_BIT		2

#define EXT_CSD_PARTITION_CFG_INDEX	179
#define EXT_CSD_BUS_WIDTH_INDEX		183

#define CMD6_BIT_MODE_OFFSET_ACCESS	24
#define CMD6_BIT_MODE_MASK_ACCESS	0x03000000
#define CMD6_BIT_MODE_OFFSET_INDEX		16
#define CMD6_BIT_MODE_MASK_INDEX	0x00FF0000
#define CMD6_BIT_MODE_OFFSET_VALUE		8
#define CMD6_BIT_MODE_MASK_VALUE	0x0000FF00
#define CMD6_BIT_MODE_OFFSET_CMD_SET	0
#define CMD6_BIT_MODE_MASK_CMD_SET	0x00000003

#define CMD6_ACCESS_MODE_COMMAND_SET	(0<<CMD6_BIT_MODE_OFFSET_ACCESS)
#define CMD6_ACCESS_MODE_SET_BITS	(1<<CMD6_BIT_MODE_OFFSET_ACCESS)
#define CMD6_ACCESS_MODE_CLEAR_BITS	(2<<CMD6_BIT_MODE_OFFSET_ACCESS)
#define CMD6_ACCESS_MODE_WRITE_BYTE	(3<<CMD6_BIT_MODE_OFFSET_ACCESS)

#define CMD6_CMD_SET_OLD	(0<<CMD6_BIT_MODE_OFFSET_CMD_SET)
#define CMD6_CMD_SET		(1<<CMD6_BIT_MODE_OFFSET_CMD_SET)

/*
 * CSD field definitions
 */

#define CSD_STRUCT_VER_1_0  0           /* Valid for system specification 1.0 - 1.2 */
#define CSD_STRUCT_VER_1_1  1           /* Valid for system specification 1.4 - 2.2 */
#define CSD_STRUCT_VER_1_2  2           /* Valid for system specification 3.1 - 3.2 - 3.31 - 4.0 - 4.1 */
#define CSD_STRUCT_EXT_CSD  3           /* Version is coded in CSD_STRUCTURE in EXT_CSD */

#define CSD_SPEC_VER_0      0           /* Implements system specification 1.0 - 1.2 */
#define CSD_SPEC_VER_1      1           /* Implements system specification 1.4 */
#define CSD_SPEC_VER_2      2           /* Implements system specification 2.0 - 2.2 */
#define CSD_SPEC_VER_3      3           /* Implements system specification 3.1 - 3.2 - 3.31 */
#define CSD_SPEC_VER_4      4           /* Implements system specification 4.0 - 4.1 */

/*
 * EXT_CSD fields
 */

#define EXT_CSD_BUS_WIDTH_INDEX	183	/* R/W */
#define EXT_CSD_HS_TIMING_INDEX	185	/* R/W */
#define EXT_CSD_CARD_TYPE_INDEX	196	/* RO */
#define EXT_CSD_SEC_CNT_INDEX	212	/* RO, 4 bytes */
#define EXT_CSD_PARTITION_CFG_INDEX	179
/*
 * EXT_CSD field definitions
 */

#define EXT_CSD_CMD_SET_NORMAL		(1<<0)
#define EXT_CSD_CMD_SET_SECURE		(1<<1)
#define EXT_CSD_CMD_SET_CPSECURE	(1<<2)

#define EXT_CSD_CARD_TYPE_26	(1<<0)	/* Card can run at 26MHz */
#define EXT_CSD_CARD_TYPE_52	(1<<1)	/* Card can run at 52MHz */

#define EXT_CSD_BUS_WIDTH_1	0	/* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4	1	/* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8	2	/* Card is in 8 bit mode */

/*
 * MMC_SWITCH access modes
 */

#define MMC_SWITCH_MODE_CMD_SET	0x00	/* Change the command set */
#define MMC_SWITCH_MODE_SET_BITS	0x01	/* Set bits which are 1 in value */
#define MMC_SWITCH_MODE_CLEAR_BITS	0x02	/* Clear bits which are 1 in value */
#define MMC_SWITCH_MODE_WRITE_BYTE	0x03	/* Set target to value */


typedef struct CARD_PORT_TAG
{
	BOOLEAN open_flag;

	SDIO_CARD_PAL_HANDLE sdioPalHd;

	CARD_VERTION_E vertion;
	uint16 RCA;
	CARD_BUS_WIDTH_E bus_width;

	uint32 ClockRate;
	uint32 BlockLen;

	uint32 Nac_Max;
	uint32 GrpSize;
	uint32 Capacity;
	uint32 Rpmb_Capacity;
	uint32 Boot1_Capacity;
	uint32 Boot2_Capacity;
	CARD_EMMC_PARTITION_TPYE Cur_Partition;

	uint16  CCC;
	BOOLEAN ifSupportEC;
	BOOLEAN ifSupportHighSpeed;

	BOOLEAN ifEnterEC;
	BOOLEAN ifEnterHighSpeed;
}CARD_PORT_T;

__align(32) uint8 s_extcsdbuf[512];
__align(32) uint8 s_sendbuf[512];
__align(32) uint8 s_receivebuf1[512];
__align(32) uint8 s_receivebuf2[512];

CARD_SDIO_HANDLE  emmc_handle = NULL;

LOCAL CARD_PORT_T cardPort[CARD_SDIO_SLOT_MAX] = {0};

//-----------------------------------------------------------------------------------
//	To judge whether the handle is valid
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _IsCardHandleValid(CARD_SDIO_HANDLE cardHandle)
{
	uint32 i;

	for(i = 0; i < CARD_SDIO_SLOT_MAX; i++)
	{
		if(cardHandle == &cardPort[i])
		{
			if(TRUE == cardHandle->open_flag)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	return FALSE;

}


/*****************************************************************************/
//  Description:  Get Card operation handle,after we get the handle ,we can get next operation way of card
//  Author: Jason.wu
//  Param
//		slotNo: slot number
//  Return:
//		Not zero: success
//		zero : fail
//  Note: 
/*****************************************************************************/
PUBLIC CARD_SDIO_HANDLE CARD_SDIO_Open(CARD_SDIO_SLOT_NO slotNo)
{
	CARD_SDIO_ASSERT(slotNo < CARD_SDIO_SLOT_MAX);	/*assert verified*/

	if(TRUE == cardPort[slotNo].open_flag)
	{
		return NULL;
	}

	cardPort[slotNo].open_flag = TRUE;
	cardPort[slotNo].sdioPalHd = SDIO_Card_Pal_Open((SDIO_CARD_PAL_SLOT_E)slotNo);
	cardPort[slotNo].BlockLen = 0;
	cardPort[slotNo].RCA = 0x1000 | slotNo;
	cardPort[slotNo].bus_width = CARD_WIDTH_1_BIT;

	return &cardPort[slotNo];
}

/*****************************************************************************/
//  Description:  close operation handle of card. afer we free it ,other application can operate on this card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//  Return:
//		NONE
//  Note: 
/*****************************************************************************/
PUBLIC void CARD_SDIO_Close(CARD_SDIO_HANDLE cardHandle)
{
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	SDIO_Card_Pal_Close(cardHandle->sdioPalHd);
	cardHandle->open_flag = FALSE;
}

PUBLIC void CARD_SDIO_PwrCtl(CARD_SDIO_HANDLE cardHandle,BOOLEAN switchCtl)
{
	if(TRUE == switchCtl)
	{
		SDIO_Card_Pal_Pwr(cardHandle->sdioPalHd,SDIO_CARD_PAL_ON);
	}
	else if(FALSE == switchCtl)
	{
		SDIO_Card_Pal_Pwr(cardHandle->sdioPalHd,SDIO_CARD_PAL_OFF);
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
	}
}

//-----------------------------------------------------------------------------------
//	Analyze SD CID buffer
//-----------------------------------------------------------------------------------
LOCAL void _SD_CID_Analyze(uint8* CIDbuf,CARD_SD_CID_T *CID)
{
	uint16 tmp16;
	uint32 tmp32;

	CID->MID = CIDbuf[0];

	tmp16 = CIDbuf[1];
	tmp16 = tmp16<<8;
	CID->OID = tmp16 + CIDbuf[2];

	CID->PNM[0] = CIDbuf[3];
	CID->PNM[1] = CIDbuf[4];
	CID->PNM[2] = CIDbuf[5];
	CID->PNM[3] = CIDbuf[6];
	CID->PNM[4] = CIDbuf[7];

	CID->PRV = CIDbuf[8];

	tmp32 = CIDbuf[9];
	tmp32 = tmp32<<8;
	tmp32 += CIDbuf[10];
	tmp32 = tmp32<<8;
	tmp32 += CIDbuf[11];
	tmp32 = tmp32<<8;
	tmp32 += CIDbuf[12];
	CID->PSN = tmp32;

	tmp16 = CIDbuf[13]&0x0F;
	tmp16 = tmp16<<8;
	CID->MDT = tmp16 + CIDbuf[14];

//	CID->CRC = ((CIDbuf[15]&0xFE)>>1);
//	CID->lastBit = (CIDbuf[15]&0x01);
}

//-----------------------------------------------------------------------------------
//	Analyze MMC CID buffer
//-----------------------------------------------------------------------------------
LOCAL void _MMC_CID_Analyze(uint8* CIDbuf,CARD_MMC_CID_T *CID)
{
	uint16 tmp16;
	uint32 tmp32;

	CID->MID = CIDbuf[0];

	tmp16 = CIDbuf[1];
	tmp16 = tmp16<<8;
	CID->OID = tmp16 + CIDbuf[2];

	CID->PNM[0] = CIDbuf[3];
	CID->PNM[1] = CIDbuf[4];
	CID->PNM[2] = CIDbuf[5];
	CID->PNM[3] = CIDbuf[6];
	CID->PNM[4] = CIDbuf[7];
	CID->PNM[5] = CIDbuf[8];

	CID->PRV = CIDbuf[9];

	tmp32 = CIDbuf[10];
	tmp32 = tmp32<<8;
	tmp32 += CIDbuf[11];
	tmp32 = tmp32<<8;
	tmp32 += CIDbuf[12];
	tmp32 = tmp32<<8;
	tmp32 += CIDbuf[13];
	CID->PSN = tmp32;

	CID->MDT = CIDbuf[14];

//	CID->CRC = ((CIDbuf[15]&0xFE)>>1);
//	CID->lastBit = (CIDbuf[15]&0x01);
}


//-----------------------------------------------------------------------------------
//	Analyze SD CSD buffer
//
//	Expand SD CSD1.x to CSD struct member
//
//-----------------------------------------------------------------------------------
LOCAL void _SD_CSD10_Analyze(uint8* CSDbuf,CARD_SD_CSD10_T *CSD)
{
	uint8 tmp8;
	uint16 tmp16;

//R
	tmp8 = CSDbuf[0]&0xC0; //0b11000000
	CSD->CSD_STRUCTURE = tmp8>>6;	// 2 bit

	tmp8 = CSDbuf[0]&0x3F;//0b00111111;
	CSD->reserved1 = tmp8;			// 6 bit

	tmp8 = CSDbuf[1];
	CSD->TAAC = tmp8;			// 8 bit

	tmp8 = CSDbuf[2];
	CSD->NSAC = tmp8;			// 8 bit

	tmp8 = CSDbuf[3];
	CSD->TRAN_SPEED = tmp8;		// 8 bit

	tmp16 = CSDbuf[4];
	tmp16 = tmp16<<4;
	tmp8 = CSDbuf[5]&0xF0;//0b11110000;
	tmp8 = tmp8>>4;
	tmp16 += tmp8;
	CSD->CCC = tmp16;			//12 bit

	tmp8 = CSDbuf[5]&0x0F;//0b00001111;
	CSD->READ_BL_LEN = tmp8;		// 4 bit

	tmp8 = CSDbuf[6]&0x80;//0b10000000;
	CSD->READ_BL_PARTIAL = tmp8>>7;		// 1 bit

	tmp8 = CSDbuf[6]&0x40;//0b01000000;
	CSD->WRITE_BLK_MISALIGN = tmp8>>6;	// 1 bit

	tmp8 = CSDbuf[6]&0x20;//0b00100000;
	CSD->READ_BLK_MISALIGN = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[6]&0x10;//0b00010000;
	CSD->DSR_IMP = tmp8>>4;			// 1 bit

	tmp8 = CSDbuf[6]&0x0C;//0b00001100;
	CSD->reserved2 = tmp8>>2;			// 2 bit

	tmp16 = CSDbuf[6]&0x03;//0b00000011;
	tmp16 = tmp16<<8;
	tmp16 += CSDbuf[7];
	tmp16 = tmp16<<2;
	tmp8 = CSDbuf[8]&0xC0;//0b11000000;
	tmp8 = tmp8>>6;
	tmp16 = tmp16 + tmp8;
	CSD->C_SIZE = tmp16;				//12 bit

	tmp8 = CSDbuf[8]&0x38;//0b00111000;
	CSD->VDD_R_CURR_MIN = tmp8>>3;	// 3 bit

	tmp8 = CSDbuf[8]&0x07;//0b00000111;
	CSD->VDD_R_CURR_MAX = tmp8;		// 3 bit

	tmp8 = CSDbuf[9]&0xE0;//0b11100000;
	CSD->VDD_W_CURR_MIN = tmp8>>5;	// 3 bit

	tmp8 = CSDbuf[9]&0x1C;//0b00011100;
	CSD->VDD_W_CURR_MAX = tmp8>>2;	// 3 bit

	tmp8 = CSDbuf[9]&0x03;//0b00000011;
	tmp8 = tmp8<<1;
	tmp8 = tmp8 + ((CSDbuf[10]&0x80/*0b10000000*/)>>7);
	CSD->C_SIZE_MULT = tmp8;			// 3 bit
//--
	tmp8 = CSDbuf[10]&0x40;//0b01000000;
	CSD->ERASE_BLK_EN = tmp8>>6;	// 1 bit

	tmp8 = CSDbuf[10]&0x3F;//0b00111111;
	tmp8 = tmp8<<1;
	tmp8 = tmp8 + ((CSDbuf[11]&0x80/*0b10000000*/)>>7);
	CSD->SECTOR_SIZE = tmp8;		// 7 bit

	tmp8 = CSDbuf[11]&0x7F;//0b01111111;
	CSD->WP_GRP_SIZE = tmp8;			// 7 bit
//--
	tmp8 = CSDbuf[12]&0x80;//0b10000000;
	CSD->WP_GRP_ENABLE = tmp8>>7;	// 1 bit

	tmp8 = CSDbuf[12]&0x60;//0b01100000;
	CSD->reserved3 = tmp8>>5;		// 2 bit

	tmp8 = CSDbuf[12]&0x1C;//0b00011100;
	CSD->R2W_FACTOR = tmp8>>2;		// 3 bit

	tmp8 = CSDbuf[12]&0x03;//0b00000011;
	tmp8 = tmp8<<2;
	tmp8 = tmp8 + ((CSDbuf[13]&0xC0/*0b11000000*/)>>6);
	CSD->WRITE_BL_LEN = tmp8;		// 4 bit

	tmp8 = CSDbuf[13]&0x20;//0b00100000;
	CSD->WRITE_BL_PARTIAL = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[13]&0x1F;//0b00011111;
	CSD->reserved4 = tmp8;			// 5 bit
//RW
	tmp8 = CSDbuf[14]&0x80;//0b10000000;
	CSD->FILE_FORMAT_GRP = tmp8>>7;	// 1 bit

	tmp8 = CSDbuf[14]&0x40;//0b01000000;
	CSD->COPY = tmp8>>6;				// 1 bit

	tmp8 = CSDbuf[14]&0x20;//0b00100000;
	CSD->PERM_WRITE_PROTECT = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[14]&0x10;//0b00010000;
	CSD->TMP_WRITE_PROTECT = tmp8>>4;	// 1 bit

	tmp8 = CSDbuf[14]&0x0C;//0b00001100;
	CSD->FILE_FORMAT = tmp8>>2;		// 2 bit
	tmp8 = CSDbuf[14]&0x03;//0b00000011;
	CSD->reserved5 = tmp8;					// 2 bit

//	tmp8 = CSDbuf[15]&0xFE;//0b11111110;
//	CSD->CRC = tmp8>>1;				// 7 bit

//	tmp8 = CSDbuf[15]&0x01;//0b00000001;
//	CSD->LastBit = tmp8;				// 1 bit: always 1
}


//-----------------------------------------------------------------------------------
//	Analyze SD CSD buffer
//
//	Expand SD CSD2.0 to CSD struct member
//
//-----------------------------------------------------------------------------------
LOCAL void _SD_CSD20_Analyze(uint8* CSDbuf,CARD_SD_CSD20_T *CSD)
{
	uint8 tmp8;
	uint16 tmp16;
	uint32 tmp32;

//R
	tmp8 = CSDbuf[0]&0xC0; //0b11000000
	CSD->CSD_STRUCTURE = tmp8>>6;	// 2 bit

	tmp8 = CSDbuf[0]&0x3F;//0b00111111;
	CSD->reserved1 = tmp8;			// 6 bit

	tmp8 = CSDbuf[1];
	CSD->TAAC = tmp8;			// 8 bit

	tmp8 = CSDbuf[2];
	CSD->NSAC = tmp8;			// 8 bit

	tmp8 = CSDbuf[3];
	CSD->TRAN_SPEED = tmp8;		// 8 bit

	tmp16 = CSDbuf[4];
	tmp16 = tmp16<<4;
	tmp8 = CSDbuf[5]&0xF0;//0b11110000;
	tmp8 = tmp8>>4;
	tmp16 += tmp8;
	CSD->CCC = tmp16;			//12 bit

	tmp8 = CSDbuf[5]&0x0F;//0b00001111;
	CSD->READ_BL_LEN = tmp8;		// 4 bit

	tmp8 = CSDbuf[6]&0x80;//0b10000000;
	CSD->READ_BL_PARTIAL = tmp8>>7;		// 1 bit

	tmp8 = CSDbuf[6]&0x40;//0b01000000;
	CSD->WRITE_BLK_MISALIGN = tmp8>>6;	// 1 bit

	tmp8 = CSDbuf[6]&0x20;//0b00100000;
	CSD->READ_BLK_MISALIGN = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[6]&0x10;//0b00010000;
	CSD->DSR_IMP = tmp8>>4;			// 1 bit
//--
	tmp8 = CSDbuf[6]&0x0F;//0b00001111;
	tmp8 = tmp8<<2;
	tmp8 += ((CSDbuf[7]&0xC0/*0b11000000*/)>>6);
	CSD->reserved2 = tmp8;				// 6 bit

	tmp32 = CSDbuf[7]&0x3F;//0b00111111
	tmp32 = tmp32<<8;
	tmp32 += CSDbuf[8];
	tmp32 = tmp32<<8;
	tmp32 += CSDbuf[9];
	CSD->C_SIZE = tmp32;				// 22 bit

	tmp8 = CSDbuf[10]&0x80;//0b10000000
	tmp8 = tmp8>>7;
	CSD->reserved3 = tmp8;
//--
	tmp8 = CSDbuf[10]&0x40;//0b01000000;
	CSD->ERASE_BLK_EN = tmp8>>6;	// 1 bit

	tmp8 = CSDbuf[10]&0x3F;//0b00111111;
	tmp8 = tmp8<<1;
	tmp8 = tmp8 + ((CSDbuf[11]&0x80/*0b10000000*/)>>7);
	CSD->SECTOR_SIZE = tmp8;		// 7 bit

	tmp8 = CSDbuf[11]&0x7F;//0b01111111;
	CSD->WP_GRP_SIZE = tmp8;			// 7 bit
//--
	tmp8 = CSDbuf[12]&0x80;//0b10000000;
	CSD->WP_GRP_ENABLE = tmp8>>7;	// 1 bit

	tmp8 = CSDbuf[12]&0x60;//0b01100000;
	CSD->reserved4 = tmp8>>5;		// 2 bit

	tmp8 = CSDbuf[12]&0x1C;//0b00011100;
	CSD->R2W_FACTOR = tmp8>>2;		// 3 bit

	tmp8 = CSDbuf[12]&0x03;//0b00000011;
	tmp8 = tmp8<<2;
	tmp8 = tmp8 + ((CSDbuf[13]&0xC0/*0b11000000*/)>>6);
	CSD->WRITE_BL_LEN = tmp8;		// 4 bit

	tmp8 = CSDbuf[13]&0x20;//0b00100000;
	CSD->WRITE_BL_PARTIAL = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[13]&0x1F;//0b00011111;
	CSD->reserved5 = tmp8;			// 5 bit
//RW
	tmp8 = CSDbuf[14]&0x80;//0b10000000;
	CSD->FILE_FORMAT_GRP = tmp8>>7;	// 1 bit

	tmp8 = CSDbuf[14]&0x40;//0b01000000;
	CSD->COPY = tmp8>>6;				// 1 bit

	tmp8 = CSDbuf[14]&0x20;//0b00100000;
	CSD->PERM_WRITE_PROTECT = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[14]&0x10;//0b00010000;
	CSD->TMP_WRITE_PROTECT = tmp8>>4;	// 1 bit

	tmp8 = CSDbuf[14]&0x0C;//0b00001100;
	CSD->FILE_FORMAT = tmp8>>2;		// 2 bit
	tmp8 = CSDbuf[14]&0x03;//0b00000011;
	CSD->reserved6 = tmp8;					// 2 bit

//	tmp8 = CSDbuf[15]&0xFE;//0b11111110;
//	CSD->CRC = tmp8>>1;				// 7 bit

//	tmp8 = CSDbuf[15]&0x01;//0b00000001;
//	CSD->LastBit = tmp8;				// 1 bit: always 1
}

//-----------------------------------------------------------------------------------
//	Analyze MMC CSD buffer
//
//	Expand MMC CSD to CSD struct member
//
//-----------------------------------------------------------------------------------
LOCAL void _MMC_CSD_Analyze(uint8* CSDbuf,CARD_MMC_CSD_T *CSD)
{
	uint8 tmp8;
	uint16 tmp16;

//R
	tmp8 = CSDbuf[0]&0xC0; //0b11000000
	CSD->CSD_STRUCTURE = tmp8>>6;	// 2 bit

	tmp8 = CSDbuf[0]&0x3C;//0b00111100;
	CSD->SPEC_VERS = tmp8>>2;	// 4 bit

	tmp8 = CSDbuf[0]&0x03;//0b00000011;
	CSD->reserved1 = tmp8;			// 2 bit

	tmp8 = CSDbuf[1];
	CSD->TAAC = tmp8;			// 8 bit

	tmp8 = CSDbuf[2];
	CSD->NSAC = tmp8;			// 8 bit

	tmp8 = CSDbuf[3];
	CSD->TRAN_SPEED = tmp8;		// 8 bit

	tmp16 = CSDbuf[4];
	tmp16 = tmp16<<4;
	tmp8 = CSDbuf[5]&0xF0;//0b11110000;
	tmp8 = tmp8>>4;
	tmp16 += tmp8;
	CSD->CCC = tmp16;			//12 bit

	tmp8 = CSDbuf[5]&0x0F;//0b00001111;
	CSD->READ_BL_LEN = tmp8;		// 4 bit

	tmp8 = CSDbuf[6]&0x80;//0b10000000;
	CSD->READ_BL_PARTIAL = tmp8>>7;		// 1 bit

	tmp8 = CSDbuf[6]&0x40;//0b01000000;
	CSD->WRITE_BLK_MISALIGN = tmp8>>6;	// 1 bit

	tmp8 = CSDbuf[6]&0x20;//0b00100000;
	CSD->READ_BLK_MISALIGN = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[6]&0x10;//0b00010000;
	CSD->DSR_IMP = tmp8>>4;			// 1 bit

	tmp8 = CSDbuf[6]&0x0C;//0b00001100;
	CSD->reserved2 = tmp8>>2;			// 2 bit

	tmp16 = CSDbuf[6]&0x03;//0b00000011;
	tmp16 = tmp16<<8;
	tmp16 += CSDbuf[7];
	tmp16 = tmp16<<2;
	tmp8 = CSDbuf[8]&0xC0;//0b11000000;
	tmp8 = tmp8>>6;
	tmp16 = tmp16 + tmp8;
	CSD->C_SIZE = tmp16;				//12 bit

	tmp8 = CSDbuf[8]&0x38;//0b00111000;
	CSD->VDD_R_CURR_MIN = tmp8>>3;	// 3 bit

	tmp8 = CSDbuf[8]&0x07;//0b00000111;
	CSD->VDD_R_CURR_MAX = tmp8;		// 3 bit

	tmp8 = CSDbuf[9]&0xE0;//0b11100000;
	CSD->VDD_W_CURR_MIN = tmp8>>5;	// 3 bit

	tmp8 = CSDbuf[9]&0x1C;//0b00011100;
	CSD->VDD_W_CURR_MAX = tmp8>>2;	// 3 bit

	tmp8 = CSDbuf[9]&0x03;//0b00000011;
	tmp8 = tmp8<<1;
	tmp8 = tmp8 + ((CSDbuf[10]&0x80/*0b10000000*/)>>7);
	CSD->C_SIZE_MULT = tmp8;			// 3 bit

	tmp8 = CSDbuf[10]&0x7C;//0b01111100;
	CSD->ERASE_GRP_SIZE = tmp8>>2;	// 5 bit

	tmp8 = CSDbuf[10]&0x03;//0b00000011;
	tmp8 = tmp8<<3;
	tmp8 = tmp8 + ((CSDbuf[11]&0xE0/*0b11100000*/)>>5);
	CSD->ERASE_GRP_MULT = tmp8;		// 5 bit

	tmp8 = CSDbuf[11]&0x1F;//0b00011111;
	CSD->WP_GRP_SIZE = tmp8;			// 5 bit

	tmp8 = CSDbuf[12]&0x80;//0b10000000;
	CSD->WP_GRP_ENABLE = tmp8>>7;	// 1 bit

	tmp8 = CSDbuf[12]&0x60;//0b01100000;
	CSD->DEFAULT_ECC = tmp8>>5;		// 2 bit

	tmp8 = CSDbuf[12]&0x1C;//0b00011100;
	CSD->R2W_FACTOR = tmp8>>2;		// 3 bit

	tmp8 = CSDbuf[12]&0x03;//0b00000011;
	tmp8 = tmp8<<2;
	tmp8 = tmp8 + ((CSDbuf[13]&0xC0/*0b11000000*/)>>6);
	CSD->WRITE_BL_LEN = tmp8;		// 4 bit

	tmp8 = CSDbuf[13]&0x20;//0b00100000;
	CSD->WRITE_BL_PARTIAL = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[13]&0x1E;//0b00011110;
	CSD->reserved3 = tmp8>>1;			// 4 bit

	tmp8 = CSDbuf[13]&0x01;//0b00000001;
	CSD->CONTENT_PROT_APP = tmp8;	// 1 bit
//RW
	tmp8 = CSDbuf[14]&0x80;//0b10000000;
	CSD->FILE_FORMAT_GRP = tmp8>>7;	// 1 bit

	tmp8 = CSDbuf[14]&0x40;//0b01000000;
	CSD->COPY = tmp8>>6;				// 1 bit

	tmp8 = CSDbuf[14]&0x20;//0b00100000;
	CSD->PERM_WRITE_PROTECT = tmp8>>5;	// 1 bit

	tmp8 = CSDbuf[14]&0x10;//0b00010000;
	CSD->TMP_WRITE_PROTECT = tmp8>>4;	// 1 bit

	tmp8 = CSDbuf[14]&0x0C;//0b00001100;
	CSD->FILE_FORMAT = tmp8>>2;		// 2 bit
	tmp8 = CSDbuf[14]&0x03;//0b00000011;
	CSD->ECC = tmp8;					// 2 bit

//	tmp8 = CSDbuf[15]&0xFE;//0b11111110;
//	CSD->CRC = tmp8>>1;				// 7 bit

//	tmp8 = CSDbuf[15]&0x01;//0b00000001;
//	CSD->LastBit = tmp8;				// 1 bit: always 1
}



/*****************************************************************************/
//  Description:  Read CSD register and analyze it
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		CSD: CSD data will be stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CARD_SDIO_ReadCSD(CARD_SDIO_HANDLE cardHandle,CARD_CSD_T* CSD)
{
	uint8 rspBuf[16];
	uint16 RCA = cardHandle->RCA;
	uint32 argument = 0;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/
	cardHandle->CCC = 0;

	argument = RCA;
	argument = argument<<16;


	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD9_SEND_CSD,argument,NULL,rspBuf))
	{
		return FALSE;
	}
//--
	CARD_SDIO_PRINT(("SD20 %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3],rspBuf[4],rspBuf[5],rspBuf[6],rspBuf[7],rspBuf[8],rspBuf[9],rspBuf[10],rspBuf[11],rspBuf[12],rspBuf[13],rspBuf[14]));

 if((CARD_MMC_441_STANDARD == cardHandle->vertion)||(CARD_MMC_441_HIGHCAP == cardHandle->vertion))
	{
		_MMC_CSD_Analyze(rspBuf,&(CSD->MMC_CSD));
		cardHandle->CCC = CSD->MMC_CSD.CCC;
		cardHandle->CCC &= (~((1<<10)|(1<<11)));	// for MMC mode This two classes is reserved

		CARD_SDIO_PRINT(("CARD_MMC_331:"));
		CARD_SDIO_PRINT(("CSD_STRUCTURE\t\t  = 0x%x",CSD->MMC_CSD.CSD_STRUCTURE));
		CARD_SDIO_PRINT(("SPEC_VERS\t\t  = 0x%x",CSD->MMC_CSD.SPEC_VERS));
		CARD_SDIO_PRINT(("reserved1\t\t  = 0x%x",CSD->MMC_CSD.reserved1));
		CARD_SDIO_PRINT(("TAAC\t\t\t  = 0x%x",CSD->MMC_CSD.TAAC));
		CARD_SDIO_PRINT(("NSAC\t\t\t  = 0x%x",CSD->MMC_CSD.NSAC));
		CARD_SDIO_PRINT(("TRAN_SPEED\t\t  = 0x%x",CSD->MMC_CSD.TRAN_SPEED));
		CARD_SDIO_PRINT(("CCC\t\t\t  = 0x%x",CSD->MMC_CSD.CCC));
		CARD_SDIO_PRINT(("READ_BL_LEN\t\t  = 0x%x",CSD->MMC_CSD.READ_BL_LEN));
		CARD_SDIO_PRINT(("READ_BL_PARTIAL\t\t  = 0x%x",CSD->MMC_CSD.READ_BL_PARTIAL));
		CARD_SDIO_PRINT(("WRITE_BLK_MISALIGN\t  = 0x%x",CSD->MMC_CSD.WRITE_BLK_MISALIGN));
		CARD_SDIO_PRINT(("READ_BLK_MISALIGN\t  = 0x%x",CSD->MMC_CSD.READ_BLK_MISALIGN));
		CARD_SDIO_PRINT(("DSR_IMP\t\t\t  = 0x%x",CSD->MMC_CSD.DSR_IMP));
		CARD_SDIO_PRINT(("reserved2\t\t  = 0x%x",CSD->MMC_CSD.reserved2));
		CARD_SDIO_PRINT(("C_SIZE\t\t\t  = 0x%x",CSD->MMC_CSD.C_SIZE));
		CARD_SDIO_PRINT(("VDD_R_CURR_MIN\t\t  = 0x%x",CSD->MMC_CSD.VDD_R_CURR_MIN));
		CARD_SDIO_PRINT(("VDD_R_CURR_MAX\t\t  = 0x%x",CSD->MMC_CSD.VDD_R_CURR_MAX));
		CARD_SDIO_PRINT(("VDD_W_CURR_MIN\t\t  = 0x%x",CSD->MMC_CSD.VDD_W_CURR_MIN));
		CARD_SDIO_PRINT(("VDD_W_CURR_MAX\t\t  = 0x%x",CSD->MMC_CSD.VDD_W_CURR_MAX));
		CARD_SDIO_PRINT(("C_SIZE_MULT\t\t  = 0x%x",CSD->MMC_CSD.C_SIZE_MULT));
		CARD_SDIO_PRINT(("ERASE_GRP_SIZE\t\t  = 0x%x",CSD->MMC_CSD.ERASE_GRP_SIZE));
		CARD_SDIO_PRINT(("ERASE_GRP_MULT\t\t  = 0x%x",CSD->MMC_CSD.ERASE_GRP_MULT));
		CARD_SDIO_PRINT(("WP_GRP_SIZE\t\t  = 0x%x",CSD->MMC_CSD.WP_GRP_SIZE));
		CARD_SDIO_PRINT(("WP_GRP_ENABLE\t\t  = 0x%x",CSD->MMC_CSD.WP_GRP_ENABLE));
		CARD_SDIO_PRINT(("DEFAULT_ECC\t\t  = 0x%x",CSD->MMC_CSD.DEFAULT_ECC));
		CARD_SDIO_PRINT(("R2W_FACTOR\t\t  = 0x%x",CSD->MMC_CSD.R2W_FACTOR));
		CARD_SDIO_PRINT(("WRITE_BL_LEN\t\t  = 0x%x",CSD->MMC_CSD.WRITE_BL_LEN));
		CARD_SDIO_PRINT(("WRITE_BL_PARTIAL\t  = 0x%x",CSD->MMC_CSD.WRITE_BL_PARTIAL));
		CARD_SDIO_PRINT(("reserved3\t\t  = 0x%x",CSD->MMC_CSD.reserved3));
		CARD_SDIO_PRINT(("CONTENT_PROT_APP\t  = 0x%x",CSD->MMC_CSD.CONTENT_PROT_APP));
		CARD_SDIO_PRINT(("----RW bit---"));
		CARD_SDIO_PRINT(("FILE_FORMAT_GRP\t\t  = 0x%x",CSD->MMC_CSD.FILE_FORMAT_GRP));
		CARD_SDIO_PRINT(("COPY\t\t\t  = 0x%x",CSD->MMC_CSD.COPY));
		CARD_SDIO_PRINT(("PERM_WRITE_PROTECT\t  = 0x%x",CSD->MMC_CSD.PERM_WRITE_PROTECT));
		CARD_SDIO_PRINT(("TMP_WRITE_PROTECT\t  = 0x%x",CSD->MMC_CSD.TMP_WRITE_PROTECT));
		CARD_SDIO_PRINT(("FILE_FORMAT\t\t  = 0x%x",CSD->MMC_CSD.FILE_FORMAT));
		CARD_SDIO_PRINT(("ECC\t\t\t  = 0x%x",CSD->MMC_CSD.ECC));
//		CARD_SDIO_PRINT(("CRC\t\t\t  = 0x%x",CSD->MMC_CSD.CRC));
//		CARD_SDIO_PRINT(("LastBit\t\t\t  = 0x%x",CSD->MMC_CSD.LastBit));

	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
	}

	return TRUE;
}


/*****************************************************************************/
//  Description:  Read EXT CSD register and analyze it
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		CSD: CSD data will be stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CARD_SDIO_ReadExtCSD(CARD_SDIO_HANDLE cardHandle)
{
          uint8 rspBuf[16];
	
	uint32 argument = 0;
         CARD_DATA_PARAM_T data;
         
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/
	cardHandle->CCC = 0;

         data.blkLen = 512;
	data.blkNum = 1;
	data.databuf = s_extcsdbuf;
	data.direction = SDIO_DMA_IN;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD8_SEND_IF_COND,argument,&data,rspBuf))
	{
		return FALSE;
	}
	
	return TRUE;
}


/*****************************************************************************/
//  Description:  Read CID register and analyze it
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		CID: CID data will be stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
uint8 CARD_SDIO_ReadCID(CARD_SDIO_HANDLE cardHandle,CARD_CID_T* CID)
{
	uint8 rspBuf[16];

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD2_ALL_SEND_CID,0,NULL,rspBuf))
	{
		return 0;
	}
    
        if(CARD_MMC_441_STANDARD == cardHandle->vertion)
	{
		_MMC_CID_Analyze(rspBuf,&CID->MMC_CID);
		CARD_SDIO_PRINT(("-----MMC CID value---\r\n"));
		CARD_SDIO_PRINT(("MID\t\t = 0x%x\r\n",CID->MMC_CID.MID));
		CARD_SDIO_PRINT(("OID\t\t = 0x%x\r\n",CID->MMC_CID.OID));
		CARD_SDIO_PRINT(("PNM\t\t = %c,%c,%c,%c,%c,%c\r\n",CID->MMC_CID.PNM[0],CID->MMC_CID.PNM[1],CID->MMC_CID.PNM[2],CID->MMC_CID.PNM[3],CID->MMC_CID.PNM[4],CID->MMC_CID.PNM[5]));
		CARD_SDIO_PRINT(("PRV\t\t = 0x%x\r\n",CID->MMC_CID.PRV));
		CARD_SDIO_PRINT(("PSN\t\t = 0x%x\r\n",CID->MMC_CID.PSN));
		CARD_SDIO_PRINT(("MDT\t\t = 0x%x\r\n",CID->MMC_CID.MDT));
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
	}

	return 1;
}

/*****************************************************************************/
//  Description:  Set relate address to card ,this address is used to select card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		RCA: new relative address
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CARD_SDIO_SetRCA(CARD_SDIO_HANDLE cardHandle,uint16 RCA)
{
	uint8 rspBuf[16];
	uint32 argument = 0;

	if(CARD_MMC_331 != cardHandle->vertion)
	{
	// only MMC card can set RCA
		return FALSE;
	}

	argument = RCA;
	argument = argument<<16;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD3_SET_RELATIVE_ADDR,argument,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;

}

/*****************************************************************************/
//  Description:  Get relate address from card ,this address is used to select card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		RCA: relative address will stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CARD_SDIO_ReadRCA(CARD_SDIO_HANDLE cardHandle,uint16*RCA)
{
	uint8 rspBuf[16];

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/


	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD3_SEND_RELATIVE_ADDR,1<<16,NULL,rspBuf))
	{
		return FALSE;
	}

	*RCA = 1;
	return TRUE;
}

//-----------------------------------------------------------------------------------
//	Before operate card ,we must active card first,this function is used to active card
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _SelectCard(CARD_SDIO_HANDLE cardHandle)
{
	uint8 rspBuf[16];
	uint16 RCA = cardHandle->RCA;
	uint32 argument = 0;

	argument = RCA;
	argument = argument<<16;

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD7_SELECT_DESELECT_CARD,argument,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
}

LOCAL BOOLEAN MMC_SWITCH(CARD_SDIO_HANDLE cardHandle, uint8 index, uint8 value)
{
	uint8 rspBuf[16];
	uint32 argument = 0;

	argument = CMD6_ACCESS_MODE_WRITE_BYTE | CMD6_CMD_SET;
	argument |= index<<CMD6_BIT_MODE_OFFSET_INDEX;
	argument |= value<<CMD6_BIT_MODE_OFFSET_VALUE;
	

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_ACMD6_SET_EXT_CSD,argument,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
}

BOOLEAN CARD_SDIO_Select_CurPartition(CARD_SDIO_HANDLE cardHandle, CARD_EMMC_PARTITION_TPYE cardPartiton)
{
	uint8 rspBuf[16];
	uint16 RCA = cardHandle->RCA;
	uint32 argument = 0;

	argument = CMD6_ACCESS_MODE_WRITE_BYTE | ( EXT_CSD_PARTITION_CFG_INDEX<<CMD6_BIT_MODE_OFFSET_INDEX & CMD6_BIT_MODE_MASK_INDEX) | CMD6_CMD_SET;
	
	switch(cardPartiton)
	{
		case PARTITION_USER:
			argument |= (PARTITION_CFG_BOOT_PARTITION_ENABLE_BOOT1 |PARTITION_CFG_BOOT_PARTITION_ACCESS_PARTITION_USER)<<CMD6_BIT_MODE_OFFSET_VALUE;
			break;
			
		case PARTITION_BOOT1:
			argument |= (PARTITION_CFG_BOOT_PARTITION_ENABLE_BOOT1 |PARTITION_CFG_BOOT_PARTITION_ACCESS_PARTITION_BOOT1)<<CMD6_BIT_MODE_OFFSET_VALUE;
			break;
				
		case PARTITION_BOOT2:
			argument |= (PARTITION_CFG_BOOT_PARTITION_ENABLE_BOOT1 |PARTITION_CFG_BOOT_PARTITION_ACCESS_PARTITION_BOOT2)<<CMD6_BIT_MODE_OFFSET_VALUE;
			break;
			
		case PARTITION_RPMB:
		case PARTITION_GENERAL_P1:
		case PARTITION_GENERAL_P2:
		case PARTITION_GENERAL_P3:
		case PARTITION_GENERAL_P4:
			CARD_SDIO_ASSERT(0);	//if want to support RPMB or GENERAL Partiton, need add code
			break;
		default:
			CARD_SDIO_ASSERT(0);	/*assert verified*/
			break;

	}	

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_ACMD6_SET_EXT_CSD,argument,NULL,rspBuf))
	{
		return FALSE;
	}
	
	cardHandle->Cur_Partition = cardPartiton;
	return TRUE;

}

//-----------------------------------------------------------------------------------
//	Set data bus width of card
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _SetBusWidth(CARD_SDIO_HANDLE cardHandle,CARD_BUS_WIDTH_E width)
{
	uint8 rspBuf[16];
	uint16 RCA = cardHandle->RCA;
	uint32 argument = 0;

	argument = CMD6_ACCESS_MODE_WRITE_BYTE | ( EXT_CSD_BUS_WIDTH_INDEX<<CMD6_BIT_MODE_OFFSET_INDEX & CMD6_BIT_MODE_MASK_INDEX) ;

	switch(width)
	{
		case CARD_WIDTH_1_BIT:
		{
			argument |= EXT_CSD_CARD_WIDTH_1_BIT<<CMD6_BIT_MODE_OFFSET_VALUE;
		}
		break;

		case CARD_WIDTH_4_BIT:
		{
			argument |=EXT_CSD_CARD_WIDTH_4_BIT<<CMD6_BIT_MODE_OFFSET_VALUE;
		}
		break;
		case CARD_WIDTH_8_BIT:
		{
			argument |=EXT_CSD_CARD_WIDTH_8_BIT<<CMD6_BIT_MODE_OFFSET_VALUE;
		}
		break;

		default:
		{
			CARD_SDIO_ASSERT(0);	/*assert verified*/
		}
		break;
	}
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_ACMD6_SET_EXT_CSD,argument,NULL,rspBuf))
	{
		return FALSE;
	}

	switch(width)
	{
		case CARD_WIDTH_1_BIT:
		{
			SDIO_Card_Pal_SetBusWidth(cardHandle->sdioPalHd,SDIO_CARD_PAL_1_BIT);
		}
		break;

		case CARD_WIDTH_4_BIT:
		{
			SDIO_Card_Pal_SetBusWidth(cardHandle->sdioPalHd,SDIO_CARD_PAL_4_BIT);
		}
		break;
#if defined (CONFIG_SC8825) || defined(CONFIG_SC7710G2)
		case CARD_WIDTH_8_BIT:
		{
			SDIO_Card_Pal_SetBusWidth(cardHandle->sdioPalHd,SDIO_CARD_PAL_8_BIT);
		}
		break;
#endif
		default:
		{
			CARD_SDIO_ASSERT(0);	/*assert verified*/
		}
		break;
	}
	cardHandle->bus_width = width;

	return TRUE;
}

/*****************************************************************************/
//  Description:  Set block length of card.  the data will be transmited in block unit
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		length: block length
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_SetBlockLength(CARD_SDIO_HANDLE cardHandle,uint32 length)
{
	uint8 rspBuf[16];

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if((CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)&&(512 != length))
	{
		CARD_SDIO_PRINT(("SD20 Block Length of SD V2.0 High capacity is fixed to 512,you cannot set to value 0x%x\r\n",length));
		return FALSE;
	}
	if(length == cardHandle->BlockLen)
	{
		return TRUE;
	}
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD16_SET_BLOCKLEN,length,NULL,rspBuf))
	{
		return FALSE;
	}

	cardHandle->BlockLen = length;

	return TRUE;
}



/*

*/


typedef struct
{
	uint16	currentVal;
	uint16	grp6Supported;
	uint16	grp5Supported;
	uint16	grp4Supported;
	uint16	grp3Supported;
	uint16	grp2Supported;
	uint16	grp1Supported;

	uint8	grp6SwitchResult;
	uint8	grp5SwitchResult;
	uint8	grp4SwitchResult;
	uint8	grp3SwitchResult;
	uint8	grp2SwitchResult;
	uint8	grp1SwitchResult;

	uint8	version;

	uint16	grp6BusyStatus;
	uint16	grp5BusyStatus;
	uint16	grp4BusyStatus;
	uint16	grp3BusyStatus;
	uint16	grp2BusyStatus;
	uint16	grp1BusyStatus;

}CARD_CMD6_STATUS_T;


LOCAL void _CMD6_Response_Analyze(uint8* resBuf,CARD_CMD6_STATUS_T* cmdStatus)
{
	cmdStatus->currentVal		= ((resBuf[0]<<8)|(resBuf[1]));
	cmdStatus->grp6Supported	= ((resBuf[2]<<8)|(resBuf[3]));
	cmdStatus->grp5Supported	= ((resBuf[4]<<8)|(resBuf[5]));
	cmdStatus->grp4Supported	= ((resBuf[6]<<8)|(resBuf[7]));
	cmdStatus->grp3Supported	= ((resBuf[8]<<8)|(resBuf[9]));
	cmdStatus->grp2Supported	= ((resBuf[10]<<8)|(resBuf[11]));
	cmdStatus->grp1Supported	= ((resBuf[12]<<8)|(resBuf[13]));

	cmdStatus->grp6SwitchResult	= ((resBuf[14]&0xF0)>>4);
	cmdStatus->grp5SwitchResult	= (resBuf[14]&0x0F);
	cmdStatus->grp4SwitchResult	= ((resBuf[15]&0xF0)>>4);
	cmdStatus->grp3SwitchResult	= (resBuf[15]&0x0F);
	cmdStatus->grp2SwitchResult	= ((resBuf[16]&0xF0)>>4);
	cmdStatus->grp1SwitchResult	= (resBuf[16]&0x0F);

	cmdStatus->version		= resBuf[17];

	cmdStatus->grp6BusyStatus	= ((resBuf[18]<<8)|(resBuf[19]));
	cmdStatus->grp5BusyStatus	= ((resBuf[20]<<8)|(resBuf[21]));
	cmdStatus->grp4BusyStatus	= ((resBuf[22]<<8)|(resBuf[23]));
	cmdStatus->grp3BusyStatus	= ((resBuf[24]<<8)|(resBuf[25]));
	cmdStatus->grp2BusyStatus	= ((resBuf[26]<<8)|(resBuf[27]));
	cmdStatus->grp1BusyStatus	= ((resBuf[28]<<8)|(resBuf[29]));

	CARD_SDIO_PRINT(("currentVal = 0x%x",cmdStatus->currentVal));

	CARD_SDIO_PRINT(("grp6Supported = 0x%x",cmdStatus->grp6Supported));
	CARD_SDIO_PRINT(("grp5Supported = 0x%x",cmdStatus->grp5Supported));
	CARD_SDIO_PRINT(("grp4Supported = 0x%x",cmdStatus->grp4Supported));
	CARD_SDIO_PRINT(("grp3Supported = 0x%x",cmdStatus->grp3Supported));
	CARD_SDIO_PRINT(("grp2Supported = 0x%x",cmdStatus->grp2Supported));
	CARD_SDIO_PRINT(("grp1Supported = 0x%x",cmdStatus->grp1Supported));

	CARD_SDIO_PRINT(("grp6SwitchResult = 0x%x",cmdStatus->grp6SwitchResult));
	CARD_SDIO_PRINT(("grp5SwitchResult = 0x%x",cmdStatus->grp5SwitchResult));
	CARD_SDIO_PRINT(("grp4SwitchResult = 0x%x",cmdStatus->grp4SwitchResult));
	CARD_SDIO_PRINT(("grp3SwitchResult = 0x%x",cmdStatus->grp3SwitchResult));
	CARD_SDIO_PRINT(("grp2SwitchResult = 0x%x",cmdStatus->grp2SwitchResult));
	CARD_SDIO_PRINT(("grp1SwitchResult = 0x%x",cmdStatus->grp1SwitchResult));

	CARD_SDIO_PRINT(("status version = 0x%x",cmdStatus->version));

	CARD_SDIO_PRINT(("grp6BusyStatus = 0x%x",cmdStatus->grp6BusyStatus));
	CARD_SDIO_PRINT(("grp5BusyStatus = 0x%x",cmdStatus->grp5BusyStatus));
	CARD_SDIO_PRINT(("grp4BusyStatus = 0x%x",cmdStatus->grp4BusyStatus));
	CARD_SDIO_PRINT(("grp3BusyStatus = 0x%x",cmdStatus->grp3BusyStatus));
	CARD_SDIO_PRINT(("grp2BusyStatus = 0x%x",cmdStatus->grp2BusyStatus));
	CARD_SDIO_PRINT(("grp1BusyStatus = 0x%x",cmdStatus->grp1BusyStatus));

	return;
}

#define EMMC_TEST
#define SECTOR_MODE 0x40000000 
#define BYTE_MODE   0x00000000 
#ifdef EMMC_TEST
PUBLIC BOOLEAN CARD_SDIO_InitCard(CARD_SDIO_HANDLE cardHandle, CARD_SPEED_MODE speedmode)
{
	uint8 rspBuf[16];
	uint32 pre_tick, cur_tick;
	CARD_CID_T CID;
	CARD_CSD_T CSD;
	CARD_BUS_WIDTH_E busWidth = CARD_WIDTH_1_BIT;
	uint16 RCA;
	BOOLEAN vertion_flag; //TRUE: SD2.0 FALSE: SD1.x
	SDIO_CARD_PAL_ERROR_E errCode;

	uint8 extcsd_Part_Config; 

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

//	CARD_SDIO_PwrCtl(cardHandle,TRUE);
	cardHandle->bus_width = CARD_WIDTH_1_BIT;
	cardHandle->BlockLen = 0;
	cardHandle->vertion = CARD_V_UNKONWN;
	cardHandle->Cur_Partition =  PARTITION_MAX;

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD0_GO_IDLE_STATE,0,NULL,rspBuf))
	{
		return FALSE;
	}
//Now the card is in Idle State
    if(CARD_V_UNKONWN== cardHandle->vertion)
    {
           pre_tick = SCI_GetTickCount(); /*set start tick value*/       
    	do
    	{
    		if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD1_SEND_OP_COND,0x00FF8000 | SECTOR_MODE,NULL,rspBuf))
    		{
    			//return FALSE;
    		}
                  //break;            
    		if(0 != (rspBuf[0]&BIT_7))
    		{
    			cardHandle->vertion = CARD_MMC_441_STANDARD;
    			break;
    		}

    		cur_tick = SCI_GetTickCount();
    		if(CARD_CMD_MAX_TIME <  (cur_tick - pre_tick))
    		{
    			/*cmd time out, return false*/
    			return FALSE;
    		} 
            
    	}
    	while(1); /*lint !e506*/
    }

// Now SD is in Ready State
    if(1 != CARD_SDIO_ReadCID(cardHandle,&CID))
	{
		return FALSE;
	}
// Now SD is in Identification State
    if(FALSE == CARD_SDIO_ReadRCA(cardHandle,&RCA))
    {
    	return FALSE;
    }
    cardHandle->RCA = 1;
    if(FALSE == CARD_SDIO_ReadCSD(cardHandle,&CSD))
	{
		return FALSE;
	}
     if(!(CARD_MMC_441_STANDARD == cardHandle->vertion))
	{
		return FALSE;
	}
	if(0xFFF != CSD.MMC_CSD.C_SIZE)
	{
		cardHandle->GrpSize = (CSD.MMC_CSD.ERASE_GRP_SIZE+1)*(CSD.MMC_CSD.ERASE_GRP_MULT+1)*(1 << CSD.MMC_CSD.WRITE_BL_LEN);
		cardHandle->Nac_Max = (uint32)((((CSD.MMC_CSD.TAAC*cardHandle->ClockRate) + (100*CSD.MMC_CSD.NSAC))*10) / 8);
		cardHandle->Capacity = (CSD.MMC_CSD.C_SIZE+1) <<(CSD.MMC_CSD.C_SIZE_MULT+2 + CSD.MMC_CSD.READ_BL_LEN - 9); //unit is 512 byte

	}
	else
	{
	    	cardHandle->vertion = CARD_MMC_441_HIGHCAP;
	}

        if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	{
		return FALSE;
	}
        
        if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD7_SELECT_DESELECT_CARD,  1<<16,NULL,rspBuf))
           {
                   return FALSE;
           }
         if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	{
		return FALSE;
	}

       //get card  Capacity and RPMB Capacity
       //(void)memset(s_extcsdbuf, 0x0, 512);
       CARD_SDIO_ReadExtCSD(cardHandle);
      if(CARD_MMC_441_HIGHCAP == cardHandle->vertion)
	{
	cardHandle->Capacity = (((uint32)s_extcsdbuf[215])<<24)+ (((uint32)s_extcsdbuf[214])<<16)+ (((uint32)s_extcsdbuf[213])<<8)+ ((uint32)s_extcsdbuf[212]); //unit is 512 byte
	}
	extcsd_Part_Config = s_extcsdbuf[179];
	cardHandle->Rpmb_Capacity = s_extcsdbuf[168]*256; //128/512 ==256 ,unit is 512 byte  
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	{
		return FALSE;
	}		   
			
	//get Boot1 Capacity
	CARD_SDIO_Select_CurPartition(cardHandle, PARTITION_BOOT1); 	  
        if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
		  {
			  return FALSE;
		  }
	CARD_SDIO_ReadExtCSD(cardHandle);
	extcsd_Part_Config = s_extcsdbuf[179];
	cardHandle->Boot1_Capacity = s_extcsdbuf[226]*256; //128/512 ==256 ,unit is 512 byte  
	 if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	 {
		 return FALSE;
	 }	
	 
	 //get Boot2 Capacity
	CARD_SDIO_Select_CurPartition(cardHandle, PARTITION_BOOT2); 	  
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	{
		return FALSE;
	 }
	CARD_SDIO_ReadExtCSD(cardHandle);
	extcsd_Part_Config = s_extcsdbuf[179];
	cardHandle->Boot2_Capacity = s_extcsdbuf[226]*256; //128/512 ==256 ,unit is 512 byte  
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	{
		return FALSE;
	}

	MMC_SWITCH(cardHandle, EXT_CSD_HS_TIMING_INDEX, 1);
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	{
		return FALSE;
	}
#if defined (CONFIG_SC8825) || defined(CONFIG_SC7710G2)
	SDIO_Card_Pal_SetSpeedMode(cardHandle->sdioPalHd, EMMC_SPEED_SDR25);
	SDIO_Card_Pal_SetClk(cardHandle->sdioPalHd, SDIO_CARD_PAL_50MHz);
#else
	SDIO_Card_Pal_SetClk(cardHandle->sdioPalHd,SDIO_CARD_PAL_25MHz);
#endif
	__udelay (100*1000);
#if defined (CONFIG_SC8825) || defined(CONFIG_SC7710G2)
	busWidth = CARD_WIDTH_8_BIT;
#else
	busWidth = CARD_WIDTH_4_BIT;
#endif
	if(FALSE == _SetBusWidth(cardHandle,busWidth))
	{
		return FALSE;
	}
	if(FALSE == CARD_SDIO_SetBlockLength(cardHandle,DEFAULT_CARD_BLOCKLEN))
	{
		return FALSE;
	}

    #if 0		
	uint16 i, j;
	for(i=0; i<512; i++)
		s_sendbuf[i] = 0x1f;
	CARD_SDIO_WriteSingleBlock(cardHandle, 0, (uint8*)s_sendbuf);
	for(i=0; i<512; i++)
		s_sendbuf[i] = 0x5a;
	CARD_SDIO_WriteSingleBlock(cardHandle, 1, (uint8*)s_sendbuf);
#endif
         /*          
	CARD_SDIO_ReadSingleBlock(cardHandle, 0, (uint8*)s_receivebuf1);
	CARD_SDIO_ReadSingleBlock(cardHandle, 1, (uint8*)s_receivebuf2);

        (void)memset((uint8*)0x2000000, 0x0, 0x100000);
        (void)memset((uint8*)0x3000000, 0x0, 0x100000);
         CARD_SDIO_WriteMultiBlock(cardHandle, 0, 50, (uint8*)0x2000000);
         
         CARD_SDIO_ReadMultiBlock(cardHandle, 0, 50, (uint8*)0x3000000);

        {
            uint32 argument = 0;
            argument = CMD6_ACCESS_MODE_WRITE_BYTE | ( 162<<CMD6_BIT_MODE_OFFSET_INDEX & CMD6_BIT_MODE_MASK_INDEX) | CMD6_CMD_SET |(1<<8);
            if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_ACMD6_SET_EXT_CSD,argument,NULL,rspBuf))
            {
                return FALSE;
            }
            if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
            {
                return FALSE;
            }	
        }

         CARD_SDIO_Select_CurPartition(cardHandle, PARTITION_BOOT2); 
	 
	 if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	 {
		 return FALSE;
	 }	
     
          CARD_SDIO_ReadSingleBlock(cardHandle, 0, (uint8*)s_receivebuf1);
         CARD_SDIO_ReadSingleBlock(cardHandle, 1, (uint8*)s_receivebuf2);

       */

//---
	return TRUE;

}

#else
/*****************************************************************************/
//  Description:  Initialize card, change card from idle state to standy by state ,and get some information from card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_InitCard(CARD_SDIO_HANDLE cardHandle, CARD_SPEED_MODE speedmode)
{
	uint8 rspBuf[16];
	uint32 pre_tick, cur_tick;
	CARD_CID_T CID;
	CARD_CSD_T CSD;
	CARD_BUS_WIDTH_E busWidth = CARD_WIDTH_1_BIT;
	uint16 RCA;
	BOOLEAN vertion_flag; //TRUE: SD2.0 FALSE: SD1.x
	SDIO_CARD_PAL_ERROR_E errCode;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

//	CARD_SDIO_PwrCtl(cardHandle,TRUE);
	cardHandle->bus_width = CARD_WIDTH_1_BIT;
	cardHandle->BlockLen = 0;
	cardHandle->vertion = CARD_V_UNKONWN;

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD0_GO_IDLE_STATE,0,NULL,rspBuf))
	{
		return FALSE;
	}
//Now the card is in Idle State
	vertion_flag = TRUE;
       pre_tick = SCI_GetTickCount();
	cur_tick = pre_tick;
	do
	{
		if(CARD_CMD_MAX_TIME < (cur_tick - pre_tick))
		{
			return FALSE;
		}
		errCode = SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD8_SEND_IF_COND,0x000001AA,NULL,rspBuf);
		if(SDIO_CARD_PAL_ERR_NONE != errCode)
		{
			if(0 != (errCode&SDIO_CARD_PAL_ERR_CMD_TIMEOUT))  //lint !e655
			{
				//SD2.0 not support current voltage or it is SD1.x
				vertion_flag = FALSE;
				break;
			}
			else
			{
				cur_tick = SCI_GetTickCount();
				continue;
			}
		}
		CARD_SDIO_PRINT(("SD20 %x,%x,%x,%x",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3]));
		if((0xAA == rspBuf[3])&&(0x00 == rspBuf[2]))
		{
			//SD2.0 not support current voltage
			CARD_SDIO_ASSERT(0);	/*assert verified*/
			return FALSE;    /*lint !e527*/
		}
		cur_tick = SCI_GetTickCount();
	}
	while(0xAA != rspBuf[3]);	/*lint !e644 */

	CARD_SDIO_PRINT(("SD20 vertion_flag = %x",vertion_flag));

       pre_tick = SCI_GetTickCount(); /*set start tick value*/
	if(FALSE == vertion_flag)
	{
		do
		{
			errCode = SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD55_APP_CMD,0,NULL,rspBuf);
			if(SDIO_CARD_PAL_ERR_NONE != errCode)
			{
				if(0 != (errCode&SDIO_CARD_PAL_ERR_CMD_TIMEOUT))  //lint !e655
				{
					//not SD1.x maybe it is MMC
					break;
				}
				else
				{
					return FALSE;
				}
			}

			errCode = SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_ACMD41_SD_SEND_OP_COND,0x00FF8000,NULL,rspBuf);
			if(SDIO_CARD_PAL_ERR_NONE != errCode)
			{
				if(0 != (errCode&SDIO_CARD_PAL_ERR_CMD_TIMEOUT))  //lint !e655
				{
					//not SD1.x maybe it is MMC
					break;
				}
				else
				{
					return FALSE;
				}
			}
			//---
			CARD_SDIO_PRINT(("SD20 %x,%x,%x,%x",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3]));
			if(0 != (rspBuf[0]&BIT_7))
			{
				cardHandle->vertion = CARD_SD_V1_X;
				break;
			}

			cur_tick = SCI_GetTickCount();
			if(CARD_CMD_MAX_TIME < (cur_tick - pre_tick))
			{
				/*cmd time out, return false*/
				return FALSE;
			}
		}
		while(1); /*lint !e506*/

		//MMC_INIT
		if(CARD_V_UNKONWN== cardHandle->vertion)
		{
		       pre_tick = SCI_GetTickCount(); /*set start tick value*/

			do
			{
				if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD1_SEND_OP_COND,0x00FF8000,NULL,rspBuf))
				{
					return FALSE;
				}
				if(0 != (rspBuf[0]&BIT_7))
				{
					cardHandle->vertion = CARD_MMC_331;
					break;
				}

				cur_tick = SCI_GetTickCount();
				if(CARD_CMD_MAX_TIME <  (cur_tick - pre_tick))
				{
					/*cmd time out, return false*/
					return FALSE;
				}
			}
			while(1); /*lint !e506*/
		}


	}
	else
	{
		do
		{
			if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD55_APP_CMD,0,NULL,rspBuf))
			{
				return FALSE;
			}

			if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_ACMD41_SD_SEND_OP_COND,0x40FF8000,NULL,rspBuf))
			{
				return FALSE;
			}
			//---
			CARD_SDIO_PRINT(("SD20 %x,%x,%x,%x",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3]));
			if(0 != (rspBuf[0]&BIT_7))
			{
				if(0 != (rspBuf[0]&BIT_6))
				{
					cardHandle->vertion = CARD_SD_V2_0_HIGHCAP;
				}
				else
				{
					cardHandle->vertion = CARD_SD_V2_0_STANDARD;
				}
				break;
			}

			cur_tick = SCI_GetTickCount();
			if(CARD_CMD_MAX_TIME <  (cur_tick - pre_tick))
			{
				/*cmd time out, return false*/
				return FALSE;
			}
		}
		while(1); /*lint !e506*/

	}
	if(CARD_V_UNKONWN == cardHandle->vertion)
	{
		return FALSE;
	}

// Now SD is in Ready State
	if(FALSE == CARD_SDIO_ReadCID(cardHandle,&CID))
	{
		return FALSE;
	}
// Now SD is in Identification State

	if(CARD_MMC_331 == cardHandle->vertion)
	{
		if(FALSE == CARD_SDIO_SetRCA(cardHandle,cardHandle->RCA))
		{
			return FALSE;
		}
	}
	else if(
	// only SD card can Read RCA
		(CARD_SD_V1_X == cardHandle->vertion)
		||(CARD_SD_V2_0_STANDARD == cardHandle->vertion)
		||(CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)
	)
	{
		if(FALSE == CARD_SDIO_ReadRCA(cardHandle,&RCA))
		{
			return FALSE;
		}
		cardHandle->RCA = RCA;
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
		return FALSE;    /*lint !e527*/
	}

// Now SD is in Stand-by State

	CARD_SDIO_PRINT(("SD20 type is %x",cardHandle->vertion));

	if(FALSE == CARD_SDIO_ReadCSD(cardHandle,&CSD))
	{
		return FALSE;
	}

	if(!(
		(((CARD_SD_V1_X == cardHandle->vertion)||(CARD_SD_V2_0_STANDARD== cardHandle->vertion))&&(0 == CSD.SD_CSD10.CSD_STRUCTURE))
		||((CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)&&(1 == CSD.SD_CSD20.CSD_STRUCTURE))
		||((CARD_MMC_331 == cardHandle->vertion))
	))
	{
		return FALSE;
	}
	if((CARD_SD_V1_X == cardHandle->vertion)||(CARD_SD_V2_0_STANDARD== cardHandle->vertion))
	{
		cardHandle->Nac_Max = (uint32)((CSD.SD_CSD10.TAAC*cardHandle->ClockRate) + (100*CSD.SD_CSD10.NSAC));
		cardHandle->Capacity = (CSD.SD_CSD10.C_SIZE+1) <<(CSD.SD_CSD10.C_SIZE_MULT+2 + CSD.SD_CSD10.READ_BL_LEN-9); //unit is 512byte
	}
	else if(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	{
		cardHandle->Nac_Max = (uint32)((CSD.SD_CSD20.TAAC*cardHandle->ClockRate) + (100*CSD.SD_CSD20.NSAC));
		cardHandle->Capacity = (CSD.SD_CSD20.C_SIZE+1)<<10; // (CSD.SD_CSD20.C_SIZE+1)*1024 unit is 512byte
	}
	else if(CARD_MMC_331 == cardHandle->vertion)
	{
		cardHandle->GrpSize = (CSD.MMC_CSD.ERASE_GRP_SIZE+1)*(CSD.MMC_CSD.ERASE_GRP_MULT+1)*(1 << CSD.MMC_CSD.WRITE_BL_LEN);
		cardHandle->Nac_Max = (uint32)((((CSD.MMC_CSD.TAAC*cardHandle->ClockRate) + (100*CSD.MMC_CSD.NSAC))*10) / 8);
		cardHandle->Capacity = (CSD.MMC_CSD.C_SIZE+1) <<(CSD.MMC_CSD.C_SIZE_MULT+2 + CSD.MMC_CSD.READ_BL_LEN - 9); //unit is 512 byte

	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
		return FALSE;     /*lint !e527*/
	}

//---
	if(CARD_MMC_331 == cardHandle->vertion)
	{
		SDIO_Card_Pal_SetClk(cardHandle->sdioPalHd,SDIO_CARD_PAL_20MHz);
	}
	else if(
		(CARD_SD_V1_X == cardHandle->vertion)
		||(CARD_SD_V2_0_STANDARD== cardHandle->vertion)
		||(CARD_SD_V2_0_HIGHCAP== cardHandle->vertion)
	)
	{
		SDIO_Card_Pal_SetClk(cardHandle->sdioPalHd,SDIO_CARD_PAL_25MHz);
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
		return FALSE;	/*lint !e527*/
	}

	if(FALSE == _SelectCard(cardHandle))
	{
		return FALSE;
	}
	if(
		(CARD_SD_V1_X == cardHandle->vertion)
		||(CARD_SD_V2_0_STANDARD == cardHandle->vertion)
		||(CARD_SD_V2_0_HIGHCAP == cardHandle->vertion)
	)
	{
		busWidth = CARD_WIDTH_4_BIT;
		if(FALSE == _SetBusWidth(cardHandle,CARD_WIDTH_4_BIT))
		{
			return FALSE;
		}
	}
	else if(CARD_MMC_331 == cardHandle->vertion)
	{
		busWidth = CARD_WIDTH_1_BIT;
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
		return FALSE;     /*lint !e527*/
	}

	if(FALSE == CARD_SDIO_SetBlockLength( cardHandle,DEFAULT_CARD_BLOCKLEN))
	{
		return FALSE;
	}

#if defined(CARD_SDIO_HIGHSPEED_SUPPORT)
	if(HIGH_SPEED_MODE == speedmode)
	{
		if(FALSE == ifSupportHighSpeed(cardHandle))
		{
			return FALSE;
		}
		if(TRUE == cardHandle->ifSupportHighSpeed)
		{
			if(FALSE == CARD_SDIO_EnterHighSpeed(cardHandle))
			{
				return FALSE;
			}
		}
	}
	else
	{
		cardHandle->ifEnterHighSpeed = FALSE;
		CARD_SDIO_PRINT(("[Card_sdio] CARD_SDIO_InitCard: Disable HighSpeed Mode !"));
	}
#endif

#if defined(SPRD_SUPPORT_MCEX)
	if(FALSE == ifSupportMcex(cardHandle))
	{
		return FALSE;
	}
	if(TRUE == cardHandle->ifEnterEC)
	{
		BOOLEAN ifEnterMcex;
		if(FALSE == CARD_SDIO_EnterMcex(cardHandle,&ifEnterMcex))
		{
			return FALSE;
		}
	}
#endif



//---
	return TRUE;

}
#endif

//-----------------------------------------------------------------------------------
//	Inquire Card whethe ready for data
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _IsCardReady(CARD_SDIO_HANDLE cardHandle)
{
	uint32 pre_tick = 0, cur_tick = 0;
	uint8 rspBuf[16] = {0};
	uint16 RCA = cardHandle->RCA;
	uint32 argument = 0;

	argument = RCA;
	argument = argument<<16;
	pre_tick = SCI_GetTickCount(); /*set start tick value*/

	do
	{
		if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD13_SEND_STATUS,argument,NULL,rspBuf))
		{
			return FALSE;
		}

//		CARD_SDIO_PRINT(("SD20 %x,%x,%x,%x",rspBuf[0],rspBuf[1],rspBuf[2],rspBuf[3]));
		
		if(0 != (rspBuf[2]&BIT_0))
		{
			return TRUE;
		}

		cur_tick = SCI_GetTickCount();
		if(CARD_CMD_MAX_TIME <  (cur_tick - pre_tick))
		{
			/*cmd time out, return false*/
			CARD_SDIO_PRINT(("[Card_sdio] _IsCardReady timeout !"));
			return FALSE;
		}
	}
	while(1); /*lint !e506*/

}

/*****************************************************************************/
//  Description:  Read single block data from card , unit is block
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		BlockId: block number that you want to read
//		buf : that data read from card will be stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_ReadSingleBlock(CARD_EMMC_PARTITION_TPYE  cardPartiton, CARD_SDIO_HANDLE cardHandle,uint32 BlockId,uint8* buf)
{
	uint8 rspBuf[16];
	uint32 address = 0xFFFFFFFF;
	CARD_DATA_PARAM_T data;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if((CARD_MMC_441_STANDARD == cardHandle->vertion))
	{
		address = BlockId*cardHandle->BlockLen;
	}
	else if(CARD_MMC_441_HIGHCAP== cardHandle->vertion)
	{
		address = BlockId;
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
	}
	
	if(FALSE == _IsCardReady(cardHandle))
	{
		return FALSE;
	}	
	
	data.blkLen = cardHandle->BlockLen;
	data.blkNum = 1;
	data.databuf = buf;
	data.direction = SDIO_DMA_IN;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD17_READ_SINGLE_BLOCK,address,&data,rspBuf))
	{
		return FALSE;
	}

	return TRUE;

}

/*****************************************************************************/
//  Description:  Read multi block data from card , unit is block
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		startBlock:  start block number that you want to read
//		num: the number of block you want to read
//		buf : that data read from card will be stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_ReadMultiBlock(CARD_EMMC_PARTITION_TPYE  cardPartiton, CARD_SDIO_HANDLE cardHandle,uint32 startBlock,uint32 num,uint8* buf)
{
	uint8 rspBuf[16];
	uint32 address = 0xFFFFFFFF;
	CARD_DATA_PARAM_T data;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if((CARD_MMC_441_STANDARD == cardHandle->vertion))
	{
		address = startBlock*cardHandle->BlockLen;
	}

	else if(CARD_MMC_441_HIGHCAP== cardHandle->vertion)
	{
		address = startBlock;
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
	}

	if(FALSE == _IsCardReady(cardHandle))
	{
		return FALSE;
	}	
	
	data.blkLen = cardHandle->BlockLen;
	data.blkNum = num;
	data.databuf = buf;
	data.direction = SDIO_DMA_IN;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD18_READ_MULTIPLE_BLOCK,address,&data,rspBuf))
	{
		SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD12_STOP_TRANSMISSION,NULL,NULL,rspBuf);
		return FALSE;
	}
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD12_STOP_TRANSMISSION,NULL,NULL,rspBuf))
	{
		return FALSE;
	}
	return TRUE;

}

/*****************************************************************************/
//  Description:  Wrtie single block data to card , unit is block
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		BlockId: block number that you want to write
//		buf : that data to be writen to card is stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_WriteSingleBlock(CARD_EMMC_PARTITION_TPYE  cardPartiton, CARD_SDIO_HANDLE cardHandle,uint32 BlockId,uint8* buf)
{
	uint8 rspBuf[16];
	uint32 address = 0xFFFFFFFF;
	CARD_DATA_PARAM_T data;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if((CARD_MMC_441_STANDARD == cardHandle->vertion))
	{
		address = BlockId*cardHandle->BlockLen;
	}

	else if(CARD_MMC_441_HIGHCAP== cardHandle->vertion)
	{
		address = BlockId;
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
	}

	if(FALSE == _IsCardReady(cardHandle))
	{
		return FALSE;
	}	
	
	data.blkLen = cardHandle->BlockLen;
	data.blkNum = 1;
	data.databuf = buf;
	data.direction = SDIO_DMA_OUT;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD24_WRITE_BLOCK,address,&data,rspBuf))
	{
		return FALSE;
	}

	return TRUE;

}

/*****************************************************************************/
//  Description:  Wrtie multi block data to card , unit is block
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		startBlock: start block number that you want to write
//		num: the number of block you want to write
//		buf : the data to be writen to card is stored in this place
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_WriteMultiBlock(CARD_EMMC_PARTITION_TPYE  cardPartiton, CARD_SDIO_HANDLE cardHandle,uint32 startBlock,uint32 num,uint8* buf)
{
	uint8 rspBuf[16];
	uint32 address = 0xFFFFFFFF;
	CARD_DATA_PARAM_T data;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if((CARD_MMC_441_STANDARD == cardHandle->vertion))
	{
		address = startBlock*cardHandle->BlockLen;
	}

	else if(CARD_MMC_441_HIGHCAP== cardHandle->vertion)
	{
		address = startBlock;
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
	}

	if(FALSE == _IsCardReady(cardHandle))
	{
		return FALSE;
	}	
	
	data.blkLen = cardHandle->BlockLen;
	data.blkNum = num;
	data.databuf = buf;
	data.direction = SDIO_DMA_OUT;
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD25_WRITE_MULTIPLE_BLOCK,address,&data,rspBuf))
	{
		SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD12_STOP_TRANSMISSION,NULL,NULL,rspBuf);
		return FALSE;
	}
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD12_STOP_TRANSMISSION,NULL,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;

}


//-----------------------------------------------------------------------------------
//	Set  SD20 erase start address
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _SD20_SetEraseStart(CARD_SDIO_HANDLE cardHandle,uint32 startBlock)
{
	uint8 rspBuf[16];

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD32_ERASE_WR_BLK_START,startBlock,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
//	Set SD20 erase end address
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _SD20_SetEraseEnd(CARD_SDIO_HANDLE cardHandle,uint32 endBlock)
{
	uint8 rspBuf[16];

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD33_ERASE_WR_BLK_END,endBlock,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
//	Set MMC441 erase start address
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _MMC441_SetEraseStart(CARD_SDIO_HANDLE cardHandle,uint32 startBlock)
{
	uint8 rspBuf[16];

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD35_ERASE_GROUP_START,startBlock,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
//	Set MMC441erase end address
//-----------------------------------------------------------------------------------
LOCAL BOOLEAN _MMC441_SetEraseEnd(CARD_SDIO_HANDLE cardHandle,uint32 endBlock)
{
	uint8 rspBuf[16];

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD36_ERASE_GROUP_END,endBlock,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
}
/*****************************************************************************/
//  Description:  erase card
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//		startBlock: start block number that you want to erase
//		endBlock : end block number that you want to erase
//  Return:
//		TRUE: success
//		FALSE: fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_Erase(CARD_SDIO_HANDLE cardHandle,uint32 startBlock,uint32 endBlock)
{
	uint8 rspBuf[16];
	uint32 startAddress = 0xFFFFFFFF, endAddress = 0xFFFFFFFF;

	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	if((CARD_SD_V2_0_STANDARD == cardHandle->vertion)
			||(CARD_SD_V1_X == cardHandle->vertion)
			|| (CARD_MMC_441_STANDARD == cardHandle->vertion))
	{
		startAddress = startBlock*cardHandle->BlockLen;
		endAddress = endBlock*cardHandle->BlockLen;
	}
	else if((CARD_MMC_441_HIGHCAP== cardHandle->vertion) || (CARD_SD_V2_0_HIGHCAP== cardHandle->vertion))
	{
		startAddress = startBlock;
		endAddress = endBlock;
	}
	else if(CARD_MMC_331 == cardHandle->vertion)
	{
		return TRUE;
	}
	else
	{
		CARD_SDIO_ASSERT(0);	/*assert verified*/
	}

	if((CARD_MMC_441_HIGHCAP== cardHandle->vertion) || (CARD_MMC_441_STANDARD == cardHandle->vertion))
	{
//set  MMC441start group Id
		if(FALSE == _MMC441_SetEraseStart( cardHandle,startAddress))
		{
			return FALSE;
		}
//set MMC441 end group Id
		if(FALSE == _MMC441_SetEraseEnd( cardHandle,endAddress))
		{
			return FALSE;
		}
	}
	else
	{
//set SD20 start group Id
		if(FALSE == _SD20_SetEraseStart( cardHandle,startAddress))
		{
			return FALSE;
		}
//set SD20 end group Id
		if(FALSE == _SD20_SetEraseEnd( cardHandle,endAddress))
		{
			return FALSE;
		}
	}
//send erase command
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(cardHandle->sdioPalHd,CARD_CMD38_ERASE,0,NULL,rspBuf))
	{
		return FALSE;
	}

	return TRUE;
 //--
}

/*****************************************************************************/
//  Description:  Get total block number of card ,block length is fix to 512 byte
//  Author: Jason.wu
//  Param
//		cardHandle: the handle returned by CARD_SDIO_Open
//  Return:
//		uint32 value : total block num
//  Note: 
/*****************************************************************************/
PUBLIC uint32 CARD_SDIO_GetCapacity(CARD_SDIO_HANDLE cardHandle)
{
	CARD_SDIO_ASSERT(TRUE == _IsCardHandleValid(cardHandle));	/*assert verified*/

	return (cardHandle->Capacity -cardHandle->Rpmb_Capacity -cardHandle->Boot1_Capacity -cardHandle->Boot2_Capacity);
}

/*****************************************************************************/
/*****************************************************************************/
PUBLIC uint32 CARD_SDIO_BOOT_Enable(CARD_SDIO_HANDLE cardHandle)
{
    return 0;
}

PUBLIC BOOLEAN Emmc_Init()
{
	uint32 ret = 0;
	LDO_Init();
#if defined (CONFIG_TIGER) || defined (CONFIG_SC7710G2)
	emmc_handle = CARD_SDIO_Open(CARD_SDIO_SLOT_7);
#else
	emmc_handle = CARD_SDIO_Open(CARD_SDIO_SLOT_1);
#endif
	//CARD_SDIO_PwrCtl(emmc_handle, FALSE);
	CARD_SDIO_PwrCtl(emmc_handle, TRUE);
	ret = CARD_SDIO_InitCard(emmc_handle, HIGH_SPEED_MODE);

	return ret;
}

PUBLIC BOOLEAN Emmc_Write(CARD_EMMC_PARTITION_TPYE  cardPartiton, uint32 startBlock,uint32 num,uint8* buf)
{
	uint32 ret = 0;
		uint8 rspBuf[16];
	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(emmc_handle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	{
		return FALSE;
	}  
	if(emmc_handle->Cur_Partition != cardPartiton){
		CARD_SDIO_Select_CurPartition(emmc_handle,cardPartiton);

	 if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(emmc_handle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	   {
		   return FALSE;
	   }  
	}
	if ( 1 == num ) 	  	
	  ret = CARD_SDIO_WriteSingleBlock(cardPartiton,emmc_handle, startBlock, buf);
	else
	  ret = CARD_SDIO_WriteMultiBlock(cardPartiton,emmc_handle, startBlock, num, buf); 	

	 return ret;

}

PUBLIC BOOLEAN Emmc_Read(CARD_EMMC_PARTITION_TPYE  cardPartiton, uint32 startBlock,uint32 num,uint8* buf)
{
	uint32 ret = 0;
		uint8 rspBuf[16];

	 if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(emmc_handle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	 {
		 return FALSE;
	 }	
	 if(emmc_handle->Cur_Partition != cardPartiton){
		CARD_SDIO_Select_CurPartition(emmc_handle,cardPartiton);
	 
		 if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(emmc_handle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
		{
			return FALSE;
		}  

	 }
	 if ( 1 == num )	   
	 ret = CARD_SDIO_ReadSingleBlock(cardPartiton,emmc_handle, startBlock, buf);
	else
	 ret = CARD_SDIO_ReadMultiBlock(cardPartiton,emmc_handle, startBlock, num, buf);    

	return ret;

}

PUBLIC BOOLEAN Emmc_Erase(CARD_EMMC_PARTITION_TPYE cardPartiton, uint32 startBlock,uint32 num)
{
	uint32 ret = 0;
	uint8 rspBuf[16];
	uint32 endBlock = startBlock +num -1;

	 if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(emmc_handle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	 {
		 return FALSE;
	 }	
	 if(emmc_handle->Cur_Partition != cardPartiton){
		CARD_SDIO_Select_CurPartition(emmc_handle,cardPartiton);
	 
	 	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(emmc_handle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
		{
			return FALSE;
		}  

	 }

	 ret = CARD_SDIO_Erase(emmc_handle,startBlock, endBlock);

	if(SDIO_CARD_PAL_ERR_NONE != SDIO_Card_Pal_SendCmd(emmc_handle->sdioPalHd,CARD_CMD13_SEND_STATUS, 1<<16,NULL,rspBuf))
	{
		return FALSE;
	}

	return ret;

}

PUBLIC uint32 Emmc_GetCapacity(CARD_EMMC_PARTITION_TPYE cardPartiton)
{
	switch(cardPartiton)
	{
		case PARTITION_USER:
			return (emmc_handle->Capacity - emmc_handle->Boot1_Capacity - emmc_handle->Boot2_Capacity);
			
		case PARTITION_BOOT1:
			return emmc_handle->Boot1_Capacity;			
				
		case PARTITION_BOOT2:
			return emmc_handle->Boot2_Capacity;
			
		default:
			return 0;
	}	
}

