// interpreter.h

#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "object.h"

#include <vector>

#define STACK_SIZE 128

struct Interpreter {
    std::vector<int> code;
    std::vector<void*> program;
    unsigned int ip;
    unsigned int codeip;
    std::vector<struct Object> storage;
    struct Scope global;
    struct Scope* current;  // What scope are we in now?
    struct Object stack[STACK_SIZE];
    int stackPointer;
    int status;
    bool isRunning;
    FILE* out;
};

int storeVariable(struct Interpreter* vm, const char* name, struct Object object);

int storeVariable2(struct Interpreter* vm, const char* name);

int getVariableLocation(struct Interpreter* vm, const char* name);

bool variableExists(struct Interpreter* vm, const char* name);

int interpreter(int argc, char** argv);

#endif