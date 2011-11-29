/******************************************************************************
 ** File Name:    dma_reg_v3.h                                                *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         11/13/2005                                                  *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 11/13/2005    Daniel.Ding     Create.                                     *
 ** 01/29/2007    Aiguo.Miao      Port to SC8800H                             *
 ** 05/02/2007    Tao.Zhou        Modify it for SC8800H.                      *
 ** 05/06/2010    Mingwei.zhang   Modify it for SC8800G.                      *
 ******************************************************************************/
#ifndef _DMA_REG_V3_H_
#define _DMA_REG_V3_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/

//0X00
#define DMA_CFG                         (DMA_REG_BASE + 0x0000)
#define DMA_CHx_EN_STATUS               (DMA_REG_BASE + 0x0004)
#define DMA_LINKLIST_EN                 (DMA_REG_BASE + 0x0008)
#define DMA_SOFTLINK_EN                 (DMA_REG_BASE + 0x000C)
#define DMA_SOFTLIST_SIZE               (DMA_REG_BASE + 0x0010)
#define DMA_SOFTLIST_CMD                (DMA_REG_BASE + 0x0014)
#define DMA_SOFTLIST_STS                (DMA_REG_BASE + 0x0018)
#define DMA_SOFTLIST_BASEADDR           (DMA_REG_BASE + 0x001C)
//0X20
#define DMA_PRI_REG0                    (DMA_REG_BASE + 0x0020)
#define DMA_PRI_REG1                    (DMA_REG_BASE + 0x0024)
//0X30
#define DMA_INT_STS                     (DMA_REG_BASE + 0x0030)
#define DMA_INT_RAW                     (DMA_REG_BASE + 0x0034)
//0X40
#define DMA_LISTDONE_INT_EN             (DMA_REG_BASE + 0x0040)
#define DMA_BURST_INT_EN                (DMA_REG_BASE + 0x0044)
#define DMA_TRANSF_INT_EN               (DMA_REG_BASE + 0x0048)
//0X50
#define DMA_LISTDONE_INT_STS            (DMA_REG_BASE + 0x0050)
#define DMA_BURST_INT_STS               (DMA_REG_BASE + 0x0054)
#define DMA_TRANSF_INT_STS              (DMA_REG_BASE + 0x0058)
//0X60
#define DMA_LISTDONE_INT_RAW            (DMA_REG_BASE + 0x0060)
#define DMA_BURST_INT_RAW               (DMA_REG_BASE + 0x0064)
#define DMA_TRANSF_INT_RAW              (DMA_REG_BASE + 0x0068)
//0X70
#define DMA_LISTDONE_INT_CLR            (DMA_REG_BASE + 0x0070)
#define DMA_BURST_INT_CLR               (DMA_REG_BASE + 0x0074)
#define DMA_TRANSF_INT_CLR              (DMA_REG_BASE + 0x0078)
//0X80
#define DMA_SOFT_REQ                    (DMA_REG_BASE + 0x0080)
#define DMA_TRANS_STS                   (DMA_REG_BASE + 0x0084)//for debug
#define DMA_REQ_PEND                    (DMA_REG_BASE + 0x0088)//for debug
//0X90
#define DMA_WRAP_START                  (DMA_REG_BASE + 0x0090)
#define DMA_WRAP_END                    (DMA_REG_BASE + 0x0094)

#define DMA_CHN_UID_BASE                (DMA_REG_BASE + 0x0098)
#define DMA_CHN_UID0                    (DMA_REG_BASE + 0x0098)
#define DMA_CHN_UID1                    (DMA_REG_BASE + 0x009C)
#define DMA_CHN_UID2                    (DMA_REG_BASE + 0x00A0)
#define DMA_CHN_UID3                    (DMA_REG_BASE + 0x00A4)
#define DMA_CHN_UID4                    (DMA_REG_BASE + 0x00A8)
#define DMA_CHN_UID5                    (DMA_REG_BASE + 0x00AC)
#define DMA_CHN_UID6                    (DMA_REG_BASE + 0x00B0)
#define DMA_CHN_UID7                    (DMA_REG_BASE + 0x00B4)

#define DMA_CHx_EN                      (DMA_REG_BASE + 0x00C0)
#define DMA_CHx_DIS                     (DMA_REG_BASE + 0x00C4)

//Channel x dma contral regisers address ;
#define DMA_CHx_CTL_BASE                (DMA_REG_BASE + 0x0400)
#define DMA_CHx_BASE(x)                 (DMA_CHx_CTL_BASE + 0x20 * (x) )
#define DMA_CHx_CFG0(x)                 (DMA_CHx_CTL_BASE + 0x20 * (x) + 0x0000)
#define DMA_CHx_CFG1(x)                 (DMA_CHx_CTL_BASE + 0x20 * (x) + 0x0004)
#define DMA_CHx_SRC_ADDR(x)             (DMA_CHx_CTL_BASE + 0x20 * (x) + 0x0008)
#define DMA_CHx_DEST_ADDR(x)            (DMA_CHx_CTL_BASE + 0x20 * (x) + 0x000c)
#define DMA_CHx_LLPTR(x)                (DMA_CHx_CTL_BASE + 0x20 * (x) + 0x0010)
#define DMA_CHx_SDEP(x)                 (DMA_CHx_CTL_BASE + 0x20 * (x) + 0x0014)
#define DMA_CHx_SBP(x)                  (DMA_CHx_CTL_BASE + 0x20 * (x) + 0x0018)
#define DMA_CHx_DBP(x)                  (DMA_CHx_CTL_BASE + 0x20 * (x) + 0x001c)

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
/*lint -save -e530 -e533 */

/////////////////////////////
//add new dma driver
/////////////////////////////
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
#define DMA_SOFTBLOCK_MASK               (0xFFFF0000)
#define DMA_SOFTBLOCK_OFFSET              16

#define DMA_HARDBLOCK_MASK               (0x000000FF)
#define DMA_HARDBLOCK_OFFSET              0

#define DMA_PAUSE_MASK                   (0x00000100)
#define DMA_PAUSE_ENABLE                 (0x00000100)
#define DMA_PAUSE_DISABLE                (0x00000000)

#define DMA_WRAP_ADDR_MASK               (0x0FFFFFFF)
#define DMA_WRAP_ADDR_OFFSET              0

//channel priority
#define DMA_PRI_BITS                      2
#define DMA_PRI_CHx_PER_REG               16
#define DMA_CHx_PRI_INDEX(x)              (x/DMA_PRI_CHx_PER_REG)
#define DMA_CHx_PRI_OFFSET(x)             (x%DMA_PRI_CHx_PER_REG)*DMA_PRI_BITS

#define DMA_CHx_PRIORITY_MASK             (0x00000003)
#define DMA_CHx_PRIORITY_0                (0x00000000)
#define DMA_CHx_PRIORITY_1                (0x00000001)
#define DMA_CHx_PRIORITY_2                (0x00000002)
#define DMA_CHx_PRIORITY_3                (0x00000003)

//channel user id  Register
#define DMA_UID_BITS                     8
#define DMA_UID_CHx_PER_REG              4
#define DMA_CHx_UID_INDEX(x)             (x/DMA_UID_CHx_PER_REG)
#define DMA_CHx_UID_OFFSET(x)            ((x%DMA_UID_CHx_PER_REG)*DMA_UID_BITS)

#define DMA_CHx_UID_MASK                 (0x0000001F)

#define DMA_SOFTLIST_SIZE_MASK           (0x0000FFFF)
#define DMA_SOFTLIST_APPENDLEN_MASK      (0x0000FFFF)
#define DMA_SOFTLIST_APPENDLEN_OFFSET    0

#define DMA_SOFTLIST_CURINDEX_MASK       (0xFFFF0000)
#define DMA_SOFTLIST_CURINDEX_OFFSET     (16)
#define DMA_SOFTLIST_LEFT_MASK           (0x0000FFFF)
#define DMA_SOFTLIST_LEFT_OFFSET         (0)

//Channel x dma contral regisers address ;
#define DMA_CHx_CTL_REG_BASE(x)          (DMA_REG_BASE + 0x0400 + 0x20 * x )

//LL End
#define DMA_CHx_LL_END_MASK              (0x80000000)
#define DMA_CHx_LL_END_YES               (0x80000000)
#define DMA_CHx_LL_END_NO                (0x00000000)

//endaian type
#define DMA_CHx_ENDIANSEL_MASK           (0x40000000)
#define DMA_CHx_ENDIANSEL_BIG            (0x00000000)
#define DMA_CHx_ENDIANSEL_LITTLE         (0x40000000)

//endaian switch mode
#define DMA_CHx_ENDIANSWMODESEL_MASK     (0x30000000)
#define DMA_CHx_ENDIANSWMODESEL_UN       (0x00000000)
#define DMA_CHx_ENDIANSWMODESEL_FULL     (0x10000000)
#define DMA_CHx_ENDIANSWMODESEL_MODE0    (0x20000000)
#define DMA_CHx_ENDIANSWMODESEL_MODE1    (0x30000000)

//src data width
#define DMA_CHx_SRC_DATAWIDTH_MASK       (0x0C000000)
#define DMA_CHx_SRC_DATAWIDTH_BYTE       (0x00000000)
#define DMA_CHx_SRC_DATAWIDTH_HALFWORD   (0x04000000)
#define DMA_CHx_SRC_DATAWIDTH_WORD       (0x08000000)

//des data width
#define DMA_CHx_DEST_DATAWIDTH_MASK      (0x03000000)
#define DMA_CHx_DEST_DATAWIDTH_BYTE      (0x00000000)
#define DMA_CHx_DEST_DATAWIDTH_HALFWORD  (0x01000000)
#define DMA_CHx_DEST_DATAWIDTH_WORD      (0x02000000)

//request mode
#define DMA_CHx_REQMODE_MASK             (0x00C00000)
#define DMA_CHx_REQMODE_NORMAL           (0x00000000)
#define DMA_CHx_REQMODE_TRANSACTION      (0x00400000)
#define DMA_CHx_REQMODE_LIST             (0x00800000)
#define DMA_CHx_REQMODE_INFINITE         (0x00C00000)

//src wrap enable
#define DMA_CHx_SRCWRAP_MASK             (0x00200000)
#define DMA_CHx_SRCWRAP_DISABLE          (0x0000000)
#define DMA_CHx_SRCWRAP_ENABLE           (0x00200000)

//dest wrap enable
#define DMA_CHx_DESTWRAP_MASK            (0x00100000)
#define DMA_CHx_DESTWRAP_DISABLE         (0x00000000)
#define DMA_CHx_DESTWRAP_ENABLE          (0x00100000)

//no auto close
#define DMA_CHx_NOAUTO_CLOSE_MASK        (0x00020000)
#define DMA_CHx_NOAUTO_CLOSE_NO          (0x00000000)
#define DMA_CHx_NOAUTO_CLOSE_YES         (0x00020000)

//blocke length bits
#define DMA_CHx_BLOCKLENGTH_MASK         (0x0000FFFF)
#define DMA_CHx_BLOCKLENGTH_OFFSET        0

//src/des step bits
#define DMA_CHx_SRC_STEP_MASK            (0xFFFF0000)
#define DMA_CHx_SRC_STEP_OFFSET          16
#define DMA_CHx_DEST_STEP_MASK           (0x0000FFFF)
#define DMA_CHx_DEST_STEP_OFFSET         0
//total length
#define DMA_CHx_TOTALLENGTH_MASK         (0x01FFFFFF)
#define DMA_CHx_TOTALLENGTH_OFFSET        0
//block mode
#define DMA_CHx_BLOCKMODE_MASK           (0x70000000)
#define DMA_CHx_BLOCKMODE_SIGNLE         (0x00000000)
#define DMA_CHx_BLOCKMODE_INCR           (0x10000000)
#define DMA_CHx_BLOCKMODE_INCR4          (0x30000000)
#define DMA_CHx_BLOCKMODE_INCR8          (0x50000000)
#define DMA_CHx_BLOCKMODE_INCR16         (0x70000000)

#define DMA_CHx_BLOCKSTEP_MASK           (0x03FFFFFF)
#define DMA_CHx_BLOCKSTEP_OFFSET         (0)

#define DMA_PRI_REG_COUNT                2
#define DMA_UID_REG_COUNT                8

#define DMA_PAUSE_STAT_BIT               (0x40000000)

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
/*lint -save -e530 -e533 */
//dma general control register arry
typedef struct _DMA_GEN_CTL_REG
{
    volatile uint32     cfg;                          //0x0000
    volatile uint32     chx_en_sts;                   //0x0004
    volatile uint32     linklist_en;                  //0x0008
    volatile uint32     softlist_en;                  //0x000c

    volatile uint32     softlist_size;                //0x0010
    volatile uint32     softlist_cmd;                 //0x0014
    volatile uint32     softlist_sts;                 //0x0018
    volatile uint32     softlist_baseaddr;            //0x001c

    volatile uint32     priority[DMA_PRI_REG_COUNT];  //0x0020-0x0024
    volatile uint32     reserve1[2];                  //0x0028-0x002c

    volatile uint32     int_masksts;                  //0x0030
    volatile uint32     int_rawsts;                   //0x0034
    volatile uint32     reserve2[2];                  //0x0038-0x003c

    volatile uint32     listdone_int_en;              //0x0040
    volatile uint32     blockdone_int_en;             //0x0044
    volatile uint32     transdone_int_en;             //0x0048
    volatile uint32     reserver3;                    //0x004c

    volatile uint32     listdone_int_masksts;         //0x0050
    volatile uint32     blockdone_int_masksts;        //0x0054
    volatile uint32     transdone_int_masksts;        //0x0058
    volatile uint32     reserver4;                    //0x005c

    volatile uint32     listdone_int_rawsts;          //0x0060
    volatile uint32     blockdone_int_rawsts;         //0x0064
    volatile uint32     transdone_int_rawsts;        //0x0068
    volatile uint32     reserve5;                     //0x006c

    volatile uint32     listdone_int_clr;             //0x0070
    volatile uint32     blockdone_int_clr;            //0x0074
    volatile uint32     transdone_int_clr;            //0x0078
    volatile uint32     reserve6;                     //0x007c

    volatile uint32     soft_request;                 //0x0080
    volatile uint32     transfer_sts;                 //0x0084 for debug
    volatile uint32     req_pend;                     //0x0088 for debug
    volatile uint32     reserve7;                     //0x008c

    volatile uint32     wrap_startaddr;               //0x0090
    volatile uint32     wrap_endaddr;                 //0x0094
    volatile uint32     uid[DMA_UID_REG_COUNT];       //0x0098-0x00b4
    volatile uint32     reserve8[2];                   //0x00b8-0x00bc
    volatile uint32     chx_en;                        //0x00c0
    volatile uint32     chx_dis;                       //0x00c4
} DMA_GEN_CTL_REG_T;

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End





