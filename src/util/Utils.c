#define _CRT_SECURE_NO_WARNINGS
#pragma warning(push)
#pragma warning(disable: 26451)

#include"util/Utils.h"
#include"Log.h"

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<assert.h>

#ifdef IC_WINDOWS
    #include<Windows.h>
#elif defined(IC_LINUX)
    #include<sys/time.h>
#endif // IC_WINDOWS

const int MAX_BUFFER_SIZE = 256;

String read_source(const char *path, IC_ERROR_CODE *error_code)
{
    String res;
    res.c_str = NULL;
    res.length = 0;

    FILE *file = fopen(path, "rb");

    if (!file)
    {
        log_trace("Error opening source at '%s'", path);
        if (error_code) *error_code = IC_READ_OPEN_FILE_ERROR;
        return res;
    }

    if (fseek(file, 0L, SEEK_END) != 0)
    {
        log_trace("Error reading source file size at '%s'. Make sure the source file is no larger than 4GB.", path);
        fclose(file);
        if (error_code) *error_code = IC_READ_FILE_SIZE_ERROR;
        return res;
    }

    long size = ftell(file);

    if (size < 0)
    {
        log_trace("Error reading source file size at '%s'. Make sure the source file is no larger than 4GB.", path);
        fclose(file);
        if (error_code) *error_code = IC_READ_FILE_SIZE_ERROR;
        return res;
    }

    if (fseek(file, 0L, SEEK_SET) != 0)
    {
        log_trace("Error reading source file size at '%s'. Make sure the source file is no larger than 4GB.", path);
        fclose(file);
        if (error_code) *error_code = IC_READ_FILE_SIZE_ERROR;
        return res;
    }

    res.c_str = malloc(sizeof(char) * ((size_t)size + 1));
    
    if (!fread(res.c_str, sizeof(char), size, file))
    {
        log_trace("Error reading source file contents at '%s'.", path);
        fclose(file);
        free(res.c_str);
        res.c_str = NULL;
        if (error_code) *error_code = IC_READ_ERROR;
        return res;
    }

    res.length = size;
    res.c_str[size] = '\0';

    if (fclose(file) != 0)
    {
        log_trace("Error closing source file at '%s'. Returning contents anyway.", path);
        if (error_code) *error_code = IC_READ_CLOSE_FILE_ERROR;
    }
    else
    {
        if (error_code) *error_code = IC_NO_ERROR;
    }

    return res;
}

struct _Loader_Data
{
    uint32_t num_resources;
    void **resources;
    uint32_t index;

    IC_ERROR_CODE error_code;
};

void loader_create(Loader *dest)
{
    Loader_Data *data = malloc(sizeof(Loader_Data));
    data->num_resources = dest->num_resources * 3;
    data->index = 0;
    data->error_code = IC_NO_ERROR;
    dest->data = data;

    data->resources = malloc(sizeof(void*) * data->num_resources); // Each resource has two functions associated
}

void loader_destroy(const Loader *loader)
{
    free(loader->data->resources);
    free(loader->data);
}

void loader_add_resource(const Loader *loader, void *resource, Loader_Init_proc initializer, Loader_Dest_proc destructor)
{
    // Adding more resources than specified in loader_create
    assert(loader->data->index < loader->data->num_resources);

    void **itr = loader->data->resources + loader->data->index;

    *(itr++) = resource;
    *(itr++) = initializer;
    *(itr++) = destructor;

    loader->data->index += 3;
}

void __loader_fail_impl(Loader *loader, uint32_t index, void **itr)
{
    do
    {
        index -= 3;
        void(*destructor)(void *resource) = *(--itr);
        IC_ERROR_CODE(*initializer)(void *resource) = *(--itr);
        void *resource = *(--itr);
        
        destructor(resource);

    } while (index > 0);
}

void loader_load(Loader *loader)
{
    void **itr = loader->data->resources;

    uint32_t index = 0;
    while (index < loader->data->num_resources)
    {
        void *resource = *(itr++);
        IC_ERROR_CODE(*initializer)(void *resource) = *(itr++);
        void(*destructor)(void *resource) = *(itr++);

        IC_ERROR_CODE ec = initializer(resource);

        if (ec != IC_NO_ERROR)
        {
            loader->data->error_code = ec;
            itr -= 3;
            __loader_fail_impl(loader, index, itr);
            break;
        }

        index += 3;
    }
}

void loader_unload(Loader *loader)
{
    if (loader->data->error_code != IC_NO_ERROR) return;

    void **itr = loader->data->resources + loader->data->num_resources;

    uint32_t index = 0;
    while (index < loader->data->num_resources)
    {
        void(*destructor)(void *resource) = *(--itr);
        IC_ERROR_CODE(*initializer)(void *resource) = *(--itr);
        void *resource = *(--itr);

        destructor(resource);

        index += 3;
    }
}

IC_ERROR_CODE loader_error(const Loader *loader)
{
    return loader->data->error_code;
}

IC_ERROR_CODE timer_create(Timer *dest)
{
    dest->_frame_length = (uint64_t)1000000000L / (uint64_t)dest->fps;
    return IC_NO_ERROR;
}

void timer_destroy(const Timer *timer)
{}

uint64_t __timer_get_time_impl()
{
#ifdef IC_LINUX
    struct timeval now;
    gettimeofday(&now, NULL);

    return (uint64_t)now.tv_sec * (uint64_t)1000000000L + (uint64_t)now.tv_usec * (uint64_t)1000L;
#elif defined(IC_WINDOWS)
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (uint64_t)now.QuadPart * (uint64_t)1000000000L / (uint64_t)freq.QuadPart;
#endif // IC_LINUX
    // TODO: Add more platforms
}

void timer_start(Timer *timer)
{
    timer->_time_start = __timer_get_time_impl();
}

IC_BOOL timer_should_update(Timer *timer)
{
    uint64_t now = __timer_get_time_impl();
    uint64_t diff = now - timer->_time_start;

    if (diff > timer->_frame_length)
    {
        timer->_time_start = now;
        timer->_diff = (float)((double)diff * 1E-9); // double cast first to improve conversion accuracy
        return IC_TRUE;
    }

    return IC_FALSE;
}

float timer_get_dt(const Timer *timer)
{
    return timer->_diff;
}

#pragma warning(pop)