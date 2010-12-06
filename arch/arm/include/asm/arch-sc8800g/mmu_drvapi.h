/******************************************************************************
 ** File Name:    mmu_drvapi.h                                                    *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         8/16/2010                                                    *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 8/16/2010     steve.zhan      Create.                                     *
 ******************************************************************************/
#ifndef _MMU_DRVAPI_H_
#define _MMU_DRVAPI_H_
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
#define  MMU_PGD_ITEM_NUM    0x1000

typedef uint32   PGD_T;
typedef uint32   PTE_T;
typedef unsigned long pgdir_t;


/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

typedef enum
{
    MMU_SECTION_MAPPING = 0x00002,
    MMU_LARGE_PAGE_MAPPING_COARSE = 0x11,
    MMU_SMALL_PAGE_MAPPING_COARSE = 0x21,

    MMU_LARGE_PAGE_MAPPING_FINE = 0x13,
    MMU_SMALL_PAGE_MAPPING_FINE =0x23,
    MMU_TINY_PAGE_MAPPING_FINE = 0x33,

    MMU_FAULT_PAGE = 0x0000
} MMU_MapType_E;

#define PTE_TYPE_MASK2(x) ((x & 0xf0)>>4)
#define GET_PGTABLE_TYPE(x)     (x&0x3)

/* access permission */
typedef enum
{
    MMU_AP_NO_ACCESS  =0,//control by S, R bits
    MMU_AP_SVC_R      =0,
    MMU_AP_SVC_RW     =0x1,
    MMU_AP_NO_USR_W   =0x2,
    MMU_AP_ALL_ACCESS =0x3,
    MMU_AP_INVALID_PG =0x4
} MMU_AccessP_E;


/* cache and buffer */
typedef enum
{
    PAGE_CB_UNCACHED = 0,         /* uncached/unbuffered */
    PAGE_CB_BUFFERED = 0x1,       /* uncached/buffered */
    PAGE_CB_WT = 0x2,             /* write through */
    PAGE_CB_WB =0x3               /* write back */
} MMU_CacheAttr_E;

typedef enum
{
    MMU_DOMAIN_CLIENT_ACCESS = 0,
    MMU_DOMAIN_ALL_ACCESS    = 1 ,
    MMU_DOMAIN_NO_ACCESS = 2 ,
    MMU_DOMAIN_3 ,
    MMU_DOMAIN_4 ,
    MMU_DOMAIN_5 ,
    MMU_DOMAIN_6 ,
    MMU_DOMAIN_7 ,
    MMU_DOMAIN_8 ,
    MMU_DOMAIN_9 ,
    MMU_DOMAIN_10 ,
    MMU_DOMAIN_11 ,
    MMU_DOMAIN_12 ,
    MMU_DOMAIN_13 ,
    MMU_DOMAIN_14 ,
    MMU_DOMAIN_15 ,
    MMU_NUM_DOMAINS
} MMU_Domain_E;

typedef struct  MMU_MMAP_CFG_Tag
{
    uint32    vm_addr;
    uint32    phy_addr;
    uint32    size_kbytes;
    MMU_MapType_E   map_type;

    MMU_AccessP_E   ap[4];

    MMU_CacheAttr_E  cache_attr;
    MMU_Domain_E  domain;
} MMU_MMAP_CFG_T;


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

PUBLIC void VM_Init (void);
PUBLIC BOOLEAN VM_IsDPSection (uint32 addr);
/*****************************************************************************/
//  Description:   Change MMU setting in assert mode
//  Global resource dependence:
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC void MMU_AlignFaultDisable (void);
PUBLIC void MMU_AlignFaultEnable (void);


typedef BOOLEAN (* MMU_FAULT_CALLBACK) (uint32, void *);
typedef enum
{
    PREFETCH_FAULT,
    I_ABORT_FAULT,
    MMU_FAULT_MAX//Pls don't modify this.
} MMU_FAULT_TYPE_E;

BOOLEAN MMU_HandlePrefetch (uint32 r14_abt);
BOOLEAN MMU_HandleAbort (uint32 dfsr_reg, uint32 far_reg);
PUBLIC BOOLEAN MMU_FaultRegCallback (MMU_FAULT_TYPE_E type, MMU_FAULT_CALLBACK func);


typedef struct
{
    uint32 map_item_cnt;
    MMU_MMAP_CFG_T *map_p;

} MMU_CFG_TABLE_T;


/*****************************************************************************/
//  Description : Set mmu tlb address before using mmu any api.
//  Note:   if ony MMU SECTION is specified, you can specify NULL and 0 for "PageTableEntery"
//          and "PageTableSize"
//  Global resource dependence :
//  Author: steve.zhan
//  Note:
/*****************************************************************************/
PUBLIC void MMU_SetPageAddr (uint32 *PageTlbAddr, uint32 *PageTableEntery, uint32 PageTableSize);


/*****************************************************************************/
//  Description : Set memory region Area. According to the compiler generates
//  Note:   if no data is specified, you can specify NULL and 0.
//  Global resource dependence :
//  Author: steve.zhan
//  Note:
/*****************************************************************************/
PUBLIC void MMU_PHY_SetMemRegionMap (MMU_MMAP_CFG_T *pM,uint32 item_num);


/*****************************************************************************/
//  Description : Special area. For example, if need ro area IN RW section(come
//                from compiler linking) Or need rw area IN RO section(come from
//                compiler linking), you can list in the table, as a special
//                section dealing with.
//  Note:   if no data is specified, you can specify NULL and 0.
//  Global resource dependence :
//  Author: steve.zhan
//  Note:
/*****************************************************************************/
PUBLIC void MMU_PHY_SetSpecialRegionMap (MMU_MMAP_CFG_T *pS,uint32 item_num);



/*****************************************************************************/
//  Description : Set Read Only Region. If you need to make an Ro section, changing
//                between in the RO and RW, you can specify in the table.
//  Note:   if no data is specified, you can specify NULL and 0.
//  Global resource dependence :
//  Author: steve.zhan
//  Note:
/*****************************************************************************/
PUBLIC void MMU_PHY_SetProtectRoRegion (MMU_MMAP_CFG_T *pP,uint32 item_num);

PUBLIC void MMU_Init (void);
PUBLIC void MMU_Enable (void);
PUBLIC void MMU_Disable (void);
PUBLIC uint32  MMU_Map_Section_Page (MMU_MMAP_CFG_T *map_cfg);
PUBLIC uint32  MMU_Map_Page (MMU_MMAP_CFG_T *map_cfg, MMU_MapType_E PageType);
PUBLIC void MMU_ProtectRoRegion (uint32 isprotected);
PUBLIC void MMU_IcacheSync (void);
PUBLIC void MMU_DcacheSync (void);
PUBLIC void MMU_CacheOn (uint32 i, uint32 d);
PUBLIC void MMU_CacheOff (uint32 i, uint32 d);
PUBLIC void MMU_InvalideDCACHE (void);
PUBLIC void MMU_EnableICacheOnly(void);

/*****************************************************************************/
// Description :    This function delay some ticks .
// // Global resource dependence :
// // Author :         Daniel.ding
// // Note :
// /*****************************************************************************/
PUBLIC void MMU_InvalideDCACHE (void);


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif // _MMU_DRVAPI_H_
