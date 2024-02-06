#include "fileread.h" // char *file_contents
#include "optflags.h" // char *inputfile, *outputfile
#include "lexer.h"    // lexical analyzer and tokens
#include "parser.h"    // lexical analyzer and tokens

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

    int return_error = 0;

    if (file_contents == NULL) {
        return 1;
    }

    // process inputfile's characters
    /* Lexer* lexer = startLexer(file_contents, &return_error); */
    /* Parser* parser = startParser(lexer, ) */


    Lexer* lexer = initLexer(file_contents);
    Parser* parser = initParser(lexer);
    AST* root = parser_parse(parser);

    printf("%d\n", root->type);
    printf("Hello World\n");







    if (return_error) {
        return 1;
    }

    return 0;
}
