#include"Utils.h"
#include"Log.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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