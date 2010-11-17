/******************************************************************************
 ** File Name:      emc_drvapi.h                                                                                   *
 ** Author:         lin.liu                                                                                         *
 ** DATE:           17/12/2008                                                                                  *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.                                   *
 ** Description:    EMC drv public type and exprot interface declaration                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 27/11/2001     Lin.liu          Create.                                   *
 ** 17/12/2008    hyy       Modify
 ******************************************************************************/
#ifndef _EMC_DRVAPI_H_
#define _EMC_DRVAPI_H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

#if defined(PLATFORM_SC6600L)

#define DISABLE_CACHE   EMC_DisableCache();
#define ENABLE_CACHE    EMC_EnableCache();

#elif defined(PLATFORM_SC6800H)
/*Since MMU Config Flash Code region to read only, then before send flash
  command at code region should close MMU and Cache*/
#define DISABLE_CACHE   MMU_Disable();
#define ENABLE_CACHE     MMU_Enable();
#endif
#define EMC_REG(_ADDR)              (REG32(_ADDR))

#define EMC_CS_REG_INIT_FLAG        0x5a5a5a5a


typedef enum
{
    EMC_CS0 = 0,
    EMC_CS1,
    EMC_CS2,
    EMC_CS3,
    EMC_CS4,
    EMC_CS_MAX
}
EMC_CS_NUM_E;


typedef enum
{
    EMC_MEM_FLASH = 0,
    EMC_MEM_SRAM,
    EMC_MEM_LCM,
    EMC_MEM_OTHER
} EMC_CS_MEM_TYPE_E;

typedef enum
{
    EMC_READ_NORMAL_MODE    = 0x0,  //asynchronous single-mode read
    EMC_READ_PAGE_MODE          ,       //asynchronous page-mode read
    EMC_READ_BURST_MODE         //synchronous burst-mode read
} EMC_READ_MODE_E;

typedef enum
{
    EMC_WRITE_NORMAL_MODE = 0,//asynchronous single-mode write
    EMC_WRITE_BURST_MODE        //synchronous burst-mode write
} EMC_WRITE_MODE_E;

typedef enum
{
    EMC_LEN_SINGLE = 0x0,
    EMC_LEN_4 ,
    EMC_LEN_8 ,
    EMC_LEN_16 ,
    EMC_LEN_32,
    EMC_LEN_CONTINUOUS
} EMC_PAGE_BURST_LEN_E;

typedef struct emc_mode_cfg_s
{
    EMC_CS_MEM_TYPE_E       emc_cs_mem_type;         //flash,psram

    EMC_READ_MODE_E         emc_read_mode;
    EMC_PAGE_BURST_LEN_E    emc_read_len;
    EMC_WRITE_MODE_E        emc_write_mode;
    EMC_PAGE_BURST_LEN_E    emc_write_len;
    void                       *other;
} EMC_MODE_CFG_T, * EMC_MODE_CFG_PTR;

typedef void *EMC_TIMING_PTR;

typedef struct emc_reginfo_s
{
    EMC_CS_NUM_E            emc_cs;
    EMC_MODE_CFG_PTR    emc_cs_cfg_ptr;
    EMC_TIMING_PTR          emc_cs_mem_info_ptr;
    uint32              reg_flag;
} EMC_REGINFO_T;

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :
//      1.call FLASH_ReadID to get manufacture ID and device ID
//      2.call GetNorMCP_Entity to get normcp configuration from user table
//      3.call FLASH_InitHAL to init flash HAL
//  Global resource dependence :
//      s_normcp_entity_ptr
//      g_normcp_spec_ptr
//      g_Nor_Config_ptr
//      g_Nor_Driver_ptr
//
//  Author:
//      Younger.yang
//  Note:
//      code must be located at SRAM or internal-RAM.
//      must be called before EMC_Timing_cfg()
/*****************************************************************************/
PUBLIC void EMC_Init (void);

PUBLIC void EMC_RegMemoryInfo (EMC_CS_NUM_E cs_num, EMC_MODE_CFG_PTR emc_cs_cfg_ptr, EMC_TIMING_PTR emc_cs_mem_info_ptr);

/*****************************************************************************/
//  Description :
//  Author:
//      Younger.yang
//  Note:
//      Enable CS0 Cache
/*****************************************************************************/
void EMC_EnableCache (void);

/*****************************************************************************/
//  Description :
//  Author:
//      Younger.yang
//  Note:
//      Disable CS0 Cache
/*****************************************************************************/
void EMC_DisableCache (void);


/*****************************************************************************/
//  Description:    This function is used to set EMC reg value according different ahb clk.
//  Global resource dependence :
//  Author:          younger.yang
//  Note:
/*****************************************************************************/
PUBLIC void EMC_Timing_chng (uint32 ahb_clk);

/*****************************************************************************/
//  Description :
//  Set All EMC CS Parameter To Default.
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
PUBLIC void EMC_Close (void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _EMC_DRVAPI_H_
