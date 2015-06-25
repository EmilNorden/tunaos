#ifndef CPU_H_
#define CPU_H_

struct __attribute__((packed)) cpu_info {
	char vendor_id[13];
};

void get_cpu_info(struct cpu_info *info);

#endif