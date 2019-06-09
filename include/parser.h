// parser.h

#ifndef _PARSER_H
#define _PARSER_H

#include <stdio.h>
#include <stdlib.h>

struct Interpreter;

struct Operator {
    unsigned char left,
        right;
};

static const struct Operator priority[] = {
    {0, 0},
    {10, 10}, {10, 10},     // + -
    {11, 11}, {11, 11},     // * /
    {3, 3}, {3, 3}, {3, 3}, // <, >, ==
    {3, 3}, {3, 3}, {3, 3}  // <=, >=, !=
};

int parse(struct Interpreter* vm, char* input);

#endif