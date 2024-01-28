#ifndef PARSER_H_
#define PARSER_H_

#include "lexer.h"
#include "tokens.h"
#include "AST.h"

typedef struct ParserStruct {
	Lexer* lexer;
	Token* current_token;
} Parser;

Parser* initParser(Lexer* lexer);

void parser_eat(Parser* parser, int token_type);

AST* parser_parse(Parser* parser);

AST* parser_parse_statement(Parser* parser);

AST* parser_parse_statements(Parser* parser);

AST* parser_parse_expr(Parser* parser);

AST* parser_parse_factor(Parser* parser);

AST* parser_parse_term(Parser* parser);

AST* parser_parse_func_call(Parser* parser);

AST* parser_parse_var_def(Parser* parser);

AST* parser_parse_var(Parser* parser);

AST* parser_parse_string(Parser* parser);

AST* parser_parse_int(Parser* parser);
#endif
