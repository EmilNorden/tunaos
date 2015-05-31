#ifndef KERNEL_LOW_LEVEL_H_
#define KERNEL_LOW_LEVEL_H_

/* Read a byte from specified port */
unsigned char port_byte_in(unsigned short port);

/* Writes a byte to specified port */
void port_byte_out(unsigned short port, unsigned char data);

/* Read a word from specified port */
unsigned short port_word_in(unsigned short port);

/* Writes a word to specified port */
void port_word_out(unsigned short port, unsigned short data);

void io_wait(void);

#endif