#include "pic.h"
#include "../kernel/low_level.h"
#include <stdint.h>

#define PIC1			0x20
#define PIC2			0xA0
#define PIC1_COMMAND	PIC1
#define PIC1_DATA		(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA		(PIC2+1)

#define PIC_EOI			0x20 			// End-of-interrupt command

/* 
	http://pdos.csail.mit.edu/6.828/2005/readings/hardware/8259A.pdf
	Refer to page 11 for detailed info on these bitfields
*/
// ICW = Initialization Command Words
#define ICW1_ICW4		0x01			// ICW4 (not) needed
#define ICW1_SINGLE		0x02			// Single (cascade) mode
#define ICW1_INTERVAL4	0x04			// Call address interval 4 (8)
#define ICW1_LEVEL		0x08			// Level triggered (edge) mode
#define ICW1_INIT		0x10			// Initialization - required

#define ICW4_8086		0x01			// 8086/88 (MCS-80/85) mode
#define ICW4_AUTO		0x02			// Auto (normal) EOI
#define ICW4_BUF_SLAVE	0x08			// Buffered mode/slave
#define ICW4_BUF_MASTER	0x0C			// BUffered mode/master
#define ICW4_SFNM		0x10			// (not) special fully nested mode

void pic_remap(void);


void pic_initialize(void)
{
	pic_remap();
	/* When we initialize the pic, we change their default vector offsets (0x08 for master PIC, 0x70 for slave PIC)
	 so that they do not conflict with the reserved CPU exceptions. Also known as remapping the PIC
	*/ 
}

void pic_remap(void)
{
	unsigned char master_masks, slave_masks;
	
	master_masks = port_byte_in(PIC1_DATA);					// Save masks
	slave_masks = port_byte_in(PIC2_DATA);
	
	// Starts the initialization sequence (in cascade mode)
	port_byte_out(PIC1_COMMAND, ICW1_INIT + ICW1_ICW4);		
	io_wait();
	port_byte_out(PIC2_COMMAND, ICW1_INIT + ICW1_ICW4);
	io_wait();
	
	// Set start of hardware interrupts (0x20 for master, 0x28 for slave)
	// TODO: Make these into #defines later
	port_byte_out(PIC1_DATA, 0x20);				
	io_wait();
	port_byte_out(PIC2_DATA, 0x28);
	io_wait();
	
	// Tell master PIC that there is a slave PIC at IRQ2 (0000 0100)
	port_byte_out(PIC1_DATA, 0x04);							
	io_wait();
	
	// Tell slave PIC its cascade identity (0000 0010)
	port_byte_out(PIC2_DATA, 0x02);							
	io_wait();
	
	// Set 8086 mode
	port_byte_out(PIC1_DATA, ICW4_8086);					
	io_wait();
	port_byte_out(PIC2_DATA, ICW4_8086);
	io_wait();
	
	port_byte_out(PIC1_DATA, 0);					// Restore saved masks // DEBUG: set masks to 0 always
	port_byte_out(PIC2_DATA, 0);
}

void pic_send_eoi(unsigned char irq)
{
	if(irq >= 8) // If the IRQ came from the slave PIC, send the EOI to the slave aswell.
		port_byte_out(PIC2_COMMAND, PIC_EOI);
		
	port_byte_out(PIC1_COMMAND, PIC_EOI);
}

void pic_set_mask(unsigned char irq)
{
	pic_type dest;
	unsigned char mask;
	
	if(irq >= 8) {
		dest = PIC_SLAVE;
		irq -= 8;
	}
	else {
		dest = PIC_MASTER;
	}
	
	mask |= (1<<irq) | pic_get_mask(dest);
	port_byte_out((short)dest, mask);
}

void pic_clear_mask(unsigned char irq)
{
	pic_type dest;
	
	if(irq >= 8)  {
		dest = PIC_SLAVE;
		irq -= 8;
	}
	else {
		dest = PIC_MASTER;
	}
	unsigned char mask = pic_get_mask(dest) & ~(1<<irq);
	
	port_byte_out((short)dest, mask);
}

unsigned char pic_get_mask(pic_type type)
{
	return port_byte_in((short)type);
}