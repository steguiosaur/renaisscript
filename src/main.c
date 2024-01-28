#include "fileread.h" // char *file_contents
#include "lexer.h"    // lexical analyzer and tokens
#include "parser.h"    // syntax analyzer and parser
#include "optflags.h" // char *inputfile, *outputfile

#include "tokens.h" // 

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
    Token* token = lexerGetNextToken(lexer);

    printf("%s\n", token->lexeme);
    token = lexerGetNextToken(lexer);
    printf("%s\n", token->lexeme);

    

    if (return_error) {
        return 1;
    }

    return 0;
}
