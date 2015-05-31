#ifndef DRIVER_SCREEN_H_
#define DRIVER_SCREEN_H_

#define VIDEO_ADDRESS 			0xb8000
#define MAX_ROWS				25
#define MAX_COLS				80
#define WHITE_ON_BLACK			0x0f

/* Screen device I/O ports */
#define REG_SCREEN_CTRL 		0x3D4
#define REG_SCREEN_DATA			0x3D5

/* Device internal register indices */
#define REG_INTERNAL_HIOFFSET	14
#define REG_INTERNAL_LOOFFSET	15

void clear_screen(void);
void print_at(char *message, int col, int row);
void print(char *message);

#endif