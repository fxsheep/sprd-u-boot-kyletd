#ifndef _SDRAM_H_
#define _SDRAM_H_
#define DCFG0_BKPOS_HADDR_6_5           0
#define DCFG0_BKPOS_HADDR_11_10         1
#define DCFG0_BKPOS_HEADR_16_15         2
#define DCFG0_BKPOS_HADDR_21_20         3
#define DCFG0_BKPOS_HADDR_23_22         4
#define DCFG0_BKPOS_HADDR_24_23         5
#define DCFG0_BKPOS_HADDR_25_24         6
#define DCFG0_BKPOS_HEADR_26_25         7

#define DCFG0_AUTO_PRE_POSITION_A10     (0u << 6)
#define DCFG0_AUTO_PRE_POSITION_A11     (1u << 6)
#define DCFG0_AUTO_PRE_POSITION_A12     (2u << 6)
#define DCFG0_AUTO_PRE_POSITION_A13     (3u << 6)

#define DCFG0_CLKDMEM_OUT_EN            (BIT_14)
#define DCFG0_CLKDMEM_OUT_SEL           (BIT_15)
#define DCFG0_ALTERNATIVE_EN            (BIT_16)
#define DCFG0_ROWHIT_EN                 (BIT_17)
#define DCFG0_AUTOREF_EN                (BIT_18)
#define DCFG0_AUTO_REF_ALLCS            (BIT_19)



#define DCFG2_CNT_DONE                  (BIT_14)
#define DCFG2_REF_CNT_RST               (BIT_15)
#define DCFG2_AUTO_SLEEP_MODE           (BIT_22)
#define DCFG2_AUTO_SLEEP_EN             (BIT_23)
#define DCFG2_SAMPLE_RST                (BIT_24)
#define DCFG2_SAMPLE_AUTO_RST_EN        (BIT_25)

//define mode register domain..
#define MODE_REG_BL_2               (1)
#define MODE_REG_BL_4               (2)
#define MODE_REG_BL_8               (3)

#define MODE_REG_BT_SEQ               (0)
#define MODE_REG_BT_INT               (1)

#define MODE_REG_CL_1                 (1)
#define MODE_REG_CL_2                 (2)
#define MODE_REG_CL_3                 (3)

#define MODE_REG_OPMODE               (0)

#define MODE_REG_WB_PRORAM            (0)
#define MODE_REG_WB_SINGLE            (1)

//define extended mode register domain...
#define EX_MODE_REG_PASR_4_BANKS      (0)
#define EX_MODE_REG_PASR_2_BANKS      (1)
#define EX_MODE_REG_PASR_1_BANKS      (2)
#define EX_MODE_REG_PASR_HALF_BANK      (5)
#define EX_MODE_REG_PASR_QUART_BANK      (6)  

#define EX_MODE_REG_DS_FULL           (0)
#define EX_MODE_REG_DS_HALF           (1)            


#endif

