#ifndef AST_H
#define AST_H
#include <stdlib.h>

typedef struct ASTStruct {
	enum {
		AST_VAR_DEF,
		AST_VAR,
		AST_FUNC_CALL,
		AST_STRING,
        AST_COMPOUND,
	} type;

	// AST_VAR_DEF
	char* var_def_var_name;
	struct ASTStruct* var_def_val;

	// AST_VAR
	char* var_name;

	// AST_FUNC_CALL
	char* func_call_name;
	struct ASTStruct** func_call_args;
	size_t func_call_args_size;

	// AST_STRING
	char* string_val;

    // AST_COMPOUND
    struct ASTStruct** compound_val;
    size_t compound_size;


} AST;

AST* init_ast(int type);



#endif
