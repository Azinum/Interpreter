// code.h (Code generator)

#ifndef _CODE_H
#define _CODE_H

struct Interpreter;

int storeNumber(struct Interpreter* vm, double number);

void codePushNumber(struct Interpreter* vm, struct Token token);

int codePushVariable(struct Interpreter* vm, struct Token token);

void codeAssign(struct Interpreter* vm, int location);

void codeOperator(struct Interpreter* vm, struct Token token);

#endif