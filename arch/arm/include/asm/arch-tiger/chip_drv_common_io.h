/******************************************************************************
 ** File Name:    chip_drv_common_io.h                                        *
 ** Author:       Steve.Zhan                                                  *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE                NAME            DESCRIPTION                           *
 **  06/05/2010          Steve.Zhan       Create.                             *
 ******************************************************************************/
#ifndef _CHIP_DRV_COMMON_IO_H_
#define _CHIP_DRV_COMMON_IO_H_
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
typedef uint32  DRV_WORD;
typedef uint8   DRV_BYTE;
typedef uint16  DRV_WORD16;
typedef uint32  DRV_WORD32;
typedef uint64  DRV_WORD64;
typedef uint32  DRV_ADDRWORD;
#define drv_count32 long


//Bit def
#ifndef BIT_0
#define BIT_0               0x00000001
#define BIT_1               0x00000002
#define BIT_2               0x00000004
#define BIT_3               0x00000008
#define BIT_4               0x00000010
#define BIT_5               0x00000020
#define BIT_6               0x00000040
#define BIT_7               0x00000080
#define BIT_8               0x00000100
#define BIT_9               0x00000200
#define BIT_10              0x00000400
#define BIT_11              0x00000800
#define BIT_12              0x00001000
#define BIT_13              0x00002000
#define BIT_14              0x00004000
#define BIT_15              0x00008000
#define BIT_16              0x00010000
#define BIT_17              0x00020000
#define BIT_18              0x00040000
#define BIT_19              0x00080000
#define BIT_20              0x00100000
#define BIT_21              0x00200000
#define BIT_22              0x00400000
#define BIT_23              0x00800000
#define BIT_24              0x01000000
#define BIT_25              0x02000000
#define BIT_26              0x04000000
#define BIT_27              0x08000000
#define BIT_28              0x10000000
#define BIT_29              0x20000000
#define BIT_30              0x40000000
#define BIT_31              0x80000000
#endif
#define BIT_(__n)       (1<<(__n))
#define MASK_(__n,__w)  (((1<<(__w))-1)<<(__n))
#define VALUE_(__n,__v) ((__v)<<(__n))

//
typedef uint32 ChipRegT;
//#define REG32(ChipReg)        ( *((volatile ChipRegT*) (ChipReg)))
#define REG32(x)                                    (*((volatile uint32 *)(x)))

typedef uint16 ChipRegT16;
#define REG16(ChipReg)        ( *((volatile ChipRegT16*) (ChipReg)))

#define REG8(ChipReg)  ( *((volatile uint8*) (ChipReg)))
#define CHIP_REG_OR(reg_addr, value)    (*(volatile uint32 *)(reg_addr) |= (uint32)(value))
#define CHIP_REG_AND(reg_addr, value)   (*(volatile uint32 *)(reg_addr) &= (uint32)(value))
#define CHIP_REG_GET(reg_addr)          (*(volatile uint32 *)(reg_addr))
#define CHIP_REG_SET(reg_addr, value)   (*(volatile uint32 *)(reg_addr)  = (uint32)(value))
#define SET_BIT(reg,bit)    ((reg) |= (1<<(bit)))
#define CLR_BIT(reg,bit)    ((reg) &= ~(1<<(bit)))
#define TST_BIT(a,b)        (((a)>>(b))&1)
#define XOR_BIT(a,b)        ((a) ^= (1<<(b)))

#undef  ARR_SIZE
#define ARR_SIZE(x)   (sizeof(x) / sizeof(x[0]) )

#undef  MIN
#define MIN(x,y)      (((x) < (y)) ? (x): (y))
#undef  MAX
#define MAX(x,y)      (((x) > (y)) ? (x): (y))

#undef  ABS
#define ABS(x)        (((x) < (0)) ? (-x):(x))

#define SWAP16(X)    ( ((X & 0xFF00) >> 8) | ((X & 0x00FF) << 8) )
#define SWAP32(X)    (((n) << 24) | (((n) & 0xff00) << 8) | (((n) >> 8) & 0xff00) | ((n) >> 24))
#define SWAP_AB(type,a,b) { type tmp; tmp=a; a=b; b=tmp; }

//

//Debug info Function and MacroS.

#if 1
#define CHIP_DRV_ASSERT SCI_ASSERT
#define CHIP_DRV_PASSERT SCI_PASSERT
#else
#define CHIP_DRV_ASSERT NULL
#define CHIP_DRV_PASSERT NULL
#endif
//
#ifndef DRV_LSBFIRST
#define DRV_LSBFIRST 1234
#define DRV_MSBFIRST 4321
#define DRV_BYTEORDER           DRV_MSBFIRST    // Big endian
#endif

#define DRV_MACRO_START do {
#define DRV_MACRO_END   } while (0)

#define DRV_EMPTY_STATEMENT DRV_MACRO_START DRV_MACRO_END

#define DRV_UNUSED_PARAM( _type_, _name_ ) DRV_MACRO_START      \
    _type_ __tmp1 = (_name_);                                     \
    _type_ __tmp2 = __tmp1;                                       \
    __tmp1 = __tmp2;                                              \
    DRV_MACRO_END



#ifndef HAL_IO_MACROS_DEFINED

//-----------------------------------------------------------------------------
// BYTE Register access.
// Individual and vectorized access to 8 bit registers.

// Little-endian version or big-endian version that doesn't need address munging
#if (DRV_BYTEORDER == DRV_LSBFIRST) || defined(HAL_IO_MACROS_NO_ADDRESS_MUNGING)

#define HAL_READ_UINT8( _register_, _value_ ) \
    ((_value_) = *((volatile DRV_BYTE *)(_register_)))

#define HAL_WRITE_UINT8( _register_, _value_ ) \
    (*((volatile DRV_BYTE *)(_register_)) = (_value_))

#define HAL_READ_UINT8_VECTOR( _register_, _buf_, _count_, _step_ )     \
    DRV_MACRO_START                                                     \
    drv_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
        (_buf_)[_i_] = ((volatile DRV_BYTE *)(_register_))[_j_];        \
    DRV_MACRO_END

#define HAL_WRITE_UINT8_VECTOR( _register_, _buf_, _count_, _step_ )    \
    DRV_MACRO_START                                                     \
    drv_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
        ((volatile DRV_BYTE *)(_register_))[_j_] = (_buf_)[_i_];        \
    DRV_MACRO_END

#define HAL_READ_UINT8_STRING( _register_, _buf_, _count_ )             \
    DRV_MACRO_START                                                     \
    drv_count32 _i_;                                                    \
    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
        (_buf_)[_i_] = ((volatile DRV_BYTE *)(_register_))[_i_];        \
    DRV_MACRO_END

#define HAL_WRITE_UINT8_STRING( _register_, _buf_, _count_ )            \
    DRV_MACRO_START                                                     \
    drv_count32 _i_;                                                    \
    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
        ((volatile DRV_BYTE *)(_register_)) = (_buf_)[_i_];             \
    DRV_MACRO_END

#else // Big-endian version

#define HAL_READ_UINT8( _register_, _value_ ) \
    ((_value_) = *((volatile DRV_BYTE *)((DRV_ADDRWORD)(_register_)^3)))

#define HAL_WRITE_UINT8( _register_, _value_ ) \
    (*((volatile DRV_BYTE *)((DRV_ADDRWORD)(_register_)^3)) = (_value_))

#define HAL_READ_UINT8_VECTOR( _register_, _buf_, _count_, _step_ )     \
    DRV_MACRO_START                                                     \
    drv_count32 _i_,_j_;                                                \
    volatile DRV_BYTE* _r_ = ((DRV_ADDRWORD)(_register_)^3);            \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
        (_buf_)[_i_] = _r_[_j_];                                        \
    DRV_MACRO_END

#define HAL_WRITE_UINT8_VECTOR( _register_, _buf_, _count_, _step_ )    \
    DRV_MACRO_START                                                     \
    drv_count32 _i_,_j_;                                                \
    volatile DRV_BYTE* _r_ = ((DRV_ADDRWORD)(_register_)^3);            \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
        _r_[_j_] = (_buf_)[_i_];                                        \
    DRV_MACRO_END

#define HAL_READ_UINT8_STRING( _register_, _buf_, _count_ )             \
    DRV_MACRO_START                                                     \
    drv_count32 _i_;                                                    \
    volatile DRV_BYTE* _r_ = ((DRV_ADDRWORD)(_register_)^3);            \
    for( _i_ = 0; _i_ < (_count_); _i_++;                               \
            (_buf_)[_i_] = _r_[_i_];                                        \
            DRV_MACRO_END

#define HAL_WRITE_UINT8_STRING( _register_, _buf_, _count_ )            \
    DRV_MACRO_START                                                     \
    drv_count32 _i_;                                                    \
    volatile DRV_BYTE* _r_ = ((DRV_ADDRWORD)(_register_)^3);            \
    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
        _r_[_i_] = (_buf_)[_i_];                                        \
    DRV_MACRO_END

#endif // Big-endian

//-----------------------------------------------------------------------------
// 16 bit access.
// Individual and vectorized access to 16 bit registers.

// Little-endian version or big-endian version that doesn't need address munging
#if (DRV_BYTEORDER == DRV_LSBFIRST) || defined(HAL_IO_MACROS_NO_ADDRESS_MUNGING)

#define HAL_READ_UINT16( _register_, _value_ ) \
    ((_value_) = *((volatile DRV_WORD16 *)(_register_)))

#define HAL_WRITE_UINT16( _register_, _value_ ) \
    (*((volatile DRV_WORD16 *)(_register_)) = (_value_))

#define HAL_READ_UINT16_VECTOR( _register_, _buf_, _count_, _step_ )    \
    DRV_MACRO_START                                                     \
    drv_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
        (_buf_)[_i_] = ((volatile DRV_WORD16 *)(_register_))[_j_];      \
    DRV_MACRO_END

#define HAL_WRITE_UINT16_VECTOR( _register_, _buf_, _count_, _step_ )   \
    DRV_MACRO_START                                                     \
    drv_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
        ((volatile DRV_WORD16 *)(_register_))[_j_] = (_buf_)[_i_];      \
    DRV_MACRO_END

#define HAL_READ_UINT16_STRING( _register_, _buf_, _count_)             \
    DRV_MACRO_START                                                     \
    drv_count32 _i_;                                                    \
    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
        (_buf_)[_i_] = ((volatile DRV_WORD16 *)(_register_))[_i_];      \
    DRV_MACRO_END

#define HAL_WRITE_UINT16_STRING( _register_, _buf_, _count_)            \
    DRV_MACRO_START                                                     \
    drv_count32 _i_;                                                    \
    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
        ((volatile DRV_WORD16 *)(_register_))[_i_] = (_buf_)[_i_];      \
    DRV_MACRO_END


#else // Big-endian version

#define HAL_READ_UINT16( _register_, _value_ ) \
    ((_value_) = *((volatile DRV_WORD16 *)((DRV_ADDRWORD)(_register_)^3)))

#define HAL_WRITE_UINT16( _register_, _value_ ) \
    (*((volatile DRV_WORD16 *)((DRV_ADDRWORD)(_register_)^3)) = (_value_))

#define HAL_READ_UINT16_VECTOR( _register_, _buf_, _count_, _step_ )    \
    DRV_MACRO_START                                                     \
    drv_count32 _i_,_j_;                                                \
    volatile DRV_WORD16* _r_ = ((DRV_ADDRWORD)(_register_)^3);          \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
        (_buf_)[_i_] = _r_[_j_];                                        \
    DRV_MACRO_END

#define HAL_WRITE_UINT16_VECTOR( _register_, _buf_, _count_, _step_ )   \
    DRV_MACRO_START                                                     \
    drv_count32 _i_,_j_;                                                \
    volatile DRV_WORD16* _r_ = ((DRV_ADDRWORD)(_register_)^3);          \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
        _r_[_j_] = (_buf_)[_i_];                                        \
    DRV_MACRO_END

#define HAL_READ_UINT16_STRING( _register_, _buf_, _count_)             \
    DRV_MACRO_START                                                     \
    drv_count32 _i_;                                                    \
    volatile DRV_WORD16* _r_ = ((DRV_ADDRWORD)(_register_)^3);          \
    for( _i_ = 0 = 0; _i_ < (_count_); _i_++)                           \
        (_buf_)[_i_] = _r_[_i_];                                        \
    DRV_MACRO_END

#define HAL_WRITE_UINT16_STRING( _register_, _buf_, _count_)            \
    DRV_MACRO_START                                                     \
    drv_count32 _i_;                                                    \
    volatile DRV_WORD16* _r_ = ((DRV_ADDRWORD)(_register_)^3);          \
    for( _i_ = 0 = 0; _i_ < (_count_); _i_++)                           \
        _r_[_i_] = (_buf_)[_i_];                                        \
    DRV_MACRO_END


#endif // Big-endian

//-----------------------------------------------------------------------------
// 32 bit access.
// Individual and vectorized access to 32 bit registers.

// Note: same macros for little- and big-endian systems.

#define HAL_READ_UINT32( _register_, _value_ ) \
    ((_value_) = *((volatile DRV_WORD32 *)(_register_)))

#define HAL_WRITE_UINT32( _register_, _value_ ) \
    (*((volatile DRV_WORD32 *)(_register_)) = (_value_))

#define HAL_READ_UINT32_VECTOR( _register_, _buf_, _count_, _step_ )    \
    DRV_MACRO_START                                                     \
    drv_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
        (_buf_)[_i_] = ((volatile DRV_WORD32 *)(_register_))[_j_];      \
    DRV_MACRO_END

#define HAL_WRITE_UINT32_VECTOR( _register_, _buf_, _count_, _step_ )   \
    DRV_MACRO_START                                                     \
    drv_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
        ((volatile DRV_WORD32 *)(_register_))[_j_] = (_buf_)[_i_];      \
    DRV_MACRO_END

#define HAL_READ_UINT32_STRING( _register_, _buf_, _count_)             \
    DRV_MACRO_START                                                     \
    drv_count32 _i_;                                                    \
    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
        (_buf_)[_i_] = ((volatile DRV_WORD32 *)(_register_))[_i_];      \
    DRV_MACRO_END

#define HAL_WRITE_UINT32_STRING( _register_, _buf_, _count_)            \
    DRV_MACRO_START                                                     \
    drv_count32 _i_;                                                    \
    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
        ((volatile DRV_WORD32 *)(_register_))[_i_] = (_buf_)[_i_];      \
    DRV_MACRO_END


#define HAL_IO_MACROS_DEFINED

#endif // !HAL_IO_MACROS_DEFINED


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

