/******************************************************************************
 ** File Name:    sc8800h_reg_bm.h                                            *
 ** Author:       Aiguo.Miao                                                  *
 ** DATE:         06/26/2007                                                  *
 ** Copyright:    2007 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 06/26/2007    Aiguo.Miao      Create.                                     *
 ******************************************************************************/
#ifndef _SC8800H_REG_BM_H_
    #define _SC8800H_REG_BM_H_
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
#define BM0_BASE                    0x20900004
#define BM1_BASE                    0x20900020

#define BM_EN                     	(BM0_BASE - 0x04)

#define BM0_ADDR_LOW               	(BM0_BASE + 0x00)
#define BM0_ADDR_HIGH               (BM0_BASE + 0x04)
#define BM0_WRITE_DATA         		(BM0_BASE + 0x08)
#define BM0_DATA_MASK               (BM0_BASE + 0x0c)
#define BM0_READ_DATA              	(BM0_BASE + 0x10)
#define BM0_SIZE                    (BM0_BASE + 0x14)
#define BM0_CTL                     (BM0_BASE + 0x18)

#define BM1_ADDR_LOW               	(BM0_BASE + 0x00)
#define BM1_ADDR_HIGH               (BM0_BASE + 0x04)
#define BM1_WRITE_DATA         		(BM0_BASE + 0x08)
#define BM1_DATA_MASK               (BM0_BASE + 0x0c)
#define BM1_READ_DATA              	(BM0_BASE + 0x10)
#define BM1_SIZE                    (BM0_BASE + 0x14)
#define BM1_CTL                     (BM0_BASE + 0x18)


//Monitor sel
#define BM_MON_SEL_RESERVED         0
#define BM_MON_SEL_ARMI             1
#define BM_MON_SEL_ARMD             2
#define BM_MON_SEL_MISC_CELL        3

//Monitor ctl
#define BM_CTL_WRITE_EQUAL          0
#define BM_CLT_WRITE_LESS           1
#define BM_CTL_WRITE_MORE           2
#define BM_CTL_READ_EQUAL           3

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

typedef union _bm_en_tag {
	struct _bm_en_map {
		volatile unsigned int reserved       	:30;//Reserved ;
		volatile unsigned int bm1				:1;	//bus monitor1 interrupt enable
		volatile unsigned int bm0				:1;	//bus monitor0 interrupt enable
	}mBits ;
	volatile unsigned int dwValue ;
}BM_EN_U;

typedef union _bm_addr_low_tag {
	struct _bm_addr_low_map {
		volatile unsigned int addr_high7		:7;	//the high 7 bits of the address in monitoring range (the monitoring range is no larger than 32M byte)
		volatile unsigned int addr_low25    	:25;//the low 25 bits of the low address of the monitoring range
	}mBits ;
	volatile unsigned int dwValue ;
}BM_ADDR_LOW_U;

typedef union _bm_addr_high_tag {
	struct _bm_addr_high_map {
		volatile unsigned int reserved		    :7;	//Reserved
		volatile unsigned int addr_low25    	:25;//the low 25 bits of the high address of the monitoring range
	}mBits ;
	volatile unsigned int dwValue ;
}BM_ADDR_HIGH_U;

typedef union _bm_size_tag {
	struct _bm_size_map {
		volatile unsigned int reserved		    :29;//Reserved
		volatile unsigned int size    	        :3; //the expect  hsize
	}mBits ;
	volatile unsigned int dwValue ;
}BM_SIZE_U;

typedef union _bm_ctl_tag {
	struct _bm_ctl_map {
		volatile unsigned int reserved		    :26;//Reserved
		volatile unsigned int ctl   		    :2; //ctl
		volatile unsigned int mon_sel    	    :4; //the expect  hsize
	}mBits ;
	volatile unsigned int dwValue ;
}BM_CTL_U;


typedef struct bm_dev_reg_tag
{
	volatile BM_ADDR_LOW_U 		addr_low;
	volatile BM_ADDR_HIGH_U     addr_high;
	volatile uint32             write_data;
	volatile uint32             data_mask;
	volatile uint32             read_data;
	volatile BM_SIZE_U          size;
	volatile BM_CTL_U           ctl;
}BM_DEV_REG_T;
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

