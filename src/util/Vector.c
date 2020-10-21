#include"util/Vector.h"

#include<stdlib.h>
#include<string.h>
#include<assert.h>

void vector_create(Vector *dest, uint32_t initial_capacity)
{
    assert(initial_capacity > 0);

    dest->capacity = initial_capacity;
    dest->size = 0;
    dest->arr = malloc((size_t)dest->elem_size * (size_t)initial_capacity);
}

void vector_destroy(const Vector *vector)
{
    free(vector->arr);
}

void vector_add(Vector *vector, const void *elem)
{
    if (vector->size == vector->capacity)
    {
        vector->arr = realloc(vector->arr, (size_t)vector->elem_size * (size_t)(vector->capacity *= 2)); // double capacity
    }

    memcpy((int8_t*)vector->arr + (size_t)vector->elem_size * (size_t)vector->size++, elem, vector->elem_size);
}

void *vector_add_r(Vector *vector, const void *elem)
{
    if (vector->size == vector->capacity)
    {
        vector->arr = realloc(vector->arr, (size_t)vector->elem_size * (size_t)(vector->capacity *= 2)); // double capacity
    }

    void *dest = (int8_t*)vector->arr + (size_t)vector->elem_size * (size_t)vector->size++;

    memcpy(dest, elem, vector->elem_size);
    return dest;
}

void vector_put(Vector *vector, const void *elem, uint32_t index)
{
    assert(index <= vector->size);

    if (index == vector->size)
        vector_add(vector, elem);
    else
        memcpy((int8_t*)vector->arr + (size_t)vector->elem_size * (size_t)index, elem, vector->elem_size);
}

void *vector_get(const Vector *vector, uint32_t index)
{
    assert(index < vector->size);

    return (int8_t*)vector->arr + (size_t)vector->elem_size * (size_t)index;
}