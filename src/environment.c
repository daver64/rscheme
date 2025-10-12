#include "rscheme.h"

Environment* make_environment(Environment* parent) {
    Environment* env = (Environment*)scheme_malloc(sizeof(Environment));
    env->bindings = NULL;
    env->parent = parent;
    env->ref_count = 1;
    
    if (parent) {
        retain_environment(parent);
    }
    
    return env;
}

void retain_environment(Environment* env) {
    if (env) {
        env->ref_count++;
    }
}

void release_environment(Environment* env) {
    if (!env || --env->ref_count > 0) {
        return;
    }
    
    // Free all bindings
    Binding* current = env->bindings;
    while (current) {
        Binding* next = current->next;
        scheme_free(current->name);
        if (current->value) {
            release_object(current->value);
        }
        scheme_free(current);
        current = next;
    }
    
    // Release parent
    if (env->parent) {
        release_environment(env->parent);
    }
    
    scheme_free(env);
}

void define_variable(Environment* env, const char* name, SchemeObject* value) {
    if (!env || !name) {
        return;
    }
    
    // Check if already bound in this environment
    Binding* current = env->bindings;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            // Update existing binding
            if (current->value) {
                release_object(current->value);
            }
            current->value = value;
            if (value) {
                retain_object(value);
            }
            return;
        }
        current = current->next;
    }
    
    // Create new binding
    Binding* binding = (Binding*)scheme_malloc(sizeof(Binding));
    binding->name = scheme_strdup(name);
    binding->value = value;
    binding->next = env->bindings;
    
    if (value) {
        retain_object(value);
    }
    
    env->bindings = binding;
}

void set_variable(Environment* env, const char* name, SchemeObject* value) {
    if (!env || !name) {
        return;
    }
    
    // Search this environment and all parents
    Environment* current_env = env;
    while (current_env) {
        Binding* current = current_env->bindings;
        while (current) {
            if (strcmp(current->name, name) == 0) {
                // Found binding, update it
                if (current->value) {
                    release_object(current->value);
                }
                current->value = value;
                if (value) {
                    retain_object(value);
                }
                return;
            }
            current = current->next;
        }
        current_env = current_env->parent;
    }
    
    // Variable not found, define in current environment
    define_variable(env, name, value);
}

bool set_variable_if_exists(Environment* env, const char* name, SchemeObject* value) {
    if (!env || !name) {
        return false;
    }
    
    // Search this environment and all parents
    Environment* current_env = env;
    while (current_env) {
        Binding* current = current_env->bindings;
        while (current) {
            if (strcmp(current->name, name) == 0) {
                // Found binding, update it
                if (current->value) {
                    release_object(current->value);
                }
                current->value = value;
                if (value) {
                    retain_object(value);
                }
                return true;
            }
            current = current->next;
        }
        current_env = current_env->parent;
    }
    
    return false; // Variable not found
}

SchemeObject* lookup_variable(Environment* env, const char* name) {
    if (!env || !name) {
        return NULL;
    }
    
    // Search this environment and all parents
    Environment* current_env = env;
    while (current_env) {
        Binding* current = current_env->bindings;
        while (current) {
            if (strcmp(current->name, name) == 0) {
                return current->value;
            }
            current = current->next;
        }
        current_env = current_env->parent;
    }
    
    return NULL; // Not found
}

bool is_bound(Environment* env, const char* name) {
    return lookup_variable(env, name) != NULL;
}

Environment* make_global_environment(void) {
    Environment* env = make_environment(NULL);
    init_builtins(env);
    return env;
}

Environment* extend_environment(Environment* base, SchemeObject* vars, SchemeObject* vals) {
    Environment* env = make_environment(base);
    
    // Bind variables to values
    SchemeObject* var_list = vars;
    SchemeObject* val_list = vals;
    
    while (var_list && is_pair(var_list) && val_list && is_pair(val_list)) {
        SchemeObject* var = car(var_list);
        SchemeObject* val = car(val_list);
        
        if (is_symbol(var)) {
            define_variable(env, var->value.symbol_name, val);
        }
        
        var_list = cdr(var_list);
        val_list = cdr(val_list);
    }
    
    // Handle dotted parameter lists (for variadic functions)
    if (var_list && is_symbol(var_list)) {
        define_variable(env, var_list->value.symbol_name, val_list);
    }
    
    return env;
}

void print_environment(Environment* env, FILE* out) {
    if (!env) {
        fprintf(out, "Environment: null\n");
        return;
    }
    
    fprintf(out, "Environment:\n");
    
    Binding* current = env->bindings;
    while (current) {
        fprintf(out, "  %s = ", current->name);
        print_object(current->value, out);
        fprintf(out, "\n");
        current = current->next;
    }
    
    if (env->parent) {
        fprintf(out, "Parent ");
        print_environment(env->parent, out);
    }
}

size_t environment_size(Environment* env) {
    if (!env) {
        return 0;
    }
    
    size_t count = 0;
    Binding* current = env->bindings;
    while (current) {
        count++;
        current = current->next;
    }
    
    return count;
}