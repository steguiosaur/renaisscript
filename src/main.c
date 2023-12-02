#include "optflags.h"
#include <stdio.h>

int main(const int argc, char *argv[]) {
    // parse command line arguments
    int status = parseOptionFlags(argc, argv);
    if (status) {
        return status;
    }

    // confirm inputfile == .rens || .rn file extension
    printf("%s\n", inputfile);
    printf("%s\n", outputfile);
    // read inputfile

    // get tokens

    // pass to lexer

    return 0;
}
