// `optflags.h` - header file for argument parsing
//
// Usage: renaisscript [option...] [rensfile...].rens
//   -o <filename>     write output to file
//   -h                print this help guide
//   -v                print version information
// Only one option flag must be specified.\n");

#ifndef OPTFLAGS_H_
#define OPTFLAGS_H_

// access file argument names with 'inputfile' and 'outputfile'
extern const char *inputfile;  // access the specified file
extern const char *outputfile; // output executable name

// detect argument type ( -h || -o <outputfile> || -v ) && inputfile
extern int parseOptionFlags(int argc, char *argv[]);

#endif // !OPTFLAGS_H_
