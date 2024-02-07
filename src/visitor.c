#include "visitor.h"
#include <stdio.h>

AST* visitor_visit(AST* node) {

	switch (node->type) {
	case AST_VAR_DEF:
		return visitor_visit_var_def(node);
		break;
	case AST_VAR:
		return visitor_visit_var(node);
		break;
	case AST_FUNC_CALL:
		return visitor_visit_func_call(node);
		break;
	case AST_STRING:
		return visitor_visit_string(node);
		break;
	case AST_COMPOUND:
		return visitor_visit_compound(node);
		break;
	}

	printf("Uncaught statement of type `%d`\n", node->type);

	exit(1);
	return init_ast(AST_NOOP);
}

AST* visitor_visit_var_def(AST* node) {

    printf("HELLO1\n");
}

AST* visitor_visit_func_call(AST* node) {

    printf("HELLO2\n");
}

AST* visitor_visit_var(AST* node) {
    printf("HELLO3\n");

}

AST* visitor_visit_string(AST* node) {
    printf("HELLO4\n");

}

AST* visitor_visit_compound(AST* node) {
	for (int i = 0; i < node->compound_size; i++) {
		printf("%d\n", i);
		visitor_visit(node->compound_val[i]);
	}

	return init_ast(AST_NOOP);
}
