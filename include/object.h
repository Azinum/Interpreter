// object.h

#ifndef _OBJECT_H
#define _OBJECT_H

#include <vector>
#include <map>

union ObjValue {
    double number;
    void* pointer;
    struct Object* object;
};

struct Object {
    unsigned int type;
    union ObjValue value;
};

struct Scope {
    struct Scope* parent;
    std::map<const char*, int> varLocations;
    std::vector<struct Object> variables;
};

#endif