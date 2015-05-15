#include "screen.h"
#include "../kernel/low_level.h"
#include "../kernel/util.h"


int get_screen_offset(int col, int row);
int get_cursor();
int handle_scrolling(int offset);
void print_char(char character, int col, int row, char attribute);
void set_cursor(int offset);

void print_char(char character, int col, int row, char attribute) {
	
	unsigned char *vidmem = (unsigned char*)VIDEO_ADDRESS;
	
	if(!attribute) {
		attribute = WHITE_ON_BLACK;
	}
	
	int offset;
	if(col >= 0 && row >= 0) {
		offset = get_screen_offset(col, row);
	}
	else {
		offset = get_cursor();
	}	
	
	/* If we see a new line character, set offset to the end of current row 
	 so it will be advanced to the first col of the next row */
	if(character == '\n') {
		int rows = offset / (2*MAX_COLS);
		offset = get_screen_offset(79, rows);
	}
	else {
		vidmem[offset] = character;
		vidmem[offset+1] = attribute;
	}
	
	/* Update the offset to the next character cell, which is two bytes ahead of the current cell */
	offset += 2;
	
	/* Make scrolling adjustment, for when we reach the bottom of the screen */
	offset = handle_scrolling(offset);
	
	/* Update the cursor position on the screen device */
	set_cursor(offset);
}

int get_screen_offset(int col, int row) {
	return (row * MAX_COLS + col) * 2;	
}

int get_cursor() {
	int offset;
	/* 
		The device uses its control register as an index to select its
		internal registers, of which we are interested in:
			reg 14: high byte of the cursor's offset
			reg 15: low byte of the cursor's offset
	*/
	
	port_byte_out(REG_SCREEN_CTRL, REG_INTERNAL_HIOFFSET);
	offset = port_byte_in(REG_SCREEN_DATA) << 8;
	
	port_byte_out(REG_SCREEN_CTRL, REG_INTERNAL_LOOFFSET);
	offset |= port_byte_in(REG_SCREEN_DATA);
	
	/* 
		The offset reported by the VGA hardware is the number of characters, so
		we need to multiply by 2 to get byte offset
	 */
	 return offset * 2;
}

void set_cursor(int offset) {
	/* 
		Convert from byte offset to character offset. For info on the rest of the code,
		check get_cursor()
	*/
	offset /= 2;
	
	port_byte_out(REG_SCREEN_CTRL, REG_INTERNAL_HIOFFSET);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, REG_INTERNAL_LOOFFSET);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)offset);
}

void print_at(char *message, int col, int row) {
	if(col >= 0 && row >= 0) {
		set_cursor(get_screen_offset(col, row));
	}
	
	int i = 0;
	while(message[i] != 0) {
		print_char(message[i++], col, row, WHITE_ON_BLACK);
	}
}

void print(char *message) {
	print_at(message, -1, -1);
}

void clear_screen() {
	int row;
	int col;
	
	for(row = 0; row < MAX_ROWS; row++) {
		for(col = 0; col < MAX_COLS; col++) {
			print_char(' ', col, row, WHITE_ON_BLACK);
		}
	}
	
	set_cursor(get_screen_offset(0, 0));
}

int handle_scrolling(int offset) {
	char *vidmem = (char*)VIDEO_ADDRESS;
	/* If the cursor is still within the bounds of the screen, return it unmodified */
	if(offset < MAX_ROWS * MAX_COLS * 2) {
		return offset;
	}
	
	/* Move all rows up by one */
	int i;
	for(i = 1; i < MAX_ROWS; i++) {
		memory_copy(
			&vidmem[get_screen_offset(0, i)],
			&vidmem[get_screen_offset(0, i-1)],
			MAX_COLS * 2
		);
	}
	
	/* Blank out last row */
	char *last_line = &vidmem[get_screen_offset(0, MAX_ROWS - 1)];
	for(i = 0; i < MAX_COLS; i++) {
		last_line[i] = 0;
	}
	
	/* Move cursor back one row */
	offset -= 2 * MAX_COLS;
	
	return offset;
}