/* 
	Driver for the 8259 Programmable Interrupt Controller
*/

#ifndef PIC_8259A_H_
#define PIC_8259A_H_
typedef enum { PIC_MASTER = 0x20, PIC_SLAVE = 0xA0 } pic_type;


void pic_initialize(void);
void pic_send_eoi(unsigned char irq);
void pic_set_mask(unsigned char irq);
void pic_clear_mask(unsigned char irq);
unsigned char pic_get_mask(pic_type type);

#endif