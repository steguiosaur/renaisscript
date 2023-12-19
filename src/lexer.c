// 'lexer.c' - lexical analyzer functionalities

#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Token *tokenCreate(TokenType type, char *lexeme);

static void lexerSkipWhitespace(Lexer *lexer);
static void lexerReadNextChar(Lexer *lexer);
static char lexerPeekNextChar(Lexer *lexer);
static char *lexerGetLexAsString(Lexer *lexer);

static int isValidIdentifier(char chr);
static int isValidNumber(char chr);

static TokenType lexerIdReservedKeyword(const char *ident, unsigned long len);

/// PUBLIC FUNCTIONS

// start lexical analysis
Lexer *initLexer(const char *contents) {
    Lexer *lexer = calloc(1, sizeof(Lexer));

    lexer->contents = contents;
    lexer->content_length = strlen(contents);
    lexer->index = 0;
    lexer->read_index = 0;
    lexer->line_number = 1;

    return lexer;
}

// iterate lexer to create and return tokens (tokenization and classification)
Token *lexerGetNextToken(Lexer *lexer) {
    lexerReadNextChar(lexer);
    lexerSkipWhitespace(lexer);

    lexer->index = lexer->read_index - 1; // mark token starting index

    switch (lexer->ch) {
    case '{':
        return tokenCreate(TK_LCURLY, lexerGetLexAsString(lexer));
    case '}':
        return tokenCreate(TK_RCURLY, lexerGetLexAsString(lexer));
    case '(':
        return tokenCreate(TK_LPAREN, lexerGetLexAsString(lexer));
    case ')':
        return tokenCreate(TK_RPAREN, lexerGetLexAsString(lexer));
    case '[':
        return tokenCreate(TK_LBRACKET, lexerGetLexAsString(lexer));
    case ']':
        return tokenCreate(TK_RBRACKET, lexerGetLexAsString(lexer));
    case ',':
        return tokenCreate(TK_COMMA, lexerGetLexAsString(lexer));
    case ';':
        return tokenCreate(TK_SEMICOLON, lexerGetLexAsString(lexer));
    case ':':
        return tokenCreate(TK_COLON, lexerGetLexAsString(lexer));
    case '%':
        return tokenCreate(TK_MODULO, lexerGetLexAsString(lexer));
    case '+':
        if (lexerPeekNextChar(lexer) == '+') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_INCREMENT, lexerGetLexAsString(lexer));
        }
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_ASSIGNINC, lexerGetLexAsString(lexer));
        }
        return tokenCreate(TK_PLUS, lexerGetLexAsString(lexer));
    case '-':
        if (lexerPeekNextChar(lexer) == '-') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_DECREMENT, lexerGetLexAsString(lexer));
        }
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_ASSIGNDEC, lexerGetLexAsString(lexer));
        }
        return tokenCreate(TK_MINUS, lexerGetLexAsString(lexer));
    case '=':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_EQUAL, lexerGetLexAsString(lexer));
        }
        return tokenCreate(TK_ASSIGN, lexerGetLexAsString(lexer));
    case '!':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_NOTEQUAL, lexerGetLexAsString(lexer));
        }
        return tokenCreate(TK_BANG, lexerGetLexAsString(lexer));
    case '/':
        if (lexerPeekNextChar(lexer) == '/') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_FLOORDIV, lexerGetLexAsString(lexer));
        }
        return tokenCreate(TK_SLASH, lexerGetLexAsString(lexer));
    case '*':
        if (lexerPeekNextChar(lexer) == '*') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_EXPONENT, lexerGetLexAsString(lexer));
        }
        return tokenCreate(TK_ASTERISK, lexerGetLexAsString(lexer));
    case '>':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_GEQUAL, lexerGetLexAsString(lexer));
        }
        return tokenCreate(TK_GT, lexerGetLexAsString(lexer));
    case '<':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_LEQUAL, lexerGetLexAsString(lexer));
        }
        return tokenCreate(TK_LT, lexerGetLexAsString(lexer));
    case '&':
        if (lexerPeekNextChar(lexer) == '&') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_AND, lexerGetLexAsString(lexer));
        }
        return tokenCreate(TK_AMPERSAND, lexerGetLexAsString(lexer));
    case '|':
        if (lexerPeekNextChar(lexer) == '|') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_OR, lexerGetLexAsString(lexer));
        }
        break;
    case '\0':
        return tokenCreate(TK_EOF, lexerGetLexAsString(lexer));
    default:
        break;
    }

    // detect string literals
    if (lexer->ch == '"') {
        lexerReadNextChar(lexer);
        while (lexer->ch != '"' && lexerPeekNextChar(lexer) != '\0') {
            if (lexer->ch == '\\' && lexerPeekNextChar(lexer) == '"') {
                lexerReadNextChar(lexer);
            }
            lexerReadNextChar(lexer);
        }

        if (lexerPeekNextChar(lexer) != '\0') {
            return tokenCreate(TK_STRING, lexerGetLexAsString(lexer));
        }

        return tokenCreate(TK_ERR, lexerGetLexAsString(lexer));
    }

    // detect identifier and keyword types
    if (isValidIdentifier(lexer->ch)) {
        while (isValidIdentifier(lexerPeekNextChar(lexer))) {
            lexerReadNextChar(lexer);
        }

        const char *value = lexer->contents + lexer->index;
        unsigned long len = lexer->read_index - lexer->index;
        char *lexeme = strndup(value, len);

        TokenType type = lexerIdReservedKeyword(value, len);
        return tokenCreate(type, lexeme);
    }

    // detect integer literals
    if (isValidNumber(lexer->ch)) {
        while (isValidNumber(lexerPeekNextChar(lexer))) {
            lexerReadNextChar(lexer);
        }

        return tokenCreate(TK_INTLIT, lexerGetLexAsString(lexer));
    }

    return tokenCreate(TK_ILLEGAL, lexerGetLexAsString(lexer));
}

// pass tokens here to filter TK_ERR and TK_ILLEGAL types
int lexerErrorHandler(Lexer *lexer, Token *token, const char *filename) {
    if (token->type == TK_ILLEGAL) {
        printf("[ERROR] InvalidToken: %s not recognized as token on %s "
               "line:%lu\n",
               token->lexeme, filename, lexer->line_number);
        return 1;
    }

    if (token->type == TK_ERR) {
        printf("[ERROR] UnterminatedString: unterminated string literal "
               "reached EOF on %s line:%lu\n",
               filename, lexer->line_number);
        return 1;
    }

    return 0;
}

// free lexer allocated memory
void lexerCleanUp(Lexer **lexer) {
    if (*lexer) {
        free(*lexer);
    }

    *lexer = NULL;
}

// free token allocated memory
void tokenCleanup(Token **token) {
    if (*token && (*token)->lexeme) {
        free((*token)->lexeme);
    }

    if (*token) {
        free(*token);
    }

    *token = NULL;
}

/// PRIVATE FUNCTIONS

// token builder (to be used by parser or syntax analyzer)
static Token *tokenCreate(TokenType type, char *lexeme) {
    Token *token = calloc(1, sizeof(Token));

    token->lexeme = lexeme;
    token->type = type;

    return token;
}

// skip whitespaces, unneeded file escape sequences, and comments
static void lexerSkipWhitespace(Lexer *lexer) {
    while (lexer->ch == ' ' || lexer->ch == '\t' || lexer->ch == '\n' ||
           lexer->ch == '\r' || lexer->ch == '#') {

        // track current line number
        if (lexer->ch == '\n') {
            lexer->line_number++;
        }

        // skip block line comment
        if (lexer->ch == '#' && lexerPeekNextChar(lexer) == '#') {
            lexerReadNextChar(lexer);
            while (lexerPeekNextChar(lexer) != '#' &&
                   lexerPeekNextChar(lexer) != '\0') {
                lexerReadNextChar(lexer);
                if (lexerPeekNextChar(lexer) == '#') {
                    lexerReadNextChar(lexer);
                }
            }
            lexerReadNextChar(lexer);
        }

        // skip single line comment
        if (lexer->ch == '#') {
            while (lexerPeekNextChar(lexer) != '\n') {
                lexerReadNextChar(lexer);
            }
        }

        lexerReadNextChar(lexer);
    }
}

// access next char value in lexer
static void lexerReadNextChar(Lexer *lexer) {
    if (lexer->read_index >= lexer->content_length) {
        lexer->ch = '\0';
    } else {
        lexer->ch = lexer->contents[lexer->read_index];
    }

    lexer->read_index++;
}

// view next char value in lexer
static char lexerPeekNextChar(Lexer *lexer) {
    if (lexer->read_index >= lexer->content_length) {
        return '\0';
    }
    return lexer->contents[lexer->read_index];
}

// return ptr of tracked lexeme index and end read_index in lexer->contents
static char *lexerGetLexAsString(Lexer *lexer) {
    const char *value = lexer->contents + lexer->index;
    unsigned long len = lexer->read_index - lexer->index;
    char *lexeme = strndup(value, len);
    return lexeme;
}

static int isValidIdentifier(const char chr) {
    return 'a' <= chr && chr <= 'z' || 'A' <= chr && chr <= 'Z' || chr == '_';
}

static int isValidNumber(const char chr) { return '0' <= chr && '9' >= chr; }

// detect if given identifier is a reserved keyword and return the type
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

    if (strncmp(ident, "glyph", len) == 0) {
        return TK_CHAR;
    }

    if (strncmp(ident, "portion", len) == 0) {
        return TK_FLOAT;
    }

    if (strncmp(ident, "fraction", len) == 0) {
        return TK_DOUBLE;
    }

    if (strncmp(ident, "verdict", len) == 0) {
        return TK_BOOL;
    }

    if (strncmp(ident, "nought", len) == 0) {
        return TK_VOID;
    }

    if (strncmp(ident, "thither", len) == 0) {
        return TK_GOTO;
    }

    if (strncmp(ident, "switch", len) == 0) {
        return TK_SWITCH;
    }

    if (strncmp(ident, "case", len) == 0) {
        return TK_CASE;
    }

    if (strncmp(ident, "cease", len) == 0) {
        return TK_BREAK;
    }

    if (strncmp(ident, "rehearse", len) == 0) {
        return TK_WHILE;
    }

    if (strncmp(ident, "persist", len) == 0) {
        return TK_CONTINUE;
    }

    return TK_IDENTIFIER;
}
