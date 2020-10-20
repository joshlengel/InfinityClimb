#pragma once

#include<stdint.h>

struct _Vector
{
    void *arr;
    uint32_t elem_size;
    uint32_t size, capacity;
};

typedef struct _Vector Vector;

void vector_create(Vector *dest, uint32_t initial_capacity);
void vector_destroy(const Vector *vector);

void vector_add(Vector *vector, const void *elem);
void *vector_add_r(Vector *vector, const void *elem);
void vector_put(Vector *vector, const void *elem, uint32_t index);
void *vector_get(const Vector *vector, uint32_t index);