#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "lexer.h"

typedef struct PARSER_STRUCT {

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

AST* parser_parse_function_call(Parser* parser);

AST* parser_parse_variable_definition(Parser* parser);

AST* parser_parse_variable(Parser* parser);

AST* parser_parse_string(Parser* parser);

AST* parser_parse_id(Parser* parser);


#endif
