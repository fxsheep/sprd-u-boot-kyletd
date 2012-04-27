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
#include <common.h>
#include <asm/io.h>

#include <asm/arch/regs_adi.h>
#include <asm/arch/adi_hal_internal.h>
#include <asm/proc/system.h>

#define TIMEOUT_ADI	(300)//for 8810 fpga
#define CHIP_REG_OR(reg_addr, value)    (*(volatile unsigned int *)(reg_addr) |= (unsigned int)(value))
#define CHIP_REG_AND(reg_addr, value)   (*(volatile unsigned int *)(reg_addr) &= (unsigned int)(value))
#define CHIP_REG_GET(reg_addr)          (*(volatile unsigned int *)(reg_addr))
#define CHIP_REG_SET(reg_addr, value)   (*(volatile unsigned int *)(reg_addr)  = (unsigned int)(value))

#define SCI_ASSERT(condition) BUG_ON(!(condition))  
#define SCI_PASSERT(condition, format...)  \
	do {		\
		if(!(condition)) { \
			printf("function :%s\r\n", __FUNCTION__);\
			BUG();	\
		} \
	}while(0)
	
#define ADI_PHYS	ADI_BASE
#define __adi_virt_to_phys(x) ((x) - SPRD_ADI_BASE + ADI_PHYS)

/*****************************************************************************
 *  Description:    this function performs read operation to the analog die reg .   *
 *                      it will disable all interrupt and polling the ready bit,        *
 *              and return a half-word value after read complete.             *
 *  Global Resource Dependence:                                              *
 *  Author: Tim Luo                                                        *
 *  Note:   return register value                                               *
******************************************************************************/
unsigned short ADI_Analogdie_reg_read (unsigned int addr)

{
	unsigned int adi_rd_data;
	unsigned long flags;
	int timeout = TIMEOUT_ADI;
	local_irq_save(flags);
   // SCI_DisableIRQ();
   // SCI_DisableFIQ();

    //SCI_ASSERT ( (addr>=ANA_REG_ADDR_START) && (addr<=ANA_REG_ADDR_END));

    //Set read command
   addr = __adi_virt_to_phys(addr);
    CHIP_REG_SET (ADI_ARM_RD_CMD, addr);

    //wait read operation complete, RD_data[31] will be cleared after the read operation complete
	do {
		adi_rd_data = CHIP_REG_GET (ADI_RD_DATA);		
		if (!timeout--)
			break;
	} while (adi_rd_data & BIT_31);
	
    //rd_data high part should be the address of the last read operation
    //SCI_ASSERT ( (adi_rd_data & 0xFFFF0000) == ((addr) <<16));

    //read operation complete
    //SCI_RestoreFIQ();
    //SCI_RestoreIRQ();
	local_irq_restore(flags);
	
    return ( (unsigned short) (adi_rd_data & 0x0000FFFF));

}

/*****************************************************************************
 *  Description:    this function performs write operation to the analog die reg .   *
 *                      it will write the analog die register if the fifo is not full       *
 *              It will polling the fifo full status til it is not full                  *
 *  Global Resource Dependence:                                              *
 *  Author: Tim Luo                                                        *
 *  Note:                                                                      *
******************************************************************************/
void ADI_Analogdie_reg_write (unsigned int addr, unsigned short data)
{
	
	int timeout = TIMEOUT_ADI;
	do {////ADI_wait_fifo_empty
		if ( ( (CHIP_REG_GET (ADI_FIFO_STS) & ( (unsigned int) ADI_FIFO_EMPTY)) != 0))
		{
			break;
		}
		if (!timeout--)
			return;//NEED TO return ERROR.
	} while (1);/*lint !e506*/

	CHIP_REG_SET (addr, data);
}


/*****************************************************************************
 *  Description:    this function is used to init analog to digital module.   *
 *                      it will enable adi_acc and soft reset adi_module,        *
 *              and then config the priority of each channel.             *
 *  Global Resource Dependence:                                              *
 *  Author: Tim Luo                                                        *
 *  Note:                                                                                     *
******************************************************************************/
void ADI_init (void)
{
    // volatile ADI_CFG_REG_T *adi_handle = (ADI_CFG_REG_T *) ADI_BASE_ADDR;

    //enable ADI_ACC to put the adi master to normal operation mode
    CHIP_REG_OR (GR_GEN0, GEN0_ADI_EN);

    //reset ADI module
    CHIP_REG_OR (GR_SOFT_RST, ADI_SOFT_RST);
    {
        unsigned int wait = 50;

        while (wait--);
    }
    CHIP_REG_AND (GR_SOFT_RST, (~ADI_SOFT_RST));

    //Please refer to Section 5. Program guide, SC8800G Analog-Digital Interface module Implementation Specifications.doc
    CHIP_REG_AND (ADI_CTL_REG, (~ARM_SERCLK_EN));

    //config channel priority
    CHIP_REG_SET (ADI_CHANNEL_PRI, ( (0<<INT_STEAL_PRI) | (1<<STC_WR_PRI) | (0<<ARM_WR_PRI)
                                     | (0<<ARM_RD_PRI) | (0<<DSP_WR_PRI) | (0<<DSP_RD_PRI)
                                     | (1<<RFT_WR_PRI) | (1<<PD_WR_PRI)));

}
