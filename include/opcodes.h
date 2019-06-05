// opcodes.h

#ifndef _OPCODES_H
#define _OPCODES_H

enum OpCodes {
    OP_UNKNOWN,

    OP_ADD,
    OP_SUB,
    OP_MULT,
    OP_DIV,

    OP_PUSH,
    OP_PUSH_OBJECT,
    OP_POP,
};

#endif