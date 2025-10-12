#ifndef RUNTIME_H
#define RUNTIME_H

#include "scheme_objects.h"

// Runtime initialization and cleanup
void init_runtime(void);
void cleanup_runtime(void);

// Memory management
void* scheme_malloc(size_t size);
void* scheme_realloc(void* ptr, size_t size);
void scheme_free(void* ptr);
char* scheme_strdup(const char* str);

// Memory statistics
size_t get_allocated_memory(void);
size_t get_object_count(void);
void print_memory_stats(FILE* out);

// Garbage collection
void gc_init(void);
void gc_cleanup(void);
void gc_add_root(SchemeObject** root);
void gc_remove_root(SchemeObject** root);
void gc_run(void);
void gc_enable(void);
void gc_disable(void);
bool gc_is_enabled(void);

// Error handling
void runtime_error(const char* format, ...);
void runtime_warning(const char* format, ...);

// Debugging support
void set_debug_mode(bool enabled);
bool is_debug_mode(void);
void debug_print(const char* format, ...);

// Procedure creation for compiled lambdas
SchemeObject* make_compiled_procedure(SchemeObject* (*func)(SchemeObject**, int), int arity, const char* name);

#endif // RUNTIME_H