#include "sci_types.h"
#include "flash_command.h"
#include "fdl_nand.h"
#include "packet.h"
#include "fdl_conf.h"
#include "fdl_crc.h"
#include "fdl_stdio.h"

typedef struct _DL_FILE_STATUS
{
    unsigned long   total_size;
    unsigned long   recv_size;
} DL_FILE_STATUS, *PDL_FILE_STATUS;
unsigned long g_checksum;
static unsigned long g_sram_addr;
__align(4) unsigned char g_fixnv_buf[0x10000];

#define CHECKSUM_OTHER_DATA       0x5555aaaa
static DL_FILE_STATUS g_status;
static int g_prevstatus;
static __inline void FDL2_SendRep (unsigned long err)
{
    FDL_SendAckPacket (convert_err (err));
}
unsigned long FDL2_GetRecvDataSize (void)
{
    return g_status.recv_size;
}
int FDL2_DataStart (PACKET_T *packet, void *arg)
{
    unsigned long *data = (unsigned long *) (packet->packet_body.content);
    unsigned long start_addr = *data;
    unsigned long size = * (data + 1);
    int           ret;
#if defined(CHIP_ENDIAN_LITTLE)
    start_addr = EndianConv_32 (start_addr);
    size = EndianConv_32 (size);
#else
#endif

    if (packet->packet_body.size == 12)
    {
        g_checksum = * (data+2);
        g_sram_addr = (unsigned long) g_fixnv_buf;
    }
    else
    {
        g_checksum = CHECKSUM_OTHER_DATA;
    }

    if (0 == (g_checksum & 0xffffff))
    {
        //The fixnv checksum is error.
        SEND_ERROR_RSP (BSL_EEROR_CHECKSUM); /*lint !e527*/
    }

    do
    {
        /* Check the validity of the address and size of the file to be downloaded,
         * and erase this space of NAND flash. */
        ret = nand_start_write (start_addr, size);

        if (NAND_SUCCESS != ret)
        {
            break;
        }

        g_status.total_size  = size;
        g_status.recv_size   = 0;
        g_prevstatus = NAND_SUCCESS;

        FDL_SendAckPacket (BSL_REP_ACK);

        return 1;
    }
    while (0);

    FDL2_SendRep (ret);
    return 0;
}

int FDL2_DataMidst (PACKET_T *packet, void *arg)
{
    unsigned short  size;
    int i;

    /* The previous download step failed. */
    if (NAND_SUCCESS != g_prevstatus)
    {
        FDL2_SendRep (g_prevstatus);
        return 0;
    }

    size = packet->packet_body.size;

    if ( (g_status.recv_size + size) > g_status.total_size)
    {
        g_prevstatus = NAND_INVALID_SIZE;
        FDL2_SendRep (g_prevstatus);
        return 0;
    }

    if (CHECKSUM_OTHER_DATA == g_checksum)
    {
        g_prevstatus = nand_write_fdl( (unsigned int) size, (unsigned char *) (packet->packet_body.content));

        if (NAND_SUCCESS == g_prevstatus)
        {
            g_status.recv_size += size;

            if (!packet->ack_flag)
            {
                packet->ack_flag = 1;
                FDL_SendAckPacket (BSL_REP_ACK);
                return NAND_SUCCESS == g_prevstatus;
            }
        }

        FDL2_SendRep (g_prevstatus);
        return NAND_SUCCESS == g_prevstatus;
    }
    else //It's fixnv data. We should backup it.
    {
        memcpy ( (unsigned char *) g_sram_addr, (char *) (packet->packet_body.content), size); /*lint !e718*/
        g_sram_addr+=size;
        FDL_SendAckPacket (BSL_REP_ACK);
        return 1;

    }

}

int FDL2_DataEnd (PACKET_T *packet, void *arg)
{
    unsigned long i, fix_nv_size, fix_nv_checksum;

    if (CHECKSUM_OTHER_DATA != g_checksum)
    {
        fix_nv_size = g_sram_addr - (unsigned long) g_fixnv_buf;

        fix_nv_checksum = Get_CheckSum ( (unsigned char *) g_fixnv_buf, fix_nv_size);

        fix_nv_checksum = EndianConv_32 (fix_nv_checksum);

        if (fix_nv_checksum != g_checksum)
        {

            SEND_ERROR_RSP (BSL_CHECKSUM_DIFF); /*lint !e527*/
        }

        for (i=0; i<3; i++)
        {
            if (nand_write_fdl (fix_nv_size, g_fixnv_buf) == NAND_SUCCESS)
            {
                //Double check the flash nv area is correct. If it is Nand, we needn't do this.
                g_prevstatus = NAND_SUCCESS;
                break;
            }
        }

        if (i==3)
        {
            //Write error happened
            SEND_ERROR_RSP (BSL_WRITE_ERROR); /*lint !e527*/
        }
    }

    if (NAND_SUCCESS != g_prevstatus)
    {
        FDL2_SendRep (g_prevstatus);
        return 0;
    }

    g_prevstatus = nand_end_write();
    FDL2_SendRep (g_prevstatus);
    return (NAND_SUCCESS == g_prevstatus);
}

int FDL2_ReadFlash (PACKET_T *packet, void *arg)
{
    unsigned long *data = (unsigned long *) (packet->packet_body.content);
    unsigned long addr = *data;
    unsigned long size = * (data + 1);
    unsigned long off = 0;
    int           ret;

#if defined(CHIP_ENDIAN_LITTLE)
    addr = EndianConv_32 (addr);
    size = EndianConv_32 (size);
#endif

    if (size > MAX_PKT_SIZE)
    {
        FDL_SendAckPacket (BSL_REP_DOWN_SIZE_ERROR);
        return 0;
    }

    if (packet->packet_body.size > 8)
    {
        off = EndianConv_32 (* (data + 2));
    }

    ret = nand_read_fdl (addr, off, size, (unsigned char *) (packet->packet_body.content));

    if (NAND_SUCCESS == ret)
    {

        packet->packet_body.type = BSL_REP_READ_FLASH;
        packet->packet_body.size = size;
        FDL_SendPacket (packet);
        return 1;
    }
    else
    {
        FDL2_SendRep (ret);
        return 0;
    }
}

int FDL2_EraseFlash (PACKET_T *packet, void *arg)
{
    unsigned long *data = (unsigned long *) (packet->packet_body.content);
    unsigned long addr = *data;
    unsigned long size = * (data + 1);
    int           ret;

    addr = EndianConv_32 (addr);
    size = EndianConv_32 (size);

    ret = nand_erase_fdl (addr, size);
    FDL2_SendRep (ret);
    return (NAND_SUCCESS == ret);
}

int FDL2_FormatFlash (PACKET_T *pakcet, void *arg)
{
    int ret = nand_format();
    FDL2_SendRep (ret);
    return (NAND_SUCCESS == ret);
}
