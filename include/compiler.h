#ifndef COMPILER_H
#define COMPILER_H

#include "scheme_objects.h"
#include "environment.h"
#include <stdio.h>

// Lambda function storage
typedef struct LambdaFunction {
    char* name;
    char* code;
    struct LambdaFunction* next;
} LambdaFunction;

// Compilation context
typedef struct CompilerContext {
    FILE* output;
    Environment* env;
    int indent_level;
    int temp_var_counter;
    int label_counter;
    int function_counter;
    bool optimize;
    char* current_function;
    LambdaFunction* lambda_functions;  // Linked list of lambda functions
    FILE* lambda_output;               // Temporary storage for lambda functions
} CompilerContext;

// Compiler functions
bool compile_to_c(SchemeObject* expr, const char* output_file, bool optimize);
bool compile_file(const char* input_file, const char* output_file, bool optimize);

// Code generation
void generate_c_header(CompilerContext* ctx);
void generate_c_footer(CompilerContext* ctx);
void generate_main_function(CompilerContext* ctx, SchemeObject* expr);

// Expression compilation
void compile_expression(SchemeObject* expr, CompilerContext* ctx);
void compile_literal(SchemeObject* obj, CompilerContext* ctx);
void compile_variable(SchemeObject* sym, CompilerContext* ctx);
void compile_if(SchemeObject* args, CompilerContext* ctx);
void compile_lambda(SchemeObject* args, CompilerContext* ctx);
void compile_application(SchemeObject* expr, CompilerContext* ctx);
void compile_define(SchemeObject* args, CompilerContext* ctx);
void compile_set(SchemeObject* args, CompilerContext* ctx);
void compile_begin(SchemeObject* args, CompilerContext* ctx);
void compile_quote(SchemeObject* args, CompilerContext* ctx);
void compile_arithmetic(SchemeObject* operands, CompilerContext* ctx, const char* op);
void compile_comparison(SchemeObject* operands, CompilerContext* ctx, const char* op);
void compile_cons(SchemeObject* operands, CompilerContext* ctx);
void compile_car_cdr(SchemeObject* operands, CompilerContext* ctx, const char* operation);
void compile_type_predicate(SchemeObject* operands, CompilerContext* ctx, const char* predicate);
void compile_list(SchemeObject* operands, CompilerContext* ctx);
void compile_single_arg_builtin(SchemeObject* operands, CompilerContext* ctx, const char* function);
void compile_two_arg_builtin(SchemeObject* operands, CompilerContext* ctx, const char* function);

// Runtime support generation
void generate_runtime_functions(CompilerContext* ctx);
void generate_object_functions(CompilerContext* ctx);
void generate_builtin_functions(CompilerContext* ctx);

// Utility functions
char* generate_temp_var(CompilerContext* ctx);
char* generate_label(CompilerContext* ctx);
char* generate_function_name(CompilerContext* ctx);
void emit_indent(CompilerContext* ctx);
void emit_line(CompilerContext* ctx, const char* format, ...);
void emit_comment(CompilerContext* ctx, const char* comment);
void emit_lambda_functions(CompilerContext* ctx);

// C code templates
void emit_object_creation(CompilerContext* ctx, SchemeObject* obj);
void emit_type_check(CompilerContext* ctx, const char* var, SchemeType type);
void emit_error_check(CompilerContext* ctx, const char* condition, const char* message);

// Optimization
void optimize_tail_calls(SchemeObject* expr);
void optimize_constants(SchemeObject* expr);
void optimize_dead_code(SchemeObject* expr);

// Context management
CompilerContext* create_compiler_context(FILE* output, Environment* env, bool optimize);
void destroy_compiler_context(CompilerContext* ctx);

#endif // COMPILER_H