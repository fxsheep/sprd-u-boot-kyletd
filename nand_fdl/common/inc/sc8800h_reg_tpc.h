/******************************************************************************
 ** File Name:        sc8800h_reg_tpc.h                                                *
 ** Author:           Yi.Qiu                                                        *
 ** DATE:             09/16/2009                                                    *
 ** Copyright:        2009 Spreatrum, Incoporated. All Rights Reserved.                    *
 ** Description:                                                                *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                                    *
 **---------------------------------------------------------------------------* 
 ** DATE            NAME            DESCRIPTION                                    * 
 ** 09/16/2009    Yi.Qiu            Reconstruct TPC driver                            *
 ******************************************************************************/
#ifndef _SC8800H_REG_TPC_H_
#define _SC8800H_REG_TPC_H_
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
/* TPC control registers */
#define TPC_REG_BASE                        0x81003000
#define TPC_CTRL                        (TPC_CTRL + 0x0000)
#define TPC_SAMPLE_CTRL            (TPC_CTRL + 0x0004)
#define TPC_BOUNCE_CTRL            (TPC_CTRL + 0x0008)
#define TPC_FILTER_CTRL                (TPC_CTRL + 0x000C)
#define TPC_CALC_CTRL                (TPC_CTRL + 0x0010)
#define TPC_CALC_X_COEF            (TPC_CTRL + 0x0014)
#define TPC_CALC_Y_COEF            (TPC_CTRL + 0x0018)
#define TPC_INT_EN                    (TPC_CTRL + 0x001C)
#define TPC_INT_STAT                (TPC_CTRL + 0x0020)
#define TPC_INT_SRC                    (TPC_CTRL + 0x0024)
#define TPC_INT_CLR                    (TPC_CTRL + 0x0028)
#define TPC_INT_CTRL                (TPC_CTRL + 0x002C)
#define TPC_BUF_DATA                (TPC_CTRL + 0x0030)
#define TPC_Z_BUF_DATA                (TPC_CTRL + 0x0034)

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
/*TPC control register */
typedef union tpc_ctrl_tag 
{
    struct tpc_ctrl_map 
    {
        volatile unsigned int reserved_2        :16;//Reserved ;        
        volatile unsigned int pclk_prescale            :8;//The pre-scale coeff of ADC clock. The divided clock is interval count clock.0 and 1 as bypass.
        volatile unsigned int reserved_1         :2;//Reserved ;    
        volatile unsigned int tpc_stop             :1;//TPC stop control, write ¡®1¡¯ to stop TPC, it is cleared by HW ;        
        volatile unsigned int tpc_run            :1;//TPC run control, write ¡®1¡¯ to start TPC, it is cleared by HW ;
        volatile unsigned int tpc_mode             :1;//TPC work mode, it is same as ¡°tpc_mode¡± in ADC_CTRL        
        volatile unsigned int xy_input_switch    :1;//X/Y analog input switch control ;        
        volatile unsigned int pen_req_polarity      :1;//The polarity of external pen-down signal ;        
        volatile unsigned int tpc_en            :1;//TPC global enable ;
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_CTRL_U;

/* TPC sample control register */
typedef union tpc_sample_ctrl_tag 
{
    struct tpc_sample_ctrl_map 
    {
        volatile unsigned int data_interval        :12;//The interval between 2 output point. It should be >1 ;    
        volatile unsigned int point_interval    :8;//The interval between 2 output point. It should be >1 ;
        volatile unsigned int sample_interval    :8;//The interval between 2 sampling.It should be >1 ;
        volatile unsigned int sample_num        :4;//The number of sampling, active range is <=8 for filter limited ;        
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_SAMPLE_CTRL_U;

/* TPC debounce control register */
typedef union tpc_bounce_ctrl_tag 
{
    struct tpc_bounce_ctrl_map 
    {
        volatile unsigned int reserved               :23;//Reserved ;                
        volatile unsigned int debounce_num        :8;//De-bouncing check time ;
        volatile unsigned int debounce_en        :1;//De-bouncing enable ;
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_BOUNCE_CTRL_U;

/* TPC filter control register */
typedef union tpc_filter_ctrl_tag 
{
    struct tpc_filter_ctrl_map 
    {
        volatile unsigned int reserved               :26;//Reserved ;                
        volatile unsigned int filter_tap               :4;//Filter taps ;                
        volatile unsigned int filter_mode            :1;//Filter mode ;
        volatile unsigned int filter_en            :1;//Noise filter enable ;
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_FILTER_CTRL_U;

/* TPC cordinate calculation control register */
typedef union tpc_calc_ctrl_tag 
{
    struct tpc_calc_ctrl_map 
    {
        volatile unsigned int reserved               :29;//Reserved ;                
        volatile unsigned int dummy                :2;//dummy ;
        volatile unsigned int calc_en            :1;//Calculation enable ;
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_CALC_CTRL_U;

/* TPC cordinate calculation x coef register */
typedef union tpc_calc_x_coef_tag 
{
    struct tpc_x_coef_ctrl_map 
    {
        volatile unsigned int reserved_2            :5;//Reserved ;
        volatile signed int x_coef_b               :11;//x_coef_b is S11, it is in [-1024, 1023] ;                
        volatile unsigned int reserved_1            :6;//Reserved ;
        volatile unsigned int x_coef_a            :10;//X = x_coef_a*ADCX + x_coef_b, x_coef_a is S2.8, it is in [-2, 1.996];
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_X_COEF_CTRL_U;

/* TPC cordinate calculation y coef register */
typedef union tpc_calc_y_coef_tag 
{
    struct tpc_y_coef_ctrl_map 
    {
        volatile unsigned int reserved_2            :5;//Reserved ;
        volatile signed int y_coef_b               :11;//y_coef_b is S11, it is in [-1024, 1023] ;                
        volatile unsigned int reserved_1            :6;//Reserved ;
        volatile unsigned int y_coef_a            :10;//Y = x_coef_a*ADCY + y_coef_b, y_coef_a is S2.8, it is in [-2, 1.996];
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_Y_COEF_CTRL_U;

/* TPC interrupt enable register */
typedef union tpc_int_en_tag 
{
    struct tpc_int_en_map 
    {
        volatile unsigned int reserved               :29;//Reserved ;                
        volatile unsigned int done_irq_en            :1;//Specified sample done interrupt enable ;                            
        volatile unsigned int up_irq_en            :1;//pen-up interrupt enable ;
        volatile unsigned int down_irq_en        :1;//pen-down interrupt enable ;
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_INT_EN_U;

/* TPC interrupt status register */
typedef union tpc_int_status_tag 
{
    struct tpc_int_status_map 
    {
        volatile unsigned int reserved               :29;//Reserved ;                
        volatile unsigned int done_irq                :1;//Specified sample done interrupt status ;                            
        volatile unsigned int up_irq                :1;//pen-up interrupt status ;
        volatile unsigned int down_irq            :1;//pen-down interrupt status ;
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_INT_STATUS_U;

/* TPC interrupt source register */
typedef union tpc_int_src_tag 
{
    struct tpc_int_src_map 
    {
        volatile unsigned int reserved               :29;//Reserved ;                
        volatile unsigned int done_irq                :1;//Raw specified sample done interrupt ;                            
        volatile unsigned int up_irq                :1;//Raw pen-up interrupt ;
        volatile unsigned int down_irq            :1;//Raw pen-down interrupt ;
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_INT_SRC_U;

/* TPC interrupt clear register */
typedef union tpc_int_clr_tag 
{
    struct tpc_int_clr_map 
    {
        volatile unsigned int reserved               :29;//Reserved ;                
        volatile unsigned int done_irq_clr           :1;//Specified sample done interrupt clear ;                            
        volatile unsigned int up_irq_clr            :1;//pen-up interrupt clear ;
        volatile unsigned int down_irq_clr        :1;//pen-down interrupt clear ;
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_INT_CLR_U;

/* TPC data buffer control register bit */
#define TPC_BUF_FULL_FLAG                    BIT_4
#define TPC_BUF_EMPTY_FLAG                 BIT_5

/* TPC data buffer control register */
typedef union tpc_buf_ctrl_tag 
{
    struct tpc_buf_ctrl_map 
    {
        volatile unsigned int reserved              :26;//Reserved ;                
        volatile unsigned int buf_status            :2;//Internal buffer status ;
        volatile unsigned int buf_len                :4;//Specify sample done length, It should be >=1 ;
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_BUF_CTRL_U;

/* TPC data buffer register */
typedef union tpc_buf_data_tag 
{
    struct tpc_buf_data_map 
    {
        volatile unsigned int reserved_2              :6;//Reserved ;                
        volatile unsigned int data_y                :10;//Y ADC or position value ;        
        volatile unsigned int reserved_1              :6;//Reserved ;                
        volatile unsigned int data_x                :10;//X ADC or position value ;
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_BUF_DATA_U;

/* TPC Z data buffer register */
typedef union tpc_z_buf_data_tag 
{
    struct tpc_z_buf_data_map 
    {
        volatile unsigned int reserved                :22;//Z2 ADC value ;                
        volatile unsigned int z                 :10;//Z1 ADC value ;
    }mBits ;
    volatile unsigned int dwValue ;
}TPC_Z_BUF_DATA_U;

/* TPC module control registers */
typedef struct tpc_reg_tag
{
    volatile TPC_CTRL_U             ctrl;
    volatile TPC_SAMPLE_CTRL_U    sample_ctrl;
    volatile TPC_BOUNCE_CTRL_U    bounce_ctrl;
    volatile TPC_FILTER_CTRL_U        filter_ctrl;
    volatile TPC_CALC_CTRL_U        calc_ctrl;
    volatile TPC_X_COEF_CTRL_U    x_coef_ctrl;
    volatile TPC_Y_COEF_CTRL_U    y_coef_ctrl;
    volatile TPC_INT_EN_U            int_en;
    volatile TPC_INT_STATUS_U        int_status;
    volatile TPC_INT_SRC_U        int_src;
    volatile TPC_INT_CLR_U        int_clr;
    volatile TPC_BUF_CTRL_U        buf_ctrl;
    volatile TPC_BUF_DATA_U        buf_data;    
        volatile TPC_Z_BUF_DATA_U        z_buf_data;    
}TPC_REG_T;

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


