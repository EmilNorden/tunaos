#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <stdint.h>

void irq_initialize(void);

void irq_set_interrupt_gate(uint32_t num, uint32_t base, uint16_t selector, uint8_t dpl);


#endif