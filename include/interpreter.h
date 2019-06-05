// interpreter.h

#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "object.h"

#include <vector>

struct Interpreter {
    std::vector<int> code;
    std::vector<struct Object> storage;
    struct Object stack[128];
    int stackPointer;
    FILE* out;
};

int interpreter(int argc, char** argv);

#endif