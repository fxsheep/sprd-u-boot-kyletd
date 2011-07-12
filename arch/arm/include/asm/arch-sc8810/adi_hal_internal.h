/******************************************************************************
 ** File Name:      adi_hal_internal.h                                                 *
 ** Author:         tim.luo                                             *
 ** DATE:           2/25/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 Analog to Digital Module.                                       *
 **                                                                                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2/25/2010     Tim Luo      Create.                                   *
 **                                                                                                *
 ******************************************************************************/


#ifndef _ADI_HAL_INTERNAL_H_
#define  _ADI_HAL_INTERNAL_H_

#include "sci_types.h"

    
unsigned short ADI_Analogdie_reg_read (unsigned int addr);

void ADI_Analogdie_reg_write (unsigned int addr, unsigned short data);


void ADI_init (void);

///for analog die register operation
#define ANA_REG_OR(reg_addr, value)     \
    do{\
        unsigned short adi_tmp_val = ADI_Analogdie_reg_read(reg_addr); \
        adi_tmp_val |= (uint16)(value); \
        ADI_Analogdie_reg_write(reg_addr, adi_tmp_val); \
    }while(0)
#define ANA_REG_MSK_OR(reg_addr, value, msk)        \
    do{\
        unsigned short adi_tmp_val = ADI_Analogdie_reg_read(reg_addr); \
        adi_tmp_val &= (unsigned short)(~(msk)); \
        adi_tmp_val |= (unsigned short)((value)&(msk)); \
        ADI_Analogdie_reg_write(reg_addr, adi_tmp_val); \
    }while(0)
#define ANA_REG_AND(reg_addr, value)    \
    do{\
        unsigned short adi_tmp_val = ADI_Analogdie_reg_read(reg_addr); \
        adi_tmp_val &= (unsigned short)(value); \
        ADI_Analogdie_reg_write(reg_addr, adi_tmp_val); \
    }while(0)

#define ANA_REG_SET(reg_addr, value)    ADI_Analogdie_reg_write(reg_addr, (unsigned short)(value))

#define ANA_REG_GET(reg_addr)           ADI_Analogdie_reg_read(reg_addr)


#endif  //_ADI_HAL_INTERNAL_H_
