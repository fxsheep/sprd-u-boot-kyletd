#include "config.h"
#include "fdl_channel.h"
#include "packet.h"

extern void JumpToTarget(unsigned long addr);

#if defined CONFIG_SC8825
#define MEMORY_START		0x80000000
#elif defined CONFIG_SC7710G2
#define MEMORY_START		0x00000000
#else
#define MEMORY_START		0x30000000
#endif
#define MEMORY_SIZE		0x01000000 /* 16M */

typedef struct _DL_FILE_STATUS
{
    unsigned long start_address;
    unsigned long total_size;
    unsigned long recv_size;
    unsigned long next_address;
} DL_FILE_STATUS, *PDL_FILE_STATUS;

static DL_FILE_STATUS g_file;


int sys_connect(PACKET_T *packet, void *arg)
{
    FDL_SendAckPacket(BSL_REP_ACK);
    return 1;
}

int data_start(PACKET_T *packet, void *arg)
{
    unsigned long *data = (unsigned long*)(packet->packet_body.content);
    unsigned long start_addr = *data;
    unsigned long file_size  = *(data + 1);

#if defined (CHIP_ENDIAN_LITTLE)
    start_addr = EndianConv_32(start_addr);
    file_size  = EndianConv_32(file_size);
#endif

    if ((start_addr < MEMORY_START) || (start_addr >= MEMORY_START + MEMORY_SIZE))
    {
        while(1);
        FDL_SendAckPacket(BSL_REP_DOWN_DEST_ERROR);
        return 0;
    }

    if ((start_addr + file_size) > (MEMORY_START + MEMORY_SIZE))
    {
        while(1);
        FDL_SendAckPacket(BSL_REP_DOWN_SIZE_ERROR);
        return 0;
    }

    g_file.start_address = start_addr;
    g_file.total_size = file_size;
    g_file.recv_size = 0;
    g_file.next_address = start_addr;

    FDL_memset((void*)start_addr, 0, file_size);
    if (!packet->ack_flag)
    {
        packet->ack_flag = 1;
        FDL_SendAckPacket(BSL_REP_ACK);
    }
    return 1;
}

int data_midst(PACKET_T *packet, void *arg)
{
    unsigned short data_len = packet->packet_body.size;

    if ((g_file.recv_size + data_len) > g_file.total_size) {
        FDL_SendAckPacket(BSL_REP_DOWN_SIZE_ERROR);
        return 0;
    }

    FDL_memcpy((void *)g_file.next_address, (const void*)(packet->packet_body.content), data_len);
    g_file.next_address += data_len;
    g_file.recv_size += data_len;
    if (!packet->ack_flag)
    {
        packet->ack_flag = 1;
        FDL_SendAckPacket(BSL_REP_ACK);
    }
    return 1;
}

int data_end(PACKET_T *packet, void *arg)
{
    if (!packet->ack_flag)
    {
        packet->ack_flag = 1;
        FDL_SendAckPacket(BSL_REP_ACK);
    }
    return 1;
}

int data_exec(PACKET_T *packet, void *arg)
{
//    JumpToTarget(g_file.start_address);
    typedef void(*entry)(void);
    entry entry_func = g_file.start_address;
    entry_func();
    return 0;
}

int set_baudrate(PACKET_T *packet, void *arg)
{
    unsigned long baudrate = *(unsigned long*)(packet->packet_body.content);
#if defined (CHIP_ENDIAN_LITTLE)
    baudrate = EndianConv_32(baudrate);
#endif   
    if (!packet->ack_flag)
    { 
        packet->ack_flag = 1;
        FDL_SendAckPacket(BSL_REP_ACK);
    }

    gFdlUsedChannel->SetBaudrate(gFdlUsedChannel, baudrate);

    return 0;
}

