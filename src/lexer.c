#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

struct LexerStruct {
    const char *input;
    size_t input_length;
    size_t position;
    size_t read_position;
    char ch;
};

static void readLexerNextChar(Lexer *lexer);
static char peekLexerNextChar(Lexer *lexer);
static const char *readIdentifierLexer(Lexer *lexer, size_t *len);
static const char *readIntegerLexer(Lexer *lexer, size_t *len);

static uint8_t isLetter(char chr);
static uint8_t isNumber(char chr);

static TokenType getTokenTypeFromLiteral(const char *literal, size_t len);

Lexer *createLexer(const char *input) {
    size_t len = sizeof(Lexer);
    Lexer *lexer = malloc(len);
    memset(lexer, 0, len);

    lexer->input = input;
    lexer->input_length = strlen(input);
    lexer->position = 0;
    lexer->read_position = 0;

    readLexerNextChar(lexer);

    return lexer;
}

Token *analyzeLexerNext(Lexer *lexer) {
    Token *tok = NULL;

    // skip whitespace (space, tab, newline, carriage return)
    while (lexer->ch == ' ' || lexer->ch == '\t' || lexer->ch == '\n' ||
           lexer->ch == '\r') {
        readLexerNextChar(lexer);
    }

    switch (lexer->ch) {
    case '{':
        tok = createToken(TK_LCURLY, NULL);
        break;
    case '}':
        tok = createToken(TK_RCURLY, NULL);
        break;
    case '(':
        tok = createToken(TK_LPAREN, NULL);
        break;
    case ')':
        tok = createToken(TK_RPAREN, NULL);
        break;
    case ',':
        tok = createToken(TK_COMMA, NULL);
        break;
    case ';':
        tok = createToken(TK_SEMICOLON, NULL);
        break;
    case '+':
        tok = createToken(TK_PLUS, NULL);
        break;
    case '-':
        tok = createToken(TK_MINUS, NULL);
        break;
    case '=':
        if (peekLexerNextChar(lexer) == '=') {
            readLexerNextChar(lexer);
            tok = createToken(TK_EQUAL, NULL);
        } else {
            tok = createToken(TK_ASSIGN, NULL);
        }
        break;
    case '!':
        if (peekLexerNextChar(lexer) == '=') {
            readLexerNextChar(lexer);
            tok = createToken(TK_NOTEQUAL, NULL);
        } else {
            tok = createToken(TK_BANG, NULL);
        }
        break;
    case '/':
        tok = createToken(TK_SLASH, NULL);
        break;
    case '*':
        tok = createToken(TK_ASTERISK, NULL);
        break;
    case '>':
        tok = createToken(TK_GT, NULL);
        break;
    case '<':
        tok = createToken(TK_LT, NULL);
        break;
    case '\0':
        tok = createToken(TK_EOF, NULL);
        break;
    default:
        break;
    }

    if (isLetter(lexer->ch)) {
        size_t len = 0;
        char *literal = NULL;
        const char *ident = readIdentifierLexer(lexer, &len);

        TokenType type = getTokenTypeFromLiteral(ident, len);
        if (type == TK_IDENTIFIER) {
            literal = strndup(ident, len);
        }

        tok = createToken(type, literal);
        return tok;
    }

    if (isNumber(lexer->ch)) {
        size_t len = 0;
        char *literal = NULL;
        const char *ident = readIntegerLexer(lexer, &len);

        literal = strndup(ident, len);
        tok = createToken(TK_INTEGER, literal);
        return tok;
    }

    if (!tok) {
        tok = createToken(TK_ILLEGAL, NULL);
    }

    readLexerNextChar(lexer);

    return tok;
}

void cleanupLexer(Lexer **lexer) {
    if (*lexer) {
        free(*lexer);
    }

    *lexer = NULL;
}

Token *createToken(TokenType type, char *literal) {
    size_t len = sizeof(Token);
    Token *token = malloc(len);
    memset(token, 0, len);

    token->literal = literal;
    token->type = type;

    return token;
}

void cleanupToken(Token **token) {
    if (*token && (*token)->literal) {
        free((*token)->literal);
    }

    if (*token) {
        free(*token);
    }

    *token = NULL;
}

static void readLexerNextChar(Lexer *lexer) {
    if (lexer->read_position >= lexer->input_length) {
        lexer->ch = '\0';
    } else {
        lexer->ch = lexer->input[lexer->read_position];
    }

    lexer->position = lexer->read_position;
    lexer->read_position++;
}

static char peekLexerNextChar(Lexer *lexer) {
    if (lexer->read_position >= lexer->input_length) {
        return '\0';
    }
    return lexer->input[lexer->read_position];
}

static const char *readIdentifierLexer(Lexer *lexer, size_t *len) {
    char *result = NULL;
    size_t position = lexer->position;

    while (isLetter(lexer->ch)) {
        readLexerNextChar(lexer);
    }

    if (len) {
        *len = lexer->position - position;
    }

    return lexer->input + position;
}

static const char *readIntegerLexer(Lexer *lexer, size_t *len) {
    char *result = NULL;
    size_t position = lexer->position;

    while (isNumber(lexer->ch)) {
        readLexerNextChar(lexer);
    }

    if (len) {
        *len = lexer->position - position;
    }

    return lexer->input + position;
}

static uint8_t isLetter(char chr) {
    return 'a' <= chr && chr <= 'z' || 'A' <= chr && chr <= 'Z' || chr == '_';
}

static uint8_t isNumber(char chr) { return '0' <= chr && '9' >= chr; }

static TokenType getTokenTypeFromLiteral(const char *literal, size_t len) {
    if (strncmp(literal, "let", len) == 0) {
        return TK_LET;
    }

    if (strncmp(literal, "fn", len) == 0) {
        return TK_FUNCTION;
    }

    if (strncmp(literal, "true", len) == 0) {
        return TK_TRUE;
    }

    if (strncmp(literal, "false", len) == 0) {
        return TK_FALSE;
    }

    if (strncmp(literal, "if", len) == 0) {
        return TK_IF;
    }

    if (strncmp(literal, "else", len) == 0) {
        return TK_ELSE;
    }

    if (strncmp(literal, "return", len) == 0) {
        return TK_RETURN;
    }

    return TK_IDENTIFIER;
}
