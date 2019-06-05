// str.cpp

#include "str.h"

#include <limits.h>
#include <string.h>
#include <stdlib.h>

bool isNumber(const char* string) {
    if (strlen(string) <= 0)
        return false;
    
    char* end;
    strtod(string, &end);

    if (*end != '\0')
        return false;

    return true;
}

double stringToNumber(const char* string) {    
    char* end;
    double number = strtod(string, &end);
    
    if (*end != '\0')
        return INT8_MIN;
    
    return number;
}