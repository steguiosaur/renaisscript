#ifndef PARSER_H_
#define PARSER_H_

#include "lexer.h"
#include "tokens.h"
#include "AST.h"

typedef struct ParserStruct {
	Lexer* lexer;
	Token* current_token;
	Token* prev_token;
} Parser;

Parser* initParser(Lexer* lexer);

void parser_eat(Parser* parser, int token_type);

AST* parser_parse(Parser* parser);

#endif
