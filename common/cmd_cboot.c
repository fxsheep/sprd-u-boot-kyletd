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

int do_cboot(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	uint32_t key_code = 0;
    volatile int i;

    if(argc > 2)
	  goto usage;
    //find the power up trigger
    if(!power_button_pressed()){
        DBG("%s: power button press\n", __FUNCTION__);
        //do nothing, let it down
    }else if(charger_connected()){
        DBG("%s: charger connected\n", __FUNCTION__);
        charge_mode();
    }else if(alarm_triggered()){
        DBG("%s: alarm triggered\n", __FUNCTION__);
        alarm_mode();
    }else{
        DBG("%s: power done again\n", __FUNCTION__);
        power_down_devices();
    }


    board_keypad_init();
    for(i=0; i<0x50000;i++){
        key_code = board_key_scan();
        if(key_code != KEY_RESERVED)
          break;
    }

    switch(key_code){
    case KEY_MENU:
        fastboot_mode();
        break;
    case KEY_HOME:
        recovery_mode();
        break;
    case KEY_CAMERA:
        charge_mode();
        return; //back to normal boot
        break;
    case KEY_SEND:
        dloader_mode();
        break;
    default:
        break;
    }

    unsigned check_reboot_mode(void);
    unsigned rst_mode= check_reboot_mode();
    if(rst_mode == RECOVERY_MODE)
      recovery_mode();
    else if(rst_mode == FASTBOOT_MODE)
      fastboot_mode();

    int recovery_init(void);
    int ret =0;
    ret = recovery_init();
    if(ret == 1)
      recovery_mode();
	
	if(argc == 1){
		normal_mode();
		return 1;
	}

    if(argc == 2){
	if(strcmp(argv[1],"vlx") == 0){
            vlx_mode();
            return 1;
        }

        if(strcmp(argv[1],"normal") == 0){
            normal_mode();
            return 1;
        }
        
        if(strcmp(argv[1],"recovery") == 0){
            recovery_mode();
            return 1;
        }

        if(strcmp(argv[1],"fastboot") == 0){
            fastboot_mode();
            return 1;
        }

        if(strcmp(argv[1],"dloader") == 0){
            dloader_mode();
            return 1;
        }

        if(strcmp(argv[1],"charge") == 0){
            charge_mode();
            return 1;
        }
    }

usage:
	cmd_usage(cmdtp);
	return 1;
}

U_BOOT_CMD(
	cboot, CONFIG_SYS_MAXARGS, 1, do_cboot,
	"choose boot mode",
	"mode: \nrecovery, fastboot, dloader, charge, normal, vlx.\n"
	"cboot could enter a mode specified by the mode descriptor.\n"
	"it also could enter a proper mode automatically depending on "
	"the environment\n"
);
