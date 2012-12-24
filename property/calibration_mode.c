#include <config.h>
#include <common.h>
#ifdef CONFIG_CALIBRATION_MODE_NEW

#include <linux/types.h>
#include <asm/arch/bits.h>
#include <linux/string.h>
#include <android_bootimg.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <nand.h>
#include <android_boot.h>
#include <environment.h>
#include <jffs2/jffs2.h>
#include <boot_mode.h>
#include <malloc.h>

#include <asm/io.h>
#include <asm/arch/sio_drv.h>
#include "asm/arch/sci_types.h"

#ifdef CONFIG_EMMC_BOOT
#include <part.h>
#include "../disk/part_uefi.h"
#include "../drivers/mmc/card_sdio.h"
#define EMMC_SECTOR_SIZE 512
#endif

#define CALIBRATION_CHANNEL 1 // 0 : UART0 1: UART1
#define MODE_REQUEST_LENGTH 10
#define RUNMODE_REQUESET_CMD 0xFE
#define CALIBRATION_REQUEST_SUBCMD 0x1
#define MAX_USB_BUF_LEN 10*1024
#define MAX_UART_BUF_LEN 1024
#define MAX_SPI_BUF_LEN (65*1024)
#define MIN_SPI_BUF_LEN 64

#define SPI_HEADER_TAG 0x7e7f
#define SPI_HEADER_TYPE 0xaa55
#define NV_FLAG 0x5a5a5a5a

#define BIT27 (1<<27)
#define BIT28 (1<<28)
#define BIT29 (1<<29)

#define mdelay(n)	udelay((n) * 1000)

#define CALI_NV_WRITEBACK_SDIO_MERGE

typedef void* SDIO_HANDLE;

typedef struct _Mode_Data
{
	uint32 sn;
	uint16 length;
	uint8 type;
	uint8 subtype;
}Mode_Data_Type;

typedef struct {
	uint32 flag; 
	uint32 attribute;
	uint32 offset;
	uint32 size;
	uint32 reserved;	
} request_header_t;

typedef struct _nv_sync_type{
	request_header_t header;
	unsigned char data[FIXNV_SIZE];
}nv_sync_type;

typedef struct _nv_sync_reponse_type{
	request_header_t header;
	uint32 info;
}nv_sync_response_type;

typedef struct _spi_packet_header{
	uint16 tag;
	uint16 type;
	uint32 length;
	uint32 frame_num;
	uint32 reserved;
}spi_packet_header;

typedef struct _spi_packet_type{
	spi_packet_header header;
	unsigned char data[MAX_SPI_BUF_LEN];
}spi_packet_type;


struct FDL_ChannelHandler *gUsedChannel;
static unsigned char g_usb_buf[MAX_USB_BUF_LEN];
static unsigned char g_uart_buf[MAX_UART_BUF_LEN];
//static unsigned char g_spi_buf[MAX_SPI_BUF_LEN];
static unsigned char g_fixnv_buf[FIXNV_SIZE +1024];
//static request_header_t g_req_header;
static nv_sync_type g_nv_sync_data;
static spi_packet_type g_spi_packet;
//static int bcheknv;
#ifdef SPI_CHECKSUM
static uint32 frame_num;
#endif
extern int __dl_log_share__ ;
extern int usb_trans_status;
extern SDIO_HANDLE  sdio_handle;

extern int dwc_otg_driver_init(void);
extern int usb_serial_init(void);
extern int usb_is_configured(void);
extern int get_cal_enum_ms(void);
extern int usb_is_port_open(void);
extern int get_cal_io_ms(void);
extern int spi_channel_read (unsigned long dev_id,unsigned char *buffer_ptr,int len);
extern int spi_channel_write (unsigned long dev_id,unsigned char *buffer_ptr,int len);
extern void gpio_direction_output(int PinNo, int OutIn);
extern void gpio_set_value(int PinNo, int Level);
extern int gpio_get_value(int PinNo);
extern int gs_open(void);
extern int spi_channel_init(unsigned long phy_id);
extern void spi_channel_open (unsigned long phy_id, unsigned long mode, unsigned long tx_bit_length);
extern int usb_is_trans_done(int direct);
extern int gs_read(const unsigned char *buf, int *count);
extern int gs_write(const unsigned char *buf, int count);
extern void usb_wait_trans_done(int direct);
extern void gs_reset_usb_param(void);
extern void calibration_reset_composite(void);
#ifndef CONFIG_EMMC_BOOT
extern void cmd_yaffs_mount(char *mp);
extern int cmd_yaffs_ls_chk(const char *dirfilename);
extern void cmd_yaffs_mread_file(char *fn, unsigned char *addr);
extern void cmd_yaffs_mwrite_file(char *fn, char *addr, int size);
extern int cmd_yaffs_ls_chk(const char *dirfilename);
extern void cmd_yaffs_umount(char *mp);
#endif
extern SDIO_HANDLE sdio_open(void);
extern int sdio_read(SDIO_HANDLE handle,unsigned char *buffer,int size);
extern int sdio_write(SDIO_HANDLE handle,unsigned char *buffer,int size);

unsigned short EndianConv_16 (unsigned short value)
{
#if 0 //def _LITTLE_ENDIAN
    return (value >> 8 | value << 8);
#else
    return value;
#endif
}

unsigned int EndianConv_32 (unsigned int value)
{
#if 0 //def _LITTLE_ENDIAN
    unsigned int nTmp = 0;
	nTmp = (value >> 24 | value << 24);

    nTmp |= ( (value >> 8) & 0x0000FF00);
    nTmp |= ( (value << 8) & 0x00FF0000);
    return nTmp;
#else
    return value;
#endif
}


static struct FDL_ChannelHandler *Calibration_ChannelGet(int nchannel)
{
    struct FDL_ChannelHandler *channel;

    switch (nchannel)
    {
        case 1:
            channel = &gUart1Channel;
            break;
        case 0:
            channel = &gUart0Channel;
            break;
        default:
            channel = &gUart0Channel;
            break;
    }

    return channel;
}
static int Calibration_ParseRequsetMode(const unsigned char *buf, int len)
{
	Mode_Data_Type *pReqType = NULL;
	char tempbuf[MODE_REQUEST_LENGTH - 2] = {0};
	if(buf[0] != 0x7e || buf[len - 1] != 0x7e)
		return -1;
	memcpy((void*)tempbuf, (void*)&buf[1], len-2);
	pReqType = (Mode_Data_Type*)tempbuf;
	if(pReqType->type != RUNMODE_REQUESET_CMD || pReqType->subtype != CALIBRATION_REQUEST_SUBCMD){
		return -1;
	}
	return 0;
}

static int Calibration_SetMode(const uint8_t *pcmd, int length)
{
	if(pcmd[0] != 0x7e || pcmd[length - 1] != 0x7e)
		return -1;
	if(gUsedChannel->Write(gUsedChannel, pcmd, length) != length)
		return -1;
	return 0;
}

static int Calibration_ReinitUsb(void)
{
	unsigned long long start_time, end_time;
	gs_reset_usb_param();
	calibration_reset_composite();
	dwc_otg_driver_init();
	usb_serial_init();
	start_time = get_timer_masked();
	while(!usb_is_configured()){
		end_time = get_timer_masked();
		if(end_time - start_time > get_cal_enum_ms())
			return -1;
	}	
	printf("USB SERIAL CONFIGED\n");

	start_time = get_timer_masked();
	while(!usb_is_port_open()){
		end_time = get_timer_masked();
		if(end_time - start_time > get_cal_io_ms())
			return -1;
	}	
	return 0;
}

static int Calibration_SpiReadPacket(spi_packet_type *p_packet)
{
	uint32 length, count,r_count = 0;
	#ifdef SPI_CHECKSUM
	uint32 sum;
	int i;
	#endif
	if(NULL == p_packet)
		return 0;
	#if 1
	if(sdio_read(sdio_handle, (unsigned char*)&(p_packet->header), sizeof(spi_packet_header)) != sizeof(spi_packet_header))
		return 0;
	#else
	if(spi_channel_read(0, (unsigned char*)&(p_packet->header), 0x100) != sizeof(spi_packet_header))
		return 0;
	#endif
	if(SPI_HEADER_TAG !=p_packet->header.tag || SPI_HEADER_TAG != EndianConv_16(p_packet->header.tag))
		return 0;
	if(SPI_HEADER_TYPE != p_packet->header.type || SPI_HEADER_TYPE != EndianConv_16(p_packet->header.type))
		return 0;
	length = EndianConv_32(p_packet->header.length);
	while(length > 0){
		count = sdio_read(sdio_handle, &p_packet->data[r_count], length);
		length -= count;
		r_count += count;
	}
	#ifdef SPI_CHECKSUM
	for(i = 0; i < r_count; i++){
		sum += *((uint8*)(&p_packet->data[i]));
	}
	if(sum == p_packet->header.reserved)
		return r_count;
	else
		return 0;
	#endif
	return r_count;
}

#ifdef CALI_NV_WRITEBACK_SDIO_MERGE
enum {
    READ_OK,
    READ_DISCARD,
    READ_RESEND,
    READ_CONTINUE,
};

enum {
    STEP_READ_PACKET_HEADER,
    STEP_READ_NV_HEADER,
    STEP_READ_NV_HEADER_AGAIN,
    STEP_HANDLE_NV_DATA,
    STEP_HANDLE_NV_DATA_AGAIN,
    STEP_RESET,
};

static int dirty = 0;
static int step = STEP_READ_PACKET_HEADER;
static int offset = 0, size = 0;
static unsigned char *start;
static spi_packet_header *p_packet = NULL;
static request_header_t *p_header = NULL;
static spi_packet_header packet;
static request_header_t header;
static char retry = 0;
static unsigned char rbuf[MAX_SPI_BUF_LEN];

void Calibration_SyncResponse(int ret);

static void Calibration_read_empty(void) {
    unsigned char p[128];
    //while(sdio_read(sdio_handle, p, 128));
}

static int Calibration_read(void) {
    int count, read;
    int start_offset = (offset + size) % MAX_SPI_BUF_LEN;
    int left = MAX_SPI_BUF_LEN - size;
    unsigned char *p = (unsigned char *)rbuf;
    if(!left)
        return 0;
    if(left <= (MAX_SPI_BUF_LEN - start_offset))
        read = left;
    else
        read = MAX_SPI_BUF_LEN - start_offset;
    count = sdio_read(sdio_handle, p + start_offset, read);
    size += count;
    if(count < read)
        return count;
    if(left > count) {
        start_offset = (start_offset + count) % MAX_SPI_BUF_LEN;
        count += sdio_read(sdio_handle, p + start_offset, left - count);
    }
    return count;
}

static int Calibration_copy(unsigned char *dst, const unsigned char *src, int len) {
    int count;
    src = start;
    if(len <= (MAX_SPI_BUF_LEN - offset))
        count = len;
    else
        count = MAX_SPI_BUF_LEN - offset;
    memcpy((void*)dst, (void*)src, count);
    if(len > count) {
        src = (const unsigned char *)rbuf;
        dst += count;
        memcpy((void*)dst, (void*)src, len - count);
    }
    return len;
}

static int Calibration_pre_read_nv(void) {
    static int read_once = 0;
    if(!read_once) {
	#ifdef CONFIG_EMMC_BOOT
	block_dev_desc_t *p_block_dev = NULL;  
	p_block_dev = get_dev("mmc", 1);
	if(NULL == p_block_dev){
		return -1;
	}
	memset(g_fixnv_buf, 0xff, sizeof(g_fixnv_buf));
	if(-1 == Calibration_read_partition(p_block_dev, PARTITION_FIX_NV1, (char*)g_fixnv_buf, FIXNV_SIZE + 4))
		return -1;
	#else
	if (mtdparts_init() != 0){
		printf("mtdparts init error \n");
		return -1;
	}
	{
                char *fixnvpoint = "/fixnv";
                char *fixnvfilename = "/fixnv/fixnv.bin";
                char *backupfixnvpoint = "/backupfixnv";
                char *backupfixnvfilename = "/backupfixnv/fixnv.bin";

		memset(g_fixnv_buf, 0xff, sizeof(g_fixnv_buf));
		cmd_yaffs_mount(fixnvpoint);
		if (cmd_yaffs_ls_chk(fixnvfilename) == (FIXNV_SIZE + 4)) {
			cmd_yaffs_mread_file(fixnvfilename, g_fixnv_buf);
			if (-1 == Calibration_nv_correct_endflag(g_fixnv_buf, FIXNV_SIZE)) {
                                cmd_yaffs_umount(fixnvpoint);
				return -1;
			}
		}else {
                        cmd_yaffs_umount(fixnvpoint);
			return -1;
		}
                cmd_yaffs_umount(fixnvpoint);

        	cmd_yaffs_mount(backupfixnvpoint);
        	if (cmd_yaffs_ls_chk(backupfixnvfilename) == (FIXNV_SIZE + 4)) {
        	}else {
                            cmd_yaffs_umount(backupfixnvpoint);
        		return -1;
        	}
                cmd_yaffs_umount(backupfixnvpoint);
	}
	#endif
        read_once = 1;
    }
    return 0;
}

static int Calibration_post_write_nv(void) {    
#ifdef CONFIG_EMMC_BOOT
	block_dev_desc_t *p_block_dev = NULL;  
	p_block_dev = get_dev("mmc", 1);
	if(NULL == p_block_dev){
		return -1;
	}
	if(-1 == Calibration_write_partition(p_block_dev, PARTITION_FIX_NV2, (char*)g_fixnv_buf, FIXNV_SIZE + 4))
		return -1;
	if(-1 == Calibration_write_partition(p_block_dev, PARTITION_FIX_NV1, (char*)g_fixnv_buf, FIXNV_SIZE + 4))
		return -1;
#else
	char *fixnvpoint = "/fixnv";
	char *fixnvfilename = "/fixnv/fixnv.bin";
	char *backupfixnvpoint = "/backupfixnv";
	char *backupfixnvfilename = "/backupfixnv/fixnv.bin";
    
	cmd_yaffs_mount(fixnvpoint);
	cmd_yaffs_mwrite_file(fixnvfilename, (char*)g_fixnv_buf, (FIXNV_SIZE + 4));
	cmd_yaffs_ls_chk(fixnvfilename);
	cmd_yaffs_umount(fixnvpoint);

	cmd_yaffs_mount(backupfixnvpoint);
	cmd_yaffs_mwrite_file(backupfixnvfilename, (char*)g_fixnv_buf, (FIXNV_SIZE + 4));
	cmd_yaffs_ls_chk(backupfixnvfilename);
	cmd_yaffs_umount(backupfixnvpoint);
#endif
        dirty = 0;
        return 0;
}

static int Calibration_handle_data(void) {
    do {
            switch(step) {
                case STEP_HANDLE_NV_DATA_AGAIN:
                case STEP_READ_PACKET_HEADER:
                    if(size < sizeof(spi_packet_header))
                        return READ_CONTINUE;
                    p_packet = (spi_packet_header *)&packet;
                    Calibration_copy((void*)p_packet, (void *)start, sizeof(spi_packet_header));
                    p_packet->length = EndianConv_32(p_packet->length);
                    p_packet->tag = EndianConv_16(p_packet->tag);
                    p_packet->type = EndianConv_16(p_packet->type);
                    if(SPI_HEADER_TAG != p_packet->tag || SPI_HEADER_TAG != EndianConv_16(p_packet->tag))
                        return READ_RESEND;
                    if(SPI_HEADER_TYPE != p_packet->type || SPI_HEADER_TYPE != EndianConv_16(p_packet->type))
                        return READ_RESEND;
                    if(p_packet->length <= sizeof(request_header_t)) {
                        step = STEP_RESET;
                        break;
                    }
                    p_packet->length -= sizeof(request_header_t);
                    start = (unsigned char *)((unsigned int)start +  sizeof(spi_packet_header));
                    size -= sizeof(spi_packet_header);
                    offset = (offset +  sizeof(spi_packet_header)) % MAX_SPI_BUF_LEN;
                    if(step == STEP_READ_PACKET_HEADER)
                        step = STEP_READ_NV_HEADER;
                    else
                        step = STEP_HANDLE_NV_DATA;
                    break;
                case STEP_READ_NV_HEADER_AGAIN:
                    if(size < sizeof(request_header_t)) {
                        if(retry++ == 0) {
                            return READ_CONTINUE;
                        }
                        retry = 0;
                        step = STEP_RESET;
                        break;
                    }
                case STEP_READ_NV_HEADER:
                    if(size < sizeof(request_header_t))
                        return READ_CONTINUE;
                    p_header = (request_header_t *)&header;
                    Calibration_copy((void*)p_header, (void *)start, sizeof(request_header_t));
                    p_header->flag = EndianConv_32(p_header->flag);
                    p_header->attribute = EndianConv_32(p_header->attribute);
                    p_header->offset = EndianConv_32(p_header->offset);
                    p_header->size = EndianConv_32(p_header->size);
                    if(NV_FLAG != p_header->flag) {
                        step = STEP_RESET;
                        break;
                    }
                    if(p_header->attribute & BIT29) {
                        step = STEP_RESET; //calibration not support read
                        break;
                    }
                    if((p_header->attribute & BIT28) == 0) {
                        step = STEP_RESET; //calibration not support non-sync mode
                        break;
                    }
                    if(p_header->attribute & BIT27) {
                        step = STEP_RESET; //calibration must be data
                        break;
                    }
                    if(p_header->offset + p_header->size > FIXNV_SIZE) {
                        step = STEP_RESET;
                        break;
                    }
                    start = (unsigned char *)((unsigned int)start + sizeof(request_header_t));
                    size -= sizeof(request_header_t);
                    offset = (offset +  sizeof(request_header_t)) % MAX_SPI_BUF_LEN;
                    step = STEP_HANDLE_NV_DATA;
                    break;
                case STEP_HANDLE_NV_DATA:
                    if(size < p_packet->length)
                        return READ_CONTINUE;
                    if(p_packet->length < p_header->size) {
                        Calibration_copy((void*)&g_fixnv_buf[p_header->offset], (void *)start, p_packet->length);
                        dirty = 1;
                        p_header->offset += p_packet->length;
                        p_header->size -= p_packet->length;
                        start =  (unsigned char*)rbuf;
                        size = 0;
                        offset = 0;
                        step = STEP_HANDLE_NV_DATA_AGAIN;
                        return READ_OK;
                    } else if(p_packet->length == p_header->size) {
                        Calibration_copy((void*)&g_fixnv_buf[p_header->offset], (void *)start, p_header->size);
                        dirty = 1;
                        start =  (unsigned char*)rbuf;
                        size = 0;
                        offset = 0;
                        step = STEP_READ_PACKET_HEADER;
                        return READ_OK;
                    } else {
                        Calibration_copy((void*)&g_fixnv_buf[p_header->offset], (void *)start, p_header->size);
                        dirty = 1;
                        p_packet->length -= p_header->size;
                        start = (unsigned char *)((unsigned int)start + p_header->size);
                        size -= p_header->size;
                        offset = (offset +  p_header->size) % MAX_SPI_BUF_LEN;
                        step = STEP_READ_NV_HEADER_AGAIN;
                    }
                    break;
                case STEP_RESET:
                    start =  (unsigned char*)rbuf;
                    size = 0;
                    offset = 0;
                    retry = 0;
                    step = STEP_READ_PACKET_HEADER;
                    return READ_OK;
                default:
                    step = STEP_RESET;
                    break;
            }
    }  while(1);
}

static int Calibration_data_handler(void) {
    if(Calibration_pre_read_nv() < 0)
        return -1;
    do {
        int code = 0;
        int status = READ_CONTINUE;
        if(0 != gpio_get_value(CP_AP_RTS))
            break;
        gpio_set_value(AP_CP_RDY, 1); // r
        gpio_set_value(AP_CP_RTS, 1);
        while(0 == gpio_get_value(CP_AP_RDY));
        do {
            Calibration_read();
            if(size > 0)
                status = Calibration_handle_data();
        } while(status == READ_CONTINUE);
        Calibration_read_empty();
        if(status == READ_OK || status == READ_DISCARD) {
            gpio_set_value(AP_CP_RDY, 1);
            gpio_set_value(AP_CP_RTS, 0);
            while(1 == gpio_get_value(CP_AP_RDY));
        } else {
            gpio_set_value(AP_CP_RDY, 0);
            gpio_set_value(AP_CP_RTS, 0);
            while(1 == gpio_get_value(CP_AP_RDY));
        }
        if(dirty)
            if(Calibration_post_write_nv() < 0)
                code = -1;
        memcpy( (void*)&(g_nv_sync_data.header), (void*)&header, sizeof(request_header_t));
        Calibration_SyncResponse(code);
    } while(1);
    
    
    return 0;
}
#endif

#ifdef CONFIG_MACH_KYLEW
/*static*/ int Calibration_SpiReadData(void)
{
	int i;
	int ret;
	uint32 length;
    
	if(0 != gpio_get_value(CP_AP_RTS))
		return 0;
	do {
	        gpio_set_value(AP_CP_RDY, 1); // r
	        gpio_set_value(AP_CP_RTS, 1);
	        while(0 == gpio_get_value(CP_AP_RDY));
	        if(0 < (length = Calibration_SpiReadPacket(&g_spi_packet))) {
	            if(g_spi_packet.header.length >= sizeof(request_header_t)) {
	                break;
	            }
	        }
	        gpio_set_value(AP_CP_RDY, 0);
	        gpio_set_value(AP_CP_RTS, 0);
	        while(1 == gpio_get_value(CP_AP_RDY));
	}  while(1);
	memcpy((void*)&g_nv_sync_data, (void*)g_spi_packet.data, g_spi_packet.header.length);
	if(NV_FLAG != EndianConv_32(g_nv_sync_data.header.flag)){
		ret =  -1;
		goto fail;
	}
	if(g_nv_sync_data.header.size + sizeof(request_header_t) <= g_spi_packet.header.length){
		ret =  g_nv_sync_data.header.size;
		//memcpy(g_spi_buf, (void*)(uint32(g_spi_packet.data) + sizeof(request_header_t)), g_req_header.size);
	}else{
		int count = g_nv_sync_data.header.size;
		uint32 ncpy = 0;
		//memcpy(g_spi_buf, (void*)(uint32(g_spi_packet.data) + sizeof(request_header_t)), g_spi_packet.header.length -sizeof(request_header_t));
		count -= g_spi_packet.header.length -sizeof(request_header_t);
		ncpy += g_spi_packet.header.length -sizeof(request_header_t);
		while(count > 0){
			memset(&g_spi_packet, 0x0, sizeof(g_spi_packet));
			if(0 == Calibration_SpiReadPacket(&g_spi_packet)){
			ret =  -1;
			goto fail;
		}else{
				memcpy((void*)((uint32)g_nv_sync_data.data + ncpy), g_spi_packet.data, g_spi_packet.header.length);
				count -= g_spi_packet.header.length;
				ncpy += g_spi_packet.header.length;
			}
		}
	}
	ret = g_nv_sync_data.header.size;
        gpio_set_value(AP_CP_RDY, 1);
        gpio_set_value(AP_CP_RTS, 0);
        while(1 == gpio_get_value(CP_AP_RDY)) ;
	return ret;
fail:
        gpio_set_value(AP_CP_RDY, 0);
        gpio_set_value(AP_CP_RTS, 0);
        while(1 == gpio_get_value(CP_AP_RDY)) ;
	return ret;
}

#endif

static int Calibration_ProcessSyncData(nv_sync_type *p_syncdata)
{
	return 0;
}

#ifdef CONFIG_EMMC_BOOT
#define EMMC_SECTOR_SIZE 512

int Calibration_read_partition(block_dev_desc_t *p_block_dev, EFI_PARTITION_INDEX part, char *buf, int len)
{
	disk_partition_t info;
	unsigned long size = (len +(EMMC_SECTOR_SIZE - 1)) & (~(EMMC_SECTOR_SIZE - 1));
	int ret = 0; /* success */

	if (!get_partition_info(p_block_dev, part, &info)) {
		if (TRUE !=  Emmc_Read(PARTITION_USER, info.start, size / EMMC_SECTOR_SIZE, (uint8*)buf)) {
			printf("emmc image read error \n");
			ret = -1; /* fail */
		}
	}
	 
	return ret;
}


int Calibration_write_partition(block_dev_desc_t *p_block_dev, EFI_PARTITION_INDEX part, char *buf, int len)
{
	disk_partition_t info;
	unsigned long size = (len +(EMMC_SECTOR_SIZE - 1)) & (~(EMMC_SECTOR_SIZE - 1));
	int ret = 0; /* success */

	if (!get_partition_info(p_block_dev, part, &info)) {
		if (TRUE !=  Emmc_Write(PARTITION_USER, info.start, size / EMMC_SECTOR_SIZE, (uint8*)buf)) {
			printf("emmc image read error \n");
			ret = -1; /* fail */
		}
	}

	return ret;
}
#else
int Calibration_nv_correct_endflag(unsigned char *array, unsigned long size)
{
	if ((array[size] == 0x5a) && (array[size + 1] == 0x5a) && (array[size + 2] == 0x5a) && (array[size + 3] == 0x5a))
		return 1;
	else
		return -1;
}
#endif

static int Calibration_ProcessFixNv(nv_sync_type *p_syncdata)
{
	int i;
	request_header_t *p_header = NULL;
	#ifdef CONFIG_EMMC_BOOT
	block_dev_desc_t *p_block_dev = NULL;  
	#endif
	if(NULL == p_syncdata)
		return -1;
	p_header = &(p_syncdata->header);
	if(p_header->attribute & BIT29){
		return -1; //calibration not support read
	}
	if((p_header->attribute & BIT28) == 0){
		return -1; //calibration not support non-sync mode
	}
	if(p_header->attribute & BIT27){
		return -1; // calibration must be data
	}
	if(p_header->offset + p_header->size > FIXNV_SIZE)
		return 0;
	#ifdef CONFIG_EMMC_BOOT
	p_block_dev = get_dev("mmc", 1);
	if(NULL == p_block_dev){
		return -1;
	}
	memset(g_fixnv_buf, 0xff, sizeof(g_fixnv_buf));
	if(-1 == Calibration_read_partition(p_block_dev, PARTITION_FIX_NV2, (char*)g_fixnv_buf, FIXNV_SIZE + 4))
		return -1;
	for(i = 0;  i < p_header->size; i++){
		g_fixnv_buf[p_header->offset + i] = p_syncdata->data[i];
	}
	if(-1 == Calibration_write_partition(p_block_dev, PARTITION_FIX_NV2, (char*)g_fixnv_buf, FIXNV_SIZE + 4))
		return -1;
	memset(g_fixnv_buf, 0xff, sizeof(g_fixnv_buf));
	if(-1 == Calibration_read_partition(p_block_dev, PARTITION_FIX_NV1, (char*)g_fixnv_buf, FIXNV_SIZE + 4))
		return -1;
	for(i = 0;  i < p_header->size; i++){
		g_fixnv_buf[p_header->offset + i] = p_syncdata->data[i];
	}
	if(-1 == Calibration_write_partition(p_block_dev, PARTITION_FIX_NV1, (char*)g_fixnv_buf, FIXNV_SIZE + 4))
		return -1;
	#else
	if (mtdparts_init() != 0){
		printf("mtdparts init error \n");
		return -1;
	}
	{
		char *fixnvpoint = "/fixnv";
		char *fixnvfilename = "/fixnv/fixnv.bin";
		char *backupfixnvpoint = "/backupfixnv";
		char *backupfixnvfilename = "/backupfixnv/fixnv.bin";

		memset(g_fixnv_buf, 0xff, sizeof(g_fixnv_buf));
		cmd_yaffs_mount(fixnvpoint);
		if (cmd_yaffs_ls_chk(fixnvfilename) == (FIXNV_SIZE + 4)) {
			cmd_yaffs_mread_file(fixnvfilename, g_fixnv_buf);
			if (-1 == Calibration_nv_correct_endflag(g_fixnv_buf, FIXNV_SIZE)) {
                                cmd_yaffs_umount(fixnvpoint);
				return -1;
			}
		}else {
                        cmd_yaffs_umount(fixnvpoint);
			return -1;
		}
                cmd_yaffs_umount(fixnvpoint);

		for(i = 0;  i < p_header->size; i++){
			g_fixnv_buf[p_header->offset + i] = p_syncdata->data[i];
		}

		cmd_yaffs_mount(fixnvpoint);
		cmd_yaffs_mwrite_file(fixnvfilename, (char*)g_fixnv_buf, (FIXNV_SIZE + 4));
		cmd_yaffs_ls_chk(fixnvfilename);
		cmd_yaffs_umount(fixnvpoint);

		cmd_yaffs_mount(backupfixnvpoint);
		if (cmd_yaffs_ls_chk(backupfixnvfilename) == (FIXNV_SIZE + 4)) {
		}else {
                        cmd_yaffs_umount(backupfixnvpoint);
			return -1;
		}

		cmd_yaffs_mwrite_file(backupfixnvfilename, (char*)g_fixnv_buf, (FIXNV_SIZE + 4));
		cmd_yaffs_ls_chk(backupfixnvfilename);
		cmd_yaffs_umount(backupfixnvpoint);
	}
	#endif
	return 0;
}

/*static*/ int Calibration_ProcessData(nv_sync_type *p_syncdata)
{
	uint32 attrb;
	request_header_t *p_header = NULL;
	if(NULL == p_syncdata)
		return -1;
	p_header = &(p_syncdata->header);
	p_header->flag = EndianConv_32(p_header->flag);
	if(NV_FLAG != p_header->flag)
		return -1;
	attrb = EndianConv_32(p_header->attribute);
	p_header->attribute = attrb;
	p_header->offset = EndianConv_32(p_header->offset);
	p_header->size = EndianConv_32(p_header->size);
	
	if(attrb >> 30 == 2)
		return Calibration_ProcessSyncData(p_syncdata);
	if(attrb >>30 == 0)
		return Calibration_ProcessFixNv(p_syncdata);
}

#ifdef CONFIG_MACH_KYLEW
int Calibration_SpiWritePacket(unsigned char *buf, uint32 size)
{
	int ret;
	#ifdef SPI_CHECKSUM
	int i;
	uint32 sum;
	#endif
	if(size > MAX_SPI_BUF_LEN)
		return -1;
	memset(&g_spi_packet, 0x0, sizeof(g_spi_packet));
	g_spi_packet.header.tag = SPI_HEADER_TAG;
	g_spi_packet.header.type = SPI_HEADER_TYPE;
	g_spi_packet.header.length = size;
	g_spi_packet.header.frame_num = 0;
	#ifdef SPI_CHECKSUM
	for(i = 0; i < size; i++){
		sum += *((uint8*)&buf[i]);
	}
	g_spi_packet.header.reserved = sum;
	g_spi_packet.header.frame_num = frame_num++;
	#endif
	memcpy((void*)g_spi_packet.data, (void*)buf, size);
	do {
		gpio_set_value(AP_CP_RDY, 0); // w
		gpio_set_value(AP_CP_RTS, 1);
		while(0 == gpio_get_value(CP_AP_RDY)) ;
                ret = sdio_write(sdio_handle, (unsigned char*)&g_spi_packet, sizeof(spi_packet_header) + size);
                if(ret == (sizeof(spi_packet_header) + size)) {
                    gpio_set_value(AP_CP_RDY, 1);
                    gpio_set_value(AP_CP_RTS, 0);
                    while(1 == gpio_get_value(CP_AP_RDY)) ;
                    break;
                } else {
                    gpio_set_value(AP_CP_RDY, 0);
                    gpio_set_value(AP_CP_RTS, 0);
                    while(1 == gpio_get_value(CP_AP_RDY));
                }
        }  while(1);
	return ret;
}

void Calibration_SyncResponse(int ret)
{
	uint32 ninfo = 0;
	nv_sync_response_type t_resp;
	switch(ret){
		case 0:
			ninfo = 0x80;
			break;
		case -1:
			ninfo = 0x81;
			break;
		default:
			ninfo = 0x80;
	}
	memcpy((void*)&t_resp, (void*)&(g_nv_sync_data.header), sizeof(request_header_t));
	t_resp.header.attribute |= BIT27;
	t_resp.info = ninfo;
	Calibration_SpiWritePacket((unsigned char*)&t_resp, sizeof(t_resp));
		
}
#endif

extern void init_calibration_mode(void);
void calibration_mode(const uint8_t *pcmd, int length)
{
	int ret;
	int count = 0;
	unsigned char ch;
	int index = 0;
	int i;
	unsigned char buf[MODE_REQUEST_LENGTH] = {0};

#ifdef CALI_NV_WRITEBACK_SDIO_MERGE
        start =  rbuf;
#endif	
	init_calibration_mode();
#ifndef __DL_UART0__
	__dl_log_share__ = 1;
#endif	

	gUsedChannel = Calibration_ChannelGet(CALIBRATION_CHANNEL);
	gUsedChannel->Open(gUsedChannel, 115200);
#if 1    
        while(gUsedChannel->GetChar(gUsedChannel) != 0x7e); // purge fifo
        buf[0] = 0x7e;
	for(i = 1; i < MODE_REQUEST_LENGTH; i++){
		ch = gUsedChannel->GetChar(gUsedChannel);
		buf[i] = ch;
	}
    
	if(-1 == Calibration_ParseRequsetMode(buf, MODE_REQUEST_LENGTH))
		return ;
	if(-1 == Calibration_SetMode(pcmd, length))
		return ;

	// wait for cp ready
	while(gpio_get_value(CP_AP_LIV) == 0);
	sdio_handle = sdio_open();
#endif
	printf("Calibration_ReinitUsb......\n");
	if(-1 == Calibration_ReinitUsb())
		return ;
	gs_open();

	while(TRUE){
		if(usb_is_trans_done(0)){
			if(usb_trans_status)
				printf("func: %s line %d usb trans with error %d\n", __func__, __LINE__, usb_trans_status);
			count = MAX_USB_BUF_LEN;
			gs_read(g_usb_buf, &count);
			if(usb_trans_status)
				printf("func: %s line %d usb trans with error %d\n", __func__, __LINE__, usb_trans_status);
		}
		if(count > 0){
			if(count != gUsedChannel->Write(gUsedChannel, g_usb_buf, count)) {
				return ;
			}
                        mdelay(3);
		}
		count = 0;		
		while(-1 != (ret = gUsedChannel->GetSingleChar(gUsedChannel))){
			g_uart_buf[index++] = (ret & 0xff);
		}
		while(index > 0){
			ret = gs_write(g_uart_buf, index);
			printf("func: %s waitting %d write done\n", __func__, index);
			if(usb_trans_status)
				printf("func: %s line %d usb trans with error %d\n", __func__, __LINE__, usb_trans_status);
			usb_wait_trans_done(1);
			if(ret > 0)
				index -= ret;			
		}
#ifdef CALI_NV_WRITEBACK_SDIO_MERGE
                if(Calibration_data_handler() < 0)
                    break;
#else
		index = 0;
		ret = Calibration_SpiReadData();
		if(ret == 0){
			continue;
		}else if(ret < 0){
			continue; //just for test
			//Calibration_SyncResponse(ret);
		}else{
			ret = Calibration_ProcessData(&g_nv_sync_data);
			Calibration_SyncResponse(ret);
		}
#endif		
		
	}

#ifndef __DL_UART0__
		__dl_log_share__ = 0;
#endif	

}

#endif /* CONFIG_CALIBRATION_MODE_NEW */
