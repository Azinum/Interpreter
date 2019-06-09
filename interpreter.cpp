// interpreter.cpp

#include "interpreter.h"
#include "parser.h"
#include "lexer.h"
#include "file.h"
#include "object.h"
#include "code.h"
#include "opcodes.h"

#include <signal.h>
#include <math.h>

#define INPUT_MAX 128

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

static struct Interpreter* _interpreter;

void printTop(struct Interpreter* vm, int offset);
void exitInterpreter(int i);

inline void stackPop(struct Interpreter* vm);
inline void clearStack(struct Interpreter* vm);

inline void stackPop(struct Interpreter* vm) {
    if (vm->stackPointer > 0) {
        vm->stackPointer--;
    }
}

inline void clearStack(struct Interpreter* vm) {
    vm->stackPointer = 0;
}

// printTop(1) is top of stack
void printTop(struct Interpreter* vm, int offset) {
    int index = vm->stackPointer - offset;
    if (index >= 0 && index < STACK_SIZE) {
        struct Object object = vm->stack[vm->stackPointer - offset];
        if (object.type == T_NUMBER)
            printf("%g\n", object.value.number);
    }
}

void exitInterpreter(int i) {
    if (_interpreter) {
        _interpreter->isRunning = false;
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
        struct Object toAssign;
        location = vm->code[vm->ip + 1];
        toAssign = vm->stack[vm->stackPointer - 1];
        if (toAssign.type == T_NUMBER) {
            vm->current->variables[location] = toAssign;
            // printTop(vm, 1);
            // stackPop(vm);
        }
        // stackPop(vm);
        vm->ip++;
    });
    VM_CASE(POP, {
        stackPop(vm);
    });
    EXIT: {
        printTop(vm, 1);
        stackPop(vm);
        vm->program.pop_back(); // Remove EXIT instruction
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

int storeEmptyVariable(struct Interpreter* vm, const char* name) {
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
    _interpreter = &interpreter;
    interpreter.isRunning = true;
    interpreter.out = fopen("out.tmp", "w");
    interpreter.current = &interpreter.global;
    signal(SIGINT, exitInterpreter);

    storeVariable(&interpreter, "pi", (struct Object) {
        .type = T_NUMBER,
        .value = { .number = M_PI }
    });

    if (argc >= 2) {    // Execute file
        char* read = readFile(argv[1]);
        if (read) {
            status = parse(&interpreter, read);
            interpreterExecute(&interpreter);
            free(read);
        }
    } else {
        char input[INPUT_MAX] = {0};

        while (interpreter.isRunning) {
            printf("> ");
            if (fgets(input, INPUT_MAX, stdin) != NULL) {
                status = parse(&interpreter, input);
                interpreterExecute(&interpreter);  
            } else {
                interpreter.isRunning = false;
            }
        }
    }
    writeOut(interpreter.out, "[stack size: %i]\n", interpreter.stackPointer);
    writeOut(interpreter.out, "[program size: %i]\n", interpreter.program.size());
    if (interpreter.out) fclose(interpreter.out);
    return status;
}