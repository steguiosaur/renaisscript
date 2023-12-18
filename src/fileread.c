// fileread header implementation
//
// `fileread.c` scans if file is the accepted extension file (*.rens ||
// *.rn). It also allocates memory for a dynamic array where the contents of the
// file is stored.
//
// Several references:
// https://stackoverflow.com/questions/54943083

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *array_textdata = NULL; // access file character array

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

    fseek(file_ptr, 0, SEEK_END);
    unsigned long size = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

    array_textdata = (char *)malloc(size + 1);
    if (array_textdata == NULL) {
        printf("[ERROR] Memory Allocation Failure\n");
        fclose(file_ptr);
        return 1;
    }

    fread(array_textdata, 1, size, file_ptr);
    (array_textdata)[size] = '\0';

    fclose(file_ptr);
    return 0;
}

// free allocated array_textdata memory
void cleanupTextData() { 
    if (array_textdata != NULL) {
        free(array_textdata); 
    }
    array_textdata = NULL;
}
