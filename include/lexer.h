#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdbool.h>

// Token types
typedef enum {
    TOKEN_EOF,
    TOKEN_LPAREN,     // (
    TOKEN_RPAREN,     // )
    TOKEN_LBRACKET,   // [
    TOKEN_RBRACKET,   // ]
    TOKEN_DOT,        // .
    TOKEN_QUOTE,      // '
    TOKEN_QUASIQUOTE, // `
    TOKEN_UNQUOTE,    // ,
    TOKEN_UNQUOTE_SPLICING, // ,@
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_SYMBOL,
    TOKEN_BOOLEAN,    // #t or #f
    TOKEN_ERROR
} TokenType;

// Token structure
typedef struct Token {
    TokenType type;
    char* value;
    size_t line;
    size_t column;
    size_t length;
} Token;

// Lexer state
typedef struct Lexer {
    const char* input;
    size_t position;
    size_t line;
    size_t column;
    size_t length;
    Token current_token;
    bool has_current;
} Lexer;

// Lexer functions
Lexer* create_lexer(const char* input);
void destroy_lexer(Lexer* lexer);

// Tokenization
Token next_token(Lexer* lexer);
Token peek_token(Lexer* lexer);
void consume_token(Lexer* lexer);

// Token utilities
void print_token(const Token* token, FILE* out);
char* token_to_string(const Token* token);
void free_token(Token* token);

// Character classification
bool is_delimiter(char c);
bool is_symbol_start(char c);
bool is_symbol_char(char c);
bool is_digit_char(char c);

#endif // LEXER_H