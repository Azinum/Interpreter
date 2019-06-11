// code.h (Code generator)

#ifndef _CODE_H
#define _CODE_H

struct Interpreter;

int storeNumber(struct Interpreter* vm, double number);

int storeObject(struct Interpreter* vm, struct Object object);

void codePushObject(struct Interpreter* vm, struct Token token);

int codePushVariable(struct Interpreter* vm, struct Token token);

void codeAssign(struct Interpreter* vm, int location);

void codePop(struct Interpreter* vm);

void codeIfBegin(struct Interpreter* vm, int* writeIndex);

void codeWriteAt(struct Interpreter* vm, int what, int index);

void codeOperator(struct Interpreter* vm, struct Token token);

#endif