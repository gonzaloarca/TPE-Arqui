#include <libasm64.h>
#include <stdint.h>

uint64_t sys_cpuinfo(char buffer[70])
{
	cpuVendor(buffer);

	buffer[13] = buffer[15] = ' ';
	buffer[14] = '-';

	cpuBrand(&(buffer[16]));

	return (uint64_t) buffer;
}
