#pragma warning(push)
#pragma warning(disable: 26451)

#include"util/Vector.h"
#include"Log.h"

#include<stdlib.h>
#include<string.h>
#include<assert.h>

IC_ERROR_CODE vector_create(Vector *dest)
{
    if (dest->init_capacity == 0)
    {
        log_trace("Warning creating vector: Initial capacity must be larger than 0. Using default capacity 10");
        dest->init_capacity = 10;
    }

    dest->capacity = dest->init_capacity;
    dest->size = 0;
    dest->arr = malloc((size_t)dest->elem_size * (size_t)dest->init_capacity);

    log_assert(dest->arr != NULL, "Error creating vector. Out of memory");
    return IC_NO_ERROR;
}

void vector_destroy(const Vector *vector)
{
    free(vector->arr);
}

void vector_add(Vector *vector, const void *elem)
{
    if (vector->size == vector->capacity)
    {
        void *temp = vector->arr;
        vector->arr = realloc(vector->arr, (size_t)vector->elem_size * (size_t)(vector->capacity *= 2)); // double capacity
        
        if (vector->arr == NULL)
        {
            free(temp);
            log_assert(IC_FALSE, "Error adding element to vector. Out of memory");
        }
    }

    memcpy((int8_t*)vector->arr + (size_t)vector->elem_size * (size_t)vector->size++, elem, vector->elem_size);
}

void *vector_add_r(Vector *vector, const void *elem)
{
    if (vector->size == vector->capacity)
    {
        void *temp = vector->arr;
        vector->arr = realloc(vector->arr, (size_t)vector->elem_size * (size_t)(vector->capacity *= 2)); // double capacity

        if (vector->arr == NULL)
        {
            free(temp);
            log_assert(IC_FALSE, "Error adding element to vector. Out of memory");
        }
    }

    void *dest = (int8_t*)vector->arr + (size_t)vector->elem_size * (size_t)vector->size++;

    memcpy(dest, elem, vector->elem_size);
    return dest;
}

void vector_put(Vector *vector, const void *elem, uint32_t index)
{
    log_assert(index <= vector->size, "Error putting element into vector. Index %u out of bounds for size %u", index, vector->size);

    if (index == vector->size)
        vector_add(vector, elem);
    else
        memcpy((int8_t*)vector->arr + (size_t)vector->elem_size * (size_t)index, elem, vector->elem_size);
}

void *vector_get(const Vector *vector, uint32_t index)
{
    log_assert(index < vector->size, "Error getting element from vector. Index %u out of bounds for size %u", index, vector->size);

    return (int8_t*)vector->arr + (size_t)vector->elem_size * (size_t)index;
}

#pragma warning(pop)