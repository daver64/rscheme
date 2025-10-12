#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "scheme_objects.h"

// Parser error types
typedef enum {
    PARSE_OK,
    PARSE_ERROR_UNEXPECTED_TOKEN,
    PARSE_ERROR_UNEXPECTED_EOF,
    PARSE_ERROR_INVALID_NUMBER,
    PARSE_ERROR_INVALID_STRING,
    PARSE_ERROR_UNMATCHED_PAREN,
    PARSE_ERROR_TOO_MANY_DOTS
} ParseError;

// Parser state
typedef struct Parser {
    Lexer* lexer;
    ParseError last_error;
    char* error_message;
    size_t error_line;
    size_t error_column;
} Parser;

// Parser creation and destruction
Parser* create_parser(const char* input);
void destroy_parser(Parser* parser);

// Parsing functions
SchemeObject* parse_expression(Parser* parser);
SchemeObject* parse_list(Parser* parser);
SchemeObject* parse_vector(Parser* parser);
SchemeObject* parse_quote(Parser* parser);
SchemeObject* parse_quasiquote(Parser* parser);
SchemeObject* parse_unquote(Parser* parser);
SchemeObject* parse_atom(Parser* parser);

// Number parsing
SchemeObject* parse_number(const char* str);
bool is_valid_number(const char* str);

// String parsing
SchemeObject* parse_string_literal(const char* str);
char* unescape_string(const char* str);

// Error handling
void set_parse_error(Parser* parser, ParseError error, const char* message);
void print_parse_error(Parser* parser, FILE* out);
bool has_parse_error(Parser* parser);

// Utility functions
bool expect_token(Parser* parser, TokenType expected);
bool consume_if_match(Parser* parser, TokenType expected);

#endif // PARSER_H