#ifndef PRODRULES_H_
#define PRODRULES_H_

#include "parser.h"

Parser* root(Parser* parser);
Parser* stmt(Parser* parser);
Parser* stmt_group(Parser* parser);
Parser* declarations(Parser* parser);
Parser* input_stmt(Parser* parser);
Parser* output_stmt(Parser* parser);
Parser* assignment_stmt(Parser* parser);
Parser* conditional_stmt(Parser* parser);
Parser* condition(Parser* parser);
Parser* iterative_stmt(Parser* parser);
Parser* for_stmt(Parser* parser);
Parser* while_stmt(Parser* parser);
Parser* expr_stmt(Parser* parser);
Parser* return_stmt(Parser* parser);
Parser* goto_stmt(Parser* parser);
Parser* func_callback(Parser* parser);
Parser* func_declaration(Parser* parser);
Parser* compound_stmt(Parser* parser);
Parser* var_dec_stmt(Parser* parser);
Parser* var_init_stmt(Parser* parser);
Parser* var_stmt(Parser* parser);
Parser* expr(Parser* parser);
Parser* addsub_expr(Parser* parser);
Parser* muldiv_expr(Parser* parser);
Parser* expo_expr(Parser* parser);
Parser* primary_expr(Parser* parser);
Parser* number(Parser* parser);
Parser* relational_expr(Parser* parser);
Parser* logical_expr(Parser* parser);
Parser* int_declare(Parser* parser);
Parser* float_declare(Parser* parser);
Parser* double_declare(Parser* parser);
Parser* char_declare(Parser* parser);
Parser* string_declare(Parser* parser);
Parser* bool_declare(Parser* parser);
Parser* lit(Parser* parser);
Parser* int_lit(Parser* parser);
Parser* float_lit(Parser* parser);
Parser* double_lit(Parser* parser);
Parser* char_lit(Parser* parser);
Parser* string_lit(Parser* parser);
Parser* bool_lit(Parser* parser);
Parser* type(Parser* parser);
Parser* id_list(Parser* parser);
Parser* id(Parser* parser);
Parser* digit(Parser* parser);
Parser* ALPHASCRIPT(Parser* parser);
Parser* UPPERSCRIPT(Parser* parser);
Parser* LOWERSCRIPT(Parser* parser);
Parser* NUMERALS(Parser* parser);
Parser* SYMBOLS(Parser* parser);

#endif