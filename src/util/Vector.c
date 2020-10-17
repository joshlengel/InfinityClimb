#include"util/Vector.h"

#include<stdlib.h>
#include<string.h>
#include<assert.h>

void vector_create(Vector *dest, uint32_t initial_capacity)
{
    assert(initial_capacity > 0);

    dest->capacity = initial_capacity;
    dest->size = 0;
    dest->arr = malloc(dest->elem_size * initial_capacity);
}

void vector_destroy(const Vector *vector)
{
    free(vector->arr);
}

void vector_add(Vector *vector, const void *elem)
{
    if (vector->size == vector->capacity)
    {
        vector->arr = realloc(vector->arr, vector->elem_size * (vector->capacity *= 2)); // double capacity
    }

    memcpy(vector->arr + vector->elem_size * vector->size++, elem, vector->elem_size);
}

void vector_put(Vector *vector, const void *elem, uint32_t index)
{
    assert(index <= vector->size);

    if (index == vector->size)
        vector_add(vector, elem);
    else
        memcpy(vector->arr + vector->elem_size * index, elem, vector->elem_size);
}

void *vector_get(const Vector *vector, uint32_t index)
{
    assert(index < vector->size);

    return vector->arr + vector->elem_size * index;
}