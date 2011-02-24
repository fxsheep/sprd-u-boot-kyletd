#include <asm/arch/sci_types.h>
#include <asm/arch/sc_reg.h>
#include <asm/arch/adi_hal_internal.h>
#include <boot_mode.h>

unsigned check_reboot_mode(void)
{
    unsigned rst_mode= 0;
    rst_mode = ANA_REG_GET(ANA_HWRST_STATUS);
    rst_mode &= HWRST_STATUS_POWERON_MASK;
    if(rst_mode == HWRST_STATUS_RECOVERY)
      return RECOVERY_MODE;
    else if(rst_mode == HWRST_STATUS_FASTBOOT)
      return FASTBOOT_MODE;
}

void reboot_devices(unsigned reboot_mode)
{
    unsigned rst_mode = 0;
    if(reboot_mode == RECOVERY_MODE){
      rst_mode = HWRST_STATUS_RECOVERY;
    }
    else if(reboot_mode == FASTBOOT_MODE){
      rst_mode = HWRST_STATUS_FASTBOOT;
    }
    else{
      rst_mode = 0;
    }

    ANA_REG_SET(ANA_HWRST_STATUS, rst_mode);
    reset_cpu(0);
#if 0
    asm volatile("ldr r1,=0x20900218"); //remap ROM to 0x0000_0000
    asm volatile("ldr r2,=1");
    asm volatile("str r2,[r1]");
    asm volatile("mov pc,#0");
#endif
}

