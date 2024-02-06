#include "parser.h"
#include "AST.h"
#include "lexer.h"
#include "tokens.h"
#include <stdio.h>
#include <string.h>

Parser* initParser(Lexer* lexer) {
    Parser* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexerGetNextToken(lexer);

    return parser;
}

void parser_eat(Parser* parser, int token_type) {
    if (parser->current_token->type == token_type) {
        printf("Eating `%s`\n", parser->current_token->lexeme);
        parser->current_token = lexerGetNextToken(parser->lexer);
    } else {
        printf(
            "Unexpected token `%s`, with type %d\n",
            parser->current_token->lexeme,
            parser->current_token->type
        );
    }

}

AST* parser_parse(Parser* parser) {
    return parser_parse_statements(parser);
}

AST* parser_parse_statement(Parser* parser) {
    switch (parser->current_token->type) {
        case TK_CHAR:
            return parser_parse_id(parser);


        default:
            break;
    }
}

AST* parser_parse_statements(Parser* parser) {
    AST* compound = init_ast(AST_COMPOUND);
    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));

    AST* ast_statement = parser_parse_statement(parser);
    compound->compound_value[0] = ast_statement;

    while (parser->current_token->type == TK_SEMICOLON) {

        parser_eat(parser, TK_SEMICOLON);
        AST* ast_statement = parser_parse_statement(parser);
        compound->compound_size += 1;
        compound->compound_value = realloc(
            compound->compound_value,
            compound->compound_size * sizeof(struct AST_STRUCT*)
        );
        compound->compound_value[compound->compound_size-1] = ast_statement;

    }

    return compound;

}

AST* parser_parse_expr(Parser* parser) {}

AST* parser_parse_factor(Parser* parser) {}

AST* parser_parse_term(Parser* parser) {}

AST* parser_parse_function_call(Parser* parser) {}

AST* parser_parse_variable_definition(Parser* parser) {

    parser_eat(parser, TK_CHAR); // glyph
    parser_eat(parser, TK_ASTERISK); // glyph*
    char* variable_definition_variable_name = parser->current_token->lexeme;
    parser_eat(parser, TK_IDENTIFIER); // glyph* id
    parser_eat(parser, TK_ASSIGN); //glyph* id = 

    AST* variable_definition_value = parser_parse_expr(parser);

    AST* variable_definition = init_ast(AST_VARIABLE_DEFINITION);
    variable_definition->variable_definition_variable_name = variable_definition_variable_name;
    variable_definition->variable_definition_value = variable_definition_value;

    return variable_definition;
}

AST* parser_parse_variable(Parser* parser) {

    char* token_value = parser->current_token->lexeme;
    parser_eat(parser, TK_IDENTIFIER); // var name or function call name
    
    if (parser->current_token->type == TK_LPAREN) {
        return parser_parse_function_call(parser);
    }
    
    AST* ast_variable = init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_value;

    return ast_variable;


}

AST* parser_parse_string(Parser* parser) {}

AST* parser_parse_id(Parser* parser) {
    if (parser->current_token->type == TK_CHAR) {
        return parser_parse_variable_definition(parser);
    } else {
        return parser_parse_variable(parser);
    }
}











