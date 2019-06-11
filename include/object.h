// object.h

#ifndef _OBJECT_H
#define _OBJECT_H

#include <vector>
#include <map>
#include <string>

struct String {
    char* index;
    int length;
};

union ObjValue {
    double number;
    void* pointer;
    struct String string;
    struct Object* object;
};

struct Object {
    unsigned char type;
    union ObjValue value;
};

struct Scope {
    struct Scope* parent;
    std::map<std::string, int> varLocations;
    std::vector<struct Object> variables;
};

#endif