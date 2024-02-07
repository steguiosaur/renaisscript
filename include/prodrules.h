#ifndef PRODRULES_H_
#define PRODRULES_H_

#include "parser.h"

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
AST* term(Parser* parser);
AST* factor(Parser* parser);
AST* expo(Parser* parser);
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
AST* int_init_declare(Parser* parser);
AST* float_init_declare(Parser* parser);
AST* double_init_declare(Parser* parser);
AST* char_init_declare(Parser* parser);
AST* string_init_declare(Parser* parser);
AST* bool_init_declare(Parser* parser);
AST* int_func_declare(Parser* parser);
AST* float_func_declare(Parser* parser);
AST* double_func_declare(Parser* parser);
AST* char_func_declare(Parser* parser);
AST* string_func_declare(Parser* parser);
AST* bool_func_declare(Parser* parser);
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