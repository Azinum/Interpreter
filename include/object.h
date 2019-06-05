// object.h

#ifndef _OBJECT_H
#define _OBJECT_H

struct Object {
    unsigned int type;
    union Value {
        double number;
        void* pointer;
        struct Object* object;
    } value;
};

#endif