#include <config.h>
#include <common.h>
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


static int s_is_calibration_mode = 0;

/* calibration support uart only */
#ifdef CONFIG_MODEM_CALI_UART

#define mdelay(_ms) udelay(_ms*1000)
#define CALIBERATE_STRING_LEN 10
#define CALIBERATE_HEAD 0x7e
#define CALIBERATE_COMMOND_T 0xfe
#define CALIBERATE_COMMAND_REQ  1

#define CALIBERATE_DEVICE_NULL  0
#define CALIBERATE_DEVICE_USB   1
#define CALIBERATE_DEVICE_UART  2

typedef  struct tag_cali_command {
	unsigned int   	reserved;
	unsigned short  size;
	unsigned char   cmd;
	unsigned char   sub_cmd;
} COMMAND_T;

extern int serial_tstc(void);
static unsigned long long start_time;
static unsigned long long now_time;

static caliberate_device = CALIBERATE_DEVICE_NULL;

static void send_caliberation_request(void)
{
	COMMAND_T cmd;
        unsigned int i;
        unsigned char *data = (unsigned char *)&cmd;

        cmd.reserved = 0;
        cmd.cmd = CALIBERATE_COMMOND_T;
        cmd.size = CALIBERATE_STRING_LEN-2;
        cmd.sub_cmd = CALIBERATE_COMMAND_REQ;

        serial_putc(CALIBERATE_HEAD);

        for (i = 0; i < sizeof(COMMAND_T); i++)
             serial_putc(data[i]);

        serial_putc(CALIBERATE_HEAD);
}

static int receive_caliberation_response(uint8_t *buf,int len)
{
        int count = 0;
        int ch;
        uint32_t is_not_empty = 0;
        uint32_t start_time = 0,current_time = 0;

	if ((buf == NULL) || (len == 0))
        	return 0;
        
        is_not_empty = serial_tstc();
        if (is_not_empty) {
             start_time = get_timer_masked();
             do {
                  do {
                  	ch = serial_getc();
                  	if (count < CALIBERATE_STRING_LEN)
		        	buf[count++] = ch;
                  } while (serial_tstc());

                  if ((count >= CALIBERATE_STRING_LEN) || (count >= len)) {
                       caliberate_device = CALIBERATE_DEVICE_UART;
                       break;
                  }

                  current_time = get_timer_masked();
             } while((current_time - start_time) < 500);
        }
        
        return count;
}

unsigned int check_caliberate(uint8_t * buf, int len)
{
	unsigned int command = 0;
    	unsigned int freq = 0;

	if (len != CALIBERATE_STRING_LEN)
		return 0;

	if ((*buf == CALIBERATE_HEAD) && (*(buf + len -1) == CALIBERATE_HEAD)) {
		if ((*(buf+7) == CALIBERATE_COMMOND_T) && (*(buf + len - 2) != 0x1)) {
			command = *(buf + len - 2);
			command &= 0xf;

            		freq = *(buf + 1);
            		freq = freq << 8;
            		freq += *(buf + 2);

            		command += freq << 8;
		}
	}

	return command;
}

void calibration_detect(int key)
{
	int ret;
	int i ;
	unsigned int caliberate_mode;
	char cmd_buf[50];
	uint8_t buf[20];
	int got = 0;

	printf("%s\n", "uart calibrate detecting");
	loff_t off = 0;    	
    	send_caliberation_request();
   
#ifdef CONFIG_MODEM_CALIBERATE
	for(i = 0; i < 20; i++)
		buf[i] = i + 'a';

    	start_time = get_timer_masked();
    	while (1) {
   		got = receive_caliberation_response(buf, sizeof(buf));
   		if (caliberate_device == CALIBERATE_DEVICE_UART)
			break;

    		now_time = get_timer_masked();
    		if ((now_time - start_time) > CALIBRATE_ENUM_MS) {
      			printf("usb calibrate configuration timeout\n");
			return;
    		}
    	}	

	printf("caliberate : what got from host total %d is \n", got);
	for (i = 0; i < got; i++)
		printf("0x%x ", buf[i]);
	printf("\n");

	caliberate_mode = check_caliberate(buf, CALIBERATE_STRING_LEN);
	if (!caliberate_mode) {
        	printf("func: %s line: %d caliberate failed\n", __func__, __LINE__);
		return;
        } else {
		memset(cmd_buf, 0, 50);
              	if (caliberate_device == CALIBERATE_DEVICE_UART)
                   	sprintf(cmd_buf, "calibration=%d,%d,0", caliberate_mode&0xff, (caliberate_mode&(~0xff)) >> 8);
        	vlx_nand_boot(BOOT_PART, cmd_buf, BACKLIGHT_OFF);
	}	
    
    	/* nerver come to here */
   	return;
#endif
}

#else	/* usb calibration only */

extern int dwc_otg_driver_init(void);
extern void dwc_otg_driver_cleanup(void);
extern int usb_fastboot_initialize(void);
extern int usb_serial_init(void);
extern void  usb_serial_cleanup(void);
extern int usb_serial_configed;
extern int usb_read_done;
extern int usb_write_done;
extern int usb_trans_status;
extern int usb_gadget_handle_interrupts(void);

extern int gs_open(void);
extern int gs_close(void);
extern int gs_read(const unsigned char *buf, int *count);
extern int gs_write(const unsigned char *buf, int count);
extern void usb_wait_trans_done(int direct);
extern int usb_is_trans_done(int direct);
extern int usb_is_configured(void);
extern int usb_is_port_open(void);
extern void udc_power_on(void);
extern void udc_power_off(void);

#define mdelay(_ms) udelay(_ms*1000)

//#define DEBUG
#ifdef DEBUG
#define dprintf(fmt, args...) printf(fmt, ##args)
#else
#define dprintf(fmt, args...) 
#endif

#ifdef DEBUG
#define IO_DEBUG 1     // open write and read debug
#define WRITE_DEBUG 1  // 0: for read debug, 1 for write debug
#else
#define IO_DEBUG 0
#endif


#define CALIBERATE_STRING_LEN 10
#define CALIBERATE_HEAD 0x7e
#define CALIBERATE_COMMOND_T 0xfe

extern int get_cal_enum_ms(void);
extern int get_cal_io_ms(void);
extern void calibration_mode(const uint8_t *pcmd, int length);		

unsigned int check_caliberate(uint8_t * buf, int len)
{
	unsigned int command = 0;
    unsigned int freq = 0;
	if(len != CALIBERATE_STRING_LEN)
		return 0;

	if((*buf == CALIBERATE_HEAD) && (*(buf + len -1) == CALIBERATE_HEAD)){
		if((*(buf+7)==CALIBERATE_COMMOND_T) && (*(buf+len-2) != 0x1)){
			command = *(buf+len-2);
			command &= 0xf;
            freq = *(buf+1);
            freq = freq<<8;
            freq += *(buf+2);
            command += freq<<8;
		}
	}
	return command;
}

extern int power_button_pressed(void);
static int count_ms;
static unsigned long long start_time;
static unsigned long long now_time;

static int recheck_power_button(void)
{
    int cnt = 0;
    int ret = 0;
    do{
        ret = power_button_pressed();
        if(ret == 0)
          cnt++;
        else
          return 1;

        if(cnt>4)
          return 0;
        else{
            mdelay(1);
        }
    }while(1);
}
int is_timeout(int key)
{
    if(!key){
        if(!recheck_power_button() || charger_connected())
          return 2;
    }
    
    now_time = get_timer_masked();

    if(now_time - start_time>count_ms)
      return 1;
    else{
        return 0;
    }
}
    
void calibration_detect(int key)
{
	int ret;
	int i ;
    unsigned int caliberate_mode;
    char * cmd_buf = NULL;
	loff_t off = 0;
    printf("%s\n", "usb calibrate detecting");

    count_ms = get_cal_enum_ms();
   // extern lcd_display(void);
   // extern void set_backlight(uint32_t value);
   // lcd_printf("   caliberation mode");
   // lcd_display();
   // set_backlight(50);

#ifdef CONFIG_MODEM_CALIBERATE
	uint8_t buf[20];
	for(i = 0; i<20; i++)
		buf[i] = i+'a';
	dwc_otg_driver_init();
	usb_serial_init();
#if IO_DEBUG 
	while(!usb_serial_configed)
		usb_gadget_handle_interrupts();
	printf("USB SERIAL CONFIGED\n");
	gs_open();
#if WRITE_DEBUG
	while(1){
		ret = gs_write(buf, 20);
		printf("func: %s waitting write done\n", __func__);
		if(usb_trans_status)
			printf("func: %s line %d usb trans with error %d\n", __func__, __LINE__, usb_trans_status);
		usb_wait_trans_done(1);
		printf("func: %s readly send %d\n", __func__, ret);
    }		
#else
	while(1){
		int count = 20;
		usb_wait_trans_done(0);	
		if(usb_trans_status)
					printf("func: %s line %d usb trans with error %d\n", __func__, __LINE__, usb_trans_status);
		ret = gs_read(buf, &count);
		printf("func: %s readly read %d\n", __func__, count);		
		if(usb_trans_status)
			printf("func: %s line %d usb trans with error %d\n", __func__, __LINE__, usb_trans_status);
		for(i = 0; i<count; i++)
			printf("%c ", buf[i]);
		printf("\n");
	}	

#endif
#endif
    start_time = get_timer_masked();
    while(!usb_is_configured()){
        ret = is_timeout(key);
        if(ret == 0)
          continue;
        else if(ret == 2) // POWER KEY pressed
          power_down_devices();
        else{
            printf("usb calibrate configuration timeout\n");
            return;
        }
    }	
	printf("USB SERIAL CONFIGED\n");

    start_time = get_timer_masked();
    count_ms = get_cal_io_ms();
    while(!usb_is_port_open()){
        ret = is_timeout(key);
        if(ret == 0)
          continue;
        else if(ret == 2) // POWER KEY pressed
          power_down_devices();
        else{
            printf("usb calibrate port open timeout\n");
            return;
        }
    }	
	printf("USB SERIAL PORT OPENED\n");
	gs_open();
//code for caliberate detect
	int got = 0;
	int count = CALIBERATE_STRING_LEN;
	dprintf("start to calberate\n");
	
	while(got < CALIBERATE_STRING_LEN){
		if(usb_is_trans_done(0))		
//	while(got < CALIBERATE_STRING_LEN){
//		usb_wait_trans_done(0);
        {		
            if(usb_trans_status)
              printf("func: %s line %d usb trans with error %d\n", __func__, __LINE__, usb_trans_status);
            ret = gs_read(buf + got, &count);
            if(usb_trans_status)
              printf("func: %s line %d usb trans with error %d\n", __func__, __LINE__, usb_trans_status);
            for(i=0; i<count; i++)
              dprintf("0x%x \n", buf[got+i]);
            dprintf("\n");
            got+=count;
        }

        if(got<CALIBERATE_STRING_LEN){
            ret = is_timeout(key);
            if(ret == 0){
                count=CALIBERATE_STRING_LEN-got;
                continue;
            }else if(ret == 2){
                power_down_devices();
            }else{
                printf("usb read timeout\n");
                return;
            }
        }else{
            break;
        }
	}

	printf("caliberate:what got from host total %d is \n", got);
	for(i=0; i<got;i++)
		printf("0x%x ", buf[i]);
	printf("\n");

	caliberate_mode = check_caliberate(buf, CALIBERATE_STRING_LEN);
	dprintf("check_caliberate return is 0x%x\n", ret);
	if(!caliberate_mode){
        printf("func: %s line: %d caliberate failed\n", __func__, __LINE__);
		return;
    }
	else{
		ret = gs_write(buf, got);
		dprintf("func: %s waitting %d write done\n", __func__, got);
		if(usb_trans_status)
			printf("func: %s line %d usb trans with error %d\n", __func__, __LINE__, usb_trans_status);
		usb_wait_trans_done(1);
		udc_power_off();
        cmd_buf=malloc(50);
        if(cmd_buf==NULL){
            printf("%s: out of memory\n", __func__);
            return;
        }
        sprintf(cmd_buf, "calibration=%d,%d", caliberate_mode&0xff, (caliberate_mode&(~0xff))>>8);
        s_is_calibration_mode=1;
        vlx_nand_boot(BOOT_PART, cmd_buf, BACKLIGHT_OFF);
	}	
    
    //nerver come to here
    free(cmd_buf);
    return;
#endif
}

#endif // CONFIG_MODEM_CALI_UART

int poweron_by_calibration(void)
{
	return s_is_calibration_mode;
}

