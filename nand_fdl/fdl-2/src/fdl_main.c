#include "sci_types.h"
#include "fdl_main.h"
#include "cmd_def.h"
#include "packet.h"
#include "dl_engine.h"
#include "fdl_nand.h"
#include "sio_drv.h"
#include "flash_command.h"
#include "mcu_command.h"
#include "usb_boot.h"
#include "dma_drv_fdl.h"
#include "sc_reg.h"
#include "fdl_emmc.h"

extern  const unsigned char FDL2_signature[][24];
int sprd_clean_rtc(void);

static void fdl_error(void)
{
    //sio_putstr("The second FDL failed!\r\n");
    for (;;) /*Do nothing*/;
}
/*
 * Avoid const string comment out by -O2 opt level of compiler
*/
const unsigned char **FDL2_GetSig (void)
{
    return (const unsigned char **) FDL2_signature;
}
extern unsigned long _bss_end;
#ifdef CONFIG_SC8810
extern unsigned long _bss_start;

static int bss_end_end;
static int bss_start_start;
char mempool[1024*1024] = {0};
#endif
int main(void)
{
	/* All hardware initialization has been done in the 1st FDL,
	 * so we don't do initialization stuff here.
	 * The UART has also been opened by the 1st FDL and the baudrate
	 * has been setted correctly.
	 */  
	int err;
	uint32 sigture_address;
	unsigned int i, j;

  	MMU_Init(0);
 	sigture_address = (uint32)FDL2_signature;

#if defined(CHIP_ENDIAN_DEFAULT_LITTLE) && defined(CHIP_ENDIAN_BIG)    
	usb_boot(1);  
#endif

       FDL_PacketInit();

#ifdef CONFIG_SC8810	
	bss_start_start = _bss_start;
	bss_end_end = _bss_end;
	mem_malloc_init (&mempool[0], 1024*1024);
#else
	mem_malloc_init (_bss_end, CONFIG_SYS_MALLOC_LEN);	   
#endif	   
	   timer_init();
#ifndef CONFIG_SC8810
       sprd_clean_rtc();
#endif

//        FDL_SendAckPacket (BSL_REP_ACK);
	do {
#ifdef CONFIG_EMMC_BOOT		
		if(!FDL_BootIsEMMC()){
#endif			
			/* Initialize NAND flash. */
			err = nand_flash_init();
			if ((NAND_SUCCESS != err) && (NAND_INCOMPATIBLE_PART != err)) {
				FDL_SendAckPacket (convert_err (err));
				break;
			}
#ifdef CONFIG_EMMC_BOOT	
		}else{
			extern PARTITION_CFG g_sprd_emmc_partition_cfg[];
			extern int mmc_legacy_init(int dev);
			mmc_legacy_init(1);
			if(!FDL_Check_Partition_Table()){
//				write_uefi_parition_table(g_sprd_emmc_partition_cfg);
				FDL_SendAckPacket (convert_err (NAND_INCOMPATIBLE_PART));
			}
			err = EMMC_SUCCESS;
		}		
#endif               
		/* Register command handler */
		FDL_DlInit();
#ifdef CONFIG_EMMC_BOOT	
		if(FDL_BootIsEMMC()){
	  		FDL_DlReg(BSL_CMD_START_DATA,     FDL2_eMMC_DataStart,         0);
	   		FDL_DlReg(BSL_CMD_MIDST_DATA,     FDL2_eMMC_DataMidst,         0);
	   		FDL_DlReg(BSL_CMD_END_DATA,       FDL2_eMMC_DataEnd,           0);
	   		FDL_DlReg(BSL_CMD_READ_FLASH,     FDL2_eMMC_Read,         0);
	   		FDL_DlReg(BSL_ERASE_FLASH,        FDL2_eMMC_Erase,        0);
		    	FDL_DlReg(BSL_REPARTITION,    	   FDL2_eMMC_Repartition,       0);	
		}else
#endif		
		{
	  		FDL_DlReg(BSL_CMD_START_DATA,     FDL2_DataStart,         0);
	   		FDL_DlReg(BSL_CMD_MIDST_DATA,     FDL2_DataMidst,         0);
	   		FDL_DlReg(BSL_CMD_END_DATA,       FDL2_DataEnd,           0);
	   		FDL_DlReg(BSL_CMD_READ_FLASH,     FDL2_ReadFlash,         0);
	   		FDL_DlReg(BSL_ERASE_FLASH,        FDL2_EraseFlash,        0);
		    	FDL_DlReg(BSL_REPARTITION,    	   FDL2_FormatFlash,       0);	
		}
   		FDL_DlReg(BSL_CMD_NORMAL_RESET,   FDL_McuResetNormal/*mcu_reset_boot*/,   0);
	    	FDL_DlReg(BSL_CMD_READ_CHIP_TYPE, FDL_McuReadChipType, 0);  
			
		/* Reply the EXEC cmd received in the 1st FDL. */
		FDL_SendAckPacket (NAND_SUCCESS == err ? BSL_REP_ACK :
                           		BSL_INCOMPATIBLE_PARTITION);

        /* Start the download process. */
        FDL_DlEntry (DL_STAGE_CONNECTED);

    } while (0);

    /* If we get here, there must be something wrong. */
   	fdl_error();
   	return 0;
}

