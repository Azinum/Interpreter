// interpreter.cpp

#include "interpreter.h"
#include "parser.h"
#include "lexer.h"
#include "file.h"
#include "object.h"
#include "code.h"
#include "opcodes.h"

#define VM_NEXT goto *(vm->program[++vm->ip])
#define VM_SKIP(COUNT) (vm->ip += COUNT)

#define VM_CASE(CASE, BODY, ...) { \
    CASE : { BODY } \
    VM_NEXT; \
}

#define OP_ARITH(OP) \
if (vm->stackPointer > 1) { \
    struct Object left, right; \
    left = vm->stack[vm->stackPointer - 2]; \
    right = vm->stack[vm->stackPointer - 1]; \
    if (OP_SAMETYPE(left, right, T_NUMBER)) { \
        vm->stack[vm->stackPointer - 2].value.number = left.value.number OP right.value.number; \
        vm->stackPointer--; \
    } \
} \

#define OP_SAMETYPE(LEFT, RIGHT, TYPE) (LEFT.type == TYPE && RIGHT.type == TYPE)

// printTop(1) is top of stack
void printTop(struct Interpreter* vm, int offset) {
    if (vm->stackPointer - offset >= 0) {
        struct Object object = vm->stack[vm->stackPointer - offset];
        if (object.type == T_NUMBER)
            printf("%g\n", object.value.number);
    }
}

int interpreterExecute(struct Interpreter* vm) {
    void* instructions[] = {
        NULL,
        &&ADD,
        &&SUB,
        &&MULT,
        &&DIV,

        &&POP,
        &&PUSH,
        &&PUSH_VAR,
        &&ASSIGN,

        &&EXIT
    };
    
    for (; vm->codeip < vm->code.size(); vm->codeip++) {
        int instruction = vm->code[vm->codeip];

        vm->program.push_back(instructions[instruction]);

        switch (instruction) {
            // 1 arg, skip 1 instruction
            case OP_ASSIGN:
            case OP_PUSH_VAR:
            case OP_PUSH: {
                vm->program.push_back(instructions[instruction]);
                vm->codeip++;
            }
                break;

            default:
                break;
        }
    }

    vm->program.push_back(instructions[OP_EXIT]);

    goto *(vm->program[vm->ip]);

    VM_CASE(ADD, {
        OP_ARITH(+);
    });
    VM_CASE(SUB, {
        OP_ARITH(-);
    });
    VM_CASE(MULT, {
       OP_ARITH(*);
    });
    VM_CASE(DIV, {
       OP_ARITH(/);
    });
    VM_CASE(PUSH, {
        int pointer = vm->code[vm->ip + 1];
        struct Object obj;
        if (pointer < vm->storage.size()) {
            obj = vm->storage[pointer];
            vm->stack[vm->stackPointer++] = obj;
        }
        vm->ip++;
    });
    VM_CASE(PUSH_VAR, {
        int pointer = vm->code[vm->ip + 1];
        struct Object obj;
        if (pointer < vm->current->variables.size() && pointer != -1) {
            obj = vm->current->variables[pointer];
            vm->stack[vm->stackPointer++] = obj;
        }
        vm->ip++;
    });
    VM_CASE(ASSIGN, {
        int location;
        struct Object right;
        location = vm->code[vm->ip + 1];
        right = vm->stack[vm->stackPointer - 1];
        if (right.type == T_NUMBER) {
            vm->current->variables[location] = right;
            vm->stackPointer--;
        }
        vm->ip++;
    });
    VM_CASE(POP, {
        if (vm->stackPointer > 0) {
            vm->stackPointer--;
        }
    });
    EXIT: {
        printTop(vm, 1);
        vm->program.pop_back();
        return vm->status;
    }
    return 0;
}

int storeVariable(struct Interpreter* vm, const char* name, struct Object object) {
    if (!vm) return -1;
    if (variableExists(vm, name)) {
        return -1;
    }
    int location = vm->current->variables.size();
    vm->current->varLocations[name] = location;
    vm->current->variables.push_back(object);
    return location;
}

// Store empty variable
int storeVariable2(struct Interpreter* vm, const char* name) {
    struct Object object = { .type = T_EMPTYVAR };
    int location = storeVariable(vm, name, object);
    return location;
}

int getVariableLocation(struct Interpreter* vm, const char* name) {
    if (!vm) return -1;
    if (!variableExists(vm, name)) {
        printf("Variable '%s' doesn't exist\n", name);
        return -1;
    }
    int location = vm->current->varLocations[name];
    return location;
}

bool variableExists(struct Interpreter* vm, const char* name) {
    return vm->current->varLocations.count(name) != 0;
}

int interpreter(int argc, char** argv) {
    int status = 0;
    struct Interpreter interpreter = {};
    interpreter.out = fopen("out.tmp", "w");
    interpreter.current = &interpreter.global;
    interpreter.current->parent = NULL;

    storeVariable(&interpreter, "a", (struct Object) {
        .type = T_NUMBER,
        .value = { .number = 15 }
    });

    if (argc >= 2) {    // Execute file
        char* read = readFile(argv[1]);
        if (read) {
            status = parse(&interpreter, read);
            interpreterExecute(&interpreter);
            free(read);
        }
    } else {
        char* input = NULL;
        unsigned long size = 0;

        while (true) {
            printf("> ");
            if (getline(&input, &size, stdin) > 0) {
                status = parse(&interpreter, input);
                interpreterExecute(&interpreter);
            }
        }
    }

    if (interpreter.out) fclose(interpreter.out);
    return status;
}