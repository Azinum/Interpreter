// opcodes.cpp

#include "opcodes.h"
#include "lexer.h"

#include <stdlib.h>

int toOpCode(int tokenType) {
    // TODO: Cleanup
    switch (tokenType) {
        case T_ADD:
            return OP_ADD;
        case T_SUB:
            return OP_SUB;
        case T_MULT:
            return OP_MULT;
        case T_DIV:
            return OP_DIV;
        
        default:
            break;
    }
    return OP_UNKNOWN;
}

const char* opCodeToString(int opcode) {
    if (opcode < OP_CODES_COUNT)
        return opCodesStrings[opcode];
    return NULL;
}