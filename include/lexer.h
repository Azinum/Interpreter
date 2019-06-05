// lexer.h

#ifndef _LEXER_H
#define _LEXER_H

#define array_size(arr) ((sizeof(arr)) / (sizeof(arr[0])))

enum TokenTypes {
    T_UNKNOWN = 0,

    T_ADD,
    T_SUB,
    T_MULT,
    T_DIV,
    T_NOOP,

    T_ASSIGN,
    T_SEMICOLON,

    T_STRING,
    T_IDENTIFIER,
    T_NUMBER,
    T_VARIABLE, // let / var
    T_OBJECT,   // { ... }

    T_PUSH,
    T_POP,

    T_EOF,
};

struct Operator {
    unsigned char left,
        right;
};

static const struct Operator priority[] = {
    {0, 0},
    {10, 10}, {10, 10},     // + -
    {11, 11}, {11, 11},     // * /
};

struct Token {
    int type;
    int length;
    char* string;
};

struct Interpreter;

struct Lexer {
    struct Interpreter* vm;
    char* index;
    int line;
    struct Token token;
};

struct Token lexerNextToken(struct Lexer* lexer);

int lexerTokenEquals(struct Lexer* lexer, char* toMatch);

void lexerPrintToken(struct Token token);

#endif