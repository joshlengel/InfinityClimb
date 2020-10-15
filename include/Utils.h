#pragma once

#include"Core.h"

#include<stdint.h>

const char *read_source(const char *path, IC_ERROR_CODE *error_code);

typedef struct _Loader_Data Loader_Data;

struct _Loader
{
    uint32_t num_resources;

    Loader_Data *data;
};

typedef struct _Loader Loader;

typedef IC_ERROR_CODE(*Loader_Init_proc)(void *resource);
typedef void(*Loader_Dest_proc)(const void *resource);

void loader_create(Loader *dest);
void loader_destroy(const Loader *loader);
void loader_add_resource(const Loader *loader, void *resource, Loader_Init_proc initializer, Loader_Dest_proc destructor);
void loader_load(Loader *loader);
void loader_unload(Loader *loader);
IC_ERROR_CODE loader_error(const Loader *loader);