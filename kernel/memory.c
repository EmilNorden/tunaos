#include "memory.h"
#include "util.h"
#include "../drivers/screen.h"
#include "system.h"
#include <stdint.h>

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
	struct free_region *prev;
};

static struct free_region *free_head = 0;
static size_t total_memory;
static size_t available_memory;

static struct memory_map_entry *memory_map = (struct memory_map_entry*) (MEMORY_MAP_BASE_ADDRESS+2);
static uint16_t memory_map_entry_count;

static void merge_adjacent_regions(struct free_region *reg);
static void find_adjacent_free_regions(struct free_region *reg, struct free_region **lower_region, struct free_region **higher_region);
static uintptr_t get_region_start(struct free_region *reg);
static uintptr_t get_region_end(struct free_region *reg);
static void remove_region(struct free_region *reg);

void memory_init(void)
{
	memory_map_entry_count = *(uint16_t*)MEMORY_MAP_BASE_ADDRESS;	
	
	total_memory = 0;
	
	struct free_region *current_region;
	char *buf = "        ";
	
	for(int i = 0; i < memory_map_entry_count; ++i) {
		if(memory_map[i].type == REGION_TYPE_USABLE || memory_map[i].type == REGION_TYPE_ACPI_RECLAIMABLE) {
			
			if(memory_map[i].base_address + memory_map[i].length >= 0xFFFFFFFF) {
				
				print("Ignoring memory region outside 32-bit range.");
				continue;
				
			}
			
			uintptr_t base = (uintptr_t)memory_map[i].base_address;
			current_region = (struct free_region*)base;
			current_region->length = memory_map[i].length;
			current_region->prev = 0;
			current_region->next = free_head;
			free_head->prev = current_region;
			free_head = current_region;
			
			total_memory += memory_map[i].length;
		}
	}
	
	available_memory = total_memory;
}

void *malloc(size_t size)
{
	size += sizeof(int);
	size = size < sizeof(struct free_region) ? sizeof(struct free_region) : size;
	// Align size by 4. Size is divisible by 4 if the two least significant bits are 0.	
	
	struct free_region *current_region = free_head;
	while(current_region && current_region->length < size)
		current_region = current_region->next;
		
	if(current_region) {
		
		current_region->length -= size;
		
		int *ptr = (int*)(((char*)current_region) + current_region->length);
		*ptr = size;
		
		available_memory -= size;
		return ++ptr;
		
	}
	
#ifdef DEBUG_ALLOCATIONS
	char buffer[32];
	snprintf(buffer, 32, "Unable to allocate size of %d\n.", 1, size);
	print(buffer);
	debug_print_free_regions();
#endif
	
	return 0;
}

void free(void *p)
{
	int *intptr = (int*)p;
	int region_length = *(--intptr);
	
	PANIC_ON(region_length <= 0, "An attempt was made to free invalid memory block.");
	
	struct free_region *region = (struct free_region*)intptr;
	region->length = region_length;
	region->prev = 0;
	region->next = free_head;
	free_head->prev = region;
	free_head = region;
	
	merge_adjacent_regions(region);
	
	available_memory += region_length;
}

size_t get_available_memory(void)
{
	return available_memory;
}

size_t get_total_memory(void)
{
	return total_memory;
}

void memory_zero(void *p, size_t size)
{
	char *p_current = (char*)p;
	char *p_end = p_current + size;
	do
	{
		*p_current = 0;
	}while(++p_current != p_end);
}

void memory_copy(void *source, void *dest, size_t count) {
	char *csource = (char*)source;
	char *cdest = (char*)dest;
	for(size_t i = 0; i < count; i++) {
		*(cdest + i) = *(csource + i);
	}
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
		print(" - Location : ");
		int_to_string((uintptr_t)current, buf);
		print(buf);
		print(", Size: ");
		int_to_string(current->length, buf);
		print(buf);
		print("\n");
		
		total += current->length;
		current = current->next;	
	}
	
	print("Total: ");
	int_to_string(total, buf);
	print(buf);
	print(" bytes\n");
}

static void find_adjacent_free_regions(struct free_region *reg, struct free_region **lower_region, struct free_region **higher_region)
{
	struct free_region *current_region = free_head;
	
	uintptr_t reg_start = get_region_start(reg);
	uintptr_t reg_end = get_region_end(reg); 
	
	while(current_region) {
		
		if(current_region != reg) {
			
			uintptr_t current_region_start = get_region_start(current_region);
			uintptr_t current_region_end = get_region_end(current_region);
			
			if(current_region_end == reg_start) {
				*lower_region = current_region;
			}
			else if(current_region_start  == reg_end) {
				*higher_region = current_region;
			}
		}
		
		current_region = current_region->next;
	}
}

static void merge_adjacent_regions(struct free_region *reg)
{
	uintptr_t reg_start = get_region_start(reg);
	uintptr_t reg_end = get_region_end(reg);
	
	struct free_region *current_region = free_head;
	
	struct free_region *lower_region = 0;
	struct free_region *higher_region = 0;
	
	// Find adjacent regions, if any.
	find_adjacent_free_regions(reg, &lower_region, &higher_region);
	
	if(higher_region) {
		reg->length += higher_region->length;
		remove_region(higher_region);
	}
	
	if(lower_region) {
		lower_region->length += reg->length;
		remove_region(reg);
	}
}

static uintptr_t get_region_start(struct free_region *reg)
{
	return (uintptr_t)reg;	
}

static uintptr_t get_region_end(struct free_region *reg)
{
	unsigned char *reg_start = (unsigned char*)reg;
	return (uintptr_t)(reg_start + reg->length);
}

static void remove_region(struct free_region *reg)
{
	if(reg->prev) {
		reg->prev->next = reg->next;
	}
	
	if(reg->next) {
		reg->next->prev = reg->prev;
	}
	
	if(reg == free_head) {
		free_head = reg->next;
	}
}