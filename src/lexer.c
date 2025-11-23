#include "rscheme.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

Lexer* create_lexer(const char* input) {
    Lexer* lexer = (Lexer*)scheme_malloc(sizeof(Lexer));
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(input);
    lexer->has_current = false;
    memset(&lexer->current_token, 0, sizeof(Token));
    return lexer;
}

void destroy_lexer(Lexer* lexer) {
    if (lexer) {
        if (lexer->has_current) {
            free_token(&lexer->current_token);
        }
        scheme_free(lexer);
    }
}

static char current_char(Lexer* lexer) {
    if (lexer->position >= lexer->length) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

static void advance_char(Lexer* lexer) {
    if (lexer->position < lexer->length) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

static void skip_whitespace(Lexer* lexer) {
    while (current_char(lexer) && isspace((unsigned char)current_char(lexer))) {
        advance_char(lexer);
    }
}

static void skip_comment(Lexer* lexer) {
    if (current_char(lexer) == ';') {
        while (current_char(lexer) && current_char(lexer) != '\n') {
            advance_char(lexer);
        }
    }
}

bool is_delimiter(char c) {
    return isspace((unsigned char)c) || c == '(' || c == ')' || c == '[' || c == ']' || 
           c == '"' || c == ';' || c == '\0';
}

bool is_symbol_start(char c) {
    return isalpha((unsigned char)c) || strchr("!$%&*+-./:<=>?@^_~", c) != NULL;
}

bool is_symbol_char(char c) {
    return is_symbol_start(c) || isdigit((unsigned char)c);
}

bool is_digit_char(char c) {
    return isdigit((unsigned char)c);
}

static Token make_token(TokenType type, const char* value, size_t line, size_t column) {
    Token token;
    token.type = type;
    token.value = value ? scheme_strdup(value) : NULL;
    token.line = line;
    token.column = column;
    token.length = value ? strlen(value) : 0;
    return token;
}

static Token read_string(Lexer* lexer) {
    size_t start_line = lexer->line;
    size_t start_column = lexer->column;
    
    advance_char(lexer); // Skip opening quote
    
    char* buffer = (char*)scheme_malloc(1024);
    size_t buffer_size = 1024;
    size_t length = 0;
    
    while (current_char(lexer) && current_char(lexer) != '"') {
        if (length >= buffer_size - 1) {
            buffer_size *= 2;
            buffer = (char*)scheme_realloc(buffer, buffer_size);
        }
        
        if (current_char(lexer) == '\\') {
            advance_char(lexer);
            char next = current_char(lexer);
            switch (next) {
                case 'n': buffer[length++] = '\n'; break;
                case 't': buffer[length++] = '\t'; break;
                case 'r': buffer[length++] = '\r'; break;
                case '\\': buffer[length++] = '\\'; break;
                case '"': buffer[length++] = '"'; break;
                default:
                    buffer[length++] = next;
                    break;
            }
        } else {
            buffer[length++] = current_char(lexer);
        }
        advance_char(lexer);
    }
    
    if (current_char(lexer) == '"') {
        advance_char(lexer); // Skip closing quote
    } else {
        scheme_free(buffer);
        return make_token(TOKEN_ERROR, "Unterminated string", start_line, start_column);
    }
    
    buffer[length] = '\0';
    Token token = make_token(TOKEN_STRING, buffer, start_line, start_column);
    scheme_free(buffer);
    return token;
}

static Token read_number_or_symbol(Lexer* lexer) {
    size_t start_line = lexer->line;
    size_t start_column = lexer->column;
    
    char* buffer = (char*)scheme_malloc(256);
    size_t buffer_size = 256;
    size_t length = 0;
    
    // Special handling for character literals #\c
    if (current_char(lexer) == '#') {
        buffer[length++] = '#';
        advance_char(lexer);
        
        if (current_char(lexer) == '\\') {
            // This is a character literal
            buffer[length++] = '\\';
            advance_char(lexer);
            
            // Read the character name/value
            while (current_char(lexer) && !is_delimiter(current_char(lexer))) {
                if (length >= buffer_size - 1) {
                    buffer_size *= 2;
                    buffer = (char*)scheme_realloc(buffer, buffer_size);
                }
                buffer[length++] = current_char(lexer);
                advance_char(lexer);
            }
            
            buffer[length] = '\0';
            Token token = make_token(TOKEN_SYMBOL, buffer, start_line, start_column);
            scheme_free(buffer);
            return token;
        }
        
        // Not a character literal, continue reading as normal
    }
    
    while (current_char(lexer) && !is_delimiter(current_char(lexer))) {
        if (length >= buffer_size - 1) {
            buffer_size *= 2;
            buffer = (char*)scheme_realloc(buffer, buffer_size);
        }
        buffer[length++] = current_char(lexer);
        advance_char(lexer);
    }
    
    buffer[length] = '\0';
    
    // Check if it's a boolean
    if (strcmp(buffer, "#t") == 0 || strcmp(buffer, "#f") == 0) {
        Token token = make_token(TOKEN_BOOLEAN, buffer, start_line, start_column);
        scheme_free(buffer);
        return token;
    }
    
    // Check if it's a number
    if (is_valid_number(buffer)) {
        Token token = make_token(TOKEN_NUMBER, buffer, start_line, start_column);
        scheme_free(buffer);
        return token;
    }
    
    // It's a symbol
    Token token = make_token(TOKEN_SYMBOL, buffer, start_line, start_column);
    scheme_free(buffer);
    return token;
}

Token next_token(Lexer* lexer) {
    while (true) {
        skip_whitespace(lexer);
        
        if (current_char(lexer) == ';') {
            skip_comment(lexer);
            continue;
        }
        
        size_t line = lexer->line;
        size_t column = lexer->column;
        char c = current_char(lexer);
        
        if (c == '\0') {
            return make_token(TOKEN_EOF, NULL, line, column);
        }
        
        switch (c) {
            case '(':
                advance_char(lexer);
                return make_token(TOKEN_LPAREN, "(", line, column);
            case ')':
                advance_char(lexer);
                return make_token(TOKEN_RPAREN, ")", line, column);
            case '[':
                advance_char(lexer);
                return make_token(TOKEN_LBRACKET, "[", line, column);
            case ']':
                advance_char(lexer);
                return make_token(TOKEN_RBRACKET, "]", line, column);
            case '.':
                advance_char(lexer);
                return make_token(TOKEN_DOT, ".", line, column);
            case '\'':
                advance_char(lexer);
                return make_token(TOKEN_QUOTE, "'", line, column);
            case '`':
                advance_char(lexer);
                return make_token(TOKEN_QUASIQUOTE, "`", line, column);
            case ',':
                advance_char(lexer);
                if (current_char(lexer) == '@') {
                    advance_char(lexer);
                    return make_token(TOKEN_UNQUOTE_SPLICING, ",@", line, column);
                }
                return make_token(TOKEN_UNQUOTE, ",", line, column);
            case '"':
                return read_string(lexer);
            default:
                if (is_symbol_start(c) || is_digit_char(c) || c == '+' || c == '-' || c == '#') {
                    return read_number_or_symbol(lexer);
                } else {
                    advance_char(lexer);
                    char error_msg[32];
                    snprintf(error_msg, sizeof(error_msg), "Unexpected character: %c", c);
                    return make_token(TOKEN_ERROR, error_msg, line, column);
                }
        }
    }
}

Token peek_token(Lexer* lexer) {
    if (!lexer->has_current) {
        lexer->current_token = next_token(lexer);
        lexer->has_current = true;
    }
    
    return lexer->current_token;
}

void consume_token(Lexer* lexer) {
    if (lexer->has_current) {
        free_token(&lexer->current_token);
        lexer->has_current = false;
    }
}

void print_token(const Token* token, FILE* out) {
    const char* type_names[] = {
        "EOF", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET", "DOT",
        "QUOTE", "QUASIQUOTE", "UNQUOTE", "UNQUOTE_SPLICING",
        "NUMBER", "STRING", "SYMBOL", "BOOLEAN", "ERROR"
    };
    
    fprintf(out, "Token{type=%s, value='%s', line=%zu, column=%zu}",
            type_names[token->type],
            token->value ? token->value : "",
            token->line,
            token->column);
}

char* token_to_string(const Token* token) {
    char* buffer = (char*)scheme_malloc(256);
    snprintf(buffer, 256, "Token{type=%d, value='%s', line=%zu, column=%zu}",
             token->type,
             token->value ? token->value : "",
             token->line,
             token->column);
    return buffer;
}

void free_token(Token* token) {
    if (token && token->value) {
        scheme_free(token->value);
        token->value = NULL;
    }
}

bool is_valid_number(const char* str) {
    if (!str || *str == '\0') {
        return false;
    }
    
    char* endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}