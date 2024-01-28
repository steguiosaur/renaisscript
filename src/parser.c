#include "parser.h"
#include "AST.h"
#include "tokens.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser* initParser(Lexer* lexer) {
	Parser* parser = (Parser*)malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->current_token = lexerGetNextToken(lexer);


	return parser;

}

void parser_eat(Parser* parser, int token_type) {
	if (parser->current_token->type == token_type) {

        parser->current_token = lexerGetNextToken(parser->lexer);

	} else {

		printf(
            "Unexpected Token: %s, with type %s\n",
            parser->current_token->lexeme,
            tk_map[token_type]
        );

		// Immediately exit the program
		exit(1);
	}
}


AST* parser_parse(Parser* parser) {
    return parser_parse_statements(parser);
}

AST* parser_parse_statement(Parser* parser) { 

    switch (parser->current_token->type) {
        case TK_INT:
            return parser_parse_int(parser);
            break;
        case TK_IDENTIFIER:
            break;
    }
} 

AST* parser_parse_statements(Parser* parser) {

    AST* compound = init_ast(AST_COMPOUND);
    compound->compound_val = (AST**)malloc(sizeof(AST*));

    AST* ast_statement = parser_parse_statement(parser);
    compound->compound_val[0] = ast_statement;

    while (parser->current_token->type == TK_SEMICOLON) {

        parser_eat(parser, TK_SEMICOLON);

        AST* ast_statement = parser_parse_statement(parser);
        compound->compound_size += 1;
        compound->compound_val = realloc(
            compound->compound_val,
            compound->compound_size * sizeof(AST*)
        );
        compound->compound_val[compound->compound_size - 1] = ast_statement;
    }

    return compound;
} 

AST* parser_parse_expr(Parser* parser) { 
} 

AST* parser_parse_factor(Parser* parser) { 
} 

AST* parser_parse_term(Parser* parser) { 
} 

AST* parser_parse_func_call(Parser* parser) { 
} 

AST* parser_parse_var_def(Parser* parser) {
    // Expecting int, char, float, bool, double
    parser_eat(parser, (TK_INT || TK_CHAR || TK_FLOAT || TK_BOOL || TK_DOUBLE));
    char* var_def_var_name = parser->current_token->lexeme;
    parser_eat(parser, TK_IDENTIFIER); // var name
    parser_eat(parser, TK_EQUAL);
    AST* var_def_val = parser_parse_expr(parser);

    AST* var_def = init_ast(AST_VAR_DEF);
    var_def->var_def_var_name = var_def_var_name;
    var_def->var_def_val = var_def_val;
}

AST* parser_parse_var(Parser* parser) { 

    char* token_val = parser->current_token->lexeme;
    parser_eat(parser, TK_IDENTIFIER); // var name or function call name

    if (parser->current_token->type == TK_LPAREN) {
        return parser_parse_func_call(parser);
    }

    AST* ast_var = init_ast(AST_VAR);
    ast_var->var_name = token_val;

    return ast_var;
}

AST* parser_parse_string(Parser* parser) { 
}

AST* parser_parse_int(Parser* parser) {
    if (strcmp(parser->current_token->lexeme, "count")) {
        return parser_parse_var_def(parser);
    } else {
        return parser_parse_var(parser);
    }
}
