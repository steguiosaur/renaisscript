#include "prodrules.h"
#include "AST.h"
#include "tokens.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void checkCurrent(Parser* parser) {
	printf("---|CURRENT: %s| TYPE: %s|---\n", parser->current_token->lexeme, tk_map[parser->current_token->type]);
}

Parser* initParser(Lexer* lexer) {
	Parser* parser = (Parser*)malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->current_token = lexerGetNextToken(lexer);
    parser->prev_token = parser->current_token;


	return parser;

}

void parser_eat(Parser* parser, int token_type) {
	if (parser->current_token->type == token_type) {

		// Check what the parser is taking in
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
	
	if (parser->current_token->type == TK_SEMICOLON) {
		do {
			parser->current_token = lexerGetNextToken(parser->lexer);

			if (parser->current_token->type == TK_EOF) {
				exit(1);
			}
		} while (
			parser->current_token->type == TK_SEMICOLON
		);

	} else if (
		parser->current_token->type == TK_LINECOMMENT ||
		parser->current_token->type == TK_BLCKCOMMENT
	) {
		do {
			parser->current_token = lexerGetNextToken(parser->lexer);

			if (parser->current_token->type == TK_EOF) {
				exit(1);
			}
		} while (
			parser->current_token->type == TK_LINECOMMENT ||
			parser->current_token->type == TK_BLCKCOMMENT
		);

	}
	
	// Starting point
	AST* ast_statement = stmt(parser);
	compound->compound_val[0] = ast_statement;
	compound->compound_size += 1;
	parser_eat(parser, TK_SEMICOLON);

    while (
		parser->current_token->type == TK_LINECOMMENT ||
		parser->current_token->type == TK_BLCKCOMMENT ||
		parser->current_token->type == TK_SEMICOLON ||
		parser->prev_token->type == TK_LINECOMMENT ||
		parser->prev_token->type == TK_BLCKCOMMENT ||
		parser->prev_token->type == TK_RCURLY ||
		parser->current_token->type != TK_EOF
	) {

		if (
			(parser->current_token->type == TK_SEMICOLON &&
			parser->prev_token->type == TK_SEMICOLON) ||
			parser->current_token->type == TK_LINECOMMENT ||
			parser->current_token->type == TK_BLCKCOMMENT
		) {
			
			parser->current_token = lexerGetNextToken(parser->lexer);
			continue;
		}

		if (parser->current_token->type == TK_SEMICOLON) {
			parser_eat(parser, TK_SEMICOLON);
		}

		if (parser->current_token->type == TK_EOF) {
			return compound;
		}

		// Use this to separate lines
		printf("------------\n");

        AST* ast_statement = stmt(parser);

        if (ast_statement) {

            compound->compound_size += 1;
            compound->compound_val = realloc(
                compound->compound_val,
                compound->compound_size * sizeof(AST*)
            );
            compound->compound_val[compound->compound_size - 1] = ast_statement;
        }

		if (parser->current_token->type == TK_EOF) {
			break;
		}

    }
	
	if (
		(parser->prev_token->type != TK_SEMICOLON &&
		parser->current_token->type != TK_SEMICOLON)

	) {
		checkCurrent(parser);
		printf("Expecting a `;`\n");
		exit(1);
	}

	// parser_eat(parser, TK_RCURLY);

	// checkCurrent(parser);

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

        // Functions
        case TK_FUNCTION:
            return func_declaration(parser);
			break;
		
    }
}

AST* declarations(Parser* parser) {

    switch (parser->current_token->type) {

        case TK_INT:
        case TK_FLOAT:
        case TK_DOUBLE:
        case TK_CHAR:
        case TK_BOOL:
            return var_stmt(parser);
            break;
		default:
			printf("Unknown Datatype: `%s`\n", parser->current_token->lexeme);
    }
}

AST* stmt_group(Parser* parser) {
    switch (parser->current_token->type) {
        case TK_IN:
            return input_stmt(parser);
            break;

        case TK_OUT:
            return output_stmt(parser);
            break;

        case TK_IDENTIFIER:
            parser_eat(parser, TK_IDENTIFIER);
            if (parser->current_token->type == TK_LPAREN) {
                return func_callback(parser);
            } else {
                return assignment_stmt(parser);
            }

        case TK_IF:
			conditional_stmt(parser);
			break;

        case TK_FOR:
        case TK_WHILE:
			return iterative_stmt(parser);
			
        case TK_RETURN:
        case TK_GOTO:
            break;
    }

}

AST* input_stmt(Parser* parser) {
    parser_eat(parser, TK_IN);

    AST* ast_func_call = init_ast(AST_FUNC_CALL);
    ast_func_call->func_call_name = parser->prev_token->lexeme;

    parser_eat(parser, TK_LPAREN);

    ast_func_call->func_call_args = (AST**)malloc(sizeof(AST*));

    AST* ast_expr = expr(parser);
    ast_func_call->func_call_args[0] = ast_expr;
    ast_func_call->func_call_args_size += 1;

    parser_eat(parser, TK_RPAREN);
}

AST* output_stmt(Parser* parser) {
    parser_eat(parser, TK_OUT);

    AST* ast_func_call = init_ast(AST_FUNC_CALL);
    ast_func_call->func_call_name = parser->prev_token->lexeme;

    parser_eat(parser, TK_LPAREN);

    ast_func_call->func_call_args = (AST**)malloc(sizeof(AST*));

    AST* ast_expr = expr(parser);
    ast_func_call->func_call_args[0] = ast_expr;
    ast_func_call->func_call_args_size += 1;

    while (parser->current_token->type == TK_COMMA) {

        parser_eat(parser, TK_COMMA);

        AST* ast_expr = expr(parser);
        ast_func_call->func_call_args_size += 1;
        ast_func_call->func_call_args = realloc(
            ast_func_call->func_call_args, 
            ast_func_call->func_call_args_size * sizeof(AST*)
        );
        ast_func_call->func_call_args
            [ast_func_call->func_call_args_size - 1] = ast_expr;
    }
    parser_eat(parser, TK_RPAREN);


}

AST* assignment_stmt(Parser* parser) {
	AST* ast_assignment_stmt = init_ast(AST_ASSIGN);

	switch (parser->current_token->type) {

		case TK_ASSIGN:
			parser_eat(parser, TK_ASSIGN);
			ast_assignment_stmt->exprs = (AST**)malloc(sizeof(AST*));
			AST* ast_assignment = expr(parser);
			ast_assignment_stmt->exprs = ast_assignment;
			break;

		case TK_INCREMENT:
			parser_eat(parser, TK_INCREMENT);
			AST* ast_increment = init_ast(AST_ASSIGN_UNARY);
			ast_increment->assign_op = TK_PLUS;
			ast_increment->int_val = 1;
			return ast_increment;
			break;

		case TK_ASSIGNINC:
			parser_eat(parser, TK_ASSIGNINC);
			ast_assignment_stmt->exprs = (AST**)malloc(sizeof(AST*));
			AST* ast_assign_inc = expr(parser);
			ast_assignment_stmt->exprs = ast_assign_inc;
			break;

		case TK_DECREMENT:
			parser_eat(parser, TK_DECREMENT);
			AST* ast_decrement = init_ast(AST_ASSIGN_UNARY);
			ast_decrement->assign_op = TK_MINUS;
			ast_decrement->int_val = 1;

			return ast_decrement;

		case TK_ASSIGNDEC:
			parser_eat(parser, TK_ASSIGNDEC);
			ast_assignment_stmt->exprs = (AST**)malloc(sizeof(AST*));
			AST* ast_assign_dec = expr(parser);
			ast_assignment_stmt->exprs = ast_assign_dec;
			break;

		case TK_ASSIGNMUL:
			parser_eat(parser, TK_ASSIGNMUL);
			ast_assignment_stmt->exprs = (AST**)malloc(sizeof(AST*));
			AST* ast_assign_mul = expr(parser);
			ast_assignment_stmt->exprs = ast_assign_mul;
			break;

		case TK_ASSIGNDIV:
			parser_eat(parser, TK_ASSIGNDIV);
			ast_assignment_stmt->exprs = (AST**)malloc(sizeof(AST*));
			AST* ast_assign_div = expr(parser);
			ast_assignment_stmt->exprs = ast_assign_div;
			break;

	}

	// ast_assignment_stmt->exprs[0] = ast_expr;
	// ast_assignment_stmt->exprs_size += 1;

	// while (parser->current_token->type == TK_COMMA) {

	// 	parser_eat(parser, TK_COMMA);

	// 	AST* ast_expr = expr(parser);
	// 	ast_assignment_stmt->exprs_size += 1;
	// 	ast_assignment_stmt->exprs = realloc(
	// 		ast_assignment_stmt->exprs,
	// 		ast_assignment_stmt->exprs_size * sizeof(AST*)
	// 	);
	// 	ast_assignment_stmt->exprs
	// 		[ast_assignment_stmt->exprs_size - 1] = ast_expr;
	// }

	return ast_assignment_stmt;

}

AST* conditional_stmt(Parser* parser) {
	parser_eat(parser, TK_IF);
	parser_eat(parser, TK_LPAREN);

	AST* ast_condition = condition(parser);
	parser_eat(parser, TK_RPAREN);

	compound_stmt(parser);


}
AST* condition(Parser* parser) {
	logical_expr(parser);
}

AST* iterative_stmt(Parser* parser) {
	
	switch (parser->current_token->type) {
	case TK_FOR:
		return for_stmt(parser);
		break;
	case TK_WHILE:
		return while_stmt(parser);
		break;
	}
}

AST* for_stmt(Parser* parser) {
	parser_eat(parser, TK_FOR);
	parser_eat(parser, TK_LPAREN);
	
	var_stmt(parser);
	parser_eat(parser, TK_SEMICOLON);
	relational_expr(parser);
	parser_eat(parser, TK_SEMICOLON);
	stmt(parser);

	parser_eat(parser, TK_RPAREN);

	compound_stmt(parser);

	checkCurrent(parser);

}
AST* while_stmt(Parser* parser) {
	parser_eat(parser, TK_WHILE);
	parser_eat(parser, TK_LPAREN);

	logical_expr(parser);

	parser_eat(parser, TK_RPAREN);

	compound_stmt(parser);

}
AST* expr_stmt(Parser* parser) {}
AST* return_stmt(Parser* parser) {}
AST* goto_stmt(Parser* parser) {}

AST* func_callback(Parser* parser) {
    AST* ast_func_call = init_ast(AST_FUNC_CALL);

    ast_func_call->func_call_name = parser->prev_token->lexeme;
    parser_eat(parser, TK_LPAREN);

    ast_func_call->func_call_args = (AST**)malloc(sizeof(AST*));

    AST* ast_expr = expr(parser);
    ast_func_call->func_call_args[0] = ast_expr;
    ast_func_call->func_call_args_size += 1;

    while (parser->current_token->type == TK_COMMA) {

        parser_eat(parser, TK_COMMA);

        AST* ast_expr = expr(parser);
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

AST* func_declaration(Parser* parser) {
    parser_eat(parser, TK_FUNCTION);
    switch (parser->current_token->type) {

        case TK_INT:
            return int_func_declare(parser);
            break;
        case TK_FLOAT:
            return float_func_declare(parser);
            break;
        case TK_DOUBLE:
            return double_func_declare(parser);
            break;
        case TK_CHAR:
            parser_eat(parser, TK_CHAR);

            if (parser->current_token->type == TK_ASTERISK) {
                return string_func_declare(parser);
            } else {
                return char_func_declare(parser);
            }

            break;
        case TK_BOOL:
            return bool_func_declare(parser);
            break;

    }
}
AST* compound_stmt(Parser* parser) {
	AST* compound = init_ast(AST_COMPOUND);
	compound->compound_val = (AST**)malloc(sizeof(AST*));

	
	parser_eat(parser, TK_LCURLY);
	printf("=====COMPOUND=====\n");

	AST* ast_statement = stmt(parser);
	compound->compound_val[0] = ast_statement;
	compound->compound_size += 1;


	while (
		parser->current_token->type == TK_SEMICOLON) {

		if (parser->current_token->type == TK_SEMICOLON) {
			
			parser_eat(parser, TK_SEMICOLON);
		}


		AST* ast_statement = stmt(parser);

		if (ast_statement) {

			compound->compound_size += 1;
			compound->compound_val = realloc(
				compound->compound_val,
				compound->compound_size * sizeof(AST*)
			);
			compound->compound_val[compound->compound_size - 1] = ast_statement;
		}
	}
	printf("=====COMPOUND=====\n");

	parser_eat(parser, TK_RCURLY);


	return compound;

}

AST* var_stmt(Parser* parser) {
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

AST* expr(Parser* parser) {

    switch (parser->current_token->type) {
        case TK_INTLIT:
        case TK_FLTLIT:
        case TK_CHARACLIT:
        case TK_STRINGLIT:
        case TK_TRUE:
        case TK_FALSE:
        case TK_IDENTIFIER:
            return term(parser);
            break;
            // return id(parser);
    }
}

AST* term(Parser* parser) {


    switch (parser->current_token->type) {
        case TK_INTLIT:
            parser_eat(parser, TK_INTLIT);
            if (parser->current_token->type == TK_PLUS) {
                parser_eat(parser, TK_PLUS);
				return term(parser);

            } else if (parser->current_token->type == TK_MINUS) {
                parser_eat(parser, TK_MINUS);
				return term(parser);

            }
            return factor(parser);

            break;
        case TK_FLTLIT:
            parser_eat(parser, TK_FLTLIT);
            if (parser->current_token->type == TK_PLUS) {
                parser_eat(parser, TK_PLUS);

            } else if (parser->current_token->type == TK_MINUS) {
                parser_eat(parser, TK_MINUS);

            }

            return factor(parser);
            break;
        case TK_CHARACLIT:
            parser_eat(parser, TK_CHARACLIT);
            AST* ast_characlit = init_ast(AST_CHAR);
            ast_characlit->char_val = parser->prev_token->lexeme[0];
            return ast_characlit;
            break;
        case TK_STRINGLIT:
            parser_eat(parser, TK_STRINGLIT);
            AST* ast_stringlit = init_ast(AST_STRING);
            ast_stringlit->string_val = parser->prev_token->lexeme;
            return ast_stringlit;
            break;
        case TK_TRUE:
            parser_eat(parser, TK_TRUE);
            AST* ast_bool_true = init_ast(AST_BOOL);
            ast_bool_true->bool_val = 1;
            break;
        case TK_FALSE:
            parser_eat(parser, TK_FALSE);
            AST* ast_bool_false = init_ast(AST_BOOL);
            ast_bool_false->bool_val = 0;
            break;
		case TK_IDENTIFIER:
			parser_eat(parser, TK_IDENTIFIER);
			if (parser->current_token->type == TK_PLUS) {
				parser_eat(parser, TK_PLUS);
				return term(parser);

			}
			else if (parser->current_token->type == TK_MINUS) {
				parser_eat(parser, TK_MINUS);
				return term(parser);

			}
			return factor(parser);

    }

}

AST* factor(Parser* parser) {

    switch (parser->prev_token->type) {
        case TK_INTLIT:
            if (parser->current_token->type == TK_ASTERISK) {
                parser_eat(parser, TK_ASTERISK);

            } else if (parser->current_token->type == TK_SLASH) {
                parser_eat(parser, TK_SLASH);
            }
            return expo(parser);
            break;
        case TK_FLTLIT:
            if (parser->current_token->type == TK_ASTERISK) {
                parser_eat(parser, TK_ASTERISK);

            } else if (parser->current_token->type == TK_SLASH) {
                parser_eat(parser, TK_SLASH);
            }
            return expo(parser);
            break;

		case TK_IDENTIFIER:
			if (parser->current_token->type == TK_ASTERISK) {
				parser_eat(parser, TK_ASTERISK);

			}
			else if (parser->current_token->type == TK_SLASH) {
				parser_eat(parser, TK_SLASH);
			}
			return expo(parser);
			break;

    }
}

AST* expo(Parser* parser) {
	
    switch (parser->current_token->type) {

        case TK_IDENTIFIER:
			if (parser->current_token->type == TK_EXPONENT) {
				parser_eat(parser, TK_EXPONENT);
				number(parser);
			} else {
				primary_expr(parser);
			}
			break;
        case TK_INTLIT:
			if (parser->current_token->type == TK_EXPONENT) {
				parser_eat(parser, TK_EXPONENT);
				number(parser);
			} else {
				primary_expr(parser);
			}
			break;

        case TK_FLTLIT:
			if (parser->current_token->type == TK_EXPONENT) {
				parser_eat(parser, TK_EXPONENT);
				number(parser);
			} else {
				primary_expr(parser);
			}
			break;

    }

}

AST* primary_expr(Parser* parser) {

    switch (parser->current_token->type) {
        case TK_IDENTIFIER:
			id(parser);
            break;
        case TK_INTLIT:
        case TK_FLTLIT:
			number(parser);
        case TK_CHARACLIT:
        case TK_STRINGLIT:
        case TK_TRUE:
        case TK_FALSE:
            lit(parser);
            break;
    }

}

AST* number(Parser* parser) {
    switch (parser->current_token->type) {
    case TK_INTLIT:
        return int_lit(parser);
        break;
    case TK_FLTLIT:
        return float_lit(parser);
        break;
    }
}

AST* relational_expr(Parser* parser) {
	// <number> | <id> | <bool_lit>
	switch (parser->current_token->type) {
		case TK_INTLIT:
		case TK_FLTLIT:
			number(parser);
			break;
		case TK_IDENTIFIER:
			id(parser);
			break;
		case TK_TRUE:
		case TK_FALSE:
			bool_lit(parser);
			break;
	}

	// <, >, <=, >=, ==, !=
	switch (parser->current_token->type) {
		case TK_LT:
			parser_eat(parser, TK_LT);
			break;
		case TK_GT:
			parser_eat(parser, TK_GT);
			break;
		case TK_LEQUAL:
			parser_eat(parser, TK_LEQUAL);
			break;
		case TK_GEQUAL:
			parser_eat(parser, TK_GEQUAL);
			break;
		case TK_EQUAL:
			parser_eat(parser, TK_EQUAL);
			break;
		case TK_NOTEQUAL:
			parser_eat(parser, TK_NOTEQUAL);
			break;
	}

	// <number> | <id> | <bool_lit>
	switch (parser->current_token->type) {
		case TK_INTLIT:
		case TK_FLTLIT:
			number(parser);
			break;
		case TK_IDENTIFIER:
			id(parser);
			break;
		case TK_TRUE:
		case TK_FALSE:
			bool_lit(parser);
			break;
	}

}
AST* logical_expr(Parser* parser) {

	// !
	if (parser->current_token->type == TK_BANG) {
		parser_eat(parser, TK_BANG);

		// <relational_expr> | <bool_lit>
		switch (parser->current_token->type)
		{
		case TK_INTLIT:
		case TK_FLTLIT:
		case TK_IDENTIFIER:
			relational_expr(parser);
			break;
		case TK_TRUE:
		case TK_FALSE:
			bool_lit(parser);
			break;
		}

		if (parser->current_token->type == TK_AND || parser->current_token->type == TK_OR) {
			// &&, ||
			switch (parser->current_token->type)
			{
			case TK_AND:
				parser_eat(parser, TK_AND);
				break;
			case TK_OR:
				parser_eat(parser, TK_OR);
				break;
			}

			// <relational_expr> | <bool_lit>
			switch (parser->current_token->type)
			{
			case TK_INTLIT:
			case TK_FLTLIT:
			case TK_IDENTIFIER:
				relational_expr(parser);
				break;
			case TK_TRUE:
			case TK_FALSE:
				bool_lit(parser);
				break;
			}
		}
	} else {
		// <relational_expr> | <bool_lit>
		switch (parser->current_token->type)
		{
		case TK_INTLIT:
		case TK_FLTLIT:
		case TK_IDENTIFIER:
			relational_expr(parser);
			break;
		case TK_TRUE:
		case TK_FALSE:
			bool_lit(parser);
			break;
		}

		// &&, ||
		switch (parser->current_token->type)
		{
		case TK_AND:
			parser_eat(parser, TK_AND);
			break;
		case TK_OR:
			parser_eat(parser, TK_OR);
			break;
		}

		// <relational_expr> | <bool_lit>
		switch (parser->current_token->type)
		{
		case TK_INTLIT:
		case TK_FLTLIT:
		case TK_IDENTIFIER:
			relational_expr(parser);
			break;
		case TK_TRUE:
		case TK_FALSE:
			bool_lit(parser);
			break;
		}

	}
}

AST* int_declare(Parser* parser) {
    parser_eat(parser, TK_INT); // count

    char* var_id = parser->current_token->lexeme;

    parser_eat(parser, TK_IDENTIFIER); // count id

    if (parser->current_token->type == TK_ASSIGN) {
        return int_init_declare(parser);
    } else {
        
        AST* int_var = init_ast(AST_VAR_DEF);
        int_var->var_def_type = TK_INT;
        int_var->var_def_var_name = var_id;
        int_var->var_def_val = (AST*)0; // No value

        return int_var;
    }
}

AST* float_declare(Parser* parser) {

    parser_eat(parser, TK_FLOAT); // portion

    char* var_id = parser->current_token->lexeme;

    parser_eat(parser, TK_IDENTIFIER); // portion id

    if (parser->current_token->type == TK_ASSIGN) {
        return float_init_declare(parser);
    } else {

        
        AST* float_var = init_ast(AST_VAR_DEF);
        float_var->var_def_type = TK_FLOAT;
        float_var->var_def_var_name = var_id;
        float_var->var_def_val = (AST*)0; // No value

        return float_var;
    }
}

AST* double_declare(Parser* parser) {

    parser_eat(parser, TK_DOUBLE); // fraction

    char* var_id = parser->current_token->lexeme;

    parser_eat(parser, TK_IDENTIFIER); // fraction id

    if (parser->current_token->type == TK_ASSIGN) {
        return double_init_declare(parser);
    } else {
        
        AST* double_var = init_ast(AST_VAR_DEF);
        double_var->var_def_type = TK_DOUBLE;
        double_var->var_def_var_name = var_id;
        double_var->var_def_val = (AST*)0; // No value

        return double_var;
    }
}
AST* char_declare(Parser* parser) {

    parser_eat(parser, TK_CHAR); // glyph

    if (parser->current_token->type == TK_ASTERISK) {
        parser_eat(parser, TK_ASTERISK); // glyph *

        char* var_id = parser->current_token->lexeme;
        parser_eat(parser, TK_IDENTIFIER); // glyph* id

        if (parser->current_token->type == TK_ASSIGN) {
            return string_init_declare(parser);
        } else {
            
            AST* string_var = init_ast(AST_VAR_DEF);
            string_var->var_def_type = TK_CHAR;
            string_var->var_def_var_name = var_id;
            string_var->var_def_val = (AST*)0; // No value

            return string_var;
        }
    } else {

        char* var_id = parser->current_token->lexeme;

        parser_eat(parser, TK_IDENTIFIER); // glyph id

        if (parser->current_token->type == TK_ASSIGN) {
            return char_init_declare(parser);
        } else {
            
            AST* char_var = init_ast(AST_VAR_DEF);
            char_var->var_def_type = TK_CHAR;
            char_var->var_def_var_name = var_id;
            char_var->var_def_val = (AST*)0; // No value

            return char_var;
        }
    }

}

AST* bool_declare(Parser* parser) {

    parser_eat(parser, TK_BOOL); // verdict

    char* var_id = parser->current_token->lexeme;

    parser_eat(parser, TK_IDENTIFIER); // verdict id

    if (parser->current_token->type == TK_ASSIGN) {
        return bool_init_declare(parser);
    } else {
        
        AST* bool_var = init_ast(AST_VAR_DEF);
        bool_var->var_def_type = TK_DOUBLE;
        bool_var->var_def_var_name = var_id;
        bool_var->var_def_val = (AST*)0; // No value

        return bool_var;
    }
}

AST* int_init_declare(Parser* parser) {

    char* var_id = parser->prev_token->lexeme;

    parser_eat(parser, TK_ASSIGN);

    AST* int_var_def_value = expr(parser);

    AST* int_var = init_ast(AST_VAR_DEF);
    int_var->var_def_type = TK_INT;
    int_var->var_def_var_name = var_id;
    int_var->var_def_val = int_var_def_value;
    return int_var;
}

AST* float_init_declare(Parser* parser) {

    char* var_id = parser->prev_token->lexeme;

    parser_eat(parser, TK_ASSIGN);

    AST* float_var_dev_value = expr(parser);

    AST* float_var = init_ast(AST_VAR_DEF);
    float_var->var_def_type = TK_FLOAT;
    float_var->var_def_var_name = var_id;
    float_var->var_def_val = float_var_dev_value;
    return float_var;
}

AST* double_init_declare(Parser* parser) {

    char* var_id = parser->prev_token->lexeme;

    parser_eat(parser, TK_ASSIGN);

    AST* double_var_dev_value = expr(parser);

    AST* double_var = init_ast(AST_VAR_DEF);
    double_var->var_def_type = TK_DOUBLE;
    double_var->var_def_var_name = var_id;
    double_var->var_def_val = double_var_dev_value;
    return double_var;
}

AST* char_init_declare(Parser* parser) {

    char* var_id = parser->prev_token->lexeme;

    parser_eat(parser, TK_ASSIGN);

    AST* char_var_dev_value = expr(parser);

    AST* char_var = init_ast(AST_VAR_DEF);
    char_var->var_def_type = TK_DOUBLE;
    char_var->var_def_var_name = var_id;
    char_var->var_def_val = char_var_dev_value;
    return char_var;


}
AST* string_init_declare(Parser* parser) {

    char* var_id = parser->prev_token->lexeme;

    parser_eat(parser, TK_ASSIGN);

    AST* string_var_dev_value = expr(parser);

    AST* string_var = init_ast(AST_VAR_DEF);
    string_var->var_def_type = TK_STRINGLIT;
    string_var->var_def_var_name = var_id;
    string_var->var_def_val = string_var_dev_value;
    return string_var;

}
AST* bool_init_declare(Parser* parser) {

    char* var_id = parser->prev_token->lexeme;

    parser_eat(parser, TK_ASSIGN);

    AST* bool_var_dev_value = expr(parser);

    AST* bool_var = init_ast(AST_VAR_DEF);
    bool_var->var_def_type = TK_STRINGLIT;
    bool_var->var_def_var_name = var_id;
    bool_var->var_def_val = bool_var_dev_value;
    return bool_var;

}
AST* int_func_declare(Parser* parser) {
    parser_eat(parser, TK_INT); // count
    
    char* var_id = parser->current_token->lexeme;

    parser_eat(parser, TK_IDENTIFIER); // count id
    parser_eat(parser, TK_LPAREN); // count id(
    parser_eat(parser, TK_RPAREN); // count id()
    parser_eat(parser, TK_LCURLY); // count id() {
    root(parser);
    parser_eat(parser, TK_RCURLY); // count id() {}

}
AST* float_func_declare(Parser* parser) {}
AST* double_func_declare(Parser* parser) {}
AST* char_func_declare(Parser* parser) {}
AST* string_func_declare(Parser* parser) {}
AST* bool_func_declare(Parser* parser) {}

AST* lit(Parser* parser) {
    switch (parser->current_token->type) {
        case TK_INTLIT:
            int_lit(parser);
            break;
        case TK_FLTLIT:
            float_lit(parser);
            break;
        case TK_CHARACLIT:
            char_lit(parser);
            break;
        case TK_STRINGLIT:
            string_lit(parser);
            break;
        case TK_TRUE:
        case TK_FALSE:
            bool_lit(parser);
            break;
    }

}

AST* int_lit(Parser* parser) {

    AST* ast_int_val = init_ast(AST_INT);

    ast_int_val->int_val = atoi(parser->current_token->lexeme);
    parser_eat(parser, TK_INTLIT);

    return ast_int_val;
}

AST* float_lit(Parser* parser) {

    AST* ast_float_val = init_ast(AST_FLOAT);

    ast_float_val->float_val = atof(parser->current_token->lexeme);
    parser_eat(parser, TK_FLTLIT);

    return ast_float_val;

}

AST* double_lit(Parser* parser) {

    AST* ast_double_val = init_ast(AST_DOUBLE);

    ast_double_val->float_val = atof(parser->current_token->lexeme);
    parser_eat(parser, TK_FLTLIT);

    return ast_double_val;

}

AST* char_lit(Parser* parser) {

    AST* ast_char_val = init_ast(AST_CHAR);

    ast_char_val->char_val = parser->current_token->lexeme[0];
    parser_eat(parser, TK_CHARACLIT);

    return ast_char_val;

}

AST* string_lit(Parser* parser) {

    AST* ast_string_val = init_ast(AST_STRING);

    ast_string_val->string_val = parser->current_token->lexeme;
    parser_eat(parser, TK_STRINGLIT);

    return ast_string_val;

}

AST* bool_lit(Parser* parser) {

    AST* ast_bool_val = init_ast(AST_BOOL);

    switch (parser->current_token->type) {
        case TK_TRUE:
            ast_bool_val->bool_val = 1;
            parser_eat(parser, TK_TRUE);
            break;
        case TK_FALSE:
            ast_bool_val->bool_val = 0;
            parser_eat(parser, TK_FALSE);
            break;
        default:
            printf("Unknown token\n");
            break;
    }

    return ast_bool_val;
}


AST* id(Parser* parser) {
    AST* var_id = (AST*)malloc(sizeof(AST));
    var_id->var_name = parser->current_token->lexeme;

    parser_eat(parser, TK_IDENTIFIER);

    return var_id;
}


// Lexer did this already
AST* type(Parser* parser) {}
AST* digit(Parser* parser) {}
AST* ALPHASCRIPT(Parser* parser) {}
AST* UPPERSCRIPT(Parser* parser) {}
AST* LOWERSCRIPT(Parser* parser) {}
AST* NUMERALS(Parser* parser) {}
AST* SYMBOLS(Parser* parser) {}