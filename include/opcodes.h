// opcodes.h

#ifndef _OPCODES_H
#define _OPCODES_H

enum OpCodes {
    OP_UNKNOWN,

    OP_ADD,
    OP_SUB,
    OP_MULT,
    OP_DIV,
    OP_LT,
    OP_GT,
    OP_EQ,
    OP_LEQ,
    OP_GEQ,
    OP_NEQ,

    OP_POP,
    OP_PUSH,
    OP_PUSH_VAR,
    OP_ASSIGN,
    OP_IF,


    OP_EXIT,
    OP_CODES_COUNT,
};

// For logging purposes
static const char* opCodesStrings[] = {
    "OP_UNKNOWN",

    "OP_ADD",
    "OP_SUB",
    "OP_MULT",
    "OP_DIV",
    "OP_LT",
    "OP_GT",
    "OP_EQ",
    "OP_LEQ",
    "OP_GEQ",
    "OP_NEQ",

    "OP_POP",
    "OP_PUSH",
    "OP_PUSH_VAR",
    "OP_ASSIGN",
    "OP_IF",
};

int toOpCode(int tokenType);

const char* opCodeToString(int opcode);

#endif