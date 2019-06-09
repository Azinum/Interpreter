// opcodes.cpp

#include "opcodes.h"
#include "lexer.h"

#include <stdlib.h>

// Operator to operation code
int toOpCode(int tokenType) {
    // TODO: Cleanup
    switch (tokenType) {
        case T_ADD: return OP_ADD;
        case T_SUB: return OP_SUB;
        case T_MULT: return OP_MULT;
        case T_DIV: return OP_DIV;
        case T_LT: return OP_LT;
        case T_GT: return OP_GT;
        case T_EQ: return OP_EQ;
        case T_LEQ: return OP_LEQ;
        case T_GEQ: return OP_GEQ;
        
        default: return OP_UNKNOWN;
    }
}

const char* opCodeToString(int opcode) {
    if (opcode < OP_CODES_COUNT)
        return opCodesStrings[opcode];
    return NULL;
}