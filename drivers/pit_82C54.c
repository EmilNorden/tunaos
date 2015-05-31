#include "pit_82C54.h"
#include "../kernel/low_level.h"
#include <stdint.h>
#include "../kernel/util.h"
#include "screen.h"

#define PIT_DATA0			0x40
#define PIT_DATA1			0x41
#define PIT_DATA2			0x42
#define PIT_COMMAND			0x43

void pit_init()
{
	
}

void pit_set_counter(PIT_BCD format, PIT_MODE mode, PIT_READWRITE rw, PIT_SC select_counter)
{
	uint8_t control_word = format | mode | rw | select_counter;
	
	// DEBUG: print control_word
	char *buf = "    ";
	int_to_string(control_word, buf);
	print("control_word: ");
	print(buf);
	
	int divisor = 1193180 / 100;
	port_byte_out(PIT_COMMAND, control_word);
	port_byte_out(PIT_DATA0, divisor & 0xFF);
	port_byte_out(PIT_DATA0, 0x00 >> 8);
}		