#include "rscheme.h"
#include <stdarg.h>

// Global runtime state
static struct {
    bool initialized;
    bool debug_mode;
    size_t allocated_memory;
    size_t object_count;
    bool gc_enabled;
    SchemeObject** gc_roots;
    size_t gc_root_count;
    size_t gc_root_capacity;
} runtime_state = {0};

void init_runtime(void) {
    if (runtime_state.initialized) {
        return;
    }
    
    runtime_state.initialized = true;
    runtime_state.debug_mode = false;
    runtime_state.allocated_memory = 0;
    runtime_state.object_count = 0;
    runtime_state.gc_enabled = true;
    runtime_state.gc_roots = NULL;
    runtime_state.gc_root_count = 0;
    runtime_state.gc_root_capacity = 0;
    
    gc_init();
}

void cleanup_runtime(void) {
    if (!runtime_state.initialized) {
        return;
    }
    
    gc_cleanup();
    
    if (runtime_state.gc_roots) {
        free(runtime_state.gc_roots);
        runtime_state.gc_roots = NULL;
    }
    
    runtime_state.initialized = false;
}

void* scheme_malloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        runtime_error("Out of memory: failed to allocate %zu bytes", size);
        exit(EXIT_FAILURE);
    }
    runtime_state.allocated_memory += size;
    return ptr;
}

void* scheme_realloc(void* ptr, size_t size) {
    void* new_ptr = realloc(ptr, size);
    if (!new_ptr && size > 0) {
        runtime_error("Out of memory: failed to reallocate %zu bytes", size);
        exit(EXIT_FAILURE);
    }
    runtime_state.allocated_memory += size;
    return new_ptr;
}

void scheme_free(void* ptr) {
    if (ptr) {
        free(ptr);
    }
}

char* scheme_strdup(const char* str) {
    if (!str) {
        return NULL;
    }
    size_t len = strlen(str) + 1;
    char* copy = (char*)scheme_malloc(len);
    memcpy(copy, str, len);
    return copy;
}

size_t get_allocated_memory(void) {
    return runtime_state.allocated_memory;
}

size_t get_object_count(void) {
    return runtime_state.object_count;
}

void print_memory_stats(FILE* out) {
    fprintf(out, "Memory Statistics:\n");
    fprintf(out, "  Allocated memory: %zu bytes\n", runtime_state.allocated_memory);
    fprintf(out, "  Object count: %zu\n", runtime_state.object_count);
    fprintf(out, "  GC enabled: %s\n", runtime_state.gc_enabled ? "yes" : "no");
    fprintf(out, "  GC roots: %zu\n", runtime_state.gc_root_count);
}

void gc_init(void) {
    runtime_state.gc_root_capacity = 16;
    runtime_state.gc_roots = (SchemeObject**)scheme_malloc(
        runtime_state.gc_root_capacity * sizeof(SchemeObject*));
    runtime_state.gc_root_count = 0;
}

void gc_cleanup(void) {
    if (runtime_state.gc_roots) {
        scheme_free(runtime_state.gc_roots);
        runtime_state.gc_roots = NULL;
    }
    runtime_state.gc_root_count = 0;
    runtime_state.gc_root_capacity = 0;
}

void gc_add_root(SchemeObject** root) {
    if (runtime_state.gc_root_count >= runtime_state.gc_root_capacity) {
        runtime_state.gc_root_capacity *= 2;
        runtime_state.gc_roots = (SchemeObject**)scheme_realloc(
            runtime_state.gc_roots,
            runtime_state.gc_root_capacity * sizeof(SchemeObject*));
    }
    runtime_state.gc_roots[runtime_state.gc_root_count++] = root;
}

void gc_remove_root(SchemeObject** root) {
    for (size_t i = 0; i < runtime_state.gc_root_count; i++) {
        if (runtime_state.gc_roots[i] == root) {
            // Move last element to this position
            runtime_state.gc_roots[i] = 
                runtime_state.gc_roots[runtime_state.gc_root_count - 1];
            runtime_state.gc_root_count--;
            break;
        }
    }
}

void gc_run(void) {
    if (!runtime_state.gc_enabled) {
        return;
    }
    
    // debug_print("Running garbage collection...\n");
    
    // Mark phase - simplified for now
    /* 
    for (size_t i = 0; i < runtime_state.gc_root_count; i++) {
        if (runtime_state.gc_roots[i] && *runtime_state.gc_roots[i]) {
            mark_object(*runtime_state.gc_roots[i]);
        }
    }
    */
    
    // Sweep phase
    sweep_objects();
    
    // debug_print("Garbage collection completed\n");
}

void gc_enable(void) {
    runtime_state.gc_enabled = true;
}

void gc_disable(void) {
    runtime_state.gc_enabled = false;
}

bool gc_is_enabled(void) {
    return runtime_state.gc_enabled;
}

void runtime_error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "Runtime Error: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void runtime_warning(const char* format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "Warning: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void set_debug_mode(bool enabled) {
    runtime_state.debug_mode = enabled;
}

bool is_debug_mode(void) {
    return runtime_state.debug_mode;
}

void debug_print(const char* format, ...) {
    if (runtime_state.debug_mode) {
        va_list args;
        va_start(args, format);
        fprintf(stderr, "DEBUG: ");
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

// Function to create procedure objects for compiled lambdas
SchemeObject* make_compiled_procedure(SchemeObject* (*func)(SchemeObject**, int), int arity, const char* name) {
    // Create a basic procedure object and set it up for compiled use
    SchemeObject* proc = make_procedure(NULL, NULL, NULL);  // Use existing constructor
    proc->value.procedure.func = func;
    proc->value.procedure.arity = arity;
    proc->value.procedure.name = _strdup(name);
    return proc;
}