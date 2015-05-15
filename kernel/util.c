void memory_copy(char *source, char *dest, int count) {
	int i;
	for(i = 0; i < count; i++) {
		*(dest + i) = *(source + i);
	}
}