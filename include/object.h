// object.h

#ifndef _OBJECT_H
#define _OBJECT_H

union ObjValue {
    double number;
    void* pointer;
    struct Object* object;
};

struct Object {
    unsigned int type;
    union ObjValue value;
};

#endif