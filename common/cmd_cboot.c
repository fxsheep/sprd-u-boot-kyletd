#include <config.h>
#include <common.h>
#include <command.h>
#include <linux/types.h>
#include <android_bootimg.h>
#include <linux/keypad.h>
#include <linux/key_code.h>
#include <boot_mode.h>

#define COMMAND_MAX 128

//#define DEBUG
#ifdef DEBUG
#define DBG(fmt...) printf(fmt)
#else
#define DBG(fmt...) 
#endif

extern int power_button_pressed(void);
extern int charger_connected(void);
extern int alarm_triggered(void);
#define mdelay(_ms) udelay(_ms*1000)

int recheck_power_button(void)
{
    int cnt = 0;
    int ret = 0;
    do{
        ret = power_button_pressed();
        if(ret == 0)
          cnt++;
        else
          return 1;

        if(cnt>4)
          return 0;
        else{
            mdelay(1);
        }
    }while(1);
}

int do_cboot(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
    uint32_t key_mode = 0;
    uint32_t key_code = 0;
    volatile int i;

    if(argc > 2)
      goto usage;

    board_keypad_init();

    unsigned check_reboot_mode(void);
    unsigned rst_mode= check_reboot_mode();
    if(rst_mode == RECOVERY_MODE){
        DBG("func: %s line: %d\n", __func__, __LINE__);
        recovery_mode();
    }
    else if(rst_mode == FASTBOOT_MODE){
        DBG("func: %s line: %d\n", __func__, __LINE__);
        fastboot_mode();
    }else if(rst_mode == NORMAL_MODE){
        normal_mode();
    }
    DBG("func: %s line: %d\n", __func__, __LINE__);

    int recovery_init(void);
    int ret =0;
    ret = recovery_init();
    if(ret == 1){
        DBG("func: %s line: %d\n", __func__, __LINE__);
        recovery_mode();
    }

    //find the power up trigger
    if(!recheck_power_button()){
        DBG("%s: power button press\n", __FUNCTION__);

        //go on to check other keys
        mdelay(50);
        for(i=0; i<10;i++){
            key_code = board_key_scan();
            if(key_code != KEY_RESERVED)
              break;
        }
        key_mode = check_key_boot(key_code);

        switch(key_mode){
            case BOOT_FASTBOOT:
                fastboot_mode();
                break;
            case BOOT_RECOVERY:
                recovery_mode();
                break;
            case BOOT_CALIBRATE:
                engtest_mode();
                return; //back to normal boot
                break;
            case BOOT_DLOADER:
                dloader_mode();
                break;
            default:
                break;
        }
    }else if(charger_connected()){
        DBG("%s: charger connected\n", __FUNCTION__);
        charge_mode();
    }else if(alarm_triggered()){
        DBG("%s: alarm triggered\n", __FUNCTION__);
        alarm_mode();
    }else{
        calibration_detect(0);
        //if calibrate success, it will here
        DBG("%s: power done again\n", __FUNCTION__);
        power_down_devices();
        while(1)
          ;
    }

    if(argc == 1){
        normal_mode();
        return 1;
    }

    if(argc == 2){
        DBG("func: %s line: %d\n", __func__, __LINE__);

        if(strcmp(argv[1],"normal") == 0){
            normal_mode();
            return 1;
        }
        DBG("func: %s line: %d\n", __func__, __LINE__);

        if(strcmp(argv[1],"recovery") == 0){
            recovery_mode();
            return 1;
        }
        DBG("func: %s line: %d\n", __func__, __LINE__);

        if(strcmp(argv[1],"fastboot") == 0){
            fastboot_mode();
            return 1;
        }
        DBG("func: %s line: %d\n", __func__, __LINE__);

        if(strcmp(argv[1],"dloader") == 0){
            dloader_mode();
            return 1;
        }
        DBG("func: %s line: %d\n", __func__, __LINE__);

        if(strcmp(argv[1],"charge") == 0){
            charge_mode();
            return 1;
        }
        DBG("func: %s line: %d\n", __func__, __LINE__);

        if(strcmp(argv[1],"caliberation") == 0){
            calibration_detect(1);
            return 1;
        }
        DBG("func: %s line: %d\n", __func__, __LINE__);
    }
    DBG("func: %s line: %d\n", __func__, __LINE__);

usage:
    cmd_usage(cmdtp);
    return 1;
}

U_BOOT_CMD(
            cboot, CONFIG_SYS_MAXARGS, 1, do_cboot,
            "choose boot mode",
            "mode: \nrecovery, fastboot, dloader, charge, normal, vlx, caliberation.\n"
            "cboot could enter a mode specified by the mode descriptor.\n"
            "it also could enter a proper mode automatically depending on "
            "the environment\n"
          );
