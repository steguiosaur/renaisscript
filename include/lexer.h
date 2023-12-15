#ifndef _LEXER_H_
#define _LEXER_H_

typedef enum {
    TK_ILLEGAL,
    TK_EOF,
    TK_IDENTIFIER,
    TK_INTEGER,
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
    TK_FUNCTION,
    TK_LET,
    TK_TRUE,
    TK_FALSE,
    TK_IF,
    TK_ELSE,
    TK_RETURN,
} TokenType;

typedef struct TokenStruct {
    TokenType type;
    char *literal;
} Token;

typedef struct LexerStruct Lexer;
Lexer *createLexer(const char *input);

Token *analyzeLexerNext(Lexer *lexer);
void cleanupLexer(Lexer **lexer);

Token *createToken(TokenType type, char *literal);
void cleanupToken(Token **token);

#endif // _LEXER_H_
