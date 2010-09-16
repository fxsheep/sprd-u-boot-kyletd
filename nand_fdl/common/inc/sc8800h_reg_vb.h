/******************************************************************************
 ** File Name:    sc8800h_reg_vb.h                                            *
 ** Author:       Aiguo.Miao	                                              *
 ** DATE:         08/28/2007                                                  *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 08/28/2007    Aiguo.Miao      Create.                                     *
 ******************************************************************************/
#ifndef _SC8800H_REG_VB_H_
    #define _SC8800H_REG_VB_H_
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
#define ARM_VBC_BASE     				0x82003000

#ifdef CHIP_VER_8800H5
 //BASE ADDRESS: ARM_VBC_BASE			0x82003000	
#define VBDA0                   (ARM_VBC_BASE+0x0000)                   // (0x0000)
#define VBDA1                   (ARM_VBC_BASE+0x0004)                    // (0x0004)
#define VBAD0                   (ARM_VBC_BASE+0x0008) // (0x0008)
#define VBAD1                   (ARM_VBC_BASE+0x000C) // (0x000C)
#define VBBUFFERSIZE      (ARM_VBC_BASE+0x0010) // (0x0010)
#define VBADBUFFDTA      (ARM_VBC_BASE+0x0014) // (0x0014)
#define VBDABUFFDTA      (ARM_VBC_BASE+0x0018) // (0x0018)
#define VBADCNT               (ARM_VBC_BASE+0x001C) // (0x001c)
#define VBDACNT               (ARM_VBC_BASE+0x0020) // (0x0020)
#define VBDAICTL              (ARM_VBC_BASE+0x0024)               // (0x0024)
#define VBDAIIN                (ARM_VBC_BASE+0x0028) // (0x0028)
#define VBDAIOUT             (ARM_VBC_BASE+0x002C) // (0x002c)

//internal dolphin register map
#define ARM_VBC_ANA_BASE                (ARM_VBC_BASE+0x100)
#define VBAICR                 (ARM_VBC_ANA_BASE+0x0000)  //Audio Interface Control
#define VBCR1                   (ARM_VBC_ANA_BASE+0x0004)  //Control Register 1
#define VBCR2                   (ARM_VBC_ANA_BASE+0x0008)  //Control Register 2
#define VBCCR1                 (ARM_VBC_ANA_BASE+0x000C)  //Control Clock Register1
#define VBCCR2                 (ARM_VBC_ANA_BASE+0x0010)  //Control Clock Register2
#define VBPMR1                (ARM_VBC_ANA_BASE+0x0014)  //Power Mode Register 1
#define VBPMR2                (ARM_VBC_ANA_BASE+0x0018)  //Power Mode Register 2
#define VBCRR                   (ARM_VBC_ANA_BASE+0x001C)  //Control Ramp Register
#define VBICR                   (ARM_VBC_ANA_BASE+0x0020)  //Interrupt Control Register
#define VBIFR                   (ARM_VBC_ANA_BASE+0x0024)  //Interrupt Flag Register
#define VBCGR1                (ARM_VBC_ANA_BASE+0x0028)  //Control Gain Register 1
#define VBCGR2                (ARM_VBC_ANA_BASE+0x002C)  //Control Gain Register 2
#define VBCGR3                (ARM_VBC_ANA_BASE+0x0030)  //Control Gain Register 3
#define VBCGR8                (ARM_VBC_ANA_BASE+0x0044)  //Control Gain Register 4-->8
#define VBCGR9                (ARM_VBC_ANA_BASE+0x0048)  //Control Gain Register 5-->9
#define VBCGR10              (ARM_VBC_ANA_BASE+0x004C)  //Control Gain Register 6-->10
#define VBTR1                  (ARM_VBC_ANA_BASE+0x0050)  //Test Register 1
#define VBTR2                  (ARM_VBC_ANA_BASE+0x0054)  //Test Register 2
#else
#define VBCTRL0							(ARM_VBC_BASE+0x0000)
#define VBCTRL1                         (ARM_VBC_BASE+0x0004)
#define VBAUX                           (ARM_VBC_BASE+0x0008)
#define VBDA0                           (ARM_VBC_BASE+0x0010)
#define VBDA1                           (ARM_VBC_BASE+0x0014)
#define VBAD0                           (ARM_VBC_BASE+0x0018)
#define VBBUFFERSIZE                    (ARM_VBC_BASE+0x001c)
#define VBTEST                          (ARM_VBC_BASE+0x0020)
#define VBADBUFFDTA                     (ARM_VBC_BASE+0x0024)
#define VBANAP                          (ARM_VBC_BASE+0x0028)
#define VBDABUFFDTA                     (ARM_VBC_BASE+0x002c)
#define VBNACS                          (ARM_VBC_BASE+0x0030)
#define VBADCNT                         (ARM_VBC_BASE+0x0034)
#define VBSTATUS                        (ARM_VBC_BASE+0x0038)
#define VBDACNT                         (ARM_VBC_BASE+0x003c)
#define DACOEF0                         (ARM_VBC_BASE+0x0040)
#define DACOEF1                         (ARM_VBC_BASE+0x0044)
#define DACOEF2                         (ARM_VBC_BASE+0x0048)
#define DACOEFTOP                       (ARM_VBC_BASE+0x004c)
#define VDAIIN                          (ARM_VBC_BASE+0x0054)   //new
#define VDAIOUT                         (ARM_VBC_BASE+0x0058)   //new
#define VBAD1                           (ARM_VBC_BASE+0x005c)   //new
#define VOICE_IIR_CORE                  (ARM_VBC_BASE+0x0200)
#define COEFRAM127                      (ARM_VBC_BASE+0x02fc)
#endif




/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

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

