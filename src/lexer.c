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
    case '\0':
        return tokenCreate(TK_EOF, lexerGetLexAsString(lexer));
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

    case '\"': // detect string literals
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

// skip whitespaces, unneeded file escape sequences, and comments
static void lexerSkipWhitespace(Lexer *lexer) {
    while (lexer->ch == ' ' || lexer->ch == '\t' || lexer->ch == '\n' ||
           lexer->ch == '\r' || lexer->ch == '#') {

        // track current line number
        if (lexer->ch == '\n') {
            lexer->line_number++;
            lexer->curr_line_start = lexer->read_index;
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

  switch (ident[0]) {

    // CASE - CEASE - COUNT
    case 'c':
      switch (ident[1]) {
        case 'a':
          switch (ident[2]) {
            case 's':
              switch (ident[3]) {
                case 'e':
                  return TK_CASE;
              }
          }
          break;

        case 'e':
          switch (ident[2]) {
            case 'a':
              switch (ident[3]) {
                case 's':
                  switch (ident[4]) {
                    case 'e':
                      return TK_BREAK;
                  }
              }
          }
          break;

        case 'o':
          switch (ident[2]) {
            case 'u':
              switch (ident[3]) {
                case 'n':
                  switch (ident[4]) {
                    case 't':
                      return TK_INT;
                  }
              }
          }
          break;

      }
      break;

    // DEFINE
    case 'd':
      switch (ident[1]) {
        case 'e':
          switch (ident[2]) {
            case 'f':
              switch (ident[3]) {
                case 'i':
                  switch (ident[4]) {
                    case 'n':
                      switch (ident[5]) {
                        case 'e':
                          return TK_FUNCTION;
                      }
                  }
              }
          }
      }
      break;

    // ELSE
    case 'e':
      switch (ident[1]) {
        case 'l':
          switch (ident[2]) {
            case 's':
              switch (ident[3]) {
                case 'e':
                  return TK_ELSE;
              }
          }
      }
      break;

    // FRACTION
    case 'f':
      switch (ident[1]) {
        case 'r':
          switch (ident[2]) {
            case 'a':
              switch (ident[3]) {
                case 'c':
                  switch (ident[4]) {
                    case 't':
                      switch (ident[5]) {
                        case 'i':
                          switch (ident[6]) {
                            case 'o':
                              switch (ident[7]) {
                                case 'n':
                                  return TK_DOUBLE;
                              }
                          }
                      }
                  }
              }
          }
      }
      break;

    // GLYPH
    case 'g':
      switch (ident[1]) {
        case 'l':
          switch (ident[2]) {
            case 'y':
              switch (ident[3]) {
                case 'p':
                  switch (ident[4]) {
                    case 'h':
                      return TK_CHAR;
                  }
              }
          }
      }
      break;

    // HEARETH
    case 'h':
      switch (ident[1]) {
        case 'e':
          switch (ident[2]) {
            case 'a':
              switch (ident[3]) {
                case 'r':
                  switch (ident[4]) {
                    case 'e':
                      switch (ident[5]) {
                        case 't':
                          switch (ident[6]) {
                            case 'h':
                              return TK_IN;
                          }
                      }
                  }
              }
          }
      }
      break;

    // IF
    case 'i':
      switch (ident[1]) {
        case 'f':
          return TK_IF;
      }
      break;

    // MAKETH
    case 'm':
      switch (ident[1]) {
        case 'a':
          switch (ident[2]) {
            case 'k':
              switch (ident[3]) {
                case 'e':
                  switch (ident[4]) {
                    case 't':
                      switch (ident[5]) {
                        case 'h':
                          return TK_LET;
                      }
                  }
              }
          }
      }
      break;

    // NAY - NOUGHT
    case 'n':
      switch (ident[1]) {
        case 'a':
          switch (ident[2]) {
            case 'y':
              return TK_FALSE;
          }
          break;
        case 'o':
          switch (ident[2]) {
            case 'u':
              switch (ident[3]) {
                case 'g':
                  switch (ident[4]) {
                    case 'h':
                      switch (ident[5]) {
                        case 't':
                          return TK_VOID;
                      }
                  }
              }
          }
      }
      break;

    // PERSIST - PORTION
    case 'p':
      switch (ident[1]) {
        case 'e':
          switch (ident[2]) {
            case 'r':
              switch (ident[3]) {
                case 's':
                  switch (ident[4]) {
                    case 'i':
                      switch (ident[5]) {
                        case 's':
                          switch (ident[6]) {
                            case 't':
                              return TK_CONTINUE;
                          }
                      }
                  }
              }
          }
          break;

        case 'o':
          switch (ident[2]) {
            case 'r':
              switch (ident[3]) {
                case 't':
                  switch (ident[4]) {
                    case 'i':
                      switch (ident[5]) {
                        case 'o':
                          switch (ident[6]) {
                            case 'n':
                              return TK_FLOAT;
                          }
                      }
                  }
              }
          }
          break;

      }
      break;

    // REHEARSE - RETURNETH
    case 'r':
      switch (ident[1]) {
        case 'e':
          switch (ident[2]) {
            case 'h':
              switch (ident[3]) {
                case 'e':
                  switch (ident[4]){
                    case 'a':
                      switch (ident[5]){
                        case 'r':
                          switch (ident[6]){
                            case 's':
                              switch (ident[7]){
                                case 'e':
                                  return TK_WHILE;
                              }
                          }
                      }
                  }
              }
              break;

            case 't':
              switch (ident[3]) {
                case 'u':
                  switch (ident[4]) {
                    case 'r':
                      switch (ident[5]) {
                        case 'n':
                          switch (ident[6]) {
                            case 'e':
                              switch (ident[7]) {
                                case 't':
                                  switch (ident[8]) {
                                    case 'h':
                                      return TK_RETURN;
                                  }
                              }
                          }
                      }
                  }
              }
              break;

          }
      }
      break;

    // SAYETH - SWITCH
    case 's':
      switch (ident[1]) {
        case 'a':
          switch (ident[2]) {
            case 'y':
              switch (ident[3]) {
                case 'e':
                  switch (ident[4]) {
                    case 't':
                      switch (ident[5]) {
                        case 'h':
                          return TK_OUT;
                      }
                  }
              }
          }
          break;

        case 'w':
          switch (ident[2]) {
            case 'i':
              switch (ident[3]) {
                case 't':
                  switch (ident[4]) {
                    case 'c':
                      switch (ident[5]) {
                        case 'h':
                          return TK_SWITCH;
                      }
                  }
              }
          }
      }
      break;

    // THITHER
    case 't':
      switch (ident[1]) {
        case 'h':
          switch (ident[2]) {
            case 'i':
              switch (ident[3]) {
                case 't':
                  switch (ident[4]) {
                    case 'h':
                      switch (ident[5]) {
                        case 'e':
                          switch (ident[6]) {
                            case 'r':
                              return TK_GOTO;
                          }
                      }
                  }
              }
          }
      }
      break;

    // VERDICT
    case 'v':
      switch (ident[1]) {
        case 'e':
          switch (ident[2]) {
            case 'r':
              switch (ident[3]) {
                case 'd':
                  switch (ident[4]) {
                    case 'i':
                      switch (ident[5]) {
                        case 'c':
                          switch (ident[6]) {
                            case 't':
                              return TK_BOOL;
                          }
                      }
                  }
              }
          }
      }
      break;

    // YAY
    case 'y':
      switch (ident[1]) {
        case 'a':
          switch (ident[2]) {
            case 'y':
              return TK_TRUE;
          }
      }
      break;

  }

  return TK_IDENTIFIER;
}
