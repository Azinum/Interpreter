// code.cpp

#include "code.h"
#include "opcodes.h"
#include "object.h"
#include "lexer.h"
#include "interpreter.h"
#include "str.h"
#include "file.h"

#include <stdio.h>

int storeNumber(struct Interpreter* vm, double number) {
    int location = vm->storage.size();
    
    struct Object object = (struct Object) {
        T_NUMBER,
        { .number = number }
    };

    vm->storage.push_back(object);
    return location;
}

// TODO: Support any type
void codePushNumber(struct Interpreter* vm, struct Token token) {
    if (!vm) return;

    char buffer[128] = {0};
    lexerGetTokenValue(buffer, token);
    if (!isNumber(buffer)) {
        printf("%s\n", "Invalid number");
        return;
    }

    double number = stringToNumber(buffer);
    int object = storeNumber(vm, number);
    if (object < 0) {
        printf("%s\n", "Failed to store number");
        return;
    }
    vm->code.push_back(OP_PUSH);
    vm->code.push_back(object);

    writeOut(vm->out, "%s, %i,\n", opCodeToString(OP_PUSH), object);
}

int codePushVariable(struct Interpreter* vm, struct Token token) {
    if (!vm) return -1;
    char buffer[128] = {0};
    lexerGetTokenValue(buffer, token);

    int location;
    if (variableExists(vm, buffer)) {
        location = getVariableLocation(vm, buffer);
    } else {
        location = storeEmptyVariable(vm, buffer);
    }
    
    vm->code.push_back(OP_PUSH_VAR);
    vm->code.push_back(location);
    writeOut(vm->out, "%s, %i,\n", opCodeToString(OP_PUSH_VAR), location);
    return location;
}

void codeAssign(struct Interpreter* vm, int location) {
    if (!vm) return;

    vm->code.push_back(OP_ASSIGN);
    vm->code.push_back(location);
    writeOut(vm->out, "%s, %i\n", opCodeToString(OP_ASSIGN), location);
}

// Generate code for any arithmetic operator
void codeOperator(struct Interpreter* vm, struct Token token) {
    int opCode = toOpCode(token.type);
    if (opCode == OP_UNKNOWN) {
        printf("%s\n", "Invalid operator");
        return;
    }
    vm->code.push_back(opCode);
    writeOut(vm->out, "%s,\n", opCodeToString(opCode));
}