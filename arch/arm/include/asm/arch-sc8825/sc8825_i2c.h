/******************************************************************************
 ** File Name:      I2C_drv.c                                                 *
 ** Author:         Lin.liu                                                   *
 ** DATE:           04/23/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic interfaces of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/16/2001     Richard.Yang     Create.                                   *
 ** 04/02/2002     Lin.Liu	        Code                                      *
 ******************************************************************************/
#ifndef I2C_H
#define I2C_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <asm/arch/sci_types.h>
#include <asm/arch/arm_reg.h>

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define  PUBLIC  

//	I2C command define 
#define   I2C_CMD_ICLR      BIT_0
#define   I2C_CMD_TX_ACK    BIT_1
#define   I2C_CMD_WRITE     BIT_2
#define   I2C_CMD_READ      BIT_3
#define   I2C_CMD_STOP      BIT_4
#define   I2C_CMD_START     BIT_5
#define   I2C_CMD_ACK       BIT_6
#define   I2C_CMD_BUSY      BIT_7

//I2C error code define
typedef enum
{
	ERR_I2C_NONE = 0,				// Success,no error
	ERR_I2C_ACK_TIMEOUT,			// I2C wait ACK timeout
	ERR_I2C_INT_TIMEOUT,			// I2C wait INT timeout
	ERR_I2C_BUSY_TIMEOUT,			// I2C wait BUSY timeout
	ERR_I2C_DEVICE_NOT_FOUND		// I2C device not found
} ERR_I2C_E;




/**---------------------------------------------------------------------------*
 **                      Function  Prototype
 **---------------------------------------------------------------------------*/
/*********************************************************************/
//  Description: i2c interrupt service 
//  Input:
//      param         not use
//  Return:
//      None
//	Note: 
//      None       
/*********************************************************************/
PUBLIC void I2C_Handler(uint32 param);

/*********************************************************************/
//  Description: Write a command to a slave device
//  Input:
//      addr     the slave device's address
//      command  the command to be set into the slave device's address
//		ack_en	 Enable/Disable check the slave device rsp ACK
//  Return:
//      ERR_I2C_NONE    successfully
//	Note: 
//      None       
/*********************************************************************/
PUBLIC ERR_I2C_E I2C_WriteCmd(uint8 addr,uint8 command, BOOLEAN ack_en);

/*********************************************************************/
//  Description: read a command from the slave device
//  Input:
//      addr     the slave device's address
//      pCmd     the command's pointer
//		ack_en	 Enable/Disable check the slave device rsp ACK
//  Return:
//      ERR_I2C_NONE    successfully
//	Note: 
//      None       
/*********************************************************************/
PUBLIC ERR_I2C_E I2C_ReadCmd(uint8 addr,uint8 *pCmd,BOOLEAN ack_en);

/*********************************************************************/
//  Description: Write the command array into the slave device 
//  Input:
//      addr     the slave device's address
//      pCmd     the command array's pointer
//      len      the length of the command array
//		ack_en	 Enable/Disable check the slave device rsp ACK
//  Return:
//      ERR_I2C_NONE    successfully
//	Note: 
//      None       
/*********************************************************************/
PUBLIC ERR_I2C_E I2C_WriteCmdArr(uint8 addr, uint8 *pCmd, uint32 len, BOOLEAN ack_en);

/*********************************************************************/
//  Description: Read a command array from the slave device 
//  Input:
//      addr     the slave device's address
//      pCmd     the command array's pointer
//      len      the length of command array
//		ack_en	 Enable/Disable check the slave device rsp ACK
//  Return:
//      ERR_I2C_NONE    successfully
//	Note: 
//      None       
/*********************************************************************/
PUBLIC ERR_I2C_E I2C_ReadCmdArr(uint8 addr, uint8 *pCmd, uint32 len,BOOLEAN ack_en  );

/*********************************************************************/
//  Description: i2c set SCL clock
//  Input:
//      freq     I2C SCL's frequency to be set
//  Return:
//      ERR_I2C_NONE    successfully
//	Note: 
//      None       
/*********************************************************************/
PUBLIC ERR_I2C_E I2C_SetSCLclk(uint32 freq);

/*********************************************************************/
//  Description: get i2c SCL clock
//  Input:
//      None
//  Return:
//      the freq of i2c SCL clock
//	Note: 
//      None       
/*********************************************************************/
PUBLIC uint32 I2C_GetSCLclk(void);

/*********************************************************************/
//  Description: i2c init fuction 
//  Input:
//      freq     I2C SCL's frequency
//  Return:
//      ERR_I2C_NONE    successfully
//	Note: 
//      None       
/*********************************************************************/
PUBLIC ERR_I2C_E I2C_Init(uint32 freq);




/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /* I2C_H  */

/* End Of File  */
