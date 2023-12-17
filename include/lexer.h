// 'lexer.h' - holds token types and renaisscript lexical analyzer

#ifndef LEXER_H_
#define LEXER_H_

// token types
typedef enum {
    TK_ILLEGAL,
    TK_EOF,
    TK_IDENTIFIER,
    TK_STRING,
    TK_INT,
    TK_INTLIT,
    TK_ASSIGN,
    TK_PLUS,
    TK_MINUS,
    TK_BANG,
    TK_ASTERISK,
    TK_SLASH,
    TK_LT,
    TK_GT,
    TK_EQUAL,
    TK_NOTEQUAL,
    TK_COMMA,
    TK_SEMICOLON,
    TK_LPAREN,
    TK_RPAREN,
    TK_LCURLY,
    TK_RCURLY,
    TK_OUT,
    TK_IN,
    TK_FUNCTION,
    TK_LET,
    TK_TRUE,
    TK_FALSE,
    TK_IF,
    TK_ELSE,
    TK_RETURN,
} TokenType;

// tokenizer
typedef struct TokenStruct {
    TokenType type;
    char *lexeme;
} Token;

// lexical analyzer
typedef struct LexerStruct {
    const char *contents;
    unsigned long content_length;
    unsigned long index;
    unsigned long read_index;
    char ch;
} Lexer;

// start lexical analysis
Lexer *initLexer(const char *contents);

// iterate on lexer to get and create each token
Token *lexerGetNextToken(Lexer *lexer);

// free allocated memory token
void tokenCleanup(Token **token);

// free allocated memory lexer
void lexerCleanUp(Lexer **lexer);

#endif // LEXER_H_
