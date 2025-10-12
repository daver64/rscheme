#include "rscheme.h"
#include <stdarg.h>

// Global objects
SchemeObject* SCHEME_NIL_OBJECT = NULL;
SchemeObject* SCHEME_TRUE_OBJECT = NULL;
SchemeObject* SCHEME_FALSE_OBJECT = NULL;

// Object allocation tracking
static SchemeObject* all_objects = NULL;
static size_t object_count = 0;

static SchemeObject* allocate_object(SchemeType type) {
    SchemeObject* obj = (SchemeObject*)scheme_malloc(sizeof(SchemeObject));
    obj->type = type;
    obj->ref_count = 1;
    obj->marked = false;
    
    // Add to global object list for GC (using a simple linked list)
    // We'll store the next pointer separately to avoid corrupting the union
    static SchemeObject** object_list = NULL;
    static size_t object_list_capacity = 0;
    
    if (object_count >= object_list_capacity) {
        object_list_capacity = object_list_capacity ? object_list_capacity * 2 : 16;
        object_list = (SchemeObject**)scheme_realloc(object_list, 
            object_list_capacity * sizeof(SchemeObject*));
    }
    
    object_list[object_count] = obj;
    object_count++;
    
    return obj;
}

void init_scheme_objects(void) {
    if (SCHEME_NIL_OBJECT) {
        return; // Already initialized
    }
    
    SCHEME_NIL_OBJECT = allocate_object(SCHEME_NIL);
    SCHEME_NIL_OBJECT->ref_count = 1000; // Never collect these
    
    SCHEME_TRUE_OBJECT = allocate_object(SCHEME_BOOLEAN);
    SCHEME_TRUE_OBJECT->value.boolean_value = true;
    SCHEME_TRUE_OBJECT->ref_count = 1000;
    
    SCHEME_FALSE_OBJECT = allocate_object(SCHEME_BOOLEAN);
    SCHEME_FALSE_OBJECT->value.boolean_value = false;
    SCHEME_FALSE_OBJECT->ref_count = 1000;
}

void cleanup_scheme_objects(void) {
    // For now, we'll skip the complex cleanup since we have a simpler allocation system
    object_count = 0;
    
    SCHEME_NIL_OBJECT = NULL;
    SCHEME_TRUE_OBJECT = NULL;
    SCHEME_FALSE_OBJECT = NULL;
}

SchemeObject* make_nil(void) {
    return SCHEME_NIL_OBJECT;
}

SchemeObject* make_boolean(bool value) {
    return value ? SCHEME_TRUE_OBJECT : SCHEME_FALSE_OBJECT;
}

SchemeObject* make_number(double value) {
    SchemeObject* obj = allocate_object(SCHEME_NUMBER);
    obj->value.number_value = value;
    return obj;
}

SchemeObject* make_symbol(const char* name) {
    SchemeObject* obj = allocate_object(SCHEME_SYMBOL);
    obj->value.symbol_name = scheme_strdup(name);
    return obj;
}

SchemeObject* make_string(const char* str) {
    SchemeObject* obj = allocate_object(SCHEME_STRING);
    obj->value.string_value = scheme_strdup(str);
    return obj;
}

SchemeObject* make_pair(SchemeObject* car, SchemeObject* cdr) {
    SchemeObject* obj = allocate_object(SCHEME_PAIR);
    obj->value.pair.car = car;
    obj->value.pair.cdr = cdr;
    if (car) retain_object(car);
    if (cdr) retain_object(cdr);
    return obj;
}

SchemeObject* make_procedure(SchemeObject* params, SchemeObject* body, Environment* env) {
    SchemeObject* obj = allocate_object(SCHEME_PROCEDURE);
    obj->value.procedure.parameters = params;
    obj->value.procedure.body = body;
    obj->value.procedure.closure = env;
    if (params) retain_object(params);
    if (body) retain_object(body);
    if (env) retain_environment(env);
    return obj;
}

SchemeObject* make_primitive(PrimitiveFn fn) {
    SchemeObject* obj = allocate_object(SCHEME_PRIMITIVE);
    obj->value.primitive = fn;
    return obj;
}

SchemeObject* make_vector(size_t length) {
    SchemeObject* obj = allocate_object(SCHEME_VECTOR);
    obj->value.vector.length = length;
    obj->value.vector.elements = (SchemeObject**)scheme_malloc(length * sizeof(SchemeObject*));
    for (size_t i = 0; i < length; i++) {
        obj->value.vector.elements[i] = SCHEME_NIL_OBJECT;
    }
    return obj;
}

SchemeObject* make_port(FILE* file, bool is_input, bool is_output, const char* filename) {
    SchemeObject* obj = allocate_object(SCHEME_PORT);
    obj->value.port.file = file;
    obj->value.port.is_input = is_input;
    obj->value.port.is_output = is_output;
    obj->value.port.filename = filename ? scheme_strdup(filename) : NULL;
    return obj;
}

SchemeObject* cons(SchemeObject* car, SchemeObject* cdr) {
    return make_pair(car, cdr);
}

SchemeObject* car(SchemeObject* pair) {
    if (!pair || pair->type != SCHEME_PAIR) {
        return SCHEME_NIL_OBJECT;
    }
    return pair->value.pair.car;
}

SchemeObject* cdr(SchemeObject* pair) {
    if (!pair || pair->type != SCHEME_PAIR) {
        return SCHEME_NIL_OBJECT;
    }
    return pair->value.pair.cdr;
}

void set_car(SchemeObject* pair, SchemeObject* value) {
    if (pair && pair->type == SCHEME_PAIR) {
        if (pair->value.pair.car) {
            release_object(pair->value.pair.car);
        }
        pair->value.pair.car = value;
        if (value) {
            retain_object(value);
        }
    }
}

void set_cdr(SchemeObject* pair, SchemeObject* value) {
    if (pair && pair->type == SCHEME_PAIR) {
        if (pair->value.pair.cdr) {
            release_object(pair->value.pair.cdr);
        }
        pair->value.pair.cdr = value;
        if (value) {
            retain_object(value);
        }
    }
}

// Type checking functions
bool is_nil(SchemeObject* obj) {
    return obj == SCHEME_NIL_OBJECT || (obj && obj->type == SCHEME_NIL);
}

bool is_boolean(SchemeObject* obj) {
    return obj && obj->type == SCHEME_BOOLEAN;
}

bool is_number(SchemeObject* obj) {
    return obj && obj->type == SCHEME_NUMBER;
}

bool is_symbol(SchemeObject* obj) {
    return obj && obj->type == SCHEME_SYMBOL;
}

bool is_string(SchemeObject* obj) {
    return obj && obj->type == SCHEME_STRING;
}

bool is_pair(SchemeObject* obj) {
    return obj && obj->type == SCHEME_PAIR;
}

bool is_procedure(SchemeObject* obj) {
    return obj && obj->type == SCHEME_PROCEDURE;
}

bool is_primitive(SchemeObject* obj) {
    return obj && obj->type == SCHEME_PRIMITIVE;
}

bool is_vector(SchemeObject* obj) {
    return obj && obj->type == SCHEME_VECTOR;
}

bool is_port(SchemeObject* obj) {
    return obj && obj->type == SCHEME_PORT;
}

bool is_list(SchemeObject* obj) {
    while (obj && is_pair(obj)) {
        obj = cdr(obj);
    }
    return is_nil(obj);
}

bool is_null(SchemeObject* obj) {
    return is_nil(obj);
}

size_t list_length(SchemeObject* list) {
    size_t length = 0;
    while (list && is_pair(list)) {
        length++;
        list = cdr(list);
    }
    return length;
}

SchemeObject* list_ref(SchemeObject* list, size_t index) {
    for (size_t i = 0; i < index && list && is_pair(list); i++) {
        list = cdr(list);
    }
    return list && is_pair(list) ? car(list) : SCHEME_NIL_OBJECT;
}

bool scheme_equal(SchemeObject* a, SchemeObject* b) {
    if (a == b) return true;
    if (!a || !b || a->type != b->type) return false;
    
    switch (a->type) {
        case SCHEME_NIL:
            return true;
        case SCHEME_BOOLEAN:
            return a->value.boolean_value == b->value.boolean_value;
        case SCHEME_NUMBER:
            return a->value.number_value == b->value.number_value;
        case SCHEME_SYMBOL:
            return strcmp(a->value.symbol_name, b->value.symbol_name) == 0;
        case SCHEME_STRING:
            return strcmp(a->value.string_value, b->value.string_value) == 0;
        case SCHEME_PAIR:
            return scheme_equal(car(a), car(b)) && scheme_equal(cdr(a), cdr(b));
        default:
            return false;
    }
}

bool scheme_eqv(SchemeObject* a, SchemeObject* b) {
    if (a == b) return true;
    if (!a || !b || a->type != b->type) return false;
    
    switch (a->type) {
        case SCHEME_NIL:
        case SCHEME_BOOLEAN:
        case SCHEME_NUMBER:
        case SCHEME_SYMBOL:
            return scheme_equal(a, b);
        default:
            return a == b;
    }
}

bool scheme_eq(SchemeObject* a, SchemeObject* b) {
    return a == b;
}

void retain_object(SchemeObject* obj) {
    if (obj) {
        obj->ref_count++;
    }
}

void release_object(SchemeObject* obj) {
    if (obj && --obj->ref_count <= 0) {
        // Object can be collected
        obj->marked = false;
    }
}

void mark_object(SchemeObject* obj) {
    if (!obj || obj->marked) {
        return;
    }
    
    obj->marked = true;
    
    switch (obj->type) {
        case SCHEME_PAIR:
            mark_object(obj->value.pair.car);
            mark_object(obj->value.pair.cdr);
            break;
        case SCHEME_PROCEDURE:
            mark_object(obj->value.procedure.parameters);
            mark_object(obj->value.procedure.body);
            break;
        case SCHEME_VECTOR:
            for (size_t i = 0; i < obj->value.vector.length; i++) {
                mark_object(obj->value.vector.elements[i]);
            }
            break;
        default:
            break;
    }
}

void sweep_objects(void) {
    // Simplified sweep - in a full implementation this would 
    // actually free unreferenced objects
    // debug_print("Sweep phase - simplified implementation\n");
}

void gc_collect(void) {
    gc_run();
}

char* object_to_string(SchemeObject* obj) {
    if (!obj) {
        return scheme_strdup("null");
    }
    
    char* buffer = (char*)scheme_malloc(1024);
    
    switch (obj->type) {
        case SCHEME_NIL:
            strcpy(buffer, "()");
            break;
        case SCHEME_BOOLEAN:
            strcpy(buffer, obj->value.boolean_value ? "#t" : "#f");
            break;
        case SCHEME_NUMBER:
            snprintf(buffer, 1024, "%.6g", obj->value.number_value);
            break;
        case SCHEME_SYMBOL:
            snprintf(buffer, 1024, "%s", obj->value.symbol_name);
            break;
        case SCHEME_STRING:
            snprintf(buffer, 1024, "\"%s\"", obj->value.string_value);
            break;
        case SCHEME_PAIR:
            strcpy(buffer, "(");
            // This is simplified - proper list printing would be more complex
            strcat(buffer, "...)");
            break;
        case SCHEME_PROCEDURE:
            strcpy(buffer, "#<procedure>");
            break;
        case SCHEME_PRIMITIVE:
            strcpy(buffer, "#<primitive>");
            break;
        case SCHEME_VECTOR:
            strcpy(buffer, "#(...)");
            break;
        case SCHEME_PORT:
            strcpy(buffer, "#<port>");
            break;
        default:
            strcpy(buffer, "#<unknown>");
            break;
    }
    
    return buffer;
}

void print_object(SchemeObject* obj, FILE* out) {
    char* str = object_to_string(obj);
    fprintf(out, "%s", str);
    scheme_free(str);
}