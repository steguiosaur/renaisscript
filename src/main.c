#include "optflags.h"
#include "fileread.h"
#include <stdio.h>

int main(const int argc, char **argv) {
    // optflags.h - parse command line arguments
    if (parseOptionFlags(argc, argv)) {
        return 1;
    }

    // fileread.h - validate extension and get contents in file
    if (getRensFileContents(inputfile)) {
        return 1;
    }

    // read inputfile
    printf("Input File: %s\n", inputfile);
    printf("Output File: %s\n", outputfile);
    printf("%s", array_textdata);

    // get tokens

    // pass to lexer

    freeTextdata();
    return 0;
}
