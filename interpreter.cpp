// interpreter.cpp

#include "interpreter.h"
#include "parser.h"
#include "lexer.h"
#include "file.h"
#include "object.h"
#include "code.h"
#include "opcodes.h"

void printTop(struct Interpreter* vm, int offset) {
    if (vm->stackPointer - offset >= 0)
        printf("top: %g\n", vm->stack[vm->stackPointer - offset].value.number);
}

int interpreterExecute(struct Interpreter* vm) {

    unsigned int ip = 0;    // Instruction pointer
    for (; ip < vm->code.size(); ip++) {
        switch (vm->code[ip]) {
            case OP_PUSH: {
                int pointer = vm->code[ip + 1];
                struct Object obj;
                if (pointer < vm->storage.size()) {
                    obj = vm->storage[pointer];
                    vm->stack[vm->stackPointer++] = obj;
                }
                ip++;
            }
                break;

            case OP_ADD: {
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

            case OP_MULT: {
                if (vm->stackPointer > 1) {
                    vm->stack[vm->stackPointer-2].value.number = vm->stack[vm->stackPointer-2].value.number
                    * vm->stack[vm->stackPointer-1].value.number;
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

int interpreter(int argc, char** argv) {
    int status = 0;
    struct Interpreter interpreter = {};
    interpreter.out = fopen("out.tmp", "w");

    char* read = readFile("scripts/test.lang");

    if (read) {
        status = parse(&interpreter, read);
        interpreterExecute(&interpreter);
        free(read); 
    }
    if (interpreter.out)
        fclose(interpreter.out);
    return status;
}