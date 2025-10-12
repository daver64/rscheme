#include "rscheme.h"

// Global error state
static struct {
    EvalError error;
    char* message;
} eval_error_state = {EVAL_OK, NULL};

void set_eval_error(EvalError error, const char* message) {
    eval_error_state.error = error;
    if (eval_error_state.message) {
        scheme_free(eval_error_state.message);
    }
    eval_error_state.message = message ? scheme_strdup(message) : NULL;
}

void print_eval_error(FILE* out) {
    if (eval_error_state.error != EVAL_OK) {
        fprintf(out, "Evaluation Error: %s\n",
                eval_error_state.message ? eval_error_state.message : "Unknown error");
    }
}

bool has_eval_error(void) {
    return eval_error_state.error != EVAL_OK;
}

void clear_eval_error(void) {
    eval_error_state.error = EVAL_OK;
    if (eval_error_state.message) {
        scheme_free(eval_error_state.message);
        eval_error_state.message = NULL;
    }
}

bool is_self_evaluating(SchemeObject* expr) {
    return expr && (is_number(expr) || is_string(expr) || is_boolean(expr) || is_nil(expr));
}

bool is_variable(SchemeObject* expr) {
    return is_symbol(expr);
}

bool is_application(SchemeObject* expr) {
    return is_pair(expr);
}

bool is_special_form(SchemeObject* expr) {
    if (!is_pair(expr)) {
        return false;
    }
    
    SchemeObject* operator = car(expr);
    if (!is_symbol(operator)) {
        return false;
    }
    
    const char* name = operator->value.symbol_name;
    return strcmp(name, "quote") == 0 ||
           strcmp(name, "if") == 0 ||
           strcmp(name, "define") == 0 ||
           strcmp(name, "set!") == 0 ||
           strcmp(name, "lambda") == 0 ||
           strcmp(name, "begin") == 0 ||
           strcmp(name, "cond") == 0 ||
           strcmp(name, "and") == 0 ||
           strcmp(name, "or") == 0 ||
           strcmp(name, "let") == 0 ||
           strcmp(name, "let*") == 0 ||
           strcmp(name, "letrec") == 0;
}

SchemeObject* eval_expression(SchemeObject* expr, Environment* env) {
    if (has_eval_error()) {
        return NULL;
    }
    
    if (!expr) {
        return SCHEME_NIL_OBJECT;
    }
    
    // Self-evaluating expressions
    if (is_self_evaluating(expr)) {
        return expr;
    }
    
    // Variables
    if (is_variable(expr)) {
        SchemeObject* value = lookup_variable(env, expr->value.symbol_name);
        if (!value) {
            set_eval_error(EVAL_ERROR_UNBOUND_VARIABLE, expr->value.symbol_name);
            return NULL;
        }
        return value;
    }
    
    // Special forms and applications
    if (is_pair(expr)) {
        SchemeObject* operator = car(expr);
        SchemeObject* operands = cdr(expr);
        
        if (is_symbol(operator)) {
            const char* name = operator->value.symbol_name;
            
            // Special forms
            if (strcmp(name, "quote") == 0) {
                return eval_quote(operands, env);
            } else if (strcmp(name, "if") == 0) {
                return eval_if(operands, env);
            } else if (strcmp(name, "define") == 0) {
                return eval_define(operands, env);
            } else if (strcmp(name, "set!") == 0) {
                return eval_set(operands, env);
            } else if (strcmp(name, "lambda") == 0) {
                return eval_lambda(operands, env);
            } else if (strcmp(name, "begin") == 0) {
                return eval_begin(operands, env);
            } else if (strcmp(name, "and") == 0) {
                return eval_and(operands, env);
            } else if (strcmp(name, "or") == 0) {
                return eval_or(operands, env);
            } else if (strcmp(name, "cond") == 0) {
                return eval_cond(operands, env);
            } else if (strcmp(name, "let") == 0) {
                return eval_let(operands, env);
            } else if (strcmp(name, "let*") == 0) {
                return eval_let_star(operands, env);
            } else if (strcmp(name, "letrec") == 0) {
                return eval_letrec(operands, env);
            }
        }
        
        // Application
        SchemeObject* procedure = eval_expression(operator, env);
        if (has_eval_error()) {
            return NULL;
        }
        
        // Evaluate arguments
        SchemeObject* args = SCHEME_NIL_OBJECT;
        SchemeObject* current_arg = operands;
        SchemeObject* last_pair = NULL;
        
        while (current_arg && is_pair(current_arg)) {
            SchemeObject* arg_value = eval_expression(car(current_arg), env);
            if (has_eval_error()) {
                return NULL;
            }
            
            SchemeObject* new_pair = cons(arg_value, SCHEME_NIL_OBJECT);
            if (last_pair) {
                set_cdr(last_pair, new_pair);
            } else {
                args = new_pair;
            }
            last_pair = new_pair;
            
            current_arg = cdr(current_arg);
        }
        
        return apply_procedure(procedure, args, env);
    }
    
    set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "Invalid expression");
    return NULL;
}

SchemeObject* eval_sequence(SchemeObject* exprs, Environment* env) {
    SchemeObject* result = SCHEME_NIL_OBJECT;
    
    while (exprs && is_pair(exprs)) {
        result = eval_expression(car(exprs), env);
        if (has_eval_error()) {
            return NULL;
        }
        exprs = cdr(exprs);
    }
    
    return result;
}

SchemeObject* eval_quote(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!args || !is_pair(args) || cdr(args) != SCHEME_NIL_OBJECT) {
        set_eval_error(EVAL_ERROR_WRONG_ARITY, "quote expects exactly 1 argument");
        return NULL;
    }
    
    return car(args);
}

SchemeObject* eval_if(SchemeObject* args, Environment* env) {
    if (!args || !is_pair(args)) {
        set_eval_error(EVAL_ERROR_WRONG_ARITY, "if expects at least 2 arguments");
        return NULL;
    }
    
    SchemeObject* test = car(args);
    args = cdr(args);
    
    if (!args || !is_pair(args)) {
        set_eval_error(EVAL_ERROR_WRONG_ARITY, "if expects at least 2 arguments");
        return NULL;
    }
    
    SchemeObject* then_expr = car(args);
    args = cdr(args);
    
    SchemeObject* else_expr = (args && is_pair(args)) ? car(args) : SCHEME_NIL_OBJECT;
    
    SchemeObject* test_result = eval_expression(test, env);
    if (has_eval_error()) {
        return NULL;
    }
    
    // In Scheme, only #f is false
    bool is_true = !(test_result == SCHEME_FALSE_OBJECT ||
                    (is_boolean(test_result) && !test_result->value.boolean_value));
    
    if (is_true) {
        return eval_expression(then_expr, env);
    } else {
        return eval_expression(else_expr, env);
    }
}

SchemeObject* eval_define(SchemeObject* args, Environment* env) {
    if (!args || !is_pair(args)) {
        set_eval_error(EVAL_ERROR_WRONG_ARITY, "define expects at least 2 arguments");
        return NULL;
    }
    
    SchemeObject* first = car(args);
    SchemeObject* rest = cdr(args);
    
    if (is_symbol(first)) {
        // Variable definition: (define var value)
        if (!rest || !is_pair(rest) || cdr(rest) != SCHEME_NIL_OBJECT) {
            set_eval_error(EVAL_ERROR_WRONG_ARITY, "define expects exactly 2 arguments for variable");
            return NULL;
        }
        
        SchemeObject* value = eval_expression(car(rest), env);
        if (has_eval_error()) {
            return NULL;
        }
        
        define_variable(env, first->value.symbol_name, value);
        return SCHEME_NIL_OBJECT;
    } else if (is_pair(first)) {
        // Function definition: (define (name args...) body...)
        SchemeObject* name = car(first);
        SchemeObject* params = cdr(first);
        
        if (!is_symbol(name)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "define function name must be a symbol");
            return NULL;
        }
        
        SchemeObject* procedure = make_procedure(params, rest, env);
        define_variable(env, name->value.symbol_name, procedure);
        return SCHEME_NIL_OBJECT;
    } else {
        set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "define first argument must be symbol or list");
        return NULL;
    }
}

SchemeObject* eval_lambda(SchemeObject* args, Environment* env) {
    if (!args || !is_pair(args)) {
        set_eval_error(EVAL_ERROR_WRONG_ARITY, "lambda expects at least 2 arguments");
        return NULL;
    }
    
    SchemeObject* params = car(args);
    SchemeObject* body = cdr(args);
    
    return make_procedure(params, body, env);
}

SchemeObject* eval_begin(SchemeObject* args, Environment* env) {
    if (!args) {
        return SCHEME_NIL_OBJECT;
    }
    
    return eval_sequence(args, env);
}

SchemeObject* apply_procedure(SchemeObject* proc, SchemeObject* args, Environment* env) {
    if (!proc) {
        set_eval_error(EVAL_ERROR_WRONG_TYPE, "Cannot apply null procedure");
        return NULL;
    }
    
    if (is_primitive(proc)) {
        return proc->value.primitive(args, env);
    } else if (is_procedure(proc)) {
        Environment* new_env = extend_environment(
            proc->value.procedure.closure,
            proc->value.procedure.parameters,
            args
        );
        
        SchemeObject* result = eval_sequence(proc->value.procedure.body, new_env);
        release_environment(new_env);
        return result;
    } else {
        set_eval_error(EVAL_ERROR_WRONG_TYPE, "Object is not a procedure");
        return NULL;
    }
}

void run_repl(Environment* env) {
    printf("RScheme REPL - Enter expressions, or 'quit' to exit\n");
    
    char input[1024];
    while (true) {
        printf("rscheme> ");
        fflush(stdout);
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        clear_eval_error();
        
        Parser* parser = create_parser(input);
        SchemeObject* expr = parse_expression(parser);
        
        if (has_parse_error(parser)) {
            print_parse_error(parser, stderr);
        } else if (expr) {
            SchemeObject* result = eval_expression(expr, env);
            if (has_eval_error()) {
                print_eval_error(stderr);
            } else {
                print_object(result, stdout);
                printf("\n");
            }
        }
        
        destroy_parser(parser);
    }
    
    printf("Goodbye!\n");
}

// Stub implementations for missing functions
SchemeObject* eval_cond(SchemeObject* args, Environment* env) {
    if (!args || is_nil(args)) {
        // (cond) with no clauses is unspecified, return void
        return SCHEME_NIL_OBJECT;
    }
    
    SchemeObject* current = args;
    
    while (current && !is_nil(current)) {
        if (!is_pair(current)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "Malformed cond expression");
            return NULL;
        }
        
        SchemeObject* clause = car(current);
        if (!is_pair(clause)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "cond clause must be a list");
            return NULL;
        }
        
        SchemeObject* test = car(clause);
        SchemeObject* exprs = cdr(clause);
        
        // Handle (else ...) clause
        if (is_symbol(test) && strcmp(test->value.symbol_name, "else") == 0) {
            if (!is_nil(cdr(current))) {
                set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "else clause must be last in cond");
                return NULL;
            }
            // Execute all expressions in else clause
            return eval_begin(exprs, env);
        }
        
        // Evaluate the test
        SchemeObject* test_result = eval_expression(test, env);
        if (has_eval_error()) {
            return NULL;
        }
        
        // If test is true (anything other than #f)
        if (!is_boolean(test_result) || test_result->value.boolean_value) {
            if (is_nil(exprs)) {
                // No expressions, return the test result
                return test_result;
            } else {
                // Execute all expressions in the clause
                return eval_begin(exprs, env);
            }
        }
        
        current = cdr(current);
    }
    
    // No clause matched, return unspecified (void)
    return SCHEME_NIL_OBJECT;
}

SchemeObject* eval_and(SchemeObject* args, Environment* env) {
    // (and) returns #t
    if (!args || is_nil(args)) {
        return SCHEME_TRUE_OBJECT;
    }
    
    // Evaluate arguments left to right, short-circuit on first #f
    SchemeObject* current = args;
    SchemeObject* result = SCHEME_TRUE_OBJECT;
    
    while (current && !is_nil(current)) {
        if (!is_pair(current)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "Malformed and expression");
            return NULL;
        }
        
        result = eval_expression(car(current), env);
        if (has_eval_error()) {
            return NULL;
        }
        
        // If result is #f, short-circuit and return #f
        if (is_boolean(result) && !result->value.boolean_value) {
            return SCHEME_FALSE_OBJECT;
        }
        
        current = cdr(current);
    }
    
    // Return the last evaluated result
    return result;
}

SchemeObject* eval_or(SchemeObject* args, Environment* env) {
    // (or) returns #f
    if (!args || is_nil(args)) {
        return SCHEME_FALSE_OBJECT;
    }
    
    // Evaluate arguments left to right, short-circuit on first non-#f
    SchemeObject* current = args;
    
    while (current && !is_nil(current)) {
        if (!is_pair(current)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "Malformed or expression");
            return NULL;
        }
        
        SchemeObject* result = eval_expression(car(current), env);
        if (has_eval_error()) {
            return NULL;
        }
        
        // If result is not #f, short-circuit and return it
        if (!is_boolean(result) || result->value.boolean_value) {
            return result;
        }
        
        current = cdr(current);
    }
    
    // All arguments were #f, return #f
    return SCHEME_FALSE_OBJECT;
}

SchemeObject* eval_let(SchemeObject* args, Environment* env) {
    if (!args || is_nil(args)) {
        set_eval_error(EVAL_ERROR_WRONG_ARITY, "let requires at least 1 argument");
        return NULL;
    }
    
    SchemeObject* bindings = car(args);
    SchemeObject* body = cdr(args);
    
    if (!is_pair(bindings) && !is_nil(bindings)) {
        set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "let bindings must be a list");
        return NULL;
    }
    
    // Create new environment
    Environment* let_env = make_environment(env);
    
    // Process bindings
    SchemeObject* current_binding = bindings;
    while (current_binding && !is_nil(current_binding)) {
        if (!is_pair(current_binding)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "Malformed let bindings");
            return NULL;
        }
        
        SchemeObject* binding = car(current_binding);
        if (!is_pair(binding)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "let binding must be a list");
            return NULL;
        }
        
        SchemeObject* var = car(binding);
        SchemeObject* val_expr = car(cdr(binding));
        
        if (!is_symbol(var)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "let variable must be a symbol");
            return NULL;
        }
        
        // Evaluate the value in the original environment
        SchemeObject* value = eval_expression(val_expr, env);
        if (has_eval_error()) {
            return NULL;
        }
        
        // Bind in the new environment
        define_variable(let_env, var->value.symbol_name, value);
        
        current_binding = cdr(current_binding);
    }
    
    // Execute body in new environment
    return eval_begin(body, let_env);
}

SchemeObject* eval_let_star(SchemeObject* args, Environment* env) {
    if (!args || is_nil(args)) {
        set_eval_error(EVAL_ERROR_WRONG_ARITY, "let* requires at least 1 argument");
        return NULL;
    }
    
    SchemeObject* bindings = car(args);
    SchemeObject* body = cdr(args);
    
    if (!is_pair(bindings) && !is_nil(bindings)) {
        set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "let* bindings must be a list");
        return NULL;
    }
    
    // Create new environment
    Environment* current_env = make_environment(env);
    
    // Process bindings sequentially, each in the environment created so far
    SchemeObject* current_binding = bindings;
    while (current_binding && !is_nil(current_binding)) {
        if (!is_pair(current_binding)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "Malformed let* bindings");
            return NULL;
        }
        
        SchemeObject* binding = car(current_binding);
        if (!is_pair(binding)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "let* binding must be a list");
            return NULL;
        }
        
        SchemeObject* var = car(binding);
        SchemeObject* val_expr = car(cdr(binding));
        
        if (!is_symbol(var)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "let* variable must be a symbol");
            return NULL;
        }
        
        // Evaluate the value in the current environment (includes previous bindings)
        SchemeObject* value = eval_expression(val_expr, current_env);
        if (has_eval_error()) {
            return NULL;
        }
        
        // Bind in the current environment
        define_variable(current_env, var->value.symbol_name, value);
        
        current_binding = cdr(current_binding);
    }
    
    // Execute body in final environment
    return eval_begin(body, current_env);
}

SchemeObject* eval_letrec(SchemeObject* args, Environment* env) {
    if (!args || is_nil(args)) {
        set_eval_error(EVAL_ERROR_WRONG_ARITY, "letrec requires at least 1 argument");
        return NULL;
    }
    
    SchemeObject* bindings = car(args);
    SchemeObject* body = cdr(args);
    
    if (!is_pair(bindings) && !is_nil(bindings)) {
        set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "letrec bindings must be a list");
        return NULL;
    }
    
    // Create new environment
    Environment* letrec_env = make_environment(env);
    
    // First pass: bind all variables to unspecified values
    SchemeObject* current_binding = bindings;
    while (current_binding && !is_nil(current_binding)) {
        if (!is_pair(current_binding)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "Malformed letrec bindings");
            return NULL;
        }
        
        SchemeObject* binding = car(current_binding);
        if (!is_pair(binding)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "letrec binding must be a list");
            return NULL;
        }
        
        SchemeObject* var = car(binding);
        if (!is_symbol(var)) {
            set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "letrec variable must be a symbol");
            return NULL;
        }
        
        // Bind to nil initially (unspecified value)
        define_variable(letrec_env, var->value.symbol_name, SCHEME_NIL_OBJECT);
        
        current_binding = cdr(current_binding);
    }
    
    // Second pass: evaluate and assign the actual values
    current_binding = bindings;
    while (current_binding && !is_nil(current_binding)) {
        SchemeObject* binding = car(current_binding);
        SchemeObject* var = car(binding);
        SchemeObject* val_expr = car(cdr(binding));
        
        // Evaluate the value in the environment with all variables bound
        SchemeObject* value = eval_expression(val_expr, letrec_env);
        if (has_eval_error()) {
            return NULL;
        }
        
        // Update the binding
        set_variable(letrec_env, var->value.symbol_name, value);
        
        current_binding = cdr(current_binding);
    }
    
    // Execute body in the environment
    return eval_begin(body, letrec_env);
}

SchemeObject* eval_set(SchemeObject* args, Environment* env) {
    if (list_length(args) != 2) {
        set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "set! expects exactly 2 arguments");
        return NULL;
    }
    
    SchemeObject* var = car(args);
    SchemeObject* value_expr = car(cdr(args));
    
    if (!is_symbol(var)) {
        set_eval_error(EVAL_ERROR_INVALID_SYNTAX, "set! first argument must be a symbol");
        return NULL;
    }
    
    SchemeObject* value = eval_expression(value_expr, env);
    if (!value) {
        return NULL; // Error already set
    }
    
    if (!set_variable_if_exists(env, var->value.symbol_name, value)) {
        set_eval_error(EVAL_ERROR_RUNTIME, "set! variable not defined");
        return NULL;
    }
    
    return SCHEME_NIL_OBJECT;
}

SchemeObject* eval_quasiquote(SchemeObject* args, Environment* env) {
    (void)args; (void)env;
    set_eval_error(EVAL_ERROR_RUNTIME, "quasiquote not implemented yet");
    return NULL;
}

SchemeObject* eval_unquote(SchemeObject* args, Environment* env) {
    (void)args; (void)env;
    set_eval_error(EVAL_ERROR_RUNTIME, "unquote not implemented yet");
    return NULL;
}

SchemeObject* read_expression(FILE* input) {
    (void)input;
    return NULL; // Not implemented
}

void print_result(SchemeObject* result, FILE* output) {
    print_object(result, output);
}

SchemeObject* load_file(const char* filename, Environment* env) {
    (void)filename; (void)env;
    set_eval_error(EVAL_ERROR_RUNTIME, "load-file not implemented yet");
    return NULL;
}

SchemeObject* eval_file(const char* filename, Environment* env) {
    (void)filename; (void)env;
    set_eval_error(EVAL_ERROR_RUNTIME, "eval-file not implemented yet");
    return NULL;
}

SchemeObject* get_special_form_handler(const char* name) {
    (void)name;
    return NULL; // Not implemented
}
