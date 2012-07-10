
#include <malloc.h>

#define NV_MULTI_LANG_ID   (405)
#define GSM_CALI_ITEM_ID   (0x2)
#define GSM_IMEI_ITEM_ID   (0x5)
#define XTD_CALI_ITEM_ID   (0x516)
#define LTE_CALI_ITEM_ID   (0x9C4)
#define BT_ITEM_ID         (0x191)

#define BT_ADDR_LEN  6

#define IMEI_LEN			(8)
#define GSM_CALI_VER_A      0xFF0A
#define GSM_CALI_VER_MIN    GSM_CALI_VER_A
#define GSM_CALI_VER_MAX    GSM_CALI_VER_A

#define NUM_TEMP_BANDS		(5)
#define NUM_RAMP_RANGES		(16)		/* constant parameter numbers, 16 level */
#define NUM_TX_LEVEL		(16)		/* 2db per step */
#define NUM_RAMP_POINTS		(20)
#define NUM_GSM_ARFCN_BANDS	(6)
#define NUM_DCS_ARFCN_BANDS	(8)
#define NUM_PCS_ARFCN_BANDS	(7)
#define NUM_GSM850_ARFCN_BANDS	(6)
#define MAX_COMPENSATE_POINT	(75)

enum
{
	ERR_NONE,
	ERR_NOT_FIND_PARAM_IN_FILE,
    	ERR_NOT_FIND_PARAM_IN_MODULE,
    	ERR_PARAM_LEN_NOT_MATCH,
    	ERR_UNKNOWN_VER_IN_MODULE,
    	ERR_OLD_VERSION,
    	ERR_UNKNOWN_VER_IN_FILE,  
	ERR_PARAM_LEN_NOT_MATCH_DEF
};


typedef struct BT_CONFIG_T
{
	unsigned char  bt_addr[BT_ADDR_LEN];
	unsigned short xtal_dac;
}BT_CONFIG;

/* GSM Calibration */
typedef struct 
{
	short stage0[NUM_RAMP_POINTS];
	short stage1[NUM_RAMP_POINTS];
	short stage2[NUM_RAMP_POINTS];
	short stage3[NUM_RAMP_POINTS];
	short stage4[NUM_RAMP_POINTS];
	short stage5[NUM_RAMP_POINTS];
	short stage6[NUM_RAMP_POINTS];
	short stage7[NUM_RAMP_POINTS];
	short stage8[NUM_RAMP_POINTS];
	short stage9[NUM_RAMP_POINTS];
	short stage10[NUM_RAMP_POINTS];
	short stage11[NUM_RAMP_POINTS];
	short stage12[NUM_RAMP_POINTS];
	short stage13[NUM_RAMP_POINTS];
	short stage14[NUM_RAMP_POINTS];
	short stage15[NUM_RAMP_POINTS];

} RF_ramp_table_T;

typedef struct
{
    /* unsigned short RF_ID;
    unsigned short RF_Project_ID; */	
    unsigned short slope_length ;
    unsigned short cdac ;
    unsigned short cafc ;
    unsigned short slope_struct[3][17] ;
    
}RF_aero2_AFC_T ;

typedef struct
{
    /************* AFC  ********************/
    unsigned short is_using_si4134;        /* TRUE, the si4134 from Silicon lab is used. Then the way to calibrate
    // the 13Mhz is different. For si4134, Total 64 steps( 1.0 ppm per step)
    // can be set to set coarse 13Mhz. */
    unsigned short si4134_coarse_13m;      /* For si4134, it is 6bits, and for setting the CDAC register of si4134 */
    
    unsigned short afc_center;
    unsigned short afc_slope_per_6bits;
    
}RF_aero1_AFC_T ; 
typedef struct
{
    /************* AFC  ********************/
    RF_aero1_AFC_T rf_aero1_afc_struct;
    /* Oscillator temperature calibration table. */
    short  osci_temp_comp_in_dac[ NUM_TEMP_BANDS ];     /* In Hz */
    /* Temp -30 ~ -11 
       Temp -10 ~ 9 
       Temp  10 ~ 29 
       Temp  30 ~ 49 
       Temp  50 ~ 69 */
    RF_aero2_AFC_T rf_aero2_afc_struct;                                                        
} RF_param_common_DSP_use_T;

typedef struct
{
    /* short RF_ramp_up_table_stage0[NUM_RAMP_POINTS]; //64 words totally
    short RF_ramp_up_table_stage1[NUM_RAMP_POINTS];
    short RF_ramp_down_table_stage0[NUM_RAMP_POINTS];
    short RF_ramp_down_table_stage1[NUM_RAMP_POINTS]; */
    short rf_ramp_param_constant_up[NUM_RAMP_RANGES]; /* const part of the ramp tale */
    short rf_ramp_param_constant_down[NUM_RAMP_RANGES]; /* const part of the ramp tale */
    short rf_ramp_up_param_num; /* the number of ramp up parameters in the ramp parameters table for one level */
    short rf_ramp_down_param_num; /* the number of ramp down */

    /* short temperature_and_voltage_compensate_structrue[30]*/
    short reserved[64 - 2*NUM_RAMP_RANGES - 1 - 1]; /* keep 64words totally to keep consistency with dsp code */

    short TX_compensate_boundary_arfcn_index[4];
    short RF_ramppwr_step_factor[5][NUM_TX_LEVEL];
    short RF_ramp_delta_timing[NUM_TX_LEVEL];
    short RF_ramp_PA_power_on_duration[NUM_TX_LEVEL];
    /* short RX_compensate_boundary_arfcn_index[4];
    short RX_compensate_value[5]; */
    short reserved1[9];
    short temperature_RX_compensate_boundary_ADC[4];
    short temperature_RX_compensate_value[5];
    short temperature_TX_compensate_boundary_ADC[4];
    short temperature_TX_rampwr_step_factor[5];
    short  ramp_switch_level;
    unsigned short afc_alg_bias;
    unsigned short agc_ctrl_word[91];   /* The index is the expected received signal strangth in db in 2 db steps,
                                           the content is the control word set the RF receiver chain agc. */
    short  max_rf_gain_index;
    short  reserve_for_future[20];
    
    short  RX_compensate_value[MAX_COMPENSATE_POINT];
    RF_ramp_table_T RF_ramp_table;
    
} RF_param_band_DSP_use_T;

typedef struct
{
    RF_param_common_DSP_use_T   rf_common_param_dsp_use;
    RF_param_band_DSP_use_T     rf_gsm_param_dsp_use;
    RF_param_band_DSP_use_T     rf_dcs_param_dsp_use;
    RF_param_band_DSP_use_T     rf_pcs_param_dsp_use;
    RF_param_band_DSP_use_T     rf_gsm850_param_dsp_use;	
}RF_param_DSP_use_T;

typedef struct
{
    char    rx_whole_band_comp_gsm[NUM_GSM_ARFCN_BANDS];
    char    rx_whole_band_comp_dcs[NUM_DCS_ARFCN_BANDS];
    char    rx_whole_band_comp_pcs[NUM_PCS_ARFCN_BANDS];
    char    rx_whole_band_comp_gsm850[NUM_GSM850_ARFCN_BANDS];        
    char    pa_GSM_temp_comp[ NUM_TEMP_BANDS];
    
    char    pa_DCS_temp_comp[ NUM_TEMP_BANDS];
    
    char    pa_PCS_temp_comp[ NUM_TEMP_BANDS];
    char    pa_GSM850_temp_comp[ NUM_TEMP_BANDS];
    
    /******************** RX level calculation *********************/
    /* The received signal level(RSSI) value reported to the network may need to be adjusted based on the received signal
    level. */
    char    rxlev_GSM_temp_comp[ NUM_TEMP_BANDS];
    
    char    rxlev_DCS_temp_comp[ NUM_TEMP_BANDS];
    
    char    rxlev_PCS_temp_comp[ NUM_TEMP_BANDS];
    char    rxlev_GSM850_temp_comp[ NUM_TEMP_BANDS];
    
    /****** PA ramp compensation for battery voltage variation. *******/
    char    pa_GSM_vcc_comp_threshold;      /* If the voltage is below the threshold, pa compensation is needed. Battery measure ADC value. */
    char    pa_GSM_vcc_comp_step;           /* The PA ramp level need to be changed per threshold. */
    char    pa_DCS_vcc_comp_threshold;      /* If the voltage is below the threshold, pa compensation is needed. */
    char    pa_DCS_vcc_comp_step;           /* The PA ramp level need to be changed per threshold. */
    char    pa_PCS_vcc_comp_threshold;      /* If the voltage is below the threshold, pa compensation is needed. */
    char    pa_PCS_vcc_comp_step;           /* The PA ramp level need to be changed per threshold. */
    char    pa_GSM850_vcc_comp_threshold;   /* If the voltage is below the threshold, pa compensation is needed. Battery measure ADC value. */
    char    pa_GSM850_vcc_comp_step;           /* The PA ramp level need to be changed per threshold. */
    
} RF_param_MCU_use_T;

typedef struct
{
    /**************** Battery monitor *********************/
    /* Battery measurement calibration.
       Due to the unprecise of the divider resistor value for measure the battery voltage, the voltage
       value measured may be not accurate enough for battery charging. */
    unsigned long batt_mV_per_4bits;   /* The actual voltage for 4bits ADC output. */
    
    /******************* 32K/13M calibration. ***************/
    /*the ratio of 32k and 13Mhz needs to be calibrated running
    for deep sleep mode */
    
    unsigned short  clock_32k_cal_interval; /* How often the calibration shall be invoked. */
    unsigned short  clock_32k_cal_duration; /* How long the calibration shall be done. */
}Misc_cal_MCU_use_T;

typedef struct
{
    unsigned long    adc[2];           /* calibration of ADC, two test point */
    unsigned long    battery[2];       /* calibraton of battery(include resistance), two test point */
    unsigned long    reserved[8];      /* reserved for feature use. */
}ADC_T;

typedef struct
{
    unsigned short              calibration_param_version;
    RF_param_DSP_use_T  rf_param_dsp_use;
    RF_param_MCU_use_T  rf_param_mcu_use;
    Misc_cal_MCU_use_T  misc_cal_mcu_use;
    short                   mic_gain;        /* In DB */
    short                   auxi_mic_gain;   /* In DB */
    unsigned short                  PM_version;      /* the version of phone module */
    
    /* Following Added by Lin.liu. (2003-08-29), CR: MS4474 */
    unsigned short                  software_version;   /* Software version of Tools */
    unsigned short                  station_num;
    unsigned char                   operate_code[ 16 ];
    unsigned long                  date;
    
    ADC_T                   adc;             /* Added By Lin.liu, for ADC calibration End Lin.liu Added. */ 
    
} calibration_param_T;

#define XTD_CALI_VER_3            0x0003
#define XTD_CALI_VER_4            0x0004

/* TD Calibration */
/* td_calibration_struct_v3 */
#define AGC_GAIN_LEN_QS3200_V3	1495
#define AGC_CTL_LEN_QS3200	115
#define APC_CTL_LEN_QS3200_V3	1300
#define AGC_COMP_LEN_QS3200	61
#define APC_COMP_LEN_QS3200	613

typedef struct  
{   
       unsigned short   cali_version;
       unsigned short   antenna_switch_pin_control;
       unsigned short   Antenna_switch_truth_table[8];
       unsigned short   rf_afc_dac;
       unsigned short   rf_afc_slope;
       unsigned short   agc_ctl_word[AGC_CTL_LEN_QS3200];
       unsigned short   agc_gain_word[AGC_GAIN_LEN_QS3200_V3]; 	/* 115*3+115+115*8+115 */
       unsigned short   AGC_compensation[AGC_COMP_LEN_QS3200];	/* 1+30*2 */
       unsigned short   tdpa_pin_control;
       unsigned short   TDPA_APC_control_word_index[APC_CTL_LEN_QS3200_V3]; /* 100*4+100*9 */
       unsigned short   APC_compensation[APC_COMP_LEN_QS3200];  /* 30+30+60+60+30+1+(4*13+20)*2+1+1+256 */
       unsigned short   rf_register_num;
       unsigned long    rf_Register_Initial[50];
       unsigned long    rf_action_tbl[281];
       unsigned char    reserved[500];

}td_calibration_v3_T;

/* td_calibration_struct_v4 */
#define AGC_GAIN_LEN_QS3200_V4	2875
#define APC_CTL_LEN_QS3200_V4	2500
typedef struct  
{
	unsigned short   cali_version;
	unsigned short   rf_afc_dac;
	unsigned short   rf_afc_slope;
	unsigned short   slope_length;
	unsigned short   CDAC;
	unsigned short   CAFC;
	unsigned short   agc_gain_word[AGC_GAIN_LEN_QS3200_V4]; 		/* 115*12 + 115*4 + 115*9 */
	unsigned short   TDPA_APC_control_word_index[APC_CTL_LEN_QS3200_V4]; 	/* 100*12 + 100*4 + 100*9 */
}td_calibration_v4_T;

/* LTE Calibration */
#define LTE_CALI_VER_1            0x0001

/* LTE_NV_TYPE_FAST_PLLLOCK */
typedef struct _LTE_NV_CALI_FAST_PLLLOCK
{
	unsigned short plllock_array[64];
}LTE_NV_CALI_FAST_PLLLOCK;
	
/* LTE_NV_TYPE_AGC_GAIN_MAIN */
typedef struct _LTE_NV_CALI_AGC_GAIN_MAIN
{
	unsigned short agc_gain_main_24G_center_array[124];
	unsigned short agc_gain_main_24G_channel_array[21 + 20];
	unsigned short agc_gain_main_26G_center_array[124];
	unsigned short agc_gain_main_26G_channel_array[11+20];
}LTE_NV_CALI_AGC_GAIN_MAIN;
	
/* LTE_NV_TYPE_AGC_DIV_MAIN */
typedef struct _LTE_NV_CALI_AGC_GAIN_DIV
{
	unsigned short agc_gain_div_24G_center_array[124];
	unsigned short agc_gain_div_24G_channel_array[21 + 20];
	unsigned short agc_gain_div_26G_center_array[124];
	unsigned short agc_gain_div_26G_channel_array[11 + 20];
}LTE_NV_CALI_AGC_GAIN_DIV;
	
/* LTE_NV_TYPE_AFC_CONTROL */
typedef struct _LTE_NV_CALI_AFC_CONTROL
{
	unsigned short AFC0;
	unsigned short AFC_K;
}LTE_NV_CALI_AFC_CONTROL;
	
/* LTE_NV_TYPE_APC_CONTROL_WORD_MAIN */
typedef struct _LTE_NV_CALI_APC_CONTROL_WORD_MAIN
{
	unsigned short apc_control_main_24G_array[22][80];
	unsigned short apc_control_main_26G_array[12][80];
}LTE_NV_CALI_APC_CONTROL_WORD_MAIN;
	
/* LTE_NV_TYPE_APC_CONTROL_WORD_DIV */
typedef struct _LTE_NV_CALI_APC_CONTROL_WORD_DIV
{
	unsigned short apc_control_div_24G_array[22][80];
	unsigned short apc_control_div_26G_array[12][80];
}LTE_NV_CALI_APC_CONTROL_WORD_DIV;
	
typedef struct _LTE_NV_CALI_PARAM_T
{
	unsigned short                     CALI_PARAM_VERSION;
	unsigned short                     CALI_PARAM_FLAG;
	
	LTE_NV_CALI_FAST_PLLLOCK            FAST_PLLLOCK;
	LTE_NV_CALI_AGC_GAIN_MAIN           AGC_GAIN_MAIN;
	LTE_NV_CALI_AGC_GAIN_DIV            AGC_GAIN_DIV;
	LTE_NV_CALI_AFC_CONTROL             AFC_CONTROL;
	LTE_NV_CALI_APC_CONTROL_WORD_MAIN   APC_CONTROL_WORD_MAIN;
	LTE_NV_CALI_APC_CONTROL_WORD_DIV    APC_CONTROL_WORD_DIV;    
}LTE_NV_CALI_PARAM_T;


static unsigned long XCheckNVStruct(unsigned char *lpPhoBuf, unsigned long dwPhoSize, 
	unsigned long bBigEndian, unsigned long bModule)
{
	unsigned long dwOffset = 0, dwLength = 0, bRet;
	unsigned char *lpCode = lpPhoBuf;
	unsigned long dwCodeSize = dwPhoSize;
	unsigned short wCurID;

	dwOffset = 4;     /* Skip first four bytes,that is time stamp */
    	dwLength = 0;
    	unsigned char *pTemp = lpCode + dwOffset;

	unsigned long bIMEI = 0;
	unsigned long bGSMCali = 0;
	unsigned short wGSMCaliVer = 0;
    
    	while (dwOffset < dwCodeSize) {
		wCurID = *(unsigned short *)pTemp;
        	pTemp += 2;

		dwLength = *(unsigned short *)pTemp;
		/* printf("wCurID = 0x%08x  dwLength = 0x%08x\n", wCurID, dwLength); */
		if (wCurID == GSM_IMEI_ITEM_ID) {
			if (dwLength != IMEI_LEN) {
				return 0;
			} else {
				bIMEI = 1;
			}
		} else if (wCurID == GSM_CALI_ITEM_ID) {
			wGSMCaliVer =  *(unsigned short *)(pTemp + 2); /* pTemp + 2: skip length */
	
			/* printf("wGSMCaliVer = 0x%08x\n", wGSMCaliVer); */
			if ((wGSMCaliVer > GSM_CALI_VER_MAX) || (wGSMCaliVer < GSM_CALI_VER_MIN)) {
				return 0;
			} else {
				bGSMCali = 1;
			}
		}

		/* 0xFFFF is end-flag in module (NV in phone device) */
		if (wCurID == 0xFFFF) {
			if (!bIMEI || !bGSMCali) {
				return 0;
			}
			return 1;
		}

		if (wCurID == 0 || dwLength == 0) {
			break;
		}

        	pTemp += 2;
        	dwOffset += 4;
        	/* Must be four byte aligned */
        	bRet = dwLength % 4;
        	if (bRet != 0) {
            		dwLength += 4 - bRet;
        	}
        
        	dwOffset += dwLength;
       		pTemp += dwLength;
		
		/* (dwOffset == dwCodeSize) is end condition in File */
		if (dwOffset == dwCodeSize) {
			if(!bIMEI || !bGSMCali) {
				return 0;
			}
			return 1;
		}
	}

	return 0;
}

unsigned long XCheckNVStructEx(unsigned char *lpPhoBuf, unsigned long dwPhoSize, unsigned long bBigEndian, 
	unsigned long bModule)
{
	if (!XCheckNVStruct(lpPhoBuf, dwPhoSize, bBigEndian, bModule)) {
		printf("%s %d\n", __FUNCTION__, __LINE__);
		return 0;
	}

	return 1;
}

unsigned long XFindNVOffset(unsigned short wId, unsigned char *lpCode, unsigned long dwCodeSize, 
	unsigned long *dwOffset, unsigned long *dwLength, unsigned long bBigEndian)
{
    unsigned short wCurID;
    unsigned char *pTemp;
    unsigned long bRet;

    *dwOffset = 4;     /* Skip first four bytes,that is time stamp */
    *dwLength = 0;
    pTemp = lpCode + *dwOffset;
    
    while (*dwOffset < dwCodeSize) {
    	wCurID = *(unsigned short *)pTemp;

        pTemp += 2;

	*dwLength = *(unsigned short *)pTemp;

        pTemp += 2;
        *dwOffset += 4;
        /* Must be four byte aligned */
        bRet = (*dwLength) % 4;
        if (bRet != 0) {
            *dwLength += 4 - bRet;
        }
	/* printf("wCurID = 0x%08x  dwLength = 0x%08x  bRet = %d\n", wCurID, *dwLength, bRet); */
	/* 0xFFFF is end-flag */
	if (wCurID == 0xFFFF) {
		return 0;
	}
        
        if (wCurID == wId) {
		/* check length */
		printf("dwOffset = 0x%08x  dwLength = 0x%08x  bRet = %d\n", *dwOffset, *dwLength, bRet);
		if ((*dwOffset + *dwLength - bRet) <= dwCodeSize)
			return 1;
		else
			return 0;
        } else {
            	*dwOffset += *dwLength;
            	pTemp += *dwLength;
        }
    }
    
    return 0;
}

unsigned long XFindNVOffsetEx(unsigned short wId, unsigned char *lpCode, unsigned long dwCodeSize, 
	unsigned long *dwOffset, unsigned long *dwLength, unsigned long bBigEndian, unsigned long bModule)
{
	unsigned long bOK = 0;

	if (XCheckNVStructEx(lpCode, dwCodeSize, 0, bModule)) {
		if (XFindNVOffset(wId, lpCode, dwCodeSize, dwOffset, dwLength, bBigEndian))
			bOK = 1;
	}

	return bOK;
}

unsigned long XCheckCalibration(unsigned char *lpPhoBuf, unsigned long dwPhoSize, unsigned long bModule)
{
	unsigned long dwPhoCaliFlag = 0;
	unsigned long dwOffsetPho = 0;
	unsigned long dwLengthPho = 0;
    	unsigned char *pSrcPho = lpPhoBuf;

	/*calibration_struct_va::*/calibration_param_T paraPho;	

    	if (!XFindNVOffsetEx(GSM_CALI_ITEM_ID, pSrcPho, dwPhoSize, &dwOffsetPho, &dwLengthPho, 0, bModule)) {
		printf("Not found cali in phone");
		return 0;

    	}

	printf("dwOffsetPho = 0x%08x  dwLengthPho = 0x%08x\n", dwOffsetPho, dwLengthPho);

	unsigned char *pCaliBuf = pSrcPho + dwOffsetPho;
	unsigned short wVerPho = *(unsigned short *)pCaliBuf;
	printf("wVerPho = 0x%08x  size = 0x%08x\n", wVerPho, sizeof(paraPho));
	if (wVerPho != GSM_CALI_VER_A) {
		printf("Cali version in phone is wrong\n");
		return 0;
	}

	if (dwLengthPho != sizeof(paraPho)) {
		printf("Cali length in phone is wrong\n");
		return 0;
	}
	
    	memcpy(&paraPho, pSrcPho + dwOffsetPho, sizeof(paraPho));
	dwPhoCaliFlag = paraPho.adc.reserved[7];

	if (paraPho.adc.reserved[7] == 0) {	
		printf("GSM Calibration in phone is not calibrated, Reserved[7] : 0x%08X\n", dwPhoCaliFlag);	
		return 0;
	}

	return 1;
}


static void GSM_VerAToA(unsigned char *pFileBuf, unsigned long dwOffsetFile, unsigned long bFileBigEndian, 
			unsigned char *pModuBuf, unsigned long dwOffsetModu, unsigned long bModuBigEndian,
			unsigned long CaliPolicy)
{
	int i = 0;
    	/*calibration_struct_va::*/calibration_param_T para_modu, para_file;
    	
	memcpy(&para_modu, pModuBuf + dwOffsetModu, sizeof(para_modu));
    	memcpy(&para_file, pFileBuf + dwOffsetFile, sizeof(para_file));
    
	if (CaliPolicy == 0 || CaliPolicy == 1 ) {
		/* not backup RF_ramp_delta_timing */
		for (i = 0; i < /*calibration_struct_va::*/NUM_TX_LEVEL; i++) {
			para_modu.rf_param_dsp_use.rf_gsm_param_dsp_use.RF_ramp_delta_timing[i] = para_file.rf_param_dsp_use.rf_gsm_param_dsp_use.RF_ramp_delta_timing[i];
			para_modu.rf_param_dsp_use.rf_dcs_param_dsp_use.RF_ramp_delta_timing[i] = para_file.rf_param_dsp_use.rf_dcs_param_dsp_use.RF_ramp_delta_timing[i];
			para_modu.rf_param_dsp_use.rf_pcs_param_dsp_use.RF_ramp_delta_timing[i] = para_file.rf_param_dsp_use.rf_pcs_param_dsp_use.RF_ramp_delta_timing[i];
			para_modu.rf_param_dsp_use.rf_gsm850_param_dsp_use.RF_ramp_delta_timing[i] = para_file.rf_param_dsp_use.rf_gsm850_param_dsp_use.RF_ramp_delta_timing[i];
		}

		if (CaliPolicy == 1) {
			 /* For HTC */
			/* not backup RF_ramp_PA_power_on_duration */
			for (i = 0; i < /*calibration_struct_va::*/NUM_TX_LEVEL; i++) {
				para_modu.rf_param_dsp_use.rf_gsm_param_dsp_use.RF_ramp_PA_power_on_duration[i] = para_file.rf_param_dsp_use.rf_gsm_param_dsp_use.RF_ramp_PA_power_on_duration[i];
				para_modu.rf_param_dsp_use.rf_dcs_param_dsp_use.RF_ramp_PA_power_on_duration[i] = para_file.rf_param_dsp_use.rf_dcs_param_dsp_use.RF_ramp_PA_power_on_duration[i];
				para_modu.rf_param_dsp_use.rf_pcs_param_dsp_use.RF_ramp_PA_power_on_duration[i] = para_file.rf_param_dsp_use.rf_pcs_param_dsp_use.RF_ramp_PA_power_on_duration[i];
				para_modu.rf_param_dsp_use.rf_gsm850_param_dsp_use.RF_ramp_PA_power_on_duration[i] = para_file.rf_param_dsp_use.rf_gsm850_param_dsp_use.RF_ramp_PA_power_on_duration[i];
			}
			/* not backup RF_ramp_table */
			memcpy(&para_modu.rf_param_dsp_use.rf_gsm_param_dsp_use.RF_ramp_table,
				   &para_file.rf_param_dsp_use.rf_gsm_param_dsp_use.RF_ramp_table,
				   sizeof(para_modu.rf_param_dsp_use.rf_gsm_param_dsp_use.RF_ramp_table));
			
			memcpy(&para_modu.rf_param_dsp_use.rf_dcs_param_dsp_use.RF_ramp_table,
				   &para_file.rf_param_dsp_use.rf_dcs_param_dsp_use.RF_ramp_table,
				   sizeof(para_modu.rf_param_dsp_use.rf_dcs_param_dsp_use.RF_ramp_table));

			memcpy(&para_modu.rf_param_dsp_use.rf_pcs_param_dsp_use.RF_ramp_table,
				   &para_file.rf_param_dsp_use.rf_pcs_param_dsp_use.RF_ramp_table,
				   sizeof(para_modu.rf_param_dsp_use.rf_pcs_param_dsp_use.RF_ramp_table));

			memcpy(&para_modu.rf_param_dsp_use.rf_gsm850_param_dsp_use.RF_ramp_table,
				   &para_file.rf_param_dsp_use.rf_gsm850_param_dsp_use.RF_ramp_table,
				   sizeof(para_modu.rf_param_dsp_use.rf_gsm850_param_dsp_use.RF_ramp_table));

			/* not backup reserved "temperature_and_voltage_composate_structure" */
			memcpy(&(para_modu.rf_param_dsp_use.rf_gsm_param_dsp_use.reserved[0]),
				   &(para_file.rf_param_dsp_use.rf_gsm_param_dsp_use.reserved[0]),
				   sizeof(para_modu.rf_param_dsp_use.rf_gsm_param_dsp_use.reserved));
			
			memcpy(&(para_modu.rf_param_dsp_use.rf_dcs_param_dsp_use.reserved[0]),
				   &(para_file.rf_param_dsp_use.rf_dcs_param_dsp_use.reserved[0]),
				   sizeof(para_modu.rf_param_dsp_use.rf_dcs_param_dsp_use.reserved));
			
			memcpy(&(para_modu.rf_param_dsp_use.rf_pcs_param_dsp_use.reserved[0]),
				   &(para_file.rf_param_dsp_use.rf_pcs_param_dsp_use.reserved[0]),
				   sizeof(para_modu.rf_param_dsp_use.rf_pcs_param_dsp_use.reserved));
			
			memcpy(&(para_modu.rf_param_dsp_use.rf_gsm850_param_dsp_use.reserved[0]),
				   &(para_file.rf_param_dsp_use.rf_gsm850_param_dsp_use.reserved[0]),
				   sizeof(para_modu.rf_param_dsp_use.rf_gsm850_param_dsp_use.reserved));
		}

		memcpy(pFileBuf + dwOffsetFile, &para_modu, sizeof(para_modu));
	} else {
		/* adc */
		memcpy(&para_file.adc, &para_modu.adc, sizeof(para_file.adc));
		/* afc */
		memcpy(&para_file.rf_param_dsp_use.rf_common_param_dsp_use,
			   &para_modu.rf_param_dsp_use.rf_common_param_dsp_use,
				sizeof(para_file.rf_param_dsp_use.rf_common_param_dsp_use));

		/*calibration_struct_va::*/RF_param_band_DSP_use_T *pFile = NULL;
		/*calibration_struct_va::*/RF_param_band_DSP_use_T *pModu = NULL;

		for (i = 0; i < 4; i++) {	
			switch (i) {
			case 0:
				pFile = &para_file.rf_param_dsp_use.rf_gsm_param_dsp_use;
				pModu = &para_modu.rf_param_dsp_use.rf_gsm_param_dsp_use;
			break;
			case 1:
				pFile = &para_file.rf_param_dsp_use.rf_dcs_param_dsp_use;
				pModu = &para_modu.rf_param_dsp_use.rf_dcs_param_dsp_use;
			break;
			case 2:
				pFile = &para_file.rf_param_dsp_use.rf_pcs_param_dsp_use;
				pModu = &para_modu.rf_param_dsp_use.rf_pcs_param_dsp_use;
			break;
			case 3:
				pFile = &para_file.rf_param_dsp_use.rf_gsm850_param_dsp_use;
				pModu = &para_modu.rf_param_dsp_use.rf_gsm850_param_dsp_use;
			break;
			}
			
			/* agc */
			/* agc_ctrl_word */
			memcpy(pFile->agc_ctrl_word, pModu->agc_ctrl_word, sizeof(pFile->agc_ctrl_word));
			/* RX_compensate_value */
			memcpy(pFile->RX_compensate_value, pModu->RX_compensate_value, sizeof(pFile->RX_compensate_value));
			/* max_rf_gain_index */
			memcpy(&pFile->max_rf_gain_index, &pModu->max_rf_gain_index, sizeof(pFile->max_rf_gain_index));	
			/* apc */
			/* rf_ramp_param_constant_value */
			memcpy( pFile->rf_ramp_param_constant_up, pModu->rf_ramp_param_constant_up, 
				sizeof(pFile->rf_ramp_param_constant_up));
			/* rf_edge_tx_gain_table */
			memcpy(pFile->rf_ramp_param_constant_down, pModu->rf_ramp_param_constant_down,
			 sizeof(pFile->rf_ramp_param_constant_down));
			/* RF_ramppwr_step_factor */
			memcpy(pFile->RF_ramppwr_step_factor, pModu->RF_ramppwr_step_factor, 
				sizeof(pFile->RF_ramppwr_step_factor));
			/* rf_8psk_tx_compensation */
			memcpy(pFile->reserved1, pModu->reserved1, sizeof(pFile->reserved1));
		}

		memcpy(pFileBuf + dwOffsetFile, &para_file, sizeof(para_file));
	}    
}

unsigned long GSMCaliPreserve(unsigned char *lpCode, unsigned long dwCodeSize, unsigned char *lpReadBuffer, 
			unsigned long dwReadSize, unsigned long bOldReplaceNew, unsigned long bContinue, 
			unsigned long CaliPolicy)
{
    	unsigned long dwOffsetFile = 0, dwLengthFile = 0;
    	unsigned long dwOffsetModu = 0, dwLengthModu = 0;
    	unsigned char *pFileBuf = lpCode;
	unsigned char *pModuBuf = lpReadBuffer;

	/* Find calibration in file */
    	if (!XFindNVOffsetEx(GSM_CALI_ITEM_ID, pFileBuf, dwCodeSize, &dwOffsetFile, &dwLengthFile, 0, 1)) {
		printf("%s %d\n", __FUNCTION__, __LINE__);		
		return ERR_NOT_FIND_PARAM_IN_FILE;
	}

	/*printf("dwOffsetFile = 0x%08x  dwLengthFile = 0x%08x\n", dwOffsetFile, dwLengthFile);*/
	unsigned short wVerFile = *(unsigned short *)(pFileBuf + dwOffsetFile);

    	/* Find calibration in phone */
    	if (!XFindNVOffsetEx(GSM_CALI_ITEM_ID, pModuBuf, dwReadSize, &dwOffsetModu, &dwLengthModu, 0, 1)) {
		if (bContinue)
			return ERR_NONE;
		else
			return ERR_NOT_FIND_PARAM_IN_MODULE;
	}

	/*printf("dwOffsetModu = 0x%08x  dwLengthModu = 0x%08x\n", dwOffsetModu, dwLengthModu);*/
    	unsigned short wVerModu = *(unsigned short *)(pModuBuf + dwOffsetModu);
	
	/* Update the timestamp to force nv manager to reload it */
	/* (*(unsigned long*)lpCode) = GetTickCount(); xin hongliang masked */

	unsigned long dwSizeAligned = 0;

	if (GSM_CALI_VER_A == wVerFile) { 
		dwSizeAligned = sizeof(/*calibration_struct_va::*/calibration_param_T);	
		dwSizeAligned = ((unsigned long)((dwSizeAligned + 3) / 4)) * 4;

		if (dwLengthFile != dwSizeAligned) {
            		return ERR_PARAM_LEN_NOT_MATCH_DEF;
        	}
		
        	if (GSM_CALI_VER_A == wVerModu) {
			if (dwLengthFile == dwLengthModu) {
				/* Version and length are both equal,just do copying */
				/* memcpy( pSrc + dwOffsetFile,lpReadBuffer + dwOffset,dwLength ); */
				GSM_VerAToA(pFileBuf, dwOffsetFile, 0, pModuBuf, dwOffsetModu, 0, CaliPolicy);
				return ERR_NONE;
			} else
				return ERR_PARAM_LEN_NOT_MATCH;
		} else
            		return ERR_UNKNOWN_VER_IN_MODULE;
	} else
		return ERR_UNKNOWN_VER_IN_FILE;
}

static void XTD_Ver3To3(unsigned char *pFileBuf, unsigned long dwOffsetFile, unsigned long bFileBigEndian, 
	unsigned char *pModuBuf, unsigned long dwOffsetModu, unsigned long bModuBigEndian)
{
	/*td_calibration_struct_v3::*/td_calibration_v3_T para_modu;
    	/*td_calibration_struct_v3::*/td_calibration_v3_T para_file;

    	memcpy(&para_modu, pModuBuf + dwOffsetModu, sizeof(para_modu));
    	memcpy(&para_file, pFileBuf + dwOffsetFile, sizeof(para_file));

	para_file.rf_afc_dac = para_modu.rf_afc_dac;
	para_file.rf_afc_slope = para_modu.rf_afc_slope;

    	memcpy(&(para_file.agc_ctl_word[0]), &(para_modu.agc_ctl_word[0]),
		(unsigned char *)&(para_modu.agc_ctl_word[/*td_calibration_struct_v3::*/AGC_CTL_LEN_QS3200 - 1]) - 
		(unsigned char *)&(para_modu.agc_ctl_word[0]) + sizeof(unsigned short));

    	memcpy(&(para_file.agc_gain_word[0]), &(para_modu.agc_gain_word[0]),
		(unsigned char *)&(para_modu.agc_gain_word[/*td_calibration_struct_v3::*/AGC_GAIN_LEN_QS3200_V3 - 1]) -
		(unsigned char *)&(para_modu.agc_gain_word[0]) + sizeof(unsigned short));

	memcpy(&(para_file.TDPA_APC_control_word_index[0]), &(para_modu.TDPA_APC_control_word_index[0]),
		(unsigned char *)&(para_modu.TDPA_APC_control_word_index[/*td_calibration_struct_v3::*/APC_CTL_LEN_QS3200_V3 - 1]) - (unsigned char *)&(para_modu.TDPA_APC_control_word_index[0]) + sizeof(unsigned short));

	memcpy(pFileBuf + dwOffsetFile, &para_file, sizeof(para_file));
}

static void XTD_Ver4To4(unsigned char *pFileBuf, unsigned long dwOffsetFile, unsigned long bFileBigEndian, 
	unsigned char *pModuBuf, unsigned long dwOffsetModu, unsigned long bModuBigEndian )
{
	/*td_calibration_struct_v4::*/td_calibration_v4_T para_modu;
    	/*td_calibration_struct_v4::*/td_calibration_v4_T para_file;

    	memcpy(&para_modu, pModuBuf + dwOffsetModu, sizeof(para_modu));
    	memcpy(&para_file, pFileBuf + dwOffsetFile, sizeof(para_file));

	memcpy(pFileBuf + dwOffsetFile, &para_modu, sizeof(para_modu));
}

unsigned long XTDCaliPreserve(unsigned char *lpCode, unsigned long dwCodeSize, 
	unsigned char *lpReadBuffer, unsigned long dwReadSize, 
	unsigned long bOldReplaceNew, unsigned long bContinue)
{
    	unsigned long dwOffsetFile = 0, dwLengthFile = 0;
	unsigned long dwOffsetModu = 0, dwLengthModu = 0;
    	unsigned char *pFileBuf = lpCode;
	unsigned char *pModuBuf = lpReadBuffer;

	/* Find calibration in file */
    	if (!XFindNVOffsetEx(XTD_CALI_ITEM_ID, pFileBuf, dwCodeSize, &dwOffsetFile, &dwLengthFile, 0, 1))
		return ERR_NOT_FIND_PARAM_IN_FILE;
	printf("dwOffsetFile = 0x%08x  dwLengthFile = 0x%08x\n", dwOffsetFile, dwLengthFile);
	unsigned short wVerFile = *(unsigned short *)(pFileBuf + dwOffsetFile);
	printf("wVerFile = 0x%08x\n", wVerFile);

    	/* Find calibration in phone */
    	if (!XFindNVOffsetEx(XTD_CALI_ITEM_ID, pModuBuf, dwReadSize, &dwOffsetModu, &dwLengthModu, 0, 1)) {
		if (bContinue)
			return ERR_NONE;
		else
			return ERR_NOT_FIND_PARAM_IN_MODULE;
	}
	printf("dwOffsetModu = 0x%08x  dwLengthModu = 0x%08x\n", dwOffsetModu, dwLengthModu);
    	unsigned short wVerModu = *(unsigned short*)(pModuBuf + dwOffsetModu);
	printf("wVerModu = 0x%08x\n", wVerModu);

	/* Update the timestamp to force nv manager to reload it */
	/* (*(unsigned long *)lpCode) = GetTickCount(); richard.feng masked */

	unsigned long dwSizeAligned = 0;

	if (XTD_CALI_VER_4 == wVerFile) { 
		dwSizeAligned = sizeof(/*td_calibration_struct_v4::*/td_calibration_v4_T);
		dwSizeAligned = ((unsigned long)((dwSizeAligned + 3) / 4)) * 4;
		printf("dwSizeAligned = 0x%08x  dwLengthFile = 0x%08x\n", dwSizeAligned, dwLengthFile);
		if (dwLengthFile != dwSizeAligned)
            		return ERR_PARAM_LEN_NOT_MATCH_DEF;
		
        	if (XTD_CALI_VER_4 == wVerModu) {
			if (dwLengthFile == dwLengthModu) {
				/* Version and length are both equal,just do copying */
				/* memcpy( pSrc + dwOffsetFile,lpReadBuffer + dwOffset,dwLength ); */
				printf("%s %d\n", __FUNCTION__, __LINE__);
				XTD_Ver4To4(pFileBuf, dwOffsetFile, 0, pModuBuf, dwOffsetModu, 0);
				return ERR_NONE;
			} else
				return ERR_PARAM_LEN_NOT_MATCH;
		} else
            		return ERR_UNKNOWN_VER_IN_MODULE;
	} else if (XTD_CALI_VER_3 == wVerFile) {
		dwSizeAligned = sizeof(/*td_calibration_struct_v3::*/td_calibration_v3_T);
		dwSizeAligned = ((unsigned long)((dwSizeAligned + 3) / 4)) * 4;
		printf("dwSizeAligned = 0x%08x  dwLengthFile = 0x%08x\n", dwSizeAligned, dwLengthFile);
		if (dwLengthFile != dwSizeAligned)
            		return ERR_PARAM_LEN_NOT_MATCH_DEF;
		
        	if (XTD_CALI_VER_3 == wVerModu) {
			if (dwLengthFile == dwLengthModu) {
				/* Version and length are both equal,just do copying */
				/* memcpy( pSrc + dwOffsetFile,lpReadBuffer + dwOffset,dwLength ); */
				printf("%s %d\n", __FUNCTION__, __LINE__);
				XTD_Ver3To3(pFileBuf, dwOffsetFile, 0, pModuBuf, dwOffsetModu, 0);
				return ERR_NONE;
			} else
				return ERR_PARAM_LEN_NOT_MATCH;
		} else
            		return ERR_UNKNOWN_VER_IN_MODULE;
	} else
		return ERR_UNKNOWN_VER_IN_FILE;
}

static void LTE_Ver1To1(unsigned char *pFileBuf, unsigned long dwOffsetFile, unsigned long bFileBigEndian, 
			unsigned char *pModuBuf, unsigned long dwOffsetModu,unsigned long bModuBigEndian)
{
	/*lte_calibration_struct_v1::*/LTE_NV_CALI_PARAM_T para_modu;
    	/*lte_calibration_struct_v1::*/LTE_NV_CALI_PARAM_T para_file;
	
    	memcpy(&para_modu, pModuBuf + dwOffsetModu, sizeof(para_modu));
    	memcpy(&para_file, pFileBuf + dwOffsetFile, sizeof(para_file));
	
	memcpy(pFileBuf + dwOffsetFile, &para_modu, sizeof(para_modu));
}

unsigned long LTECaliPreserve(unsigned char *lpCode, unsigned long dwCodeSize, 
                         unsigned char *lpReadBuffer, unsigned long dwReadSize,
                         unsigned long bOldReplaceNew , unsigned long bContinue )
{
    	unsigned long dwOffsetFile = 0, dwLengthFile = 0;
	unsigned long dwOffsetModu = 0, dwLengthModu = 0;
    	unsigned char *pFileBuf = lpCode;
	unsigned char *pModuBuf = lpReadBuffer;

	/* Find calibration in file */
    	if (!XFindNVOffsetEx(LTE_CALI_ITEM_ID, pFileBuf, dwCodeSize, &dwOffsetFile, &dwLengthFile, 0, 1))
		return ERR_NOT_FIND_PARAM_IN_FILE;
	unsigned short wVerFile = *(unsigned short *)(pFileBuf + dwOffsetFile);

    	/* Find calibration in phone */
   	if (!XFindNVOffsetEx(LTE_CALI_ITEM_ID, pModuBuf, dwReadSize, &dwOffsetModu, &dwLengthModu, 0, 1)) {
		if (bContinue)
			return ERR_NONE;
		else
			return ERR_NOT_FIND_PARAM_IN_MODULE;
	}
    	unsigned short wVerModu = *(unsigned short *)(pModuBuf + dwOffsetModu);

	/* Update the timestamp to force nv manager to reload it */
	/* (*(unsigned long*)lpCode) = GetTickCount(); richard.feng masked */

	unsigned long dwSizeAligned = 0;

	if (LTE_CALI_VER_1 == wVerFile) { 
		dwSizeAligned = sizeof(/*lte_calibration_struct_v1::*/LTE_NV_CALI_PARAM_T);
		dwSizeAligned = ((unsigned long)((dwSizeAligned + 3) / 4)) * 4;
		
		if (dwLengthFile != dwSizeAligned)
            		return ERR_PARAM_LEN_NOT_MATCH_DEF;

	        if (LTE_CALI_VER_1 == wVerModu) {
			if (dwLengthFile == dwLengthModu) {
				/* Version and length are both equal,just do copying */
				/* memcpy( pSrc + dwOffsetFile,lpReadBuffer + dwOffset,dwLength ); */
				LTE_Ver1To1(pFileBuf, dwOffsetFile, 0, pModuBuf, dwOffsetModu, 0);
				return ERR_NONE;
			} else
				return ERR_PARAM_LEN_NOT_MATCH;
		} else
            		return ERR_UNKNOWN_VER_IN_MODULE;
	} else
		return ERR_UNKNOWN_VER_IN_FILE;
}

unsigned long XPreserveNVItem(unsigned short wID, unsigned char *lpCode, unsigned long dwCodeSize, 
				unsigned char *lpReadBuffer, unsigned long dwReadSize, 
				unsigned long bOldReplaceNew, unsigned long bContinue)
{
    	unsigned long dwOffsetFile, dwLengthFile;
	unsigned long dwOffsetModu, dwLengthModu;

	if (wID == GSM_CALI_ITEM_ID || wID == XTD_CALI_ITEM_ID)
		return ERR_NONE;
    	printf("wID = 0x%08x\n", wID);
    	if (!XFindNVOffsetEx(wID, lpReadBuffer, dwReadSize, &dwOffsetModu, &dwLengthModu, 0, 1)) {
		if (bContinue)
			return ERR_NONE;
		else
			return ERR_NOT_FIND_PARAM_IN_MODULE;	
	}
	
	printf("wID = 0x%08x  dwOffsetModu = 0x%08x  dwLengthModu = 0x%08x\n", wID, dwOffsetModu, dwLengthModu);

	if (!XFindNVOffsetEx(wID, lpCode, dwCodeSize, &dwOffsetFile, &dwLengthFile, 0, 1))		
		return ERR_NOT_FIND_PARAM_IN_FILE;

	printf("wID = 0x%08x  dwOffsetFile = 0x%08x  dwLengthFile = 0x%08x\n", wID, dwOffsetFile, dwLengthFile);

    	if (dwLengthModu != dwLengthFile)   
        	return ERR_PARAM_LEN_NOT_MATCH;

	/* BT */
	if (wID == BT_ITEM_ID) {
		unsigned long dwSizeAligned = sizeof(BT_CONFIG);
		dwSizeAligned = ((unsigned long)((dwSizeAligned + 3) / 4)) * 4;		
		if(dwLengthFile != dwSizeAligned)
			return ERR_PARAM_LEN_NOT_MATCH_DEF;

		BT_CONFIG para = {0};
		memcpy(&para, lpReadBuffer + dwOffsetModu, dwLengthModu);
		memcpy(lpCode + dwOffsetFile, &para, dwLengthModu);
	} else 
		memcpy(lpCode + dwOffsetFile, lpReadBuffer + dwOffsetModu, dwLengthModu);
    
    	return ERR_NONE;    
}

unsigned long XPreserveIMEIs(unsigned short wID, unsigned char *lpCode, unsigned long dwCodeSize, 
				unsigned char *lpReadBuffer, unsigned long dwReadSize, 
				unsigned long bOldReplaceNew, unsigned long bContinue)
{
	unsigned long dwRet = ERR_NONE;

	if (wID == 0xFFFF) /* IMEI */
		wID = GSM_IMEI_ITEM_ID;

	if (wID == GSM_IMEI_ITEM_ID)
		dwRet = XPreserveNVItem(wID, lpCode, dwCodeSize, lpReadBuffer, dwReadSize, 
					bOldReplaceNew, bContinue);			
	else
		dwRet = XPreserveNVItem(wID, lpCode, dwCodeSize, lpReadBuffer, dwReadSize, bOldReplaceNew, 1);
	
	return dwRet;
}

