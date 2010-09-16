/******************************************************************************
 ** File Name:    sc8800h_reg_isp.h                                            *
 ** Author:                                                                   *
 ** DATE:         02/14/2006                                                  *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  This define names for each ISP register                     *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 02/14/2006                    Create.                                     *
 ******************************************************************************/


#ifndef _SC8800H_REG_ISP_H_
    #define _SCH800H_REG_ISP_H_
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
#define ISP_REG_BASE                    0x20200000UL

#define ISP_SHADOW_CNTL                 0x20200000UL
#define ISP_BUF_SW                      0x20200004UL
#define ISP_CONF                        0x20200008UL
#define ISP_SUBM_FRC_ON                 0x2020000CUL
#define ISP_SUBM_MODE                   0x20200010UL
#define ISP_BUF_CONF                    0x20200014UL
#define ISP_SELF_RATE                   0x20200018UL
#define ISP_INT_IND                     0x20200020UL
#define ISP_INT_MASK                    0x20200024UL
#define ISP_INT_CLR                     0x20200028UL
#define ISP_INT_RAW                     0x2020002CUL
#define GMC_TAB_TEST_MODE               0x20200040UL
#define GMC_TAB_TEST_ADDR               0x20200044UL
#define GMC_TAB_TEST_DOUT               0x20200048UL
#define SC1_TAB0_TEST_MODE              0x20200050UL
#define SC1_TAB0_TEST_ADDR              0x20200054UL
#define SC1_TAB0_TEST_DOUT              0x20200058UL
#define SC1_TAB1_TEST_MODE              0x20200060UL
#define SC1_TAB1_TEST_ADDR              0x20200064UL
#define SC1_TAB1_TEST_DOUT              0x20200068UL
#define CAP_CNTRL                       0x20200100UL
#define CAP_FRM_CNT                     0x20200104UL
#define CAP_START                       0x20200108UL
#define CAP_END                         0x2020010CUL
#define CAP_IMAGE_DECI                  0x20200110UL
#define CAP_TRANS                       0x20200114UL
#define PREF_CONF                       0x20200200UL
#define BLC_CONF                        0x20200210UL
#define AWB_START                       0x20200220UL
#define AWB_BLK_CONF                    0x20200224UL
#define AWBC_CONF                       0x20200230UL
#define NSR_WEIGHT                      0x20200240UL
#define NSR_LIMIT                       0x20200244UL
#define AF_SHIFT                        0x20200250UL
#define AF_START                        0x20200254UL
#define AF_END                          0x20200258UL
#define AF_RESULT						0x2020025CUL
#define LNC_CENTER                      0x20200260UL
#define LNC_PARA                        0x20200264UL
#define LNC_CENTER_SQR                  0x20200268UL
#define CFA_CONF                        0x20200270UL
#define CCE_MATRIX_YR                   0x20200280UL
#define CCE_MATRIX_YG                   0x20200284UL
#define CCE_MATRIX_YB                   0x20200288UL
#define CCE_MATRIX_UR                   0x2020028CUL
#define CCE_MATRIX_UG                   0x20200290UL
#define CCE_MATRIX_UB                   0x20200294UL
#define CCE_MATRIX_VR                   0x20200298UL
#define CCE_MATRIX_VG                   0x2020029CUL
#define CCE_MATRIX_VB                   0x202002A0UL
#define CCE_Y_SHIFT_VALUE               0x202002A4UL
#define CCE_U_SHIFT_VALUE               0x202002A8UL
#define CCE_V_SHIFT_VALUE               0x202002ACUL

#define ISP_SRC_SIZE                    0x20200300UL
#define ISP_SC_MODE						0x20200304UL
#define ISP_SC2_SIZE                    0x20200308UL
#define ISP_SC2_START                   0x2020030CUL
#define ISP_DISP_SIZE                   0x20200310UL
#define ISP_DISP_START                  0x20200314UL
#define ISP_ENC_SIZE                    0x20200318UL
#define ISP_ENC_START                   0x2020031CUL
#define SC1_CONF                        0x20200320UL
#define SC2_CONF                        0x20200324UL
#define ISP_DBLK_THR					0x20200334UL
#define ISP_SC_LINE_BLANK				0x20200338UL
#define ISP_PORTA_CONF                  0x20200340UL
#define ISP_PORTA_STATE                 0x20200344UL
#define ISP_PORTA_FRM                   0x20200348UL
#define ISP_PORTA_CURR_YBA              0x2020034CUL
#define ISP_PORTA_CURR_UBA              0x20200350UL
#define ISP_PORTA_CURR_VBA              0x20200354UL
#define ISP_PORTA_NEXT_YBA              0x20200358UL
#define ISP_PORTA_NEXT_UBA              0x2020035CUL
#define ISP_PORTA_NEXT_VBA              0x20200360UL
#define ISP_PORTA_LAST_END              0x20200364UL
#define ISP_PORTA_CURR_YADDR            0x20200368UL
#define ISP_PORTA_SKIP					0x2020036CUL
#define ISP_PORTA_TH					0x20200370UL
#define ISP_PORTB_CONF                  0x20200380UL
#define ISP_PORTB_STATE                 0x20200384UL
#define ISP_PORTB_FRM                   0x20200388UL
#define ISP_PORTB_CURR_BA               0x2020038CUL
#define ISP_PORTB_NEXT_BA               0x20200390UL
#define ISP_PORTB_LAST_END              0x20200394UL
#define ISP_PORTB_CURR_YADDR            0x20200398UL
#define ISP_PORTB_SKIP					0x2020039CUL
#define ISP_PORTB_TH					0x202003A0UL
#define ISP_PORTC_START                 0x202003C0UL
#define ISP_PORTC_CONF                  0x202003C4UL
#define ISP_PORTC_STATE                 0x202003C8UL
#define ISP_PORTC_CURR_YBA              0x202003CCUL
#define ISP_PORTC_CURR_UBA              0x202003D0UL
#define ISP_PORTC_CURR_VBA              0x202003D4UL
#define ISP_PORTC_SKIP                  0x202003D8UL
#define ISP_PORTC_CURR_YADDR            0x202003DCUL
#define VP_REVIEW_SIZE					0x202003E0UL
#define VP_REVIEW_SELF_RATE				0x202003E4UL

#define ISP_AWB_RESULT        			0x20200400UL
#define ISP_OBSERVED_ENABLE   			0x202003F8UL
#define ISP_GAMMA_TABLE       			0x20200B00UL
#define ISP_OBSERVED_STATE    			0x202003F8UL

#define ISP_IRQ_LINE_MASK     			0x00000FFFUL


#define ARM_GLOBAL_REG_GEN0				0x8b000008UL
#define ARM_GLOBAL_VPLL_MN				0x8b000068UL
#define AHB_GLOBAL_REG_CTL0				0x20900200UL
#define AHB_GLOBAL_REG_CTL1				0x20900204UL
#define AHB_GLOBAL_REG_SOFTRST			0x20900210UL
#define ISP_OBSERVED					0x202003F8UL
#define IRQ_ENABLE						0x20A00010UL
#define IRQ_DISABLE						0x20A00014UL	




typedef union isp_interrupt_tag {
	struct int_interrupt_map{
		volatile unsigned int port_c_suc		:1;	
		volatile unsigned int lcdc_sof			:1;	
		volatile unsigned int af_comp			:1;	
		volatile unsigned int cap_ovf			:1;	
		volatile unsigned int sc1_ovf			:1;	
		volatile unsigned int port_b_ovf		:1;	
		volatile unsigned int port_a_ovf		:1;	
		volatile unsigned int port_b_suc		:1;	
		volatile unsigned int port_a_suc		:1;	
		volatile unsigned int cap_eof			:1;
		volatile unsigned int cap_sof			:1;	
		volatile unsigned int sensor_sof  		:1;
	}mBits;
	volatile unsigned int dwValue ;
}ISP_INT_T;

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/

#endif

