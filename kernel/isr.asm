global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31

; Divide by zero
_isr0:
	cli
	push byte 0
	push byte 0
	jmp isr_common
    
; Debug
 _isr1:
	cli
	push byte 0
	push byte 1
	jmp isr_common
	
; Non-maskable Interrupt
 _isr2:
	cli
	push byte 0
	push byte 2
	jmp isr_common
    
; Breakpoint
 _isr3:
	cli
	push byte 0
	push byte 3
	jmp isr_common
    
; Overflow
 _isr4:
	cli
	push byte 0
	push byte 4
	jmp isr_common
    
; Bound range exceeded
 _isr5:
	cli
	push byte 0
	push byte 5
	jmp isr_common
    
; Invalid opcode
 _isr6:
	cli
	push byte 0
	push byte 6
	jmp isr_common
    
; Device not available
 _isr7:
	cli
	push byte 0
	push byte 7
	jmp isr_common
    
; Double fault
_isr8:
	cli
	push byte 0
	push byte 8
	jmp isr_common_err_code
    
; Coprocessor segment overrun
_isr9:
	cli
	push byte 0
	push byte 9
	jmp isr_common
    
; Invalid TSS
_isr10:
	cli
	push byte 0
	push byte 10
	jmp isr_common_err_code
    
; Segment not present
_isr11:
	cli
	push byte 0
	push byte 11
	jmp isr_common_err_code
    
; Stack-segment fault
_isr12:
	cli
	push byte 0
	push byte 12
	jmp isr_common_err_code
    
; General protection fault
_isr13:
	cli
	push byte 0
	push byte 13
	jmp isr_common_err_code
    
; Page fault
_isr14:
	cli
	push byte 0
	push byte 14
	jmp isr_common_err_code
    
; x87 floating-point exception
_isr16:
	cli
	push byte 0
	push byte 16
	jmp isr_common
    
; Alignment check
_isr17:
	cli
	push byte 0
	push byte 17
	jmp isr_common_err_code
    
; Machine check
_isr18:
	cli
	push byte 0
	push byte 18
	jmp isr_common
    
; SIMD floating-point exception
_isr19:
	cli
	push byte 0
	push byte 19
	jmp isr_common
    
; Virtualization exception
_isr20:
	cli
	push byte 0
	push byte 20
	jmp isr_common
    
; Security exception
_isr30:
	cli
	push byte 0
	push byte 30
	jmp isr_common_err_code
    
extern interrupt_handler

; The reason for having an isr_common and isr_common_err_code is because we need to pop the interrupt
; error code in the latter. Thats on the to-do list.
isr_common:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, interrupt_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
    

isr_common_err_code:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, interrupt_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!