/******************************************************************************
 ** File Name:      audio.h                                                   *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           05/10/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define all audio interface.                     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/10/2004     Xueliang.Wang    Create                                    *
 ******************************************************************************/
#ifndef _AUDIO_H
#define _AUDIO_H

#include "os_api.h"
#include "audio_api.h"

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
typedef enum
{
    AUD_DTMF_One,           // 1
    AUD_DTMF_Two,           // 2
    AUD_DTMF_Three,         // 3
    AUD_DTMF_letterA,       // A
    AUD_DTMF_Four,          // 4
    AUD_DTMF_Five,          // 5
    AUD_DTMF_Six,           // 6
    AUD_DTMF_letterB,       // B
    AUD_DTMF_Seven,         // 7
    AUD_DTMF_Eight,         // 8
    AUD_DTMF_Nine,          // 9
    AUD_DTMF_letterC,       // C
    AUD_DTMF_Star,          // *
    AUD_DTMF_Zero,          // 0
    AUD_DTMF_Pond,          // #
    AUD_DTMF_letterD,       // D
    AUD_DTMF_MAX_ID         // Reserved, user can't use it.
} AUD_DTMF_TONE_ID_E;
 
typedef struct
{
    uint16 UL_equalizer_coeff[33];
    uint16 DL_equalizer_coeff[33];
   

    uint16 UL_PGA_gain;
    uint16 DL_PGA_gain;
  
    uint16 UL_digital_gain;
    uint16 DL_digital_gain;

    uint16 UL_digital_scale;
    uint16 DL_digital_scale;

    uint16 midi_PGA_gain_base;
    uint16 digital_sidetone_gain;

    uint16 DL_IIR1_coeff[6];
    uint16 DL_IIR2_coeff[6];

    uint16 reserved[2];
} AUDIO_TEST_DATA_T;


 
#define AUD_PLAY_FOREVER            0xFFFFFFFF

// Volume of max value.
#define AUD_MAX_MIC_VOLUME          9
#define AUD_MAX_SPEAKER_VOLUME      9
#define AUD_MAX_SIDE_TONE_GAIN      10

//background music level
#define  AUD_BKMUSIC_LOWEST_LEVEL       0  //0:mute
#define  AUD_BKMUSIC_HIGHEST_LEVEL      5  
#define  AUD_BKMUSIC_TOTAL_LEVEL        6  
// Echo test callback function.
typedef void (*AUD_ECHOCALLBACK_PFUNC)(uint32);

// Audio error value.
typedef enum
{
    ERR_AUD_NONE,
    ERR_AUD_PARAM,               // Input parameters is error.
    ERR_AUD_CONFLICT,            // It's playing ring or midi now, don't handle tone.
    ERR_AUD_MAX                  // Reserved, user can't use it.
} ERR_AUD_E;


// Audio type.
typedef enum
{
    AUD_TYPE_CUSTOM_TONE,
    AUD_TYPE_DTMF_TONE,
    AUD_TYPE_GENERIC_TONE,
    AUD_TYPE_RING,
    AUD_TYPE_VOICE,
    AUD_TYPE_RECORD,    
    AUD_TYPE_MAX            // Reserved, user can't use it.
} AUD_TYPE_E;

// Generic tone ID.
typedef enum 
{
    AUD_GENERIC_PRESS_KEY_TONE,
    AUD_GENERIC_WARNING_TONE,
    AUD_GENERIC_BATT_WARNING_TONE,
    AUD_GENERIC_DIAL_TONE,
    AUD_GENERIC_CONGESTION_TONE,
    AUD_GENERIC_SUBSCRIBER_BUSY_TONE,
    AUD_GENERIC_SPECIAL_DIAL_TONE,
    AUD_GENERIC_SV_CALL_WAITING_TONE,
    AUD_GENERIC_SV_RADIO_PATH_ACK_TONE,
    AUD_GENERIC_SV_RADIO_PATH_NOT_AVAIL_TONE,
    AUD_GENERIC_SV_ERROR_INFO_TONE,
    AUD_GENERIC_INFO_FREE_TONE,

    // User could add new at here.
    // ......
    
	AUD_GENERIC_TONE_ID_MAX     // Reserved, user can't use it.
} AUD_GENERIC_TONE_ID_E;

// Audio ring data type.
typedef enum
{
    AUD_MIDI_RING,          // File format: 
                            //  General Midi file format
                                                        
    AUD_SINGLE_TONE_RING,   // File format(in BigEndian): 
                            //  uint16(type),       // AUD_SINGLE_TONE_RING
                            //  uint16(data_len),   // Number of bytes of ring data
                            //  uint16(freq),   uint16(duration millisecond),
                            //  uint16(freq),   uint16(duration millisecond),
                            //  ......
                            //  uint16(freq),   uint16(duration millisecond)
                                                        
                            // Note: duration = 0xFFFF, means play for ever.
                                            
    AUD_DUAL_TONE_RING,     // File format(in BigEndian):
                            //  uint16(type),       // AUD_DUAL_TONE_RING
                            //  uint16(data_len),   // Number of bytes of ring data
                            //  uint16(freq1),  uint16(freq2),  uint16(duration millisecond),
                            //  uint16(freq1),  uint16(freq2),  uint16(duration millisecond),
                            //  ......
                            //  uint16(freq1),  uint16(freq2),  uint16(duration millisecond)
                            
    AUD_TRIPLE_TONE_RING,   // File format(in BigEndian):
                            //  uint16(type),       // AUD_TRIPLE_TONE_RING
                            //  uint16(data_len),   // Number of bytes of ring data
                            //  uint16(freq1),  uint16(freq2),  uint16(freq3),  uint16(duration millisecond),
                            //  uint16(freq1),  uint16(freq2),  uint16(freq3),  uint16(duration millisecond),
                            //  ......
                            //  uint16(freq1),  uint16(freq2),  uint16(freq3),  uint16(duration millisecond)

    AUD_SMAF_RING,          // File format:
                            //  Defined by Yamaha.
                            
    AUD_WAVE_RING,          // File format:
                            //  wave sound 16bit samples data ,sound sampling rate is 16k                            
                            
    AUD_MP3_RING,          	// File format:
                            //  mp3
    
    AUD_AMR_RING,           // File format
                            // AMR_NB
    AUD_RING_TYPE_MAX       // Reserved, user can't use it.
} AUD_RING_TYPE_E;

// definition of type of background music volume  .
typedef enum
{
    AUD_BKMUSIC_BOTH_INVALID,/*both uplink and downlink paras are invalid*/
    AUD_BKMUSIC_UPLINK_VALID,        /*uplink para is valid*/
    AUD_BKMUSIC_DOWNLINK_VALID,  /*downlink para is valid*/
    AUD_BKMUSIC_BOTH_VALID,           /*both uplink and downlink paras are valid*/
    AUD_BKMUSIC_PARAM_TYPE_MAX   /* Reserved, user can't use it.*/
} AUD_BKMUSIC_PARAM_TYPE_E; 

typedef enum
{
    AUD_WAV_ADPCM,
    AUD_WAV_MAX
}AUD_WAV_TYPE_E;

typedef enum
{
    AUD_EQ_NORMAL = 0x0,
    AUD_EQ_BASS,
    AUD_EQ_ALT,
    AUD_EQ_SPECIAL
}AUD_EQ_TYPE_E;


typedef struct AUD_SINGLE_TONE_tag
{
    uint16  freq;
    uint16  duration;           // Number of milliseconds
} AUD_SINGLE_TONE_T;

typedef struct AUD_DUAL_TONE_tag
{
    uint16  freq1;
    uint16  freq2;
    uint16  duration;           // Number of milliseconds
} AUD_DUAL_TONE_T;

typedef struct AUD_TRIPLE_TONE_tag
{
    uint16  freq1;
    uint16  freq2;
    uint16  freq3;
    uint16  duration;           // Number of milliseconds
} AUD_TRIPLE_TONE_T;

typedef struct AUD_RING_DATA_INFO_tag
{
    AUD_RING_TYPE_E type;
    uint8           name[32];
    uint32          data_len;  
	const uint8	   *data_ptr;
} AUD_RING_DATA_INFO_T;

enum aud_stereo_mode 
{
    AUD_STEREO 			= 0,	// dual channel 
    AUD_NON_STEREO		= 1		// single channel 
};


// Ring callback function prototype.
typedef void (*AUD_RING_CALLBACK_PFUNC)(void);

typedef struct AUD_RING_PLAY_tag
{
	//xSignalHeaderRec signal_head;
	SIGNAL_VARS 
    uint16  type;
    uint16  sub_type;
    uint32  len;
    uint32  times;
    uint32  sample_rate;
    uint32  clk;
    uint32	file_offset;
    uint8	*data_ptr;
    AUD_RING_CALLBACK_PFUNC callback_pfunc;
} AUD_RING_PLAY_T;

typedef enum
{
	MIDI_PLAY   = 1,
	MP3_PLAY	= 2,
	WAV_PLAY	= 3,
	MP4_PLAY	= 4
} AUD_MSG_SERVICE_E;

typedef enum
{
        AUD_REC_PCM= 0x0,
        AUD_REC_FS, 
        AUD_REC_EFS,
        AUD_REC_AMR,
        AUD_REC_ADPCM,
        AUD_REC_DSP_DATA_MIN, //=5  
        AUD_REC_DSP_DATA_1 = AUD_REC_DSP_DATA_MIN,//=5
        AUD_REC_DSP_DATA_2, //=6
        AUD_REC_DSP_DATA_3, //=7
        AUD_REC_DSP_DATA_4, //=8
        AUD_REC_DSP_DATA_5, //=9
        AUD_REC_DSP_DATA_6, //=10
        AUD_REC_DSP_DATA_7, //=11
        AUD_REC_DSP_DATA_8, //=12
        AUD_REC_DSP_DATA_9, //=13
        AUD_REC_DSP_DATA_10, //=14
        AUD_REC_DSP_DATA_11, //=15
        AUD_REC_DSP_DATA_12, //=16
        AUD_REC_DSP_DATA_13, //=17
        AUD_REC_DSP_DATA_14, //=18
        AUD_REC_DSP_DATA_15, //=19
        AUD_REC_DSP_DATA_MAX = AUD_REC_DSP_DATA_15,
        AUD_REC_DSP_LOG,    //20
        AUD_REC_FORMAT_MAX 
}AUD_REC_FORMAT_E; 


typedef enum 
{
        MR475 = 0,/*4.75kbps*/
        MR515, 
        MR59,
        MR67,
        MR74,
        MR795,
        MR102,
        MR122, 
        MRDTX,
        N_MODES     /* number of (SPC) modes */
}AMR_MODE; //amr encode mode

typedef enum
{
    REC_BY_DSPVB = 0,
    REC_BY_ARMVB,
    REC_BY_PERIPHERAL,
    AUD_REC_DEV_TYPE_MAX
}AUD_REC_DEV_TYPE_E;
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function plays generic tone.
//  Author:         Xueliang.Wang
//	Note:           This function does nothing if it is called when playing
//                  Ring.
/*****************************************************************************/
PUBLIC ERR_AUD_E AUD_PlayGenericTone(   // If succeed, return ERR_AUD_NONE,
                                        // else return error value
    AUD_GENERIC_TONE_ID_E tone_id,      // Generic tone enumeration value.
    uint32  duration    // Number of milliseconds of playing this tone.
    );

/*****************************************************************************/
//  Description:    This function stops playing generic tone.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC ERR_AUD_E AUD_StopGenericTone(void); // If succeed, return ERR_AUD_NONE,
                                            // else return error value
            
/*****************************************************************************/
//  Description:    This function is used to set voice codec enable/disable.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC ERR_AUD_E AUD_EnableVoiceCodec(  // If succeed, return ERR_AUD_NONE,
                                    	// else return error value
    BOOLEAN is_enable               	// SCI_TRUE: Enable,   SCI_FALSE: Disable
    );


/*****************************************************************************/
//  Description:    This function is used to set engineering audio parameters.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC ERR_AUD_E AUD_SetEngineeringParameter(  	// If succeed, return ERR_AUD_NONE,
                                            	// else return error value
    AUDIO_DEVICE_MODE_TYPE_E  parameter_type,
    AUDIO_TEST_DATA_T  parameter_data


    );
    
/*****************************************************************************/
//  Description:    This function is used to enable/disable volice loopback 
//                  test.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC ERR_AUD_E AUD_EnableVoiceLoopback(   // If succeed, return ERR_AUD_NONE,
                                            // else return error value
    BOOLEAN is_enable,      // 0: disable  1:enable
    uint16  delay_time      // millisecones
    );

/*****************************************************************************/
//  Description:    This function is used to get engineering audio parameters.
//  Author:         Sunsome.Ju
//	Note:           
/*****************************************************************************/
PUBLIC ERR_AUD_E AUD_GetEngineeringParameter(  	// If succeed, return ERR_AUD_NONE,
                                            	// else return error value
    AUDIO_DEVICE_MODE_TYPE_E  parameter_type,
    AUDIO_TEST_DATA_T *parameter_data
    );


/*****************************************************************************/
//  Description:    This function is used to set midi playing frequency.
//  Author:         
//	Note:           
/*****************************************************************************/
PUBLIC void AUDIO_SetMidiPlayFreq(uint32 midi_play_freq);         

/*****************************************************************************/
//  Description:    This function is used to set whether the audio data is output by stereo mode.
//  Author:         BenjaminWang
//	Note:           
/*****************************************************************************/
PUBLIC void AUDIO_SetDataStereo(     // return void
        BOOLEAN stereo_state      // SCI_TRUE - stereo on      SCI_FALSE - stereo off
        );

/*****************************************************************************/
//  Description:    This function is used to know whether the audio data is output by stereo mode.
//  Author:         BenjaminWang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN AUDIO_GetDataStereo(void);     // return SCI_TRUE - stereo on    SCI_FALSE - stereo off

PUBLIC void AUD_EnableAmplifier(BOOLEAN bEnable);


/*****************************************************************************/
//  Description:    This function is used to turn on MICBIAS voltage.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
void VB_TurnOnMICBIAS(void);

/*****************************************************************************/
//  Description:    This function is used to shut down MICBIAS voltage.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
void VB_ShutDownMICBIAS(void);

//record call back function
typedef void (*RECORD_CALLBACK)(uint16 data_length);

//play call back function
typedef BOOLEAN (*PLAY_CALLBACK)();			   			 

/*****************************************************************************/
//  Description:    This function is used to set voice codec enable/disable.
//  Author:         Jimmy.Jia
//	Note:           
/*****************************************************************************/
PUBLIC ERR_AUD_E AUD_EnableVoiceRecord(  // If succeed, return ERR_AUD_NONE,
                                    	// else return error value
    BOOLEAN is_enable,               	// SCI_TRUE: Enable,   SCI_FALSE: Disable
	AUD_REC_FORMAT_E recorder_format,
	RECORD_CALLBACK callback_fun    
    );

/*****************************************************************************/
//  Description:    This function config the amr parameters
//  Author:         yujun.ke
//	Note:           
/*****************************************************************************/

extern void AUD_CfgAmrParam( AMR_MODE amr_mode, uint8 channels, uint8 sample_rate );

 /*****************************************************************************/
//  Description:    This function transmit the recorded data to uplayer buffer
//  Author:         yujun.ke
//	Note:           
/*****************************************************************************/
extern void AUD_ReadVoiceData( uint16 *destination_buf_ptr,uint16 data_length );

 /*****************************************************************************/
//  Description:    This function start the play flow
//  Author:         yujun.ke
//	Note:           
/*****************************************************************************/
extern void AUD_StartPlay( AUD_REC_FORMAT_E voice_format,
		          		   PLAY_CALLBACK callback_fun,
				           BOOLEAN is_to_speaker,
				           BOOLEAN is_to_net
                          );

 /*****************************************************************************/
//  Description:    This function transmit data to downlayer for playing
//  Author:         yujun.ke
//	Note:           
/*****************************************************************************/
extern void AUD_WriteVoiceData (uint16 * source_buf_ptr,uint16 data_length);

 /*****************************************************************************/
//  Description:    This function stop the paly process
//  Author:         yujun.ke
//	Note:           
/*****************************************************************************/
extern void AUD_StopPlay( void );
 
/*****************************************************************************/
// Description: United operation for POP noise restrain.
// Author: Benjamin
// Note:           
/*****************************************************************************/
PUBLIC void AUD_DSPUnitedOperation(void);
/*****************************************************************************/
//  Description:    This function is used to set Audio Codec Output Data Type.      
/*****************************************************************************/
PUBLIC void AUD_SetOutputDataType(AUDIO_OUTPUT_DATA_TYPE_E eDataType);

/*****************************************************************************/
//  Description:    This function is used to get Audio Codec Output Data Type.      
/*****************************************************************************/
PUBLIC AUDIO_OUTPUT_DATA_TYPE_E AUD_GetOutputDataType(void);

/*****************************************************************************/
//  Description:    This function is used to config dsp trans samplerate param
/*****************************************************************************/
PUBLIC void AUD_Cfg_PcmTrans_Param(uint16 dest_samplerate);

/*****************************************************************************/
//  Description:    This function sets volume of background music.
//  Author:         Cherry.Liu
//	Note:           
/*****************************************************************************/
PUBLIC ERR_AUD_E AUD_SetBkMusicVolume (
    AUD_BKMUSIC_PARAM_TYPE_E  eParamType,
    uint16  uiUplinkLevel, 
    uint16  uiDownlinkLevel
);
/*****************************************************************************/
//  Description:    This function eanble/disable earphone when power off.
//  Author:        
//	Note:           
/*****************************************************************************/
PUBLIC void AUD_Enable_Earphone(BOOLEAN bEnable);
/*****************************************************************************/
//  Description:    This function is used to get dsp trans samplerate param
/*****************************************************************************/
PUBLIC uint16 AUD_Get_PcmTrans_Samplerate(void);

/*****************************************************************************/
//  Description:    This function is used to get record dev type.      
/*****************************************************************************/
PUBLIC AUD_REC_DEV_TYPE_E AUD_GetRecordDevType(HAUDIODEV hDevice);

/*****************************************************************************/
//  Description:    interface for mmi to set eq mode. 
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
PUBLIC BOOLEAN AUD_SetEqMode(
    int32 eq_mode
);

/*****************************************************************************/
//  Description:    interface to set digital gain in arm volume[]. high 7bits
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
PUBLIC void AUD_SetDgGain(
    int32 dac_gain
);

/*****************************************************************************/
//  Description: Audio interface to call sbc encoder for dsp codec.
//  Author: TH
//  Note: NONE.
//****************************************************************************/
PUBLIC void AUD_SBC_Encode_For_DSPCodec(
    uint16 source_samplerate,
    uint16 frame_address,
    uint16 frame_size,
    void * callback
);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }  
#endif

#endif  // _AUDIO_H


