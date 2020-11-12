#define _CRT_SECURE_NO_WARNINGS
#pragma warning(push)
#pragma warning(disable: 26451)

#include"util/Utils.h"
#include"util/Vector.h"
#include"Log.h"
#include"IC_Config.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifdef IC_WINDOWS
    #include<Windows.h>
#elif defined(IC_LINUX)
    #include<sys/time.h>
#endif // IC_WINDOWS

static const int MAX_BUFFER_SIZE = 256;

String read_source(const char *rel_path, IC_ERROR_CODE *error_code)
{
    String res;
    res.c_str = NULL;
    res.length = 0;

    String_View work_dir;
    String_View rel_path_sv;
    string_view_create_c_str(&work_dir, IC_WORKING_DIRECTORY, 0, UINT32_MAX);
    string_view_create_c_str(&rel_path_sv, rel_path, 0, UINT32_MAX);

    String path = string_concat_sv(&work_dir, &rel_path_sv);

    FILE *file = fopen(path.c_str, "r");
    
    if (!file)
    {
        log_trace("Error opening source at '%s'", path.c_str);
        string_destroy(&path);
        if (error_code) *error_code = IC_READ_OPEN_FILE_ERROR;
        return res;
    }

    Vector chars={.elem_size=sizeof(char), .init_capacity=1000};
    IC_ERROR_CODE ec = vector_create(&chars);
    if (ec != IC_NO_ERROR)
    {
        fclose(file);
        string_destroy(&path);
        if (error_code) *error_code = ec;
        return res;
    }

    // This is slightly inefficient but the only fully-functional cross-platform solution
    char buffer[1000];
    while (fgets(buffer, 1000, file))
    {
        uint32_t length = (uint32_t)strlen(buffer);
        vector_add_all(&chars, buffer, length);
        res.length += length;
    }

    char NULL_TERMINATOR = '\0';
    vector_add(&chars, &NULL_TERMINATOR);

    res.c_str = (char*)chars.arr;
    if (chars.arr == NULL)
    {
        string_destroy(&path);
        log_assert(IC_FALSE, "Error reading source file at '%s'. Out of memory", path.c_str);
    }
    
    if (fclose(file) != 0)
    {
        log_trace("Error closing source file at '%s'. Returning contents anyway.", path.c_str);
        string_destroy(&path);
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
    log_assert(data != NULL, "Error creating loader. Out of memory");
    data->num_resources = dest->num_resources * 3;
    data->index = 0;
    data->error_code = IC_NO_ERROR;
    dest->data = data;

    data->resources = malloc(sizeof(void*) * data->num_resources); // Each resource has two functions associated
    log_assert(data->resources, "Error creating loader. Out of memory");
}

void loader_destroy(const Loader *loader)
{
    free(loader->data->resources);
    free(loader->data);
}

void loader_add_resource(const Loader *loader, void *resource, Loader_Init_proc initializer, Loader_Dest_proc destructor)
{
    // Adding more resources than specified in loader_create
    log_assert(loader->data->index < loader->data->num_resources, "Error adding resource to loader. Resource limit reached. Create loader with higher capacity");

    void **itr = loader->data->resources + loader->data->index;

    *(itr++) = resource;
    *(itr++) = initializer;
    *(itr++) = destructor;

    loader->data->index += 3;
}

static void __loader_fail_impl(Loader *loader, uint32_t index, void **itr)
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

static uint64_t __timer_get_time_impl()
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