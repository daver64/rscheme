#ifndef BUILTINS_H
#define BUILTINS_H

#include "scheme_objects.h"
#include "environment.h"

// Initialize built-in procedures in global environment
void init_builtins(Environment* env);

// Arithmetic operations
SchemeObject* builtin_add(SchemeObject* args, Environment* env);
SchemeObject* builtin_subtract(SchemeObject* args, Environment* env);
SchemeObject* builtin_multiply(SchemeObject* args, Environment* env);
SchemeObject* builtin_divide(SchemeObject* args, Environment* env);
SchemeObject* builtin_modulo(SchemeObject* args, Environment* env);
SchemeObject* builtin_abs(SchemeObject* args, Environment* env);
SchemeObject* builtin_max(SchemeObject* args, Environment* env);
SchemeObject* builtin_min(SchemeObject* args, Environment* env);

// Comparison operations
SchemeObject* builtin_num_eq(SchemeObject* args, Environment* env);
SchemeObject* builtin_eq(SchemeObject* args, Environment* env);
SchemeObject* builtin_eqv(SchemeObject* args, Environment* env);
SchemeObject* builtin_equal(SchemeObject* args, Environment* env);
SchemeObject* builtin_lt(SchemeObject* args, Environment* env);
SchemeObject* builtin_le(SchemeObject* args, Environment* env);
SchemeObject* builtin_gt(SchemeObject* args, Environment* env);
SchemeObject* builtin_ge(SchemeObject* args, Environment* env);

// List operations
SchemeObject* builtin_cons(SchemeObject* args, Environment* env);
SchemeObject* builtin_car(SchemeObject* args, Environment* env);
SchemeObject* builtin_cdr(SchemeObject* args, Environment* env);
SchemeObject* builtin_list(SchemeObject* args, Environment* env);
SchemeObject* builtin_length(SchemeObject* args, Environment* env);
SchemeObject* builtin_append(SchemeObject* args, Environment* env);
SchemeObject* builtin_reverse(SchemeObject* args, Environment* env);
SchemeObject* builtin_list_ref(SchemeObject* args, Environment* env);

// Type predicates
SchemeObject* builtin_null_p(SchemeObject* args, Environment* env);
SchemeObject* builtin_pair_p(SchemeObject* args, Environment* env);
SchemeObject* builtin_list_p(SchemeObject* args, Environment* env);
SchemeObject* builtin_number_p(SchemeObject* args, Environment* env);
SchemeObject* builtin_string_p(SchemeObject* args, Environment* env);
SchemeObject* builtin_symbol_p(SchemeObject* args, Environment* env);
SchemeObject* builtin_boolean_p(SchemeObject* args, Environment* env);
SchemeObject* builtin_procedure_p(SchemeObject* args, Environment* env);

// String operations
SchemeObject* builtin_string_length(SchemeObject* args, Environment* env);
SchemeObject* builtin_string_ref(SchemeObject* args, Environment* env);
SchemeObject* builtin_string_append(SchemeObject* args, Environment* env);
SchemeObject* builtin_string_to_symbol(SchemeObject* args, Environment* env);
SchemeObject* builtin_symbol_to_string(SchemeObject* args, Environment* env);
SchemeObject* builtin_number_to_string(SchemeObject* args, Environment* env);
SchemeObject* builtin_string_to_number(SchemeObject* args, Environment* env);

// Vector operations
SchemeObject* builtin_make_vector(SchemeObject* args, Environment* env);
SchemeObject* builtin_vector(SchemeObject* args, Environment* env);
SchemeObject* builtin_vector_length(SchemeObject* args, Environment* env);
SchemeObject* builtin_vector_ref(SchemeObject* args, Environment* env);
SchemeObject* builtin_vector_set(SchemeObject* args, Environment* env);

// I/O operations
SchemeObject* builtin_display(SchemeObject* args, Environment* env);
SchemeObject* builtin_newline(SchemeObject* args, Environment* env);
SchemeObject* builtin_write(SchemeObject* args, Environment* env);
SchemeObject* builtin_read(SchemeObject* args, Environment* env);

// Control flow
SchemeObject* builtin_apply(SchemeObject* args, Environment* env);
SchemeObject* builtin_map(SchemeObject* args, Environment* env);
SchemeObject* builtin_for_each(SchemeObject* args, Environment* env);

// Logical operations
SchemeObject* builtin_not(SchemeObject* args, Environment* env);

// Utility functions
int count_args(SchemeObject* args);
SchemeObject* get_arg(SchemeObject* args, int index);
bool check_arity(SchemeObject* args, int expected);
bool check_min_arity(SchemeObject* args, int min);

#endif // BUILTINS_H