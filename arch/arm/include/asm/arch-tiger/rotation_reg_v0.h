/******************************************************************************
 ** File Name:    rotation_reg_v0.h                                          *
 ** Author:       Tim.zhu                                                     *
 ** DATE:         07/09/2009                                                  *
 ** Copyright:    2009 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 07/09/2009    Tim.zhu         Create.                                     *
 ******************************************************************************/
#ifndef _ROTATION_REG_V0_H_
#define _ROTATION_REG_V0_H_
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
#define ROT_SRC_STAR_ADDR           (ROT_REG_BASE + 0x00)
#define ROT_DST_STAR_ADDR           (ROT_REG_BASE + 0x04)
#define ROT_IMAGE_SIZE              (ROT_REG_BASE + 0x08)
#define ROT_CONTROL                 (ROT_REG_BASE + 0x0c)
#define ROT_DMA_CFG0                (ROT_REG_BASE + 0x10)
#define ROT_DMA_CFG1                (ROT_REG_BASE + 0x14)
#define ROT_DMA_SRC_ADD             (ROT_REG_BASE + 0x18)
#define ROT_DMA_DST_ADD             (ROT_REG_BASE + 0x1c)
#define ROT_DMA_LLPTR               (ROT_REG_BASE + 0x20)
#define ROT_DMA_SDI                 (ROT_REG_BASE + 0x24)
#define ROT_DMA_SBI                 (ROT_REG_BASE + 0x28)
#define ROT_DMA_DBI                 (ROT_REG_BASE + 0x2c)
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/
#define REG_ROTATION_REG_BASE               0x20800000
#define REG_ROTATION_SRC_ADDR           (REG_ROTATION_REG_BASE + 0x0200)
#define REG_ROTATION_DST_ADDR           (REG_ROTATION_REG_BASE + 0x0204)
#define REG_ROTATION_IMG_SIZE           (REG_ROTATION_REG_BASE + 0x0208)
#define REG_ROTATION_CTRL               (REG_ROTATION_REG_BASE + 0x020c)
#define REG_ROTATION_DMA_CHN_CFG0       (REG_ROTATION_REG_BASE + 0x0210)
#define REG_ROTATION_DMA_CHN_CFG1       (REG_ROTATION_REG_BASE + 0x0214)
#define REG_ROTATION_DMA_CHN_SRC_ADDR   (REG_ROTATION_REG_BASE + 0x0218)
#define REG_ROTATION_DMA_CHN_DST_ADDR   (REG_ROTATION_REG_BASE + 0x021c)
#define REG_ROTATION_DMA_CHN_LLPTR      (REG_ROTATION_REG_BASE + 0x0220)
#define REG_ROTATION_DMA_CHN_SDI        (REG_ROTATION_REG_BASE + 0x0224)
#define REG_ROTATION_DMA_CHN_SBI        (REG_ROTATION_REG_BASE + 0x0228)
#define REG_ROTATION_DMA_CHN_DBI        (REG_ROTATION_REG_BASE + 0x022c)

/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/
typedef union _rotation_src_addr_tag
{
    struct _rotation_src_addr_map
    {
        volatile unsigned int src_addr      :32;    // rotation src addr
    } mBits ;
    volatile unsigned int dwValue ;
} ROTATION_SRC_ADDR_U;

typedef union _rotation_dst_addr_tag
{
    struct _rotation_dst_addr_map
    {
        volatile unsigned int dst_addr      :32;    // rotation src addr
    } mBits ;
    volatile unsigned int dwValue ;
} ROTATION_DST_ADDR_U;


typedef union _rotation_img_size_tag
{
    struct _rotation_img_size_map
    {
        volatile unsigned int reserved_0        :6;  //[31:26] Reserved
        volatile unsigned int pixel_format  :2;  //[25:24] pixel format 1 bytes 2bytes 4bytes
        volatile unsigned int width         :12;  //[23: 12] image width
        volatile unsigned int height            :12;  //[11:0] image heighy
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_IMG_SIZE_U;

typedef union _rotation_ctrl_tag
{
    struct _rotation_ctrl_map
    {
        volatile unsigned int reserved_1        :28;     //[31:4] Reserved
        volatile unsigned int rotation_eb       :1;  //[3] rotaton eb
        volatile unsigned int rotation_mode :2;  //[2:1] rotation mode
        volatile unsigned int rot_mode      :1;  //[0] 0:normal mode 1:uv 422 mode
    } mBits;
    volatile unsigned int dValue;
} ROTATION_CTRL_U;

typedef union _rotation_dma_chn_cfg0_tag
{
    struct _rotation_dma_chn_cfg0_map
    {
        volatile unsigned int dma_chn_cfg0          :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_CHN_CFG0_U;

typedef union _rotation_dma_chn_cfg1_tag
{
    struct _rotation_dma_chn_cfg1_map
    {
        volatile unsigned int dma_chn_cfg1          :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_CHN_CFG1_U;

typedef union _rotation_dma_src_addr_tag
{
    struct _rotation_dma_src_addr_map
    {
        volatile unsigned int dma_src_addr          :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_SRC_ADDR_U;

typedef union _rotation_dma_dst_addr_tag
{
    struct _rotation_dma_dst_addr_map
    {
        volatile unsigned int dma_dst_addr          :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_DST_ADDR_U;

typedef union _rotation_dma_chn_llptr_tag
{
    struct _rotation_dma_chn_llptr_map
    {
        volatile unsigned int dma_chn_llptr         :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_CHN_LLPTR_U;

typedef union _rotation_dma_chn_sdi_tag
{
    struct _rotation_dma_chn_sdi_map
    {
        volatile unsigned int dma_chn_sdi           :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_CHN_SDI_U;

typedef union _rotation_dma_chn_sbi_tag
{
    struct _rotation_dma_chn_sbi_map
    {
        volatile unsigned int dma_chn_sbi           :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_CHN_SBI_U;

typedef union _rotation_dma_chn_dbi_tag
{
    struct _rotation_dma_chn_dbi_map
    {
        volatile unsigned int dma_chn_dbi           :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_CHN_DBI_U;

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

