#ifndef AST_H
#define AST_H
#include <stdlib.h>

typedef struct ASTStruct {
	enum {
		AST_VAR_DEF,    // 0
		AST_VAR,        // 1
        AST_FUNC_DEF,   // 2
		AST_FUNC_CALL,  // 3

        AST_INT,        // 4
        AST_FLOAT,      // 5
        AST_DOUBLE,     // 6
        AST_CHAR,       // 7
        AST_STRING,     // 8
        AST_BOOL,       // 9

        AST_COMPOUND,   // 10
	} type;

	// AST_VAR_DEF
	char* var_def_var_name;
	struct ASTStruct* var_def_val;

	// AST_VAR
	char* var_name;

    // AST_FUNC_DEF
    char* func_def_name;
	struct ASTStruct** func_def_args;
	size_t func_def_args_size;

	// AST_FUNC_CALL
	char* func_call_name;
	struct ASTStruct** func_call_args;
	size_t func_call_args_size;

	// AST_INT
	int int_val;

	// AST_FLOAT
	float float_val;

	// AST_DOUBLE
	double double_val;

	// AST_CHAR
	char char_val;

	// AST_STRING
	char* string_val;

	// AST_BOOL
	int bool_val; // 0 - False | 1 - True

    // AST_COMPOUND
    struct ASTStruct** compound_val;
    size_t compound_size;



} AST;

AST* init_ast(int type);

void outputAST(AST* ast);


#endif
