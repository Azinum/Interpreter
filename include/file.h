// file.h

#ifndef _FILE_H
#define _FILE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

char* readFile(const char* path);

void writeOut(FILE* file, const char* format, ...);

#endif