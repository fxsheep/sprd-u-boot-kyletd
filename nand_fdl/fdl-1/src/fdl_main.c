/******************************************************************************
 ** File Name:      FDL_main.c                                                 *
 ** Author:         weihua.wang                                                  *
 ** DATE:           27/06/2005                                               *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <asm/arch/sci_types.h>
#include "fdl_main.h"
#include <asm/arch/cmd_def.h>
#include <asm/arch/packet.h>
#include <asm/arch/dl_engine.h>
#include <asm/arch/sio_drv.h>
#include "fdl_command.h"
#include <asm/arch/usb_boot.h>
#include <asm/arch/sc_reg.h>

static const char VERSION_STR[] = {"Spreadtrum Boot Block version 1.1"};

static void error(void)
{
    printf("The first FDL failed!\r\n");
    for (;;) /*Do nothing*/;
}


int main(void)
{
    PACKET_T *packet;
    usb_boot(1);
    FDL_PacketInit();

    FDL_DlInit();
    FDL_DlReg(BSL_CMD_CONNECT,    sys_connect,  0);
    FDL_DlReg(BSL_CMD_START_DATA, data_start,   0);
    FDL_DlReg(BSL_CMD_MIDST_DATA, data_midst,   0);
    FDL_DlReg(BSL_CMD_EXEC_DATA,  data_exec,    0);
    FDL_DlReg(BSL_CMD_END_DATA,   data_end,     0);
    FDL_DlReg(BSL_SET_BAUDRATE,   set_baudrate, 0);

    for (;;)
    {
        char ch = gFdlUsedChannel->GetChar(gFdlUsedChannel);
        if (0x7e == ch)
            break;
    }

    packet = FDL_MallocPacket();
    packet->packet_body.type = BSL_REP_VER;
    packet->packet_body.size = sizeof(VERSION_STR);
    FDL_memcpy(packet->packet_body.content, VERSION_STR, sizeof(VERSION_STR));
    FDL_SendPacket(packet);
    FDL_FreePacket(packet);

    FDL_DlEntry(DL_STAGE_NONE);

    error();
    return 1;
}

