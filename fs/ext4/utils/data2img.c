/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//  gcc check_ext4fs_data.c -o check_ext4fs_data

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <string.h>
#include <mtd/mtd-user.h>

#include "data2img.h"

#define BUFFER_LEN	(3840 * 1024)
char gbuffer[BUFFER_LEN];

extern struct ext4_off_data ext4_pattern[];

static int make_ext4fs_main(unsigned char *buffer, int partlen)
{
        int ii;
	int array = sizeof(ext4_pattern) / sizeof(struct ext4_off_data);

	memset(buffer, 0, partlen);
	for (ii = 0; ii < array; ii ++)
		*(buffer + ext4_pattern[ii].off) = ext4_pattern[ii].data;

        return partlen;
}

int main(int argc, char **argv)
{
        const char *filename;
	int fd;

	if (argc != 2) {
		printf("example : data2img abc.img\n");
		return 0;
	}

        filename = argv[1];
	fd = open(filename, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd < 0)
		printf("open file : %s error\n", filename);

	make_ext4fs_main(gbuffer, BUFFER_LEN);
	write(fd, gbuffer, BUFFER_LEN);

	printf("\n");
	close(fd);
	
        return 1;
}
