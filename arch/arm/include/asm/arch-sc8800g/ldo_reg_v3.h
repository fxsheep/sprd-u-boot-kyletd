/******************************************************************************
 ** File Name:    ldo_reg_v3.h                                            *
 ** Author:       mingwei.zhang                                                 *
 ** DATE:         06/11/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/11/2010    mingwei.zhang   Create.                                     *
 ******************************************************************************/
#ifndef _LDO_REG_V3_H_
#define _LDO_REG_V3_H_
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
/* save value for register */
#define LOCAL_VAR_DEF   uint32 reg_val;

#define REG_SETCLRBIT(_reg_addr, _set_bit, _clr_bit)    \
    do  \
    {   \
        reg_val = ANA_REG_GET(_reg_addr);   \
        reg_val |= (_set_bit);  \
        reg_val &= ~(_clr_bit); \
        ANA_REG_SET(_reg_addr,reg_val);   \
    }while(0);

#define REG_SETBIT(_reg_addr, _bit_mask, _bit_set) ANA_REG_MSK_OR(_reg_addr, _bit_set, _bit_mask);

#define GET_LEVELBIT(_reg_addr, _bit_mask, _bit_set, _level_var)   \
    {   \
        (_level_var) |= \
                        ((ANA_REG_GET(_reg_addr)&(_bit_mask))?(_bit_set):0);    \
    }

#define GET_LEVEL(_reg_addr, _bit0_mask, _bit1_mask, _level_var)    \
    {   \
        (_level_var) |= \
                        (((ANA_REG_GET(_reg_addr)&(_bit0_mask))?(BIT_0):0)|    \
                         ((ANA_REG_GET(_reg_addr)&(_bit1_mask))?(BIT_1):0)); \
    }

/* macro used to set voltage level according to bit field */
#define SET_LEVELBIT(_reg_addr, _bit_mask, _set_bit, _rst_bit)  \
    {   \
        REG_SETBIT( \
                    _reg_addr,  \
                    (_set_bit) | (_rst_bit),    \
                    ((_set_bit)&(_bit_mask)) | ((_rst_bit)&(~_bit_mask))    \
                  )   \
    }

/* macro used to set voltage level according to two bits */
#define SET_LEVEL(_reg_addr, _bit0_mask, _bit1_mask, _set_bit0,     \
                  _rst_bit0, _set_bit1, _rst_bit1)    \
{   \
    REG_SETBIT( \
                (_reg_addr),    \
                ((_set_bit0)|(_rst_bit0) | (_set_bit1)|(_rst_bit1)),  \
                (((_set_bit0)&(_bit0_mask)) | ((_rst_bit0)&(~_bit0_mask))|  \
                 ((_set_bit1)&(_bit1_mask)) | ((_rst_bit1)&(~_bit1_mask)))   \
              )   \
}

#define LDO_REG_OR ANA_REG_OR
#define LDO_REG_AND ANA_REG_AND
#define LDO_REG_GET  ANA_REG_GET

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/

#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif  //_LDO_REG_V3_H_


