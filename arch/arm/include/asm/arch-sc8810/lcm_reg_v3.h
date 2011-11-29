/******************************************************************************
 ** File Name:    lcm_v3_reg.h                                           *
 ** Author:       Jianping.Wang                                               *
 ** DATE:         01/21/2006                                                  *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 01/21/2006    Jianping.Wang   Create.                                     *
 ******************************************************************************/
#ifndef _LCM_V3_REG_H_
#define _LCM_V3_REG_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                          Compiler Flag                                    *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Macro Define                                **
**----------------------------------------------------------------------------*/

#define LCM_CTRL                 (LCM_REG_BASE)
#define LCM_PARAMETER0           (LCM_REG_BASE + 0x04)
#define LCM_PARAMETER1           (LCM_REG_BASE + 0x08)
#define LCM_IFMODE               (LCM_REG_BASE + 0x0c)
#define LCM_RGBMODE              (LCM_REG_BASE + 0x10)
#define LCM_RDDATA               (LCM_REG_BASE + 0x14)
#define LCM_STATUS               (LCM_REG_BASE + 0x18)
#define LCM_RSTN                 (LCM_REG_BASE + 0x1c)
#define LCM_CD0                  (LCM_REG_BASE + 0x40)
#define LCM_DATA0                (LCM_REG_BASE + 0x44)
#define LCM_CD1                  (LCM_REG_BASE + 0x50)
#define LCM_DATA1                (LCM_REG_BASE + 0x54)



/**---------------------------------------------------------------------------*
**                               Data Prototypt                              **
**----------------------------------------------------------------------------*/
typedef union _lcm_ctrl_tag
{
    struct _lcm_ctrl_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   trans_num      :20;//[19:0]
        volatile  uint   cd_sel         :6;//[25:20]
        volatile  uint   dev_id         :3;//[28:26]
        volatile  uint   reserved       :3;//Reserved
#else
        volatile  uint   reserved       :3;//Reserved
        volatile  uint   dev_id         :3;//[28:26]
        volatile  uint   cd_sel         :6;//[25:20]
        volatile  uint   trans_num      :20;//[19:0]
#endif
    } mBits;
    volatile  uint   dwValue;
} LCM_CTRL_U;

typedef union _lcm_parameter0_tag
{
    struct _lcm_paramter0_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   whpw           :4;
        volatile  uint   wlpw           :4;
        volatile  uint   wcss           :2;
        volatile  uint   rhpw           :4;
        volatile  uint   rlpw           :4;
        volatile  uint   rcss           :2;
        volatile  uint   reserved       :12;//Reserved
#else
        volatile  uint   reserved       :12;//Reserved
        volatile  uint   rcss           :2;
        volatile  uint   rlpw           :4;
        volatile  uint   rhpw           :4;
        volatile  uint   wcss           :2;
        volatile  uint   wlpw           :4;
        volatile  uint   whpw           :4;
#endif
    } mBits;
    volatile  uint   dwValue;
} LCM_PARAMTER0_U;

typedef union _lcm_parameter1_tag
{
    struct _lcm_paramter1_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   whpw           :4;
        volatile  uint   wlpw           :4;
        volatile  uint   wcss           :2;
        volatile  uint   rhpw           :4;
        volatile  uint   rlpw           :4;
        volatile  uint   rcss           :2;
        volatile  uint   reserved       :12;//Reserved
#else
        volatile  uint   reserved       :12;//Reserved
        volatile  uint   rcss           :2;
        volatile  uint   rlpw           :4;
        volatile  uint   rhpw           :4;
        volatile  uint   wcss           :2;
        volatile  uint   wlpw           :4;
        volatile  uint   whpw           :4;
#endif
    } mBits;
    volatile  uint   dwValue;
} LCM_PARAMTER1_U;

typedef union _lcm_rstn_tag
{
    struct _lcm_rstn_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   rstn           :1;
        volatile  uint   reserved       :31;//Reserved
#else
        volatile  uint   reserved       :31;//Reserved
        volatile  uint   rstn           :1;
#endif
    } mBits;
    volatile  uint   dwValue;
} LCM_RSTN_U;



typedef union _lcm_rddata_tag
{
    struct _lcm_rddata_map
    {
        volatile  uint   rd_data        :32;
    } mBits;
    volatile  uint   dValue;
} LCM_RDDATA_U;

typedef union _lcm_status_tag
{
    struct _lcm_status_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   busy           :1;
        volatile  uint   fifo_full      :1;
        volatile  uint   fifo_empty     :1;
        volatile  uint   reserved2      :5;//Reserved
        volatile  uint   remain         :20;//Reserved
        volatile  uint   reserved1      :4;//Reserved
#else
        volatile  uint   reserved1      :4;//Reserved
        volatile  uint   remain         :20;//Reserved
        volatile  uint   reserved2      :5;//Reserved
        volatile  uint   fifo_empty     :1;
        volatile  uint   fifo_full      :1;
        volatile  uint   busy           :1;
#endif
    } mBits;
    volatile  uint   dValue;
} LCM_STATUS_U;

typedef union _lcm_rgbmode_tag
{
    struct _lcm_rgbmode_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   lcm_bits0      :2;
        volatile  uint   lcm_bits1      :2;
        volatile  uint   reserved       :28;//Reserved
#else
        volatile  uint   reserved       :28;//Reserved
        volatile  uint   lcm_bits1      :2;
        volatile  uint   lcm_bits0      :2;
#endif
    } mBits;
    volatile  uint   dwValue;
} LCM_RGBMODE_U;


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End




