#ifndef _SDRAM_CFG_H_
#define _SDRAM_CFG_H_

/* The SDRAM size parameters supports */
#define     SDRAM_8M       0
#define     SDRAM_16M      1
#define     SDRAM_32M      2
#define     SDRAM_64M      3

#define     BK_MODE_1               0           //1 bank sdram
#define     BK_MODE_2               1           //2 bank sdram          
#define     BK_MODE_4               2           //4 bank sdram
#define     BK_MODE_8               3           //8 bank sdram

#define     ROW_MODE_11             0           // 11 row sdram
#define     ROW_MODE_12             1           // 12 row sdram
#define     ROW_MODE_13             2           // 13 row sdram

#define     COL_MODE_8              0           //8 column sdram
#define     COL_MODE_9              1           //9 column sdram
#define     COL_MODE_10             2           //10 column sdram
#define     COL_MODE_11             3           //11 column sdram
#define     COL_MODE_12             4           //12 column sdram

#define     DATA_WIDTH_16          16           //16 bit sdram
#define     DATA_WIDTH_32          32           //32 bit sdram

#define     BURST_LEN_2             1           // 2 words burst
#define     BURST_LEN_4             2           // 4 words burst
#define     BURST_LEN_8             3           // 8 words burst
#define     BURST_LEN_16            4           // 16 words burst
#define     BURST_LEN_FULLPAGE      7           // full page burst

#define     CAS_LATENCY_1           1           // 1 cycle cas latency
#define     CAS_LATENCY_2           2           // 2 cycle cas latency
#define     CAS_LATENCY_3           3           // 3 cycle cas latency

/* Extend mode register value supports*/
#define     SDRAM_EXT_MODE_INVALID     0xffffffff

#define     SDRAM_TIMING_PARA_NUM       10

/* Clock delay value SC6600I supports */
#define     SC6600I_SDRAM_CLK_DLY_VAL1    0x1110
#define     SC6600I_SDRAM_CLK_DLY_VAL2    0x1210
#define     SC6600I_SDRAM_CLK_DLY_VAL3    0x1310
#define     SC6600I_SDRAM_CLK_DLY_VAL4    0x1410//6600I dvb can use this value run @96MHZ

/* Clock delay value SC6600R supports */
#define     SC6600R_SDRAM_CLK_DLY_VAL1    0x2120
#define     SC6600R_SDRAM_CLK_DLY_VAL2    0x2220
#define     SC6600R_SDRAM_CLK_DLY_VAL3    0x2320
#define     SC6600R_SDRAM_CLK_DLY_VAL4    0x2320

/* Clock delay value supports for 6800*/
#define     DCLK_OUT_DLY_0X04           0x4
#define     DCLK_OUT_DLY_0X1D           0x1D
#define     DCLK_OUT_DLY_0X1E           0x1E

#define     DCLK_IN_DLY_0X09            0x9
#define     DCLK_IN_DLY_0X13            0x13
#define     DCLK_IN_DLY_0X14            0x14

//for 6800 DVB platform, config goes like this:
#define     DVB_DCLK_OUT_DLY_0X05       0x5
#define     DVB_DCLK_OUT_DLY_0X06       0x6

#define     DVB_DCLK_IN_DLY_0X0A        0xa
#define     DVB_DCLK_IN_DLY_0X0B        0xb
    

/* The sdram configuration struct */
typedef struct {
	uint32_t  bank_mode;    //Can only be set as BK_MODE_1,BK_MODE_2,BK_MODE_4,BK_MODE_8
	uint32_t  row_mode;     //Can only be set as ROW_MODE_11,ROW_MODE_12,ROW_MODE_13
	uint32_t  col_mode;     //Can only be set as COL_MODE_8,COL_MODE_9,COL_MODE_10,COL_MODE_11,COL_MODE_12
	uint32_t  data_width;   //Can only be set as DATA_WIDTH_16,DATA_WIDTH_32
	uint32_t  burst_length; //Can only be set as BURST_LEN_2,BURST_LEN_4,BURST_LEN_8,BURST_LEN_16,BURST_LEN_FULLPAGE
	uint32_t  cas_latency;  //Can only be set as CAS_LATENCY_1,CAS_LATENCY_2,CAS_LATENCY_3
	uint32_t  ext_mode_val; /* User can config extend mode register in SDRAM. If it is not used, SDRAM_EXT_INVALID
	                                  is adopted. */
	uint32_t	sdram_size;            /* Can only be set as SDRAM_8M, SDRAM_16M, SDRAM_32M or SDRAM_64M */
	uint32_t  clk_dly;	       /* The clk_dly register's value. The most likely value is 0x1011, 0x1012, 0x1013 */	
} sdram_cfg_t;

typedef struct
{
	uint32_t row_ref_max;		//ROW_REFRESH_TIME,Refresh interval time , ns, tREF-max = 7800 ns
	uint32_t row_pre_min;		//ROW_PRECHARGE_TIME , ns, tRP-min = 27 ns.                      
	uint32_t row_cyc_min;		//ROW_CYCLE_TIME
	uint32_t rcd_min;     	// T_RCD,ACTIVE to READ or WRITE delay  , ns, tRCD-min = 27 ns  
	uint32_t wr_min;      	// T_WR  ,WRITE recovery time  , ns, tWR-min = 15 ns.            
	uint32_t mrd_min;     	//T_MRD , 2 cycles, tMRD-min = 2 cycles.                        
	uint32_t rfc_min;     	//T_RFC, AUTO REFRESH command period , ns, tRFC-min = 80 ns.    
	uint32_t xsr_min;     	//T_XSR  , ns, tXSR-min = 120 ns.                               
	uint32_t ras_min;     	//T_RAS_MIN , row active time, ns, tRAS-min = 50ns              
}sdram_timing_t;
extern void sdram_init(uint32_t ahb_clk);
#endif /* SDRAM_CFG_H */
