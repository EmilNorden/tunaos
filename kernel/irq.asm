global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

; IRQ 0
_irq0:
	cli
	push byte 0
	push byte 32
	jmp irq_common
	
; IRQ 1
_irq1:
	cli
	push byte 0
	push byte 33
	jmp irq_common
		
; IRQ 2
_irq2:
	cli
	push byte 0
	push byte 34
	jmp irq_common
	
; IRQ 3
_irq3:
	cli
	push byte 0
	push byte 35
	jmp irq_common
	
; IRQ 4
_irq4:
	cli
	push byte 0
	push byte 36
	jmp irq_common
	
; IRQ 5
_irq5:
	cli
	push byte 0
	push byte 37
	jmp irq_common
		
; IRQ 6
_irq6:
	cli
	push byte 0
	push byte 38
	jmp irq_common
		
; IRQ 7
_irq7:
	cli
	push byte 0
	push byte 39
	jmp irq_common
	
; IRQ 8
_irq8:
	cli
	push byte 0
	push byte 40
	jmp irq_common
		
; IRQ 9
_irq9:
	cli
	push byte 0
	push byte 41
	jmp irq_common
		
; IRQ 10
_irq10:
	cli
	push byte 0
	push byte 42
	jmp irq_common
	
; IRQ 11
_irq11:
	cli
	push byte 0
	push byte 43
	jmp irq_common
		
; IRQ 12
_irq12:
	cli
	push byte 0
	push byte 44
	jmp irq_common
		
; IRQ 13
_irq13:
	cli
	push byte 0
	push byte 45
	jmp irq_common
		
; IRQ 14
_irq14:
	cli
	push byte 0
	push byte 44
	jmp irq_common
		
; IRQ 15
_irq15:
	cli
	push byte 0
	push byte 44
	jmp irq_common
	
extern irq_handler
	
irq_common:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, irq_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret