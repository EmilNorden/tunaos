/* 
	Driver for the 8259 Programmable Interval Timer
	Compatible with HMOS 8254, and is a superset of 8253
*/

#ifndef TIMER_H_
#define TIMER_H_

typedef enum { PIT_BCD_OFF = 0, PIT_BCD_ON } PIT_BCD;
typedef enum 
{
	PIT_MODE_0 				= 0 << 1,	// Interrupt on terminal count
	PIT_MODE_1 				= 1 << 1,	// Hardware re-triggerable one-shot
	PIT_MODE_2 				= 2 << 1,	// Rate generator
	PIT_MODE_3 				= 3 << 1,	// Square wave generator
	PIT_MODE_4 				= 4 << 1,	// Software triggered strobe
	PIT_MODE_5 				= 5 << 1	// Hardware triggered strobe
} PIT_MODE;

typedef enum
{
	PIT_RW_COUNTER_LATCH 	= 0 << 4,
	PIT_RW_LSB				= 1 << 4, 
	PIT_RW_MSB 				= 2 << 4, 
	PIT_RW_LSB_MSB 			= 3 << 4
} PIT_READWRITE;

typedef enum
{
	PIT_SC_0				= 0 << 6,
	PIT_SC_1				= 1 << 6,
	PIT_SC_2				= 2 << 6,
	PIT_SC_READBACK			= 3 << 6
} PIT_SC;

void pit_initialize(void);

void pit_set_counter(PIT_BCD format, PIT_MODE mode, PIT_READWRITE rw, PIT_SC select_counter);

#endif /* TIMER */