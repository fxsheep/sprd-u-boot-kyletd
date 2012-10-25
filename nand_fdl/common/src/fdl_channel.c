#include <asm/arch/fdl_channel.h>
#include <asm/arch/sci_types.h>
#include <asm/arch/sio_drv.h>
#include <asm/arch/usb_boot.h>

#define BOOT_FLAG_USB                   (0x5A)
#define BOOT_FLAG_UART1                 (0x6A)
#define BOOT_FLAG_UART0                 (0x7A)

extern uint32 FDL_GetBootMode (void);
//#define FDL_UART0_CHANNEL_USE

/******************************************************************************/
//  Description:    find a useable channel
//  Global resource dependence:
//  Author:         junqiang.wang
//  Note:
/******************************************************************************/
PUBLIC struct FDL_ChannelHandler *FDL_ChannelGet()
{
    uint32 bootMode = 0;
    char ch;

    struct FDL_ChannelHandler *channel;
    bootMode = FDL_GetBootMode();

    switch (bootMode)
    {
        case BOOT_FLAG_UART1:
            channel = &gUart1Channel;
            break;
        case BOOT_FLAG_UART0:
            channel = &gUart0Channel;
            break;
        case BOOT_FLAG_USB:
            channel = &gUSBChannel;
            break;
        default:
            channel = &gUSBChannel;
            break;
    }
    return channel;
}
