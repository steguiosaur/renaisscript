// `optflags.h` - header file for argument parsing

#ifndef OPTFLAGS_H_
#define OPTFLAGS_H_

// access file argument names with 'inputfile' and 'outputfile'
extern const char *inputfile;  // access the specified file
extern const char *outputfile; // output executable name
extern const char *symbolfile;  // write symbol table to file
extern int symbolout;  // print symbol table to stdout

// detect argument type ( -h || -o <outputfile> [-s] || -v ) && inputfile
extern int parseOptionFlags(int argc, char *argv[]);

#endif // !OPTFLAGS_H_
