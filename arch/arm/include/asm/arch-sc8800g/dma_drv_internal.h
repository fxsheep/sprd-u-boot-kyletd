/******************************************************************************
 ** File Name:    dma_drv_internal.h                                          *
 ** Author:       Guofu.Huang                                                 *
 ** DATE:         2010-2-28                                                   *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  dma driver infterface specfication                          *
 **               Note:In DMA7, data blcok in DMA9 was named busrt,           *
 **               Here we all used block.*
 **               any advice, please fell free to contact me.                 *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 2010-2-9      Guofu.Huang     Created, Rewrite dma driver                 *
 ******************************************************************************/
#ifndef _DMA_DRV_INTERNAL_H_
#define _DMA_DRV_INTERNAL_H_
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
#define DMA_CHANNELID_MAX                   (DMA_CH_NUM-1)//0 - 31
#define DMA_CHANNELID_INVALID               (0xFFFFFFFF)
#define DMA_LINKLIST_HANDLE_INVALID         (0xFFFFFFFF)
#define DMA_UID_SOFT                        (0)
#define DMA_UID_MAX                         (32)

#define DMA_LIST_SIZE_MAX (64)//max length used in both linklist and soflist mode

//---------------------------------------------------------------------------
// INT type macros
//
//int status BIT: 3bits value
#define DMA_INT_BIT_BLOCK  0   // bit 0
#define DMA_INT_BIT_TRANS  1   // bit 1
#define DMA_INT_BIT_LIST   2   // bit 2

#define DMA_CHN_INT_TYPE_NONE               0
#define DMA_CHN_INT_TYPE_BLOCKDONE          (1<<DMA_INT_BIT_BLOCK)
#define DMA_CHN_INT_TYPE_TRANSDONE          (1<<DMA_INT_BIT_TRANS)
#define DMA_CHN_INT_TYPE_LISTDONE           (1<<DMA_INT_BIT_LIST)

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
//int status type: 3bits value
//DMA_CHN_INT_TYPE_BLOCKDONE | DMA_CHN_INT_TYPE_TRANSDONE | DMA_CHN_INT_TYPE_LISTDONE
typedef void (*DMA_HAL_ISR_HANDLER) (uint32 inttype);

typedef uint32 DMA_LINKLIST_HANDLE;

//DMA priority
typedef enum _DMA_CHN_PRIORITY
{
    DMA_CHN_PRIORITY_0  = 0, // LOWEST PRI
    DMA_CHN_PRIORITY_1,
    DMA_CHN_PRIORITY_2,
    DMA_CHN_PRIORITY_3,      // HIGHIST
    DMA_CHN_PRIORITY_MAX
} DMA_CHN_PRIORITY_E;

//endian type
typedef enum _DMA_ENDIANTYPE
{
    DMA_ENDIANTYPE_BIG = 0,
    DMA_ENDIANTYPE_LITTLE,
    DMA_ENDIANTYPE_MAX
} DMA_ENDIANTYPE_E;

//endian switch mode
typedef enum _DMA_ENDIANSWITCHMODE
{
    DMA_ENDIANSWITCHMODE_UN = 0,
    DMA_ENDIANSWITCHMODE_FULL,
    DMA_ENDIANSWITCHMODE_MODE0,
    DMA_ENDIANSWITCHMODE_MODE1,
    DMA_ENDIANSWITCHMODE_MAX
} DMA_ENDIANSWITCHMODE_E;

//data width
typedef enum _DMA_DATAWIDTH
{
    DMA_DATAWIDTH_BYTE = 0,
    DMA_DATAWIDTH_HALFWORD,
    DMA_DATAWIDTH_WORD,
    DMA_DATAWIDTH_MAX
} DMA_DATAWIDTH_E;

//work mode
typedef enum _DMA_CHN_WORKMODE
{
    DMA_CHN_WORKMODE_NORMAL = 0,
    DMA_CHN_WORKMODE_LINKLIST,
    DMA_CHN_WORKMODE_SOFTLIST,
    DMA_CHN_WORKMODE_MAX
} DMA_CHN_WORKMODE_E;

//request mode
typedef enum _DMA_CHN_REQMODE
{
    DMA_CHN_REQMODE_NORMAL = 0,
    DMA_CHN_REQMODE_TRASACTION,
    DMA_CHN_REQMODE_LIST,
    DMA_CHN_REQMODE_INFINITE,
    DMA_CHN_REQMODE_MAX
} DMA_CHN_REQMODE_E;

//burst mode
typedef enum _DMA_BLOCKMODE
{
    DMA_BLOCKMODE_SINGLETRANS  = 0,
    DMA_BLOCKMODE_INCR,
    DMA_BLOCKMODE_INCR4,
    DMA_BLOCKMODE_INCR8,
    DMA_BLOCKMODE_INCR16,
    DMA_BLOCKMODE_MAX
} DMA_BLOCKMODE_E;

//dma channel config struct
typedef struct _DMA_ChnCfg_Struct
{
    // normal config param, these param maybe used in most scence
    DMA_CHN_TYPE_E          type;              /* hardware or software         */
    DMA_CHN_PRIORITY_E      priority;          /* dma channel priority         */
    DMA_CHN_WORKMODE_E      workmode;          /* normal/linklist/soflist      */
    DMA_DATAWIDTH_E         src_datawidth;     /* source data width            */
    DMA_DATAWIDTH_E         dest_datawidth;    /* dest data width              */
    uint32                  src_address;       /* src start address            */
    uint32                  dest_address;      /* des start address            */
    DMA_CHN_REQMODE_E       requestmode;       /* request mode                 */
    uint16                  block_length;      /* block length                 */
    uint32                  total_length;      /* total length                 */
    int32                   src_step;          /* SrcElementPost-modification  */
    int32                   dest_step;         /* DestElementPost-modification */
    DMA_BLOCKMODE_E         src_blockmode;     /* INCR4,8m16                   */
    DMA_BLOCKMODE_E         dest_blockmode;    /* INCR4,8m16                   */
    int32                   src_blockstep ;    /* SrcBlockPost-modification    */
    int32                   dest_blockstep;    /* SrcBlockPost-modification    */
    BOOLEAN                 is_srcwrapen;      /* src wrap enable              */
    BOOLEAN                 is_destwrapen;     /* dst wrap enable              */
    uint32                  wrapstart_addr;    /* wrap start addr              */
    uint32                  wrapend_addr;      /* wrap end addr                */
    BOOLEAN                 is_ll_end;         /* linklist end flag            */
    uint32                  ll_next;           /* point to next link list node */
    DMA_ENDIANSWITCHMODE_E  endian_switchmode; /* endian switch mode           */
    BOOLEAN                 is_noautoclose;    /* no auto close                */
    DMA_ENDIANTYPE_E        endian_type;       /* endian selection             */
    //uint8                   uid;               /* dma channel uid  0-31        */

} DMA_CHNCFG_T, *DMA_CHNCFG_PTR;

//DMA error type
typedef enum _DMA_ERR_TYPE_E
{
    DMA_ERR_TYPE_NONE = 0,
    DMA_ERR_TYPE_INVALID_CHNID,
    DMA_ERR_TYPE_INVALID_PARAM,
    DMA_ERR_TYPE_CHNTYPE_MISMATCHED,
    DMA_ERR_TYPE_NOT_AVAILABLE,
    DMA_ERR_TYPE_LIST_OVERRUN,
    DMA_ERR_TYPE_LIST_NODATA,
    DMA_ERR_TYPE_LIST_APPENDFAIL,
    DMA_ERR_TYPE_MAX
} DMA_ERR_TYPE_E;


/**----------------------------------------------------------------------------*
**                         Public Function Prototype                          **
**----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    dma driver init
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_Init (void);

/*****************************************************************************/
//  Description:    set dma pause
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           s_pause:SCI_TRUE,pause dma; SCI_FALSE,resume dma
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_SetPause (BOOLEAN is_pause);

/*****************************************************************************/
//  Description:    alloc soft dma channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           if success return channel id, otherwise return DMA_CHANNELID_INVALID
/*****************************************************************************/
PUBLIC uint32 DMA_HAL_AllocSoftChn (void);

/*****************************************************************************/
//  Description:    release channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_FreeSoftChn (uint32 channel);

/*****************************************************************************/
//  Description:    configure a dma channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:channel to be configured
//                  chncfgstruct:channel configure struct
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_ChnConfig (uint32 channel, DMA_CHNCFG_PTR chnparam);

/*****************************************************************************/
//  Description:    set dma priority
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//                  priority:channel priority,0-3
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_SetChnPriority (uint32 channel,DMA_CHN_PRIORITY_E priority);

/*****************************************************************************/
//  Description:    dma request enable/disable
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel :  channel No
//                  is_enable: enable/disable
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_ChnEnable (uint32 channel, BOOLEAN is_enable);

/*****************************************************************************/
//  Description:    get dma channel transdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_PollingTransDone (uint32 channel);

/*****************************************************************************/
//  Description:    get dma channel blockdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_PollingBlockDone (uint32 channel);

/*****************************************************************************/
//  Description:    get dma channel listdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_PollingListDone (uint32 channel);

/*****************************************************************************/
//  Description:    configure dma channel interrupt
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//                  inttype:interrupt type,including all transfer done,burst done and link done.
//                  is_enable:SCI_TRUE,enable the interrupt, SCI_FALSE,disable the interrupt
//                  callback:interrupt service,if is_enable=SCI_FALSE, it is unuseful.
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_ChnIntConfig (uint32 channel, uint32 inttype, DMA_HAL_ISR_HANDLER callback);

/*****************************************************************************/
//  Description:    init a link transfer
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel id
//  Return value:   if success return link list handle, otherwise return  DMA_LINKLIST_HANDLE_INVALID
/*****************************************************************************/
PUBLIC DMA_LINKLIST_HANDLE DMA_HAL_InitLinkList (uint32 channel);

/*****************************************************************************/
//  Description:    append a link element
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           linklisthandle:link list handle
//                  linklistelement:element to append
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC DMA_ERR_TYPE_E DMA_HAL_AppendInitLinkList (DMA_LINKLIST_HANDLE linklisthandle,
        DMA_CHNCFG_PTR param);

/*****************************************************************************/
//  Description:    submit a link list and start transfer
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           linklisthandle: link list handle
//                  is_int_used:polling or interrupt,if SCI_TRUE, intterrupt used
//                  callback: if interrupt used,callback need
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC DMA_ERR_TYPE_E DMA_HAL_SubmitInitLinkList (DMA_LINKLIST_HANDLE linklisthandle);

/*****************************************************************************/
//  Description:    free link list
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           linklisthandle:link list to release
/*****************************************************************************/
PUBLIC void DMA_HAL_FreeLinkList (DMA_LINKLIST_HANDLE linklisthandle);

/*****************************************************************************/
// Description :    This function is used to enable or disable dma pause function.
//  Global resource dependence : s_dma_pause_disable_cnt
//  Author :
//  Note : is_enable :   TURE: enable dma pause function; FALSE: disable dma pause function
/*****************************************************************************/
PUBLIC void DMA_HAL_EnableDMAPause (BOOLEAN is_enable);

/*****************************************************************************/
// Description :    This function is used to get the function status of dma pause
// Global resource dependence : s_dma_pause_disable_cnt
// Author :
// Note : TURE: dma pause function is enabled; FALSE: dma pause function is disabled
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_IsEnableDMAPause (void);

/*****************************************************************************/
// Description :    This function is used to get the dma pause status
// Global resource dependence : s_is_dma_pause
// Author :
// Note : TURE: dma pause function is enabled; FALSE: dma pause function is disabled
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_IsDMAPause (void);
//
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_DMA_DRV_INTERNAL_H_
// End of File

