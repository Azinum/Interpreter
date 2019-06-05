// opcodes.h

#ifndef _OPCODES_H
#define _OPCODES_H

enum OpCodes {
    OP_UNKNOWN,

    OP_ADD,
    OP_SUB,
    OP_MULT,
    OP_DIV,

    OP_POP,
    OP_PUSH,



    OP_EXIT,
    OP_CODES_COUNT,
};

// For logging purposes (and possibly caching!)
static const char* opCodesStrings[] = {
    "OP_UNKNOWN",

    "OP_ADD",
    "OP_SUB",
    "OP_MULT",
    "OP_DIV",

    "OP_PUSH",
    "OP_POP",
};

int toOpCode(int tokenType);

const char* opCodeToString(int opcode);

#endif