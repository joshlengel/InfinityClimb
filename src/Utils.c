#include"Utils.h"
#include"Log.h"

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<assert.h>

const int MAX_BUFFER_SIZE = 256;

const char *read_source(const char *path, IC_ERROR_CODE *error_code)
{
    FILE *file = fopen(path, "rb");

    if (!file)
    {
        log_trace("Error opening source at '%s'", path);
        if (error_code) *error_code = IC_READ_OPEN_FILE_ERROR;
        return NULL;
    }

    if (fseek(file, 0L, SEEK_END) != 0)
    {
        log_trace("Error reading source file size at '%s'. Make sure the source file is no larger than 4GB.", path);
        fclose(file);
        if (error_code) *error_code = IC_READ_FILE_SIZE_ERROR;
        return NULL;
    }

    long size = ftell(file);

    if (size < 0)
    {
        log_trace("Error reading source file size at '%s'. Make sure the source file is no larger than 4GB.", path);
        fclose(file);
        if (error_code) *error_code = IC_READ_FILE_SIZE_ERROR;
        return NULL;
    }

    if (fseek(file, 0L, SEEK_SET) != 0)
    {
        log_trace("Error reading source file size at '%s'. Make sure the source file is no larger than 4GB.", path);
        fclose(file);
        if (error_code) *error_code = IC_READ_FILE_SIZE_ERROR;
        return NULL;
    }

    char *source = malloc(sizeof(char) * (size + 1));
    
    if (!fread(source, sizeof(char), size, file))
    {
        log_trace("Error reading source file contents at '%s'.", path);
        fclose(file);
        free(source);
        if (error_code) *error_code = IC_READ_ERROR;
        return NULL;
    }

    source[size] = '\0';

    if (fclose(file) != 0)
    {
        log_trace("Error closing source file at '%s'. Returning contents anyway.", path);
        if (error_code) *error_code = IC_READ_CLOSE_FILE_ERROR;
    }
    else
    {
        if (error_code) *error_code = IC_NO_ERROR;
    }
    
    return source;
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