#include "system_clock.h"
#include "interrupts.h"
#include "../drivers/timer.h"
#include <stdint.h>

int ticks = 0;
int secs = 0;
void timer_handler(struct regs *r)
{
	ticks += 1;
	
	if(ticks % 100 == 0) {
		ticks = 0;
		secs++;
	}
}


void clock_init(void)
{
	irq_set_handler(0, timer_handler);
	pit_set_counter(PIT_BCD_OFF, PIT_MODE_3, PIT_RW_LSB_MSB, PIT_SC_0);	
}

int clock_get_uptime(void)
{
	return secs;
}