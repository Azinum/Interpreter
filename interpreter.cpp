// interpreter.cpp

#include "interpreter.h"
#include "parser.h"
#include "lexer.h"
#include "file.h"
#include "object.h"
#include "code.h"
#include "opcodes.h"

#define VM_NEXT goto *(program[++ip])
#define VM_SKIP(COUNT) (ip += COUNT)

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
    unsigned int ip = 0;

    void* instructions[] = {
        NULL,
        &&ADD,
        &&SUB,
        &&MULT,
        &&DIV,

        &&POP,
        &&PUSH,

        &&EXIT
    };

    std::vector<void*> program;
    
    for (int i = 0; i < vm->code.size(); i++) {
        int instruction = vm->code[i];

        program.push_back(instructions[instruction]);

        switch (instruction) {
            // 1 arg, skip 1 instruction
            case OP_PUSH: {
                program.push_back(instructions[instruction]);
                i++;
            }
                break;

            default:
                break;
        }
    }

    program.push_back(instructions[OP_EXIT]);

    goto *(program[0]);

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
        int pointer = vm->code[ip + 1];
        struct Object obj;
        if (pointer < vm->storage.size()) {
            obj = vm->storage[pointer];
            vm->stack[vm->stackPointer++] = obj;
        }
        ip++;
    });
    VM_CASE(POP, {
        if (vm->stackPointer > 0) {
            vm->stackPointer--;
        }
    });
    VM_CASE(EXIT, {
        printTop(vm, 1);
        return vm->status;
    });
    return 0;
}

int interpreter(int argc, char** argv) {
    int status = 0;
    struct Interpreter interpreter = {};
    interpreter.out = fopen("out.tmp", "w");

    // char* read = readFile("scripts/test.lang");

    // if (read) {
    //     status = parse(&interpreter, read);
    //     interpreterExecute(&interpreter);
    //     free(read);
    // }

    char* input = NULL;
    unsigned long size = 0;

    while (true) {
        printf("> ");
        if (getline(&input, &size, stdin) > 0) {
            status = parse(&interpreter, input);
            interpreterExecute(&interpreter);
        }
    }

    if (interpreter.out)
        fclose(interpreter.out);
    return status;
}