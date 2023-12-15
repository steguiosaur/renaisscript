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

        Lexer *lexer = createLexer(array_textdata);

        printf("Tokenizing input:\n");

        Token *tok = analyzeLexerNext(lexer);
        while (tok->type != TK_EOF) {
            if (tok->type == TK_IDENTIFIER || tok->type == TK_INTEGER) {
                printf("Type: %d, Literal: %s\n", tok->type, tok->literal);
            } else {
                printf("Type: %d\n", tok->type);
            }

            cleanupToken(&tok);
            tok = analyzeLexerNext(lexer);
        }

        cleanupLexer(&lexer);
        // get tokens

        // pass to lexer

        // free all memory
        free(array_textdata);
    }

    return 0;
}
