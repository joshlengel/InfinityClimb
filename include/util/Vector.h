#pragma once

#include"Core.h"

#include<stdint.h>

struct _Vector
{
    void *arr;
    uint32_t elem_size;
    uint32_t size, capacity;
    uint32_t init_capacity;
};

typedef struct _Vector Vector;

IC_ERROR_CODE vector_create(Vector *dest);
void vector_destroy(const Vector *vector);

void vector_add(Vector *vector, const void *elem);
void *vector_add_r(Vector *vector, const void *elem);
void vector_put(Vector *vector, const void *elem, uint32_t index);
void *vector_get(const Vector *vector, uint32_t index);