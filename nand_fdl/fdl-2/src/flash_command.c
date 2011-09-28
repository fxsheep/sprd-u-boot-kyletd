#include "sci_types.h"
#include "flash_command.h"
#include "fdl_nand.h"
#include "packet.h"
#include "fdl_conf.h"
#include "fdl_crc.h"
#include "fdl_stdio.h"


#if 1
#include "asm/arch/sci_types.h"
#include "asm/arch/nand_controller.h"
#include <linux/mtd/mtd.h>
#include <nand.h>
#include <linux/mtd/nand.h>
#include <jffs2/jffs2.h>

extern void cmd_yaffs_mount(char *mp);
extern void cmd_yaffs_umount(char *mp);
extern int cmd_yaffs_ls_chk(const char *dirfilename);
extern void cmd_yaffs_mread_file(char *fn, unsigned char *addr);
extern void cmd_yaffs_mwrite_file(char *fn, char *addr, int size);

#define FIXNV_SIZE			(64 * 1024)
#define PHASECHECK_SIZE		(3 * 1024)
#endif

typedef struct _DL_FILE_STATUS
{
    unsigned long   total_size;
    unsigned long   recv_size;
} DL_FILE_STATUS, *PDL_FILE_STATUS;
static unsigned long g_checksum;
static unsigned long g_sram_addr;
__align(4) unsigned char g_fixnv_buf[FIXNV_SIZE + 4];

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

static unsigned int is_nbl_write;
static unsigned int is_phasecheck_write;
#define ADDR_MASK				0x80000000
static unsigned int g_NBLFixBufDataSize = 0;
static unsigned char g_FixNBLBuf[0x8000];
static unsigned int g_PhasecheckBUFDataSize = 0;
static unsigned char g_PhasecheckBUF[0x2000];
#define PHASECHECK_ADDRESS_F4R2		(0x8D960000 & (~ADDR_MASK))
#define PHASECHECK_ADDRESS_F2R1		(0x88960000 & (~ADDR_MASK))
#define ECC_NBL_SIZE 0x4000
//bootloader header flag offset from the beginning
#define BOOTLOADER_HEADER_OFFSET   32
#define NAND_PAGE_LEN              512
#define NAND_MAGIC_DATA            0xaa55a5a5
//define nand data bus len
#define NAND_BUS_SIZE_8              8
#define NAND_BUS_SIZE_16              16
#define NAND_BUS_SIZE_32              32

int FDL2_DataStart (PACKET_T *packet, void *arg)
{
    unsigned long *data = (unsigned long *) (packet->packet_body.content);
    unsigned long start_addr = *data;
    unsigned long size = * (data + 1);
    int           ret;
#if defined(CHIP_ENDIAN_LITTLE)
    start_addr = EndianConv_32 (start_addr);
    size = EndianConv_32 (size);
#endif
    if (packet->packet_body.size == 12)
    {
	memset(g_fixnv_buf, 0xff, FIXNV_SIZE + 4);
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
            break;

	is_nbl_write = 0;
	is_phasecheck_write = 0;
	if((start_addr & ADDR_MASK) == ADDR_MASK) {
		start_addr &= ~ADDR_MASK;
		if (start_addr == 0x0) {
			is_nbl_write = 1;
			g_NBLFixBufDataSize = 0;
		}
		
		if ((start_addr == PHASECHECK_ADDRESS_F4R2) || (start_addr == PHASECHECK_ADDRESS_F2R1)) {
			is_phasecheck_write = 1;
			g_PhasecheckBUFDataSize = 0;
			memset(g_PhasecheckBUF, 0xff, 0x2000);
		}
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

/******************************************************************************
 * make the checksum of one packet
 ******************************************************************************/
unsigned short CheckSum(const unsigned int *src, int len)
{
    unsigned int   sum = 0;
    unsigned short *src_short_ptr = PNULL;

    while (len > 3)
    {
        sum += *src++;
        len -= 4;
    }

    src_short_ptr = (unsigned short *) src;

    if (0 != (len&0x2))
    {
        sum += * (src_short_ptr);
        src_short_ptr++;
    }

    if (0 != (len&0x1))
    {
        sum += * ( (unsigned char *) (src_short_ptr));
    }

    sum  = (sum >> 16) + (sum & 0x0FFFF);
    sum += (sum >> 16);

    return (unsigned short) (~sum);
}

/******************************************************************************
 * change the header of first bootloader page
 ******************************************************************************/
int NandChangeBootloaderHeader(unsigned int *bl_start_addr)
{
    unsigned int       *start_addr = bl_start_addr;
    unsigned short     check_sum = 0;

    unsigned short 	g_PageAttr = 0x1;
    unsigned short      nCycleDev = 0x5;
    unsigned int        nAdvance = 1;


    //set pointer to nand parameter config start address
    start_addr += BOOTLOADER_HEADER_OFFSET / 4;

    //set nand page attribute
    * (start_addr + 1) = g_PageAttr;
    //set nand address cycle
    * (start_addr+2) = nCycleDev;

    //set nand data bus len
    //* (start_addr + 3) = NAND_BUS_SIZE_8;
    * (start_addr + 3) = NAND_BUS_SIZE_16;
    //* (start_addr + 3) = NAND_BUS_SIZE_32;

    if (0) // for 6800h
    {
        //set magic data
        * (start_addr+4) = NAND_MAGIC_DATA;
        //make checksum of first 504 bytes
        check_sum = CheckSum ((unsigned int *) (start_addr + 1), (NAND_PAGE_LEN - BOOTLOADER_HEADER_OFFSET - 4));
    }
    else
    {
        if (nAdvance)
            * (start_addr + 4) = 1;
        else
            * (start_addr + 4) = 0;

        //set magic data
        * (start_addr + 5) = NAND_MAGIC_DATA;

        //make checksum of first 504 bytes
        check_sum = CheckSum((unsigned int *)(start_addr + 1), (NAND_PAGE_LEN - BOOTLOADER_HEADER_OFFSET - 4));
    }

    //set checksum
    * (start_addr) = (unsigned int) check_sum;

    return 1;

}

int NandWriteAndCheck(unsigned int size, unsigned char *buf)
{
    unsigned int start, end;
    unsigned int number;
    unsigned int imageIndex;
    unsigned int ecc_result;
	int aaa;

    memcpy (g_FixNBLBuf + g_NBLFixBufDataSize, buf, size); /* copy the data to the temp buffer */
    g_NBLFixBufDataSize += size;

    if ((g_NBLFixBufDataSize) <= ECC_NBL_SIZE)
    {
        return NAND_SUCCESS;
    }
#ifndef CONFIG_NAND_SC8810	
    NandChangeBootloaderHeader((unsigned int *) g_FixNBLBuf);
#endif	
	/*printf("\n\n");	
	for (aaa = 32; aaa <= 55; aaa ++)
		printf(" %02x ", g_FixNBLBuf[aaa]); 
	printf("\n\n");*/
    return NAND_SUCCESS;
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
	//printf("size = %d  recv_size = %d   total_size = %d\n", size, g_status.recv_size, g_status.total_size);
    if ( (g_status.recv_size + size) > g_status.total_size)
    {
        g_prevstatus = NAND_INVALID_SIZE;
        FDL2_SendRep (g_prevstatus);
        return 0;
    }

    if (CHECKSUM_OTHER_DATA == g_checksum)
    {
        if (is_nbl_write == 1) {
		g_prevstatus = NandWriteAndCheck( (unsigned int) size, (unsigned char *) (packet->packet_body.content));
	} else if (is_phasecheck_write == 1) {
		//printf("g_PhasecheckBUFDataSize = %d\n", g_PhasecheckBUFDataSize);
        	memcpy((g_PhasecheckBUF + g_PhasecheckBUFDataSize), (char *)(packet->packet_body.content), size);
        	g_PhasecheckBUFDataSize += size;
		g_prevstatus = NAND_SUCCESS;
	} else {
        	g_prevstatus = nand_write_fdl( (unsigned int) size, (unsigned char *) (packet->packet_body.content));
	}

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
        memcpy ( (unsigned char *) g_sram_addr, (char *) (packet->packet_body.content), size);
        g_sram_addr += size;
	g_status.recv_size += size;
        FDL_SendAckPacket (BSL_REP_ACK);
        return 1;
    }
}

int FDL2_DataEnd (PACKET_T *packet, void *arg)
{
	unsigned long pos, size, ret;
    	unsigned long i, fix_nv_size, fix_nv_checksum;
		
    	if (CHECKSUM_OTHER_DATA != g_checksum) {
		/* It's fixnv data */
        	fix_nv_size = g_sram_addr - (unsigned long) g_fixnv_buf;
        	fix_nv_checksum = Get_CheckSum ( (unsigned char *) g_fixnv_buf, fix_nv_size);
        	fix_nv_checksum = EndianConv_32 (fix_nv_checksum);
        	if (fix_nv_checksum != g_checksum)
            		SEND_ERROR_RSP(BSL_CHECKSUM_DIFF);
		
		//////////////////////////////
		pos = 0;
		while (pos < fix_nv_size) {
			if ((fix_nv_size - pos) >= 2048)
				size = 2048;
			else
				size = fix_nv_size - pos;
			//printf("pos = %d  size = %d\n", pos, size);
			if (size == 0)
				break;
			if (nand_write_fdl (size, g_fixnv_buf + pos) == NAND_SUCCESS)
                		g_prevstatus = NAND_SUCCESS;
			pos += size;
		}
		
		/* write fixnv to yaffs2 format */
		char *backupfixnvpoint = "/backupfixnv";
		char *backupfixnvfilename = "/backupfixnv/fixnv.bin";

		/* g_fixnv_buf : (FIXNV_SIZE + 4) instead of fix_nv_size */
		g_fixnv_buf[FIXNV_SIZE + 0] = g_fixnv_buf[FIXNV_SIZE + 1] = 0x5a;
		g_fixnv_buf[FIXNV_SIZE + 2] = g_fixnv_buf[FIXNV_SIZE + 3] = 0x5a;
		cmd_yaffs_mount(backupfixnvpoint);
    		cmd_yaffs_mwrite_file(backupfixnvfilename, (char *)g_fixnv_buf, (FIXNV_SIZE + 4));
		ret = cmd_yaffs_ls_chk(backupfixnvfilename);
		cmd_yaffs_umount(backupfixnvpoint);
		//////////////////////////////
    	} else if (is_nbl_write == 1) {
	   	/* write the spl loader image to the nand*/
		for (i = 0; i < 3; i++) {
			pos = 0;
			while (pos < g_NBLFixBufDataSize) {
				if ((g_NBLFixBufDataSize - pos) >= 2048)
					size = 2048;
				else
					size = g_NBLFixBufDataSize - pos;
				//printf("pos = %d  size = %d\n", pos, size);
				if (size == 0)
					break;
				if (nand_write_fdl (size, g_FixNBLBuf + pos) == NAND_SUCCESS)
                			g_prevstatus = NAND_SUCCESS;
				pos += size;
			}

        	}//for (i = 0; i < 3; i++)
		is_nbl_write = 0;
   	} else if (is_phasecheck_write == 1) {
		/* write phasecheck to yaffs2 format */
		char *productinfopoint = "/productinfo";
		char *productinfofilename = "/productinfo/productinfo.bin";

		/* g_PhasecheckBUF : (PHASECHECK_SIZE + 4) instead of g_PhasecheckBUFDataSize */
		g_PhasecheckBUF[PHASECHECK_SIZE + 0] = g_PhasecheckBUF[PHASECHECK_SIZE + 1] = 0x5a;
		g_PhasecheckBUF[PHASECHECK_SIZE + 2] = g_PhasecheckBUF[PHASECHECK_SIZE + 3] = 0x5a;
		cmd_yaffs_mount(productinfopoint);
    		cmd_yaffs_mwrite_file(productinfofilename, g_PhasecheckBUF, (PHASECHECK_SIZE + 4));
		ret = cmd_yaffs_ls_chk(productinfofilename);
		cmd_yaffs_umount(productinfopoint);
    	}

    	if (NAND_SUCCESS != g_prevstatus) {
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
	
    	ret = addr & 0x3;
    	switch (ret) {
		case 0:
			ret = nand_erase_partition(addr, size);
		break;
		case 1:
			ret = nand_erase_check_partition(addr & ~3, size);
		break;
		case 2:
			ret = nand_erase_check_write_partition(addr & ~3, size);
		break;
		default:
			ret = NAND_SUCCESS;
	}
    FDL2_SendRep (ret);
    return (NAND_SUCCESS == ret);
}

int FDL2_FormatFlash (PACKET_T *pakcet, void *arg)
{
    int ret = nand_format();
    FDL2_SendRep (ret);
    return (NAND_SUCCESS == ret);
}
