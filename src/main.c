#include "rscheme.h"

void print_version(void) {
    printf("RScheme %d.%d.%d - R5RS Scheme Implementation\n",
           RSCHEME_VERSION_MAJOR,
           RSCHEME_VERSION_MINOR,
           RSCHEME_VERSION_PATCH);
    printf("A Scheme to C compiler with interpreter capabilities\n");
}

void print_usage(const char* program_name) {
    printf("Usage: %s [options] [file]\n", program_name);
    printf("\nOptions:\n");
    printf("  -h, --help         Show this help message\n");
    printf("  -v, --version      Show version information\n");
    printf("  -i, --repl         Start interactive REPL (default if no file)\n");
    printf("  -c, --compile FILE Compile Scheme file to C\n");
    printf("  -o, --output FILE  Specify output file for compilation\n");
    printf("  -O, --optimize     Enable optimizations\n");
    printf("  --verbose          Enable verbose output\n");
    printf("  --debug            Enable debug mode\n");
    printf("\nExamples:\n");
    printf("  %s                    # Start REPL\n", program_name);
    printf("  %s program.scm        # Run Scheme file\n", program_name);
    printf("  %s -c program.scm     # Compile to C (output: program.c)\n", program_name);
    printf("  %s -c program.scm -o output.c  # Compile with custom output\n", program_name);
}

AppContext* create_app_context(void) {
    AppContext* ctx = (AppContext*)scheme_malloc(sizeof(AppContext));
    ctx->mode = MODE_REPL;
    ctx->input_file = NULL;
    ctx->output_file = NULL;
    ctx->verbose = false;
    ctx->optimize = false;
    ctx->global_env = NULL;
    return ctx;
}

void destroy_app_context(AppContext* ctx) {
    if (ctx) {
        if (ctx->global_env) {
            release_environment(ctx->global_env);
        }
        scheme_free(ctx);
    }
}

static bool parse_arguments(int argc, char* argv[], AppContext* ctx) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return false;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            return false;
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--repl") == 0) {
            ctx->mode = MODE_REPL;
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--compile") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: -c option requires a filename\n");
                return false;
            }
            ctx->mode = MODE_COMPILE;
            ctx->input_file = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: -o option requires a filename\n");
                return false;
            }
            ctx->output_file = argv[++i];
        } else if (strcmp(argv[i], "-O") == 0 || strcmp(argv[i], "--optimize") == 0) {
            ctx->optimize = true;
        } else if (strcmp(argv[i], "--verbose") == 0) {
            ctx->verbose = true;
        } else if (strcmp(argv[i], "--debug") == 0) {
            set_debug_mode(true);
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Unknown option: %s\n", argv[i]);
            return false;
        } else {
            // Input file
            if (ctx->input_file) {
                fprintf(stderr, "Error: Multiple input files specified\n");
                return false;
            }
            ctx->input_file = argv[i];
            if (ctx->mode == MODE_REPL) {
                ctx->mode = MODE_RUN_FILE;
            }
        }
    }
    
    return true;
}

static void run_interpreter_mode(AppContext* ctx) {
    if (ctx->verbose) {
        printf("Starting interpreter mode...\n");
    }
    
    if (ctx->input_file) {
        // Load and execute file
        FILE* file = fopen(ctx->input_file, "rb");  // Open in binary mode
        if (!file) {
            fprintf(stderr, "Error: Cannot open file: %s\n", ctx->input_file);
            return;
        }
        
        // Read file content
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        char* content = (char*)scheme_malloc(length + 1);
        size_t bytes_read = fread(content, 1, length, file);
        content[bytes_read] = '\0';
        fclose(file);
        
        // Parse and evaluate
        Parser* parser = create_parser(content);
        
        while (true) {
            clear_eval_error();
            SchemeObject* expr = parse_expression(parser);
            
            if (has_parse_error(parser)) {
                print_parse_error(parser, stderr);
                break;
            }
            
            if (!expr) {
                break; // End of input
            }
            
            SchemeObject* result = eval_expression(expr, ctx->global_env);
            
            if (has_eval_error()) {
                print_eval_error(stderr);
                break;
            }
            
            if (ctx->verbose && result) {
                printf("Result: ");
                print_object(result, stdout);
                printf("\n");
            }
        }
        
        destroy_parser(parser);
        scheme_free(content);
        
        // Clear evaluation error state after file execution
        clear_eval_error();
    } else {
        // Start REPL
        run_repl(ctx->global_env);
    }
}

static void run_compiler_mode(AppContext* ctx) {
    if (!ctx->input_file) {
        fprintf(stderr, "Error: No input file specified for compilation\n");
        return;
    }
    
    if (ctx->verbose) {
        printf("Compiling %s to C...\n", ctx->input_file);
    }
    
    // Generate output filename if not specified
    char* output_file = NULL;
    bool allocated_output = false;
    
    if (ctx->output_file) {
        // Check if output file has .c extension, add it if not
        const char* ext = strrchr(ctx->output_file, '.');
        if (!ext || strcmp(ext, ".c") != 0) {
            size_t len = strlen(ctx->output_file);
            output_file = (char*)scheme_malloc(len + 3); // +2 for ".c" +1 for null
            strcpy(output_file, ctx->output_file);
            strcpy(output_file + len, ".c");
            allocated_output = true;
        } else {
            output_file = (char*)ctx->output_file;
        }
    } else {
        // Replace .scm extension with .c
        const char* ext = strrchr(ctx->input_file, '.');
        size_t base_len = ext ? (size_t)(ext - ctx->input_file) : strlen(ctx->input_file);
        
        output_file = (char*)scheme_malloc(base_len + 3); // +2 for ".c" +1 for null
        strncpy(output_file, ctx->input_file, base_len);
        strcpy(output_file + base_len, ".c");
        allocated_output = true;
    }
    
    bool success = compile_file(ctx->input_file, output_file, ctx->optimize);
    
    if (success) {
        printf("Successfully compiled to: %s\n", output_file);
        
        // Automatically compile the C code to executable
        char* exe_file = NULL;
        bool allocated_exe = false;
        
        // Generate executable filename
        const char* c_ext = strrchr(output_file, '.');
        size_t exe_base_len = c_ext ? (size_t)(c_ext - output_file) : strlen(output_file);
        
        #ifdef _WIN32
            exe_file = (char*)scheme_malloc(exe_base_len + 5); // +4 for ".exe" +1 for null
            strncpy(exe_file, output_file, exe_base_len);
            strcpy(exe_file + exe_base_len, ".exe");
        #else
            exe_file = (char*)scheme_malloc(exe_base_len + 1); // +1 for null
            strncpy(exe_file, output_file, exe_base_len);
            exe_file[exe_base_len] = '\0';
        #endif
        allocated_exe = true;
        
        // Build C compiler command
        char compile_cmd[1024];
        #ifdef _WIN32
            // Try cl (Microsoft C compiler) first
            snprintf(compile_cmd, sizeof(compile_cmd), "cl /nologo %s /Fe:%s", output_file, exe_file);
        #else
            // Use gcc on Unix-like systems
            snprintf(compile_cmd, sizeof(compile_cmd), "gcc -o %s %s -lm", exe_file, output_file);
        #endif
        
        if (ctx->verbose) {
            printf("Running C compiler: %s\n", compile_cmd);
        }
        
        int compile_result = system(compile_cmd);
        
        if (compile_result == 0) {
            printf("Successfully built executable: %s\n", exe_file);
            
            // Clean up intermediate .obj file on Windows
            #ifdef _WIN32
                char obj_file[1024];
                snprintf(obj_file, sizeof(obj_file), "%.*s.obj", (int)exe_base_len, output_file);
                remove(obj_file);
            #endif
        } else {
            fprintf(stderr, "C compilation failed. You can try manually with:\n");
            #ifdef _WIN32
                fprintf(stderr, "  cl %s\n", output_file);
            #else
                fprintf(stderr, "  gcc -o %s %s -lm\n", exe_file, output_file);
            #endif
        }
        
        if (allocated_exe) {
            scheme_free(exe_file);
        }
    } else {
        fprintf(stderr, "Compilation failed\n");
    }
    
    if (allocated_output) {
        scheme_free(output_file);
    }
}

int rscheme_main(int argc, char* argv[]) {
    // Initialize runtime
    init_runtime();
    init_scheme_objects();
    
    AppContext* ctx = create_app_context();
    
    if (!parse_arguments(argc, argv, ctx)) {
        destroy_app_context(ctx);
        cleanup_scheme_objects();
        cleanup_runtime();
        return argc > 1 ? 0 : 1; // Return 0 for help/version, 1 for errors
    }
    
    // Create global environment
    ctx->global_env = make_global_environment();
    
    if (ctx->verbose) {
        print_version();
        printf("\n");
    }
    
    // Run according to mode
    switch (ctx->mode) {
        case MODE_REPL:
        case MODE_RUN_FILE:
            run_interpreter_mode(ctx);
            break;
        case MODE_COMPILE:
            run_compiler_mode(ctx);
            break;
    }
    
    // Cleanup
    destroy_app_context(ctx);
    cleanup_scheme_objects();
    cleanup_runtime();
    
    return 0;
}

int main(int argc, char* argv[]) {
    return rscheme_main(argc, argv);
}