/******************************************************************************
 ** File Name:    sc8810_plf_io.h                                            *
 ** Author:       Tim.Luo                                                    *
 ** DATE:         07/08/2010                                                  *
 ** Copyright:    2010  Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 07/08/2010    Tim.Luo         Create.                                     *
 ******************************************************************************/
#ifndef _SC8810_PLF_IO_H_
#define _SC8810_PLF_IO_H_
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

//#define DRV_LSBFIRST 1234
//#define DRV_MSBFIRST 4321
//#define DRV_BYTEORDER           DRV_MSBFIRST    // Big endian
//
//
//#define DRV_INTERRUPT_FUNCTIONS_ENABLE
//
////-----------------------------------------------------------------------------
//// BYTE Register access.
//// Individual and vectorized access to 8 bit registers.
//
//// Little-endian version or big-endian version that doesn't need address munging
//#if (DRV_BYTEORDER == DRV_LSBFIRST) || defined(HAL_IO_MACROS_NO_ADDRESS_MUNGING)
//
//#define HAL_READ_UINT8( _register_, _value_ ) \
//    ((_value_) = *((volatile DRV_BYTE *)(_register_)))
//
//#define HAL_WRITE_UINT8( _register_, _value_ ) \
//    (*((volatile DRV_BYTE *)(_register_)) = (_value_))
//
//#define HAL_READ_UINT8_VECTOR( _register_, _buf_, _count_, _step_ )     \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_,_j_;                                                \
//    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
//        (_buf_)[_i_] = ((volatile DRV_BYTE *)(_register_))[_j_];        \
//    DRV_MACRO_END
//
//#define HAL_WRITE_UINT8_VECTOR( _register_, _buf_, _count_, _step_ )    \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_,_j_;                                                \
//    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
//        ((volatile DRV_BYTE *)(_register_))[_j_] = (_buf_)[_i_];        \
//    DRV_MACRO_END
//
//#define HAL_READ_UINT8_STRING( _register_, _buf_, _count_ )             \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_;                                                    \
//    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
//        (_buf_)[_i_] = ((volatile DRV_BYTE *)(_register_))[_i_];        \
//    DRV_MACRO_END
//
//#define HAL_WRITE_UINT8_STRING( _register_, _buf_, _count_ )            \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_;                                                    \
//    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
//        ((volatile DRV_BYTE *)(_register_)) = (_buf_)[_i_];             \
//    DRV_MACRO_END
//
//#else // Big-endian version
//
//#define HAL_READ_UINT8( _register_, _value_ ) \
//    ((_value_) = *((volatile DRV_BYTE *)((DRV_ADDRWORD)(_register_)^3)))
//
//#define HAL_WRITE_UINT8( _register_, _value_ ) \
//    (*((volatile DRV_BYTE *)((DRV_ADDRWORD)(_register_)^3)) = (_value_))
//
//#define HAL_READ_UINT8_VECTOR( _register_, _buf_, _count_, _step_ )     \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_,_j_;                                                \
//    volatile DRV_BYTE* _r_ = ((DRV_ADDRWORD)(_register_)^3);            \
//    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
//        (_buf_)[_i_] = _r_[_j_];                                        \
//    DRV_MACRO_END
//
//#define HAL_WRITE_UINT8_VECTOR( _register_, _buf_, _count_, _step_ )    \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_,_j_;                                                \
//    volatile DRV_BYTE* _r_ = ((DRV_ADDRWORD)(_register_)^3);            \
//    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
//        _r_[_j_] = (_buf_)[_i_];                                        \
//    DRV_MACRO_END
//
//#define HAL_READ_UINT8_STRING( _register_, _buf_, _count_ )             \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_;                                                    \
//    volatile DRV_BYTE* _r_ = ((DRV_ADDRWORD)(_register_)^3);            \
//    for( _i_ = 0; _i_ < (_count_); _i_++;                               \
//            (_buf_)[_i_] = _r_[_i_];                                        \
//            DRV_MACRO_END
//
//#define HAL_WRITE_UINT8_STRING( _register_, _buf_, _count_ )            \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_;                                                    \
//    volatile DRV_BYTE* _r_ = ((DRV_ADDRWORD)(_register_)^3);            \
//    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
//        _r_[_i_] = (_buf_)[_i_];                                        \
//    DRV_MACRO_END
//
//#endif // Big-endian
//
////-----------------------------------------------------------------------------
//// 16 bit access.
//// Individual and vectorized access to 16 bit registers.
//
//// Little-endian version or big-endian version that doesn't need address munging
//#if (DRV_BYTEORDER == DRV_LSBFIRST) || defined(HAL_IO_MACROS_NO_ADDRESS_MUNGING)
//
//#define HAL_READ_UINT16( _register_, _value_ ) \
//    ((_value_) = *((volatile DRV_WORD16 *)(_register_)))
//
//#define HAL_WRITE_UINT16( _register_, _value_ ) \
//    (*((volatile DRV_WORD16 *)(_register_)) = (_value_))
//
//#define HAL_READ_UINT16_VECTOR( _register_, _buf_, _count_, _step_ )    \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_,_j_;                                                \
//    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
//        (_buf_)[_i_] = ((volatile DRV_WORD16 *)(_register_))[_j_];      \
//    DRV_MACRO_END
//
//#define HAL_WRITE_UINT16_VECTOR( _register_, _buf_, _count_, _step_ )   \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_,_j_;                                                \
//    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
//        ((volatile DRV_WORD16 *)(_register_))[_j_] = (_buf_)[_i_];      \
//    DRV_MACRO_END
//
//#define HAL_READ_UINT16_STRING( _register_, _buf_, _count_)             \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_;                                                    \
//    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
//        (_buf_)[_i_] = ((volatile DRV_WORD16 *)(_register_))[_i_];      \
//    DRV_MACRO_END
//
//#define HAL_WRITE_UINT16_STRING( _register_, _buf_, _count_)            \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_;                                                    \
//    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
//        ((volatile DRV_WORD16 *)(_register_))[_i_] = (_buf_)[_i_];      \
//    DRV_MACRO_END
//
//
//#else // Big-endian version
//
//#define HAL_READ_UINT16( _register_, _value_ ) \
//    ((_value_) = *((volatile DRV_WORD16 *)((DRV_ADDRWORD)(_register_)^3)))
//
//#define HAL_WRITE_UINT16( _register_, _value_ ) \
//    (*((volatile DRV_WORD16 *)((DRV_ADDRWORD)(_register_)^3)) = (_value_))
//
//#define HAL_READ_UINT16_VECTOR( _register_, _buf_, _count_, _step_ )    \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_,_j_;                                                \
//    volatile DRV_WORD16* _r_ = ((DRV_ADDRWORD)(_register_)^3);          \
//    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
//        (_buf_)[_i_] = _r_[_j_];                                        \
//    DRV_MACRO_END
//
//#define HAL_WRITE_UINT16_VECTOR( _register_, _buf_, _count_, _step_ )   \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_,_j_;                                                \
//    volatile DRV_WORD16* _r_ = ((DRV_ADDRWORD)(_register_)^3);          \
//    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
//        _r_[_j_] = (_buf_)[_i_];                                        \
//    DRV_MACRO_END
//
//#define HAL_READ_UINT16_STRING( _register_, _buf_, _count_)             \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_;                                                    \
//    volatile DRV_WORD16* _r_ = ((DRV_ADDRWORD)(_register_)^3);          \
//    for( _i_ = 0 = 0; _i_ < (_count_); _i_++)                           \
//        (_buf_)[_i_] = _r_[_i_];                                        \
//    DRV_MACRO_END
//
//#define HAL_WRITE_UINT16_STRING( _register_, _buf_, _count_)            \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_;                                                    \
//    volatile DRV_WORD16* _r_ = ((DRV_ADDRWORD)(_register_)^3);          \
//    for( _i_ = 0 = 0; _i_ < (_count_); _i_++)                           \
//        _r_[_i_] = (_buf_)[_i_];                                        \
//    DRV_MACRO_END
//
//
//#endif // Big-endian
//
////-----------------------------------------------------------------------------
//// 32 bit access.
//// Individual and vectorized access to 32 bit registers.
//
//// Note: same macros for little- and big-endian systems.
//
//#define HAL_READ_UINT32( _register_, _value_ ) \
//    ((_value_) = *((volatile DRV_WORD32 *)(_register_)))
//
//#define HAL_WRITE_UINT32( _register_, _value_ ) \
//    (*((volatile DRV_WORD32 *)(_register_)) = (_value_))
//
//#define HAL_READ_UINT32_VECTOR( _register_, _buf_, _count_, _step_ )    \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_,_j_;                                                \
//    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
//        (_buf_)[_i_] = ((volatile DRV_WORD32 *)(_register_))[_j_];      \
//    DRV_MACRO_END
//
//#define HAL_WRITE_UINT32_VECTOR( _register_, _buf_, _count_, _step_ )   \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_,_j_;                                                \
//    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
//        ((volatile DRV_WORD32 *)(_register_))[_j_] = (_buf_)[_i_];      \
//    DRV_MACRO_END
//
//#define HAL_READ_UINT32_STRING( _register_, _buf_, _count_)             \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_;                                                    \
//    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
//        (_buf_)[_i_] = ((volatile DRV_WORD32 *)(_register_))[_i_];      \
//    DRV_MACRO_END
//
//#define HAL_WRITE_UINT32_STRING( _register_, _buf_, _count_)            \
//    DRV_MACRO_START                                                     \
//    drv_count32 _i_;                                                    \
//    for( _i_ = 0; _i_ < (_count_); _i_++)                               \
//        ((volatile DRV_WORD32 *)(_register_))[_i_] = (_buf_)[_i_];      \
//    DRV_MACRO_END
//
//
//#define HAL_IO_MACROS_DEFINED


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
