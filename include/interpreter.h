#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "scheme_objects.h"
#include "environment.h"

// Evaluation functions
SchemeObject* eval_expression(SchemeObject* expr, Environment* env);
SchemeObject* eval_sequence(SchemeObject* exprs, Environment* env);

// Special forms
SchemeObject* eval_if(SchemeObject* args, Environment* env);
SchemeObject* eval_cond(SchemeObject* args, Environment* env);
SchemeObject* eval_and(SchemeObject* args, Environment* env);
SchemeObject* eval_or(SchemeObject* args, Environment* env);
SchemeObject* eval_let(SchemeObject* args, Environment* env);
SchemeObject* eval_let_star(SchemeObject* args, Environment* env);
SchemeObject* eval_letrec(SchemeObject* args, Environment* env);
SchemeObject* eval_define(SchemeObject* args, Environment* env);
SchemeObject* eval_set(SchemeObject* args, Environment* env);
SchemeObject* eval_lambda(SchemeObject* args, Environment* env);
SchemeObject* eval_quote(SchemeObject* args, Environment* env);
SchemeObject* eval_quasiquote(SchemeObject* args, Environment* env);
SchemeObject* eval_unquote(SchemeObject* args, Environment* env);
SchemeObject* eval_begin(SchemeObject* args, Environment* env);

// Application
SchemeObject* apply_procedure(SchemeObject* proc, SchemeObject* args, Environment* env);

// Type checking for special forms
bool is_special_form(SchemeObject* expr);
SchemeObject* get_special_form_handler(const char* name);

// Utilities
bool is_self_evaluating(SchemeObject* expr);
bool is_variable(SchemeObject* expr);
bool is_application(SchemeObject* expr);

// REPL support
void run_repl(Environment* env);
SchemeObject* read_expression(FILE* input);
void print_result(SchemeObject* result, FILE* output);

// File execution
SchemeObject* load_file(const char* filename, Environment* env);
SchemeObject* eval_file(const char* filename, Environment* env);

// Error handling
typedef enum {
    EVAL_OK,
    EVAL_ERROR_UNBOUND_VARIABLE,
    EVAL_ERROR_WRONG_TYPE,
    EVAL_ERROR_WRONG_ARITY,
    EVAL_ERROR_INVALID_SYNTAX,
    EVAL_ERROR_DIVISION_BY_ZERO,
    EVAL_ERROR_FILE_NOT_FOUND,
    EVAL_ERROR_RUNTIME
} EvalError;

void set_eval_error(EvalError error, const char* message);
void print_eval_error(FILE* out);
bool has_eval_error(void);
void clear_eval_error(void);

#endif // INTERPRETER_H