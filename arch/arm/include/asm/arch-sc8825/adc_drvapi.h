/******************************************************************************
 ** File Name:      adc_drvapi.h                                                  *
 ** Author:         hao.liu                                             *
 ** DATE:           06/12/2010                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic input and output operations   *
 **                 on hardware, it can be treated as a hardware abstract     *
 **                 layer interface.                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               * ** 06/12/2010     hao.liu    Create.                                   *
 ******************************************************************************/

#ifndef _ADC_DRVAPI_H_
#define _ADC_DRVAPI_H_

#define ADC_SCALE_3V       0 
#define ADC_SCALE_1V2   1

typedef enum
{
    ADIN_0 = 0,
    ADIN_1,
    ADIN_2,
    ADIN_3,
    ADIN_4,
    ADIN_5,
    ADIN_6,
    ADIN_7,
    ADIN_8,
    ADIN_9,
    ADIN_10,
    ADIN_11,
    ADIN_12,
    ADIN_13,
    ADIN_14,
    ADIN_15,
    ADC_MAX
} adc_channel;
typedef enum{false, true} bool;

#ifdef CONFIG_MACH_SP6810A
#define ADC_CHANNEL_TEMP 0
#else
#define ADC_CHANNEL_TEMP 1
#endif
#define ADC_CHANNEL_VBAT 5
#define ADC_CHANNEL_PROG 4
#define ADC_CHANNEL_VCHG 6

#ifdef __cplusplus
extern   "C"
{
#endif

void ADC_Init (void);
int32_t ADC_GetValue(adc_channel adcSource, bool scale);

#ifdef __cplusplus
}
#endif

#endif  // _ADC_DRVAPI_H_

