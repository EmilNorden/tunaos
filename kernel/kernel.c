#include "../drivers/screen.h"
#include "../drivers/pit_82C54.h"
#include "interrupts.h"
#include <stdint.h>
#include "util.h"
inline void cpuid(int code, uint32_t *a, uint32_t *d)
{
	__asm volatile ("cpuid" : "=a"(*a), "=d"(*d) : "0"(code) : "ebx", "ecx");
	
}


void main(void)
{
	clear_screen();
	print("Booting potatismOS 0.0.1-pre-alpha...\n");
	print("Init IRQ subsystem...");
	irq_initialize();
	print("done\n");
	
	for(;;) {
		__asm__ __volatile__("hlt");
	}
}
