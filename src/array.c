#include "array.h"
#include <malloc.h>
#include <string.h>

void array_init(array_t * array, size_t element_size)
{
    array->size = 0;
    array->capacity = 0;
    array->element_size = element_size;
    array->storage = nullptr;
}

void array_reserve(array_t * array, size_t size)
{
    if (!size || array->capacity >= size) {
        return;
    }
    byte_t * new_storage = (byte_t*)malloc(size*array->element_size);
    if (array->capacity) {
        memcpy(new_storage, array->storage, array->size * array->element_size);
        free(array->storage);
    }
    array->capacity = size;
    array->storage = new_storage;
}

void array_push(array_t * array, void * element)
{
    if (array->size == array->capacity) {
        array_reserve(array, array->capacity?array->capacity*2:10 * array->element_size);
    }

    memcpy(array->storage + array->size*array->element_size, element, array->element_size);
    array->size ++;
}

void * array_get(array_t * array, size_t index)
{
    if (index < array->size) {
        return array->storage + index*array->element_size;
    }
    return nullptr;
}

void array_destroy(array_t * array)
{
    if (array->capacity) {
        free(array->storage);
        array_init(array,0);
    }
}

