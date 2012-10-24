/******************************************************************************
 ** File Name:      SPI_Test.h                                                 *
 ** Author:         Zhonghe.Huang                                            *
 ** DATE:           18/06/2010                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define structure and varialbes for SPI_Test.c     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 18/06/2010     Zhonghe.Huang    Create.                                   *
 ******************************************************************************/

#ifndef _SPI_TEST_H
#define _SPI_TEST_H

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
	 **                         Macro Definition                                  *
	 **---------------------------------------------------------------------------*/
#define SPI0_ID        0
#define SPI1_ID        1
#define SPI_USED_ID    SPI0_ID   

#define CHN_SPI_INT			9
#define DMA_SPI_TX			0x13

#define SPI_TX_FIFO_DEPTH 		16
#define SPI_RX_FIFO_DEPTH 		16
#define SPI_INT_DIS_ALL		0
#define SPI_RX_FULL_INT_EN		BIT_6
#define SPI_RX_FULL_INT_STS 	BIT_6
#define SPI_RX_FULL_INT_CLR 	BIT_0
#define SPI_RX_FIFO_REAL_EMPTY	BIT_5
#define SPI_S8_MODE_EN			BIT_7
#define SPI_CD_SEL				0x2	//cs1 is sel as cd signal

	/*
	   Define the SPI interface mode for LCM
	 */
#define  SPIMODE_DISABLE           0
#define  SPIMODE_3WIRE_9BIT_SDA    1  // 3 wire 9 bit, cd bit, SDI/SDO share  one IO 
#define  SPIMODE_3WIRE_9BIT_SDIO   2  // 3 wire 9 bit, cd bit, SDI, SDO
#define  SPIMODE_4WIRE_8BIT_SDA    3  // 4 wire 8 bit, cd pin, SDI/SDO share one IO
#define  SPIMODE_4WIRE_8BIT_SDIO   4  // 4 wire 8 bit, cd pin, SDI, SDO

	/*
	   Define the clk src for SPI mode
	 */
#define SPICLK_SEL_78M    0
#define SPICLK_SEL_26M    1
#define SPICLK_SEL_104M   2
#define SPICLK_SEL_48M    3

	/*
	   SPI CS sel in master mode
	 */
#define SPI_SEL_CS0 0x0E  //2'B1110
#define SPI_SEL_CS1 0x0D  //2'B1101
#define SPI_SEL_CS2 0x0B  //2'B1011
#define SPI_SEL_CS3 0x07  //2'B0111

	/**---------------------------------------------------------------------------*
	 **                         Structure Definition                              *
	 **---------------------------------------------------------------------------*/

	typedef enum
	{
		CASE_INT_TEST = 0,
		CASE_DMA_TEST,
		CASE_LCD_CFG_TEST
	}TEST_CASE_ID;

	typedef enum
	{
		TX_POS_EDGE = 0,
		TX_NEG_EDGE
	}TX_EDGE;

	typedef enum
	{
		RX_POS_EDGE = 0,
		RX_NEG_EDGE
	}RX_EDGE;

	typedef enum
	{
		TX_RX_MSB = 0,
		TX_RX_LSB
	}MSB_LSB_SEL;

	typedef enum
	{
		IDLE_MODE = 0,
		RX_MODE,
		TX_MODE,
		RX_TX_MODE
	}TRANCIEVE_MODE;

	typedef enum
	{
		NO_SWITCH    = 0,
		BYTE_SWITCH  = 1,
		HWORD_SWITCH = 2
	}SWT_MODE;

	typedef enum
	{
		MASTER_MODE = 0, 
		SLAVE_MODE = 1    
	}SPI_OPERATE_MODE_E;

	typedef enum
	{
		DMA_DISABLE = 0,
		DMA_ENABLE
	}DMA_EN;

	typedef enum
	{
		CS_LOW = 0,
		CS_HIGH
	}CS_SIGNAL;

	typedef struct _init_param
	{
		TX_EDGE tx_edge;
		RX_EDGE rx_edge;
		MSB_LSB_SEL msb_lsb_sel;
		TRANCIEVE_MODE tx_rx_mode;
		SWT_MODE switch_mode;
		SPI_OPERATE_MODE_E op_mode;
		uint32 DMAsrcSize;
		uint32 DMAdesSize;
		uint32 clk_div;
		uint8 data_width;
		uint8 tx_empty_watermark;
		uint8 tx_full_watermark;
		uint8 rx_empty_watermark;
		uint8 rx_full_watermark;
	}SPI_INIT_PARM,*SPI_INIT_PARM_P;


	/**---------------------------------------------------------------------------*
	 **                         Data Protocol                                      *
	 **---------------------------------------------------------------------------*/
	// SPI control register filed definitions  
	typedef struct
	{
		VOLATILE uint32 data;				// Transmit word or Receive word
		VOLATILE uint32 clkd;				// clock dividor register
		VOLATILE uint32 ctl0;				// control register
		VOLATILE uint32 ctl1;				// Receive Data full threshold/Receive Data full threshold
		VOLATILE uint32 ctl2;				// 2-wire mode reigster
		VOLATILE uint32 ctl3;				// transmit data interval
		VOLATILE uint32 ctl4;				// transmit data interval
		VOLATILE uint32 ctl5;				// transmit data interval
		VOLATILE uint32 ien;				// interrutp enable register
		VOLATILE uint32 iclr;				// interrupt clear register
		VOLATILE uint32 iraw;				// interrupt clear register
		VOLATILE uint32 ists;				// interrupt clear register
		VOLATILE uint32 sts1;				// fifo cnt register, bit[5:0] for RX and [13:8] for TX
		VOLATILE uint32 sts2;				// masked interrupt status register
		VOLATILE uint32 dsp_wait;   		// Used for DSP control
		VOLATILE uint32 sts3;				// tx_empty_threshold and tx_full_threshold
		VOLATILE uint32 ctl6;
		VOLATILE uint32 sts4;
		VOLATILE uint32 fifo_rst;
		VOLATILE uint32 ctl7;               // SPI_RX_HLD_EN : SPI_TX_HLD_EN : SPI_MODE
		VOLATILE uint32 sts5;               // CSN_IN_ERR_SYNC2	
		VOLATILE uint32 ctl8;               // SPI_CD_BIT : SPI_TX_DUMY_LEN : SPI_TX_DATA_LEN_H
		VOLATILE uint32 ctl9;               // SPI_TX_DATA_LEN_L			
		VOLATILE uint32 ctl10;              // SPI_RX_DATA_LEN_H : SPI_RX_DUMY_LEN
		VOLATILE uint32 ctl11;              // SPI_RX_DATA_LEN_L	
		VOLATILE uint32 ctl12;              // SW_TX_REQ : SW_RX_REQ	
	} SPI_CTL_REG_T;

	/**---------------------------------------------------------------------------*
	 **                         Constant Variable                                  *
	 **---------------------------------------------------------------------------*/


	// ------------------------------------------------------------------------- //
	//                          Function Propertype                                
	// ------------------------------------------------------------------------- //


	PUBLIC void SPI_Enable( uint32 spi_id, BOOLEAN is_en);
	PUBLIC void SPI_Reset( uint32 spi_id, uint32 ms);
	PUBLIC void SPI_ClkSetting(uint32 spi_id, uint32 clk_src, uint32 clk_div);

	PUBLIC void SPI_SetCsLow( uint32 spi_sel_csx , BOOLEAN is_low);
	PUBLIC void SPI_SetCd( uint32 cd);
	PUBLIC void SPI_SetSpiMode(uint32 spi_mode);
	PUBLIC void SPI_SetDatawidth(uint32 datawidth);
	PUBLIC BOOLEAN SPI_EnableDMA(uint32 spi_index,BOOLEAN is_en);

	PUBLIC void SPI_SetTxLen(uint32 data_len, uint32 dummy_bitlen);
	PUBLIC void SPI_SetRxLen(uint32 data_len, uint32 dummy_bitlen);
	PUBLIC void SPI_TxReq( void );
	PUBLIC void SPI_RxReq( void );
	PUBLIC void SPI_WaitTxFinish();

	PUBLIC void SPI_Init(SPI_INIT_PARM *spi_parm);
	PUBLIC void SPI_WriteData(uint32 data, uint32 data_len, uint32 dummy_bitlen);
	PUBLIC uint32 SPI_ReadData( uint32 data_len, uint32 dummy_bitlen );
#ifdef __cplusplus
}
#endif

#endif /*_SPI_TEST_H*/
