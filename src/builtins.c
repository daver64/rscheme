#include "rscheme.h"
#include <ctype.h>

void init_builtins(Environment* env) {
    // Arithmetic operations
    define_variable(env, "+", make_primitive(builtin_add));
    define_variable(env, "-", make_primitive(builtin_subtract));
    define_variable(env, "*", make_primitive(builtin_multiply));
    define_variable(env, "/", make_primitive(builtin_divide));
    define_variable(env, "modulo", make_primitive(builtin_modulo));
    define_variable(env, "quotient", make_primitive(builtin_quotient));
    define_variable(env, "remainder", make_primitive(builtin_remainder));
    define_variable(env, "abs", make_primitive(builtin_abs));
    define_variable(env, "max", make_primitive(builtin_max));
    define_variable(env, "min", make_primitive(builtin_min));
    
    // Comparison operations
    define_variable(env, "=", make_primitive(builtin_num_eq));
    define_variable(env, "eq?", make_primitive(builtin_eq));
    define_variable(env, "eqv?", make_primitive(builtin_eqv));
    define_variable(env, "equal?", make_primitive(builtin_equal));
    define_variable(env, "<", make_primitive(builtin_lt));
    define_variable(env, "<=", make_primitive(builtin_le));
    define_variable(env, ">", make_primitive(builtin_gt));
    define_variable(env, ">=", make_primitive(builtin_ge));
    
    // List operations
    define_variable(env, "cons", make_primitive(builtin_cons));
    define_variable(env, "car", make_primitive(builtin_car));
    define_variable(env, "cdr", make_primitive(builtin_cdr));
    define_variable(env, "list", make_primitive(builtin_list));
    define_variable(env, "length", make_primitive(builtin_length));
    define_variable(env, "append", make_primitive(builtin_append));
    define_variable(env, "reverse", make_primitive(builtin_reverse));
    define_variable(env, "list-ref", make_primitive(builtin_list_ref));
    
    // Type predicates
    define_variable(env, "null?", make_primitive(builtin_null_p));
    define_variable(env, "pair?", make_primitive(builtin_pair_p));
    define_variable(env, "list?", make_primitive(builtin_list_p));
    define_variable(env, "number?", make_primitive(builtin_number_p));
    define_variable(env, "string?", make_primitive(builtin_string_p));
    define_variable(env, "symbol?", make_primitive(builtin_symbol_p));
    define_variable(env, "boolean?", make_primitive(builtin_boolean_p));
    define_variable(env, "procedure?", make_primitive(builtin_procedure_p));
    
    // I/O operations
    define_variable(env, "display", make_primitive(builtin_display));
    define_variable(env, "newline", make_primitive(builtin_newline));
    define_variable(env, "write", make_primitive(builtin_write));
    
    // String operations
    define_variable(env, "string-length", make_primitive(builtin_string_length));
    define_variable(env, "string-ref", make_primitive(builtin_string_ref));
    
    // Character operations
    define_variable(env, "char?", make_primitive(builtin_char_p));
    define_variable(env, "char=?", make_primitive(builtin_char_eq));
    define_variable(env, "char<?", make_primitive(builtin_char_lt));
    define_variable(env, "char>?", make_primitive(builtin_char_gt));
    define_variable(env, "char<=?", make_primitive(builtin_char_le));
    define_variable(env, "char>=?", make_primitive(builtin_char_ge));
    define_variable(env, "char-alphabetic?", make_primitive(builtin_char_alphabetic));
    define_variable(env, "char-numeric?", make_primitive(builtin_char_numeric));
    define_variable(env, "char-whitespace?", make_primitive(builtin_char_whitespace));
    define_variable(env, "char-upcase", make_primitive(builtin_char_upcase));
    define_variable(env, "char-downcase", make_primitive(builtin_char_downcase));
    define_variable(env, "char->integer", make_primitive(builtin_char_to_integer));
    define_variable(env, "integer->char", make_primitive(builtin_integer_to_char));
    
    // Logical operations
    define_variable(env, "not", make_primitive(builtin_not));
}

int count_args(SchemeObject* args) {
    int count = 0;
    while (args && is_pair(args)) {
        count++;
        args = cdr(args);
    }
    return count;
}

SchemeObject* get_arg(SchemeObject* args, int index) {
    for (int i = 0; i < index && args && is_pair(args); i++) {
        args = cdr(args);
    }
    return args && is_pair(args) ? car(args) : NULL;
}

bool check_arity(SchemeObject* args, int expected) {
    return count_args(args) == expected;
}

bool check_min_arity(SchemeObject* args, int min) {
    return count_args(args) >= min;
}

// Arithmetic operations
SchemeObject* builtin_add(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    double result = 0.0;
    
    while (args && is_pair(args)) {
        SchemeObject* arg = car(args);
        if (!is_number(arg)) {
            runtime_error("+ expects numbers");
            return SCHEME_FALSE_OBJECT;
        }
        result += arg->value.number_value;
        args = cdr(args);
    }
    
    return make_number(result);
}

SchemeObject* builtin_subtract(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_min_arity(args, 1)) {
        runtime_error("- expects at least 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* first = car(args);
    if (!is_number(first)) {
        runtime_error("- expects numbers");
        return SCHEME_FALSE_OBJECT;
    }
    
    double result = first->value.number_value;
    args = cdr(args);
    
    if (!args || is_nil(args)) {
        // Unary minus
        return make_number(-result);
    }
    
    while (args && is_pair(args)) {
        SchemeObject* arg = car(args);
        if (!is_number(arg)) {
            runtime_error("- expects numbers");
            return SCHEME_FALSE_OBJECT;
        }
        result -= arg->value.number_value;
        args = cdr(args);
    }
    
    return make_number(result);
}

SchemeObject* builtin_multiply(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    double result = 1.0;
    
    while (args && is_pair(args)) {
        SchemeObject* arg = car(args);
        if (!is_number(arg)) {
            runtime_error("* expects numbers");
            return SCHEME_FALSE_OBJECT;
        }
        result *= arg->value.number_value;
        args = cdr(args);
    }
    
    return make_number(result);
}

SchemeObject* builtin_divide(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_min_arity(args, 1)) {
        runtime_error("/ expects at least 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* first = car(args);
    if (!is_number(first)) {
        runtime_error("/ expects numbers");
        return SCHEME_FALSE_OBJECT;
    }
    
    double result = first->value.number_value;
    args = cdr(args);
    
    if (!args || is_nil(args)) {
        // Reciprocal
        if (result == 0.0) {
            runtime_error("Division by zero");
            return SCHEME_FALSE_OBJECT;
        }
        return make_number(1.0 / result);
    }
    
    while (args && is_pair(args)) {
        SchemeObject* arg = car(args);
        if (!is_number(arg)) {
            runtime_error("/ expects numbers");
            return SCHEME_FALSE_OBJECT;
        }
        if (arg->value.number_value == 0.0) {
            runtime_error("Division by zero");
            return SCHEME_FALSE_OBJECT;
        }
        result /= arg->value.number_value;
        args = cdr(args);
    }
    
    return make_number(result);
}

// Comparison operations
SchemeObject* builtin_num_eq(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (list_length(args) < 2) {
        runtime_error("= expects at least 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    // Get first argument for comparison
    SchemeObject* first = get_arg(args, 0);
    if (!is_number(first)) {
        runtime_error("= expects numbers");
        return SCHEME_FALSE_OBJECT;
    }
    
    // Compare all arguments to the first
    SchemeObject* current = cdr(args);
    while (!is_null(current)) {
        SchemeObject* arg = car(current);
        if (!is_number(arg)) {
            runtime_error("= expects numbers");
            return SCHEME_FALSE_OBJECT;
        }
        if (first->value.number_value != arg->value.number_value) {
            return SCHEME_FALSE_OBJECT;
        }
        current = cdr(current);
    }
    
    return SCHEME_TRUE_OBJECT;
}

SchemeObject* builtin_eq(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 2)) {
        runtime_error("eq? expects 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* a = get_arg(args, 0);
    SchemeObject* b = get_arg(args, 1);
    
    return make_boolean(scheme_eq(a, b));
}

SchemeObject* builtin_equal(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 2)) {
        runtime_error("equal? expects 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* a = get_arg(args, 0);
    SchemeObject* b = get_arg(args, 1);
    
    return make_boolean(scheme_equal(a, b));
}

SchemeObject* builtin_lt(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (list_length(args) < 2) {
        runtime_error("< expects at least 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    // Check that arguments are in ascending order
    SchemeObject* current = args;
    while (!is_null(cdr(current))) {
        SchemeObject* a = car(current);
        SchemeObject* b = car(cdr(current));
        
        if (!is_number(a) || !is_number(b)) {
            runtime_error("< expects numbers");
            return SCHEME_FALSE_OBJECT;
        }
        
        if (a->value.number_value >= b->value.number_value) {
            return SCHEME_FALSE_OBJECT;
        }
        
        current = cdr(current);
    }
    
    return SCHEME_TRUE_OBJECT;
}

// List operations
SchemeObject* builtin_cons(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 2)) {
        runtime_error("cons expects 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* car_val = get_arg(args, 0);
    SchemeObject* cdr_val = get_arg(args, 1);
    
    return cons(car_val, cdr_val);
}

SchemeObject* builtin_car(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("car expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* pair = get_arg(args, 0);
    if (!is_pair(pair)) {
        runtime_error("car expects a pair");
        return SCHEME_FALSE_OBJECT;
    }
    
    return car(pair);
}

SchemeObject* builtin_cdr(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("cdr expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* pair = get_arg(args, 0);
    if (!is_pair(pair)) {
        runtime_error("cdr expects a pair");
        return SCHEME_FALSE_OBJECT;
    }
    
    return cdr(pair);
}

SchemeObject* builtin_list(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    return args; // args is already a proper list
}

// Type predicates
SchemeObject* builtin_null_p(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("null? expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* obj = get_arg(args, 0);
    return make_boolean(is_null(obj));
}

SchemeObject* builtin_pair_p(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("pair? expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* obj = get_arg(args, 0);
    return make_boolean(is_pair(obj));
}

SchemeObject* builtin_number_p(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("number? expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* obj = get_arg(args, 0);
    return make_boolean(is_number(obj));
}

// I/O operations
SchemeObject* builtin_display(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("display expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* obj = get_arg(args, 0);
    
    if (is_string(obj)) {
        printf("%s", obj->value.string_value);
    } else {
        print_object(obj, stdout);
    }
    
    return SCHEME_NIL_OBJECT;
}

SchemeObject* builtin_newline(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    (void)args; // Unused
    
    printf("\n");
    return SCHEME_NIL_OBJECT;
}

SchemeObject* builtin_write(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("write expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* obj = get_arg(args, 0);
    print_object(obj, stdout);
    
    return SCHEME_NIL_OBJECT;
}

SchemeObject* builtin_not(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("not expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* obj = get_arg(args, 0);
    
    // In Scheme, only #f is false
    bool is_false = (obj == SCHEME_FALSE_OBJECT) || 
                   (is_boolean(obj) && !obj->value.boolean_value);
    
    return make_boolean(is_false);
}

// Stub implementations for missing functions
SchemeObject* builtin_modulo(SchemeObject* args, Environment* env) {
    (void)env;
    if (count_args(args) != 2) {
        runtime_error("modulo expects 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* first = car(args);
    SchemeObject* second = car(cdr(args));
    
    if (!is_number(first) || !is_number(second)) {
        runtime_error("modulo expects numbers");
        return SCHEME_FALSE_OBJECT;
    }
    
    int a = (int)first->value.number_value;
    int b = (int)second->value.number_value;
    
    if (b == 0) {
        runtime_error("modulo: division by zero");
        return SCHEME_FALSE_OBJECT;
    }
    
    int result = a % b;
    // Ensure result has same sign as divisor (b)
    if ((result > 0 && b < 0) || (result < 0 && b > 0)) {
        result += b;
    }
    
    return make_number((double)result);
}

SchemeObject* builtin_quotient(SchemeObject* args, Environment* env) {
    (void)env;
    if (count_args(args) != 2) {
        runtime_error("quotient expects 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* first = car(args);
    SchemeObject* second = car(cdr(args));
    
    if (!is_number(first) || !is_number(second)) {
        runtime_error("quotient expects numbers");
        return SCHEME_FALSE_OBJECT;
    }
    
    int a = (int)first->value.number_value;
    int b = (int)second->value.number_value;
    
    if (b == 0) {
        runtime_error("quotient: division by zero");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_number((double)(a / b));
}

SchemeObject* builtin_remainder(SchemeObject* args, Environment* env) {
    (void)env;
    if (count_args(args) != 2) {
        runtime_error("remainder expects 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* first = car(args);
    SchemeObject* second = car(cdr(args));
    
    if (!is_number(first) || !is_number(second)) {
        runtime_error("remainder expects numbers");
        return SCHEME_FALSE_OBJECT;
    }
    
    int a = (int)first->value.number_value;
    int b = (int)second->value.number_value;
    
    if (b == 0) {
        runtime_error("remainder: division by zero");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_number((double)(a % b));
}

SchemeObject* builtin_abs(SchemeObject* args, Environment* env) {
    (void)env;
    if (count_args(args) != 1) {
        runtime_error("abs expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* arg = car(args);
    if (!is_number(arg)) {
        runtime_error("abs expects a number");
        return SCHEME_FALSE_OBJECT;
    }
    
    double value = arg->value.number_value;
    return make_number(value < 0 ? -value : value);
}

SchemeObject* builtin_max(SchemeObject* args, Environment* env) {
    (void)env;
    if (is_nil(args)) {
        runtime_error("max expects at least 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    double max_val = car(args)->value.number_value;
    args = cdr(args);
    
    while (!is_nil(args)) {
        if (!is_number(car(args))) {
            runtime_error("max expects numbers");
            return SCHEME_FALSE_OBJECT;
        }
        double val = car(args)->value.number_value;
        if (val > max_val) {
            max_val = val;
        }
        args = cdr(args);
    }
    
    return make_number(max_val);
}

SchemeObject* builtin_min(SchemeObject* args, Environment* env) {
    (void)env;
    if (is_nil(args)) {
        runtime_error("min expects at least 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    double min_val = car(args)->value.number_value;
    args = cdr(args);
    
    while (!is_nil(args)) {
        if (!is_number(car(args))) {
            runtime_error("min expects numbers");
            return SCHEME_FALSE_OBJECT;
        }
        double val = car(args)->value.number_value;
        if (val < min_val) {
            min_val = val;
        }
        args = cdr(args);
    }
    
    return make_number(min_val);
}

SchemeObject* builtin_eqv(SchemeObject* args, Environment* env) {
    (void)args; (void)env;
    runtime_error("eqv? not implemented yet");
    return SCHEME_FALSE_OBJECT;
}

SchemeObject* builtin_gt(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (list_length(args) < 2) {
        runtime_error("> expects at least 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    // Check that arguments are in descending order
    SchemeObject* current = args;
    while (!is_null(cdr(current))) {
        SchemeObject* a = car(current);
        SchemeObject* b = car(cdr(current));
        
        if (!is_number(a) || !is_number(b)) {
            runtime_error("> expects numbers");
            return SCHEME_FALSE_OBJECT;
        }
        
        if (a->value.number_value <= b->value.number_value) {
            return SCHEME_FALSE_OBJECT;
        }
        
        current = cdr(current);
    }
    
    return SCHEME_TRUE_OBJECT;
}

SchemeObject* builtin_le(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (list_length(args) < 2) {
        runtime_error("<= expects at least 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    // Check that arguments are in non-descending order
    SchemeObject* current = args;
    while (!is_null(cdr(current))) {
        SchemeObject* a = car(current);
        SchemeObject* b = car(cdr(current));
        
        if (!is_number(a) || !is_number(b)) {
            runtime_error("<= expects numbers");
            return SCHEME_FALSE_OBJECT;
        }
        
        if (a->value.number_value > b->value.number_value) {
            return SCHEME_FALSE_OBJECT;
        }
        
        current = cdr(current);
    }
    
    return SCHEME_TRUE_OBJECT;
}

SchemeObject* builtin_ge(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (list_length(args) < 2) {
        runtime_error(">= expects at least 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    // Check that arguments are in non-ascending order
    SchemeObject* current = args;
    while (!is_null(cdr(current))) {
        SchemeObject* a = car(current);
        SchemeObject* b = car(cdr(current));
        
        if (!is_number(a) || !is_number(b)) {
            runtime_error(">= expects numbers");
            return SCHEME_FALSE_OBJECT;
        }
        
        if (a->value.number_value < b->value.number_value) {
            return SCHEME_FALSE_OBJECT;
        }
        
        current = cdr(current);
    }
    
    return SCHEME_TRUE_OBJECT;
}

SchemeObject* builtin_length(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("length expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* obj = get_arg(args, 0);
    
    if (is_null(obj)) {
        return make_number(0);
    }
    
    if (!is_pair(obj)) {
        runtime_error("length expects a list");
        return SCHEME_FALSE_OBJECT;
    }
    
    int length = list_length(obj);
    return make_number((double)length);
}

SchemeObject* builtin_append(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (is_null(args)) {
        return SCHEME_NIL_OBJECT;
    }
    
    if (is_null(cdr(args))) {
        return car(args);
    }
    
    // Recursive implementation: append first list to result of appending rest
    SchemeObject* first = car(args);
    SchemeObject* rest = builtin_append(cdr(args), env);
    
    if (is_null(first)) {
        return rest;
    }
    
    if (!is_pair(first)) {
        runtime_error("append expects lists");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_pair(car(first), builtin_append(make_pair(cdr(first), make_pair(rest, SCHEME_NIL_OBJECT)), env));
}

SchemeObject* builtin_reverse(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("reverse expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* list = get_arg(args, 0);
    
    if (is_null(list)) {
        return SCHEME_NIL_OBJECT;
    }
    
    if (!is_pair(list)) {
        runtime_error("reverse expects a list");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* result = SCHEME_NIL_OBJECT;
    SchemeObject* current = list;
    
    while (!is_null(current)) {
        if (!is_pair(current)) {
            runtime_error("reverse expects a proper list");
            return SCHEME_FALSE_OBJECT;
        }
        result = make_pair(car(current), result);
        current = cdr(current);
    }
    
    return result;
}

SchemeObject* builtin_list_ref(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 2)) {
        runtime_error("list-ref expects 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* list = get_arg(args, 0);
    SchemeObject* index_obj = get_arg(args, 1);
    
    if (!is_number(index_obj)) {
        runtime_error("list-ref expects a number as second argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    int index = (int)index_obj->value.number_value;
    if (index < 0) {
        runtime_error("list-ref index must be non-negative");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* current = list;
    for (int i = 0; i < index; i++) {
        if (is_null(current) || !is_pair(current)) {
            runtime_error("list-ref index out of bounds");
            return SCHEME_FALSE_OBJECT;
        }
        current = cdr(current);
    }
    
    if (is_null(current) || !is_pair(current)) {
        runtime_error("list-ref index out of bounds");
        return SCHEME_FALSE_OBJECT;
    }
    
    return car(current);
}

SchemeObject* builtin_list_p(SchemeObject* args, Environment* env) {
    (void)args; (void)env;
    runtime_error("list? not implemented yet");
    return SCHEME_FALSE_OBJECT;
}

SchemeObject* builtin_string_p(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("string? expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* obj = get_arg(args, 0);
    return make_boolean(is_string(obj));
}

SchemeObject* builtin_symbol_p(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("symbol? expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* obj = get_arg(args, 0);
    return make_boolean(is_symbol(obj));
}

SchemeObject* builtin_boolean_p(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("boolean? expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* obj = get_arg(args, 0);
    return make_boolean(is_boolean(obj));
}

SchemeObject* builtin_procedure_p(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("procedure? expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* obj = get_arg(args, 0);
    return make_boolean(is_procedure(obj) || is_primitive(obj));
}

// String operations
SchemeObject* builtin_string_length(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 1)) {
        runtime_error("string-length expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* obj = get_arg(args, 0);
    if (!is_string(obj)) {
        runtime_error("string-length expects a string");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_number((double)strlen(obj->value.string_value));
}

SchemeObject* builtin_string_ref(SchemeObject* args, Environment* env) {
    (void)env; // Unused
    
    if (!check_arity(args, 2)) {
        runtime_error("string-ref expects 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* str = get_arg(args, 0);
    SchemeObject* index_obj = get_arg(args, 1);
    
    if (!is_string(str)) {
        runtime_error("string-ref expects a string as first argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    if (!is_number(index_obj)) {
        runtime_error("string-ref expects a number as second argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    int index = (int)index_obj->value.number_value;
    int len = strlen(str->value.string_value);
    
    if (index < 0 || index >= len) {
        runtime_error("string-ref index out of bounds");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_char(str->value.string_value[index]);
}

// Character predicates and operations
SchemeObject* builtin_char_p(SchemeObject* args, Environment* env) {
    (void)env;
    
    if (!check_arity(args, 1)) {
        runtime_error("char? expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_boolean(is_char(get_arg(args, 0)));
}

SchemeObject* builtin_char_eq(SchemeObject* args, Environment* env) {
    (void)env;
    
    if (!check_arity(args, 2)) {
        runtime_error("char=? expects 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* c1 = get_arg(args, 0);
    SchemeObject* c2 = get_arg(args, 1);
    
    if (!is_char(c1) || !is_char(c2)) {
        runtime_error("char=? expects character arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_boolean(c1->value.char_value == c2->value.char_value);
}

SchemeObject* builtin_char_lt(SchemeObject* args, Environment* env) {
    (void)env;
    
    if (!check_arity(args, 2)) {
        runtime_error("char<? expects 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* c1 = get_arg(args, 0);
    SchemeObject* c2 = get_arg(args, 1);
    
    if (!is_char(c1) || !is_char(c2)) {
        runtime_error("char<? expects character arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_boolean(c1->value.char_value < c2->value.char_value);
}

SchemeObject* builtin_char_gt(SchemeObject* args, Environment* env) {
    (void)env;
    
    if (!check_arity(args, 2)) {
        runtime_error("char>? expects 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* c1 = get_arg(args, 0);
    SchemeObject* c2 = get_arg(args, 1);
    
    if (!is_char(c1) || !is_char(c2)) {
        runtime_error("char>? expects character arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_boolean(c1->value.char_value > c2->value.char_value);
}

SchemeObject* builtin_char_le(SchemeObject* args, Environment* env) {
    (void)env;
    
    if (!check_arity(args, 2)) {
        runtime_error("char<=? expects 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* c1 = get_arg(args, 0);
    SchemeObject* c2 = get_arg(args, 1);
    
    if (!is_char(c1) || !is_char(c2)) {
        runtime_error("char<=? expects character arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_boolean(c1->value.char_value <= c2->value.char_value);
}

SchemeObject* builtin_char_ge(SchemeObject* args, Environment* env) {
    (void)env;
    
    if (!check_arity(args, 2)) {
        runtime_error("char>=? expects 2 arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* c1 = get_arg(args, 0);
    SchemeObject* c2 = get_arg(args, 1);
    
    if (!is_char(c1) || !is_char(c2)) {
        runtime_error("char>=? expects character arguments");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_boolean(c1->value.char_value >= c2->value.char_value);
}

SchemeObject* builtin_char_alphabetic(SchemeObject* args, Environment* env) {
    (void)env;
    
    if (!check_arity(args, 1)) {
        runtime_error("char-alphabetic? expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* c = get_arg(args, 0);
    if (!is_char(c)) {
        runtime_error("char-alphabetic? expects a character");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_boolean(isalpha(c->value.char_value));
}

SchemeObject* builtin_char_numeric(SchemeObject* args, Environment* env) {
    (void)env;
    
    if (!check_arity(args, 1)) {
        runtime_error("char-numeric? expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* c = get_arg(args, 0);
    if (!is_char(c)) {
        runtime_error("char-numeric? expects a character");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_boolean(isdigit(c->value.char_value));
}

SchemeObject* builtin_char_whitespace(SchemeObject* args, Environment* env) {
    (void)env;
    
    if (!check_arity(args, 1)) {
        runtime_error("char-whitespace? expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* c = get_arg(args, 0);
    if (!is_char(c)) {
        runtime_error("char-whitespace? expects a character");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_boolean(isspace(c->value.char_value));
}

SchemeObject* builtin_char_upcase(SchemeObject* args, Environment* env) {
    (void)env;
    
    if (!check_arity(args, 1)) {
        runtime_error("char-upcase expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* c = get_arg(args, 0);
    if (!is_char(c)) {
        runtime_error("char-upcase expects a character");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_char(toupper(c->value.char_value));
}

SchemeObject* builtin_char_downcase(SchemeObject* args, Environment* env) {
    (void)env;
    
    if (!check_arity(args, 1)) {
        runtime_error("char-downcase expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* c = get_arg(args, 0);
    if (!is_char(c)) {
        runtime_error("char-downcase expects a character");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_char(tolower(c->value.char_value));
}

SchemeObject* builtin_char_to_integer(SchemeObject* args, Environment* env) {
    (void)env;
    
    if (!check_arity(args, 1)) {
        runtime_error("char->integer expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* c = get_arg(args, 0);
    if (!is_char(c)) {
        runtime_error("char->integer expects a character");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_number((double)(unsigned char)c->value.char_value);
}

SchemeObject* builtin_integer_to_char(SchemeObject* args, Environment* env) {
    (void)env;
    
    if (!check_arity(args, 1)) {
        runtime_error("integer->char expects 1 argument");
        return SCHEME_FALSE_OBJECT;
    }
    
    SchemeObject* n = get_arg(args, 0);
    if (!is_number(n)) {
        runtime_error("integer->char expects a number");
        return SCHEME_FALSE_OBJECT;
    }
    
    int val = (int)n->value.number_value;
    if (val < 0 || val > 255) {
        runtime_error("integer->char: value out of character range");
        return SCHEME_FALSE_OBJECT;
    }
    
    return make_char((char)val);
}

