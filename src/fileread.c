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

    // determine length of characters in file
    size_t size = 1;
    while (getc(file_ptr) != EOF) {
        size++;
    }

    // create memory with sizeof file contents character length
    array_textdata = (char *)malloc(sizeof(char) * size);

    fseek(file_ptr, 0, SEEK_SET); // Reset file pointer to begin
    // assign each character to each array_textdata index
    for (size_t i = 0; i < size - 1; i++) {
        array_textdata[i] = (char)getc(file_ptr);
    }

    array_textdata[size - 1] = '\0';

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
