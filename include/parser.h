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

AST* root(Parser* parser);
AST* stmt(Parser* parser);
AST* declarations(Parser* parser);
AST* stmt_group(Parser* parser);
AST* input_stmt(Parser* parser);
AST* output_stmt(Parser* parser);
AST* assignment_stmt(Parser* parser);
AST* conditional_stmt(Parser* parser);
AST* condition(Parser* parser);
AST* iterative_stmt(Parser* parser);
AST* for_stmt(Parser* parser);
AST* while_stmt(Parser* parser);
AST* expr_stmt(Parser* parser);
AST* return_stmt(Parser* parser);
AST* goto_stmt(Parser* parser);
AST* func_callback(Parser* parser);
AST* func_declaration(Parser* parser);
AST* compound_stmt(Parser* parser);
AST* var_dec_stmt(Parser* parser);
AST* var_init_stmt(Parser* parser);
AST* var_stmt(Parser* parser);
AST* expr(Parser* parser);
AST* addsub_expr(Parser* parser);
AST* muldiv_expr(Parser* parser);
AST* expo_expr(Parser* parser);
AST* primary_expr(Parser* parser);
AST* number(Parser* parser);
AST* relational_expr(Parser* parser);
AST* logical_expr(Parser* parser);
AST* int_declare(Parser* parser);
AST* float_declare(Parser* parser);
AST* double_declare(Parser* parser);
AST* char_declare(Parser* parser);
AST* string_declare(Parser* parser);
AST* bool_declare(Parser* parser);
AST* lit(Parser* parser);
AST* int_lit(Parser* parser);
AST* float_lit(Parser* parser);
AST* double_lit(Parser* parser);
AST* char_lit(Parser* parser);
AST* string_lit(Parser* parser);
AST* bool_lit(Parser* parser);
AST* type(Parser* parser);
AST* id_list(Parser* parser);
AST* id(Parser* parser);
AST* digit(Parser* parser);
AST* ALPHASCRIPT(Parser* parser);
AST* UPPERSCRIPT(Parser* parser);
AST* LOWERSCRIPT(Parser* parser);
AST* NUMERALS(Parser* parser);
AST* SYMBOLS(Parser* parser);



#endif
