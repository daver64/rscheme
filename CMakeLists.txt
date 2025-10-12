cmake_minimum_required(VERSION 3.10)
project(rscheme VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set output directories to place executable in root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.c
    src/lexer.c
    src/parser.c
    src/interpreter.c
    src/compiler.c
    src/scheme_objects.c
    src/environment.c
    src/builtins.c
    src/runtime.c
)

# Header files
set(HEADERS
    include/rscheme.h
    include/lexer.h
    include/parser.h
    include/interpreter.h
    include/compiler.h
    include/scheme_objects.h
    include/environment.h
    include/builtins.h
    include/runtime.h
)

# Create executable
add_executable(rscheme ${SOURCES} ${HEADERS})

# Compiler flags
if(MSVC)
    target_compile_options(rscheme PRIVATE /W4)
else()
    target_compile_options(rscheme PRIVATE -Wall -Wextra -pedantic)
endif()

# Debug configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        target_compile_options(rscheme PRIVATE /Od /Zi)
    else()
        target_compile_options(rscheme PRIVATE -g -O0)
    endif()
    target_compile_definitions(rscheme PRIVATE DEBUG=1)
endif()

# Math library (for Unix-like systems)
if(NOT WIN32)
    target_link_libraries(rscheme m)
endif()

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Print build configuration
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")