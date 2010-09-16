/******************************************************************************
 ** File Name:    sc8800h_reg_dma.h                                            *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         11/13/2005                                                  *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 11/13/2005    Daniel.Ding     Create.                                     *
 ** 01/29/2007    Aiguo.Miao      Port to SC8800H                             *
 ** 05/02/2007    Tao.Zhou        Modify it for SC8800H.                      *
 ******************************************************************************/
#ifndef _SC8800H_REG_DMA_
    #define _SC8800H_REG_DMA_
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
#define DMA_REG_BASE                    0x20100000

//0X00
#define DMA_CFG            		        (DMA_REG_BASE + 0x0000)
#define DMA_CHx_EN         		        (DMA_REG_BASE + 0x0004)
#define DMA_LINKLIST_EN    		        (DMA_REG_BASE + 0x0008)
#define DMA_SOFTLINK_EN    		        (DMA_REG_BASE + 0x000C)
#define DMA_SOFTLIST_SIZE               (DMA_REG_BASE + 0x0010)
#define DMA_SOFTLIST_CMD                (DMA_REG_BASE + 0x0014)
#define DMA_SOFTLIST_STS                (DMA_REG_BASE + 0x0018)
#define DMA_SOFTLIST_BASEADDR           (DMA_REG_BASE + 0x001C)
//0X20
#define DMA_PRI_REG0       		        (DMA_REG_BASE + 0x0020)
#define DMA_PRI_REG1       		        (DMA_REG_BASE + 0x0024)
//0X30
#define DMA_INT_STS        		        (DMA_REG_BASE + 0x0030)
#define DMA_INT_RAW        		        (DMA_REG_BASE + 0x0034)
//0X40
#define DMA_LISTDONE_INT_EN        		(DMA_REG_BASE + 0x0040)
#define DMA_BURST_INT_EN            	(DMA_REG_BASE + 0x0044)
#define DMA_TRANSF_INT_EN        	    (DMA_REG_BASE + 0x0048)
//0X50
#define DMA_LISTDONE_INT_STS       		(DMA_REG_BASE + 0x0050)
#define DMA_BURST_INT_STS        		(DMA_REG_BASE + 0x0054)
#define DMA_TRANSF_INT_STS        		(DMA_REG_BASE + 0x0058)
//0X60
#define DMA_LISTDONE_INT_RAW       		(DMA_REG_BASE + 0x0060)
#define DMA_BURST_INT_RAW        		(DMA_REG_BASE + 0x0064)
#define DMA_TRANSF_INT_RAW        		(DMA_REG_BASE + 0x0068)
//0X70
#define DMA_LISTDONE_INT_CLR       		(DMA_REG_BASE + 0x0070)
#define DMA_BURST_INT_CLR        		(DMA_REG_BASE + 0x0074)
#define DMA_TRANSF_INT_CLR        		(DMA_REG_BASE + 0x0078)
//0X80
#define DMA_SOFT_REQ        		    (DMA_REG_BASE + 0x0080)
#define DMA_TRANS_STS        		    (DMA_REG_BASE + 0x0084)//for debug
#define DMA_REQ_PEND        		    (DMA_REG_BASE + 0x0088)//for debug
//0X90
#define DMA_WRAP_START        		    (DMA_REG_BASE + 0x0090)
#define DMA_WRAP_END        		    (DMA_REG_BASE + 0x0094)

#define DMA_CHN_UID_BASE                (DMA_REG_BASE + 0x0098)
#define DMA_CHN_UID0                    (DMA_REG_BASE + 0x0098)
#define DMA_CHN_UID1                    (DMA_REG_BASE + 0x009C)
#define DMA_CHN_UID2                    (DMA_REG_BASE + 0x00A0)
#define DMA_CHN_UID3                    (DMA_REG_BASE + 0x00A4)
#define DMA_CHN_UID4                    (DMA_REG_BASE + 0x00A8)
#define DMA_CHN_UID5                    (DMA_REG_BASE + 0x00AC)
#define DMA_CHN_UID6                    (DMA_REG_BASE + 0x00B0)
#define DMA_CHN_UID7                    (DMA_REG_BASE + 0x00B4)

//Chanel x dma contral regisers base address ;
#define DMA_CHx_CTL_BASE				(DMA_REG_BASE + 0x0400)

#define DMA_SOFT0_BASE  		        (DMA_REG_BASE + 0x0400)//UID:0x0
#define DMA_UART0_TX_BASE  		        (DMA_REG_BASE + 0x0420)//UID:0x1
#define DMA_UART0_RX_BASE    		    (DMA_REG_BASE + 0x0440)//UID:0x2
#define DMA_UART1_TX_BASE               (DMA_REG_BASE + 0x0460)//UID:0x3
#define DMA_UART1_RX_BASE               (DMA_REG_BASE + 0x0480)//UID:0x4
#define DMA_UART2_TX_BASE               (DMA_REG_BASE + 0x04a0)//UID:0x5
#define DMA_UART2_RX_BASE               (DMA_REG_BASE + 0x04c0)//UID:0x6
#define DMA_UART3_TX_BASE               (DMA_REG_BASE + 0x04e0)//UID:0x7
#define DMA_UART3_RX_BASE      	        (DMA_REG_BASE + 0x0500)//UID:0x8
#define DMA_ENCRP_RAW_BASE     	        (DMA_REG_BASE + 0x0520)//UID:0x9
#define DMA_ENCRP_CPT_BASE     	        (DMA_REG_BASE + 0x0540)//UID:0xA
#define DMA_VB_DA0_BASE       	        (DMA_REG_BASE + 0x0560)//UID:0xB
#define DMA_VB_DA1_BASE       	        (DMA_REG_BASE + 0x0580)//UID:0xC
#define DMA_VB_AD0_BASE       	        (DMA_REG_BASE + 0x05A0)//UID:0xD
#define DMA_VB_AD1_BASE       	        (DMA_REG_BASE + 0x05C0)//UID:0xE
#define DMA_SIM0_TX_BASE       	        (DMA_REG_BASE + 0x05E0)//UID:0xF
#define DMA_SIM0_RX_BASE       	        (DMA_REG_BASE + 0x0600)//UID:0x10
#define DMA_SIM1_TX_BASE       	        (DMA_REG_BASE + 0x0620)//UID:0x11
#define DMA_SIM1_RX_BASE       	        (DMA_REG_BASE + 0x0640)//UID:0x12
#define DMA_DRM_RAW_BASE	       	    (DMA_REG_BASE + 0x0660)//UID:0x13
#define DMA_DRM_CPT_BASE     	  	    (DMA_REG_BASE + 0x0680)//UID:0x14
#define DMA_ROT_BASE     	  	        (DMA_REG_BASE + 0x06A0)//UID:0x15
#define DMA_NLC_BASE       	            (DMA_REG_BASE + 0x06C0)//UID:0x16
#define DMA_USB_EP0_IN_BASE       	    (DMA_REG_BASE + 0x06E0)//UID:0x17
#define DMA_USB_EP0_OUT_BASE	       	(DMA_REG_BASE + 0x0660)//UID:0x18
#define DMA_USB_EP1_BASE     	  	    (DMA_REG_BASE + 0x0680)//UID:0x19
#define DMA_USB_EP2_BASE     	  	    (DMA_REG_BASE + 0x06A0)//UID:0x1A
#define DMA_USB_EP3_BASE        	    (DMA_REG_BASE + 0x06C0)//UID:0x1B
#define DMA_USB_EP4_BASE        	    (DMA_REG_BASE + 0x06E0)//UID:0x1C

#define DMA_CH_NUM						31
#define DMA_UID_NUM                     29

//Chanel x dma contral regisers address ;
#define DMA_CH_CFG0    		 	        (0x0000)
#define DMA_CH_CFG1    		    	    (0x0004)
#define DMA_CH_SRC_ADDR   		        (0x0008)
#define DMA_CH_DEST_ADDR  		        (0x000c)
#define DMA_CH_LLPTR      		        (0x0010)
#define DMA_CH_SDEP       		        (0x0014)
#define DMA_CH_SBP       		        (0x0018)
#define DMA_CH_DBP	       		        (0x001c)


//Channel x dma contral regisers address ;
#define DMA_CHx_BASE(x)					(DMA_CHx_CTL_BASE + 0x20 * x )
#define DMA_CHx_CFG0(x)    		        (DMA_CHx_CTL_BASE + 0x20 * x + 0x0000)
#define DMA_CHx_CFG1(x)    		        (DMA_CHx_CTL_BASE + 0x20 * x + 0x0004)
#define DMA_CHx_SRC_ADDR(x) 		    (DMA_CHx_CTL_BASE + 0x20 * x + 0x0008)
#define DMA_CHx_DEST_ADDR(x)  		    (DMA_CHx_CTL_BASE + 0x20 * x + 0x000c)
#define DMA_CHx_LLPTR(x)      	        (DMA_CHx_CTL_BASE + 0x20 * x + 0x0010)
#define DMA_CHx_SDEP(x)       	        (DMA_CHx_CTL_BASE + 0x20 * x + 0x0014)
#define DMA_CHx_SBP(x)       	        (DMA_CHx_CTL_BASE + 0x20 * x + 0x0018)
#define DMA_CHx_DBP(x)	       	        (DMA_CHx_CTL_BASE + 0x20 * x + 0x001c)


#define DMA_CTL_END	            		0x201FFFFF

#define DMA_SOFT0                       0
#define DMA_UART0_TX					1
#define DMA_UART0_RX					2
#define DMA_UART1_TX					3
#define DMA_UART1_RX					4
#define DMA_UART2_TX					5
#define DMA_UART2_RX					6
#define DMA_UART3_TX					7
#define DMA_UART3_RX					8
#define DMA_ENCRP_RAW                   9
#define DMA_ENCRP_CPT                   0xa
#define DMA_VB_DA0						0xb
#define DMA_VB_DA1						0xc
#define DMA_VB_AD1						0xd
#define DMA_VB_AD2                      0xe
#define DMA_SIM0_TX						0xf
#define DMA_SIM0_RX						0x10
#define DMA_SIM1_TX                     0x11
#define DMA_SIM1_RX                     0x12
#define DMA_DRM_RAW                     0x13
#define DMA_DRM_CPT                     0x14
#define DMA_ROT 						0x15
#define DMA_NLC							0x16
#define DMA_USB_EP0_IN					0x17
#define DMA_USB_EP0_OUT					0x18
#define DMA_USB_EP1						0x19
#define DMA_USB_EP2						0x1a
#define DMA_USB_EP3						0x1b
#define DMA_USB_EP4						0x1c


#define DMA_SOFT0_BIT                   BIT_0
#define DMA_UART0_TX_BIT				BIT_1
#define DMA_UART0_RX_BIT				BIT_2
#define DMA_UART1_TX_BIT				BIT_3
#define DMA_UART1_RX_BIT				BIT_4
#define DMA_UART2_TX_BIT				BIT_5
#define DMA_UART2_RX_BIT				BIT_6
#define DMA_UART3_TX_BIT				BIT_7
#define DMA_UART3_RX_BIT				BIT_8
#define DMA_ENCRP_RAW_BIT               BIT_9
#define DMA_ENCRP_CPT_BIT               BIT_10
#define DMA_VB_DA0_BIT					BIT_11
#define DMA_VB_DA1_BIT					BIT_12

#ifndef CHIP_VER_8800H5
#define DMA_VB_AD0_BIT					BIT_13
#define DMA_VB_AD1_BIT					BIT_14
#else
#define DMA_VB_AD						    BIT_13
#endif

#define DMA_SIM0_TX_BIT					BIT_15
#define DMA_SIM0_RX_BIT                 BIT_16
#define DMA_SIM1_TX_BIT					BIT_17
#define DMA_SIM2_RX_BIT					BIT_18
#define DMA_DRM_RAW_BIT                 BIT_19
#define DMA_DRM_CPT_BIT                 BIT_20
#define DMA_ROT_BIT                     BIT_21
#define DMA_NLC_BIT						BIT_22
#define DMA_USB_EP0_IN_BIT				BIT_23
#define DMA_USB_EP0_OUT_BIT				BIT_24
#define DMA_USB_EP1_BIT					BIT_25
#define DMA_USB_EP2_BIT					BIT_26
#define DMA_USB_EP3_BIT					BIT_27
#define DMA_USB_EP4_BIT					BIT_28

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
/*lint -save -e530 -e533 */
typedef union _DMACHx_U
{
	struct DMACHx_bit_map{
		volatile int 	reserved		:3; //reserved
		volatile int	ep4				:1; //channel 28
		volatile int 	ep3				:1; //channel 27
		volatile int 	ep2				:1; //channel 26
		volatile int 	ep1				:1; //channel 25
		volatile int 	ep0_out			:1; //channel 24
		volatile int 	ep0_in			:1; //channel 23
        volatile int    nlc             :1; //channel 22
        volatile int    rot             :1; //channel 21
        volatile int    drm_cpt         :1; //channel 20
        volatile int    drm_raw         :1; //channel 19
		volatile int 	sim1_rx			:1; //channel 18
		volatile int 	sim1_tx			:1; //channel 17
		volatile int 	sim0_rx			:1; //channel 16
		volatile int 	sim0_tx			:1; //channel 15
		volatile int    vb_ad1          :1; //channel 14
		volatile int 	vb_ad0			:1; //channel 13
		volatile int 	vb_da1			:1; //channel 12
		volatile int 	vb_da0			:1; //channel 11
        volatile int    encrp_cpt       :1; //channel 10
        volatile int    encrp_raw       :1; //channel 09
		volatile int 	uart3_rx		:1; //channel 08
		volatile int 	uart3_tx		:1; //channel 07
		volatile int 	uart2_rx		:1; //channel 06
		volatile int 	uart2_tx		:1; //channel 05
		volatile int 	uart1_rx		:1; //channel 04
		volatile int 	uart1_tx		:1; //channel 03
		volatile int 	uart0_rx		:1; //channel 02
		volatile int 	uart0_tx		:1; //channel 01
		volatile int    soft0           :1; //channel 00
	}mBits;
	volatile int dwValue ;
}DMACHx_U;

typedef union _DMACFG_U
{
	struct DMACFG_bit_map{
		volatile int 	sw_waite		:16;//bit 16-31
		volatile int    resvd           :7;
		volatile int 	pause_req		:1; //bit 8
		volatile int 	hw_waite		:8; //bit 0-7
	}mBits;
	volatile int dwValue ;
}DMACFG_U;

typedef union _DMAPRI0_U
{
	struct DMAPRI0_bit_map{
		volatile int 	ch15            :2;//channel 15
		volatile int 	ch14    		:2;
		volatile int 	ch13			:2;
		volatile int 	ch12			:2;
		volatile int 	ch11	    	:2;
		volatile int 	ch10			:2;
		volatile int 	ch9			    :2;
		volatile int 	ch8			    :2;
		volatile int 	ch7		        :2;
		volatile int 	ch6		        :2;
		volatile int 	ch5		        :2;
		volatile int 	ch4		        :2;
		volatile int 	ch3		        :2;
		volatile int 	ch2		        :2;
		volatile int 	ch1		        :2;
		volatile int 	ch0		        :2;//channel 0
	}mBits;
	volatile int dwValue;
}DMAPRI0_U;

typedef union _DMAPRI1_U
{
	struct DMAPRI1_bit_map{
		volatile int 	ch31		    :2; //channel 31
		volatile int 	ch30		    :2;
		volatile int 	ch29		    :2;
		volatile int 	ch28			:2;
		volatile int 	ch27			:2;
		volatile int 	ch26		    :2;
		volatile int 	ch25			:2;
		volatile int 	ch24			:2;
		volatile int 	ch23			:2;
		volatile int 	ch22		    :2;
		volatile int 	ch21		    :2;
		volatile int 	ch20		    :2;
		volatile int 	ch19			:2;
		volatile int 	ch18			:2;
		volatile int 	ch17		    :2;
		volatile int 	ch16			:2; //channel 16
	}mBits;
	volatile int dwValue;
}DMAPRI1_U;

typedef struct dma_ctl_tag
{
    //0x00
    volatile DMACFG_U 	cfg;
    volatile DMACHx_U 	ch_en;
    volatile DMACHx_U 	link_en;
    volatile DMACHx_U   softlink_en;
    //0x10
    volatile int 		softlist_size;
    volatile int 		softlist_cmd;
    volatile int 		softlist_sts;
    volatile int 		softlist_baseAddr;
	//0x20
	volatile DMAPRI0_U 	reg0;
    volatile DMAPRI1_U 	reg1;
    volatile int 		reserved_4;
    volatile int 		reserved_5;
	//0x30
	volatile DMACHx_U 	int_sts;
    volatile DMACHx_U 	int_raw;
    volatile int 		reserved_6;
    volatile int 		reserved_7;
	//0x40
    volatile DMACHx_U 	listdone_en;
    volatile DMACHx_U 	burst_en;
    volatile DMACHx_U 	transf_en;
    volatile int 		reserved_8;
	//0x50
    volatile DMACHx_U 	listdone_sts;
    volatile DMACHx_U 	burst_sts;
    volatile DMACHx_U 	transf_sts;
    volatile int 		reserved_9;
	//0x60
    volatile DMACHx_U 	listdone_raw;
    volatile DMACHx_U 	burst_raw;
    volatile DMACHx_U 	transf_raw;
    volatile int 		reserved_10;
	//0x70
    volatile DMACHx_U 	listdone_clr;
    volatile DMACHx_U 	burst_clr;
    volatile DMACHx_U 	transf_clr;
    volatile int 		reserved_11;
	//0x80
    volatile DMACHx_U 	soft_req;
    volatile DMACHx_U 	trans_sts;      //for debug
    volatile DMACHx_U 	req_pend;       //for debug
    volatile DMACHx_U 	reserved_12;
    //0x90
    volatile int 		*start;
    volatile int 		*end;
    //0x98
    volatile int        user_id[8];
}DMA_CTL_T;

typedef union _CHxCFG_U
{
	struct CHxCFG_bit_map{
		volatile int 	llend      		:1; //bit31
		volatile int 	endian_sel		:1; //bit30
		volatile int 	switch_mode		:2; //bit29-28
		volatile int 	src_data_width  :2; //bit27-26
		volatile int 	dst_data_width	:2; //bit25-24
		volatile int 	ReqModeSel 		:2; //bit23-22
		volatile int 	sa_wrap    		:1; //bit21
		volatile int 	da_wrap    		:1; //bit20
		volatile int 	reserved1  		:2; //bit19-18
	    volatile int 	auto_close 		:1; //bit17
		volatile int 	reserved   		:1; //bit16
		volatile int 	burst_size 		:16;
	}mBits;
	volatile int dwValue;
}CHxCFG_U;

//Chanel x dma contral regisers address ;
#define DMA_CH_SDEP       		        (0x0014)
#define DMA_CH_SBP       		        (0x0018)
#define DMA_CH_DBP	       		        (0x001c)

typedef union _CHxSDEP_U
{
    struct CHxSDEP_bit_map{
        volatile int    SrcElement_PostModi   :16;
        volatile int    DstElement_PostModi   :16;
   }mBits;
   volatile int dwValue;
}CHxSDEP_U;

typedef union _CHxSBP_U
{
    struct CHxSBP_bit_map{
        volatile int    rev1                :1;
        volatile int    src_busrt_mode      :3;
        volatile int    rev2                :2;
        volatile int    SrcBlock_PostModi   :26;
   }mBits;
   volatile int dwValue;
}CHxSBP_U;

typedef union _CHxDBP_U
{
    struct CHxDBP_bit_map{
        volatile int    rev1                :1;
        volatile int    src_busrt_mode      :3;
        volatile int    rev2                :2;
        volatile int    SrcBlock_PostModi   :26;
   }mBits;
   volatile int dwValue;
}CHxDBP_U;

typedef struct dma_chx_ctl_tag
{
    volatile CHxCFG_U 	cfg;
    volatile int   		size;
    volatile int*  		src;
    volatile int*  		des;
    volatile int*  		ll_ptr;
    volatile CHxSDEP_U  sdep;
    volatile CHxSBP_U   sbp;
    volatile CHxDBP_U   dbp;
}DMA_CHx_CTL_T;
/*lint -restore */

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 

