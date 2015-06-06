#ifndef DRIVER_SCREEN_H_
#define DRIVER_SCREEN_H_

#define COLOR_BLACK				0x0
#define COLOR_BLUE				0x1
#define COLOR_GREEN				0x2
#define COLOR_CYAN				0x3
#define COLOR_RED				0x4
#define COLOR_MAGENTA			0x5
#define COLOR_BROWN				0x6
#define COLOR_LIGHTGRAY			0x7

#define BRIGHT(x) (x | 0x08)

// "Bake" a color attribute. Background color in top 4 bits, foreground in lower 4 bits.
#define COLOR_ATTR(fg, bg) (bg << 4) | (fg & 0xF)

#define VIDEO_ADDRESS 			0xb8000
#define MAX_ROWS				25
#define MAX_COLS				80
#define WHITE_ON_BLACK			COLOR_ATTR(BRIGHT(COLOR_LIGHTGRAY), COLOR_BLACK)

/* Screen device I/O ports */
#define REG_SCREEN_CTRL 		0x3D4
#define REG_SCREEN_DATA			0x3D5

/* Device internal register indices */
#define REG_INTERNAL_HIOFFSET	14
#define REG_INTERNAL_LOOFFSET	15

void clear_screen(void);
void clear_screen_clr(unsigned char color);
void print_at(const char *message, int col, int row);
void print_at_clr(const char *message, int col, int row, unsigned char color);
void print(const char *message);
void print_clr(const char *message, unsigned char color);

#endif