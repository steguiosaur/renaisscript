#include "fileread.h" // char *array_textdata
#include "lexer.h"
#include "optflags.h" // char *inputfile, *outputfile
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

    // process inputfile's characters
    if (array_textdata != NULL) {
        // read inputfile
        printf("Input File: %s\n", inputfile);
        printf("Output File: %s\n", outputfile);
        printf("%s", array_textdata);

        Lexer *lexer = initLexer(array_textdata);

        printf("Symbol Table:\n");

        Token *tok = lexerGetNextToken(lexer);
        while (tok->type != TK_EOF) {

            printf("Type: %2d \tLexeme: %s\n", tok->type, tok->lexeme);

            tokenCleanup(&tok);
            tok = lexerGetNextToken(lexer);
        }

        lexerCleanUp(&lexer);
        free(array_textdata);
    }

    return 0;
}
