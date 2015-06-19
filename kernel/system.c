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
	
	// TODO: Once i have implemented something similar to sprintf or printf, rewrite this printing-mess.
	print_clr("The kernel encountered an error from which it could not recover. This could be due to hardware issues, but is most likely due to a kernel bug.\n\n", print_attribute);
	print_clr("Location: ", print_attribute);
	print_clr(file, print_attribute);
	print_clr(":", print_attribute);
	print_clr(line_as_string, print_attribute);
	print_clr(" (in function '", print_attribute);
	print_clr(function, print_attribute);
	print_clr("')\n", print_attribute);
	
	if(message) {
		print_clr("Message: ", print_attribute);
		print_clr(message, print_attribute);
	}
	
	__asm__ __volatile__("hlt");
}