#include "memory.h"
#include <stdint.h>
#include "util.h"
#include "../drivers/screen.h"

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

struct memory_map_entry *memory_map = (struct memory_map_entry*) (MEMORY_MAP_BASE_ADDRESS+2);
uint16_t memory_map_entry_count;

void memory_init()
{
	memory_map_entry_count = *(uint16_t*)MEMORY_MAP_BASE_ADDRESS;
	
	char *buf = "                    ";
	int i = 0;
	uint64_t total_available = 0;
	print("Printing memory map:\n");
	for(i = 0; i < memory_map_entry_count; ++i) {
		print("== entry:");
		int_to_string(i, buf);
		print(buf);
		print("\n");
		
		print("base: ");
		int_to_string(memory_map[i].base_address, buf);
		print(buf);
		print("\n");
		
		print("length: ");
		int_to_string(memory_map[i].length, buf);
		print(buf);
		print("\n");
		
		print("type: ");
		switch(memory_map[i].type) {
			case REGION_TYPE_USABLE:
				print("REGION_TYPE_USABLE\n");
				break;	
			case REGION_TYPE_RESERVED:
				print("REGION_TYPE_RESERVED\n");
				break;
			case REGION_TYPE_ACPI_RECLAIMABLE:
				print("REGION_TYPE_ACPI_RECLAIMABLE\n");
				break;
			case REGION_TYPE_ACPI_NVS:
				print("REGION_TYPE_ACPI_NVS\n");
				break;
			case REGION_TYPE_BAD_MEMORY:
				print("REGION_TYPE_BAD_MEMORY\n");
				break;
		}
		
		if(memory_map[i].type == REGION_TYPE_USABLE || memory_map[i].type == REGION_TYPE_ACPI_RECLAIMABLE) {
			total_available += memory_map[i].length;
		}
	}
}

void *malloc(unsigned int size)
{
	
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