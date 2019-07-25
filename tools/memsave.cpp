#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>


int save_raw(char *file_name, char *buf, unsigned long len)
{
	int ret;
	FILE *fp = fopen(file_name, "wb");
	if (fp == NULL) {
		printf("Failed to open: %s\n", file_name);
		return -1;
	}
	
	ret = fwrite(buf, 1, len, fp);
	if (ret < 0) {
		printf("Failed to write file\n");
		fclose(fp);
		return -1;
	}
	
	fclose(fp);
	return 0;	
}


int main(int argc, char *argv[])
{
	int ret;
	unsigned long page_size;
	unsigned long base_phys, ddr_phys;
	void *base_virt, *ddr_virt;
	unsigned long map_size, offset_size;
	unsigned long len;
	
	if (argc != 4)
	{
		printf("usage: %s ddr_phys *.raw len\n", argv[0]);
		return -1;
	}	
	
	ddr_phys = strtol(argv[1], NULL, 16);
	char *raw_name = strdup(argv[2]);
	len = strtol(argv[3], NULL, 16);
	
	
	int fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd < 0) {
		printf("Failed to open /dev/mem\n");
		return -1;
	}
	
	page_size = sysconf(_SC_PAGESIZE);
	base_phys = ddr_phys & (~(page_size - 1));
	offset_size = ddr_phys & (page_size - 1);
	map_size = (unsigned long)(ceil((double)len / page_size) * page_size);
	
	base_virt = mmap(0, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, base_phys);
	if (base_virt == MAP_FAILED) {
		printf("Failed to mmap!\n");
		return -1;
	}
	
	ddr_virt = (void *)((unsigned long)base_virt + offset_size);
	
	ret = save_raw(raw_name, (char *)ddr_virt, len);
	if (ret == 0) {
		printf("save ok!\n");
	}
	
	munmap(base_virt, map_size);
	close(fd);
	return 0;
}





