#include "interrupts.h"
#include "../drivers/screen.h"
#include "low_level.h"
#include "util.h"
#include "isr.h"

struct  __attribute__((packed)) IDTEntry {
	uint16_t offset_lo;		// offset bits 0..15
	uint16_t selector;			// a code segment selector in GDT or LDT
	uint8_t zero;				// unused, set to 0
	uint8_t type 		: 5;	// Type
	uint8_t dpl			: 2;	// Descriptor Privilege Level.
	uint8_t present		: 1;	// 0 if descriptor is not used.	
	uint16_t offset_hi;			// offset bits 16..31
};

/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

/* 
	The first 32 are reserved for processor exceptions
	The next 16 can be used for hardware interrupts
	The remaining are available for software interrupts
*/
struct IDTEntry idt[256];

void irq_initialize_idt(void)
{
	struct
	{
		uint16_t length;
		uint32_t base;
	} __attribute__((packed)) IDTR;
	
	IDTR.length  = (sizeof(struct IDTEntry) * 256) - 1;
	IDTR.base = (uint32_t)&idt;
	__asm__("lidt (%0)" : : "p"(&IDTR));
	
	irq_set_interrupt_gate(0, (unsigned)_isr0, 0x08, 0);
	irq_set_interrupt_gate(1, (unsigned)_isr1, 0x08, 0);
	irq_set_interrupt_gate(2, (unsigned)_isr2, 0x08, 0);
	irq_set_interrupt_gate(3, (unsigned)_isr3, 0x08, 0);
	irq_set_interrupt_gate(4, (unsigned)_isr4, 0x08, 0);
}

void irq_initialize(void)
{
	irq_initialize_idt();
	pic_initialize();
	
	port_byte_out(0x21, 0xFF);
	port_byte_out(0xA1, 0xFF);
	
	pic_clear_mask(0);
	pic_clear_mask(1);
	pic_clear_mask(2);
	
	__asm__("sti");
}

void irq_set_interrupt_gate(uint32_t num, uint32_t base, uint16_t selector, uint8_t dpl)
{
	idt[num].offset_lo = base & 0xFFFF;
	idt[num].offset_hi = (base >> 16) & 0xFFFF;
	idt[num].zero = 0;
	idt[num].selector = selector;
	idt[num].type = 14;
	idt[num].dpl = dpl;
	idt[num].present = 1;
}

void interrupt_handler(struct regs *r)
{
	
	char *buf = "      ";
	int_to_string(r->int_no, buf);
	print("INT: ");
	print(buf);
	print("\n");
}