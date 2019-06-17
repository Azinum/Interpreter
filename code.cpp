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

int storeObject(struct Interpreter* vm, struct Object object) {
  int location = vm->storage.size();
  vm->storage.push_back(object);
  return location;
}

void codePushObject(struct Interpreter* vm, struct Token token) {
  if (!vm) return;
  int object = -1;

  switch (token.type) {
    case T_NUMBER: {
      char buffer[128] = {0};
      lexerGetTokenValue(buffer, token);
      if (!isNumber(buffer)) {
        printf("%s\n", "Invalid number");
        return;
      }

      double number = stringToNumber(buffer);
      object = storeNumber(vm, number);
      if (object < 0) {
        printf("%s\n", "Failed to store number");
        return;
      }
      break;
    }

    case T_STRING: {
      struct String string = (struct String) {
        .index = &token.string[0],
        .length = token.length
      };
      struct Object stringObject = (struct Object) {
        .type = T_STRING,
        .value = {
          .string = string
        }
      };
      object = storeObject(vm, stringObject);
      break;
    }
  }

  vm->code.push_back(OP_PUSH);
  vm->code.push_back(object);
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
  return location;
}

void codeAssign(struct Interpreter* vm, int location) {
  if (!vm) return;

  vm->code.push_back(OP_ASSIGN);
  vm->code.push_back(location);
}

// Add OP_POP instruction
void codePop(struct Interpreter* vm) {
  if (!vm) return;

  vm->code.push_back(OP_POP);
}

void codeIfBegin(struct Interpreter* vm, int* writeIndex) {
  if (!vm) return;

  vm->code.push_back(OP_IF);
  *writeIndex = vm->code.size();
  vm->code.push_back(-1);
}

void codeWriteAt(struct Interpreter* vm, int what, int index) {
  if (!vm) return;
  if (index < vm->code.size()) vm->code[index] = what;
}

// Generate code for any arithmetic operator
void codeOperator(struct Interpreter* vm, struct Token token) {
  int opCode = toOpCode(token.type);
  if (opCode == OP_UNKNOWN) {
    printf("%s\n", "Invalid operator");
    return;
  }
  vm->code.push_back(opCode);
  // writeOut(vm->out, "%s,\n", opCodeToString(opCode));
}