#include "rscheme.h"

Parser* create_parser(const char* input) {
    Parser* parser = (Parser*)scheme_malloc(sizeof(Parser));
    parser->lexer = create_lexer(input);
    parser->last_error = PARSE_OK;
    parser->error_message = NULL;
    parser->error_line = 0;
    parser->error_column = 0;
    return parser;
}

void destroy_parser(Parser* parser) {
    if (parser) {
        if (parser->lexer) {
            destroy_lexer(parser->lexer);
        }
        if (parser->error_message) {
            scheme_free(parser->error_message);
        }
        scheme_free(parser);
    }
}

void set_parse_error(Parser* parser, ParseError error, const char* message) {
    if (!parser) return;
    
    parser->last_error = error;
    
    if (parser->error_message) {
        scheme_free(parser->error_message);
    }
    parser->error_message = scheme_strdup(message);
    
    Token current = peek_token(parser->lexer);
    parser->error_line = current.line;
    parser->error_column = current.column;
}

void print_parse_error(Parser* parser, FILE* out) {
    if (!parser || parser->last_error == PARSE_OK) {
        return;
    }
    
    fprintf(out, "Parse Error at line %zu, column %zu: %s\n",
            parser->error_line,
            parser->error_column,
            parser->error_message ? parser->error_message : "Unknown error");
}

bool has_parse_error(Parser* parser) {
    return parser && parser->last_error != PARSE_OK;
}

bool expect_token(Parser* parser, TokenType expected) {
    Token token = peek_token(parser->lexer);
    if (token.type != expected) {
        char error_msg[128];
        snprintf(error_msg, sizeof(error_msg), "Expected token type %d, got %d", expected, token.type);
        set_parse_error(parser, PARSE_ERROR_UNEXPECTED_TOKEN, error_msg);
        return false;
    }
    return true;
}

bool consume_if_match(Parser* parser, TokenType expected) {
    Token token = peek_token(parser->lexer);
    if (token.type == expected) {
        consume_token(parser->lexer);
        return true;
    }
    return false;
}

SchemeObject* parse_expression(Parser* parser) {
    if (has_parse_error(parser)) {
        return NULL;
    }
    
    Token token = peek_token(parser->lexer);
    
    switch (token.type) {
        case TOKEN_EOF:
            return NULL;
            
        case TOKEN_LPAREN:
            return parse_list(parser);
            
        case TOKEN_LBRACKET:
            return parse_vector(parser);
            
        case TOKEN_QUOTE:
            return parse_quote(parser);
            
        case TOKEN_QUASIQUOTE:
            return parse_quasiquote(parser);
            
        case TOKEN_UNQUOTE:
        case TOKEN_UNQUOTE_SPLICING:
            return parse_unquote(parser);
            
        case TOKEN_NUMBER:
        case TOKEN_STRING:
        case TOKEN_SYMBOL:
        case TOKEN_BOOLEAN:
            return parse_atom(parser);
            
        default:
            set_parse_error(parser, PARSE_ERROR_UNEXPECTED_TOKEN, "Unexpected token");
            return NULL;
    }
}

SchemeObject* parse_list(Parser* parser) {
    if (!expect_token(parser, TOKEN_LPAREN)) {
        return NULL;
    }
    
    consume_token(parser->lexer); // consume '('
    
    // Empty list
    if (consume_if_match(parser, TOKEN_RPAREN)) {
        return make_nil();
    }
    
    SchemeObject* first = parse_expression(parser);
    if (has_parse_error(parser)) {
        return NULL;
    }
    
    SchemeObject* result = cons(first, make_nil());
    SchemeObject* current = result;
    
    while (true) {
        Token token = peek_token(parser->lexer);
        
        if (token.type == TOKEN_RPAREN) {
            consume_token(parser->lexer);
            break;
        }
        
        if (token.type == TOKEN_EOF) {
            set_parse_error(parser, PARSE_ERROR_UNEXPECTED_EOF, "Unexpected end of input in list");
            return NULL;
        }
        
        if (token.type == TOKEN_DOT) {
            consume_token(parser->lexer); // consume '.'
            
            SchemeObject* tail = parse_expression(parser);
            if (has_parse_error(parser)) {
                return NULL;
            }
            
            set_cdr(current, tail);
            
            if (!expect_token(parser, TOKEN_RPAREN)) {
                return NULL;
            }
            consume_token(parser->lexer);
            break;
        }
        
        SchemeObject* next = parse_expression(parser);
        if (has_parse_error(parser)) {
            return NULL;
        }
        
        SchemeObject* new_pair = cons(next, make_nil());
        set_cdr(current, new_pair);
        current = new_pair;
    }
    
    return result;
}

SchemeObject* parse_vector(Parser* parser) {
    if (!expect_token(parser, TOKEN_LBRACKET)) {
        return NULL;
    }
    
    consume_token(parser->lexer); // consume '['
    
    // Count elements first
    size_t count = 0;
    Lexer* saved_lexer = create_lexer(parser->lexer->input + parser->lexer->position);
    
    while (true) {
        Token token = peek_token(saved_lexer);
        if (token.type == TOKEN_RBRACKET || token.type == TOKEN_EOF) {
            break;
        }
        count++;
        // Simple skip - this is not perfect but works for basic cases
        consume_token(saved_lexer);
    }
    
    destroy_lexer(saved_lexer);
    
    SchemeObject* vector = make_vector(count);
    
    for (size_t i = 0; i < count; i++) {
        SchemeObject* element = parse_expression(parser);
        if (has_parse_error(parser)) {
            return NULL;
        }
        vector->value.vector.elements[i] = element;
        if (element) {
            retain_object(element);
        }
    }
    
    if (!expect_token(parser, TOKEN_RBRACKET)) {
        return NULL;
    }
    consume_token(parser->lexer);
    
    return vector;
}

SchemeObject* parse_quote(Parser* parser) {
    if (!expect_token(parser, TOKEN_QUOTE)) {
        return NULL;
    }
    
    consume_token(parser->lexer); // consume '
    
    SchemeObject* quoted = parse_expression(parser);
    if (has_parse_error(parser)) {
        return NULL;
    }
    
    SchemeObject* quote_symbol = make_symbol("quote");
    return cons(quote_symbol, cons(quoted, make_nil()));
}

SchemeObject* parse_quasiquote(Parser* parser) {
    if (!expect_token(parser, TOKEN_QUASIQUOTE)) {
        return NULL;
    }
    
    consume_token(parser->lexer); // consume `
    
    SchemeObject* quoted = parse_expression(parser);
    if (has_parse_error(parser)) {
        return NULL;
    }
    
    SchemeObject* quasiquote_symbol = make_symbol("quasiquote");
    return cons(quasiquote_symbol, cons(quoted, make_nil()));
}

SchemeObject* parse_unquote(Parser* parser) {
    Token token = peek_token(parser->lexer);
    consume_token(parser->lexer);
    
    SchemeObject* unquoted = parse_expression(parser);
    if (has_parse_error(parser)) {
        return NULL;
    }
    
    const char* symbol_name = (token.type == TOKEN_UNQUOTE) ? "unquote" : "unquote-splicing";
    SchemeObject* unquote_symbol = make_symbol(symbol_name);
    return cons(unquote_symbol, cons(unquoted, make_nil()));
}

SchemeObject* parse_atom(Parser* parser) {
    Token token = peek_token(parser->lexer);
    
    // Save the token value before consuming the token
    char* token_value = NULL;
    if (token.value) {
        token_value = scheme_strdup(token.value);
    }
    
    consume_token(parser->lexer);
    
    SchemeObject* result = NULL;
    switch (token.type) {
        case TOKEN_NUMBER:
            result = parse_number(token_value);
            break;
            
        case TOKEN_STRING:
            result = parse_string_literal(token_value);
            break;
            
        case TOKEN_SYMBOL:
            // Check if it's a character literal
            if (token_value && token_value[0] == '#' && token_value[1] == '\\') {
                if (strcmp(token_value, "#\\space") == 0) {
                    result = make_char(' ');
                } else if (strcmp(token_value, "#\\newline") == 0) {
                    result = make_char('\n');
                } else if (strcmp(token_value, "#\\tab") == 0) {
                    result = make_char('\t');
                } else if (strlen(token_value) == 3) {
                    result = make_char(token_value[2]);
                } else {
                    set_parse_error(parser, PARSE_ERROR_UNEXPECTED_TOKEN, "Invalid character literal");
                    result = NULL;
                }
            } else {
                result = make_symbol(token_value);
            }
            break;
            
        case TOKEN_BOOLEAN:
            result = make_boolean(strcmp(token_value, "#t") == 0);
            break;
            
        default:
            set_parse_error(parser, PARSE_ERROR_UNEXPECTED_TOKEN, "Expected atom");
            break;
    }
    
    if (token_value) {
        scheme_free(token_value);
    }
    
    return result;
}

SchemeObject* parse_number(const char* str) {
    if (!str || !is_valid_number(str)) {
        return NULL;
    }
    
    double value = strtod(str, NULL);
    return make_number(value);
}

SchemeObject* parse_string_literal(const char* str) {
    if (!str) {
        return make_string("");
    }
    
    char* unescaped = unescape_string(str);
    SchemeObject* result = make_string(unescaped);
    scheme_free(unescaped);
    return result;
}

char* unescape_string(const char* str) {
    if (!str) {
        return scheme_strdup("");
    }
    
    size_t len = strlen(str);
    char* result = (char*)scheme_malloc(len + 1);
    size_t write_pos = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '\\' && i + 1 < len) {
            switch (str[i + 1]) {
                case 'n': result[write_pos++] = '\n'; i++; break;
                case 't': result[write_pos++] = '\t'; i++; break;
                case 'r': result[write_pos++] = '\r'; i++; break;
                case '\\': result[write_pos++] = '\\'; i++; break;
                case '"': result[write_pos++] = '"'; i++; break;
                default: result[write_pos++] = str[i]; break;
            }
        } else {
            result[write_pos++] = str[i];
        }
    }
    
    result[write_pos] = '\0';
    return result;
}