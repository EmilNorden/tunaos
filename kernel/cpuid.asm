global asm_get_cpu_info

; The cpu_info structure looks like this:
; 	uint8*13 - vendor id 
; total size of structure is 13 bytes
; 

; Retrieves details about the processor using the CPUID instruction.
asm_get_cpu_info:
	pushad
	mov ebp, dword [ebp+8] 				; Store memory address in EBP, as EDX will be overwritten by CPUID
	mov eax, 0
	cpuid
	mov [ebp], ebx
	mov [ebp+4], edx
	mov [ebp+8], ecx
	popad
	ret
