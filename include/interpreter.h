// interpreter.h

#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "object.h"

#include <vector>

#define STACK_SIZE 128

struct Interpreter {
    std::vector<int> code;
    std::vector<struct Object> storage;
    struct Object stack[STACK_SIZE];
    int stackPointer;
    FILE* out;
};

int interpreter(int argc, char** argv);

#endif