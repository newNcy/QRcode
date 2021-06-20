#pragma once

#include "defs.h"
struct array_t
{
    size_t size;
    size_t capacity;
    size_t element_size;
    byte_t * storage;
};
typedef struct array_t array_t;

void array_init(array_t * array, size_t element_size);
void array_push(array_t * array, void * element);
void * array_get(array_t * array, size_t index);
void array_destroy(array_t * array);
