/*********************************************************************************/
//             Macro Definition For SDRAM CONFIG
/*********************************************************************************/
#define SDRAM_CLK_DLY 			SC6600R_SDRAM_CLK_DLY_VAL1
#if (DRAM_TYPE == SDR_SDRAM)
#define SDRAM_EXT_MODE_VALUE 	SDRAM_EXT_MODE_INVALID
#else 
#define SDRAM_EXT_MODE_VALUE	SDRAM_EXT_MODE_REG
#endif
