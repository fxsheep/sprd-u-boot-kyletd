#include "mcu_command.h"
#include <asm/arch/packet.h>
#include "fdl_nand.h"
#include <linux/string.h>
#include <asm/arch/sci_types.h>
#include <asm/arch/sc_reg.h>

#include <asm/arch/cmddef.h>
#include <asm/arch/mocor_boot_mode.h>
#include <asm/arch/chip.h>

typedef void (*BOOT_ENTRY) (void);

/*****************************************************************************/
//  Description:    This function Reset MCU
//  Author:         Haifeng.Yang
//  Note:
/*****************************************************************************/
void ResetMCU (void)
{
    // Set watchdog reset flag
    BOOT_ResetHWFlag ();
    BOOT_SetWDGHWFlag (TYPE_RESET, AUTO_TEST_MODE);
    // Reset the system via watchdog timeout
    CHIP_ResetMCU ();

    while (1);
}
/* Sorry, I don't know what this function is for.
 *
 * Ming.Zhang 2005-09-22
 */
static void FDL_ResetMcuClock (void)
{
#ifdef MCU_CLK_52M
#define REG(r)      (*((volatile unsigned int*)(r)))

    REG (0x8b000018) |= 1 << 9; // write pll enable
    REG (0x8b000024)  = 0x1E05; // M/N = 5, so clock = 78M
    REG (0x8b000018) &= ~ (1 << 9); // write pll disable

    REG (GR_PCTL) = (unsigned int) (0x0A55);
#endif /* MCU_CLK_52M */
}

int FDL_McuResetBoot (PACKET_T *pakcet, void *arg)
{
    int i;
    BOOT_ENTRY boot_entry = (BOOT_ENTRY) 0; /* The address of ROM Code */

    FDL_SendAckPacket (BSL_REP_ACK);

    /* Wait until all characters are sent out. */
    for (i=0; i<0x0A000; i++)
    {
        /* Do nothing */;
    }

    FDL_ResetMcuClock();

    /* Jump to ROM code */
    (*boot_entry) (); /*lint !e413*/

    /* We should not go here */
    return 0;
}

int FDL_McuResetNormal (PACKET_T *packet, void *arg)
{
    int i;
    //BOOT_ENTRY boot_entry = (BOOT_ENTRY) 0x40000000; /* Start of internal RAM */
#if 0

    /* Copy NBL to internal RAM */
    if (NAND_SUCCESS != nand_read_NBL ( (void *) boot_entry))
    {
        send_ack_packet (BSL_REP_OPERATION_FAILED);
        return 0;
    }

#endif
    FDL_SendAckPacket (BSL_REP_ACK);

    /* Wait until all characters are sent out. */
    for (i=0; i<0x0A000; i++)
    {
        /* Do nothing */;
    }

    FDL_ResetMcuClock();

    ResetMCU();

    /* We should not go here */
    return 0;
}

int FDL_McuReadChipType (PACKET_T *packet, void *arg)
{
    unsigned int id;


    id   =  * (unsigned int *) (0x4FFC);

    if (0x660000B6 != id)
    {
        // @Richard We should check if we want to support 6600C
        id = 0x6600b500;
    }
    else
    {
        id = 0x6600b700;
    }

    packet->packet_body.type = BSL_REP_READ_CHIP_TYPE;
    packet->packet_body.size = 4;
    memcpy (packet->packet_body.content, &id, sizeof (unsigned int));

    FDL_SendPacket (packet);
    return 1;
}

