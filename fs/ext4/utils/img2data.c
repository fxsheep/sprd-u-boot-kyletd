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

#define BUFFER_LEN	(512)
char gbuffer[BUFFER_LEN];

int main(int argc, char **argv)
{
        const char *filename;
	int fd;
	struct stat stat_buf;	
	int total, ii, recv, valdata;

	if (argc != 2) {
		printf("example : check_ext4fs_data abc.img\n");
		return 0;
	}

        filename = argv[1];
	memset(&stat_buf, 0, sizeof(struct stat));
	if (access(filename, 0) == 0)
		stat(filename, &stat_buf);
	else {
		stat_buf.st_size = 0;
		return 0;
	}

	fd = open(filename, O_RDWR);
	if (fd < 0)
		printf("open file : %s error\n", filename);
	total = stat_buf.st_size;
	//printf("%s length is %d KB\n", filename, total / 1024);

	recv = 0;
	valdata = 0;
	while (total > 0) {
		memset(gbuffer, 0, BUFFER_LEN);
		read(fd, gbuffer, BUFFER_LEN);
		for (ii = 0; ii < BUFFER_LEN; ii ++) {
			if (gbuffer[ii] != 0) {
				printf("{0x%08x, 0x%02x},", (recv + ii), (gbuffer[ii] & 0xff));
				valdata ++;
			}
		}
		total -= BUFFER_LEN;
		recv += BUFFER_LEN;
	}
	printf("\n");
	close(fd);
	
        return 1;
}