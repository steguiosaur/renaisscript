// `fileread.h` - header file for reading contents of rens file
//
// `fileread.c` scans if file is the accepted extension file (*.rens || *.rn).
// It also allocates memory for a dynamic array where the contents of the
// file is stored.

// access the file contents with 'array_textdata'
extern char *array_textdata;

// detect file extension (*.rens || *.rn) and store values to 'array_textdata'
extern int getRensFileContents(const char *filename);

// free allocated array_textdata memory
void cleanupTextData();
