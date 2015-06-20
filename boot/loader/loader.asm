; This is the second part of our bootloader, 1024 bytes large loaded into memory at 0x1000.

[bits 16]
[org 0x1000]
KERNEL_OFFSET equ 0x1600 		; 0x1000 (where our secondary bootloader is placed in memory) +
								; 1024 (size of our bootloader) + 512 (size of our memory map data)
	mov [BOOT_DRIVE], dl
	
	mov DI, memory_map_start
	mov AX, memory_map_end - memory_map_start
	call create_memory_map

	call load_kernel

	mov bx, MSG_LOADER_START			; Announce that we are loading the kernel
	call print_string

	call switch_to_pm		; Note that we never return from here

%include "../print.asm"
%include "../gdt.asm"
%include "../disk.asm"
%include "../protected_mode.asm"
%include "memory.asm"

[bits 16]
load_kernel:
	mov dl, [BOOT_DRIVE]
	call disk_reset
	
	mov bx, KERNEL_OFFSET			; Load 20 sector AFTER the boot sector
	mov dh, 23						; (where our kernel is), to address KERNEL_OFFSET
	mov dl, [BOOT_DRIVE]			; REMEMBER that when the kernel grows, we need to read more sectors...
	mov cl, 0x05					; Start reading from fourth sector (1st sector = boot sector, 2nd/3rd sector = our secondary bootloader, 5th sector = data area)
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

; ========
; From here on, 512 bytes are reserved for memory map data.
; 2 bytes for entry count, 510 bytes for actual entry list

; Memory map area
memory_map_start:
times 512 db 0		; fill with 510 zeroes, this space is reserved for memory map entries
memory_map_end: