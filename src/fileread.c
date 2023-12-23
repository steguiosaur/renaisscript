// fileread header implementation
//
// `fileread.c` scans if file is the accepted extension file (*.rens ||
// *.rn). It also allocates memory for a dynamic array where the contents of the
// file is stored.
//
// Several references:
// https://stackoverflow.com/questions/54943083

#include "fileread.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *file_contents = NULL; // access file character array
char *str_out = NULL;        // store symbol table

int getRensFileContents(const char *filename) {
    // detect rens or rn file extension
    if (!(strstr(filename, ".rens") || strstr(filename, ".rn"))) {
        printf("[ERROR] FileNotSupported: unrecognized file extension '%s'\n",
               filename);
        return 1;
    }

    // open given filename input
    FILE *file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        printf("[ERROR] FileNotFound: '%s'\n", filename);
        return 1;
    }

    // set array size to be allocated on heap via length of chars in file
    fseek(file_ptr, 0, SEEK_END);
    unsigned long size = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

    // allocate memory for array
    file_contents = (char *)malloc(size + 1);
    if (file_contents == NULL) {
        printf("[ERROR] Memory Allocation Failure\n");
        fclose(file_ptr);
        return 1;
    }

    // put all characters in file to the array
    fread(file_contents, 1, size, file_ptr);
    (file_contents)[size] = '\0';

    fclose(file_ptr);
    return 0;
}

// free allocated file_contents memory
void cleanupFileContents() {
    if (file_contents != NULL) {
        free(file_contents);
    }
    file_contents = NULL;
}

// continously collect token and lexeme strings on lexer (NEEDS REFACTOR)
void collectStringOutput(const char *tok_name, char *lexeme) {
    const int size_no_lex = 16; // from justified tok_name field + 1 for space
    char *new_str = (char *)malloc(size_no_lex + sizeof(lexeme) + 1);
    sprintf(new_str, "%-15s %-s\n", tok_name, lexeme);

    if (str_out == NULL) {
        str_out = (char *)malloc(strlen(new_str) + 1);
        strcpy(str_out, new_str);
    } else {
        str_out = (char *)realloc(str_out, strlen(new_str) + strlen(str_out) + 1);
        strcat(str_out, new_str);
    }

    free(new_str);
}

// write collected strings from str_out to file
int storeCollectedStringOutput(const char *filename) {
    if (filename == NULL) {
        printf("[ERROR] NullFilePtr: received NULL as file."
               "Filter storeCollectedStringOutput() for a non-NULL value\n");
        return 1;
    }

    if (str_out != NULL) {
        FILE *file_ptr = fopen(filename, "w");
        fprintf(file_ptr, "%s", str_out);
        fclose(file_ptr);
    }

    return 0;
}

// free allocated str_out memory
void cleanupCollectedString() {
    if (str_out != NULL) {
        free(str_out);
    }
    str_out = NULL;
}
