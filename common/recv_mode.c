#include <config.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#include <image.h>
#ifdef CONFIG_SYS_HUSH_PARSER
#include <hush.h>
#endif
#include <linux/string.h>
#define COMMAND_MAX 128

void recovery_mode(void)
{
	char buf[COMMAND_MAX] = {0};
	int cur_len = 0;
	unsigned kernel_dest = 0;

	buf[0]='\0';;
	
	strcat(buf, "nand read ");
	cur_len = strlen(buf);
	kernel_dest = 0x8000 - sizeof(image_header_t);
	sprintf(&buf[cur_len], "%x %s %x\;\0", kernel_dest, "kernel", 0x400000);


	cur_len = strlen(buf);
	sprintf(&buf[cur_len], "%s %x\0", "bootm ", kernel_dest);

	setenv("recv_env", buf);
	printf("\n %s\n", buf);

#ifdef CONFIG_SYS_HUSH_PARSER
	parse_string_outer("run recv_env",FLAG_PARSE_SEMICOLON |FLAG_EXIT_FROM_LOOP);
#else 
	run_command ("run recv_env",0);
#endif
}
