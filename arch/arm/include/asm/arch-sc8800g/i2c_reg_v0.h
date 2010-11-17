/******************************************************************************
 ** File Name:      I2C_reg_v0.h                                                 *
 ** Author:         liuhao                                                   *
 ** DATE:           06/28/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2010     liuhao     Create.                                   *
 ******************************************************************************/
#ifndef _I2C_REG_V0_H_
#define _I2C_REG_V0_H_
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
/*----------I2C Register----------*/
//I2C_BASE      0x86000000
#define I2C_CTL                         (I2C_BASE + 0x0000)
#define I2C_CMD                         (I2C_BASE + 0x0004)
#define I2C_CLKD0                       (I2C_BASE + 0x0008)
#define I2C_CLKD1                       (I2C_BASE + 0x000C)
#define I2C_RST                         (I2C_BASE + 0x0010)
#define I2C_CMD_BUF                     (I2C_BASE + 0x0014)

//The corresponding bit of I2C_CTL register.
#define I2CCTL_INT                      (1 << 0)        //I2c interrupt
#define I2CCTL_ACK                      (1 << 1)        //I2c received ack value
#define I2CCTL_BUSY                     (1 << 2)        //I2c data line value
#define I2CCTL_IE                       (1 << 3)        //I2c interrupt enable
#define I2CCTL_EN                       (1 << 4)        //I2c module enable
#define I2CCTL_CMDBUF_EN                (1 << 5)        //Enable the cmd buffer mode
#define I2CCTL_CMDBUF_EXEC              (1 << 6)        //Start to exec the cmd in the cmd buffer
#define I2CCTL_ST_CMDBUF                (7 << 7)        //The state of  I2c cmd buffer state machine.
#define I2CCTL_CMDBUF_WPTR              (7 << 7)        //I2c command buffer write pointer

//The corresponding bit of I2C_CMD register.
#define I2CCMD_INT_ACK                  (1 << 0)        //I2c interrupt clear bit
#define I2CCMD_TX_ACK                   (1 << 1)        //I2c transmit ack that need to be send
#define I2CCMD_WRITE                    (1 << 2)        //I2c write command
#define I2CCMD_READ                     (1 << 3)        //I2c read command
#define I2CCMD_STOP                     (1 << 4)        //I2c stop command
#define I2CCMD_START                    (1 << 5)        //I2c start command
#define I2CCMD_ACK                      (1 << 6)        //I2c received ack  value
#define I2CCMD_BUS                      (1 << 7)        //I2c busy in exec commands
#define I2CCMD_DATA                     0xFF00          //I2c data received or data need to be transmitted

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
//I2C control register
typedef struct i2c_tag
{
    VOLATILE uint32 ctl;
    VOLATILE uint32 cmd;
    VOLATILE uint32 div0;
    VOLATILE uint32 div1;
    VOLATILE uint32 rst;
    VOLATILE uint32 cmd_buf;
} I2C_CTL_REG_T;

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
