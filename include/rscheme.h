#ifndef RSCHEME_H
#define RSCHEME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

// Forward declarations
typedef struct SchemeObject SchemeObject;
typedef struct Environment Environment;
typedef struct Token Token;
typedef struct Parser Parser;

// Version information
#define RSCHEME_VERSION_MAJOR 1
#define RSCHEME_VERSION_MINOR 0
#define RSCHEME_VERSION_PATCH 0

// Include all component headers
#include "scheme_objects.h"
#include "environment.h"
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "compiler.h"
#include "builtins.h"
#include "runtime.h"

// Main application modes
typedef enum {
    MODE_REPL,      // Read-Eval-Print Loop
    MODE_COMPILE,   // Compile to C
    MODE_RUN_FILE   // Run file directly
} ApplicationMode;

// Application context
typedef struct {
    ApplicationMode mode;
    const char* input_file;
    const char* output_file;
    bool verbose;
    bool optimize;
    Environment* global_env;
} AppContext;

// Function prototypes
int rscheme_main(int argc, char* argv[]);
void print_version(void);
void print_usage(const char* program_name);
AppContext* create_app_context(void);
void destroy_app_context(AppContext* ctx);

#endif // RSCHEME_H