#include "AST.h"

AST* init_ast(int type) {

    AST* ast = calloc(1, sizeof(struct AST_STRUCT));
    ast->type = type;
    
    /* AST_VARIABLE_DEFINITION */
    ast->variable_definition_variable_name = (void*)0;
    ast->variable_definition_value = (void*)0;

    /* AST_VARIABLE */
   ast->variable_name = (void*)0;

    /* AST_FUNCTION_CALL */
    ast->function_call_name = (void*)0;
    ast->function_call_arguments = (void*)0;
    ast->function_call_arguments_size = 0;

    /* AST_STRING */
    ast->string_value = (void*)0;

    /* AST_COMPOUND */
    ast->compound_value = (void*)0;
    ast->compound_size = 0;
}
