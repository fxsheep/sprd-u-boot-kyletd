
#include "phasecheck.h"

extern struct ext4_off_data ext4_pattern[];

int make_ext4fs_main(unsigned char *buffer, int partlen)
{
        int ii;
	int array = sizeof(ext4_pattern) / sizeof(struct ext4_off_data);

	memset(buffer, 0, partlen);
	for (ii = 0; ii < array; ii ++)
		*(buffer + ext4_pattern[ii].off) = ext4_pattern[ii].data;

        return partlen;
}
