#include "AST.h"
#include "tokens.h"

#include <stdio.h>

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

void outputAST(AST* ast) {
    printf("{\n");

    if (ast->var_def_var_name) {
        printf("\t%s,", ast->var_def_var_name);
        printf("\n");
    }

    if (ast->var_name) {
        printf("\t%s,", ast->var_name);
        printf("\n");
    }

    if (ast->func_def_name) {
        printf("\t%s,", ast->func_def_name);
        printf("\n");
    }

    if (ast->func_def_args) {
        /* for (size_t i = 0; i < ast->func_def_args_size; i++) { */
        /*     outputAST(ast->func_def_args[i]); */
        /* } */
        printf("\n");
    }

    if (ast->func_def_args_size) {
        printf("\t%lu,", ast->func_def_args_size);
        printf("\n");
    }

    if (ast->func_call_name) {
        printf("\t%s,", ast->func_call_name);
        printf("\n");
    }

    /* if (ast->func_call_args) { */
    /*     for (size_t i = 0; i < ast->func_call_args_size; i++) { */
    /*         outputAST(ast->func_call_args[i]); */
    /*     } */
    /* printf("\n"); */
    /* } */

    if (ast->func_call_args_size) {
        printf("\t%lu,", ast->func_call_args_size);
        printf("\n");
    }

    if (ast->int_val) {
        printf("\t%d,", ast->int_val);
        printf("\n");
    }

    if (ast->float_val) {
        printf("\t%f,", ast->float_val);
        printf("\n");
    }

    if (ast->double_val) {
        printf("\t%lf,", ast->double_val);
        printf("\n");
    }

    if (ast->char_val) {
        printf("\t\'%c\',", ast->char_val);
        printf("\n");
    }

    if (ast->string_val) {
        printf("\t\"%s\",", ast->string_val);
        printf("\n");
    }

    if (ast->bool_val) {
        printf("\t%d,", ast->bool_val);
        printf("\n");
    }

    /* if (ast->compound_val) { */
    /*     for (size_t i = 0; i < ast->compound_size; i++) { */
    /*         outputAST(ast->compound_val[i]); */
    /*     } */
    /* printf("\n"); */
    /* } */

    if (ast->compound_size) {
        printf("\t%d,", ast->bool_val);
        printf("\n");
    }






    printf("}\n");
}
