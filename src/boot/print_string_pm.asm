[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; ===== print_string_pm =====
; Prints a null-terminated string
; Input:
; EBX - A pointer to the string to print
print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY		; Set EDX to start of video memory

print_string_pm_loop:
	mov al, [ebx]				; Store the char at EBX in AL
	mov ah, WHITE_ON_BLACK		; Store the attributes in AH
	
	cmp al, 0					; If AL == 0 (null byte)
	je print_string_pm_done		; Jump to done
	
	mov [edx], ax				; Store char and attribs at current character cell
	add ebx, 1					; Increment EBX to next char in string
	add edx, 2					; Move to next char cell in video memory
	
	jmp print_string_pm_loop	; Loop and print next char
	
print_string_pm_done:
	popa
	ret