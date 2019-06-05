// parser.cpp

#include "parser.h"
#include "lexer.h"
#include "interpreter.h"
#include "code.h"

#include <stdlib.h>
#include <stdio.h>

struct Operator getOperator(struct Token token);
bool tupleEnd(struct Lexer* lexer);

void parseStatement(struct Lexer* lexer);
void parseSimpleExpression(struct Lexer* lexer);
struct Operator parseExpression(struct Lexer* lexer, int priority);

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
        printf("%s\n", "Missing ')' closing parenthesis");
        return true;
    }
    return (type == T_RIGHTPAREN);
}

void parseStatement(struct Lexer* lexer) {
	struct Token token = lexerNextToken(lexer);

	switch (token.type) {
		case T_SEMICOLON: {

		}
			break;

		case T_EOF:
		case T_UNKNOWN: {

		}
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

        }
            break;

        case T_NUMBER: {
            codePushNumber(lexer->vm, token);
            lexerNextToken(lexer);
        }
            break;

        case T_LEFTPAREN: {
            lexerNextToken(lexer);
            if (!tupleEnd(lexer)) {
                parseExpression(lexer, 0);
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

	while (1) {
		parseStatement(&lexer);
		if (lexer.token.type == T_UNKNOWN ||
			lexer.token.type == T_EOF) {
			break;
		}
	}
	return 0;
}