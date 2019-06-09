// lexer.cpp

#include "lexer.h"

#include <stdio.h>

inline int isEndOfLine(char c) {
    return (c == '\n' || c == '\r');
}

inline int isWhiteSpace(char c) {
    return (
        c == '\t' ||
        c == '\v' ||
        c == '\f' ||
        c == ' '
    );
}

inline int isAlphabetical(char c) {
    return ((c >= 'a') && (c <= 'z')) ||
        ((c >= 'A') && (c <= 'Z'));
}

inline int isNumber(char c) {
    return ((c >= '0') && (c <= '9'));
}

void removeWhiteSpaces(struct Lexer* lexer) {
    while (1) {
        if (isWhiteSpace(lexer->index[0])) {
            lexer->index++;
        }
        else if (lexer->index[0] == '/' && lexer->index[1] == '/') {  // Comment
            lexer->index += 2;
            while (lexer->index[0] && !isEndOfLine(lexer->index[0])) {
                lexer->index++;
            }
        }
        else if (lexer->index[0] == '#') {  // Skip macros
            lexer->index++;
            while (lexer->index[0] && !isEndOfLine(lexer->index[0])) {
                if (lexer->index[0] == '\\') {
                    lexer->index++;
                }
                lexer->index++;
            }
        }
        else if (lexer->index[0] == '/' && lexer->index[1] == '*') {  // Multi-line comment
            lexer->index += 2;
            while ((lexer->index[0] && !(lexer->index[0] == '*' && lexer->index[1] == '/'))) {
                if (isEndOfLine(lexer->index[0])) {
                    lexer->line++;
                }
                lexer->index++;
            }
            if (lexer->index[0] == '*') {
                lexer->index += 2;
            }
        }
        else {
            break;
        }
    }
}

struct Token lexerNextToken(struct Lexer* lexer) {
    removeWhiteSpaces(lexer);
    struct Token token = {
        .length = 1,
        .string = lexer->index
    };

    char ch = lexer->index[0];
    lexer->index++;
    lexer->count++;
    switch (ch) {
        case '=':
            token.type = T_ASSIGN;
            break;
        
        case ';':
            token.type = T_SEMICOLON;
            break;

        case '+':
            token.type = T_ADD;
            break;

        case '-':
            token.type = T_SUB;
            break;

        case '*':
            token.type = T_MULT;
            break;

        case '/':
            token.type = T_DIV;
            break;

        case '(':
            token.type = T_LEFTPAREN;
            break;
        
        case ')':
            token.type = T_RIGHTPAREN;
            break;

        case '\r':
        case '\n':
            token.type = T_NEWLINE;
            break;

        case '"': {
            token.type = T_STRING;
            token.string = lexer->index;

            while (lexer->index[0] && lexer->index[0] != '"') {
                if (lexer->index[0] == '\\' && lexer->index[1]) {
                    lexer->index++;
                }
                lexer->index++;
            }

            token.length = lexer->index - token.string;
            if (lexer->index[0] == '"') {
                lexer->index++;
            }
        }
            break;

        default: {
            if (isAlphabetical(ch) || ch == '_') {
                while (
                    isAlphabetical(lexer->index[0]) ||
                    isNumber(lexer->index[0]) ||
                    lexer->index[0] == '_'
                ) {
                    lexer->index++;
                }
                token.type = T_IDENTIFIER;
                token.length = lexer->index - token.string;
            }
            else if (isNumber(ch)) {
                while (isNumber(lexer->index[0]) || lexer->index[0] == '.') {
                    lexer->index++;
                }
                token.type = T_NUMBER;
                token.length = lexer->index - token.string;
            }
            else {
                token.type = T_UNKNOWN;
            }
        }
            break;
    }
    lexer->token = token;
    return token;
}

void lexerGetTokenValue(char* buffer, struct Token token) {
    sprintf(buffer, "%.*s", token.length, token.string);
}

int lexerTokenEquals(struct Token token, char* toMatch) {
    char* index = toMatch;
    for (int i = 0; i < token.length; i++, index++) {
        if ((*index == '\0') || token.string[i] != *index) {
            return 0;
        }
    }
    return (*index == 0);
}

bool lexerExpectToken(struct Token token, int type) {
    return token.type == type;
}

void lexerPrintToken(struct Token token) {
  if (token.length) {
    printf("%.*s\n", token.length, token.string);
  }
}