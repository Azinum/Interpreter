// code.h (Code generator)

#ifndef _CODE_H
#define _CODE_H

#include "lexer.h"

#include <map>

struct Object {
    unsigned int type;
    union Value {
        double number;
        void* pointer;
        struct Object* object;
    } value;
};

enum Instructions {
    OP_UNKNOWN,
    OP_ADD,
    OP_SUB,
    OP_MULT,
    OP_DIV,

    OP_PUSH_NUMBER,
    OP_PUSH_OBJECT,
    OP_PUSH_OBJECT_REF,
    OP_POP,
};

#endif