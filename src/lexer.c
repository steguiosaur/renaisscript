#include "lexer.h"

#include <stdlib.h>
#include <string.h>

static Token *tokenCreate(TokenType type, char *lexeme);

static void lexerSkipWhitespace(Lexer *lexer);
static void lexerReadNextChar(Lexer *lexer);
static char lexerPeekNextChar(Lexer *lexer);
static char *lexerGetCharAsString(Lexer *lexer);
static const char *lexerReadIdentifier(Lexer *lexer, unsigned long *len);
static const char *lexerReadInteger(Lexer *lexer, unsigned long *len);

static unsigned long isLetter(char chr);
static unsigned long isNumber(char chr);

static TokenType lexerCollectKeyword(const char *ident, unsigned long len);

// PUBLIC FUNCTIONS

Lexer *initLexer(const char *contents) {
    Lexer *lexer = calloc(1, sizeof(Lexer));

    lexer->contents = contents;
    lexer->content_length = strlen(contents);
    lexer->index = 0;
    lexer->read_index = 0;

    lexerReadNextChar(lexer);

    return lexer;
}

Token *lexerGetNextToken(Lexer *lexer) {
    Token *tok = NULL;

    lexerSkipWhitespace(lexer);

    switch (lexer->ch) {
    case '{':
        tok = tokenCreate(TK_LCURLY, lexerGetCharAsString(lexer));
        break;
    case '}':
        tok = tokenCreate(TK_RCURLY, lexerGetCharAsString(lexer));
        break;
    case '(':
        tok = tokenCreate(TK_LPAREN, lexerGetCharAsString(lexer));
        break;
    case ')':
        tok = tokenCreate(TK_RPAREN, lexerGetCharAsString(lexer));
        break;
    case ',':
        tok = tokenCreate(TK_COMMA, lexerGetCharAsString(lexer));
        break;
    case ';':
        tok = tokenCreate(TK_SEMICOLON, lexerGetCharAsString(lexer));
        break;
    case '+':
        tok = tokenCreate(TK_PLUS, lexerGetCharAsString(lexer));
        break;
    case '-':
        tok = tokenCreate(TK_MINUS, lexerGetCharAsString(lexer));
        break;
    case '=':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            tok = tokenCreate(TK_EQUAL, lexerGetCharAsString(lexer));
        } else {
            tok = tokenCreate(TK_ASSIGN, lexerGetCharAsString(lexer));
        }
        break;
    case '!':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            tok = tokenCreate(TK_NOTEQUAL, lexerGetCharAsString(lexer));
        } else {
            tok = tokenCreate(TK_BANG, lexerGetCharAsString(lexer));
        }
        break;
    case '/':
        tok = tokenCreate(TK_SLASH, lexerGetCharAsString(lexer));
        break;
    case '*':
        tok = tokenCreate(TK_ASTERISK, lexerGetCharAsString(lexer));
        break;
    case '>':
        tok = tokenCreate(TK_GT, lexerGetCharAsString(lexer));
        break;
    case '<':
        tok = tokenCreate(TK_LT, lexerGetCharAsString(lexer));
        break;
    case '\0':
        tok = tokenCreate(TK_EOF, lexerGetCharAsString(lexer));
        break;
    default:
        break;
    }

    if (isLetter(lexer->ch)) {
        unsigned long len = 0;
        char *lexeme = NULL;
        const char *ident = lexerReadIdentifier(lexer, &len);

        TokenType type = lexerCollectKeyword(ident, len);
        lexeme = strndup(ident, len);

        tok = tokenCreate(type, lexeme);
        return tok;
    }

    if (isNumber(lexer->ch)) {
        unsigned long len = 0;
        char *lexeme = NULL;
        const char *ident = lexerReadInteger(lexer, &len);

        lexeme = strndup(ident, len);
        tok = tokenCreate(TK_INTLIT, lexeme);
        return tok;
    }

    if (!tok) {
        tok = tokenCreate(TK_ILLEGAL, lexerGetCharAsString(lexer));
    }

    lexerReadNextChar(lexer);

    return tok;
}

void lexerCleanUp(Lexer **lexer) {
    if (*lexer) {
        free(*lexer);
    }

    *lexer = NULL;
}

void tokenCleanup(Token **token) {
    if (*token && (*token)->lexeme) {
        free((*token)->lexeme);
    }

    if (*token) {
        free(*token);
    }

    *token = NULL;
}

// PRIVATE FUNCTIONS

static Token *tokenCreate(TokenType type, char *lexeme) {
    Token *token = calloc(1, sizeof(Token));

    token->lexeme = lexeme;
    token->type = type;

    return token;
}

static char *lexerGetCharAsString(Lexer *lexer) {
    char *str = calloc(1, sizeof(lexer->ch));

    for (int i = 0; i == sizeof(lexer->ch) - 1; ++i) {
        str[i] = lexer->ch;
    }

    return str;
}

static unsigned long isLetter(char chr) {
    return 'a' <= chr && chr <= 'z' || 'A' <= chr && chr <= 'Z' || chr == '_';
}

static unsigned long isNumber(char chr) { return '0' <= chr && '9' >= chr; }

static void lexerSkipWhitespace(Lexer *lexer) {
    while (lexer->ch == ' ' || lexer->ch == '\t' || lexer->ch == '\n' ||
           lexer->ch == '\r') {
        lexerReadNextChar(lexer);
    }
}

static void lexerReadNextChar(Lexer *lexer) {
    if (lexer->read_index >= lexer->content_length) {
        lexer->ch = '\0';
    } else {
        lexer->ch = lexer->contents[lexer->read_index];
    }

    lexer->index = lexer->read_index;
    lexer->read_index++;
}

static char lexerPeekNextChar(Lexer *lexer) {
    if (lexer->read_index >= lexer->content_length) {
        return '\0';
    }
    return lexer->contents[lexer->read_index];
}

static const char *lexerReadIdentifier(Lexer *lexer, unsigned long *len) {
    char *result = NULL;
    unsigned long index = lexer->index;

    while (isLetter(lexer->ch)) {
        lexerReadNextChar(lexer);
    }

    if (len) {
        *len = lexer->index - index;
    }

    return lexer->contents + index;
}

static const char *lexerReadInteger(Lexer *lexer, unsigned long *len) {
    char *result = NULL;
    unsigned long index = lexer->index;

    while (isNumber(lexer->ch)) {
        lexerReadNextChar(lexer);
    }

    if (len) {
        *len = lexer->index - index;
    }

    return lexer->contents + index;
}

// detect if identifier is a reserved keyword
static TokenType lexerCollectKeyword(const char *ident, unsigned long len) {
    if (strncmp(ident, "maketh", len) == 0) {
        return TK_LET;
    }

    if (strncmp(ident, "fn", len) == 0) {
        return TK_FUNCTION;
    }

    if (strncmp(ident, "yay", len) == 0) {
        return TK_TRUE;
    }

    if (strncmp(ident, "nay", len) == 0) {
        return TK_FALSE;
    }

    if (strncmp(ident, "if", len) == 0) {
        return TK_IF;
    }

    if (strncmp(ident, "else", len) == 0) {
        return TK_ELSE;
    }

    if (strncmp(ident, "returneth", len) == 0) {
        return TK_RETURN;
    }

    if (strncmp(ident, "sayeth", len) == 0) {
        return TK_OUT;
    }

    if (strncmp(ident, "heareth", len) == 0) {
        return TK_IN;
    }

    if (strncmp(ident, "count", len) == 0) {
        return TK_INT;
    }

    return TK_IDENTIFIER;
}
