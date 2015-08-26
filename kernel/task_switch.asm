global switch_stack_asm

global doit2
global doit3
global doit4
global restore_stack
global update_task_sp
global switch_stack_asm3
global switch_stack_asm3_b

doit2:
	push ebp
	mov ebp, esp
	mov eax, dword [ebp+8]
	sub eax, dword [ebp+12]
	pop ebp
	ret
	
doit3:
	mov eax, dword [esp]
	ret
	
doit4:
	push ebp
	mov ebp, esp
	
	mov esp, dword [ebp+8]
	pop eax
	pop ecx
	pop edx
	pop ebx
	pop esi
	pop edi
	iretd
	
	pop ebp
	ret
	
update_task_sp:
	push ebx		; used to store pointer-to-sp
	push eax		; used to "calculate" tasks sp
	
	; place first argument in ebx
	mov ebx, dword [esp+12]	
	mov eax, esp
	
	; add 8 to eax (which holds esp) to jump over the pushed ebx and eax
	add eax, 8
	mov dword [ebx], eax
	pop eax
	pop ebx
	ret
	
[bits 32]
switch_stack_asm3:
	pushfd
	; push 514
	push 0x08 		; CS	16-20
	push yield_return_point	; IP	20-24
	push edi		;		24-28
	push esi		;		32
	push ebx		;		36
	push edx		;		40
	push ecx		;		44
	push eax		;		48
	
	mov eax, cs
	
	mov eax, [esp+44]		; previous task
	cmp eax, 0
	je restore_stack3
		
	mov [eax+8], esp		; 8 is offset to current_sp
	
restore_stack3:
	mov eax, [esp+40]		; new task
	mov ebp, dword [eax+4]	; 4 is offset to stack_base
	mov esp, dword [eax+8]	; 8 is offset to current_sp
	
	pop eax
	pop ecx
	pop edx
	pop ebx
	pop esi
	pop edi
	iretd
yield_return_point:
	ret