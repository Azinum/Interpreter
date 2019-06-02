// interpreter.cpp

#include "interpreter.h"
#include "parser.h"
#include "file.h"

struct Interpreter {

};

int interpreter(int argc, char** argv) {
    char* read = readFile("scripts/test.lang");
    int status = parse(read);
    free(read);
    return status;
}