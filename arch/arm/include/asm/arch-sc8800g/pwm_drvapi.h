/******************************************************************************
 ** File Name:      pwm_drvapi.H                                                 *
 ** Author:         Lin.liu                                                   *
 ** DATE:           03/01/2004                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 PWM device.                                               *
*******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/01/2004     Lin.liu          Create.                                   *
 ******************************************************************************/
#ifndef _PWM_DRVAPI_H_
#define _PWM_DRVAPI_H_

typedef enum
{
    PWMA_E = 0,
    PWMB_E,
    PWMC_E,
    PWMD_E,
    PWME_E,
    PWMF_E,   
    PWM_MAX
} PWM_PORT_ID_E;

#define PWM_CTL_S_ONOFF 0x30 /*Set PWM ON/OFF*/
#define PWM_CTL_S_CLKSRC 0x31 /*Set CLK sorce*/

void PWM_Config (int pwm_num,  uint32  freq,  uint16  duty_cycle);
uint32 PWM_Ioctl (uint32 pwm_num, uint32 cmd, uint32 *arg);

#endif // _PWM_DRVAPI_H_