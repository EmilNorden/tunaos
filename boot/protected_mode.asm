[bits 16]
switch_to_pm:
cli							; Disable all interrupts. Very important to do before
							; we go into protected mode
lgdt [gdt_descriptor]		; Load our GDT
mov eax, cr0				; To make the switch to protected mode, we set
or eax, 0x1					; the first bit of CR0, a control register
mov cr0, eax				; Update the control register

; Now we are in "kinda" in protected mode. Since the CPU pipelines the
; execution of instructions, there is a risk that the CPU may process some
; stages of an instructions execution in the wrong mode. So we need to "flush" the pipeline.
; Pipelining works when the CPU knows ahead of time what instructions are to come,
; so it doesn't like instructions such as jmp or call.

jmp CODE_SEG:init_pm

[bits 32]
init_pm:
	mov ax, DATA_SEG		; Our old segments are now meaningless,
	mov ds, ax				; so we point our segment registers to the
	mov ss, ax				; data selector we defined in our GDT
	mov es, ax
	mov fs, ax
	mov gs, ax

mov ebp, 0x90000			; Update our stack position so it is right
mov esp, ebp				; at the top of the free space
call BEGIN_PM