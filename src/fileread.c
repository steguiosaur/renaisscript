// fileread header implementation
//
// `fileread.c` scans if file is the accepted extension file (*.rens ||
// *.rn). It also allocates memory for a dynamic array where the contents of the
// file is stored.
// Responsible for writing the symbol table text file when -s option is raised

#include "fileread.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *file_contents = NULL; // access file character array
char *str_out = NULL;       // store symbol table

int getRensFileContents(const char *filename) {
    // detect rens or rn file extension
    if (!(strstr(filename, ".rens") || strstr(filename, ".rn"))) {
        printf("ERROR: unrecognized file extension '%s' [FILE_EXTENSION_ERROR]\n",
               filename);
        return 1;
    }

    // open given filename input
    FILE *file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        printf("error: '%s'\n", filename);
        return 1;
    }

    // set array size to be allocated on heap via length of chars in file
    fseek(file_ptr, 0, SEEK_END);
    unsigned long size = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

    // allocate memory for array
    file_contents = (char *)malloc(size + 1);
    if (file_contents == NULL) {
        printf("ERROR: file contents memory allocation failure "
               "[CONTENT_ALLOCATION_ERROR]\n");
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

// continously collect token and lexeme strings on lexer
void collectStringOutput(const char *tok_name, char *lexeme) {
    unsigned long needed = snprintf(NULL, 0, "%-15s %-s\n", tok_name, lexeme);
    char *buffer = (char *)malloc(needed + 1);
    sprintf(buffer, "%-15s %-s\n", tok_name, lexeme);

    if (str_out == NULL) {
        str_out = (char *)malloc(strlen(buffer) + 1);
        strcpy(str_out, buffer);
    } else {
        str_out = (char *)realloc(str_out, strlen(buffer) + strlen(str_out) + 1);
        strcat(str_out, buffer);
    }

    free(buffer);
}

void printCollectedStringOutput() {
    printf("%s", str_out);
}

// write collected strings from str_out to file
int storeCollectedStringOutput(const char *filename) {
    if (filename == NULL) {
        printf("ERROR: received NULL as file. Filter"
               "storeCollectedStringOutput() for a non-NULL value"
               "[NULL_FILE_ERROR]\n");
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
