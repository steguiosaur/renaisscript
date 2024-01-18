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
    lexer->curr_line_start = 0;

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
    case '.':
        return tokenCreate(TK_DOT, lexerGetLexAsString(lexer));
    case ';':
        return tokenCreate(TK_SEMICOLON, lexerGetLexAsString(lexer));
    case ':':
        return tokenCreate(TK_COLON, lexerGetLexAsString(lexer));
    case '%':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_ASSIGNMOD, lexerGetLexAsString(lexer));
        }
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
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_ASSIGNDIV, lexerGetLexAsString(lexer));
        }
        if (lexerPeekNextChar(lexer) == '/') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_FLOORDIV, lexerGetLexAsString(lexer));
        }
        return tokenCreate(TK_SLASH, lexerGetLexAsString(lexer));
    case '*':
        if (lexerPeekNextChar(lexer) == '=') {
            lexerReadNextChar(lexer);
            return tokenCreate(TK_ASSIGNMUL, lexerGetLexAsString(lexer));
        }
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
    case '#':
        // block line comment
        if (lexerPeekNextChar(lexer) == '#') {
            lexerReadNextChar(lexer);
            while (lexerPeekNextChar(lexer) != '#') {
                lexerReadNextChar(lexer);

                if (lexerPeekNextChar(lexer) == '#') {
                    lexerReadNextChar(lexer);
                }
            }
            lexerReadNextChar(lexer);

            return tokenCreate(TK_BLCKCOMMENT, lexerGetLexAsString(lexer));
        }
        // in-line comment
        while (lexerPeekNextChar(lexer) != '\n') {
            lexerReadNextChar(lexer);
        }
        return tokenCreate(TK_LINECOMMENT, lexerGetLexAsString(lexer));
    case '\'': // detect character literals
        lexerReadNextChar(lexer);

        // error if character empty character constant
        if (lexer->ch == '\'') {
            return tokenCreate(TK_EMPTYCHERR, lexerGetLexAsString(lexer));
        }

        if (lexer->ch == '\\' && (lexerPeekNextChar(lexer) == '\\' ||
                                  lexerPeekNextChar(lexer) == '\'' ||
                                  lexerPeekNextChar(lexer) == '0' ||
                                  lexerPeekNextChar(lexer) == 'n' ||
                                  lexerPeekNextChar(lexer) == 't' ||
                                  lexerPeekNextChar(lexer) == 'r')) {
            lexerReadNextChar(lexer);
        }

        lexerReadNextChar(lexer);
        if (lexer->ch == '\'') {
            return tokenCreate(TK_CHARACLIT, lexerGetLexAsString(lexer));
        }

        if (lexer->ch != '\'') {
            lexerReadNextChar(lexer);
        }
        return tokenCreate(TK_MULTICHERR, lexerGetLexAsString(lexer));
    case '"': // detext string literal
        lexerReadNextChar(lexer);
        while (lexer->ch != '"' && lexerPeekNextChar(lexer) != '\0') {
            if (lexer->ch == '\\' && lexerPeekNextChar(lexer) == '"') {
                lexerReadNextChar(lexer);
            }
            lexerReadNextChar(lexer);
        }

        if (lexerPeekNextChar(lexer) != '\0') {
            return tokenCreate(TK_STRINGLIT, lexerGetLexAsString(lexer));
        }

        return tokenCreate(TK_STREOFERR, lexerGetLexAsString(lexer));
    case '\0':
        return tokenCreate(TK_EOF, lexerGetLexAsString(lexer));
    default:
        break;
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
        unsigned short dot_count = 0;
        while (isValidNumber(lexerPeekNextChar(lexer)) ||
               lexerPeekNextChar(lexer) == '.') {
            lexerReadNextChar(lexer);
            if (lexer->ch == '.') {
                dot_count++;
            }
        }

        if (dot_count == 0) {
            return tokenCreate(TK_INTLIT, lexerGetLexAsString(lexer));
        }

        if (dot_count == 1) {
            return tokenCreate(TK_FLTLIT, lexerGetLexAsString(lexer));
        }

        return tokenCreate(TK_FLOATERR, lexerGetLexAsString(lexer));
    }

    return tokenCreate(TK_ILLEGALCHR, lexerGetLexAsString(lexer));
}

// pass tokens here to filter error type tokens
int lexerErrorHandler(Lexer *lexer, Token *token, const char *filename) {
    if (!(token->type == TK_ILLEGALCHR || token->type == TK_EMPTYCHERR ||
          token->type == TK_MULTICHERR || token->type == TK_FLOATERR ||
          token->type == TK_STREOFERR)) {
        return 0;
    }
    unsigned long column = lexer->index - lexer->curr_line_start + 1;
    const char *line_start = lexer->contents + lexer->curr_line_start;
    unsigned long line_end = lexer->curr_line_start;
    while (lexer->contents[line_end] != '\n') {
        line_end++;
    }
    char *curr_line = strndup(line_start, line_end - lexer->curr_line_start);

    switch (token->type) {
    case TK_ILLEGALCHR: // illegal character error
        printf("ERROR: %s (line %lu) (column %lu): '%s' not recognized as "
               "token or symbol [ILLEGAL_CHARACTER_ERROR] \n",
               filename, lexer->line_number, column, token->lexeme);
        printf(" %5lu | %s\n", lexer->line_number, curr_line);
        printf("       | ");
        for (int i = 0; i < column - 1; i++) {
            putchar(' ');
        }
        printf("^\n");
        return 1;
    case TK_EMPTYCHERR: // empty character literal error
        printf("ERROR: %s (line %lu) (column %lu): missing character literal "
               "'' value [EMPTY_CHARACTER_ERROR] \n",
               filename, lexer->line_number, column);
        printf(" %5lu | %s\n", lexer->line_number, curr_line);
        printf("       | ");
        for (int i = 0; i < column - 1; i++) {
            putchar(' ');
        }
        printf("^^\n");
        return 1;
    case TK_MULTICHERR: // multi character error
        printf("ERROR: %s (line %lu) (column %lu): multiple value assigned on "
               "character literal '%s' [MULTIPLE_CHARACTER_ERROR]\n",
               filename, lexer->line_number, column, token->lexeme);
        printf(" %5lu | %s\n", lexer->line_number, curr_line);
        printf("       | ");
        for (int i = 0; i < column - 1; i++) {
            putchar(' ');
        }
        for (int i = 0; i < strlen(token->lexeme) + 2; i++) {
            putchar('^');
        }
        printf("\n");
        return 1;
    case TK_FLOATERR: // invalid suffix on float literal
        printf("ERROR: %s (line %lu) (column %lu): multiple decimal point "
               "occurrences detected on %s [FLOAT_SUFFIX_ERROR]\n",
               filename, lexer->line_number, column, token->lexeme);
        printf(" %5lu | %s\n", lexer->line_number, curr_line);
        printf("       | ");
        for (int i = 0; i < column - 1; i++) {
            putchar(' ');
        }
        int excess_dot = 0;
        for (int i = 0; i < strlen(token->lexeme); i++) {
            if (token->lexeme[i] == '.') {
                excess_dot++;
            }
            if (excess_dot < 2) {
                putchar(' ');
            } else {
                putchar('^');
            }
        }
        printf("\n");
        return 1;
    case TK_STREOFERR: // unterminated string literal error
        printf("ERROR: %s (line %lu) (column %lu): unterminated string literal "
               "reached EOF [UNTERMINATED_STRING_ERROR]\n",
               filename, lexer->line_number, column);
        printf(" %5lu | %s\n", lexer->line_number, curr_line);
        printf("       | ");
        for (int i = 0; i < column - 1; i++) {
            putchar(' ');
        }
        printf("^ ~~ expected another '\"' double quote\n");
        return 1;
    default:
        return 0;
    }
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

// skip whitespaces and unneeded file escape sequences
static void lexerSkipWhitespace(Lexer *lexer) {
    while (lexer->ch == ' ' || lexer->ch == '\t' || lexer->ch == '\n' ||
           lexer->ch == '\r') {
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

    // track current line number
    if (lexer->ch == '\n') {
        lexer->line_number++;
        lexer->curr_line_start = lexer->read_index;
    }
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

    // discard quotes and double quotes in character and string literals
    if (lexer->ch == '\'' || lexer->ch == '"') {
        value++;
        len = len - 2;
    }

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
