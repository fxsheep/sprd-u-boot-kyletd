#ifndef NORMAL_MODE_H
#define NORMAL_MODE_H

#include <config.h>
#include <common.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#include <linux/string.h>
#include <android_bootimg.h>
#include <linux/mtd/mtd.h>
#include <linux/crc32b.h>
#include <linux/mtd/nand.h>
#include <nand.h>
#include <android_boot.h>
#include <environment.h>
#include <jffs2/jffs2.h>
#include <boot_mode.h>
#include <malloc.h>
#include <asm/arch/secure_boot.h>


extern unsigned spl_data_buf[0x1000] __attribute__((align(4)));
extern unsigned harsh_data_buf[8]__attribute__((align(4)));
extern void *spl_data;
extern void *harsh_data;
extern unsigned char raw_header[8192];


#define VMJALUNA_PART "vmjaluna"
#define MODEM_PART "modem"
#define KERNEL_PART "kernel"
#define FIXNV_PART "fixnv"
#define BACKUPFIXNV_PART "backupfixnv"
#define RUNTIMEVN_PART "runtimenv"
#define DSP_PART "dsp"
#define SPL_PART "spl"

#ifdef CONFIG_TIGER
#define DSP_ADR			0x80020000
#define VMJALUNA_ADR		0x80400000
#define FIXNV_ADR		0x80480000
#define RUNTIMENV_ADR		0x804a0000
#define MODEM_ADR		0x80500000
#define RAMDISK_ADR 		0x85500000
#if BOOT_NATIVE_LINUX
//pls make sure uboot running area
#define VLX_TAG_ADDR            (0x82000100)
#define KERNEL_ADR		(0x82008000)

#else

#define KERNEL_ADR		0x84508000
#define VLX_TAG_ADDR            0x85100000 //after initrd

#endif

#else
#define DSP_ADR			0x00020000
#define VMJALUNA_ADR		0x00400000
#define FIXNV_ADR		0x00480000
#define RUNTIMENV_ADR		0x004a0000
#define MODEM_ADR		0x00500000
#define RAMDISK_ADR 		0x05500000

#if BOOT_NATIVE_LINUX
//pls make sure uboot running area
#define VLX_TAG_ADDR            (0x100)
#define KERNEL_ADR		(0x8000)

#else

#define KERNEL_ADR		0x04508000
#define VLX_TAG_ADDR            0x5100000 //after initrd

#endif
#endif
/////////////////////////////////////////////////////////
/* log_switch function */
#if 0
GSM_Download_Param include the following :

system_param_version  short
fcch_det_struct		12 * short
freq_est_struct		6 * short
freq_track_algo_struct	4 * short
AGC_algo_cal_struct		12 * short
time_track_algo_struct	4 * short
rx_equalizer_struct		3 * short
rx_deinterlever_struct	1 * short
power_scan_struct		----------------->  power_scan_saturation_thresh		1 * short
                                               power_exp_lev					3 * short
                                               power_underflow_thresh			3 * short
tone_para_struct		1 * short
action_tables_struct		0x120 * short
audio_filter_coef		----------------->  HPF_coef					36 * short
                                               LPF_coef					15 * short
dsp_uart_struct		1 * short
deep_sleep_struct		5 * short
log_switch_struct		-----------------> DSP_log_switch                             1 * short
#endif

struct power_scan_saturation_thresh_tag {
	unsigned short	power_scan_saturation_thresh;
};

struct power_exp_lev_tag {
	unsigned short	power_exp_lev[3];
};

struct power_underflow_thresh_tag {
	unsigned short	power_underflow_thresh[3];
};

struct HPF_coef_tag {
	unsigned short	HPF_coef[36];
};

struct LPF_coef_tag {
	unsigned short	LPF_coef[15];
};

struct power_scan_struct_tag {
	struct power_scan_saturation_thresh_tag power_scan_saturation_thresh;
	struct power_exp_lev_tag power_exp_lev;
	struct power_underflow_thresh_tag power_underflow_thresh;
};

struct audio_filter_coef_tag {
	struct HPF_coef_tag HPF_coef;
	struct LPF_coef_tag LPF_coef;
};

struct DSP_log_switch_tag {
	unsigned short DSP_log_switch_value;
};

struct log_switch_struct_tag {
	struct DSP_log_switch_tag DSP_log_switch;
};

struct GSM_Download_Param_Tag {
	unsigned short system_param_version;
	unsigned short fcch_det_struct[12];
	unsigned short freq_est_struct[6];
	unsigned short freq_track_algo_struct[4];
	unsigned short AGC_algo_cal_struct[12];
	unsigned short time_track_algo_struct[4];
	unsigned short rx_equalizer_struct[3];
	unsigned short rx_deinterlever_struct;
	struct power_scan_struct_tag power_scan_struct;
	unsigned short tone_para_struct;
	unsigned short action_tables_struct[0x120];
	struct audio_filter_coef_tag audio_filter_coef;
	unsigned short dsp_uart_struct;
	unsigned short deep_sleep_struct[5];
	struct log_switch_struct_tag log_switch_struct;
};
/////////////////////////////////////////////////////////

#define NV_MAGIC		(0x53544e56)
#define INVALID_NV_MAGIC	(0xffffffff)

#define STATUS_VALID		(0x00000001)
#define STATUS_DELETED		(0x00000002)
#define STATUS_MASK		(0x00000003)

struct npb_tag {
	unsigned long magic;
	unsigned long timestamp;
	unsigned short min_id;
	unsigned short max_id;
	unsigned short tot_scts;
	unsigned short sct_size;
	unsigned short dir_entry_count;
	unsigned short dir_entry_size;
	unsigned long next_offset;
	unsigned char backup_npb;
	unsigned char backup_dir;
};

struct nv_dev {
	struct npb_tag *npb;
	unsigned long tot_size;
	unsigned short first_dir_sct;
	unsigned short first_backup_dir_sct;
	unsigned long data_offset;
	unsigned char *runtime;
};

struct direntry_tag {
	unsigned short size;
	unsigned short checksum;
	unsigned long offset;
	unsigned long status;
	unsigned long reserved;
};

struct item_tag {
	unsigned short id;
	unsigned short size;
};



#define MAX_SN_LEN 			(24)
#define SP09_MAX_SN_LEN			MAX_SN_LEN
#define SP09_MAX_STATION_NUM		(15)
#define SP09_MAX_STATION_NAME_LEN	(10)
#define SP09_SPPH_MAGIC_NUMBER          (0X53503039)    // "SP09"
#define SP09_MAX_LAST_DESCRIPTION_LEN   (32)

typedef struct _tagSP09_PHASE_CHECK
{
	unsigned long 	Magic;                	// "SP09"
	char    	SN1[SP09_MAX_SN_LEN]; 	// SN , SN_LEN=24
	char    	SN2[SP09_MAX_SN_LEN];    // add for Mobile
	int     	StationNum;                 	// the test station number of the testing
	char    	StationName[SP09_MAX_STATION_NUM][SP09_MAX_STATION_NAME_LEN];
	unsigned char 	Reserved[13];               	//
	unsigned char 	SignFlag;
	char    	szLastFailDescription[SP09_MAX_LAST_DESCRIPTION_LEN];
	unsigned short  iTestSign;				// Bit0~Bit14 ---> station0~station 14
	                 		  			  //if tested. 0: tested, 1: not tested
	unsigned short  iItem;    // part1: Bit0~ Bit_14 indicate test Station,1 : Pass,

}SP09_PHASE_CHECK_T, *LPSP09_PHASE_CHECK_T;

extern const int SP09_MAX_PHASE_BUFF_SIZE;

void set_vibrator(int on);
void vibrator_hw_init(void);
void MMU_InvalideICACHEALL(void);
int is_factorymode();
int read_spldata();
void addbuf(char *buf);
void addcmdline(char *buf);
int eng_getphasecheck(SP09_PHASE_CHECK_T* phase_check);
int eng_phasechecktest(unsigned char *array, int len);
int fixnv_is_correct(unsigned char *array, unsigned long size);
int fixnv_is_correct_endflag(unsigned char *array, unsigned long size);
unsigned long get_nv_index(unsigned char *array, unsigned long size);
int runtimenv_is_correct(unsigned char *array, unsigned long size);
int sn_is_correct(unsigned char *array, unsigned long size);
unsigned long get_productinfo_index(unsigned char *array);

int nv_is_correct(unsigned char *array, unsigned long size);
int nv_is_correct_endflag(unsigned char *array, unsigned long size);
void lcd_display_logo(int backlight_set,ulong bmp_image,size_t size);
void creat_cmdline(char * cmdline,boot_img_hdr *hdr);
void vlx_entry();

#endif /* NORMAL_MODE_H */
