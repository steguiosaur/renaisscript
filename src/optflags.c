// optflags header implementation
//
// `optflags.c` handles option flags using getopt to parse through arguments
// and set input and output files.
//
// See getopt(3) manual
// https://man7.org/linux/man-pages/man3/getopt.3.html

#include "optflags.h"

#include <getopt.h>
#include <stdio.h>

const char *inputfile = NULL;  // access the specified file
const char *outputfile = NULL; // output executable name
const char *symbolfile = NULL;  // write symbol table to file
int symbolout = 0;  // print symbol table to stdout

static void displayVersionInfo();
static void displayHelpGuide();

/// PUBLIC FUNCTIONS

int parseOptionFlags(const int argc, char *argv[]) {
    opterr = 0; // remove default getopt() error

    while (1) {
        // define flag options with and without argument
        int flag = getopt(argc, argv, "o:s:Svh");

        // no option flags detected starting with '-'
        if (flag == -1) {
            break;
        }

        switch (flag) {
        case 'o':
            outputfile = optarg;
            break;
        case 's':
            symbolfile = optarg;
            break;
        case 'S':
            symbolout = 1;
            break;
        case 'v':
            displayVersionInfo();
            return 0;
        case 'h':
            displayHelpGuide();
            return 0;
        default:
            displayHelpGuide();
            printf("ERROR: option or flag '-%c' undefined "
                   "[UNKNOWN_OPTION_ERROR]\n", optopt);
            return 1;
        }
    }

    // set outputfile to a.out if no '-o <fileout>' option specified
    if (outputfile == NULL) {
        outputfile = "a.out";
    }

    // no argument found after command or option '-o'
    if (optind > argc - 1) {
        displayHelpGuide();
        printf("ERROR: input file not found in arguments "
               "[UNSPECIFIED_FILE_ERROR]\n");
        return 1;
    }

    // get only 1 file argument and fail if > 1
    if (optind < argc - 1) {
        printf("ERROR: unparsed argument/s detected: ");
        for (int i = optind + 1; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("[UNPARSED_ARGUMENTS_ERROR]\n");
        return 1;
    }

    // detect incomplete option flag
    if (argv[optind][0] == '-') {
        printf("ERROR: incomplete option flag '%s' on argument %d"
               "[INCOMPLETE_FLAG_ERROR]\n",
               argv[optind], optind);
        return 1;
    }

    // assign single unparsed argument to inputfile
    if (inputfile == NULL) {
        inputfile = argv[optind];
        return 0;
    }

    printf("ERROR: uncaught error [UNCAUGHT_ERROR]\n");
    return 1;
}

/// PRIVATE FUNCTIONS

static void displayVersionInfo() {
    printf("Renaisscript compiler version 0.1.0\n");
}

static void displayHelpGuide() {
    printf("Usage: renaisscript [option...] [rensfile...].rens\n"
           "\n"
           "  -h                print help guide and exit successfully\n"
           "  -o <filename>     write output to file\n"
           "  -s <filename>     write symbol table to file\n"
           "  -S                print symbol table to stdout\n"
           "  -v                print version and exit successfully\n"
           "\n"
           "Report issues on github.com/steguiosaur/renaisscript/issues\n");
}
