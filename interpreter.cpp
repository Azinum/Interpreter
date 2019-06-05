// interpreter.cpp

#include "interpreter.h"
#include "parser.h"
#include "file.h"
#include "code.h"

struct Interpreter {
    struct Object stack[128];
    int stackPointer;
    std::vector<struct Object> storage;
};

int interpreterExecute(struct Interpreter* vm) {
    int code[] = {
        T_PUSH, 0,
        T_PUSH, 1,
        T_ADD
    };

    unsigned int ip = 0;    // Instruction pointer
    for (; ip < array_size(code) - 1;) {
        switch (code[ip++]) {
            case T_PUSH: {
                int pointer = code[ip];
                struct Object obj;
                if (pointer < vm->storage.size()) {
                    obj = vm->storage[pointer];
                    vm->stack[vm->stackPointer++] = obj;
                }
            }
                break;

            case T_ADD: {
                // Top of stack:
                // vm->stack[vm->stackPointer-1]
                if (vm->stackPointer > 1) {
                    vm->stack[vm->stackPointer-2].value.number = vm->stack[vm->stackPointer-2].value.number
                    + vm->stack[vm->stackPointer-1].value.number;
                    vm->stackPointer--;
                    printf("%g\n", vm->stack[vm->stackPointer-1].value.number);
                }
            }
                break;

            default:
                break;
        }
    }

//     void* instructions[] = {
//         &&ADD,
//         &&SUB,
//         &&EXIT
//     };

//     void** program;

//     (void)program;
//     (void)instructions;

// ADD:
//     puts("ADD");
// SUB:
//     puts("SUB");
// EXIT:
//     puts("EXIT");
    return 0;
}

void storeNumber(struct Interpreter* vm, double number) {
    vm->storage.push_back(
        (struct Object) {
            .type = T_NUMBER,
            {
                .number = number
            }
        }
    );
}

int interpreter(int argc, char** argv) {
    int status = 0;
    struct Interpreter interpreter = {
        .stackPointer = 0
    };

    storeNumber(&interpreter, 12);
    storeNumber(&interpreter, 14);
    interpreterExecute(&interpreter);
    // char* read = readFile("scripts/test.lang");
    // status = parse(read);
    // free(read);
    return status;
}