/******************************************************************************
 ** File Name:      sc8800h_reg_pinmap.h                                                   *
 ** Author:         Younger.Yang                                                                     *
 ** DATE:           07/18/2008                                                                       *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.  *
 ** Description:                                                                                                 *
 **                 Pinmap Register address map for the SC8800H chip          *
 **                 Reference to the SC8800H control register document        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                                                                                                          *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                                                                                          *
 ** 07/17/2008     Liangwen.Zhen	    Create.                                                                                          * 
 ******************************************************************************/
#ifndef _SC8800H_REG_PINMAP_H_
#define _SC8800H_REG_PINMAP_H_

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                                                                                               *
 **---------------------------------------------------------------------------*/

#define PINMAP_REG_BASE		0x8C000000
#define REG_ADDR(_x_)		(PINMAP_REG_BASE + _x_)

#define PIN_SIMCLK0_REG    REG_ADDR(0x000)
#define PIN_SIMDA0_REG     REG_ADDR(0x004)
#define PIN_SIMRST0_REG    REG_ADDR(0x008)
#define PIN_SIMCLK1_REG    REG_ADDR(0x00C)
#define PIN_SIMDA1_REG     REG_ADDR(0x010)
#define PIN_SIMRST1_REG    REG_ADDR(0x014)
#define PIN_SD0_CLK_REG    REG_ADDR(0x018)
#define PIN_SD0_CMD_REG    REG_ADDR(0x01C)
#define PIN_SD0_D0_REG     REG_ADDR(0x020)
#define PIN_SD0_D1_REG     REG_ADDR(0x024)
#define PIN_SD0_D2_REG     REG_ADDR(0x028)
#define PIN_SD0_D3_REG     REG_ADDR(0x02C)
#define PIN_SD1_CLK_REG    REG_ADDR(0x030)
#define PIN_SD1_CMD_REG    REG_ADDR(0x034)
#define PIN_SD1_D0_REG     REG_ADDR(0x038)
#define PIN_SD1_D1_REG     REG_ADDR(0x03C)
#define PIN_SD1_D2_REG     REG_ADDR(0x040)
#define PIN_SD1_D3_REG     REG_ADDR(0x044)
#define PIN_KEYOUT0_REG    REG_ADDR(0x048)
#define PIN_KEYOUT1_REG    REG_ADDR(0x04C)
#define PIN_KEYOUT2_REG    REG_ADDR(0x050)
#define PIN_KEYOUT3_REG    REG_ADDR(0x054)
#define PIN_KEYOUT4_REG    REG_ADDR(0x058)
#define PIN_KEYOUT5_REG    REG_ADDR(0x05C)
#define PIN_KEYIN0_REG     REG_ADDR(0x060)
#define PIN_KEYIN1_REG     REG_ADDR(0x064)
#define PIN_KEYIN2_REG     REG_ADDR(0x068)
#define PIN_KEYIN3_REG     REG_ADDR(0x06C)
#define PIN_KEYIN4_REG     REG_ADDR(0x070)
#define PIN_XTLEN_REG      REG_ADDR(0x074)
#define PIN_MTDO_REG       REG_ADDR(0x078)
#define PIN_MTDI_REG       REG_ADDR(0x07C)
#define PIN_MTCK_REG       REG_ADDR(0x080)
#define PIN_MTMS_REG       REG_ADDR(0x084)
#define PIN_MTRST_N_REG    REG_ADDR(0x088)
#define PIN_ARMCLK_REG     REG_ADDR(0x08C)
#define PIN_U0TXD_REG      REG_ADDR(0x090)
#define PIN_U0RXD_REG      REG_ADDR(0x094)
#define PIN_SCL_REG        REG_ADDR(0x098)
#define PIN_SDA_REG        REG_ADDR(0x09C)
#define PIN_U1TXD_REG      REG_ADDR(0x0A0)
#define PIN_U1RXD_REG      REG_ADDR(0x0A4)
#define PIN_Reserved0       REG_ADDR(0x0A8)
#define PIN_Reserved1       REG_ADDR(0x0AC)
#define PIN_IISDI0_REG     REG_ADDR(0x0B0)
#define PIN_IISDO0_REG     REG_ADDR(0x0B4)
#define PIN_IISCLK0_REG    REG_ADDR(0x0B8)
#define PIN_IISLRCK0_REG   REG_ADDR(0x0BC)
#define PIN_IISMCK0_REG    REG_ADDR(0x0C0)
#define PIN_IISDI1_REG     REG_ADDR(0x0C4)
#define PIN_IISDO1_REG     REG_ADDR(0x0C8)
#define PIN_IISCLK1_REG    REG_ADDR(0x0CC)
#define PIN_IISLRCK1_REG   REG_ADDR(0x0D0)
#define PIN_IISMCK1_REG    REG_ADDR(0x0D4)
#define PIN_PBINT_REG      REG_ADDR(0x0D8)
#define PIN_Reserved2       REG_ADDR(0x0DC)
#define PIN_Reserved3       REG_ADDR(0x0E0)
#define PIN_Reserved4       REG_ADDR(0x0E4)
#define PIN_Reserved5       REG_ADDR(0x0E8)
#define PIN_EMA0_REG       REG_ADDR(0x0EC)
#define PIN_EMA1_REG       REG_ADDR(0x0F0)
#define PIN_EMA2_REG       REG_ADDR(0x0F4)
#define PIN_EMA3_REG       REG_ADDR(0x0F8)
#define PIN_EMA4_REG       REG_ADDR(0x0FC)
#define PIN_EMA5_REG       REG_ADDR(0x100)
#define PIN_EMA6_REG       REG_ADDR(0x104)
#define PIN_EMA7_REG       REG_ADDR(0x108)
#define PIN_EMA8_REG       REG_ADDR(0x10C)
#define PIN_EMA9_REG       REG_ADDR(0x110)
#define PIN_EMA10_REG      REG_ADDR(0x114)
#define PIN_EMA11_REG      REG_ADDR(0x118)
#define PIN_EMA12_REG      REG_ADDR(0x11C)
#define PIN_EMD16_REG      REG_ADDR(0x120)
#define PIN_EMD17_REG      REG_ADDR(0x124)
#define PIN_EMD18_REG      REG_ADDR(0x128)
#define PIN_EMD19_REG      REG_ADDR(0x12C)
#define PIN_EMD20_REG      REG_ADDR(0x130)
#define PIN_EMD21_REG      REG_ADDR(0x134)
#define PIN_EMD22_REG      REG_ADDR(0x138)
#define PIN_EMD23_REG      REG_ADDR(0x13C)
#define PIN_EMD24_REG      REG_ADDR(0x140)
#define PIN_EMD25_REG      REG_ADDR(0x144)
#define PIN_EMD26_REG      REG_ADDR(0x148)
#define PIN_EMD27_REG      REG_ADDR(0x14C)
#define PIN_EMD28_REG      REG_ADDR(0x150)
#define PIN_EMD29_REG      REG_ADDR(0x154)
#define PIN_EMD30_REG      REG_ADDR(0x158)
#define PIN_EMD31_REG      REG_ADDR(0x15C)
#define PIN_EMRAS_N_REG    REG_ADDR(0x160)
#define PIN_EMCAS_N_REG    REG_ADDR(0x164)
#define PIN_EMWE_N_REG     REG_ADDR(0x168)
#define PIN_CLKDPMEM_REG   REG_ADDR(0x16C)
#define PIN_CLKDMMEM_REG   REG_ADDR(0x170)
#define PIN_EMDQM0_REG     REG_ADDR(0x174)
#define PIN_EMDQM1_REG     REG_ADDR(0x178)
#define PIN_EMDQM2_REG     REG_ADDR(0x17C)
#define PIN_EMDQM3_REG     REG_ADDR(0x180)
#define PIN_EMCS_N0_REG    REG_ADDR(0x184)
#define PIN_EMCS_N1_REG    REG_ADDR(0x188)
#define PIN_EMCS_N2_REG    REG_ADDR(0x18C)
#define PIN_EMCS_N3_REG    REG_ADDR(0x190)
#define PIN_EMCKE0_REG     REG_ADDR(0x194)
#define PIN_EMCKE1_REG     REG_ADDR(0x198)
#define PIN_EMBA0_REG      REG_ADDR(0x19C)
#define PIN_EMBA1_REG      REG_ADDR(0x1A0)
#define PIN_EMDQS0_REG     REG_ADDR(0x1A4)
#define PIN_EMDQS1_REG     REG_ADDR(0x1A8)
#define PIN_EMDQS2_REG     REG_ADDR(0x1AC)
#define PIN_EMDQS3_REG     REG_ADDR(0x1B0)
#define PIN_EMD0_REG       REG_ADDR(0x1B4)
#define PIN_EMD1_REG       REG_ADDR(0x1B8)
#define PIN_EMD2_REG       REG_ADDR(0x1BC)
#define PIN_EMD3_REG       REG_ADDR(0x1C0)
#define PIN_EMD4_REG       REG_ADDR(0x1C4)
#define PIN_EMD5_REG       REG_ADDR(0x1C8)
#define PIN_EMD6_REG       REG_ADDR(0x1CC)
#define PIN_EMD7_REG       REG_ADDR(0x1D0)
#define PIN_EMD8_REG       REG_ADDR(0x1D4)
#define PIN_EMD9_REG       REG_ADDR(0x1D8)
#define PIN_EMD10_REG      REG_ADDR(0x1DC)
#define PIN_EMD11_REG      REG_ADDR(0x1E0)
#define PIN_EMD12_REG      REG_ADDR(0x1E4)
#define PIN_EMD13_REG      REG_ADDR(0x1E8)
#define PIN_EMD14_REG      REG_ADDR(0x1EC)
#define PIN_EMD15_REG      REG_ADDR(0x1F0)
#define PIN_NFWPN_REG      REG_ADDR(0x1F4)
#define PIN_LCMRSTN_REG    REG_ADDR(0x1F8)
#define PIN_NFRB_REG       REG_ADDR(0x1FC)
#define PIN_LCMCD_REG      REG_ADDR(0x200)
#define PIN_NFCLE_REG      REG_ADDR(0x204)
#define PIN_NFALE_REG      REG_ADDR(0x208)
#define PIN_NFCEN_REG      REG_ADDR(0x20C)
#define PIN_NFWEN_REG      REG_ADDR(0x210)
#define PIN_NFREN_REG      REG_ADDR(0x214)
#define PIN_NFD0_REG       REG_ADDR(0x218)
#define PIN_NFD1_REG       REG_ADDR(0x21C)
#define PIN_NFD2_REG       REG_ADDR(0x220)
#define PIN_NFD3_REG       REG_ADDR(0x224)
#define PIN_NFD4_REG       REG_ADDR(0x228)
#define PIN_NFD5_REG       REG_ADDR(0x22C)
#define PIN_NFD6_REG       REG_ADDR(0x230)
#define PIN_NFD7_REG       REG_ADDR(0x234)
#define PIN_NFD8_REG       REG_ADDR(0x238)
#define PIN_LCMCSN0_REG    REG_ADDR(0x23C)
#define PIN_LCMCSN1_REG    REG_ADDR(0x240)
#define PIN_LCD_RSTN_REG   REG_ADDR(0x244)
#define PIN_LCD_EN_REG     REG_ADDR(0x248)
#define PIN_LCD_D0_REG     REG_ADDR(0x24C)
#define PIN_LCD_D1_REG     REG_ADDR(0x250)
#define PIN_LCD_D2_REG     REG_ADDR(0x254)
#define PIN_LCD_D3_REG     REG_ADDR(0x258)
#define PIN_LCD_D4_REG     REG_ADDR(0x25C)
#define PIN_LCD_D5_REG     REG_ADDR(0x260)
#define PIN_LCD_D6_REG     REG_ADDR(0x264)
#define PIN_LCD_D7_REG     REG_ADDR(0x268)
#define PIN_LCD_D8_REG     REG_ADDR(0x26C)
#define PIN_LCD_HS_REG     REG_ADDR(0x270)
#define PIN_LCD_VS_REG     REG_ADDR(0x274)
#define PIN_CLK_LCD_REG    REG_ADDR(0x278)
#define PIN_Reserved6       REG_ADDR(0x27C)
#define PIN_CCIRCK_REG     REG_ADDR(0x280)
#define PIN_CCIRHS_REG     REG_ADDR(0x284)
#define PIN_CCIRVS_REG     REG_ADDR(0x288)
#define PIN_CCIRD0_REG     REG_ADDR(0x28C)
#define PIN_CCIRD1_REG     REG_ADDR(0x290)
#define PIN_CCIRD2_REG     REG_ADDR(0x294)
#define PIN_CCIRD3_REG     REG_ADDR(0x298)
#define PIN_CCIRD4_REG     REG_ADDR(0x29C)
#define PIN_CCIRD5_REG     REG_ADDR(0x2A0)
#define PIN_CCIRD6_REG     REG_ADDR(0x2A4)
#define PIN_CCIRD7_REG     REG_ADDR(0x2A8)
#define PIN_CCIRD8_REG     REG_ADDR(0x2AC)
#define PIN_CCIRD9_REG     REG_ADDR(0x2B0)
#define PIN_CCIRRST_REG    REG_ADDR(0x2B4)
#define PIN_CCIRPD_REG     REG_ADDR(0x2B8)
#define PIN_HRESET_N_REG   REG_ADDR(0x2BC)
#define PIN_RFSDA0_REG     REG_ADDR(0x2C0)
#define PIN_RFSCK0_REG     REG_ADDR(0x2C4)
#define PIN_RFSEN0_0_REG   REG_ADDR(0x2C8)
#define PIN_RFSEN0_1_REG   REG_ADDR(0x2CC)
#define PIN_RFSDA1_REG     REG_ADDR(0x2D0)
#define PIN_RFSCK1_REG     REG_ADDR(0x2D4)
#define PIN_RFSEN1_0_REG   REG_ADDR(0x2D8)
#define PIN_RFSEN1_1_REG   REG_ADDR(0x2DC)
#define PIN_RFCTL0_REG     REG_ADDR(0x2E0)
#define PIN_RFCTL1_REG     REG_ADDR(0x2E4)
#define PIN_RFCTL2_REG     REG_ADDR(0x2E8)
#define PIN_RFCTL3_REG     REG_ADDR(0x2EC)
#define PIN_RFCTL4_REG     REG_ADDR(0x2F0)
#define PIN_RFCTL5_REG     REG_ADDR(0x2F4)
#define PIN_RFCTL6_REG     REG_ADDR(0x2F8)
#define PIN_RFCTL7_REG     REG_ADDR(0x2FC)
#define PIN_RFCTL8_REG     REG_ADDR(0x300)
#define PIN_RFCTL9_REG     REG_ADDR(0x304)
#define PIN_RFCTL10_REG    REG_ADDR(0x308)
#define PIN_RFCTL11_REG    REG_ADDR(0x30C)
#define PIN_RFCTL12_REG    REG_ADDR(0x310)
#define PIN_RFCTL13_REG    REG_ADDR(0x314)
#define PIN_RFCTL14_REG    REG_ADDR(0x318)
#define PIN_RFCTL15_REG    REG_ADDR(0x31C)
#define PIN_CCIR_SEL_REG   REG_ADDR(0x320)
#define PIN_SD_SEL_REG     REG_ADDR(0x324)
#define PIN_KEYOUT6_REG    REG_ADDR(0x328)
#define PIN_KEYOUT7_REG    REG_ADDR(0x32C)
#define PIN_Reserved7       REG_ADDR(0x330)
#define PIN_Reserved8       REG_ADDR(0x334)
#define PIN_LCD_D9_REG     REG_ADDR(0x338)
#define PIN_LCD_D10_REG    REG_ADDR(0x33C)
#define PIN_LCD_D11_REG    REG_ADDR(0x340)
#define PIN_LCD_D12_REG    REG_ADDR(0x344)
#define PIN_LCD_D13_REG    REG_ADDR(0x348)
#define PIN_LCD_D14_REG    REG_ADDR(0x34C)
#define PIN_LCD_D15_REG    REG_ADDR(0x350)
#define PIN_LCD_D16_REG    REG_ADDR(0x354)
#define PIN_LCD_D17_REG    REG_ADDR(0x358)
#define PIN_SD1_D4_REG     REG_ADDR(0x35C)
#define PIN_SD1_D5_REG     REG_ADDR(0x360)
#define PIN_SD1_D6_REG     REG_ADDR(0x364)
#define PIN_SD1_D7_REG     REG_ADDR(0x368)


/* Pinmap ctrl REGister Bit field value
------------------------------------------------------------------------------------------------------------
|				 |				 |				   |  			|			  |				|			   |
| Reserved[31:8] | Func Sel[7:6] | Drv Strght[5:4] | Pull Up[3] | Pull Down[2]| Input EN[1] | Output EN[0] |
|				 |				 |				   |  			|			  |				|			   |
------------------------------------------------------------------------------------------------------------
*/

#define PIN_Z_EN                            0x00			// High-Z in sleep mode
#define PIN_O_EN				0x01			// Output enable in sleep mode
#define PIN_I_EN				       0x02			// Input enable in sleep mode
#define PIN_PD_EN				0x04			// Pull down enable
#define PIN_PU_EN				0x08			// Pull up enable
#define PIN_PX_EN				0x00			// Don't pull down or up
#define PIN_DS_0				0x00			// Driver strength level 0
#define PIN_DS_1				0x10			// Driver strength level 1
#define PIN_DS_2				0x20			// Driver strength level 2
#define PIN_DS_3				0x30			// Driver strength level 3
#define PIN_FUNC_DEF			0x00
#define PIN_FUNC_1				0x40
#define PIN_FUNC_2				0x80
#define PIN_FUNC_3				0xC0

/* Pinmap control register bit field value structure*/
typedef union _pinmap_ctl_reg_u
{
	struct pinmap_ctl_reg_tag
	{
		volatile int reserved		:24;
		volatile int func_sel		:2;
		volatile int drv_strght		:2;
		volatile int pull_up_en		:1;
		volatile int pull_down_en	:1;
		volatile int input_en		:1;
		volatile int output_en		:1;
	}mBits;
	volatile int dwValue;
	
}PINMAP_CTL_REG_U;


#endif  //_SC8800H_REG_PINMAP_H_


