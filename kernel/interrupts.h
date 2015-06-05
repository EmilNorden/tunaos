#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <stdint.h>

/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

typedef void (*irq_handler_func)(struct regs*);

void irq_initialize(void);
void irq_set_handler(int irq, irq_handler_func handler);






#endif