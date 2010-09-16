/******************************************************************************
 ** File Name:    sc8800h_reg_kpd.h                                           *
 ** Author:       Aiguo.Miao	                                              *
 ** DATE:         08/28/2007                                                  *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 08/28/2007    Aiguo.Miao      Create.                                     *
 ******************************************************************************/
#ifndef _SC8800H_REG_KPD_H_
    #define _SC8800H_REG_KPD_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
#define KPD_BASE                		0x87000000



#define KPD_STS                 		(KPD_BASE + 0x0000)
#define KPD_CTL                 		(KPD_BASE + 0x0004)
#define KPD_ICLR                		(KPD_BASE + 0x0008)
#define KPD_POLARITY            		(KPD_BASE + 0x000C)
#define KPD_CLK0                		(KPD_BASE + 0x0010)
#define KPD_CLK1                		(KPD_BASE + 0x0014)
#define KPD_INT_MSK             		(KPD_BASE + 0x0018)
#define KPD_PBINT_CTL           		(KPD_BASE + 0x0028)
#define KPD_PBINT_CNT           		(KPD_BASE + 0x002C)
#define KPD_PBINT_CNT_REG       		(KPD_BASE + 0x0030)

#define SYS_CNT0						(KPD_BASE + 0x001C)
#define SYS_CTL	                		(KPD_BASE + 0x0024)

//The corresponding bit of KPD_STS register.
#define KPDSTS_KPD          			(1 << 0)    //Keypad interrupt
#define KPDSTS_TOUT         			(1 << 1)    //Time out interrupt
#define KPDSTS_ROW_COUNT    			(7 << 2)    //Row counter
#define KPDSTS_COL_COUNT    			(7 << 5)    //Column counter

//The corresponding bit of KPD_CTL register.
#define KPDCTL_KPD_INT      			(0x1 << 0)  //Keypad interrupt enable
#define KPDCTL_TOUT_INT     			(0x1 << 1)  //Time out interrupt enable
#define KPDCTL_KPD          			(0x1 << 2)  //Keypad enable
#define KPDCTL_READ_ICLR    			(0x1 << 3)  //Write 1 to it, INT will be cleared
                                        //by read the keypad status reg.
#define KPDCTL_ROW          			(0xF << 4)  //Enable bit for rows(Row4~Row7).
#define KPDCTL_COL          			(0x3 << 8)  //Enable bit for cols(Col3~Col4).

// Const value of keypad.
#define KPD_ROW_MIN_NUM     			4           //When config keypad type, the value of
#define KPD_ROW_MAX_NUM     			8           //row should be one of 4/5/6/7/8.
#define KPD_COL_MIN_NUM     			3           //When config keypad type, the value of
#define KPD_COL_MAX_NUM     			5           //col should be one of 3/4/5.

//The corresponding bit of KPD_ICLR register.
// Write 1 to this bit will clear the keypad int status bit.
#define KPDICLR_KPD_INT     			(1 << 0)
#define KPDICLR_PB_INT	     			(3 << 0)
// Write 1 to this bit will clear the keypad tout int status bit.
#define KPDICLR_TOUT_INT    			(1 << 1)

//The corresponding bit of KPD_POLARITY register.
#define KPDPOLARITY_ROW     			(0x00FF)    // Internal row output xor with this
                            			            // value to generate row output.
#define KPDPOLARITY_COL     			(0xFF00)    // Column input xor with this value to
                                        // generate the internal column input.

//The corresponding bit of KPD_CLK0, KPD_CLK1 register.
#define KPDCLK0_CLK_DIV0    			0xFFFF      //Clock dividor [15:0]
#define KPDCLK1_CLK_DIV1    			0x003F      //Clock dividor [21:16]
#define KPDCLK1_TIME_CNT    			0xFFB0      //Time out counter value

//The corresponding bit of KPD_INT_MSK register.
#define KPDINTMSK_KPD       			0x003F      //Masked_kpd_int
#define KPDINTMSK_TOUT      			0xFFB0      //Masked_tout_int

// KEYPAD Interface Control.
typedef struct kpd_tag
{
    VOLATILE uint32    status;
    VOLATILE uint32    ctl;
    VOLATILE uint32    iclr;
    VOLATILE uint32    polarity;
    VOLATILE uint32    div0;
    VOLATILE uint32    div1;
    VOLATILE uint32    masked_int;
}kpd_s;
/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

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

