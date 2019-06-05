// parser.h

#ifndef _PARSER_H
#define _PARSER_H

#include <stdio.h>
#include <stdlib.h>

struct Interpreter;

int parse(struct Interpreter* vm, char* input);

#endif