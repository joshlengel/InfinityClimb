#pragma once

// Operating system defines
#ifdef _WIN32
    #define IC_WINDOWS
#endif // _WIN32

#ifdef __linux__
    #define IC_LINUX
#endif // __linux__

#ifdef __APPLE__
    #define IC_APPLE
    #error Apple platform unsupported
#endif // __APPLE__

#if !defined(IC_WINDOWS) && !defined(IC_LINUX) && !defined(IC_APPLE)
    #error Platform unsupported
#endif // OS

#ifdef IC_LINUX
    #define IC_DEBUG_BREAK() asm("int $3")
#elif defined(IC_WINDOWS)
    #define IC_DEBUG_BREAK() __debugbreak()
#endif //OS

// Build type defines
#ifndef NDEBUG
    #define IC_DEBUG
#else
    #define IC_RELEASE
#endif // NDEBUG

typedef int IC_BOOL;

#define IC_TRUE 1
#define IC_FALSE 0

enum _IC_ERROR_CODE
{
    IC_NO_ERROR,

    IC_LOG_LIB_LOAD_ERROR,

    IC_WINDOW_LIB_LOAD_ERROR,
    IC_WINDOW_CREATE_ERROR,
    IC_WINDOW_GL_CONTEXT_ERROR,

    IC_SHADER_COMPILE_ERROR,
    IC_SHADER_LINK_ERROR,

    IC_READ_OPEN_FILE_ERROR,
    IC_READ_FILE_SIZE_ERROR,
    IC_READ_ERROR,
    IC_READ_CLOSE_FILE_ERROR,

    IC_LEVEL_SYNTAX_ERROR,
    IC_LEVEL_WHITESPACE_INEFFICIENCY,
    IC_LEVEL_INCOMPLETE_ERROR,

    IC_PLAYER_SYNTAX_ERROR,
    IC_PLAYER_WHITESPACE_INEFFICIENCY,
    IC_PLAYER_INCOMPLETE_ERROR,

    IC_SKYBOX_TEXTURE_LOAD_ERROR,

    IC_KEYSET_SYNTAX_ERROR,
    IC_KEYSET_WHITESPACE_INEFFICIENCY,
    IC_KEYSET_INCOMPLETE_ERROR,

    IC_OUT_OF_MEMORY_ERROR
};

typedef enum _IC_ERROR_CODE IC_ERROR_CODE;