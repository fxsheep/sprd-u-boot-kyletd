#include <asm/arch/sci_types.h>
#include "fdl_main.h"
#include <asm/arch/cmd_def.h>
#include <asm/arch/packet.h>
#include <asm/arch/dl_engine.h>
#include <asm/arch/sio_drv.h>
#include "mcu_command.h"
#include <asm/arch/usb_boot.h>
#include <asm/arch/dma_drv_fdl.h>
#include <asm/arch/sc_reg.h>

#ifdef CONFIG_EMMC_BOOT
#include "fdl_emmc.h"
#else
#include "fdl_nand.h"
#include "flash_command.h"
#endif

extern  const unsigned char FDL2_signature[][24];
extern int sprd_clean_rtc(void);

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
#ifdef FPGA_TRACE_DOWNLOAD
#define BIN_TABLE_ADDR	0x80a00000
typedef struct {
	unsigned int base_addr;//logic id
	unsigned int bin_addr; //bin address in dram
	unsigned int bin_size; //bin size(bytes)
	unsigned int var;
}bin_table_t;
#define WRITE_MAX_SIZE	(1024*8)
static void write_bin2flash()
{
	bin_table_t *pbin_table;
	int buf_size;
	int write_size;
	unsigned char *pbuf;
	uint32 begin_time,end_time;
	for(pbin_table = (bin_table_t *)BIN_TABLE_ADDR; pbin_table->base_addr; pbin_table += 1) {
		printf("write_bin2flash, base_addr=0x%x, bin_addr=0x%x,bin_size=%x\r\n",pbin_table->base_addr, pbin_table->bin_addr,pbin_table->bin_size);
		begin_time = SCI_GetTickCount();
#ifdef CONFIG_EMMC_BOOT
		fdl_emmc_dram_download(pbin_table->base_addr, pbin_table->bin_addr, pbin_table->bin_size);
#else
		FDL2_DramStart(pbin_table->base_addr, pbin_table->bin_size);
		pbuf = (unsigned char *)pbin_table->bin_addr;
		buf_size = pbin_table->bin_size;
		while(buf_size > 0) {
			write_size = (buf_size > WRITE_MAX_SIZE) ? WRITE_MAX_SIZE : buf_size;
			printf("write_bin2flash writesize = 0x%x, pbuf = 0x%x\r\n",write_size, pbuf);
			if(FDL2_DramMidst(pbuf, write_size) == 0){
				printf("write_bin2flash, 0x%x, write_size =0x%x\r\n",pbin_table->bin_addr, write_size);
				while(1); //fail
			}
			pbuf += write_size;
			buf_size  -= write_size;
		}
		FDL2_DramEnd();
#endif
		end_time = SCI_GetTickCount();
		printf("write_bin2flash, 0x%x, sucessfully, cost time %d s !\r\n",pbin_table->bin_addr, (end_time-begin_time)/1000);
	}
}
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
#if defined (CONFIG_TIGER) || defined(CONFIG_SC7710G2)
#else
       sprd_clean_rtc();

#endif
//        FDL_SendAckPacket (BSL_REP_ACK);
	do {
#ifdef CONFIG_EMMC_BOOT		
		if(FDL_BootIsEMMC()) {
			/* Initialize NAND flash. */
			extern PARTITION_CFG g_sprd_emmc_partition_cfg[];
			extern int mmc_legacy_init(int dev);
			mmc_legacy_init(1);
			if (!FDL_Check_Partition_Table()) {
  				#if defined (CONFIG_SC8825) || defined(CONFIG_SC7710G2) // JUST FOR TEST , DELETE IT LATER
				write_uefi_parition_table(g_sprd_emmc_partition_cfg);
				#else				
				FDL_SendAckPacket (convert_err (EMMC_INCOMPATIBLE_PART));
				#endif
			}
			err = EMMC_SUCCESS;
		}
#else
			err = nand_flash_init();
			if ((NAND_SUCCESS != err) && (NAND_INCOMPATIBLE_PART != err)) {
				FDL_SendAckPacket (convert_err (err));
				break;
			}
#endif

#ifdef FPGA_TRACE_DOWNLOAD
		if(!err)
		{
			write_bin2flash();
		}
		while(1);
#else
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
		}
#else
	  		FDL_DlReg(BSL_CMD_START_DATA,     FDL2_DataStart,         0);
	   		FDL_DlReg(BSL_CMD_MIDST_DATA,     FDL2_DataMidst,         0);
	   		FDL_DlReg(BSL_CMD_END_DATA,       FDL2_DataEnd,           0);
	   		FDL_DlReg(BSL_CMD_READ_FLASH,     FDL2_ReadFlash,         0);
	   		FDL_DlReg(BSL_ERASE_FLASH,        FDL2_EraseFlash,        0);
			FDL_DlReg(BSL_REPARTITION,    	   FDL2_FormatFlash,       0);
#endif
   		FDL_DlReg(BSL_CMD_NORMAL_RESET,   FDL_McuResetNormal/*mcu_reset_boot*/,   0);
	    	FDL_DlReg(BSL_CMD_READ_CHIP_TYPE, FDL_McuReadChipType, 0);  
#ifdef CONFIG_EMMC_BOOT
		FDL_SendAckPacket (EMMC_SUCCESS == err ? BSL_REP_ACK :
					BSL_INCOMPATIBLE_PARTITION);
#else
		/* Reply the EXEC cmd received in the 1st FDL. */
		FDL_SendAckPacket (NAND_SUCCESS == err ? BSL_REP_ACK :
					BSL_INCOMPATIBLE_PARTITION);
#endif
        /* Start the download process. */
        FDL_DlEntry (DL_STAGE_CONNECTED);
#endif
    } while (0);

    /* If we get here, there must be something wrong. */
   	fdl_error();
   	return 0;
}

