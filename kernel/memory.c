#include "memory.h"
#include <stdint.h>
#include "util.h"
#include "../drivers/screen.h"
#include "system.h"

#define MEMORY_MAP_BASE_ADDRESS		0x1400

enum region_type
{
	REGION_TYPE_USABLE = 1,
	REGION_TYPE_RESERVED = 2,
	REGION_TYPE_ACPI_RECLAIMABLE = 3,
	REGION_TYPE_ACPI_NVS = 4,
	REGION_TYPE_BAD_MEMORY = 5
};

struct __attribute__((packed)) memory_map_entry {
	uint64_t base_address;
	uint64_t length;
	uint32_t type;
	uint32_t acpi; // extended
};

struct free_region {
	uint64_t length;
	struct free_region *next;
};

struct free_region *free_head = 0;

struct memory_map_entry *memory_map = (struct memory_map_entry*) (MEMORY_MAP_BASE_ADDRESS+2);
uint16_t memory_map_entry_count;

void memory_init()
{
	memory_map_entry_count = *(uint16_t*)MEMORY_MAP_BASE_ADDRESS;
	int i = 0;
	
	struct free_region *previous_region = 0;
	struct free_region *current_region = 0;
	
	uint64_t total_available = 0;
	for(int i = 0; i < memory_map_entry_count; ++i) {
		if(memory_map[i].type == REGION_TYPE_USABLE || memory_map[i].type == REGION_TYPE_ACPI_RECLAIMABLE) {
			
			if(memory_map[i].base_address + memory_map[i].length >= 0xFFFFFFFF) {
				
				print("Ignoring memory region outside 32-bit range.");
				continue;
				
			}
			
			int base = (int)memory_map[i].base_address;
			current_region = (struct free_region*)base;
			current_region->length = memory_map[i].length;
			current_region->next = 0;
			
			if(previous_region)
				previous_region->next = current_region;
			else
				free_head = current_region;
				
			previous_region = current_region;
			
			total_available += memory_map[i].length;
			
		}
	}
}

void *malloc(unsigned int size)
{
	size = size < sizeof(struct free_region) ? sizeof(struct free_region) : size;
	size += sizeof(int);
	
	struct free_region *current_region = free_head;
	while(current_region && current_region->length < size)
		current_region = current_region->next;
		
	if(current_region) {
		
		current_region->length -= size;
		
		
		int *ptr = (int*)(((char*)current_region) + current_region->length);
		*ptr = size;
		return ++ptr;
		
	}
	
	return 0;
}

void free(void *p)
{
	int *intptr = (int*)p;
	int region_length = *(--intptr);
	
	PANIC_ON(region_length <= 0, "An attempt was made to free invalid memory block.");
	
	struct free_region *region = (struct free_region*)intptr;
	region->length = region_length;
	region->next = free_head;
	free_head = region;
}

void memory_zero(void *p, unsigned int size)
{
	char *p_current = (char*)p;
	char *p_end = p_current + size;
	do
	{
		*p_current = 0;
	}while(++p_current != p_end);
}

void debug_print_free_regions()
{
	struct free_region *current = free_head;
	
	char *buf = "            ";
	uint32_t total = 0;
	int i = 1;
	while(current) {
		int_to_string(i++, buf);
		print(buf);
		print(": ");
		print("Size: ");
		int_to_string(current->length, buf);
		print(buf);
		print("\n");
		
		total += current->length;
		current = current->next;	
	}
	
	print("Total: ");
	int_to_string(total, buf);
	print(buf);
	print("\n");
}