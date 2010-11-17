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

    
PUBLIC uint16 ADI_Analogdie_reg_read(uint32 addr);

PUBLIC void ADI_Analogdie_reg_write(uint32 addr, uint16 data);


PUBLIC void ADI_init (void);

///for analog die register operation
#define ANA_REG_OR(reg_addr, value)     \
    do{\
        uint16 adi_tmp_val = ADI_Analogdie_reg_read(reg_addr); \
        adi_tmp_val |= (uint16)(value); \
        ADI_Analogdie_reg_write(reg_addr, adi_tmp_val); \
    }while(0)
#define ANA_REG_MSK_OR(reg_addr, value, msk)        \
    do{\
        uint16 adi_tmp_val = ADI_Analogdie_reg_read(reg_addr); \
        adi_tmp_val &= (uint16)(~(msk)); \
        adi_tmp_val |= (uint16)((value)&(msk)); \
        ADI_Analogdie_reg_write(reg_addr, adi_tmp_val); \
    }while(0)
#define ANA_REG_AND(reg_addr, value)    \
    do{\
        uint16 adi_tmp_val = ADI_Analogdie_reg_read(reg_addr); \
        adi_tmp_val &= (uint16)(value); \
        ADI_Analogdie_reg_write(reg_addr, adi_tmp_val); \
    }while(0)

#define ANA_REG_SET(reg_addr, value)    ADI_Analogdie_reg_write(reg_addr, (uint16)(value))

#define ANA_REG_GET(reg_addr)           ADI_Analogdie_reg_read(reg_addr)


#endif  //_ADI_HAL_INTERNAL_H_