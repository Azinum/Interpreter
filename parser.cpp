// parser.cpp

#include "parser.h"
#include "lexer.h"

#include <stdlib.h>
#include <stdio.h>

struct Operator getOperator(struct Token token);

void parseStatement(struct Lexer* lexer);
void parseSimpleExpression(struct Lexer* lexer);
struct Operator parseExpression(struct Lexer* lexer, int priority);

struct Operator getOperator(struct Token token) {
    struct Operator op = {0};
    if (token.type >= T_PLUS && token.type < T_NOOP) {
        op = priority[token.type];
    }
    return op;
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
            // (expr | unary op)
			parseExpression(lexer, 0);
		}
			break;
	}
}

void parseSimpleExpression(struct Lexer* lexer) {
    struct Token token = lexer->token;
    lexerPrintToken(token);

    switch (token.type) {
        case T_IDENTIFIER: {

        }
            break;

        case T_NUMBER: {
            
        }
            break;

        default:
            break;
    }
    lexerNextToken(lexer);
}

// (expr) op <expr> op
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
        lexerPrintToken(token);
        op = nextOp;
    }
    return op;
}

int parse(char* input) {
	struct Lexer lexer = {};
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