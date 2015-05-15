; ===== print_string =====
; prints a null-terminated string
; Input:
; BX - a pointer to the string to print

print_string:
	pusha
	mov ah, 0x0e		; BIOS tele-type output
	
print_string_loop:
	mov cx, [bx]
	cmp cl, 0
	je print_string_stop
	
	mov al, cl
	int 0x10
	
	add bx, 1
	jmp print_string_loop
	
print_string_stop:
	popa
	ret

; ===== print_hex =====
; prints a value in hexidecimal format to the display
; Input:
; DX - Value to be printed
print_hex:
	pusha
	
	mov ax, 4			; Loop counter
	
	mov bx, HEX_OUT		; Set bx to base address of template string
	add bx, 5			; Add offset of 5 to point at the last character

ph_loop:
	
	mov cx, dx			; Move the value into cx
	and cx, 0xf			; AND with 15, all lower 4 bits set
	
	
	cmp cx, 0xa			; Compare with 0xa (10)
	jl ph_set_less_10	; If lower than 10, jump to ph_set_less_10
	add cx, 0x57		; Else add offset 0x57 to cx (0x61 is 'a' in ASCII table, so if cx is 10(0xa) then 0x57+0xa=0x61)
	jmp ph_end_set
ph_set_less_10:
	add cx, 0x30		; 0x30 is 0 on the ASCII table, so add that as an offset into cx

ph_end_set:

	mov [bx], cl		; Move the ASCII character in cx to where bx is pointing
	
	sub ax, 1			; Decrement loop pointer
	cmp ax, 0
	je ph_done			; If ax == 0, jump to ph_done
						; else
	shr dx, 4			; Shift dx right 4 bits
	sub bx, 1			; Move to the left in the hex string
	jmp ph_loop
ph_done:
	mov bx, HEX_OUT
	call print_string
	
	popa
	ret

; Global variables
HEX_OUT: db '0x0000', 0