#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "scheme_objects.h"

// Environment binding
typedef struct Binding {
    char* name;
    SchemeObject* value;
    struct Binding* next;
} Binding;

// Environment structure
typedef struct Environment {
    Binding* bindings;
    struct Environment* parent;
    int ref_count;
} Environment;

// Environment creation and destruction
Environment* make_environment(Environment* parent);
void retain_environment(Environment* env);
void release_environment(Environment* env);

// Variable operations
void define_variable(Environment* env, const char* name, SchemeObject* value);
void set_variable(Environment* env, const char* name, SchemeObject* value);
bool set_variable_if_exists(Environment* env, const char* name, SchemeObject* value);
SchemeObject* lookup_variable(Environment* env, const char* name);
bool is_bound(Environment* env, const char* name);

// Standard environment creation
Environment* make_global_environment(void);
Environment* extend_environment(Environment* base, SchemeObject* vars, SchemeObject* vals);

// Environment utilities
void print_environment(Environment* env, FILE* out);
size_t environment_size(Environment* env);

#endif // ENVIRONMENT_H