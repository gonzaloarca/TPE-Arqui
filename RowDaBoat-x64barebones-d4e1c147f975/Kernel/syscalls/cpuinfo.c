#include <libasm64.h>
#include <stdint.h>
#include <cpuinfo.h>

uint64_t sys_cpuinfo(CpuInfo* info)
{
	cpuVendor(info->vendor);
	cpuBrand(info->brand);
	cpuModel(&(info->family));

	return (uint64_t) info;
}
