/******************************************************************************
 ** File Name:        sc6600l_reg_auxadc.h                                            *
 ** Author:           Yi.Qiu                                                        *
 ** DATE:             09/16/2009                                                    *
 ** Copyright:        2009 Spreatrum, Incoporated. All Rights Reserved.                    *
 ** Description:                                                                *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                                    *
 **---------------------------------------------------------------------------* 
 ** DATE            NAME            DESCRIPTION                                    * 
 ** 09/16/2009    Yi.Qiu            Reconstruct ADC driver                            *
 ******************************************************************************/
#ifndef _SC8800H_REG_AUXADC_
#define _SC8800H_REG_AUXADC_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
/* ADC Ctronl Registers */
#define ADC_REG_BASE                        0x82000000
#define ADC_CTRL                    (ADC_REG_BASE + 0x0000)
#define ADC_CS                        (ADC_REG_BASE + 0x0004)
#define ADC_TPC_CH_CTRL            (ADC_REG_BASE + 0x0008)
#define ADC_DAT                        (ADC_REG_BASE + 0x00C)
#define ADC_INT_EN                    (ADC_REG_BASE + 0x0010)
#define ADC_INT_CLR                    (ADC_REG_BASE + 0x0014)
#define ADC_INT_STAT                (ADC_REG_BASE + 0x0018)
#define ADC_INT_SRC                    (ADC_REG_BASE + 0x001C)

/* ADC channel define */
#define ADIN_0                        0
#define ADIN_1                        1
#define ADIN_2                        2
#define ADIN_3                        3
#define ADIN_4                        4
#define ADIN_VPROG                    5
#define ADIN_VBAT                    6
#define ADIN_VCHG                    7
#define ADC_MAX                    8

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
/* ADC control register */
typedef union adc_ctrl_tag 
{
    struct adc_ctrl_map 
    {
        volatile unsigned int reserved             :27;//Reserved ;        
        volatile unsigned int adc_status          :1;//ADC sampling status ;
        volatile unsigned int hw_int_en             :1;//TPC channel irq enable ;
        volatile unsigned int tpc_ch_on             :1;//TPC channel on/off ;
        volatile unsigned int sw_ch_on            :1;//SW channel on/off ;
        volatile unsigned int adc_en                :1;//ADC global enable ;
    }mBits ;
    volatile unsigned int dwValue ;
}ADC_CTRL_U;

/* ADC cs register */
/*
ADC channel select
each bit for one channel, ¡®1¡¯ for active
[0]-general channel0;
[1]-general channel1;
[2]-TPC X channel;
[3]-TPC Y channel;
[4]-PROG2ADC channel;
[5]-VBAT channel;
Only one channel can be selected at one time.
*/
typedef union adc_cs_tag 
{
    struct adc_cs_map 
    {
        volatile unsigned int reserved             :27;//Reserved ;            
        volatile unsigned int adc_scale             :1;//ADC scale setting for current ADC channel,0-1.2V;1-3.0V£»
        volatile unsigned int adc_cs                 :4;
    }mBits ;
    volatile unsigned int dwValue ;
}ADC_CS_U;

typedef union adc_tpc_ch_ctrl_tag 
{
    struct adc_tpc_ch_ctrl_map 
    {
        volatile unsigned int reserved             :24;//Reserved ;        
        volatile unsigned int tpc_y_ch            :4;//TPC Y channel ID, can't be same as X channel  ;
        volatile unsigned int tpc_x_ch            :4;//TPC X channel ID, can't be same as Y channel  ;
    }mBits ;
    volatile unsigned int dwValue ;
}ADC_TPC_CH_CTRL_U;

/* ADC data register */
typedef union adc_data_tag 
{
    struct adc_data_map 
    {
        volatile unsigned int reserved               :22;//Reserved ;                
        volatile unsigned int adc_data               :10;//ADC conversion output ;        
    }mBits ;
    volatile unsigned int dwValue ;
}ADC_DATA_U;

/* ADC interrupt enable register */
typedef union adc_int_en_tag 
{
    struct adc_int_en_map 
    {
        volatile unsigned int reserved               :31;//Reserved ;                
        volatile unsigned int done_irq_en            :1;//ADC interrupt enable ;
    }mBits ;
    volatile unsigned int dwValue ;
}ADC_INT_EN_U;

/* ADC interrupt status register */
typedef union adc_int_status_tag 
{
    struct adc_int_status_map 
    {
        volatile unsigned int reserved               :31;//Reserved ;                
        volatile unsigned int adc_irq_status          :1;//ADC interrupt status ;
    }mBits ;
    volatile unsigned int dwValue ;
}ADC_INT_STATUS_U;

/* ADC interrupt source register */
typedef union adc_int_src_tag 
{
    struct adc_int_src_map 
    {
        volatile unsigned int reserved               :31;//Reserved ;                
        volatile unsigned int adc_irq_src                :1;//ADC interrupt source ;                            
    }mBits ;
    volatile unsigned int dwValue ;
}ADC_INT_SRC_U;

/* ADC interrupt clear register */
typedef union adc_int_clr_tag 
{
    struct adc_int_clr_map 
    {
        volatile unsigned int reserved               :31;//Reserved ;                
        volatile unsigned int adc_irq_clr           :1;//ADC interrupt clear ;                            
    }mBits ;
    volatile unsigned int dwValue ;
}ADC_INT_CLR_U;

/* ADC module control registers */
typedef struct adc_reg_tag
{
    volatile ADC_CTRL_U             ctrl;
    volatile ADC_CS_U                cs;
    volatile ADC_TPC_CH_CTRL_U    ch_ctrl;
    volatile ADC_DATA_U            data;
    volatile ADC_INT_EN_U            int_en;
    volatile ADC_INT_CLR_U        int_clr;    
    volatile ADC_INT_STATUS_U        int_stat;
    volatile ADC_INT_SRC_U        int_src;    
}ADC_REG_T;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 


