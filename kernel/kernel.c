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
	
	// Initialize devices, system timer first?
	//print("Init system timer...");
	pit_set_counter(PIT_BCD_OFF, PIT_MODE_2, PIT_RW_LSB_MSB, PIT_SC_0);
	//print("done");
	
	for(;;){}
}
