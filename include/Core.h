#pragma once

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

#ifndef NDEBUG
    #define IC_DEBUG
#else
    #define IC_RELEASE
#endif // NDEBUG

typedef int IC_BOOL;

#define IC_TRUE 1
#define IC_FALSE 0

typedef int IC_ERROR_CODE;

#define IC_NO_ERROR 0

#define IC_WINDOW_LIB_LOAD_ERROR 1
#define IC_WINDOW_CREATE_ERROR 2
#define IC_WINDOW_GL_CONTEXT_ERROR 3

#define IC_SHADER_COMPILE_ERROR 4
#define IC_SHADER_LINK_ERROR 5

#define IC_READ_OPEN_FILE_ERROR 6
#define IC_READ_FILE_SIZE_ERROR 7
#define IC_READ_ERROR 8
#define IC_READ_CLOSE_FILE_ERROR 9