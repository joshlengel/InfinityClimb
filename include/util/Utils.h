#pragma once

#include"Core.h"
#include"util/String.h"

#include<stdint.h>

#define IC_PI 3.14159265358979323846

// Read source
String read_source(const char *path, IC_ERROR_CODE *error_code);

// Loader
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

// Timer
struct _Timer
{
    uint16_t fps;

    uint64_t _time_start;
    uint64_t _frame_length;
    float _diff;
};

typedef struct _Timer Timer;

IC_ERROR_CODE timer_create(Timer *dest);
void timer_destroy(const Timer *timer);

void timer_start(Timer *timer);
IC_BOOL timer_should_update(Timer *timer);
float timer_get_dt(const Timer *timer);