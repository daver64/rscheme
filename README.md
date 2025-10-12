# RScheme - Scheme to C Compiler

An experimental R5RS-compatible Scheme implementation that can both interpret Scheme code directly and compile it to C for standalone executables.

## Features

- **Dual Mode Operation**: Both interpreter and compiler in one tool
- **R5RS Compatibility**: Implements core Scheme language features
- **C Code Generation**: Compiles Scheme to readable C code
- **Interactive REPL**: Read-Eval-Print Loop for development
- **Memory Management**: Reference counting with garbage collection
- **Cross-Platform**: CMake-based build system

## Building

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

# The executable will be placed in the project root
cd ..
./rscheme
```

## Usage

### Interactive REPL
```bash
./rscheme
# or explicitly
./rscheme --repl
```

### Run Scheme File
```bash
./rscheme program.scm
```

### Compile to C
```bash
# Compile to C (creates program.c)
./rscheme -c program.scm

# Compile with custom output
./rscheme -c program.scm -o output.c

# Build the generated C code
gcc -o program program.c -lm
./program
```

### Command Line Options
- `-h, --help`: Show help message
- `-v, --version`: Show version information  
- `-i, --repl`: Start interactive REPL
- `-c, --compile FILE`: Compile Scheme file to C
- `-o, --output FILE`: Specify output file for compilation
- `-O, --optimize`: Enable optimizations
- `--verbose`: Enable verbose output
- `--debug`: Enable debug mode

## Language Support

### Basic Data Types
- Numbers (floating-point)
- Booleans (`#t`, `#f`)
- Symbols
- Strings
- Lists (pairs)
- Vectors
- Procedures

### Special Forms
- `quote`, `if`, `define`, `set!`, `lambda`, `begin`
- `cond`, `and`, `or` (planned)
- `let`, `let*`, `letrec` (planned)

### Built-in Procedures
- Arithmetic: `+`, `-`, `*`, `/`
- Comparison: `=`, `<`, `>`, `<=`, `>=`
- List operations: `cons`, `car`, `cdr`, `list`
- Type predicates: `null?`, `pair?`, `number?`, etc.
- I/O: `display`, `write`, `newline`
- Logic: `not`

## Examples

### Basic Arithmetic
```scheme
(+ 1 2 3)  ; => 6
(* 4 5)    ; => 20
(- 10 3)   ; => 7
```

### Function Definition
```scheme
(define (factorial n)
  (if (= n 0)
      1
      (* n (factorial (- n 1)))))

(factorial 5)  ; => 120
```

### List Operations
```scheme
(define my-list (list 1 2 3 4))
(car my-list)          ; => 1
(cdr my-list)          ; => (2 3 4)
(cons 0 my-list)       ; => (0 1 2 3 4)
```

## Architecture

The RScheme implementation consists of several key components:

1. **Lexer** (`src/lexer.c`): Tokenizes Scheme source code
2. **Parser** (`src/parser.c`): Builds abstract syntax trees from tokens
3. **Interpreter** (`src/interpreter.c`): Evaluates expressions directly
4. **Compiler** (`src/compiler.c`): Generates C code from AST
5. **Runtime** (`src/runtime.c`): Memory management and garbage collection
6. **Environment** (`src/environment.c`): Variable binding and scoping
7. **Objects** (`src/scheme_objects.c`): Scheme value representation
8. **Built-ins** (`src/builtins.c`): Standard library procedures

## Development Status

This is an experimental implementation. Core features work, but many R5RS features are still being implemented. Contributions welcome!

## License

This project is open source. See LICENSE file for details.
