#include <config.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#ifdef CONFIG_SYS_HUSH_PARSER
#include <hush.h>
#endif
#include <linux/string.h>
#define COMMAND_MAX 128

void normal_mode(void)
{
	char buf[COMMAND_MAX] = {0};
	int cur_len = 0;

	buf[0]='\0';;
	strcat(buf, "nand read ");
	cur_len = strlen(buf);
//	sprintf(&buf[cur_len],"%x %s\;\0", 0x3000000,"ramdisk");
	sprintf(&buf[cur_len], "%x %s %x\;\0", 0x3000000, "ramdisk", 0x400000);
	
	strcat(buf, "nand read ");
	cur_len = strlen(buf);
	sprintf(&buf[cur_len], "%x %s %x\;\0", 0x1000000, "kernel", 0x400000);


	cur_len = strlen(buf);
	sprintf(&buf[cur_len], "%s %x\0", "bootm ", 0x1000000);

	setenv("normal_env", buf);
	printf("\n %s\n", buf);

#ifdef CONFIG_SYS_HUSH_PARSER
	parse_string_outer("run normal_env",FLAG_PARSE_SEMICOLON |FLAG_EXIT_FROM_LOOP);
#else 
	run_command ("run normal_env",0);
#endif
}
