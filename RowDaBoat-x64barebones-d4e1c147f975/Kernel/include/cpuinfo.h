#ifndef CPUINFO_H

#define CPUINFO_H

typedef struct{
	char vendor[13];
	char brand[48];
	int family;
	int model;
}	CpuInfo;

uint64_t sys_cpuinfo(CpuInfo* info);

#endif
