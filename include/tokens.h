#ifndef TOKENS_H_
#define TOKENS_H_

// token types
typedef enum {
    // error tokens
    TK_ILLEGALCHR,
    TK_EMPTYCHERR,
    TK_MULTICHERR,
    TK_FLOATERR,
    TK_STREOFERR,

    // end of file
    TK_EOF,

    // comments
    TK_BLCKCOMMENT,
    TK_LINECOMMENT,

    // literals
    TK_CHARACLIT,
    TK_STRINGLIT,
    TK_INTLIT,
    TK_FLTLIT,

    // symbols
    TK_AT,
    TK_TILDE,
    TK_PIPE,
    TK_AMPERSAND,

    // logical operator
    TK_AND,
    TK_OR,
    TK_BANG,

    // arithmetic operator
    TK_PLUS,
    TK_MINUS,
    TK_ASTERISK,
    TK_EXPONENT,
    TK_SLASH,
    TK_FLOORDIV,
    TK_MODULO,

    // assignment operator
    TK_ASSIGN,
    TK_ASSIGNINC,
    TK_ASSIGNDEC,
    TK_ASSIGNMUL,
    TK_ASSIGNDIV,
    TK_ASSIGNMOD,
    TK_INCREMENT,
    TK_DECREMENT,

    // relational operator
    TK_EQUAL,
    TK_GT,
    TK_GEQUAL,
    TK_LT,
    TK_LEQUAL,
    TK_NOTEQUAL,

    // delimeters and brackets
    TK_COMMA,
    TK_DOT,
    TK_SEMICOLON,
    TK_COLON,
    TK_LPAREN,
    TK_RPAREN,
    TK_LBRACKET,
    TK_RBRACKET,
    TK_LCURLY,
    TK_RCURLY,

    // data types
    TK_INT,
    TK_CHAR,
    TK_FLOAT,
    TK_DOUBLE,
    TK_BOOL,
    TK_VOID,

    // etc keywords, noise words
    TK_IDENTIFIER,
    TK_VOLATILE,
    TK_REGISTER,

    // stdin and stdout
    TK_OUT,
    TK_IN,

    // tags
    TK_FUNCTION,
    TK_LET,

    // boolean
    TK_TRUE,
    TK_FALSE,

    // loops and conditionals
    TK_IF,
    TK_ELSE,
    TK_WHILE,
    TK_FOR,
    TK_CONTINUE,
    TK_GOTO,
    TK_SWITCH,
    TK_CASE,
    TK_BREAK,
    TK_RETURN,
} TokenType;

typedef struct TokenStruct {
    TokenType type;
    char *lexeme;
} Token;

// for printing actual TokenType string
static const char *const tk_map[] = {
    "TK_ILLEGALCHR",
    "TK_EMPTYCHERR",
    "TK_MULTICHERR",
    "TK_FLOATERR",
    "TK_STREOFERR",
    "TK_EOF",
    "TK_BLCKCOMMENT",
    "TK_LINECOMMENT",
    "TK_CHARACLIT",
    "TK_STRINGLIT",
    "TK_INTLIT",
    "TK_FLTLIT",
    "TK_AT",
    "TK_TILDE",
    "TK_PIPE",
    "TK_AMPERSAND",
    "TK_AND",
    "TK_OR",
    "TK_BANG",
    "TK_PLUS",
    "TK_MINUS",
    "TK_ASTERISK",
    "TK_EXPONENT",
    "TK_SLASH",
    "TK_FLOORDIV",
    "TK_MODULO",
    "TK_ASSIGN",
    "TK_ASSIGNINC",
    "TK_ASSIGNDEC",
    "TK_ASSIGNMUL",
    "TK_ASSIGNDIV",
    "TK_ASSIGNMOD",
    "TK_INCREMENT",
    "TK_DECREMENT",
    "TK_EQUAL",
    "TK_GT",
    "TK_GEQUAL",
    "TK_LT",
    "TK_LEQUAL",
    "TK_NOTEQUAL",
    "TK_COMMA",
    "TK_DOT",
    "TK_SEMICOLON",
    "TK_COLON",
    "TK_LPAREN",
    "TK_RPAREN",
    "TK_LBRACKET",
    "TK_RBRACKET",
    "TK_LCURLY",
    "TK_RCURLY",
    "TK_INT",
    "TK_CHAR",
    "TK_FLOAT",
    "TK_DOUBLE",
    "TK_BOOL",
    "TK_VOID",
    "TK_IDENTIFIER",
    "TK_VOLATILE",
    "TK_REGISTER",
    "TK_OUT",
    "TK_IN",
    "TK_FUNCTION",
    "TK_LET",
    "TK_TRUE",
    "TK_FALSE",
    "TK_IF",
    "TK_ELSE",
    "TK_WHILE",
    "TK_FOR",
    "TK_CONTINUE",
    "TK_GOTO",
    "TK_SWITCH",
    "TK_CASE",
    "TK_BREAK",
    "TK_RETURN",
};

#endif // !TOKENS_H_
