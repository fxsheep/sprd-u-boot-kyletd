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

extern  const unsigned char FDL2_signature[][24];

static void error (void)
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

#if 0
	/* while(1); */
	volatile unsigned int cnt=0;
	for(i=0;i<10  ;i++)
	  for(j=0; j<1005528;j++)
	   cnt++;
#endif
	sigture_address = (uint32)FDL2_signature;

#if defined(CHIP_ENDIAN_DEFAULT_LITTLE) && defined(CHIP_ENDIAN_BIG)    
    usb_boot(1);  
#endif

       FDL_PacketInit();
	   mem_malloc_init (_bss_end, CONFIG_SYS_MALLOC_LEN);
	   timer_init();

//        FDL_SendAckPacket (BSL_REP_ACK);
	do {
		/* Initialize NAND flash. */
		err = nand_flash_init();
		if ((NAND_SUCCESS != err) && (NAND_INCOMPATIBLE_PART != err)) {
            FDL_SendAckPacket (convert_err (err));
            break;
        }
   		/* Register command handler */
   		FDL_DlInit();

  		FDL_DlReg(BSL_CMD_START_DATA,     FDL2_DataStart,         0);
   		FDL_DlReg(BSL_CMD_MIDST_DATA,     FDL2_DataMidst,         0);
   		FDL_DlReg(BSL_CMD_END_DATA,       FDL2_DataEnd,           0);
   		FDL_DlReg(BSL_CMD_READ_FLASH,     FDL2_ReadFlash,         0);
   		FDL_DlReg(BSL_ERASE_FLASH,        FDL2_EraseFlash,        0);
   		FDL_DlReg(BSL_CMD_NORMAL_RESET,   FDL_McuResetNormal/*mcu_reset_boot*/,   0);
	    	FDL_DlReg(BSL_CMD_READ_CHIP_TYPE, FDL_McuReadChipType, 0);  
	    	FDL_DlReg(BSL_REPARTITION,    	   FDL2_FormatFlash,       0);	
			
		/* Reply the EXEC cmd received in the 1st FDL. */
        FDL_SendAckPacket (NAND_SUCCESS == err ? BSL_REP_ACK :
                           BSL_INCOMPATIBLE_PARTITION);

        /* Start the download process. */
        FDL_DlEntry (DL_STAGE_CONNECTED);

    }
    while (0);

    /* If we get here, there must be something wrong. */
   	error();
   	return 0;
}
