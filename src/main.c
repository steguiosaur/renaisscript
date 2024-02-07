#include "fileread.h" // char *file_contents
#include "lexer.h"    // lexical analyzer and tokens
#include "parser.h"    // syntax analyzer and parser
#include "optflags.h" // char *inputfile, *outputfile
#include "visitor.h" // visitor

#include "tokens.h" // tokentypes

#include <stdio.h>


int main(const int argc, char **argv) {
    // optflags.h - parse command line arguments
    if (parseOptionFlags(argc, argv)) {
        return 1;
    }

    // fileread.h - validate extension and get contents in file
    if (inputfile != NULL) {
        if (getRensFileContents(inputfile)) {
            return 1;
        }
    }

    unsigned int return_error = 0;

    if (file_contents == NULL) {

        return 1;
    }

    // process inputfile's characters
    // Lexer* lexer = startLexer(file_contents, return_error);
    // Parser* parser = startParser(lexer, return_error);

    Lexer* lexer = initLexer(file_contents);
    Parser* parser = initParser(lexer);
    AST* head = parser_parse(parser);

    // visitor_visit(head);
    /* visitor_visit(head); */

    // printf("\nAST Type: %d\n", head->type);
    printf("Number of Statements (Compound): %lu\n", head->compound_size);

    // outputAST(head);

	// printf("%s\n", parser->current_token->lexeme);
	// parser->current_token = lexerGetNextToken(parser->lexer);
	// printf("%s\n", parser->current_token->lexeme);
	// parser->current_token = lexerGetPrevToken(parser->lexer);
	// printf("%s\n", parser->current_token->lexeme);


    if (return_error) {
        return 1;
    }

    return 0;
}
