#ifndef SCHEME_OBJECTS_H
#define SCHEME_OBJECTS_H

#include <stdint.h>
#include <stdbool.h>

// Scheme object types
typedef enum {
    SCHEME_NIL,
    SCHEME_BOOLEAN,
    SCHEME_NUMBER,
    SCHEME_SYMBOL,
    SCHEME_STRING,
    SCHEME_PAIR,
    SCHEME_PROCEDURE,
    SCHEME_PRIMITIVE,
    SCHEME_VECTOR,
    SCHEME_PORT
} SchemeType;

// Forward declaration for circular reference
typedef struct SchemeObject SchemeObject;
typedef struct Environment Environment;

// Primitive function type
typedef SchemeObject* (*PrimitiveFn)(SchemeObject* args, Environment* env);

// Procedure representation
typedef struct {
    SchemeObject* parameters;  // List of parameter symbols
    SchemeObject* body;        // List of expressions
    Environment* closure;      // Captured environment
} SchemeProcedure;

// Vector representation
typedef struct {
    size_t length;
    SchemeObject** elements;
} SchemeVector;

// Port representation (for I/O)
typedef struct {
    FILE* file;
    bool is_input;
    bool is_output;
    char* filename;
} SchemePort;

// Pair (cons cell) representation
typedef struct {
    SchemeObject* car;
    SchemeObject* cdr;
} SchemePair;

// Main Scheme object structure
struct SchemeObject {
    SchemeType type;
    union {
        bool boolean_value;
        double number_value;
        char* symbol_name;
        char* string_value;
        SchemePair pair;
        SchemeProcedure procedure;
        PrimitiveFn primitive;
        SchemeVector vector;
        SchemePort port;
    } value;
    
    // Reference counting for garbage collection
    int ref_count;
    
    // Mark for mark-and-sweep GC
    bool marked;
};

// Object creation functions
SchemeObject* make_nil(void);
SchemeObject* make_boolean(bool value);
SchemeObject* make_number(double value);
SchemeObject* make_symbol(const char* name);
SchemeObject* make_string(const char* str);
SchemeObject* make_pair(SchemeObject* car, SchemeObject* cdr);
SchemeObject* make_procedure(SchemeObject* params, SchemeObject* body, Environment* env);
SchemeObject* make_primitive(PrimitiveFn fn);
SchemeObject* make_vector(size_t length);
SchemeObject* make_port(FILE* file, bool is_input, bool is_output, const char* filename);

// Object manipulation functions
SchemeObject* cons(SchemeObject* car, SchemeObject* cdr);
SchemeObject* car(SchemeObject* pair);
SchemeObject* cdr(SchemeObject* pair);
void set_car(SchemeObject* pair, SchemeObject* value);
void set_cdr(SchemeObject* pair, SchemeObject* value);

// Type checking functions
bool is_nil(SchemeObject* obj);
bool is_boolean(SchemeObject* obj);
bool is_number(SchemeObject* obj);
bool is_symbol(SchemeObject* obj);
bool is_string(SchemeObject* obj);
bool is_pair(SchemeObject* obj);
bool is_procedure(SchemeObject* obj);
bool is_primitive(SchemeObject* obj);
bool is_vector(SchemeObject* obj);
bool is_port(SchemeObject* obj);
bool is_list(SchemeObject* obj);
bool is_null(SchemeObject* obj);

// List operations
size_t list_length(SchemeObject* list);
SchemeObject* list_ref(SchemeObject* list, size_t index);
SchemeObject* append_lists(SchemeObject* list1, SchemeObject* list2);
SchemeObject* reverse_list(SchemeObject* list);

// Object comparison
bool scheme_equal(SchemeObject* a, SchemeObject* b);
bool scheme_eqv(SchemeObject* a, SchemeObject* b);
bool scheme_eq(SchemeObject* a, SchemeObject* b);

// Memory management
void retain_object(SchemeObject* obj);
void release_object(SchemeObject* obj);
void mark_object(SchemeObject* obj);
void sweep_objects(void);
void gc_collect(void);

// String representation
char* object_to_string(SchemeObject* obj);
void print_object(SchemeObject* obj, FILE* out);

// Built-in constants
extern SchemeObject* SCHEME_NIL_OBJECT;
extern SchemeObject* SCHEME_TRUE_OBJECT;
extern SchemeObject* SCHEME_FALSE_OBJECT;

// Initialization
void init_scheme_objects(void);
void cleanup_scheme_objects(void);

#endif // SCHEME_OBJECTS_H