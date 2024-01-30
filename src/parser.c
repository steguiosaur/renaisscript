#include "parser.h"
#include "AST.h"
#include "tokens.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser* initParser(Lexer* lexer) {
	Parser* parser = (Parser*)malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->current_token = lexerGetNextToken(lexer);
    parser->prev_token = parser->current_token;


	return parser;

}

void parser_eat(Parser* parser, int token_type) {
	if (parser->current_token->type == token_type) {

        printf("Eating: %s\n", parser->current_token->lexeme);

        parser->prev_token = parser->current_token;
        parser->current_token = lexerGetNextToken(parser->lexer);

	} else {

		printf(
            "Unexpected Token: \"%s\", with type \"%s\" when needing \"%s\"\n",
            parser->current_token->lexeme,
            tk_map[parser->current_token->type],
            tk_map[token_type]

        );

		// Immediately exit the program
		exit(1);
	}
}

AST* parser_parse(Parser* parser) {
    return root(parser);
}

AST* root(Parser* parser) {

    AST* compound = init_ast(AST_COMPOUND);
    compound->compound_val = (AST**)malloc(sizeof(AST*));

    AST* ast_statement = stmt(parser);
    compound->compound_val[0] = ast_statement;

    while (parser->current_token->type == TK_SEMICOLON) {

        parser_eat(parser, TK_SEMICOLON);

        AST* ast_statement = stmt(parser);
        compound->compound_size += 1;
        compound->compound_val = realloc(
            compound->compound_val,
            compound->compound_size * sizeof(AST*)
        );
        compound->compound_val[compound->compound_size - 1] = ast_statement;
    }

    return compound;
}

AST* stmt(Parser* parser) {
    switch (parser->current_token->type) {

        // Declarations
        case TK_INT:
        case TK_FLOAT:
        case TK_DOUBLE:
        case TK_CHAR:
        case TK_BOOL:
            return declarations(parser);
            break;

        // Statement Groups
        case TK_IN:
        case TK_OUT:
        case TK_IDENTIFIER:
        case TK_IF:
        case TK_FOR:
        case TK_WHILE:
        case TK_RETURN:
        case TK_GOTO:
            return stmt_group(parser);
            break;

        // Literals
        case TK_INTLIT:
        case TK_FLTLIT:
        case TK_CHARACLIT:
        case TK_STRINGLIT:
        case TK_TRUE:
        case TK_FALSE:
            return lit(parser);

    }

}

AST* declarations(Parser* parser) {

    switch (parser->current_token->type) {

        case TK_INT:
            return int_declare(parser);
            break;
        case TK_FLOAT:
            return float_declare(parser);
            break;
        case TK_DOUBLE:
            return double_declare(parser);
            break;
        case TK_CHAR:
            return char_declare(parser);
            break;
        case TK_BOOL:
            return bool_declare(parser);
            break;

    }
}


AST* int_declare(Parser* parser) {

    parser_eat(parser, TK_INT); // count
    char* id = parser->current_token->lexeme;
    parser_eat(parser, TK_IDENTIFIER); // count id

    if (parser->current_token->type == TK_LPAREN) {
        return func_declaration(parser);
    } else {
        return var_dec_stmt(parser);
    }

}
AST* float_declare(Parser* parser) {

    parser_eat(parser, TK_FLOAT); // portion
    char* id = parser->current_token->lexeme;
    parser_eat(parser, TK_IDENTIFIER); // portion id

    if (parser->current_token->type == TK_LPAREN) {
        return func_declaration(parser);
    } else {
        return var_dec_stmt(parser);
    }

}
AST* double_declare(Parser* parser) {

    parser_eat(parser, TK_DOUBLE); // fraction
    char* id = parser->current_token->lexeme;
    parser_eat(parser, TK_IDENTIFIER); // fraction id

    if (parser->current_token->type == TK_LPAREN) {
        return func_declaration(parser);
    } else {
        return var_dec_stmt(parser);
    }

}
AST* char_declare(Parser* parser) {

    parser_eat(parser, TK_CHAR); // glyph
    if (parser->current_token->type == TK_ASTERISK) {
        parser_eat(parser, TK_ASTERISK); // glyph*
        char* id = parser->current_token->lexeme;

    } else {
        char* id = parser->current_token->lexeme;

    }
    parser_eat(parser, TK_IDENTIFIER); // glyph id

    if (parser->current_token->type == TK_LPAREN) {
        return func_declaration(parser);
    } else {
        return var_dec_stmt(parser);
    }

}
AST* bool_declare(Parser* parser) {

    parser_eat(parser, TK_BOOL); // verdict
    parser_eat(parser, TK_IDENTIFIER); // verdict id

    if (parser->current_token->type == TK_LPAREN) {
        return func_declaration(parser);
    } else {
        return var_dec_stmt(parser);
    }

}



AST* func_callback(Parser* parser) {
    AST* ast_func_call = init_ast(AST_FUNC_CALL);

    parser_eat(parser, TK_LPAREN);

    ast_func_call->func_call_name = parser->prev_token->lexeme;


    ast_func_call->func_call_args = (AST**)malloc(sizeof(AST*));

    AST* ast_expr = stmt(parser);
    ast_func_call->func_call_args[0] = ast_expr;

    while (parser->current_token->type == TK_COMMA) {

        parser_eat(parser, TK_COMMA);

        AST* ast_expr = stmt(parser);
        ast_func_call->func_call_args_size += 1;
        ast_func_call->func_call_args = realloc(
            ast_func_call->func_call_args, 
            ast_func_call->func_call_args_size * sizeof(AST*)
        );
        ast_func_call->func_call_args
            [ast_func_call->func_call_args_size - 1] = ast_expr;
    }

    parser_eat(parser, TK_RPAREN);

    return ast_func_call;

}


AST* var_dec_stmt(Parser* parser) {

    parser_eat(parser, TK_ASSIGN);

    switch (parser->current_token->type) {
        case TK_INTLIT:
            return int_lit(parser);
            break;
        case TK_FLTLIT:
            return float_lit(parser);
            break;
        /* case TK_DOUBLE: */
        /*     return double_lit(parser); */
        /*     break; */
        case TK_CHARACLIT:
            return char_lit(parser);
            break;
        case TK_STRINGLIT:
            return string_lit(parser);
            break;
        case TK_TRUE:
        case TK_FALSE:
            return bool_lit(parser);
            break;
    
    }
}

AST* stmt_group(Parser* parser) {
    switch (parser->current_token->type) {
        case TK_IN:
            break;
        case TK_OUT:
            break;
        case TK_IDENTIFIER:
            parser_eat(parser, TK_IDENTIFIER);
            if (parser->current_token->type == TK_LPAREN) {
                return func_callback(parser);
            } else {
            }
            break;
        case TK_IF:
            break;
        case TK_FOR:
            break;
        case TK_WHILE:
            break;
        case TK_RETURN:
            break;
        case TK_GOTO:
            break;
    }
}

AST* lit(Parser* parser) {
    switch (parser->current_token->type) {
        case TK_INTLIT:
            return int_lit(parser);
            break;
        case TK_FLTLIT:
            return float_lit(parser);
            break;
            /* return double_lit(parser); */
            /* break; */
        case TK_CHARACLIT:
            return char_lit(parser);
            break;
        case TK_STRINGLIT:
            return string_lit(parser);
            break;
        case TK_TRUE:
        case TK_FALSE:
            return bool_lit(parser);
            break;

    }
}





AST* int_lit(Parser* parser) {
    AST* ast_int = init_ast(AST_INT);
    ast_int->int_val = atoi(parser->current_token->lexeme);

    parser_eat(parser, TK_INTLIT);

    return ast_int;
}

AST* float_lit(Parser* parser) {
    AST* ast_float = init_ast(AST_FLOAT);
    ast_float->float_val = atof(parser->current_token->lexeme);
    parser_eat(parser, TK_FLTLIT);

    return ast_float;
}

/* AST* double_lit(Parser* parser) { */
/*     AST* ast_double = init_ast(AST_DOUBLE); */
/*     ast_double->double_val = atof(parser->current_token->lexeme); */
/*     parser_eat(parser, TK_DOUBLE); */

/*     return ast_double; */
/* } */

AST* char_lit(Parser* parser) {
    AST* ast_char = init_ast(AST_CHAR);
    ast_char->char_val = parser->current_token->lexeme[0];
    parser_eat(parser, TK_CHARACLIT);

    return ast_char;
}

AST* string_lit(Parser* parser) {
    AST* ast_string = init_ast(AST_STRING);
    ast_string->string_val = parser->current_token->lexeme;
    parser_eat(parser, TK_STRINGLIT);

    return ast_string;

}

AST* bool_lit(Parser* parser) {
    AST* ast_bool = init_ast(AST_BOOL);
    if (strcmp(parser->current_token->lexeme, "yay") == 0) {
        ast_bool->bool_val = 1;
        parser_eat(parser, TK_TRUE);
    } else if (strcmp(parser->current_token->lexeme, "nay") == 0) {
        ast_bool->bool_val = 0;
        parser_eat(parser, TK_FALSE);
    }

    return ast_bool;

}



AST* func_declaration(Parser* parser) {

}






/* AST* parser_parse(Parser* parser) { */
/*     return parser_parse_statements(parser); */
/* } */

/* AST* parser_parse_statement(Parser* parser) { */ 

/*     switch (parser->current_token->type) { */
/*         case TK_INT: */
/*             return parser_parse_int(parser); */
/*             break; */
/*         case TK_IDENTIFIER: */
/*             break; */
/*     } */
/* } */ 

/* AST* parser_parse_statements(Parser* parser) { */

/*     AST* compound = init_ast(AST_COMPOUND); */
/*     compound->compound_val = (AST**)malloc(sizeof(AST*)); */

/*     AST* ast_statement = parser_parse_statement(parser); */
/*     compound->compound_val[0] = ast_statement; */

/*     while (parser->current_token->type == TK_SEMICOLON) { */

/*         parser_eat(parser, TK_SEMICOLON); */

/*         AST* ast_statement = parser_parse_statement(parser); */
/*         compound->compound_size += 1; */
/*         compound->compound_val = realloc( */
/*             compound->compound_val, */
/*             compound->compound_size * sizeof(AST*) */
/*         ); */
/*         compound->compound_val[compound->compound_size - 1] = ast_statement; */
/*     } */

/*     return compound; */
/* } */ 

/* AST* parser_parse_expr(Parser* parser) { */ 
/* } */ 

/* AST* parser_parse_factor(Parser* parser) { */ 
/* } */ 

/* AST* parser_parse_term(Parser* parser) { */ 
/* } */ 

/* AST* parser_parse_func_call(Parser* parser) { */ 
/* } */ 

/* AST* parser_parse_var_def(Parser* parser) { */
/*     // Expecting int, char, float, bool, double */
/*     parser_eat(parser, (TK_INT || TK_CHAR || TK_FLOAT || TK_BOOL || TK_DOUBLE)); */
/*     char* var_def_var_name = parser->current_token->lexeme; */
/*     parser_eat(parser, TK_IDENTIFIER); // var name */
/*     parser_eat(parser, TK_EQUAL); */
/*     AST* var_def_val = parser_parse_expr(parser); */

/*     AST* var_def = init_ast(AST_VAR_DEF); */
/*     var_def->var_def_var_name = var_def_var_name; */
/*     var_def->var_def_val = var_def_val; */
/* } */

/* AST* parser_parse_var(Parser* parser) { */ 

/*     char* token_val = parser->current_token->lexeme; */
/*     parser_eat(parser, TK_IDENTIFIER); // var name or function call name */

/*     if (parser->current_token->type == TK_LPAREN) { */
/*         return parser_parse_func_call(parser); */
/*     } */

/*     AST* ast_var = init_ast(AST_VAR); */
/*     ast_var->var_name = token_val; */

/*     return ast_var; */
/* } */

/* AST* parser_parse_string(Parser* parser) { */ 
/* } */

/* AST* parser_parse_int(Parser* parser) { */
/*     if (strcmp(parser->current_token->lexeme, "count")) { */
/*         return parser_parse_var_def(parser); */
/*     } else { */
/*         return parser_parse_var(parser); */
/*     } */
/* } */
