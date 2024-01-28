#include "AST.h"

AST* init_ast(int type) {
    AST* ast = (AST*)malloc(sizeof(AST));
    ast->type = type;

    // AST_VAR_DEF
    ast->var_def_var_name = (void*)0;
    ast->var_def_val = (void*)0;

    // AST_VAR
    ast->var_name = (void*)0;

    // AST_FUNC_CALL
    ast->func_call_name = (void*)0;
    ast->func_call_args = (void*)0;
    ast->func_call_args_size = 0;

    // AST_STRING
    ast->string_val = (void*)0;

    // AST_COMPOUND
    struct ASTStruct** compound_val = (void*)0;
    size_t compound_size = 0;

    return ast;

}

