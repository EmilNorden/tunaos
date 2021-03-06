#include "interrupts.h"
#include "low_level.h"
#include "util.h"
#include "isr.h"
#include "irq.h"
#include "system.h"
#include "../drivers/pic.h"
#include "../drivers/screen.h"

void _irq_initialize_idt(void);
void _irq_set_interrupt_gate(uint32_t num, uint32_t base, uint16_t selector, uint8_t dpl);
void handle_irq7(void);

/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ */
irq_handler_func irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Page Fault",
	"Reserved",
	"x87 Floating-Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization Exception",
    "Reserved",
    "Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Security Exception",
	"Reserved"
};


struct  __attribute__((packed)) IDTEntry {
	uint16_t offset_lo;		// offset bits 0..15
	uint16_t selector;			// a code segment selector in GDT or LDT
	uint8_t zero;				// unused, set to 0
	uint8_t type 		: 5;	// Type
	uint8_t dpl			: 2;	// Descriptor Privilege Level.
	uint8_t present		: 1;	// 0 if descriptor is not used.	
	uint16_t offset_hi;			// offset bits 16..31
};

/* 
	The first 32 are reserved for processor exceptions
	The next 16 can be used for hardware interrupts
	The remaining are available for software interrupts
*/
struct IDTEntry idt[256];

void _irq_initialize_idt(void)
{
	struct
	{
		uint16_t length;
		uint32_t base;
	} __attribute__((packed)) IDTR;
	
	IDTR.length  = (sizeof(struct IDTEntry) * 256) - 1;
	IDTR.base = (uint32_t)&idt;
	__asm__("lidt (%0)" : : "p"(&IDTR));
	
	// Setup interrupt gates for all CPU exceptions
	_irq_set_interrupt_gate(0, (unsigned)_isr0, 0x08, 0);
	_irq_set_interrupt_gate(1, (unsigned)_isr1, 0x08, 0);
	_irq_set_interrupt_gate(2, (unsigned)_isr2, 0x08, 0);
	_irq_set_interrupt_gate(3, (unsigned)_isr3, 0x08, 0);
	_irq_set_interrupt_gate(4, (unsigned)_isr4, 0x08, 0);
	_irq_set_interrupt_gate(5, (unsigned)_isr5, 0x08, 0);
	_irq_set_interrupt_gate(6, (unsigned)_isr6, 0x08, 0);
	_irq_set_interrupt_gate(7, (unsigned)_isr7, 0x08, 0);
	_irq_set_interrupt_gate(8, (unsigned)_isr8, 0x08, 0);
	_irq_set_interrupt_gate(9, (unsigned)_isr9, 0x08, 0);
	_irq_set_interrupt_gate(10, (unsigned)_isr10, 0x08, 0);
	_irq_set_interrupt_gate(11, (unsigned)_isr11, 0x08, 0);
	_irq_set_interrupt_gate(12, (unsigned)_isr12, 0x08, 0);
	_irq_set_interrupt_gate(13, (unsigned)_isr13, 0x08, 0);
	_irq_set_interrupt_gate(14, (unsigned)_isr14, 0x08, 0);
	_irq_set_interrupt_gate(15, (unsigned)_isr15, 0x08, 0);
	_irq_set_interrupt_gate(16, (unsigned)_isr16, 0x08, 0);
	_irq_set_interrupt_gate(17, (unsigned)_isr17, 0x08, 0);
	_irq_set_interrupt_gate(18, (unsigned)_isr18, 0x08, 0);
	_irq_set_interrupt_gate(19, (unsigned)_isr19, 0x08, 0);
	_irq_set_interrupt_gate(20, (unsigned)_isr20, 0x08, 0);
	_irq_set_interrupt_gate(21, (unsigned)_isr21, 0x08, 0);
	_irq_set_interrupt_gate(22, (unsigned)_isr22, 0x08, 0);
	_irq_set_interrupt_gate(23, (unsigned)_isr23, 0x08, 0);
	_irq_set_interrupt_gate(24, (unsigned)_isr24, 0x08, 0);
	_irq_set_interrupt_gate(25, (unsigned)_isr25, 0x08, 0);
	_irq_set_interrupt_gate(26, (unsigned)_isr26, 0x08, 0);
	_irq_set_interrupt_gate(27, (unsigned)_isr27, 0x08, 0);
	_irq_set_interrupt_gate(28, (unsigned)_isr28, 0x08, 0);
	_irq_set_interrupt_gate(29, (unsigned)_isr29, 0x08, 0);
	_irq_set_interrupt_gate(30, (unsigned)_isr30, 0x08, 0);
	_irq_set_interrupt_gate(31, (unsigned)_isr31, 0x08, 0);
	
	_irq_set_interrupt_gate(32, (unsigned)_irq0, 0x08, 0);
	_irq_set_interrupt_gate(33, (unsigned)_irq1, 0x08, 0);
	_irq_set_interrupt_gate(34, (unsigned)_irq2, 0x08, 0);
	_irq_set_interrupt_gate(35, (unsigned)_irq3, 0x08, 0);
	_irq_set_interrupt_gate(36, (unsigned)_irq4, 0x08, 0);
	_irq_set_interrupt_gate(37, (unsigned)_irq5, 0x08, 0);
	_irq_set_interrupt_gate(38, (unsigned)_irq6, 0x08, 0);
	_irq_set_interrupt_gate(39, (unsigned)_irq7, 0x08, 0);
	_irq_set_interrupt_gate(40, (unsigned)_irq8, 0x08, 0);
	_irq_set_interrupt_gate(41, (unsigned)_irq9, 0x08, 0);
	_irq_set_interrupt_gate(42, (unsigned)_irq10, 0x08, 0);
	_irq_set_interrupt_gate(43, (unsigned)_irq11, 0x08, 0);
	_irq_set_interrupt_gate(44, (unsigned)_irq12, 0x08, 0);
	_irq_set_interrupt_gate(45, (unsigned)_irq13, 0x08, 0);
	_irq_set_interrupt_gate(46, (unsigned)_irq14, 0x08, 0);
	_irq_set_interrupt_gate(47, (unsigned)_irq15, 0x08, 0);
}

void irq_initialize(void)
{
	_irq_initialize_idt();
	pic_initialize();
	
	port_byte_out(0x21, 0xfc);
	port_byte_out(0xa1, 0xff);
	
	irq_enable();
}

void irq_set_handler(int irq, irq_handler_func handler)
{
	ASSERT(irq >= 0 && irq < 16, "Invalid IRQ number");
	
	irq_routines[irq] = handler;
}

void irq_enable(void)
{
	__asm__ __volatile__("sti");
}

void irq_disable(void)
{
	__asm__ __volatile__("cli");
}

void _irq_set_interrupt_gate(uint32_t num, uint32_t base, uint16_t selector, uint8_t dpl)
{
	ASSERT(num >= 0 && num < 256, "Invalid IDT entry index");
	
	idt[num].offset_lo = base & 0xFFFF;
	idt[num].offset_hi = (base >> 16) & 0xFFFF;
	idt[num].zero = 0;
	idt[num].selector = selector;
	idt[num].type = 14;
	idt[num].dpl = dpl;
	idt[num].present = 1;
}

// Invoked for any CPU exception
void interrupt_handler(struct regs *r)
{
	if(r->int_no < 32) {
		print(exception_messages[r->int_no]);
		__asm__ __volatile__("hlt");
	}
}


// Invoked for any hardware IRQ
void irq_handler(struct regs *r)
{
	irq_handler_func handler;
	int irq = r->int_no - 32;
	
	ASSERT(irq >= 0 && irq < 32, "Intercepted unknown hardware IRQ");
	
	// IRQ7 gets special treatment
	if(irq == 7) {
		handle_irq7();
		return;
	}
		
	handler = irq_routines[irq];
	if(handler) {
		handler(r);
	}
	else {
		char *buf = "       ";
		int_to_string(irq, buf);
		print("IRQ '");
		print(buf);
		print("' not handled.\n");
	}
	
	pic_send_eoi(irq);
}

void handle_irq7(void)
{
	port_byte_out(0x20, 0x0B);
	unsigned char irr = port_byte_in(0x20);
	
	// If bit 7 is not set, exit from the IRQ handler without sending EOI
	if(irr & 0x80){
		pic_send_eoi(0x07);
	}
}