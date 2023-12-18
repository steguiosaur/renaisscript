#include "lexer.h"

#include <stdlib.h>
#include <string.h>

static Token *tokenCreate(TokenType type, char *lexeme);

static void lexerSkipWhitespace(Lexer *lexer);
static void lexerReadNextChar(Lexer *lexer);
static char lexerPeekNextChar(Lexer *lexer);
static char *lexerGetLexAsString(Lexer *lexer, unsigned long curr_idx);

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
    lexer->line_number = 1;

    return lexer;
}

Token *lexerGetNextToken(Lexer *lexer) {
    lexerReadNextChar(lexer);
    lexerSkipWhitespace(lexer);

    const unsigned long curr_i = lexer->index; // track current index

    switch (lexer->ch) {
    case '{':
        return tokenCreate(TK_LCURLY, lexerGetLexAsString(lexer, curr_i));
    case '}':
        return tokenCreate(TK_RCURLY, lexerGetLexAsString(lexer, curr_i));
    case '(':
        return tokenCreate(TK_LPAREN, lexerGetLexAsString(lexer, curr_i));
    case ')':
        return tokenCreate(TK_RPAREN, lexerGetLexAsString(lexer, curr_i));
    case ',':
        return tokenCreate(TK_COMMA, lexerGetLexAsString(lexer, curr_i));
    case ';':
        return tokenCreate(TK_SEMICOLON, lexerGetLexAsString(lexer, curr_i));
    case ':':
        return tokenCreate(TK_COLON, lexerGetLexAsString(lexer, curr_i));
    case '+':
        return tokenCreate(TK_PLUS, lexerGetLexAsString(lexer, curr_i));
    case '-':
        return tokenCreate(TK_MINUS, lexerGetLexAsString(lexer, curr_i));
    case '=':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_EQUAL, lexerGetLexAsString(lexer, curr_i));
        }
        return tokenCreate(TK_ASSIGN, lexerGetLexAsString(lexer, curr_i));
    case '!':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_NOTEQUAL, lexerGetLexAsString(lexer, curr_i));
        }
        return tokenCreate(TK_BANG, lexerGetLexAsString(lexer, curr_i));
    case '/':
        return tokenCreate(TK_SLASH, lexerGetLexAsString(lexer, curr_i));
    case '*':
        if (lexerPeekNextChar(lexer) == '*') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_EXPONENT, lexerGetLexAsString(lexer, curr_i));
        }
        return tokenCreate(TK_ASTERISK, lexerGetLexAsString(lexer, curr_i));
    case '>':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_GEQUAL, lexerGetLexAsString(lexer, curr_i));
        }
        return tokenCreate(TK_GT, lexerGetLexAsString(lexer, curr_i));
    case '<':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_LEQUAL, lexerGetLexAsString(lexer, curr_i));
        }
        return tokenCreate(TK_LT, lexerGetLexAsString(lexer, curr_i));
    case '&':
        if (lexerPeekNextChar(lexer) == '&') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_AND, lexerGetLexAsString(lexer, curr_i));
        }
        return tokenCreate(TK_AMPERSAND, lexerGetLexAsString(lexer, curr_i));
    case '|':
        if (lexerPeekNextChar(lexer) == '|') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_OR, lexerGetLexAsString(lexer, curr_i));
        }
        break;
    case '\0':
        return tokenCreate(TK_EOF, lexerGetLexAsString(lexer, curr_i));
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
            return tokenCreate(TK_STRING, lexerGetLexAsString(lexer, curr_i));
        }

        return tokenCreate(TK_ERR, lexerGetLexAsString(lexer, curr_i));
    }

    if (isValidIdentifier(lexer->ch)) {
        while (isValidIdentifier(lexerPeekNextChar(lexer))) {
            lexerReadNextChar(lexer);
        }

        const char *value = lexer->contents + curr_i;
        unsigned long len = lexer->index - curr_i + 1;
        char *lexeme = strndup(value, len);

        TokenType type = lexerIdReservedKeyword(value, len);
        return tokenCreate(type, lexeme);
    }

    if (isValidNumber(lexer->ch)) {
        while (isValidNumber(lexerPeekNextChar(lexer))) {
            lexerReadNextChar(lexer);
        }

        return tokenCreate(TK_INTLIT, lexerGetLexAsString(lexer, curr_i));
    }

    return tokenCreate(TK_ILLEGAL, lexerGetLexAsString(lexer, curr_i));
}

int lexerErrorHandler(Lexer *lexer, Token *token, const char *filename) {
    if (token->type == TK_ILLEGAL) {
        printf("[ERROR]: InvalidToken: %s not recognized as token on %s "
               "line %lu\n",
               token->lexeme, filename, lexer->line_number);
        return 1;
    }

    if (token->type == TK_ERR) {
        printf("[ERROR]: UnterminatedString: unterminated string literal "
               "reached EOF on %s line %lu\n",
               filename, lexer->line_number);
        return 1;
    }
    
    return 0;
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

// comments are also skipped like whitespace
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

// access next value char in lexer
static void lexerReadNextChar(Lexer *lexer) {
    if (lexer->read_index >= lexer->content_length) {
        lexer->ch = '\0';
    } else {
        lexer->ch = lexer->contents[lexer->read_index];
    }

    lexer->index = lexer->read_index;
    lexer->read_index++;
}

// view next char value in lexer
static char lexerPeekNextChar(Lexer *lexer) {
    if (lexer->read_index >= lexer->content_length) {
        return '\0';
    }
    return lexer->contents[lexer->read_index];
}

// return ptr of tracked lexeme start and end index in lexer->contents
static char *lexerGetLexAsString(Lexer *lexer, const unsigned long curr_idx) {
    const char *value = lexer->contents + curr_idx;
    unsigned long len = lexer->index - curr_idx + 1;
    char *lexeme = strndup(value, len);
    return lexeme;
}

static int isValidIdentifier(const char chr) {
    return 'a' <= chr && chr <= 'z' || 'A' <= chr && chr <= 'Z' || chr == '_';
}

static int isValidNumber(const char chr) { return '0' <= chr && '9' >= chr; }

// detect if given identifier is a reserved keyword
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
