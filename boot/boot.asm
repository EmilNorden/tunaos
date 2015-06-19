[bits 16]
[org 0x7c00]
LOADER_OFFSET equ 0x1000	; This is the memory offset to which we will load our second bootloader

jmp word load

; BIOS parameter block
                db "EMILEMIL"           ; OEM Label String
                dw 512                  ; Bytes per sector
                db 1                    ; Sectors per FAT cluster
                dw 1                   ; Reserved sector count
                db 2                    ; number of FATs
                dw 224                  ; Root dir entries
                dw 2880                 ; Total Sectors
                db 240                  ; Double sided, 18 sectors per track
                dw 9                    ; Sectors per FAT
                dw 18                   ; Sectors per track
                dw 2                    ; Head count (double sided)
                dd 0                    ; Hidden sector count 

load:
	mov [BOOT_DRIVE], dl
	
	mov bp, 0x9000			; Set the stack
	mov sp, bp
	
	mov bx, MSG_REAL_MODE
	call print_string
	
	call load_loader		; Load our kernel
	
	jmp LOADER_OFFSET
	; call switch_to_pm		; Note that we never return from here
	
	jmp $

%include "print.asm"
%include "disk.asm"

load_loader:
	mov bx, MSG_LOAD_KERNEL			; Announce that we are loading the kernel
	call print_string
	
	mov dl, [BOOT_DRIVE]
	call disk_reset
	
	mov bx, LOADER_OFFSET			; Load 2 sectors (1024 bytes) AFTER the boot sector
	mov dh, 2						; to address LOADER_OFFSET
	mov dl, [BOOT_DRIVE]			
	mov cl, 0x02					; Start reading from second sector (i.e after the boot sector)
	call disk_load
	
	ret
	
; Global variables
BOOT_DRIVE		db 0
MSG_REAL_MODE	db "Started in 16-bit Real mode", 0
MSG_LOAD_KERNEL	db "Loading secondary boot loader into memory", 0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55
