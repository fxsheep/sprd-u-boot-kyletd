#include <config.h>
#include <common.h>
#include <command.h>
#include <linux/types.h>

int do_cboot(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{


}

U_BOOT_CMD(
	cboot, CONFIG_SYS_MAXARGS, 1, do_cboot,
	"choose boot mode",
	"mode: \nrecovery, fastboot, dloader, charge, normal.\n"
	"cboot could enter a mode specified by the mode descriptor.\n"
	"it also could enter a proper mode automatically depending on "
	"the environment\n"
);
