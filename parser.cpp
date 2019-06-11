// parser.cpp

#include "parser.h"
#include "lexer.h"
#include "interpreter.h"
#include "code.h"

#include <stdlib.h>
#include <stdio.h>

void error(struct Lexer* lexer, const char* message, struct Token token);
struct Operator getOperator(struct Token token);
inline bool tupleEnd(struct Lexer* lexer);
inline bool blockEnd(struct Lexer* lexer);

void statements(struct Lexer* lexer);
void statement(struct Lexer* lexer);
void simpleExpression(struct Lexer* lexer);
struct Operator expression(struct Lexer* lexer, int priority);
void ifStatement(struct Lexer* lexer);

void error(struct Lexer* lexer, const char* message, struct Token token) {
    char buffer[128] = {0};
    lexerGetTokenValue(buffer, token);
    printf("[SyntaxError] %s near '%s' (line: %i)\n", message, buffer, lexer->line);
}

struct Operator getOperator(struct Token token) {
    struct Operator op = {0};
    if (token.type > T_UNKNOWN && token.type < T_NOOP) {
        op = priority[token.type];
    }
    return op;
}

bool tupleEnd(struct Lexer* lexer) {
    int type = lexer->token.type;
    if (type == T_EOF) {
        error(lexer, "Missing closing ')' parenthesis", lexer->token);
        return true;
    }
    return (type == T_RIGHTPAREN);
}

bool blockEnd(struct Lexer* lexer) {
    return (lexer->token.type == T_BLOCKEND);
}

bool statementsEnd(struct Lexer* lexer) {
    int type = lexer->token.type;
    return (type == T_BLOCKEND || type == T_EOF);
}

void statements(struct Lexer* lexer) {
    while (!statementsEnd(lexer)) {
        statement(lexer);
    }
}

void statement(struct Lexer* lexer) {
	struct Token token = lexer->token;

	switch (token.type) {
        case T_EOF:
            break;
        case T_NEWLINE:
            lexerNextToken(lexer);
            break;
        case T_SEMICOLON:
            lexerNextToken(lexer);
            break;
        case T_IF:
            ifStatement(lexer);
            break;
		default:
			expression(lexer, 0);
            break;
	}
}

void simpleExpression(struct Lexer* lexer) {
    struct Token token = lexer->token;

    switch (token.type) {
        case T_IDENTIFIER: {
            int location = codePushVariable(lexer->vm, token);
            lexerNextToken(lexer);  // Skip identifier
            if (lexer->token.type == T_ASSIGN) {
                lexerNextToken(lexer);  // Skip '='
                // expression(lexer, 0);
                statement(lexer);
                codeAssign(lexer->vm, location);
                if (lexerTokenIs(lexer, T_SEMICOLON)) {
                    codePop(lexer->vm);
                }
            }
            break;
        }
        case T_NUMBER: {
            codePushNumber(lexer->vm, token);
            lexerNextToken(lexer);
            break;
        }
        case T_LEFTPAREN: {
            lexerNextToken(lexer);  // Skip '('
            if (!tupleEnd(lexer)) {
                // expression(lexer, 0);
                statement(lexer);
            }
            if (lexerExpectToken(lexer->token, T_RIGHTPAREN)) {
                lexerNextToken(lexer);   // Skip ')'
                
                if (lexerTokenIs(lexer, T_LEFTPAREN)) {
                    error(lexer, "", lexer->token);
                }
            } else {
                error(lexer, "Missing closing ')' parenthesis", lexer->token);
            }
            break;
        }

        default:
            lexerNextToken(lexer);
            break;
    }
}

// (expr) op (expr)
struct Operator expression(struct Lexer* lexer, int priority) {
	// TODO: Add unary operators
    simpleExpression(lexer);

    struct Operator op = getOperator(lexer->token);
    struct Token token = lexer->token;

    while (token.type != T_NOOP && op.left > priority) {
        struct Operator nextOp;
        token = lexer->token;
        lexerNextToken(lexer);
        nextOp = expression(lexer, op.right);
        codeOperator(lexer->vm, token);
        op = nextOp;
    }
    return op;
}

void ifStatement(struct Lexer* lexer) {
    int writeIndex = -1;  // What index to write the jump location to
    lexerNextToken(lexer);  // Skip 'if'
    if (!lexerExpectToken(lexer->token, T_LEFTPAREN)) {
        error(lexer, "Missing condition", lexer->token);
        return;
    }
    expression(lexer, 0);   // read condition
    codeIfBegin(lexer->vm, &writeIndex);
    if (!lexerExpectToken(lexer->token, T_BLOCKBEGIN)) {
        error(lexer, "Missing block", lexer->token);
        return;
    }
    lexerNextToken(lexer);  // Skip '{'
    statements(lexer);
    if (!lexerExpectToken(lexer->token, T_BLOCKEND)) {
        error(lexer, "Missing block end '}'", lexer->token);
    }
    lexerNextToken(lexer);  // Skip '}'
    if (writeIndex >= 0) {
        int jumpTo = lexer->vm->code.size()-1;
        codeWriteAt(lexer->vm, jumpTo, writeIndex);
    }
}

int parse(struct Interpreter* vm, char* input) {
	struct Lexer lexer = {};
    lexer.vm = vm;
	lexer.line = 1;
	lexer.index = input;
    lexerNextToken(&lexer); // Read first token
    statements(&lexer);
	return 0;
}