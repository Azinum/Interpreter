// parser.cpp

#include "parser.h"
#include "lexer.h"
#include "interpreter.h"
#include "code.h"

#include <stdlib.h>
#include <stdio.h>

void error(struct Lexer* lexer, const char* message, struct Token token);
struct Operator getOperator(struct Token token);
bool tupleEnd(struct Lexer* lexer);

void parseStatement(struct Lexer* lexer);
void parseSimpleExpression(struct Lexer* lexer);
struct Operator parseExpression(struct Lexer* lexer, int priority);

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

void parseStatement(struct Lexer* lexer) {
	struct Token token = lexer->token;

	switch (token.type) {
        // case T_NEWLINE:
        //     break;

        case T_SEMICOLON:
            lexerNextToken(lexer);
            break;

		default: {
			parseExpression(lexer, 0);
		}
			break;
	}
}

void parseSimpleExpression(struct Lexer* lexer) {
    struct Token token = lexer->token;

    switch (token.type) {
        case T_IDENTIFIER: {
            int location = codePushVariable(lexer->vm, token);
            lexerNextToken(lexer);  // Skip identifier
            if (lexer->token.type == T_ASSIGN) {
                lexerNextToken(lexer);  // Skip '='
                parseExpression(lexer, 0);
                codeAssign(lexer->vm, location);
            }
        }
            break;

        case T_NUMBER: {
            codePushNumber(lexer->vm, token);
            lexerNextToken(lexer);
        }
            break;

        case T_LEFTPAREN: {
            lexerNextToken(lexer);  // Skip '('
            if (!tupleEnd(lexer)) {
                parseExpression(lexer, 0);
            }
            if (lexerExpectToken(lexer->token, T_RIGHTPAREN)) {
                lexerNextToken(lexer);   // Skip ')'
                
                if (lexerTokenIs(lexer, T_LEFTPAREN)) {
                    error(lexer, "", lexer->token);
                }
            } else {
                error(lexer, "Missing closing ')' parenthesis", lexer->token);
            }
        }
            break;

        default:
            break;
    }
}

// (expr) op (expr)
struct Operator parseExpression(struct Lexer* lexer, int priority) {
	// TODO: Add unary operators

    parseSimpleExpression(lexer);

    struct Operator op = getOperator(lexer->token);
    struct Token token = lexer->token;

    while (token.type != T_NOOP && op.left > priority) {
        struct Operator nextOp;
        token = lexer->token;
        lexerNextToken(lexer);
        nextOp = parseExpression(lexer, op.right);
        codeOperator(lexer->vm, token);
        op = nextOp;
    }
    return op;
}

int parse(struct Interpreter* vm, char* input) {
	struct Lexer lexer = {};
    lexer.vm = vm;
	lexer.line = 1;
	lexer.index = input;
    lexerNextToken(&lexer);
    parseStatement(&lexer);
	return 0;
}