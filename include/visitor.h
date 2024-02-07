#ifndef VISITOR_H_
#define VISITOR_H_

#include <stdio.h>

// AST_VAR_DEF, // 0
// AST_VAR, // 1

// AST_FUNC_DEF,  // 2
// AST_FUNC_CALL, // 3

// AST_INT,    // 4
// AST_FLOAT,  // 5
// AST_DOUBLE, // 6
// AST_CHAR,   // 7
// AST_STRING, // 8
// AST_BOOL,   // 9

// AST_EXPR, // 10

// AST_ASSIGN,       // 11
// AST_ASSIGN_UNARY, // 12

// AST_COMPOUND, // 13

#include "AST.h"

AST* visitor_visit(AST *node);

AST* visitor_visit_var_def(AST* node);

AST* visitor_visit_func_call(AST* node);

AST* visitor_visit_var(AST* node);

AST* visitor_visit_string(AST* node);

AST* visitor_visit_compound(AST* node);







#endif