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
    int pointer = vm->storage.size();
    
    struct Object object = (struct Object) {
        T_NUMBER,
        { .number = number }
    };

    vm->storage.push_back(object);
    return pointer;
}

void codePushNumber(struct Interpreter* vm, struct Token token) {
    if (!vm) return;

    char buffer[128] = {0};
    lexerGetTokenValue(buffer, token);
    if (!isNumber(buffer)) {
        printf("%s\n", "Invalid number");
        return;
    }

    int number = stringToNumber(buffer);
    int object = storeNumber(vm, number);
    if (object < 0) {
        printf("%s\n", "Failed to store number");
        return;
    }
    vm->code.push_back(OP_PUSH);
    vm->code.push_back(object);
}