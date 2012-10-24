#include <asm/arch/sci_types.h>
//#include <asm/arch/sys_init.h>
#include <asm/arch/os_api.h>
#include <asm/arch/arm_reg.h>
#include <asm/arch/sc8810_reg_base.h>
#include <asm/arch/sc8810_reg_ahb.h>
#include <asm/arch/sc8810_reg_global.h>

#include <asm/arch/sc8825_spi.h>

#define SPI_USED_BASE SPI_BASE


/**---------------------------------------------------------------------------*
 **                         Globle Variable                                  *
 **---------------------------------------------------------------------------*/

//endian switch mode
typedef enum
{
	ENDIAN_SWITCH_NONE    = 0x00,    //  2'b00: 0xABCD => 0xABCD
	ENDIAN_SWITCH_ALL     =0x01,     //  2'b01: 0xABCD => 0xDCBA
	ENDIAN_SWITCH_MODE0   =0x02 ,    //  2'b01: 0xABCD => 0xBADC
	ENDIAN_SWITCH_MODE1   = 0x03 ,   //  2'b01: 0xABCD => 0xCDAB
	ENDIAN_SWITCH_MAX
} DMA_ENDIANSWITCH_E ;  //should be added to dma_hal_new.h
//data width
typedef enum DMA_DATAWIDTH
{
	DMA_DATAWIDTH_BYTE = 0,
	DMA_DATAWIDTH_HALFWORD,
	DMA_DATAWIDTH_WORD,
	DMA_DATAWIDTH_MAX
}  DMA_DATAWIDTH_E;
//request mode
typedef enum DMA_CHN_REQMODE
{
	DMA_CHN_REQMODE_NORMAL = 0,
	DMA_CHN_REQMODE_TRASACTION,
	DMA_CHN_REQMODE_LIST,
	DMA_CHN_REQMODE_INFINITE,
	DMA_CHN_REQMODE_MAX
} DMA_CHN_REQMODE_E;


typedef enum LCM_DMA_RETURN_E
{
	LCM_ERR_ID    = 0,
	LCM_ERR_BUSWIDTH =1,   //only support:8-bit,16-bit 80_sys_bus
	LCM_ERR_SRCADDR =2,    //not HalfWORD_align
	LCM_ERR_ENDIAN
} LCM_DMA_RETURN_E;       //SHOULD define in lcd.h

/**---------------------------------------------------------------------------*
 **                         Function Define                                    *
 **---------------------------------------------------------------------------*/

PUBLIC void SPI_Enable( uint32 spi_id, BOOLEAN is_en)
{
	if(is_en)
	{
		if (spi_id == 0) 
			*(volatile uint32 *)GR_GEN0 |= ( 1 << GEN0_SPI0_EN);
		else 
			*(volatile uint32 *)GR_GEN0 |= ( 1 << GEN0_SPI1_EN);			
	}
	else
	{

	}

}

PUBLIC void SPI_Reset( uint32 spi_id, uint32 ms)
{
	uint32 i = 0;

	if(spi_id == 0)
	{
		*(volatile uint32 *)GR_SOFT_RST |= (1 << 14);
		for (i=0; i<0x100; i++);
		*(volatile uint32 *)GR_SOFT_RST &=~(1 << 14);        
	}
	else
	{
		*(volatile uint32 *)GR_SOFT_RST |= (1 << 15);
		for (i=0; i<0x100; i++);
		*(volatile uint32 *)GR_SOFT_RST |=~(1 << 15);
	}

}


LOCAL void SPI_PinConfig(void)
{
#ifndef FPGA_TEST
	*(volatile uint32 *)(0x8C000100) = 0x18A; 
	*(volatile uint32 *)(0x8C000104) = 0x107;
	*(volatile uint32 *)(0x8C000108) = 0x109;
	*(volatile uint32 *)(0x8C00010C) = 0x109;
	*(volatile uint32 *)(0x8C000110) = 0x109;
#endif
}


// The dividend is clk_spiX_div[1:0] + 1
PUBLIC void SPI_ClkSetting(uint32 spi_id, uint32 clk_src, uint32 clk_div)
{
	//clk_spi0_sel: [3:2]---->2'b:00-78M 01-26M,01-104M,11-48M,
	//clk_spi0_div: [5:4]---->div,  clk/(div+1)

	if(spi_id == 0)
	{
		*(volatile uint32 *) GR_CLK_DLY |= (clk_src&3)<<26;
		*(volatile uint32 *) GR_GEN2   |= (clk_div&7)<<21;
	} else {
		*(volatile uint32 *) GR_CLK_DLY |= (clk_src&3)<<30;
		*(volatile uint32 *) GR_GEN2   |= (clk_div&7)<<11;    
	}
}


#define SPI_SEL_CS_SHIFT 8
#define SPI_SEL_CS_MASK (0x0F<<SPI_SEL_CS_SHIFT)
PUBLIC void SPI_SetCsLow( uint32 spi_sel_csx , BOOLEAN is_low)
{
	volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T*)(SPI_USED_BASE);
	uint32 temp;

	if(is_low)     {
		//spi_ctl0[11:8]:cs3<->cs0 chip select, 0-selected;1-none
		spi_ctr_ptr->ctl0 &= ~(SPI_SEL_CS_MASK); 
		spi_ctr_ptr->ctl0 &= ~((1<<spi_sel_csx)<<SPI_SEL_CS_SHIFT); 
	}
	else
	{
		//spi_ctl0[11:8]:cs3<->cs0 chip select, 0-selected;1-none
		spi_ctr_ptr->ctl0 |= ((1<<spi_sel_csx)<<SPI_SEL_CS_SHIFT); 
	}
}

#define SPI_CD_MASK  BIT_15
PUBLIC void SPI_SetCd( uint32 cd)
{
	volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T*)(SPI_USED_BASE);

	//0-command;1-data
	if(cd == 0)
		spi_ctr_ptr->ctl8 &= ~(SPI_CD_MASK);  
	else
		spi_ctr_ptr->ctl8 |= (SPI_CD_MASK);  
}

// USE spi interface to write cmd/data to the lcm
// pay attention to the data_format
typedef enum data_width
{
	DATA_WIDTH_7bit =7,
	DATA_WIDTH_8bit =8,
	DATA_WIDTH_9bit =9,
	DATA_WIDTH_10bit=10,
	DATA_WIDTH_11bit=11,
	DATA_WIDTH_12bit=12,
}  SPI_DATA_WIDTH;

// Set spi work mode for LCM with spi interface
#define SPI_MODE_SHIFT    3 //[5:3]
#define SPI_MODE_MASK     (0x07<<SPI_MODE_SHIFT)
PUBLIC void SPI_SetSpiMode(uint32 spi_mode)
{
	volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);
	uint32 temp = spi_ctr_ptr->ctl7;

	temp &= ~SPI_MODE_MASK;
	temp |= (spi_mode<<SPI_MODE_SHIFT);

	//SCI_TraceLow("SPI_SetSpiMode: temp=%d\r\n",temp); 

	spi_ctr_ptr->ctl7 = temp;
	//SCI_TraceLow("SPI_SetSpiMode: spi_ctr_ptr->ctl7=%d\r\n",spi_ctr_ptr->ctl7); 
}

// Transmit data bit number:spi_ctl0[6:2] 
PUBLIC void  SPI_SetDatawidth(uint32 datawidth)
{
	volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);
	uint32 temp = spi_ctr_ptr->ctl0;

	if( 32 == datawidth )
	{
		spi_ctr_ptr->ctl0 &= ~0x7C;  //  [6:2]
		return;
	}

	temp &= ~0x0000007C;  //mask
	temp |= (datawidth<<2);

	spi_ctr_ptr->ctl0 = temp;
}

#define TX_MAX_LEN_MASK     0xFFFFF
#define TX_DUMY_LEN_MASK    0x3F    //[09:04]
#define TX_DATA_LEN_H_MASK  0x0F    //[03:00]
#define TX_DATA_LEN_L_MASK  0xFFFF  //[15:00]
/*****************************************************************************/
//  Description:  Set rxt data length with dummy_len
//  Author     :  lichd
//    Note       :  the unit is identical with datawidth you set
/*****************************************************************************/ 
PUBLIC void SPI_SetTxLen(uint32 data_len, uint32 dummy_bitlen)
{
	volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);
	uint32 ctl8 = spi_ctr_ptr->ctl8;
	uint32 ctl9 = spi_ctr_ptr->ctl9;

	data_len &= TX_MAX_LEN_MASK;

	ctl8 &= ~((TX_DUMY_LEN_MASK<<4) | TX_DATA_LEN_H_MASK);
	ctl9 &= ~( TX_DATA_LEN_L_MASK );

	// set dummy_bitlen in bit[9:4] and data_len[19:16] in bit[3:0]
	spi_ctr_ptr->ctl8 = (ctl8 | (dummy_bitlen<<4) | (data_len>>16));
	// set data_len[15:00]
	spi_ctr_ptr->ctl9 = (ctl9 | (data_len&0xFFFF));
}


#define RX_MAX_LEN_MASK     0xFFFFFF
#define RX_DUMY_LEN_MASK    0x3F    //[09:04]
#define RX_DATA_LEN_H_MASK  0x0F    //[03:00]
#define RX_DATA_LEN_L_MASK  0xFFFF  //[15:00]
/*****************************************************************************/
//  Description:  Set rxt data length with dummy_len
//  Author     :  lichd
//    Note       :  the unit is identical with datawidth you set
/*****************************************************************************/ 
PUBLIC void SPI_SetRxLen(uint32 data_len, uint32 dummy_bitlen)
{
	volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);
	uint32 ctl10 = spi_ctr_ptr->ctl10;
	uint32 ctl11 = spi_ctr_ptr->ctl11;

	data_len &= RX_MAX_LEN_MASK;

	ctl10 &= ~((RX_DUMY_LEN_MASK<<4) | RX_DATA_LEN_H_MASK);
	ctl11 &= ~( RX_DATA_LEN_L_MASK );

	// set dummy_bitlen in bit[9:4] and data_len[19:16] in bit[3:0]
	spi_ctr_ptr->ctl10 = (ctl10 | (dummy_bitlen<<4) | (data_len>>16));
	// set data_len[15:00]
	spi_ctr_ptr->ctl11 = (ctl11 | (data_len&0xFFFF));
}

// Request txt trans before send data
#define SW_RX_REQ_MASK BIT_0
#define SW_TX_REQ_MASK BIT_1
PUBLIC void SPI_TxReq( void )
{
	volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);

	spi_ctr_ptr->ctl12 |= SW_TX_REQ_MASK;
}

PUBLIC void SPI_RxReq( void )
{
	volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);

	spi_ctr_ptr->ctl12 |= SW_RX_REQ_MASK;
}

/*****************************************************************************/
//  Description:      To enable or disable DMA depending on parameter(is_enable)
//  Author:         @Vine.Yuan 2010.5.10
//    Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN SPI_EnableDMA(uint32 spi_index,BOOLEAN is_enable)
{
	volatile SPI_CTL_REG_T* spi_ctl;

	spi_ctl = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE+0x3000*spi_index); 

	if (is_enable)
	{
		spi_ctl->ctl2 |= BIT_6;
	}
	else
	{
		spi_ctl->ctl2 &= ~BIT_6;
	}

	//spi_ctl->ctl7 |= (BIT_7 | BIT_8);
	spi_ctl->ctl7 |= (BIT_7);

	return SCI_TRUE;
}

#define SPI_DMA_TIME_OUT        0x80000
#define BURST_SIZE              16
#define BURST_SIZE_MARK         0xF
#define LENGTH_4_DIVIDE         4

PUBLIC void SPI_Init(SPI_INIT_PARM *spi_parm)
{
	volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);
	uint32 temp;
	uint32 ctl0, ctl1, ctl2, ctl3;
	//SCI_ASSERT((spi_parm->data_width >=0) && (spi_parm->data_width < 32));

	SPI_Reset(0, 1000);  //Reset spi0&spi1
	//SPI_Reset(1, 1000);

	spi_ctr_ptr->clkd = spi_parm->clk_div;

	temp  = 0;
	temp |= (spi_parm->tx_edge << 0)    |
		(spi_parm->rx_edge << 1)    |
		(spi_parm->data_width << 2) |
		(spi_parm->msb_lsb_sel<< 7) |
		(0xE<<8);//CS-------------------------select cs0/cs1: 0-selected. 1-none
	spi_ctr_ptr->ctl0 = temp;

	// storage registers
	ctl0 = spi_ctr_ptr->ctl0;
	ctl1 = spi_ctr_ptr->ctl1;
	ctl3 = spi_ctr_ptr->ctl3;

	spi_ctr_ptr->ctl0  =  ctl0 & ~0x7C;                 // set bit-length to 32
	//spi_ctr_ptr->ctl1  = (ctl1 & ~BIT_12) | BIT_13;     // set transmite mode
	spi_ctr_ptr->ctl1  = (ctl1 | BIT_12 | BIT_13);     // set rx/tx mode

	// set water mark of reveive FIFO
	spi_ctr_ptr->ctl3  = (ctl3 & ~0xFFFF) | 
		((BURST_SIZE >>2) <<8) | 
		(BURST_SIZE >>2);
}

PUBLIC void SPI_WaitTxFinish()
{
	volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);

	while( !(spi_ctr_ptr->iraw)&BIT_8 ) // IS tx finish
	{
	}  
	spi_ctr_ptr->iclr |= BIT_8;

	// Wait for spi bus idle
	while((spi_ctr_ptr->sts2)&BIT_8) 
	{
	}
	// Wait for tx real empty
	while( !((spi_ctr_ptr->sts2)&BIT_7) ) 
	{
	}      
}

PUBLIC void SPI_WriteData(uint32 data, uint32 data_len, uint32 dummy_bitlen)
{
	uint32 command;
	volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);

	// The unit of data_len is identical with buswidth
	SPI_SetTxLen(data_len, dummy_bitlen);
	SPI_TxReq( );

	spi_ctr_ptr->data = data;

	SPI_WaitTxFinish();
}

PUBLIC uint32 SPI_ReadData( uint32 data_len, uint32 dummy_bitlen )
{
	uint32 read_data=0, rxt_cnt=0;
	volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);

	// The unit of data_len is identical with buswidth
	SPI_SetRxLen(data_len, dummy_bitlen);
	SPI_RxReq( );

	//Wait for spi receive finish
	while( !((spi_ctr_ptr->iraw)&BIT_9) )
	{
		//wait rxt fifo full
		if((spi_ctr_ptr->iraw)&BIT_6)
		{
			rxt_cnt = (spi_ctr_ptr->ctl3)&0x1F;
			//SCI_TraceLow("---FIFOFULL:rxt_cnt=0x%x", rxt_cnt);
			while(rxt_cnt--)
			{
				read_data = spi_ctr_ptr->data;   
				//SCI_TraceLow("---FIFOFULL: SPI_ReadData =0x%x", read_data);
			}
		}
	}

	// Wait for spi bus idle
	while((spi_ctr_ptr->sts2)&BIT_8) 
	{
	}

	//
	while(data_len--)
	{
		read_data = spi_ctr_ptr->data;   
		//SCI_TraceLow("---Finish: SPI_ReadData =0x%x", read_data);
	}

	return (read_data);
}
