/*
 * Copyright (c) 2009, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the 
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
//#define DEBUG
#include <common.h>
#include <asm/errno.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include "gadget_chips.h"
#include <linux/ctype.h>
#include <malloc.h>
#include <command.h>
#include <nand.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <jffs2/jffs2.h>
#include <asm/types.h>
#include <android_boot.h>
#include <android_bootimg.h>
#include <boot_mode.h>

typedef struct {
    unsigned char colParity;
    unsigned lineParity;
    unsigned lineParityPrime;
} yaffs_ECCOther;
typedef struct {
    unsigned sequenceNumber;
    unsigned objectId;
    unsigned chunkId;
    unsigned byteCount;
} yaffs_PackedTags2TagsPart;

typedef struct {
    yaffs_PackedTags2TagsPart t;
    yaffs_ECCOther ecc;
} yaffs_PackedTags2;

//#define FASTBOOT_DEBUG
#ifdef FASTBOOT_DEBUG
#define fb_printf(fmt, args...) printf(fmt, ##args)
#else
#define fb_printf(fmt, args...) do {} while(0)
#endif

#ifdef FLASH_PAGE_SIZE
#undef FLASH_PAGE_SIZE
#endif
#define FLASH_PAGE_SIZE 2048

#define ROUND_TO_PAGE(x,y) (((x) + (y)) & (~(y)))

int nand_do_write_ops(struct mtd_info *mtd, loff_t to,struct mtd_oob_ops *ops);
int nand_do_write_oob(struct mtd_info *mtd, loff_t to,
                             struct mtd_oob_ops *ops);

#define GFP_ATOMIC ((gfp_t) 0)


/* todo: give lk strtoul and nuke this */
static unsigned hex2unsigned(const char *x)
{
    unsigned n = 0;

    while(*x) {
        switch(*x) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            n = (n << 4) | (*x - '0');
            break;
        case 'a': case 'b': case 'c':
        case 'd': case 'e': case 'f':
            n = (n << 4) | (*x - 'a' + 10);
            break;
        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
            n = (n << 4) | (*x - 'A' + 10);
            break;
        default:
            return n;
        }
        x++;
    }

    return n;
}

struct fastboot_cmd {
	struct fastboot_cmd *next;
	const char *prefix;
	unsigned prefix_len;
	void (*handle)(const char *arg, void *data, unsigned sz);
};

struct fastboot_var {
	struct fastboot_var *next;
	const char *name;
	const char *value;
};
	
static struct fastboot_cmd *cmdlist;

void fastboot_register(const char *prefix,
		       void (*handle)(const char *arg, void *data, unsigned sz))
{
	struct fastboot_cmd *cmd;
	cmd = malloc(sizeof(*cmd));
	if (cmd) {
		cmd->prefix = prefix;
		cmd->prefix_len = strlen(prefix);
		cmd->handle = handle;
		cmd->next = cmdlist;
		cmdlist = cmd;
	}
}

static struct fastboot_var *varlist;

void fastboot_publish(const char *name, const char *value)
{
	struct fastboot_var *var;
	var = malloc(sizeof(*var));
	if (var) {
		var->name = name;
		var->value = value;
		var->next = varlist;
		varlist = var;
	}
}


//static event_t usb_online;
//static event_t txn_done;
static volatile int txn_done;
static unsigned char buffer[4096];
static struct usb_ep *in, *out;
//static struct udc_request *req;
static struct usb_request *tx_req, *rx_req;
int txn_status;

static void *download_base;
static unsigned download_max;
static unsigned download_size;

#define STATE_OFFLINE	0
#define STATE_COMMAND	1
#define STATE_COMPLETE	2
#define STATE_ERROR	3

static unsigned fastboot_state = STATE_OFFLINE;

//static void req_complete(struct udc_request *req, unsigned actual, int status)
static void req_complete(struct usb_ep *ep, struct usb_request *req)
{
	if (req->status || req->actual != req->length)
		debug("req complete --> %d, %d/%d\n",
				req->status, req->actual, req->length);
	
	txn_status = req->status;
	txn_done = 1;
	/*
	req->length = actual;
	event_signal(&txn_done, 0);
	*/
}

static int usb_read(void *_buf, unsigned len)
{
	int r;
	unsigned xfer;
	unsigned char *buf = _buf;
	int count = 0;
	struct usb_request * req = rx_req;
	
	if (fastboot_state == STATE_ERROR)
		goto oops;

	while (len > 0) {
		xfer = (len > 4096) ? 4096 : len;
		req->buf = buf;
		req->length = xfer;
		req->complete = req_complete;
		//r = udc_request_queue(out, req);
		r = usb_ep_queue(out, req, GFP_ATOMIC);
		if (r < 0) {
			printf("usb_read() queue failed\n");
			goto oops;
		}
		//event_wait(&txn_done);
		txn_done = 0;
		while(!txn_done)
			usb_gadget_handle_interrupts();

		if (txn_status < 0) {
			printf("usb_read() transaction failed\n");
			goto oops;
		}

		count += req->actual;
		buf += req->actual;
		len -= req->actual;

		/* short transfer? */
		if (req->actual != xfer) break;
	}

	return count;

oops:
	fastboot_state = STATE_ERROR;
	return -1;
}

static int usb_write(void *buf, unsigned len)
{
	int r;
	struct usb_request * req = tx_req;
	
	if (fastboot_state == STATE_ERROR)
		goto oops;

	req->buf = buf;
	req->length = len;
	req->complete = req_complete;
	txn_done = 0;
	//r = udc_request_queue(in, req);
	r = usb_ep_queue(in, req, GFP_ATOMIC);
	if (r < 0) {
		printf("usb_write() queue failed\n");
		goto oops;
	}
	//event_wait(&txn_done);
	while(!txn_done)
		usb_gadget_handle_interrupts();
	if (txn_status < 0) {
		printf("usb_write() transaction failed\n");
		goto oops;
	}
	return req->actual;

oops:
	fastboot_state = STATE_ERROR;
	return -1;
}

void fastboot_ack(const char *code, const char *reason)
{
	char response[64] = {0};

	if (fastboot_state != STATE_COMMAND)
		return;

	if (reason == 0)
		reason = "";

	//snprintf(response, 64, "%s%s", code, reason);
	if(strlen(code) + strlen(reason) >= 64) {
		printf("%s too long string\r\n", __func__);
	}
	sprintf(response, "%s%s", code, reason);
	fastboot_state = STATE_COMPLETE;

	usb_write(response, strlen(response));

}

void fastboot_fail(const char *reason)
{
	fastboot_ack("FAIL", reason);
}

void fastboot_okay(const char *info)
{
	fastboot_ack("OKAY", info);
}

static void cmd_getvar(const char *arg, void *data, unsigned sz)
{
	struct fastboot_var *var;

	for (var = varlist; var; var = var->next) {
		if (!strcmp(var->name, arg)) {
			fastboot_okay(var->value);
			return;
		}
	}
	fastboot_okay("");
}

static void dump_log(char * buf, int len)
{
	int i = 0;

	fb_printf("**dump log_buf ...addr:0x%08x, len:%d\r\n", buf, len);

	for (i = 0; i < len; i++)	{
		fb_printf("%02x ", *((unsigned char*)buf+i) );
		if(i%0x20 == 0x1f)
			fb_printf("\n");
	}
}
static void cmd_download(const char *arg, void *data, unsigned sz)
{
	char response[64];
	unsigned len = hex2unsigned(arg);
	int r;

	fb_printf("%s\n", __func__);
	
	fb_printf("arg'%s' data %p, %d\n",arg, data,sz);
	download_size = 0;
	if (len > download_max) {
		fastboot_fail("data too large");
		return;
	}

	sprintf(response,"DATA%08x", len);
	if (usb_write(response, strlen(response)) < 0)
		return;

	r = usb_read(download_base, len);
	if ((r < 0) || (r != len)) {
		fastboot_state = STATE_ERROR;
		return;
	}
	download_size = len;
	fastboot_okay("");
	//dump_log(download_base, len);
}

void cmd_flash(const char *arg, void *data, unsigned sz)
{
	struct mtd_info *nand;
    struct mtd_device *dev;
    struct part_info *part;
	size_t size = 0;
    u8 pnum;
	unsigned extra = 0;
    int ret;

	data = download_base; //previous downloaded date to download_base

	fb_printf("%s, arg:%x date: 0x%x, sz 0x%x\n", __func__, arg, data, sz);
    ret = mtdparts_init();
    if(ret != 0){
        fastboot_fail("mtdparts init error");
        return;
    }

    ret = find_dev_and_part(arg, &dev, &pnum, &part);
    if(ret){
		fastboot_fail("unknown partition name");
        return;
    }else if(dev->id->type != MTD_DEV_TYPE_NAND){
        fastboot_fail("mtd dev type error");
        return;
    }

	nand = &nand_info[dev->id->num];

    nand_erase_options_t opts;
    memset(&opts, 0, sizeof(opts));
    opts.offset = (loff_t)part->offset;
    opts.length = (loff_t)part->size;
    opts.jffs2 = 0;
	opts.quiet = 1;

    fb_printf("opts off  0x%08x\n", (uint32_t)opts.offset);
    fb_printf("opts size 0x%08x\n", (uint32_t)opts.length);
	fb_printf("nand write size 0x%08x\n", nand->writesize);
    ret = nand_erase_opts(nand, &opts);

    if(ret){
      fastboot_fail("nand erase error");
      return;
    }
	
	if (!strcmp(part->name, "boot") || !strcmp(part->name, "recovery")) {
		if (memcmp((void *)data, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
			fastboot_fail("image is not a boot image");
			return;
		}
	}

	if (!strcmp(part->name, "system") || !strcmp(part->name, "userdata"))
		extra = 64;
	else
		sz = ROUND_TO_PAGE(sz, nand->writesize -1);

	size = sz;

	fb_printf("writing 0x%x bytes to '%s' offset: 0x%08x\n", size, part->name, part->offset);
    if(!extra){ // boot or recovery partition write
        ret = nand_write_skip_bad(nand, (loff_t)part->offset, &size, data);
    }else{
        struct nand_chip *chip = nand->priv;
        chip->ops.mode = MTD_OOB_AUTO;
        chip->ops.len = nand->writesize;
        chip->ops.ooblen = sizeof(yaffs_PackedTags2);
        chip->ops.ooboffs = 0;
        loff_t part_off = (loff_t)part->offset;

        while(size){
            chip->ops.datbuf = (uint8_t *)data;
            chip->ops.oobbuf = (uint8_t *)(data + nand->writesize);
            if(!nand_block_isbad(nand, part_off)){
                ret = nand_do_write_ops(nand, part_off, &(chip->ops));
                if(ret){
                    //fastboot_fail("flash write failure");
                    break;
                    nand->block_markbad(nand, part_off);
                    part_off += nand->writesize;
                    continue;
                }
                data += (nand->writesize + nand->oobsize);
                size -= (nand->writesize + nand->oobsize);
            }
			part_off += nand->writesize;
        }
    }

	if(!ret)
		fastboot_okay("");
	else
		fastboot_fail("flash error");
}

void cmd_erase(const char *arg, void *data, unsigned sz)
{
	struct mtd_info *nand;
    struct mtd_device *dev;
    struct part_info *part;
    u8 pnum;
	unsigned extra = 0;
    int ret;

	fb_printf("%s\n", __func__);

    ret = mtdparts_init();
    if(ret != 0){
        fastboot_fail("mtdparts init error");
        return;
    }

    ret = find_dev_and_part(arg, &dev, &pnum, &part);
    if(ret){
		fastboot_fail("unknown partition name");
        return;
    }else if(dev->id->type != MTD_DEV_TYPE_NAND){
        fastboot_fail("mtd dev type error");
        return;
    }
	
    nand = &nand_info[dev->id->num];
    nand_erase_options_t opts;
    memset(&opts, 0, sizeof(opts));
    opts.offset = (loff_t)part->offset;
    opts.length = (loff_t)part->size;
    opts.jffs2 = 0;
	opts.quiet = 1;

    fb_printf("opts off  0x%08x\n", (uint32_t)opts.offset);
    fb_printf("opts size 0x%08x\n", (uint32_t)opts.length);
	fb_printf("nand write size 0x%08x\n", nand->writesize);
    ret = nand_erase_opts(nand, &opts);
    if(ret)
      fastboot_fail("nand erase error");
    else
      fastboot_okay("");
}

extern void udc_power_off(void);

extern unsigned char raw_header[2048];

void cmd_boot(const char *arg, void *data, unsigned sz)
{
	boot_img_hdr *hdr = raw_header;
	unsigned kernel_actual;
	unsigned ramdisk_actual;
	unsigned kernel_addr;
	unsigned ramdisk_addr;
	char * cmdline;

	fb_printf("%s, arg: %s, data: %p, sz: 0x%x\n", __func__, arg, data, sz);
	memcpy(raw_header, data, 2048);
	if(memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)){
		fb_printf("boot image headr: %s\n", hdr->magic);
		fastboot_fail("bad boot image header");
		return;
	}
	kernel_actual= ROUND_TO_PAGE(hdr->kernel_size,(FLASH_PAGE_SIZE - 1));
	if(kernel_actual<=0){
		fastboot_fail("kernel image should not be zero");
		return;
	}
	ramdisk_actual= ROUND_TO_PAGE(hdr->ramdisk_size,(FLASH_PAGE_SIZE - 1));
	if(ramdisk_actual<0){
		fastboot_fail("ramdisk size error");
		return;
	}
	
	memcpy((void*)hdr->kernel_addr, (void *)data + FLASH_PAGE_SIZE, kernel_actual);
	memcpy((void*)hdr->ramdisk_addr, (void *)data + FLASH_PAGE_SIZE + kernel_actual, ramdisk_actual);
	
	fb_printf("kernel @0x%08x (0x%08x bytes)\n", hdr->kernel_addr, kernel_actual);
	fb_printf("ramdisk @0x%08x (0x%08x bytes)\n", hdr->ramdisk_addr, ramdisk_actual);
	//set boot environment
	if(hdr->cmdline[0]){
		cmdline = (char *)hdr->cmdline;
	}else{
		cmdline = getenv("bootargs");
	}
	fb_printf("cmdline %s\n", cmdline);

	fastboot_okay("");
	udc_power_off();
	creat_atags(hdr->tags_addr, cmdline, hdr->ramdisk_addr, hdr->ramdisk_size);
	boot_linux(hdr->kernel_addr,hdr->tags_addr);
}

extern int do_cboot(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[]);

void cmd_continue(const char *arg, void *data, unsigned sz)
{
	fastboot_okay("");
	udc_power_off();
	//do_cboot(NULL, 0, 1, NULL);
    normal_mode();
}

void cmd_reboot(const char *arg, void *data, unsigned sz)
{
	fastboot_okay("");
	//udc_power_off();
    reboot_devices(NORMAL_MODE);
}

void cmd_reboot_bootloader(const char *arg, void *data, unsigned sz)
{
	fastboot_okay("");
	udc_power_off();
    reboot_devices(FASTBOOT_MODE);
}

void cmd_powerdown(const char *arg, void *data, unsigned sz)
{
	fastboot_okay("");
    power_down_devices(0);

}

static void fastboot_command_loop(void)
{
	struct fastboot_cmd *cmd;
	int r;
	fb_printf("fastboot: processing commands\n");

again:
	while (fastboot_state != STATE_ERROR) {
		memset(buffer, 0 , 64);
		r = usb_read(buffer, 64);
		if (r < 0) break;
		buffer[r] = 0;
		fb_printf("fastboot: %s, r:%d\n", buffer, r);

		for (cmd = cmdlist; cmd; cmd = cmd->next) {
			fb_printf("cmd list :%s \n", cmd->prefix);
			if (memcmp(buffer, cmd->prefix, cmd->prefix_len))
				continue;
			fastboot_state = STATE_COMMAND;
			cmd->handle((const char*) buffer + cmd->prefix_len,
				    (void*) download_base, download_size);
			if (fastboot_state == STATE_COMMAND)
				fastboot_fail("unknown reason");
			goto again;
		}

		fastboot_fail("unknown command");
			
	}
	fastboot_state = STATE_OFFLINE;
	fb_printf("fastboot: oops!\n");
}

static int fastboot_handler(void *arg)
{
	for (;;) {
		fastboot_command_loop();
	}
	return 0;
}

/*
static void fastboot_notify(struct udc_gadget *gadget, unsigned event)
{
	if (event == UDC_EVENT_ONLINE) {
		event_signal(&usb_online, 0);
	}
}

static struct udc_endpoint *fastboot_endpoints[2];

static struct udc_gadget fastboot_gadget = {
	.notify		= fastboot_notify,
	.ifc_class	= 0xff,
	.ifc_subclass	= 0x42,
	.ifc_protocol	= 0x03,
	.ifc_endpoints	= 2,
	.ifc_string	= "fastboot",
	.ept		= fastboot_endpoints,
};
*/
#if defined(CONFIG_CMD_FASTBOOT)
int do_fastboot (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	fb_printf("%s is alive\n", __func__);
	while(1){
		usb_gadget_handle_interrupts();
	}
	
	return 0;
}

U_BOOT_CMD(
	fastboot,	1,	1,	do_fastboot,
	"android fastboot protocol",
);
#endif

int fastboot_init(void *base, unsigned size, struct usb_ep * ep_in, struct usb_ep *ep_out)
{
	printf("fastboot_init()\n");

	download_base = base;
	download_max = size;
	if(!ep_in) {
		printf("ep in is not alloc\r\n");
	}
	in = ep_in;
	
	if(!ep_out) {
		printf("ep out is not alloc\r\n");
	}
	out = ep_out;

	tx_req = usb_ep_alloc_request(in, 0);
	rx_req =  usb_ep_alloc_request(out, 0);
/*
	in = udc_endpoint_alloc(UDC_TYPE_BULK_IN, 512);
	if (!in)
		goto fail_alloc_in;
	out = udc_endpoint_alloc(UDC_TYPE_BULK_OUT, 512);
	if (!out)
		goto fail_alloc_out;

	fastboot_endpoints[0] = in;
	fastboot_endpoints[1] = out;

	req = udc_request_alloc();
	if (!req)
		goto fail_alloc_req;

	if (udc_register_gadget(&fastboot_gadget))
		goto fail_udc_register;
*/
/*
	static char cmd1[] = "getvar:";
	fastboot_register(cmd1, cmd_getvar);
*/
	fastboot_register("getvar:", cmd_getvar);
	fastboot_register("download:", cmd_download);
	//fastboot_register("flash:", cmd_flash);
	fastboot_publish("version", "1.0");

	fastboot_register("flash:", cmd_flash);
    fastboot_register("erase:", cmd_erase);
	fastboot_register("boot", cmd_boot);
	fastboot_register("reboot", cmd_reboot);
	fastboot_register("powerdown", cmd_powerdown);
	fastboot_register("continue", cmd_continue);
	fastboot_register("reboot-bootloader", cmd_reboot_bootloader);
	
    //fastboot_register(
	fastboot_handler(0);

	return 0;
/*
fail_udc_register:
	udc_request_free(req);
fail_alloc_req:
	udc_endpoint_free(out);	
fail_alloc_out:
	udc_endpoint_free(in);
fail_alloc_in:
	return -1;
*/
}

