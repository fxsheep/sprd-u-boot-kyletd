/*
 *   gcc img2data.c -o img2data
 *   make_ext4fs -s -l 5M -a productinfo /home/apuser/productinfo.img
 *   ./img2data /home/apuser/productinfo.img ../inc/phasecheck.h
 */

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
        const char *filename, *outname;
	int fd;
	FILE *fp;
	struct stat stat_buf;	
	int total, ii, recv, valdata;
	unsigned long offset;

	if (argc != 3) {
		printf("example : ./img2data /home/apuser/productinfo.img ../inc/myphasecheck.h\n");
		return 0;
	}

        filename = argv[1];
	outname = argv[2];
	memset(&stat_buf, 0, sizeof(struct stat));
	if (access(filename, 0) == 0)
		stat(filename, &stat_buf);
	else {
		stat_buf.st_size = 0;
		return 0;
	}

	fd = open(filename, O_RDWR);
	if (fd < 0)
		printf("open input file : %s error\n", filename);
	fp = fopen(outname, "w");
	if (fp == NULL) {
		close(fd);
		printf("open output file : %s error\n", filename);
		return 0;
	}

	/* output header */
	fprintf(fp, "\nstruct ext4_off_data {\n");
	fprintf(fp, "      unsigned long off;\n");
	fprintf(fp, "      unsigned char data;\n");
	fprintf(fp, "};\n\n");
	fprintf(fp, "struct ext4_off_data ext4_pattern[] = {\n");

	total = stat_buf.st_size;
	recv = 0;
	valdata = 0;
	while (total > 0) {
		memset(gbuffer, 0, BUFFER_LEN);
		read(fd, gbuffer, BUFFER_LEN);
		for (ii = 0; ii < BUFFER_LEN; ii ++) {
			if (gbuffer[ii] != 0) {
				//printf("{0x%08x, 0x%02x},", (recv + ii), (gbuffer[ii] & 0xff));
				fprintf(fp, "{0x%08x, 0x%02x},", (recv + ii), (gbuffer[ii] & 0xff));
				valdata ++;
			}
		}
		total -= BUFFER_LEN;
		recv += BUFFER_LEN;
	}
	
	offset = ftell(fp);
	//printf("\nvaldata = %d  offset = %ld\n", valdata, offset);
	fseek(fp, offset - 1,SEEK_SET);
	fprintf(fp, "\n};\n\n");
	close(fd);
	fclose(fp);
	
        return 1;
}
