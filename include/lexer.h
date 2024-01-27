// 'lexer.h' - holds token types and renaisscript lexical analyzer

#ifndef LEXER_H_
#define LEXER_H_

#include "tokens.h"

typedef struct LexerStruct {
    const char *contents;
    unsigned long content_length;
    unsigned long index;
    unsigned long read_index;
    unsigned long line_number;
    unsigned long curr_line_start;
    char ch;
} Lexer;

// initialize lexical analysis
Lexer *initLexer(const char *contents);

// free lexer allocated memory
void lexerCleanUp(Lexer **lexer);

// iterate lexer to create and return tokens (tokenization and classification)
Token *lexerGetNextToken(Lexer *lexer);

// free token allocated memory
void tokenCleanup(Token **token);

// pass tokens here to filter TK_ERR and TK_ILLEGAL types
int lexerErrorHandler(Lexer *lexer, Token *token, const char *filename);

// start lexer process
TokenList* startLexer(const char* contents, unsigned int* return_error);

#endif // LEXER_H_
