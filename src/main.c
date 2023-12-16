#include "fileread.h" // char *array_textdata
#include "optflags.h" // char *inputfile, *outputfile
#include "lexer.h" // lexer_T* init_lexer
#include <stdio.h>

FILE* fp;

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
        // get tokens

        // pass to lexer
        lexer_T* lexer = init_lexer(array_textdata);
        token_T* token = (void*)0;

        fp = fopen("test/tokens.txt", "w");
        while ((token = lexer_get_next_token(lexer)) != (void*)0) {
            fprintf(fp, "TOKEN(%d, %s)\n", token->type, token->value);
        }
        fclose(fp);
        
        // free all memory
        free(array_textdata);
    }

    return 0;
}
