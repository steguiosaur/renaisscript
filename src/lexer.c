#include "lexer.h"

#include <stdlib.h>
#include <string.h>

static Token *tokenCreate(TokenType type, char *lexeme);

static void lexerSkipWhitespace(Lexer *lexer);
static void lexerReadNextChar(Lexer *lexer);
static char lexerPeekNextChar(Lexer *lexer);
static char *lexerGetLexAsString(Lexer *lexer, unsigned long start_idx);

static int isValidIdentifier(char chr);
static int isValidNumber(char chr);

static TokenType lexerIdReservedKeyword(const char *ident, unsigned long len);

// PUBLIC FUNCTIONS

Lexer *initLexer(const char *contents) {
    Lexer *lexer = calloc(1, sizeof(Lexer));

    lexer->contents = contents;
    lexer->content_length = strlen(contents);
    lexer->index = 0;
    lexer->read_index = 0;

    return lexer;
}

Token *lexerGetNextToken(Lexer *lexer) {
    lexerReadNextChar(lexer);
    lexerSkipWhitespace(lexer);

    const unsigned long start_index = lexer->index;

    switch (lexer->ch) {
    case '{':
        return tokenCreate(TK_LCURLY, lexerGetLexAsString(lexer, start_index));
    case '}':
        return tokenCreate(TK_RCURLY, lexerGetLexAsString(lexer, start_index));
    case '(':
        return tokenCreate(TK_LPAREN, lexerGetLexAsString(lexer, start_index));
    case ')':
        return tokenCreate(TK_RPAREN, lexerGetLexAsString(lexer, start_index));
    case ',':
        return tokenCreate(TK_COMMA, lexerGetLexAsString(lexer, start_index));
    case ';':
        return tokenCreate(TK_SEMICOLON, lexerGetLexAsString(lexer, start_index));
    case '+':
        return tokenCreate(TK_PLUS, lexerGetLexAsString(lexer, start_index));
    case '-':
        return tokenCreate(TK_MINUS, lexerGetLexAsString(lexer, start_index));
    case '=':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_EQUAL, lexerGetLexAsString(lexer, start_index));
        }
        return tokenCreate(TK_ASSIGN, lexerGetLexAsString(lexer, start_index));
    case '!':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_NOTEQUAL, lexerGetLexAsString(lexer, start_index));
        }
        return tokenCreate(TK_BANG, lexerGetLexAsString(lexer, start_index));
    case '/':
        return tokenCreate(TK_SLASH, lexerGetLexAsString(lexer, start_index));
    case '*':
        return tokenCreate(TK_ASTERISK, lexerGetLexAsString(lexer, start_index));
    case '>':
        return tokenCreate(TK_GT, lexerGetLexAsString(lexer, start_index));
    case '<':
        return tokenCreate(TK_LT, lexerGetLexAsString(lexer, start_index));
    case '\0':
        return tokenCreate(TK_EOF, lexerGetLexAsString(lexer, start_index));
    default:
        break;
    }

    if (lexer->ch == '"') {
        lexerReadNextChar(lexer);
        while (lexer->ch != '"') {
            lexerReadNextChar(lexer);
        }

        return tokenCreate(TK_STRING, lexerGetLexAsString(lexer, start_index));
    }

    if (isValidIdentifier(lexer->ch)) {
        while (isValidIdentifier(lexerPeekNextChar(lexer))) {
            lexerReadNextChar(lexer);
        }

        const char *value = lexer->contents + start_index;
        unsigned long len = lexer->index - start_index + 1;
        char *lexeme = strndup(value, len);

        TokenType type = lexerIdReservedKeyword(value, len);
        return tokenCreate(type, lexeme);
    }

    if (isValidNumber(lexer->ch)) {
        while (isValidNumber(lexerPeekNextChar(lexer))) {
            lexerReadNextChar(lexer);
        }

        return tokenCreate(TK_INTLIT, lexerGetLexAsString(lexer, start_index));
    }

    return tokenCreate(TK_ILLEGAL, lexerGetLexAsString(lexer, start_index));
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

static char *lexerGetLexAsString(Lexer *lexer, const unsigned long start_idx) {
    const char *value = lexer->contents + start_idx;
    unsigned long len = lexer->index - start_idx + 1;
    char* lexeme = strndup(value, len);
    return lexeme;
}

static int isValidIdentifier(const char chr) {
    return 'a' <= chr && chr <= 'z' || 'A' <= chr && chr <= 'Z' || chr == '_';
}

static int isValidNumber(const char chr) { return '0' <= chr && '9' >= chr; }

// detect if identifier is a reserved keyword
static TokenType lexerIdReservedKeyword(const char *ident, unsigned long len) {
    if (strncmp(ident, "maketh", len) == 0) {
        return TK_LET;
    }

    if (strncmp(ident, "define", len) == 0) {
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
