#include "low_level.h"

unsigned char port_byte_in(unsigned short port) {
	/*	"=a" (result) means: put AL register in variable RESULT when finished
		"d" (port) means: load EDX with port */
	
	unsigned char result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void port_byte_out(unsigned short port, unsigned char data) {
	/*	"a" (data) means: load EAX with data
	 	"d" (port) means: load EDX with port */
	__asm__("out %%al, %%dx" :  :"a" (data), "d" (port));
}

unsigned short port_word_in(unsigned short port) {
	/*	"=a" (result) means: put AL register in variable RESULT when finished
		"d" (port) means: load EDX with port */
	
	unsigned short result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void port_word_out(unsigned short port, unsigned short data) {
	/*	"a" (data) means: load EAX with data
		"d" (port) means: load EDX with port */
	__asm__("out %%al, %%dx" :  :"a" (data), "d" (port));
}

void io_wait(void)
{
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    __asm__ volatile ( "outb %%al, $0x80" : : "a"(0) );
    /* TODO: Is there any reason why al is forced? */
}