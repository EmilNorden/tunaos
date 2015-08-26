#include "util.h"
#include "../drivers/screen.h"
#include <stdarg.h>

static void snprintf_signed_decimal_int(char **str, char *str_end, int value);
static void snprintf_string(char **str, char *str_end, char *value);

void int_to_string(int value, char *input)
{
	char *buffer = "                    ";
	const int divisor = 10;
	
	if(value == 0) {
		input[0] = '0';
		input[1] = '\0';
		return;
	}
	
	int char_count = 1;
	buffer[0] = '\0';
	
	if(value < 0) {
		*input++ = '-';
		value = -value;
	}
	
	while(value > 0) {
		int remainder = value % divisor;
		buffer[char_count++] = 48 + remainder;
		
		value = value / divisor;
	}
	
	for(int i = 0; i < char_count; ++i) {
		input[i] = buffer[char_count - i - 1];
	}
}

size_t strlen(const char *str)
{
	const char *base = str;
	while(*str) {
		str++;
	}
	
	return str - base;
}

int snprintf(char *str, size_t n, const char *format, int nargs, ...)
{
	va_list format_args;
	va_start(format_args, nargs);
	
	char *str_beg = str;
	char *str_end = str + n - 1;
	
	while(str != str_end && *format){
		char c = *format++;
		if(c == '%') {
			char token = *format++;
			if(token == 'd') {
				snprintf_signed_decimal_int(&str, str_end, va_arg(format_args, int));
			}
			else if(token == 's') {
				snprintf_string(&str, str_end, va_arg(format_args, char*));
			}
		}
		else {
			*str++ = c;
		}
		
	}
			
	*str = '\0';
	
	va_end(format_args);
	
	return str - str_end;
}

static void snprintf_signed_decimal_int(char **str, char *str_end, int value) {
	char buf[32];
	int_to_string(value, buf);
	
	char *pbuf = buf;
	
	while(*pbuf && *str != str_end) {
		**str = *pbuf;
		(*str)++;
		pbuf++;
	}
}

static void snprintf_string(char **str, char *str_end, char *value) {	
	while(*value && *str != str_end) {
		**str = *value;
		(*str)++;
		value++;
	}
}

int hex8(uint8_t value, char *buf) {
	const char *hex = "0123456789ABCDEF";
	
	buf[0] = hex[(value >> 4)];
	buf[1] = hex[value & 0xF];
	buf[2] = '\0';
}

int hex16(uint16_t value, char *buf) {
	const char *hex = "0123456789ABCDEF";
	
	buf[0] = hex[(value >> 12) & 0xF];
	buf[1] = hex[(value >> 8) & 0xF];
	buf[2] = hex[(value >> 4) & 0xF];
	buf[3] = hex[value & 0xF];
	buf[4] = '\0';
}

void hex_dump(void *data, size_t len) {
	const int bytes_per_row = 16;
	uint8_t *ptr = (uint8_t*)data;
	
	char hex_buffer[5];
	
	size_t rows = len / bytes_per_row;
	
	uint16_t row = 0;
	for(int i = 0; i < len; ++i) {
		if((i % bytes_per_row) == 0) {
			if(i != 0) {
				print("\n");
			}
			hex16(row * bytes_per_row, hex_buffer);
			print(hex_buffer);
			print(" ");
			
			row++;
		}
		
		hex8(ptr[i], hex_buffer);
		print(hex_buffer);
		print(" ");
		
		
	}
}