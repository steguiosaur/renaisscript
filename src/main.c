#include "fileread.h" // char *array_textdata
#include "lexer.h"       // lexical analyzer and tokens
#include "optflags.h"    // char *inputfile, *outputfile

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

    FILE *symbol_file = fopen("test/symbol-table.txt", "w");
    // process inputfile's characters
    if (array_textdata != NULL) {
        // read inputfile
        printf("Input File: %s\n", inputfile);
        printf("Output File: %s\n", outputfile);
        printf("%s", array_textdata);

        Lexer *lexer = initLexer(array_textdata);

        fprintf(symbol_file, "[TokenType - Lexeme] Symbol table\n");

        Token *tok = lexerGetNextToken(lexer);
        while (tok->type != TK_EOF) {

            fprintf(symbol_file, "%-15s %-s\n", tk_map[tok->type], tok->lexeme);

            tokenCleanup(&tok);
            tok = lexerGetNextToken(lexer);
        }

        tokenCleanup(&tok);
        lexerCleanUp(&lexer);
        fclose(symbol_file);
        cleanupTextData();
    }

    return 0;
}
