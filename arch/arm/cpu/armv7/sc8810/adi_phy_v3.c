/******************************************************************************
 ** File Name:      adi_phy_v3.c                                                 *
 ** Author:         tim.luo                                             *
 ** DATE:           2/25/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 Analog to Digital Module.                                       *
 **                                                                                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2/25/2010     Tim Luo      Create.                                   *
 **                                                                                                *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sc_reg.h"
#include "adi_hal_internal.h"

#if 0
typedef struct ADI_CFG_REG_TAG
{
    volatile uint32 clk_div;
    volatile uint32 ctl_reg;
    volatile uint32 channel_pri;
    volatile uint32 int_en;
    volatile uint32 int_raw_sts;
    volatile uint32 int_mask_st;
    volatile uint32 int_clr;
    volatile uint32   reserved_0;
    volatile uint32   reserved_1;
    volatile uint32 arm_rd_cmd;
    volatile uint32 rd_data;
    volatile uint32 fifo_sts;
    volatile uint32 sts;
    volatile uint32 req_sts;
} ADI_CFG_REG_T;
#endif

/*****************************************************************************
 *  Description:    this function performs read operation to the analog die reg .   *
 *                      it will disable all interrupt and polling the ready bit,        *
 *              and return a half-word value after read complete.             *
 *  Global Resource Dependence:                                              *
 *  Author: Tim Luo                                                        *
 *  Note:   return register value                                               *
******************************************************************************/
PUBLIC uint16 ADI_Analogdie_reg_read (uint32 addr)

{
    uint32 adi_rd_data;


    //Set read command
    CHIP_REG_SET(ADI_ARM_RD_CMD, addr);
    
    //wait read operation complete, RD_data[31] will be cleared after the read operation complete
    do
    {
        adi_rd_data = CHIP_REG_GET(ADI_RD_DATA);
    }
    while (adi_rd_data & BIT_31);


    return ( (uint16) (adi_rd_data & 0x0000FFFF));

}
/*****************************************************************************
 *  Description:    this function performs write operation to the analog die reg .   *
 *                      it will write the analog die register if the fifo is not full       *
 *              It will polling the fifo full status til it is not full                  *
 *  Global Resource Dependence:                                              *
 *  Author: Tim Luo                                                        *
 *  Note:                                                                      *
******************************************************************************/
PUBLIC  void ADI_Analogdie_reg_write (uint32 addr, uint16 data)
{

    do{             ////ADI_wait_fifo_empty
        if (((CHIP_REG_GET(ADI_FIFO_STS) & ((uint32)ADI_FIFO_EMPTY)) != 0))
        {
            break;    
        }
    }while(1);

    CHIP_REG_SET(addr, data);
}
 

/*****************************************************************************
 *  Description:    this function is used to init analog to digital module.   *
 *                      it will enable adi_acc and soft reset adi_module,        *
 *              and then config the priority of each channel.             *
 *  Global Resource Dependence:                                              *
 *  Author: Tim Luo                                                        *
 *  Note:                                                                                     *
******************************************************************************/
PUBLIC void ADI_init (void)
{
   // volatile ADI_CFG_REG_T *adi_handle = (ADI_CFG_REG_T *) ADI_BASE_ADDR;

    //enable ADI_ACC to put the adi master to normal operation mode
    CHIP_REG_OR(GR_GEN0, GEN0_ADI_EN);

    //reset ADI module
    CHIP_REG_OR(GR_SOFT_RST, ADI_SOFT_RST);
    {
        uint32 wait = 50;

        while (wait--);
    }
    CHIP_REG_AND(GR_SOFT_RST, (~ADI_SOFT_RST));

    //Please refer to Section 5. Program guide, SC8800G Analog-Digital Interface module Implementation Specifications.doc
    CHIP_REG_AND(ADI_CTL_REG,(~ARM_SERCLK_EN));

    //config channel priority
    CHIP_REG_SET(ADI_CHANNEL_PRI, ((0<<INT_STEAL_PRI) | (1<<STC_WR_PRI) | (0<<ARM_WR_PRI)
                              | (0<<ARM_RD_PRI) | (0<<DSP_WR_PRI) | (0<<DSP_RD_PRI)
                              | (1<<RFT_WR_PRI) | (1<<PD_WR_PRI)));

}


