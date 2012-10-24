/******************************************************************************
 ** File Name:      i2c_drv.c                                                 *
 ** Author:         Richard Yang                                              *
 ** DATE:           01/16/2001                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/16/2001     Richard.Yang     Create.                                   *
 ** 04/02/2002     Lin.Liu            Code                                      *
 ** 04/22/2002     Lin.liu          Set CPC_ZD_WPU after change clock.        *
 ** 04/23/2002     Lin.liu          1. Remove i2c_clk to i2c_drv.h            * 
 **                                 2. Remove i2c_inited flag                 *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <asm/arch/sci_types.h>
#include <asm/arch/os_api.h>
#include <asm/arch/i2c_phy.h>
#include <asm/arch/i2c_reg_v0.h>
#include <asm/arch/chip_drvapi.h>
#include <asm/arch/chip_plf_export.h>
#include <asm/arch/sc8810_reg_global.h>
#include <asm/arch/sc8810_reg_base.h>
//#define I2C_BASE 		I2C0_BASE

//#include "dcam_fpga_misc.h"

#ifndef TIGER_REG
#define TIGER_REG
#endif

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



/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#define I2C_TIMEOUT_FACTOR  500000  //the critical value is 10000

#define ARM_CLK_13M         13000000
#define ARM_CLK_24M         24000000


#define I2C_READ_BIT        0x1

//#define I2C_USE_INT

#ifndef I2C_USE_INT 

#define I2C_WAIT_INT                                                  \
{                                                                     \
    timetick = SYSTEM_CURRENT_CLOCK;                                  \
    while(!(ptr->ctl & I2CCTL_INT))                                   \
    {                                                                 \
        if((SYSTEM_CURRENT_CLOCK - timetick) >= g_i2c_timeout)        \
        {                                                             \
            if(ERR_I2C_NONE == ret_value)                             \
            {                                                         \
                ret_value = ERR_I2C_INT_TIMEOUT;                      \
            }                                                         \
            break;                                                    \
        }                                                             \
                                                                      \
    }                                                                 \
                                                                      \
}

#else

#define I2C_WAIT_INT                                                  \
{                                                                     \
    timetick = SYSTEM_CURRENT_CLOCK;                                  \
    while (g_wait_i2c_int_flag)                                       \
    {                                                                 \
        if ((SYSTEM_CURRENT_CLOCK - timetick) >= g_i2c_timeout)       \
        {                                                             \
            if(ERR_I2C_NONE == ret_value)                             \
            {                                                         \
                ret_value = ERR_I2C_INT_TIMEOUT;                      \
            }                                                         \
            break;                                                    \
        }                                                             \
    }                                                                 \
    g_wait_i2c_int_flag = 1;                                          \
}

#endif 
       
#define I2C_WAIT_BUSY                                                 \
{                                                                     \
    timetick = SYSTEM_CURRENT_CLOCK;                                  \
    while(ptr->cmd & I2CCMD_BUS)                                      \
    {                                                                 \
        if ((SYSTEM_CURRENT_CLOCK - timetick) >= g_i2c_timeout)       \
        {                                                             \
            if(ERR_I2C_NONE == ret_value)                             \
            {                                                         \
                ret_value = ERR_I2C_BUSY_TIMEOUT;                     \
            }                                                         \
            break;                                                    \
        }                                                             \
    }                                                                 \
                                                                      \
}
       
#define I2C_WAIT_ACK                                                  \
{                                                                     \
    timetick = SYSTEM_CURRENT_CLOCK;                                  \
    while(ptr->cmd & I2CCMD_ACK)                                      \
    {                                                                 \
        if ((SYSTEM_CURRENT_CLOCK - timetick) >= g_i2c_timeout)       \
        {                                                             \
            if(ERR_I2C_NONE == ret_value)                             \
            {                                                         \
                ret_value = ERR_I2C_ACK_TIMEOUT;                      \
            }                                                         \
            break;                                                    \
        }                                                             \
    }                                                                 \
}

#ifndef I2C_USE_INT

#define I2C_CLEAR_INT                                                 \
{                                                                     \
    I2C_WAIT_BUSY                                                     \
    CHIP_REG_OR(I2C_CMD, I2CCMD_INT_ACK);                               \
}

#else

#define I2C_CLEAR_INT  

#endif

void ms_delay(uint32 ticks)
{
	int i;
	for(i=0; i<10*ticks; i++);
}	
/**---------------------------------------------------------------------------*
 **                            Global Variables
 **---------------------------------------------------------------------------*/
volatile BOOLEAN    g_wait_i2c_int_flag;
volatile BOOLEAN    g_i2c_open_flag=FALSE;
volatile uint32     g_i2c_timeout = 10; //unit is ms

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

uint32 CHIP_GetAPBClk(void)
{
    return ARM_CLK_26M;
}

/*********************************************************************/
//  Description: i2c interrupt service 
//  Input:
//      param         not use
//  Return:
//      None
//    Note: 
//      None       
/*********************************************************************/
PUBLIC void I2C_Handler(uint32 param)
{
    param = param; // avoid compiler warning

    /* set i2c flag  */    
    g_wait_i2c_int_flag = FALSE;
    
    while((*(volatile uint32*)(I2C_CMD)) & I2CCMD_BUS);
    
    /* clear i2c int  */
    CHIP_REG_OR(I2C_CMD, I2CCMD_INT_ACK);    
}
/*********************************************************************/
//  Description: i2c set SCL clock
//  Input:
//      freq     I2C SCL's frequency to be set
//  Return:
//      ERR_I2C_NONE    successfully
//    Note: 
//      None       
/*********************************************************************/
PUBLIC ERR_I2C_E I2C_SetSCLclk(uint32 freq)
{
    uint32 APB_clk,i2c_dvd;
    
    volatile I2C_CTL_REG_T *ptr = (volatile I2C_CTL_REG_T *)I2C_BASE;

    SCI_ASSERT(freq > 0);
    SCI_ASSERT(g_i2c_open_flag);

    APB_clk= CHIP_GetAPBClk();
    i2c_dvd=APB_clk/(4*freq)-1;

    ptr->div0=(uint16)(i2c_dvd & 0xffff);
    ptr->div1=(uint16)(i2c_dvd>>16);

    g_i2c_timeout = I2C_TIMEOUT_FACTOR / (freq);
    
    if(g_i2c_timeout < 2)
        g_i2c_timeout = 2;
    //g_i2c_timeout will be changed according I2C frequency
    
    return ERR_I2C_NONE;
     
      
}
/*********************************************************************/
//  Description: i2c init fuction 
//  Input:
//      freq     I2C SCL's frequency
//  Return:
//      ERR_I2C_NONE    successfully
//    Note: 
//      None       
/*********************************************************************/
PUBLIC ERR_I2C_E I2C_Init(uint32 freq)
{    
    /*SC8810 use IIC1 for sensor init, but befoe SC8810 all chip use
    IIC0 for sensor init. IIC1 use bit29 for Clock enable.
    */
    volatile I2C_CTL_REG_T *ptr = (volatile I2C_CTL_REG_T *)I2C_BASE;

    freq*=1000;
    SCI_ASSERT (freq > 0);

    g_wait_i2c_int_flag = TRUE;
    g_i2c_open_flag=TRUE;

    // gen0, enable i2c device.
#if 0
	CHIP_REG_OR(GR_GEN0, APB_I2C1_EB); //GEN0_I2C1_EN
#else
    CHIP_REG_OR(GR_GEN0, GEN0_I2C_EN); //GEN0_I2C0_EN
#endif

    ptr->rst = BIT_0;//why reset
    ptr->ctl &= ~(I2CCTL_EN);//you must first disable i2c module then change clock  
    ptr->ctl &= ~(I2CCTL_IE);
    ptr->ctl &= ~(I2CCTL_CMDBUF_EN);

    I2C_SetSCLclk(freq);

    CHIP_REG_OR(I2C_CTL, (I2CCTL_IE | I2CCTL_EN));

     //Clear I2C int
    CHIP_REG_OR(I2C_CMD, I2CCMD_INT_ACK); 
   
    return ERR_I2C_NONE;
    
#if 0    
    volatile I2C_CTL_REG_T *ptr = (I2C_CTL_REG_T *)I2C_BASE;

    freq*=1000;
    SCI_ASSERT (freq > 0);

    g_wait_i2c_int_flag = TRUE;
    g_i2c_open_flag=TRUE;

    // gen0, enable i2c device.
    CHIP_REG_OR(GR_GEN0, GEN0_I2C_EN);
    
    ptr->rst = BIT_0;//why reset
    ptr->ctl &= ~(I2CCTL_EN);//you must first disable i2c module then change clock  
    ptr->ctl &= ~(I2CCTL_IE);
    ptr->ctl &= ~(I2CCTL_CMDBUF_EN);

    I2C_SetSCLclk(freq);

    CHIP_REG_OR(I2C_CTL, (I2CCTL_IE | I2CCTL_EN));

     //Clear I2C int
    CHIP_REG_OR(I2C_CMD, I2CCMD_INT_ACK); 
   
    return ERR_I2C_NONE;
#endif    
}
/*********************************************************************/
//  Description: get i2c SCL clock
//  Input:
//      None
//  Return:
//      the freq of i2c SCL clock
//    Note: 
//      None       
/*********************************************************************/
PUBLIC uint32 I2C_GetSCLclk()
{
    uint32 APB_clk,i2c_dvd,freq;
    
    volatile I2C_CTL_REG_T *ptr = (I2C_CTL_REG_T *)I2C_BASE;

    SCI_ASSERT(g_i2c_open_flag);

    APB_clk= CHIP_GetAPBClk();

    i2c_dvd=((ptr->div1)<<16)|(ptr->div0);

    freq=APB_clk/(4*(i2c_dvd+1));


    return freq;

}
/*********************************************************************/
//  Description: Write a command to a slave device
//  Input:
//      addr     the slave device's address
//      command  the command to be set into the slave device's address
//        ack_en     Enable/Disable check the slave device rsp ACK
//  Return:
//      ERR_I2C_NONE    successfully
//    Note: 
//      None       
/*********************************************************************/
PUBLIC ERR_I2C_E I2C_WriteCmd(uint8 addr,uint8 command, BOOLEAN ack_en)
{
    volatile uint32 timetick = 0; 
    volatile uint32 cmd = 0;
    volatile I2C_CTL_REG_T * ptr = (volatile I2C_CTL_REG_T *)I2C_BASE;
    uint32   ret_value = ERR_I2C_NONE;

    SCI_ASSERT(g_i2c_open_flag);
    SCI_ASSERT(g_i2c_timeout > 0);
    
    cmd = ((uint32)addr)<<8;
    cmd = cmd | I2CCMD_START | I2CCMD_WRITE;//send device address
    ptr->cmd = cmd; 

    I2C_WAIT_INT
    
    I2C_CLEAR_INT
    
    //check ACK
    if(ack_en)
    {
        I2C_WAIT_ACK
    }

    cmd = ((uint32)command)<<8;
    cmd = cmd | I2CCMD_WRITE | I2CCMD_STOP;//send command
    ptr->cmd = cmd; 

    I2C_WAIT_INT
       
    I2C_CLEAR_INT  

    //check ACK
    if(ack_en)
    {
        I2C_WAIT_ACK
    }

    return ERR_I2C_NONE;
            
}

/*********************************************************************/
//  Description: Write a data to a slave device
//  Input:
//      addr     the slave device's address
//      command  the command to be set into the slave device's address
//        ack_en     Enable/Disable check the slave device rsp ACK
//  Return:
//      ERR_I2C_NONE    successfully
//    Note: 
//      None       
/*********************************************************************/
PUBLIC ERR_I2C_E I2C_WriteData(uint8 addr,uint8 data, BOOLEAN ack_en)
{
    volatile uint32 timetick = 0; 
    volatile uint32 cmd = 0;
    volatile I2C_CTL_REG_T * ptr = (volatile I2C_CTL_REG_T *)I2C_BASE;
    uint32   ret_value = ERR_I2C_NONE;

    SCI_ASSERT(g_i2c_open_flag);
    SCI_ASSERT(g_i2c_timeout > 0);
    
    cmd = ((uint32)addr)<<8;
    cmd = cmd | I2CCMD_START | I2CCMD_WRITE;//send device address
    ptr->cmd = cmd; 

    I2C_WAIT_INT
    
    I2C_CLEAR_INT
    
    //check ACK
    if(ack_en)
    {
        I2C_WAIT_ACK
    }

    data = ((uint32)data)<<8;
    data = data | I2CCMD_WRITE | I2CCMD_STOP;//send command
    ptr->cmd = data; 

    I2C_WAIT_INT
       
    I2C_CLEAR_INT  

    //check ACK
    if(ack_en)
    {
        I2C_WAIT_ACK
    }

    return ERR_I2C_NONE;
            
}
/*********************************************************************/
//  Description: read a command from the slave device
//  Input:
//      addr     the slave device's address
//      pCmd     the command's pointer
//        ack_en     Enable/Disable check the slave device rsp ACK
//  Return:
//      ERR_I2C_NONE    successfully
//    Note: 
//      None       
/*********************************************************************/
PUBLIC ERR_I2C_E I2C_ReadCmd(uint8 addr,uint8 *pCmd, BOOLEAN ack_en)
{
    volatile uint32 timetick = 0; 
    volatile uint32 cmd = 0;
    volatile I2C_CTL_REG_T * ptr = (volatile I2C_CTL_REG_T *)I2C_BASE;
    uint32   ret_value = ERR_I2C_NONE;
    
    SCI_ASSERT(NULL != pCmd);
    SCI_ASSERT(g_i2c_open_flag);
    SCI_ASSERT(g_i2c_timeout > 0);

    cmd = ((uint32)(addr|I2C_READ_BIT))<<8;
    cmd = cmd | I2CCMD_START | I2CCMD_WRITE;//send device address
    ptr->cmd = cmd; 

    I2C_WAIT_INT
           
    I2C_CLEAR_INT  

    //check ACK
    if(ack_en)
    {
        I2C_WAIT_ACK
    }

    cmd = I2CCMD_READ | I2CCMD_STOP | I2CCMD_TX_ACK;
    ptr->cmd = cmd;
     
    I2C_WAIT_INT
       
    I2C_CLEAR_INT  


    *pCmd=(uint8)((ptr->cmd)>>8);

    return ERR_I2C_NONE;
}
/*********************************************************************/
//  Description: Write the command array into the slave device 
//  Input:
//      addr     the slave device's address
//      pCmd     the command array's pointer
//      len      the length of the command array
//        ack_en     Enable/Disable check the slave device rsp ACK
//  Return:
//      ERR_I2C_NONE    successfully
//    Note: 
//      None       
/*********************************************************************/
    uint32   ret_value = ERR_I2C_NONE;
    volatile uint32 timetick = 0; 
PUBLIC ERR_I2C_E I2C_WriteCmdArr(uint8 addr, uint8 *pCmd, uint32 len, BOOLEAN ack_en)
{
    volatile uint32 curtime = 0; 	
    volatile uint32 i = 0;
    volatile uint32 cmd = 0;
    volatile I2C_CTL_REG_T * ptr = (volatile I2C_CTL_REG_T *)I2C_BASE;
    
    SCI_ASSERT(NULL != pCmd);
    SCI_ASSERT(g_i2c_open_flag);
    SCI_ASSERT(g_i2c_timeout > 0);
    
    cmd = ((uint32)addr)<<8;
    cmd = cmd | I2CCMD_START | I2CCMD_WRITE ;//send device address 0x9824
    ptr->cmd = cmd; 

    I2C_WAIT_INT		
    
    I2C_CLEAR_INT   
    
    //check ACK
    if(ack_en)
    {
        I2C_WAIT_ACK
    }

    for(i=0;i<len;i++)
    {
        cmd = ((uint32)pCmd[i])<<8;
        if(i== (len-1))     
            cmd = cmd | I2CCMD_WRITE | I2CCMD_STOP;//send command
        else
            cmd = cmd | I2CCMD_WRITE ;

        ptr->cmd = cmd; 

        I2C_WAIT_INT
           
        I2C_CLEAR_INT    

        //check ACK
        if(ack_en)
        {
            I2C_WAIT_ACK
        }

    }

    return ret_value;
}
/*********************************************************************/
//  Description: Read a command array from the slave device 
//  Input:
//      addr     the slave device's address
//      pCmd     the command array's pointer
//      len      the length of command array
//        ack_en     Enable/Disable check the slave device rsp ACK
//  Return:
//      ERR_I2C_NONE    successfully
//    Note: 
//      None       
/*********************************************************************/
PUBLIC ERR_I2C_E I2C_ReadCmdArr(uint8 addr, uint8 *pCmd, uint32 len,BOOLEAN ack_en )
{
    volatile uint32 timetick = 0; 
    volatile uint32 i = 0;
    volatile uint32 cmd = 0;
    volatile I2C_CTL_REG_T * ptr = (volatile I2C_CTL_REG_T *)I2C_BASE;
    uint32   ret_value = ERR_I2C_NONE;
    
    SCI_ASSERT(NULL !=pCmd );
    SCI_ASSERT(g_i2c_open_flag);
    SCI_ASSERT(g_i2c_timeout > 0);

    cmd = ((uint32)(addr|I2C_READ_BIT))<<8;
    cmd = cmd | I2CCMD_START | I2CCMD_WRITE;//send device address
    ptr->cmd = cmd; 

    I2C_WAIT_INT
           
    I2C_CLEAR_INT
    
    //check ACK
    if(ack_en)
    {
        I2C_WAIT_ACK
    }

    for(i=0;i<len;i++)
    {
        if(i<len-1)
            cmd = I2CCMD_READ;  // I2CCMD_READ|I2CCMD_TX_ACK;
        else
            cmd = I2CCMD_READ|I2CCMD_STOP|I2CCMD_TX_ACK;

        ptr->cmd = cmd;

        I2C_WAIT_INT
                   
        I2C_CLEAR_INT   

        pCmd[i] = (uint8)((ptr->cmd)>>8);
    }

    return ERR_I2C_NONE;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/


#ifdef   __cplusplus
    } 
#endif

/*  End Of File */
