disk_load:
	push dx			; Store DX on stack so we later can recall how many
					; sectors were request to be read
	mov ah, 0x02	; BIOS read sector function
	mov al, dh		; Read DH sectors
	mov ch, 0x00	; Select cylinder (0 for floppy, 1 for USB, apparently..)
	mov dh, 0x00	; Select head 0
	int 0x13		; BIOS interrupt
	
	jc disk_error	; Jump if error (i.e. carry flag set)
	
	pop dx			; Restore DX from the stack
	
	cmp dh, al		; if AL (sectors read) != DH (sectors expected)
	jne disk_error	; display error message
	ret

disk_error:
	mov ah, 0x01	; 01h = Get Status of Last Drive Operation
	int 0x13
		; Set high byte to 0 so we can print ax in its entirety to display
					; the return code
	mov dx, ax
	mov bx, DISK_READ_ERROR_MSG
	call print_string
	call print_hex
	jmp $

; Gets error from last drive operation
; OUT AL = return code
disk_get_last_error:
	mov ah, 0x01	; 01h = Get Status of Last Drive Operation
	int 0x13
	ret
	
; Resets disk drive
; AL = drive number
disk_reset:
	pusha
	
	mov ah, 0		; Reset command
	int 0x13
	or ah, ah
	jz disk_reset_ok
	mov bx, DISK_RESET_ERROR_MSG
	call print_string
	jmp $
disk_reset_ok:
	popa
	ret
	
; Variables
DISK_READ_ERROR_MSG	db "Disk read error: ", 0
DISK_RESET_ERROR_MSG db "Disk reset error", 0
