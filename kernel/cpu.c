#include "cpu.h"

extern void asm_get_cpu_info(struct cpu_info *info);

void get_cpu_info(struct cpu_info *info)
{
	asm_get_cpu_info(info);
	info->vendor_id[12] = 0;
}