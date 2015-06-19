; This is the second part of our bootloader, 1024 bytes large loaded into memory at 0x1000.

[bits 16]
[org 0x1000]
KERNEL_OFFSET equ 0x1400 		; 0x1000 (where our secondary bootloader is placed in memory) +
								; 1024 (size of our bootloader) 
jmp start

; Data area
db 'Hello, world!', 0

start:
	mov [BOOT_DRIVE], dl
; do stuff

call load_kernel

mov bx, MSG_LOADER_START			; Announce that we are loading the kernel
	call print_string

call switch_to_pm		; Note that we never return from here

%include "../print.asm"
%include "../gdt.asm"
%include "../disk.asm"
%include "../protected_mode.asm"

[bits 16]
load_kernel:
	mov dl, [BOOT_DRIVE]
	call disk_reset
	
	mov bx, KERNEL_OFFSET			; Load 20 sector AFTER the boot sector
	mov dh, 23						; (where our kernel is), to address KERNEL_OFFSET
	mov dl, [BOOT_DRIVE]			; REMEMBER that when the kernel grows, we need to read more sectors...
	mov cl, 0x04					; Start reading from fourth sector (1st sector = boot sector, 2nd/3rd sector = our secondary bootloader)
	call disk_load
	
	ret

[bits 32]
BEGIN_PM:
	call KERNEL_OFFSET		; Now jump to the address of our loaded kernel code.
	
	jmp $					; Hang
	
; VARIABLES
BOOT_DRIVE: db 0
MSG_LOADER_START: db 'we made the jump!', 0
times 1024-($-$$) db 0