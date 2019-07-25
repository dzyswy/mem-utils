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




int main(int argc, char *argv[])
{
	unsigned long page_size;
	unsigned long base_phys, ddr_phys;
	void *base_virt, *ddr_virt;
	unsigned long map_size, offset_size;
	
	if (argc != 2)
	{
		printf("usage: %s ddr_phys\n", argv[0]);
		return -1;
	}	
	
	ddr_phys = strtol(argv[1], NULL, 16);
	
	int fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd < 0) {
		printf("Failed to open /dev/mem\n");
		return -1;
	}
	
	page_size = sysconf(_SC_PAGESIZE);
	base_phys = ddr_phys & (~(page_size - 1));
	offset_size = ddr_phys & (page_size - 1);
	map_size = page_size;
	
	base_virt = mmap(0, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, base_phys);
	if (base_virt == MAP_FAILED) {
		printf("Failed to mmap!\n");
		return -1;
	}
	
	ddr_virt = (void *)((unsigned long)base_virt + offset_size);
	
	printf("0x%lx: 0x%lx\n", ddr_phys, *((volatile uint32_t *)ddr_virt));
	
	munmap(base_virt, map_size);
	close(fd);
	return 0;
}





