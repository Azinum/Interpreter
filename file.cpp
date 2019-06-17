// file.cpp

#include "file.h"

char* readFile(const char* path) {
	char* buffer = NULL;
	long buffer_size, read_size;
	FILE* file = fopen(path, "rb");

	if (file == NULL) {
		printf("Failed to open file '%s\'\n", path);
		fclose(file);
		return buffer;
	}

	fseek(file, 0, SEEK_END);

	buffer_size = ftell(file);

	rewind(file);

	buffer = (char*)malloc(sizeof(char) * buffer_size);

	read_size = fread(buffer, sizeof(char), buffer_size, file);
	buffer[read_size] = '\0';

	if (buffer_size != read_size) {
		free(buffer);
		buffer = NULL;
	}

	fclose(file);
	return buffer;
}

void writeOut(FILE* file, const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(file, format, args);
  va_end(args);
}