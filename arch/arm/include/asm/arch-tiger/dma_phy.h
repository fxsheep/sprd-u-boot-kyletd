/******************************************************************************
 ** File Name:    dma_phy.h                                             *
 ** Author:       Jeff.Li                                                     *
 ** DATE:         12/06/2010                                                  *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 2005-9-17     weihua.wang     Create.                                     *
 ** 2005-12-28    weihua.wang     modify the macro def of DMA_MAX_TRANSSIZE   *
 **                               DMA_MAX_TRANSSIZE and include file for 6800 *
 ** 12/06/2010    Jeff.Li         Modify for new chip driver architecture.    *
 ******************************************************************************/

#ifndef _DMA_PHY_H_
#define _DMA_PHY_H_

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**-----------------------------------------------------------------------------*
**                                Macro for interface
**------------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
#if defined(PLATFORM_SC6600L)
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
ISR_EXE_T DMA_IrqHandler (uint32 num);

//*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
void DMA_HALInit (void);

/*****************************************************************************/
//  Description:    dma hw channel config
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           channel : dma channel no
//                  dma_type: hard dma/soft dma
//                  int_type: burst/alltransfer/none
//                  chn_param: pointer to the dma channel param
/*****************************************************************************/
uint32 DMA_HALChannelCfg (uint32 channel, uint32 dma_type,uint32 int_type, DMA_CTL_CFG_PTR chn_param);

/*****************************************************************************/
//  Description:    dma hw channel config
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           channel : dma channel no
//                  dma_type: hard dma/soft dma
//                  int_type: burst/alltransfer/none
//                  chn_param: pointer to the dma channel param
/*****************************************************************************/
uint32 DMA_HALlinklistCfg (DMA_CTL_CFG_PTR chn_param,DMA_CHx_CTL_T *listlinkcfg);

//*****************************************************************************/
//  Description:    dma interrupt call back register
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
uint32 DMA_HALRegCallBack (uint32 channel, DMA_ISR_HANDLER callback);

/*****************************************************************************/
void DMA_SetBlockDelay (uint32 delay);

#elif defined(PLATFORM_SC6800H) || defined(PLATFORM_SC8800G) || defined(PLATFORM_SC8800H)|| defined(CONFIG_SC8810)
//=============================================================================
// add function for new dma driver
//=============================================================================

/**---------------------------------------------------------------------------*
**                             Data Prototype                                **
**----------------------------------------------------------------------------*/
//dma channel control register array
//this regster struct is used both by dma_phy and dma.c, it is also used as logic element for list mode
//so this struct is moved from dma_reg_v0.h to dma_phy.h
typedef struct _DMA_CHN_CTL_REG
{
    volatile uint32     cfg;
    volatile uint32     total_length;
    volatile uint32     src_addr;
    volatile uint32     dest_addr;
    volatile uint32     ll_ptr;
    volatile uint32     sdep;
    volatile uint32     sbp;
    volatile uint32     dbp;
} DMA_CHN_CTL_REG_T, *DMA_CHN_CTL_REG_T_PTR;

/**---------------------------------------------------------------------------*
**                           Function Prototype                              **
**----------------------------------------------------------------------------*/

//*****************************************************************************/
//  Description:    Init DMA controller
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:
/*****************************************************************************/
PUBLIC void DMA_PHY_Init (void);

//*****************************************************************************/
//  Description:    pause or resume dma transfer
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           is_pause: SCI_TRUE=pause,SCI_FALSE=resume
/*****************************************************************************/
PUBLIC void DMA_PHY_SetPause (BOOLEAN is_pause);

//*****************************************************************************/
//  Description:    setting dma wrap addr
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           wrapstart_addr: start addr
//                  wrapend_addr:end addr
/*****************************************************************************/
PUBLIC void DMA_PHY_SetWrapAddr (uint32 wrapstart_addr, uint32 wrapend_addr);

//*****************************************************************************/
//  Description:    setting dma channnel user id
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  uid:channel user id
/*****************************************************************************/
PUBLIC void DMA_PHY_SetChnUID (uint32 channel, uint32 uid);

//*****************************************************************************/
//  Description:    setting dma channnel work mode
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  workmode:channel work mode
/*****************************************************************************/
PUBLIC void DMA_PHY_SetChnWorkMode (uint32 channel, DMA_CHN_WORKMODE_E workmode);
//*****************************************************************************/
//  Description:    setting dma channel priority
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  priority: new priority value
/*****************************************************************************/
PUBLIC void DMA_PHY_SetChnPriority (uint32 chn, DMA_CHN_PRIORITY_E pri);

//*****************************************************************************/
//  Description:    Enable/Disable a dma channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  is_enable: enable/disable a chn
/*****************************************************************************/
PUBLIC void DMA_PHY_ChnEnable (uint32 channel, BOOLEAN is_enable);

//*****************************************************************************/
//  Description:    configure dma interrupt
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  inttype: dma chn interrupt type
//                  is_enable:enable/disable interrupt
/*****************************************************************************/
PUBLIC void DMA_PHY_ChnIntConfig (uint32 channel, uint32 inttype, DMA_HAL_ISR_HANDLER callback);

//*****************************************************************************/
//  Description:    configure dma channel related register
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           chncfgstruct: dma chn configure structure
//                  chn_ctl_ptr: a pointer of dma chn control register array
/*****************************************************************************/
PUBLIC void DMA_PHY_FillChnCtlReg (DMA_CHNCFG_PTR chnparam, DMA_CHN_CTL_REG_T_PTR dma_chn_ctl_ptr);
//*****************************************************************************/
//  Description:    configure dma channel related register
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  chncfgstruct: dma chn configure structure
/*****************************************************************************/
PUBLIC void DMA_PHY_ChnConfig (uint32 channel,DMA_CHNCFG_PTR chnparam);

/*****************************************************************************/
//  Description:    get dma channel transdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_PHY_PollingTransDone (uint32 channel);

/*****************************************************************************/
//  Description:    get dma channel blockdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_PHY_PollingBlockDone (uint32 channel);

/*****************************************************************************/
//  Description:    get dma channel listdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_PHY_PollingListDone (uint32 channel);

//*****************************************************************************/
//  Description:    get next nodeptr of list when using link list or soft list
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: dma channel index
//  Return Value:   the ptr of next node
/*****************************************************************************/
PUBLIC uint32 DMA_PHY_GetLinkListLLPtr (uint32 channel);

//*****************************************************************************/
//  Description:    check the current tranfser is the last node in the list
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: dma channel index
//  Return Value:   if LL_END is valid return SCI_TRUE, else return SCI_FALSE
/*****************************************************************************/
PUBLIC BOOLEAN DMA_PHY_IsLinkListLLEnd (uint32 channel);
//*****************************************************************************/
//  Description:    Set the node is the end
//  Global resource dependence:
//  Author:         Guofu.Huang
//                  node_ptr:pointer of the node to be modified
//                  is_ll_end: is Linklist end or not
//  Note:           return the ptr of next node
/*****************************************************************************/
PUBLIC void DMA_PHY_SetLinkListNodeLLEnd (DMA_CHN_CTL_REG_T_PTR node_ptr, BOOLEAN is_ll_end);

/*****************************************************************************/
#endif

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
