void memory_copy(char *source, char *dest, int count) {
	int i;
	for(i = 0; i < count; i++) {
		*(dest + i) = *(source + i);
	}
}

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

unsigned int strlen(const char *str)
{
	const char *base = str;
	while(*str) {
		str++;
	}
	
	return str - base;
}