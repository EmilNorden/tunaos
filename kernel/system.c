#include "system.h"
#include "interrupts.h"
#include "util.h"
#include "../drivers/screen.h"

void panic(const char *file, const char *function, int line, const char *message)
{
	irq_disable();	
	char *line_as_string = "     ";
	int_to_string(line, line_as_string);
	unsigned char print_attribute = COLOR_ATTR(BRIGHT(COLOR_LIGHTGRAY), COLOR_RED);
	clear_screen_clr(print_attribute);
	
	print_clr("================================ Kernel Panic!! ================================\n\n", print_attribute);
	print_clr("The kernel encountered an error from which it could not recover. This could be due to hardware issues, but is most likely due to a kernel bug.\n\n", print_attribute);
	
	char line_buffer[256];
	snprintf(line_buffer, 256, "Location: %s:%d (in function '%s')\n", 3, file, line, function);
	print_clr(line_buffer, print_attribute);
	
	if(message) {
		print_clr("Message: ", print_attribute);
		print_clr(message, print_attribute);
	}
	
	__asm__ __volatile__("hlt");
}