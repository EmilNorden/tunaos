#include "util.h"
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
		buffer[char_count++] = '-';
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
	
	return 0;
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