#include "fileread.h" // char *file_contents
#include "lexer.h"    // lexical analyzer and tokens
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

    unsigned int return_error = 0;

    // process inputfile's characters
    if (file_contents != NULL) {
        Lexer *lexer = initLexer(file_contents);

        Token *tok = lexerGetNextToken(lexer);
        while (tok->type != TK_EOF) {

            // print error and exit fail if token type ERR and INVALID detected
            if (lexerErrorHandler(lexer, tok, inputfile)) {
                return_error = 1;
            }

            // for symbol table file output
            if (symbolout == 1 || symbolfile != NULL) {
                collectStringOutput(tk_map[tok->type], tok->lexeme);
            }

            tokenCleanup(&tok);
            tok = lexerGetNextToken(lexer);
        }

        if (symbolout) {
            printCollectedStringOutput();
        }

        // write symbol table on specified symbol file in arguments
        if (symbolfile != NULL) {
            storeCollectedStringOutput(symbolfile);
        }

        tokenCleanup(&tok);
        lexerCleanUp(&lexer);
        cleanupCollectedString();
        cleanupFileContents();
    }

    if (return_error) {
        return 1;
    }

    return 0;
}
