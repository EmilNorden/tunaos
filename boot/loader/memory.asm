[bits 16]
; The entry structure looks like this:
; 	uint64 - base address
; 	uint64 - length of region (can apparently be 0, in which case it should be skipped)
; 	uint32 - type
; 	uint32 - extended ACPI attribute (unused?)
; total size of structure is 24 bytes
; 


; Creates a memory map.
; Inputs:
; ES:DI	- memory address to store map at
; AX	- size of memory region at ES:DI
create_memory_map:
	pusha
	
	; Store base address for memory map, we will need it later.
	mov [MEMORY_MAP_START_ADDR], DI
	
	; Skip two first bytes. We will use them for storing the entry count later
	add DI, 2
	
	; Decrement total region size by 2, since we have kinda "claimed" the first two bytes nowl.
	SUB AX, 2
	
	; Divide AX with 24 to find out how many entries we can pull out
	mov BX, 0x18
	xor DX, DX
	div BX
	mov [MAX_ENTRIES], AX
	
	; Clear our entry counter (bp)
	xor bp, bp
	
	; Clear EBX (status register for this procedure) 
	xor EBX, EBX
	mov EDX, 0x534D4150
loop:
	mov EAX, 0xE820
	
	mov ECX, 24
	INT 0x15
	
	; If carry flag is set, the call failed
	jc fail
	
	; EAX should be set to the magic word if call was successful
	mov EDX, 0x534D4150
	cmp EAX, EDX
	jne fail
	
	; Should "investigate" entry here. For instance if length is zero we can skip it.
	; (and by skipping, its just a matter of NOT incrementing DI, so it will be overwritten by the next entry)
	
	; entry looks good, keep it in memory by incrementing ES:DI
	add DI, 24
	
	; If EBX is 0, we have reached the end of the list
	cmp EBX, 0
	je done
	
	inc BP
	cmp BP, [MAX_ENTRIES]
	je done
	
	jmp loop
	
done:
	; Place the entry count (from BP) at the base address of the supplied memory range
	mov BX, [MEMORY_MAP_START_ADDR]
	mov [BX], BP
	popa
	ret
	
fail:
	mov BX, MSG_MEMORY_MAP_FAIL
	call print_string
	jmp $
	
MEMORY_MAP_START_ADDR: dw 0
MAX_ENTRIES: dw 0	
MSG_MEMORY_MAP_FAIL: db 'Failed to create memory map', 0