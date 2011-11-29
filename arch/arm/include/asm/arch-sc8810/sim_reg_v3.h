/******************************************************************************
 ** File Name:    sim_reg_v3.h                                           *
 ** Author:       Mingwei.Zhang                                               *
 ** DATE:         05/13/2010                                                  *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 05/13/2010    Mingwei.Zhang   Create.                                     *
 ******************************************************************************/
#ifndef _SIM_REG_V3_H_
#define _SIM_REG_V3_H_
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
**                               Macro Define                                **
**---------------------------------------------------------------------------*/
/*----------SIM Control Regosters----------*/
//SIM_BASE      0x85000000
#define SIM_TX                          (SIM0_BASE + 0x0000)     //Writing to this reg will send data to tx fifo and then the data get transmitted.
#define SIM_RX                          (SIM0_BASE + 0x0004)     //Read from this address retrieve data from rx fifo.
#define SIM_STS0                        (SIM0_BASE + 0x0008)     //Status.
#define SIM_STS1                        (SIM0_BASE + 0x000C)     //Status.
#define SIM_IE                                 (SIM0_BASE + 0x0010)     //Interrupt Enable.
#define SIM_ICLR                        (SIM0_BASE + 0x0014)     //Interrupt clear.
#define SIM_CTL0                        (SIM0_BASE + 0x0018)        //Contorl 0 register
#define SIM_CTL1                        (SIM0_BASE + 0x001C)        //Control 1 register
#define SIM_RX_CK_DVD               (SIM0_BASE + 0x0020)        //the SIM clock divider to interpret the SIM data
#define SIM_SEH                         (SIM0_BASE + 0x0024)
#define SIM_TGC                         (SIM0_BASE + 0x0028)
#define SIM_WDT                         (SIM0_BASE + 0x002C)
#define SIM_INT_M                       (SIM0_BASE + 0x0030)
#define SIM_TX_CK_DVD               (SIM0_BASE + 0x0034)
#define SIM_WDT1                        (SIM0_BASE + 0x0038)

#define SIM1_TX                         (SIM1_BASE + 0x0000)     //Writing to this reg will send data to tx fifo and then the data get transmitted.
#define SIM1_RX                         (SIM1_BASE + 0x0004)     //Read from this address retrieve data from rx fifo.
#define SIM1_STS0                       (SIM1_BASE + 0x0008)     //Status.
#define SIM1_STS1                       (SIM1_BASE + 0x000C)     //Status.
#define SIM1_IE                         (SIM1_BASE + 0x0010)     //Interrupt Enable.
#define SIM1_ICLR                       (SIM1_BASE + 0x0014)     //Interrupt clear.
#define SIM1_CTL0                       (SIM1_BASE + 0x0018)        //Contorl 0 register
#define SIM1_CTL1                       (SIM1_BASE + 0x001C)        //Control 1 register
#define SIM1_RX_CK_DVD                  (SIM1_BASE + 0x0020)        //the SIM clock divider to interpret the SIM data
#define SIM1_SEH                        (SIM1_BASE + 0x0024)
#define SIM1_TGC                        (SIM1_BASE + 0x0028)
#define SIM1_WDT                        (SIM1_BASE + 0x002C)
#define SIM1_INT_M                      (SIM1_BASE + 0x0030)
#define SIM1_TX_CK_DVD                  (SIM1_BASE + 0x0034)
#define SIM1_WDT1                       (SIM1_BASE + 0x0038)


//The corresponding bit of SIM_IMSK register.
//the status0 that masked with the IE regiseter??
#define SIMIMSK_B_RX_FIFO_FULL          (1 << 0)        //Masked int bit for rx full int
#define SIMIMSK_B_TX_FIFO_EMPTY         (1 << 1)        //Masked int bit for tx empty int
#define SIMIMSK_B_RX_PARITY_ERR         (1 << 2)        //Masked int bit for rx_parity_error
#define SIMIMSK_B_TX_PARITY_ERR         (1 << 3)        //Masked int bit for tx_parity_error
#define SIMIMSK_B_UNRESP_CARD           (1 << 4)        //Masked int bit for unresp_card
#define SIMIMSK_B_CARD_IN               (1 << 5)        //Masked int bit for card_in
#define SIMIMSK_B_CARD_OUT              (1 << 6)        //Masked int bit for card_out
#define SIMIMSK_B_EARLY_ATR             (1 << 7)        //Masked int bit for early answer to reset
#define SIMIMSK_B_ACTIVE_DONE           (1 << 8)        //Masked int bit for active done

//the default register value of SIM register that used to initialize the
//sim register at the drive start,the value may be changed
#define SIM_DEFAULT_STS0                0x0
#define SIM_DEFAULT_STS1                0x0
#define SIM_DEFAULT_IE                  0x01fc
#define SIM_DEFAULT_ICLR                0x01ff
#define SIM_DEFAULT_CTL0                0x2003

#define SIM_13M_CTL1                    0x0101
#define SIM_26M_CTL1                    0x0201
#define SIM_39M_CTL1                    0x0201
#define SIM_52M_CTL1                    0x0301
#define SIM_78M_CTL1                    0x0300

#define SIM_DIV4_CTL1                    0x0101
#define SIM_DIV8_CTL1                    0x0201
#define SIM_DIV16_CTL1                   0x0301
#define SIM_DIV32_CTL1                   0x0401


#define SIM_DEFAULT_SHE                 0x00         //enable the rx and tx retransmit to handle the rx and tx parity error
#define SIM_ENABLED_SHE                 0x33         //Rx retransmit limit = 3;Tx retransmit limit = 3
//the tx etu is the same as rx etu

///#define SIM_DEFAULT_TGC                  0x0505
///#define SIM_DEFAULT_WDT                  0x802        //Disable the rx time out watch dog
//Enable the rx retransmit watch dog timer
//Watch dog timer limit for  rx retransmit watch dog = 0x200
#define SIM_ENABLED_WDT                 0x803        //Enable the rx time out watch dog
//Enable the rx retransmit watch dog timer
//Watch dog timer limit for  rx retransmit watch dog = 0x200
#define SIM_DEFAULT_IMASKED             0x0
#define SIM_DEFAULT_WDT1                0x100        //Watch dog count limit for rx time out.

//2002-04-10  Raislin.Kong   Modify the SIM relative register define
//The corresponding bit of SIM_STS0 register.
#define SIMSTS0_B_RX_FIFO_FULL          (1 << 0)        //rx fifo data number bigger than rx_int_mark
#define SIMSTS0_B_TX_FIFO_EMPTY         (1 << 1)        //tx fifo data number bigger than tx_int_mark
#define SIMSTS0_B_RX_PARITY_ERR         (1 << 2)        //Int status bit for rx parity error
#define SIMSTS0_B_TX_PARITY_ERR         (1 << 3)        //Int status bit for tx parity error
#define SIMSTS0_B_UNRESP_CARD           (1 << 4)        //Int status bit for card unresponsive
#define SIMSTS0_B_CARD_IN               (1 << 5)        //Int status bit for card inserted
#define SIMSTS0_B_CARD_OUT              (1 << 6)        //Int status bit for card removed
#define SIMSTS0_B_EARLY_ATR             (1 << 7)        //Int status bit for early answer to reset
#define SIMSTS0_B_ACTIVE_DONE           (1 << 8)        //Int status bit to show activation is done
#define SIMSTS0_B_RX_TOUT               (1 << 9)        //Int status bit to show activation is done


//The corresponding bit of SIM_STS1 register.
//Get the RX byte number in the rx fifo,rx_fifo_count--SIM_STS1[3:0]
#define GET_RX_FIFO_COUNT(sim_register_map)     ((sim_register_map)->sts1 & 0x001f)
//Get the TX byte number in the TX fifo,rx_fifo_count--SIM_STS1[8:5]
#define GET_TX_FIFO_COUNT(sim_register_map)     (((sim_register_map)->sts1 & 0x03e0) >> 5)
#define SIMSTS1_B_CARD_INSERTED         (1 << 10)        //Reflect of card in input pin
#define SIMSTS1_B_SIM_DATA              (1 << 11)        //Reflect of sim data io pin
#define SIMSTS1_B_SIM_ACTIVE_STS        (1 << 12)        //Activation status, 1: activated. 0: not activated.
#define SIMSTS1_B_ACTIVE_ON             (1 << 13)        //Busy in activation process
#define SIMSTS1_B_DEACTIVE              (1 << 14)        //Busy in deactivation process

//The corresponding bit of SIM_IE register.
#define SIMIE_B_RX_FULL                 (1 << 0)        //Enable bit for rx full int
#define SIMIE_B_TX_EMPTY                (1 << 1)        //Enable bit for tx empty int
#define SIMIE_B_RX_PARITY_ERR           (1 << 2)        //Enable bit for rx_parity_error
#define SIMIE_B_TX_PARITY_ERR           (1 << 3)        //Enable bit for tx_parity_error
#define SIMIE_B_UNRESP_CARD             (1 << 4)        //Enable bit for unresp_card
#define SIMIE_B_CARD_IN                 (1 << 5)        //Enable bit for card_in
#define SIMIE_B_CARD_OUT                (1 << 6)        //Enable bit for card_out
#define SIMIE_B_EARLY_ATR               (1 << 7)        //Enable bit for early answer to reset
#define SIMIE_B_ACTIVE_DONE             (1 << 8)        //Enable bit for active done
#define SIMIE_B_RX_TOUT                 (1 << 9)        //Enable bit for rx time out

//The corresponding bit of SIM_ICLR register.
#define SIMICLR_B_RX_FULL               (1 << 0)        //Int clear bit for rx full int
#define SIMICLR_B_TX_EMPTY              (1 << 1)        //Int clear bit for tx empty int
#define SIMICLR_B_RX_PARITY_ERR         (1 << 2)        //Int clear bit for rx_parity_error
#define SIMICLR_B_TX_PARITY_ERR         (1 << 3)        //Int clear bit for tx_parity_error
#define SIMICLR_B_UNRESP_CARD           (1 << 4)        //Int clear bit for unresp_card
#define SIMICLR_B_CARD_IN               (1 << 5)        //Int clear bit for card_in
#define SIMICLR_B_CARD_OUT              (1 << 6)        //Int clear bit for card_out
#define SIMICLR_B_EARLY_ATR             (1 << 7)        //Int clear bit for early answer to reset
#define SIMICLR_B_ACTIVE_DONE           (1 << 8)        //Int clear bit bit for active done
#define SIMICLR_B_RX_TOUT               (1 << 9)        //Int clear bit bit for rx time out

//The corresponding bit of SIM_CTL0 register.
#define SIMCTL0_B_BIT_CONVENTION        (1 << 0)        //Bit Convention:0-MSB Transmitted first;1-LSM transmitted first                         1
#define SIMCTL0_B_LOGIC_LEVEL           (1 << 1)        //Logic Level:0-high logic level represet "0";1-high logic level represet "1"
#define SIMCTL0_B_EVEN_PARITY           (1 << 2)        //0:Even Parity;1:Odd parity
#define SIMCTL0_B_LOOPBACK_MODE         (1 << 3)        //Transmit data looped back to receive.
#define SIMCTL0_B_RX_FIFO_RST           (1 << 4)        //Reset the rx fifo 
#define SIMCTL0_B_TX_FIFO_RST           (1 << 5)        //Reset the tx fifo
#define SIMCTL0_B_SIM_RST               (1 << 6)        //Reset the sim card module 
#define SIMCTL0_B_POWER_EN              (1 << 7)        //Enable the Power supply to sim card 
#define SIMCTL0_B_CARD_OUT_LATCHED      (1 << 8)        //This bit will be 1 if card_out happened.
#define SIMCTL0_B_RX_EN                 (1 << 9)        //Enable the sim rx
#define SIMCTL0_B_TX_EN                 (1 << 10)       //Enable the sim tx
#define SIMCTL0_B_EN_ACTIVE             (1 << 11)       //Enactive the SIM card
#define SIMCTL0_B_DE_ACTIVE             (1 << 12)       //Deactive the SIM card
#define SIMCTL0_B_ACT_DEACT_EN          (1 << 13)       //Enable the active/deactive procedure
#define SIMCTL0_B_AUTO_ACT_DEACT        (1 << 14)       //Enable the auto start of active/deactive when card_in or card_out happen.
#define SIMCTL0_B_TX_DATA_OUT_LOW       (1 << 15)       //Force the tx data to low logic level.

//the SIM clock mode
typedef enum sim_clock_mode_tag
{
    SIM_CLOCK_DIVIDE_2 = 0x0,
    SIM_CLOCK_DIVIDE_4,
    SIM_CLOCK_DIVIDE_8,
    SIM_CLOCK_DIVIDE_16,
    SIM_CLOCK_DIVIDE_32,
    SIM_CLOCK_DIVIDE_64,
    SIM_CLOCK_DIVIDE_128,
    SIM_CLOCK_DIVIDE_256
}
sim_clock_mode;

//the sim card fifo buf size
#define SIM_FIFO_BUF_SIZE               16

//SIM Card Interface.
typedef struct sim_tag
{
    VOLATILE uint32 tx;
    VOLATILE uint32 rx;
    VOLATILE uint32 sts0;
    VOLATILE uint32 sts1;
    VOLATILE uint32 ie;
    VOLATILE uint32 iclr;
    VOLATILE uint32 ctl0;
    VOLATILE uint32 ctl1;
    VOLATILE uint32 rx_clkd;
    VOLATILE uint32 she;
    VOLATILE uint32 tgc;
    VOLATILE uint32 wdt;
    VOLATILE uint32 imsk;
    VOLATILE uint32 tx_clkd;
    VOLATILE uint32 wdt1;
} sim_s;

//The corresponding bit of SIM_CTL1 register.
//set the rx int mark,that when rx data num in the rx fifo up to the mark
//the sim card will interupt with rx_fifo_full bit of SIM_STS1 set to 1---SIM_CTL1[3:0]
#define SET_RX_INT_MARK(sim_register_map,mark)      (((sim_register_map)->ctl1 & 0xfff0) | mark)
//set the tx int mark,that when tx data num in the tx fifo down to the mark
//the sim card will inerupt with tx_fifo_empty bit of SIM_STS1 set to 1---SIM_CTL1[7:4]
#define SET_TX_INT_MARK(sim_register_map,mark)      (((sim_register_map)->ctl1 & 0xff0f) | (mark << 4))
//Set the SIM clock mode.SIM clock mode--SIM_CTL1[10:8]
#define SET_CLOCK_MODE(sim_register_map,clock_mode)     (((sim_register_map)->ctl1 & 0xf8ff) | (clock_mode << 8))
#define SIMCTL1_B_CLK_ENABLE            (1 << 11)       //Enable or Disable the SIM clock
#define SIMCTL1_B_CLK_POLARITY          (1 << 12)       //The logic Level when the SIM clock is Disabled
//set the ad speed ctl,ad speed ctl is the time interval
//between set control register---SIM_CTL1[15:13]
#define SET_AD_SPEED_CTL(sim_register_map,ad_speed_ctl) (((sim_register_map)->ctl1 & 0x1fff) | (ad_speed_ctl << 13))

//The corresponding bit of SIM_SHE register.
//set the RX retransmit time limit---SIM_SHE[3:0]
#define SET_RX_RETRX_LMT(sim_register_map,retransmit_limit)     (((sim_register_map)->she & 0xfff0) | retransmit_limit)
//set the TX retransmit time limit---SIM_SHE[7:4]
#define SET_TX_RETRX_LMT(sim_register_map,retransmit_limit)     (((sim_register_map)->she & 0xff0f) | (retransmit_limit << 4))

//The corresponding bit of SIM_TGC register
//set the time interval between the tx consecutive byte---SIM_TGC[7:0]
#define SET_GUARD_TIME(sim_register_map,guard_time)     ((((sim_s*)sim_register_map)->tgc & 0xff00) | guard_time)
//set the time interal between the tx and rx---SIM_TGC[15:8]
#define SET_TURN_AROUND_GUARD_TIME(sim_register_map,turn_around_time)   (((sim_register_map)->tgc & 0x00ff) | (turn_around_time << 8))


//the corresponding bit of SIM_WDT register
#define SIMWDT_B_TRIGGER                (1 << 0)        //Trigger the start of watch dog timmer
#define SIMWDT_B_REPEAT_EN              (1 << 1)        //Enable the timer repeat mode
//set the watch dog count limit,for example it's in rx,now the rx_int_mark is 8 but there only
//4 byte data to rx,and the rx_full interupt will not happened as soon as rx finish untill the
//watch dog count limit time expired---SIM_WDT[?:2]
#define SET_WATCH_DOG_COUNT_LIMIT(sim_register_map,count_limit) (((sim_register_map->wdt) & 0x0003) | (count_limit << 2))



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
#endif //_SIM_REG_V3_H_
// End

