/******************************************************************************
 ** File Name:      nand_controller.h                                         *
 ** Author:         Nick.zhao                                                 *
 ** DATE:           01/03/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
#ifndef NAND_CONTROLLER_H
#define NAND_CONTROLLER_H
#include "sci_types.h"

/******************************************************************************
                          Macro define
******************************************************************************/
//Nand Controller register
#define NFC_MBUF_ADDR      0x60000000
#define NFC_SBUF_ADDR      0x60000C00
#define NFC_WPN            0x60001c38
#define NFC_CMD            0x60001C00
#define NFC_STR0           0x60001C04
#define NFC_STR1           0x60001C08
#define NFC_STR2           0x60001C0C
#define NFC_STR3           0x60001C10
#define NFC_END0           0x60001C14
#define NFC_END1           0x60001C18
#define NFC_END2           0x60001C1C
#define NFC_END3           0x60001C20
#define NFC_PARA           0x60001C24
#define NFC_ID_STATUS      0x60001c3c
#define NFC_CMD_CLR        0x60001c68
#define NFC_ECC_EN         0x60001d00
#define NFC_TIMEOUT        0x60001d18
#define NFC_ENDIAN         0x60001d40
#define NFC_CMD_SET        0x60001d44

//INT CTL
#define NFC_INT_SRC        0x60001c84
#define NFC_INT_EN         0x60001c88
#define NFC_INT_MSK        0x60001c8c

//Nand
#define NAND_CTL_BASE                   0x60001c00

typedef enum
{
    ERR_NF_SUCCESS = 0, // Success,no error
    ERR_NF_FAIL,        // Oprate nand flash fail
    ERR_NF_TIMEOUT,     // Oprate nand flash timeout;
    ERR_NF_BUSY,        // Current device is busy;
    ERR_NF_PROTECTED    // Current device is protected;
} ERR_NF_E;
/******************************************************************************
                          Struct define
******************************************************************************/
//Nand control register
typedef struct nand_tag
{
    VOLATILE uint32 cmd;
    VOLATILE uint32 start_addr0;
    VOLATILE uint32 start_addr1;
    VOLATILE uint32 start_addr2;
    VOLATILE uint32 start_addr3;
    VOLATILE uint32 end_addr0;
    VOLATILE uint32 end_addr1;
    VOLATILE uint32 end_addr2;
    VOLATILE uint32 end_addr3;
    VOLATILE uint32 para;
    VOLATILE uint32 ecc0;
    VOLATILE uint32 ecc1;
    VOLATILE uint32 ecc2;
    VOLATILE uint32 ecc3;
    VOLATILE uint32 wpn;
    VOLATILE uint32 idstatus;
} NAND_CTL_REG_T;
typedef struct
{
    uint16 acycle;        //address cycle, which can be set 3,4,5
    uint16 pagesize;      //page size, which can be set 512 or 2048
    uint16 sparesize;     //spare size, which can be set 16 or 64
    uint16 buswidth;      //bus width, which can be set 0 or 1
    uint16 advance;       //advance property, which can be set 0 or 1
    uint16 eccpos;        //ECC position
} NAND_PARA_T;


typedef struct
{
    uint32 nTrTime;       //Read wait R/B ready
    uint32 nTpTime;      //Program wait R/B ready
    uint32 nTeTime;      //Erase wait R/B ready
    uint32 nTwcTime;   //Read/Write cycle time
} NAND_TIMING_T, *NAND_TIMING_PTR;

/******************************************************************************/
//  Description:   Set NF parameter according to AHB_CLK
//  Parameter:
//      ahb_clk    the frequency of AHB_CLK
//  Return:
//      ERR_NF_SUCCESS    Set NF parameter successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_SetParam (uint32 ahb_clk);

/******************************************************************************/
//  Description:   Init nand controller
//  Parameter:
//      ahb_clk:   the frequency of AHB clock
//  Return:
//      ERR_NF_SUCCESS    Init nand controller successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Init (uint32 ahb_clk);

/******************************************************************************/
//  Description:   Open nand controller
//  Parameter:
//      pNandPara    the parameter of nandflash
//  Return:
//      ERR_NF_SUCCESS    Open nand controller successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Open (NAND_PARA_T *pNandPara);

/******************************************************************************/
//  Description:   Close nand controller
//  Parameter:
//      NULL
//  Return:
//      ERR_NF_SUCCESS    Close nand controller successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Close (void);

/******************************************************************************/
//  Description:   Read nandflash ID
//  Parameter:
//      pID      in  the address of nandflash id
//  Return:
//      ERR_NF_SUCCESS     Get the ID successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Read_ID (uint8 *pID);

/******************************************************************************/
//  Description:   Get nand status
//  Parameter:
//      none
//  Return:
//      ERR_NF_SUCCESS    NAND is ready
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Read_Status (void);

/******************************************************************************/
//  Description:   Reset NandFlash
//  Parameter:
//      None
//  Return:
//      ERR_NF_SUCCESS     Reset nandflash successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Reset (void);

/******************************************************************************/
//  Description:   Read main part and spare part of nand, ECC value can be get
//                 if necessary. This function is used for small page nandflash.
//  Parameter:
//      page_addr:   in   the nand page address.
//      pMBuf:       in   the address of Main part
//      pSBuf:       in   the address of Spare part
//      pEcc:        out  the address of ECC value
//  Return:
//      ERR_NF_SUCCESS    Read nand successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_MS_Read_S (
    uint32 page_addr,
    uint8 *pMBuf,
    uint8 *pSBuf,
    uint8 *pEcc
);
/******************************************************************************/
//  Description:   Read main part of nand, ECC value can be get
//                 if necessary.
//  Parameter:
//      page_addr:   in   the nand page address.
//      pMBuf:       in   the address of Main part
//  Return:
//      ERR_NF_SUCCESS    Read nand successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_M_Read_S (
    uint32 page_addr,
    uint8 *pMBuf
);
/******************************************************************************/
//  Description:   Read spare part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      pSBuf:       in   the address of Spare part
//  Return:
//      ERR_NF_SUCCESS    Read nand successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_S_Read_S (
    uint32 page_addr,
    uint8 *pSBuf
);
/******************************************************************************/
//  Description:   Write main part and spare part of nand
//  Parameter:
//      page_addr:  in   the nand page address.
//      pMBuf:      in   the address of main part
//      pSBuf:      in   the address of spare part
//      ecc_en:     in   the flag to enable ECC
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_MS_Write_S (
    uint32 page_addr,
    uint8 *pMBuf,
    uint8 *pSBuf,
    uint8 ecc_en
);
/******************************************************************************/
//  Description:   Write main part of nand
//  Parameter:
//      page_addr: in    the nand page address.
//      pMBuf:     in    the main part address.
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_M_Write_S (uint32 page_addr,uint8 *pMBuf);

/******************************************************************************/
//  Description:   Write spare part of nand
//  Parameter:
//      page_addr:  in   the nand page address.
//      pSBuf       in   the address of spare part
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_S_Write_S (uint32 page_addr,uint8 *pSBuf);

/******************************************************************************/
//  Description:   Erase a block of nandflash
//  Parameter:
//      block_addr:  in   the nand block address.
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Erase_Block_S (uint32 block_addr);
/******************************************************************************/
//  Description:   Read main part and spare part of nand, ECC value can be get
//                 if necessary. This function is used for large page nandflash.
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number
//      pMBuf:       in   the address of Main part
//      pSBuf:       in   the address of Spare part
//      pEcc:        out  the address of ECC value
//  Return:
//      ERR_NF_SUCCESS    Read nand successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_MS_Read_L (
    uint32 page_addr,
    uint8 index,
    uint8 nSct,
    uint8 *pMBuf,
    uint8 *pSBuf,
    uint8 *pEcc
);
/******************************************************************************/
//  Description:   Read main part of nand, ECC value can be get
//                 if necessary.
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number
//      pMBuf:       in   the address of Main part
//  Return:
//      ERR_NF_SUCCESS    Read nand successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_M_Read_L (
    uint32 page_addr,
    uint8 index,
    uint8 nSct,
    uint8 *pMBuf
);
/******************************************************************************/
//  Description:   Read spare part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number
//      pSBuf:       in   the address of Spare part
//  Return:
//      ERR_NF_SUCCESS    Read nand successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_S_Read_L (
    uint32 page_addr,
    uint8 index,
    uint8 nSct,
    uint8 *pSBuf
);
/******************************************************************************/
//  Description:   Write main part and spare part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number
//      pMBuf:       in   the address of Main part
//      pSBuf:       in   the address of Spare part
//      ecc_en:      in   the flag to enable ECC
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_MS_Write_L (
    uint32 page_addr,
    uint8 index,
    uint8 nSct,
    uint8 *pMBuf,
    uint8 *pSBuf,
    uint8 ecc_en
);
/******************************************************************************/
//  Description:   Write main part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number
//      pMBuf:       in   the address of Main part
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_M_Write_L (uint32 page_addr,uint8 index,uint8 nSct,uint8 *pMBuf);
/******************************************************************************/
//  Description:   Write spare part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number
//      pSBuf:       in   the address of Spare part
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_S_Write_L (uint32 page_addr,uint8 index,uint8 nSct,uint8 *pSBuf);
/******************************************************************************/
//  Description:   Erase a block of nandflash
//  Parameter:
//      block_addr:  in    the nand block address.
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Erase_Block_L (uint32 block_addr);
/******************************************************************************/
//  Description:   Input the nandflash parameter
//  Parameter:
//      pNandTiming    the frequency of AHB_CLK
//  Return:
//      ERR_NF_SUCCESS    Set NF parameter successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_InitParam (NAND_TIMING_PTR pNandTiming);
#endif // NAND_CONTROLLER_H