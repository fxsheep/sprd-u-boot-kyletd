#include <config.h>
#include <common.h>
#include <command.h>
#include <linux/types.h>

#define COMMAND_MAX 128
int do_cboot(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	if(argc > 2)
	  goto usage;

	if(argc == 1){
		normal_mode();
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

usage:
	cmd_usage(cmdtp);
	return 1;
}

U_BOOT_CMD(
	cboot, CONFIG_SYS_MAXARGS, 1, do_cboot,
	"choose boot mode",
	"mode: \nrecovery, fastboot, dloader, charge, normal.\n"
	"cboot could enter a mode specified by the mode descriptor.\n"
	"it also could enter a proper mode automatically depending on "
	"the environment\n"
);
