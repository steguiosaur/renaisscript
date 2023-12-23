// `fileread.h` - header file for reading contents of rens file
//
// `fileread.c` scans if file is the accepted extension file (*.rens || *.rn).
// It also allocates memory for a dynamic array where the contents of the
// file is stored.

// access the file contents with 'file_contents'
extern char *file_contents;
extern char *str_out; // store symbol table

// detect file extension (*.rens || *.rn) and store values to 'file_contents'
extern int getRensFileContents(const char *filename);

// free allocated file_contents memory
void cleanupFileContents();

// continously collect token and lexeme strings on lexer
void collectStringOutput(const char *tok_name, char *lexeme);

// write collected strings from str_out to file
int storeCollectedStringOutput(const char *filename);

// free allocated str_out memory
void cleanupCollectedString();
