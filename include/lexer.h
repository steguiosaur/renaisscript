// 'lexer.h' - holds token types and renaisscript lexical analyzer

#ifndef LEXER_H_
#define LEXER_H_

// token types
typedef enum {
    TK_ILLEGAL,
    TK_EOF,
    TK_COMMENT,
    TK_IDENTIFIER,
    TK_STRING,
    TK_INTLIT,
    TK_AMPERSAND,
    TK_AND,
    TK_OR,
    TK_BANG,
    TK_PLUS,
    TK_MINUS,
    TK_ASTERISK,
    TK_EXPONENT,
    TK_SLASH,
    TK_ASSIGN,
    TK_EQUAL,
    TK_GT,
    TK_GEQUAL,
    TK_LT,
    TK_LEQUAL,
    TK_NOTEQUAL,
    TK_COMMA,
    TK_SEMICOLON,
    TK_COLON,
    TK_LPAREN,
    TK_RPAREN,
    TK_LCURLY,
    TK_RCURLY,

    TK_INT,
    TK_CHAR,
    TK_FLOAT,
    TK_DOUBLE,
    TK_BOOL,
    TK_VOID,
    TK_GOTO,
    TK_SWITCH,
    TK_CASE,
    TK_BREAK,
    TK_OUT,
    TK_IN,
    TK_FUNCTION,
    TK_LET,
    TK_TRUE,
    TK_FALSE,
    TK_IF,
    TK_ELSE,
    TK_WHILE,
    TK_CONTINUE,
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

// free lexer allocated memory
void lexerCleanUp(Lexer **lexer);

// iterate on lexer to get and create each token
Token *lexerGetNextToken(Lexer *lexer);

// free token allocated memory
void tokenCleanup(Token **token);

#endif // LEXER_H_
