[org 0x7c00]
KERNEL_OFFSET equ 0x1000	; This is the memory offset to which we will load our kernel

	mov [BOOT_DRIVE], dl
	
	mov bp, 0x9000			; Set the stack
	mov sp, bp
	
	mov bx, MSG_REAL_MODE
	call print_string
	
	call load_kernel		; Load our kernel
	
	call switch_to_pm		; Note that we never return from here
	
	jmp $

%include "print.asm"
%include "print_string_pm.asm"
%include "gdt.asm"
%include "protected_mode.asm"
%include "disk.asm"

[bits 16]
load_kernel:
	mov bx, MSG_LOAD_KERNEL			; Announce that we are loading the kernel
	call print_string
	
	mov bx, KERNEL_OFFSET			; Load 4 sector AFTER the boot sector
	mov dh, 4						; (where our kernel is), to address KERNEL_OFFSET
	mov dl, [BOOT_DRIVE]			; REMEMBER that when the kernel grows, we need to read more sectors...
	call disk_load
	
	ret

[bits 32]
BEGIN_PM:
	mov ebx, MSG_PROT_MODE	; Announce that we are in 32-bit protected mode.
	call print_string_pm
	
	call KERNEL_OFFSET		; Now jump to the address of our loaded kernel code.
	
	jmp $					; Hang
	
; Global variables
BOOT_DRIVE		db 0
MSG_REAL_MODE	db "Started in 16-bit Real mode", 0
MSG_PROT_MODE	db "Successfully landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL	db "Loading kernel into memory", 0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55
